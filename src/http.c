#ifdef __CYGWIN__

#define __INSIDE_HTTP
#include "global.h"
#include "http.h"
#  include <assert.h>
#  include <stdio.h>
#  include <stdlib.h>
#  include <string.h>
#  include <signal.h>
#  include <time.h>
#  include "myprintf.h"
#else

#  include <unistd.h>
#  include <fcntl.h>
#  include <sys/types.h>
#  include <sys/socket.h>
#  include <sys/time.h>
#  include <sys/stat.h>
/*#  include <sys/poll.h>*/ /* TODO: se souvenir des raisons de la présence de ce #include (genant pour darwin, inutile sous linux..) */
#  include <netinet/in.h>
#  include <arpa/inet.h>
#  include <netdb.h>
#  include <stdarg.h>
#  include <assert.h>
#  include <stdlib.h>
#  include <stdio.h>
#  include <string.h>
#  include <errno.h>
#  include "myprintf.h"
#  include "coincoin.h"
#  include "http.h"

#endif /* ifdef __CYGWIN */


#ifdef __CYGWIN__
#  define LASTERR_EINTR (WSAGetLastError() == WSAEINTR)
#  define LASTERR_EAGAIN (WSAGetLastError() == WSAEINPROGRESS)
#  define SETERR_TIMEOUT
#  define STR_LAST_ERROR strerror(WSAGetLastError())
#else
#  define LASTERR_EINTR (errno==EINTR)
#  define SETERR_TIMEOUT errno=ETIMEDOUT
#  define STR_LAST_ERROR strerror(errno)
#  define LASTERR_EAGAIN (errno==EAGAIN)
#endif


#define HTTP_ERR_MSG_SZ 512
#define HTTP_LAST_ERR_URL_SZ 128

static char http_last_err_msg[HTTP_ERR_MSG_SZ] = "";
static char http_last_err_url[HTTP_LAST_ERR_URL_SZ] = "";
time_t http_err_time = 0;
static char http_last_url[HTTP_LAST_ERR_URL_SZ] = "";
static char http_used_ip[20] = "xxx.xxx.xxx.xxx";

static char base64[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";


static SOCKET http_connect(const char *host_name, int port);

char*
http_complete_error_info()
{
  char s[2048];
  char s_err[1024];
  char s_heure[80];

  if (flag_http_transfert == 0) {
    if (http_err_time == 0) {
      snprintf(s_heure, 80, "<i>il n'y a pas encore eu d'erreurs http</i>");
    } else {
      struct tm *t;
      t = localtime(&http_err_time);
      
      snprintf(s_heure, 80, "dernière erreur à: <b>%02d:%02d:%02d</b>",
	       t->tm_hour, t->tm_min, t->tm_sec);
    }

    if (flag_http_error) {
      snprintf(s_err, 1024, "erreur: <b><font color=#800000>%s</font></b><br>", http_last_err_msg);
    } else {
      if (http_err_time) {
	snprintf(s_err, 1024, "<br>la dernière erreur a été: %s<br>pour l'url:<tt>%s</tt><br>",
		 http_last_err_msg, http_last_err_url);
      } else {
	s_err[0] = 0;
      }
    }
    snprintf(s, 2048, "%s<br>%s<br>%s: <tt>%s</tt><br>ip de l'hôte: <font color=#0000ff>%s</font><br>%s",
	     flag_http_error ? "<b>il vient d'y avoir une erreur !!</b>" : "le dernier transfert s'est bien passé",
	     s_heure, flag_http_error ? "url fautive" : "on vient de récuperer", http_last_url, http_used_ip, s_err);
  } else {
    snprintf(s, 2048, "un transfert est en cours...<br>url: <tt>%s</tt><br>ip: <font color=#0000ff>%s</font><br>", http_last_url, http_used_ip);
  }
  return strdup(s);
}

static void
set_http_err()
{
  strcpy(http_last_err_url, http_last_url);
  time(&http_err_time);
}


int base64_encode(const void *data, int size, char **str)
{
  char *s, *p;
  int i;
  int c;
  const unsigned char *q;

  p = s = (char*)malloc(size*4/3+4);
  if (p == NULL)
      return -1;
  q = (const unsigned char*)data;
  i=0;
  for(i = 0; i < size;){
    c=q[i++];
    c*=256;
    if(i < size)
      c+=q[i];
    i++;
    c*=256;
    if(i < size)
      c+=q[i];
    i++;
    p[0]=base64[(c&0x00fc0000) >> 18];
    p[1]=base64[(c&0x0003f000) >> 12];
    p[2]=base64[(c&0x00000fc0) >> 6];
    p[3]=base64[(c&0x0000003f) >> 0];
    if(i > size)
      p[3]='=';
    if(i > size+1)
      p[2]='=';
    p+=4;
  }
  *p=0;
  *str = s;
  return strlen(s);
}

/* 
   et hop ! les 3 fonctions suivantes ont ete piquees dans wget 1.6 
   (dont le code source est fort joli, soit dit en passant :)

   gpl roulaize :)
*/

#ifdef HAVE_SELECT
/* Wait for file descriptor FD to be readable, MAXTIME being the
   timeout in seconds.  If WRITEP is non-zero, checks for FD being
   writable instead.

   Returns 1 if FD is accessible, 0 for timeout and -1 for error in
   select().  */
static int
http_select_fd (int fd, int maxtime_sec, int writep)
{
  fd_set fds, exceptfds;
  struct timeval timeout;
  int retval;

  FD_ZERO (&fds);
  FD_SET (fd, &fds);
  FD_ZERO (&exceptfds);
  FD_SET (fd, &exceptfds);
  timeout.tv_sec = maxtime_sec;
  timeout.tv_usec = 0;
  /* HPUX reportedly warns here.  What is the correct incantation?  */
  retval = select (fd + 1, writep ? NULL : &fds, writep ? &fds : NULL,
		 &exceptfds, &timeout);
  ALLOW_X_LOOP_MSG("http_select_fd"); ALLOW_ISPELL;
  return retval;
}
#endif /* HAVE_SELECT */


/* Read at most LEN bytes from FD, storing them to BUF.  This is
   virtually the same as read(), but takes care of EINTR braindamage
   and uses select() to timeout the stale connections (a connection is
   stale if more than OPT.TIMEOUT time is spent in select() or
   read()).  */
int
http_iread (SOCKET fd, char *buf, int len)
{
  int res;

  flag_http_transfert++;

  do
    {
#ifdef HAVE_SELECT
      if (Prefs.http_timeout)
	{
	  int tic0;
	  tic0 = wmcc_tic_cnt;
	  do
	    {
	      res = http_select_fd (fd, Prefs.http_timeout, 0);	
	      ALLOW_X_LOOP; ALLOW_ISPELL; 
	      if ((wmcc_tic_cnt - tic0) > Prefs.http_timeout*(1000/WMCC_TIMER_DELAY_MS)) {
		SETERR_TIMEOUT;
		printf("timeout (t=%d millisecondes)..\n", (wmcc_tic_cnt - tic0)*WMCC_TIMER_DELAY_MS);
	      }
	    }
	  while (res == SOCKET_ERROR && LASTERR_EINTR);
#ifndef __CYGWIN__
	  if (res <= 0) {
	    /* Set errno to ETIMEDOUT on timeout.  */
	    if (res == 0)
	      /* #### Potentially evil!  */
	      SETERR_TIMEOUT;
	    goto error;
	  }
#else
	  if (res == SOCKET_ERROR)
            goto error;
	  if (res == 0) {
	    printf ("http_iread: Timeout...\n");
	    goto error;
	  }
#endif
	}
#endif

#ifdef __CYGWIN__      	  
      res = recv (fd, buf, len, 0);
#else
      res = read(fd, buf, len); 
#endif
      ALLOW_X_LOOP; ALLOW_ISPELL; 
    }
  while (res == SOCKET_ERROR && LASTERR_EINTR);

  flag_http_error = 0;
  flag_http_transfert--;

  ALLOW_X_LOOP; ALLOW_ISPELL;
  return res;

 error:
  flag_http_error = 1;
  flag_http_transfert--;
  ALLOW_X_LOOP; ALLOW_ISPELL;
  return SOCKET_ERROR;
}

/* Write LEN bytes from BUF to FD.  This is similar to iread(), but
   doesn't bother with select().  Unlike iread(), it makes sure that
   all of BUF is actually written to FD, so callers needn't bother
   with checking that the return value equals to LEN.  Instead, you
   should simply check for -1.  */
int
http_iwrite (SOCKET fd, char *buf, int len)
{
  int res = 0;

  flag_http_transfert++;

  /* `write' may write less than LEN bytes, thus the outward loop
     keeps trying it until all was written, or an error occurred.  The
     inner loop is reserved for the usual WSAEINTR f*kage, and the
     innermost loop deals with the same during select().  */
  while (len > 0) {
    do {
#ifdef HAVE_SELECT
      if (Prefs.http_timeout) {
	do {
	  res = http_select_fd (fd, Prefs.http_timeout, 1);
	  ALLOW_X_LOOP; ALLOW_ISPELL;
	} while (res == SOCKET_ERROR && LASTERR_EINTR);
#ifndef __CYGWIN__
	if (res <= 0) {
	  /* Set errno to ETIMEDOUT on timeout.  */
	  if (res == 0)
	    /* #### Potentially evil!  */
	    SETERR_TIMEOUT;
	  goto error;
	}
#else
	if (res == SOCKET_ERROR)
	  goto error;
	if (res == 0) {
	  printf ("http_iwrite:Timeout...\n");
	  goto error;
	}
#endif /* ifndef __CYGWIN__ */
      }
#endif

#ifdef __CYGWIN__
      res = send (fd, buf, len, 0);
#else
      res = write(fd, buf, len);
#endif
    } while (res == SOCKET_ERROR && LASTERR_EINTR);
    if (res == SOCKET_ERROR)
      break;
    buf += res;
    len -= res;
  }
  flag_http_error = 0;
  flag_http_transfert--;
  return res;

 error:
  flag_http_error = 1;
  flag_http_transfert--;
  return SOCKET_ERROR;
}

/* c'est fini pour les fonctions de wget */

char *
http_error() {
  return http_last_err_msg;
}


#ifdef __CYGWIN__
#include "http_win.c"
#else
#include "http_unix.c"
#endif

/* -1 => erreur */
static SOCKET
http_connect(const char *host_name, int port)
{
  static struct hostent *host = NULL;
  SOCKET sockfd = INVALID_SOCKET;
  struct sockaddr_in dest_addr;   /* Contiendra l'adresse de destination */
  int num_try;

   /* 
     un peu tordu : 
     on ne fait qu'un gethostbyname au debut et on sauve son addresse (economie de 0.25s en moyenne)
     ensuite, on rappelle la structure sauvee. en cas de probleme de connect, on retente un gethostbyname
     (des fois que l'ip aurait change...)

     BIEN SUR CA NE MARCHE QUE SI L'ON S'ADDRESSE TOUJOURS AU MEME SITE !
  */
  for (num_try = 0; num_try < 2; num_try++) 
    {
      if (num_try == 1 || host == NULL)
	{
	  /*
	  int t0,t1;
	  myprintf("%<CYA gethostname>: "); fflush(stdout);
	  t0 = times(NULL);*/
	  BLAHBLAH(1, printf("gethostbyname('%s') -> si le reseau rame, on risque de bloquer le coincoin ici\n", host_name));
	  ALLOW_X_LOOP_MSG("gethostbyname(1)"); ALLOW_ISPELL;
	  host = gethostbyname(host_name); /* rahhh comme c'est lent :-( */
	  ALLOW_X_LOOP_MSG("gethostbyname(2)"); ALLOW_ISPELL;
	  if (host) {
	    snprintf(http_used_ip, 20, "%u.%u.%u.%u", 
		     (unsigned char)host->h_addr_list[0][0],
		     (unsigned char)host->h_addr_list[0][1],
		     (unsigned char)host->h_addr_list[0][2],
		     (unsigned char)host->h_addr_list[0][3]);
	    BLAHBLAH(1, myprintf("--> host='%<YEL %s>', ip=%<MAG %s>\n", host->h_name, http_used_ip));
	  } else {
	    snprintf(http_used_ip, 20, "???.???.???.???");
	  }
	  /*	  t1 = times(NULL);
		  printf(" %f millisecondes\n", (t1-t0)*10.0);*/
	}
      if (host == NULL) {
	if (num_try == 0) continue;
	else {
	  set_http_err();
	  snprintf(http_last_err_msg, HTTP_ERR_MSG_SZ, "Impossible de resoudre le nom '%s'", host_name);
	  return INVALID_SOCKET;
	}
      }

      sockfd = socket(AF_INET, SOCK_STREAM, 0); /* Vérification d'erreurs! */
      ALLOW_X_LOOP; ALLOW_ISPELL;
      if (sockfd == INVALID_SOCKET) {
	set_http_err();
	snprintf(http_last_err_msg, HTTP_ERR_MSG_SZ, "Impossible de creer un socket ! (%s)", STR_LAST_ERROR);
	return INVALID_SOCKET;
      }
  
      dest_addr.sin_family = AF_INET;       
      dest_addr.sin_port = htons(port);

      /* pris dans wget, donc ca doit etre du robuste */
      memcpy (&dest_addr.sin_addr, host->h_addr_list[0], host->h_length);

      memset(&(dest_addr.sin_zero), 0, 8);
  
      /* y'a le probleme des timeout de connect ...
	 d'ailleurs je n'ai toujours pas compris pourquoi tous les
	 sigalrm balance par l'itimer de wmcoincoin n'interferent
	 pas avec le connect...
      */
      BLAHBLAH(4, printf("connecting..\n"));
      //      if (connect(sockfd, (struct sockaddr *)&dest_addr, sizeof(dest_addr))) {

#ifdef CONNECT_WITHOUT_TIMEOUT // a definir pour les os chiants
      if (net_tcp_connect(sockfd, &dest_addr))
#else
      if (net_tcp_connect_with_timeout(sockfd, &dest_addr, Prefs.http_timeout))
#endif
	{
	  set_http_err();
	  snprintf(http_last_err_msg, HTTP_ERR_MSG_SZ, "connect(): %s", STR_LAST_ERROR);
	  http_close(sockfd);
	  ALLOW_X_LOOP; ALLOW_ISPELL;
	  BLAHBLAH(4, printf("connection failed: %s..\n", http_last_err_msg));
	  host = NULL;
	  if (num_try == 1) {
	    return INVALID_SOCKET;
	}
      } else {
	BLAHBLAH(4, printf("connected..\n"));
	break;
      }
    }
  return sockfd;
}


/*
  analyse tres rapidement la reponse du serveur

  si il renvoie un vrai header, avec un 200 OK ou 302 Found, on renvoie 0
  si il renvoie autre chose (404 etc..) on renvoie -1
  si il y a une connexion timeout, on renvoie -2
*/
int
http_skip_header(SOCKET fd, char *last_modified, int last_modified_sz)
{
  char buff[512];
  int i, got, lnum;
  char last;
  
  lnum = 0;
  i = 0;
  //  printf("http_skip header\n");
  buff[511] = 0;
  last = 0;
  
  do {
    while((got = http_iread(fd, buff+i, 1)) > 0) {
      buff[i+1] = 0;
      BLAHBLAH(2, myprintf("%<GRN %c>", buff[i]););
      if(buff[i] == '\n' && (last == '\n'))
	break;
      if(buff[i] == '\r')
	continue;
      last = buff[i];
      if (buff[i] == '\n') {
	if (lnum == 0) {
	  int j;
	  j = 0;
	  while (buff[j] != ' ' && buff[j]) j++;
	  if (buff[j] == ' ') {
	    if (strcasecmp("200 OK\n", buff+j+1) != 0 && 
		strcasecmp("302 Found\n", buff+j+1) != 0 &&
		strcasecmp("304 Not Modified\n", buff+j+1) != 0 &&
		strcasecmp("302 Moved Temporarily\n", buff+j+1) != 0) {
	      set_http_err();
	      snprintf(http_last_err_msg, HTTP_ERR_MSG_SZ, "%s",buff+j+1); 
	      printf("http_skip_header[%s]: erreur detectee: '%s'", http_last_url, buff+j+1);
	      return -1;
	    }
	  }
	} else if (last_modified) {
	  if (strncmp(buff,"Last-Modified: ",15) == 0) {
	    strncpy(last_modified, buff + 15, last_modified_sz); /* oula pas tres joli le strncpy on connait pas la taille de last_modified.. */
	    last_modified[last_modified_sz-1] = 0;
	  }
	}
	lnum++;
	i=0;
      } else {
	i++; if (i >= 511) i = 510;
      }
    }
  } while (got==SOCKET_ERROR && LASTERR_EAGAIN); 
  if (got == SOCKET_ERROR) {
    set_http_err();
    snprintf(http_last_err_msg, HTTP_ERR_MSG_SZ, "http_skip_header a un problème de chaussette !(%s)", STR_LAST_ERROR);
    return -2;
  }
  return 0;
  //  return (got <= 0 ? -1 : 0);
}

int
http_get_line(char *s, int sz, SOCKET fd)
{
  int i, got,cnt;


  flag_http_transfert++;

  i = 0;
  cnt = 0;
  s[0] = 0;

  do {
    while ((got = http_iread(fd, s+i, 1)) > 0) {
      cnt++;
      if (s[i] == '\n' || s[i] == 0) {
	s[i] = 0; break;
      }
      if ((unsigned char)s[i] >= (unsigned char)' ') {
	i++;
	if (i >= sz) i = sz-1; /* pas cool */
      }
      s[i] = 0;
    }
  } while (got == SOCKET_ERROR && LASTERR_EAGAIN);

  if (got == SOCKET_ERROR) {
    set_http_err();
    snprintf(http_last_err_msg, HTTP_ERR_MSG_SZ, "http_get_line s'est vautré: %s!", STR_LAST_ERROR);
    printf("[%s] %s\n", http_last_url, http_last_err_msg);
    goto error;
  }
  BLAHBLAH(4,myprintf("http_get_line renvoie (cnt=%d): '%<yel %s>'\n", cnt, s));
  flag_http_transfert--;

  flag_http_error = 0;
  return cnt;

 error:
  flag_http_error = 1;
  flag_http_transfert--;
  return SOCKET_ERROR;
}

#define CRLF "\015\012"

/*
  renvoie le descripteur de fichier vers les donnees renvoyees ,
  descripteur a fermer par un close(d)
*/
SOCKET
http_get_with_cookie(const char *host_name, int host_port, const char *host_path, 
		     const char *proxy, const char *userpass, int proxy_port, 
		     const char *user_agent, const char *cookie, 
		     char *last_modified, int last_modified_sz)
{
  SOCKET sockfd;
#define BSIZE 1024
  char buff[BSIZE];
  char cookie_s[512];
  const char *pnocache;
  char last_modified_s[512];

  flag_http_transfert++;

  snprintf(http_last_url, HTTP_LAST_ERR_URL_SZ, "%s:%d%s", host_name, host_port, host_path);
  //  sprintf(http_last_err_msg, "aucune erreur http");

  cookie_s[0] = 0;
  if (cookie) {
    snprintf(cookie_s,512,"Cookie: %s" CRLF, cookie);
  }

  last_modified_s[0] = 0;
  if (last_modified && last_modified[0] && Prefs.use_if_modified_since) {
    int l;
    l = strlen(last_modified); l--;
    while (l>=0 && (unsigned char)last_modified[l] < ' ') last_modified[l--]=0;
    snprintf(last_modified_s,512,"If-Modified-Since: %s" CRLF, last_modified);
  }

  pnocache = Prefs.proxy_nocache ? "Pragma: no-cache" CRLF "Cache-Control: no cache" CRLF : "";
  
  if (proxy) {
    sockfd = http_connect(proxy, proxy_port);
  } else {
    sockfd = http_connect(host_name, host_port);
  }
  if (sockfd == INVALID_SOCKET) goto error;

  if (userpass == NULL) {
    if (proxy == NULL) {
      snprintf(buff, BSIZE, "GET %s HTTP/1.1" CRLF
	       "Host: %s" CRLF
	       "%s"
	       "User-Agent: %s" CRLF
	       "%s%s"
	       "Accept: */*" CRLF CRLF,
	       host_path, host_name, cookie_s, user_agent, last_modified_s, pnocache);
    } else {
      snprintf(buff, BSIZE, "GET http://%s%s HTTP/1.1" CRLF
	       "Host: %s" CRLF
	       "%s"
	       "User-Agent: %s" CRLF
	       "%s%s"
	       "Accept: */*" CRLF CRLF,
	       host_name, host_path, host_name, cookie_s, user_agent, last_modified_s, pnocache);
    }
  } else {
    char *auth;
    base64_encode(userpass, strlen(userpass), &auth);
    
    /* on ne gère que le schema d'authentification basique [base64(USER:PASS)]
       wget 1.6 fait mieux, mais pas curl 7.6 donc ca ira...
     */
    snprintf(buff, BSIZE, "GET http://%s:%d%s HTTP/1.1" CRLF
	     "Host: %s:%d" CRLF
	     "%s"
	     "User-Agent: %s" CRLF
	     "%s"
	     "Proxy-Authorization: Basic %s" CRLF
	     "%s"
	     "Accept: */*" CRLF CRLF,
	     host_name, host_port, host_path, host_name, host_port, cookie_s,user_agent, last_modified_s, auth, pnocache);
    free(auth);
  }
  BLAHBLAH(2,printf("sending:\n%s", buff));
  if (http_iwrite(sockfd, buff, strlen(buff)) == SOCKET_ERROR) {
    set_http_err();
    snprintf(http_last_err_msg, HTTP_ERR_MSG_SZ, "http_get n'a pas pu envoyer sa requête: %s", STR_LAST_ERROR);
    goto error;
  }

  //  write(sockfd, buff, strlen(buff));
  BLAHBLAH(2,printf("ok, sent\n"));
  
  if (http_skip_header(sockfd, last_modified, last_modified_sz) != 0) {
    http_close(sockfd);
    goto error;
  }

  flag_http_error = 0;
  flag_http_transfert--;
  return sockfd;

 error:
  flag_http_error = 1;
  flag_http_transfert--;
  return INVALID_SOCKET;
}

SOCKET
http_get(const char *host_name, int host_port, const char *host_path, 
	 const char *proxy, const char *userpass, int proxy_port, 
	 const char *user_agent, char *last_modified, int last_modified_sz)
{
  return http_get_with_cookie(host_name, host_port, host_path, proxy, userpass, proxy_port, user_agent, NULL, last_modified, last_modified_sz);
}

/* 
   renvoie une chaine mallocee contenant l'urlencodage de string 
   encore un fonction piquee dans curl
 */
char *
http_url_encode(char *string)
{
   int alloc=strlen(string)+1;
   char *ns = malloc(alloc);
   unsigned char in;
   int newlen = alloc;
   int index=0;

   while(*string) {
      in = *string;
      if(' ' == in)
	 ns[index++] = '+';
      else if(!(in >= 'a' && in <= 'z') &&
	      !(in >= 'A' && in <= 'Z') &&
	      !(in >= '0' && in <= '9')) {
	 /* encode it */
	 newlen += 2; /* the size grows with two, since this'll become a %XX */
	 if(newlen > alloc) {
	    alloc *= 2;
	    ns = realloc(ns, alloc);
	    if(!ns)
	       return NULL;
	 }
	 sprintf(&ns[index], "%%%02X", in);
	 index+=3;
      }
      else {
	 /* just copy this */
	 ns[index++]=in;
      }
      string++;
   }
   ns[index]=0; /* terminate it */
   return ns;
}

SOCKET 
http_post_with_cookie(const char *host_name, int host_port, const char *host_path, 
		      const char *proxy, const char *userpass, int proxy_port, 
		      const char *user_agent, const unsigned char *referer, const char *cookie, 
		      const unsigned char *post)
{
  char buff[BSIZE];
  SOCKET sockfd;
  char *auth;
  char cookie_s[512];

  flag_http_transfert++;

  snprintf(http_last_url, HTTP_LAST_ERR_URL_SZ, "%s:%d%s", host_name, host_port, host_path);

  //  sprintf(http_last_err_msg, "aucune erreur http");

  cookie_s[0] = 0;
  if (cookie) {
    snprintf(cookie_s,512,"Cookie: %s" CRLF, cookie);
  }
  if (proxy) {
    sockfd = http_connect(proxy, proxy_port);
  } else {
    sockfd = http_connect(host_name, host_port);
  }
  if (sockfd == INVALID_SOCKET) goto error;


  if (userpass) {
    base64_encode(userpass, strlen(userpass), &auth);
  }

  if (proxy == NULL) {
    snprintf(buff, BSIZE, 
	     "POST %s HTTP/1.1" CRLF, host_path);
  } else {
    snprintf(buff, BSIZE, 
	     "POST http://%s:%d%s HTTP/1.1" CRLF, host_name, host_port, host_path);
  }
  snprintf(buff+strlen(buff), BSIZE-strlen(buff),
	   "Host: %s:%d" CRLF, host_name, host_port);
  snprintf(buff+strlen(buff), BSIZE-strlen(buff),
	   "User-Agent: %s" CRLF
	   "Referer: %s" CRLF "%s", user_agent, referer, cookie_s);
  if (userpass) {
    snprintf(buff+strlen(buff), BSIZE-strlen(buff),
	     "Proxy-Authorization: Basic %s" CRLF, auth);
  }

  snprintf(buff+strlen(buff), BSIZE-strlen(buff),
	   "Content-Type: application/x-www-form-urlencoded" CRLF
	   "Content-Length: %d" CRLF CRLF "%s", (int)strlen(post),post);
  
  if (userpass) {
    free(auth);
  }
  BLAHBLAH(2,printf("POSTING:\n%s", buff));
  if (http_iwrite(sockfd, buff, strlen(buff)) == -1) {
    set_http_err();
    snprintf(http_last_err_msg, HTTP_ERR_MSG_SZ, "http_post n'a pas pu envoyer sa requête: '%s'", STR_LAST_ERROR);
    goto error;
  }

  flag_http_error = 0;
  flag_http_transfert--;
  return sockfd;

 error:
  flag_http_error = 1;
  flag_http_transfert--;
  return INVALID_SOCKET;
}
