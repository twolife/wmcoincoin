/*
  rcsid=$Id: http.h,v 1.15 2002/12/20 15:49:51 pouaite Exp $
  ChangeLog:
  $Log: http.h,v $
  Revision 1.15  2002/12/20 15:49:51  pouaite
  prout 2.4.2b ?

  Revision 1.14  2002/09/08 18:21:26  pouaite
  microfix pou cygwin + compil

  Revision 1.13  2002/09/05 23:11:57  pouaite
  <blog>ce soir g mangé une omelette</blog>

  Revision 1.12  2002/08/29 00:15:53  pouaite
  cosmétique et capillotraction

  Revision 1.11  2002/08/17 18:33:39  pouaite
  grosse commition

  Revision 1.10  2002/06/23 14:01:36  pouaite
  ouups, j'avais flingué les modifs depuis la v2.3.8b

  Revision 1.9  2002/06/23 10:44:05  pouaite
  i18n-isation of the coincoin(kwakkwak), thanks to the incredible jjb !

  Revision 1.7  2002/05/12 22:06:27  pouaite
  grosses modifs dans http.c

  Revision 1.6  2002/04/13 11:55:19  pouaite
  fix kde3 + deux trois conneries

  Revision 1.5  2002/03/07 18:54:34  pouaite
  raaa .. fix login_color (jjb) patch plop_words (estian) et bidouille pour le chunk encoding (a tester)

  Revision 1.4  2002/02/02 23:49:17  pouaite
  plop

  Revision 1.3  2002/01/10 09:03:06  pouaite
  integration du patch de glandium (requetes http/1.1 avec header 'If-Modified-Since' --> coincoin plus gentil avec dacode)

  Revision 1.2  2001/12/02 18:34:54  pouaite
  ajout de tags cvs Id et Log un peu partout...

*/

#ifndef HTTP_H
#define HTTP_H

#ifndef HAVE_WINDOWS_H
# define SOCKET_ERROR -1
# define INVALID_SOCKET -1
# define SOCKET int
#else /* WIN32 Detected */
//# ifdef __INSIDE_HTTP
//#   include "windows.h"
//# else /* !__INSIDE_HTTP */
#  ifndef SOCKET_ERROR
#   define SOCKET_ERROR -1
#  endif
#  ifndef INVALID_SOCKET
#   define INVALID_SOCKET 0
#  endif
#  ifndef SOCKET
#   define SOCKET unsigned int
#  endif
//# endif
#endif
#include "global.h"


typedef struct {
  /* input members */
  enum { HTTP_GET, HTTP_POST} type;
  char *host;
  int port;
  char *host_path;

  char *proxy_name;
  char *proxy_user_pass;
  int         proxy_port;

  int         pragma_nocache;
  int use_if_modified_since;

  char *user_agent;
  char *referer;
  char *cookie;

  char **p_last_modified; /* is modified by http_skip_header */

  /* output members */

  int        is_chunk_encoded;
  int        chunk_num;
  long       chunk_pos, chunk_size;
  SOCKET     fd;

  int        content_length;
  int        error;

  int        response; /* 404, 200, 502 etc.. */

  char * post;
} HttpRequest;



void http_init();
char *http_error();
char *http_complete_error_info(); /* renvoie une chaine alloué, ATTENTION */


void http_request_init(HttpRequest *r);
void http_request_send(HttpRequest *r);
void http_request_close(HttpRequest *r);
int  http_read(HttpRequest *r, char *buff, int max_len);
int http_get_line(HttpRequest *r, char *s, int sz);
int http_get_line_trim(HttpRequest *r, char *s, int sz);
char *http_url_encode(const char *string);
unsigned char *http_read_all(HttpRequest *r, char *what);
#endif
