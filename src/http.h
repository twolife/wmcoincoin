/*
  rcsid=$Id: http.h,v 1.9 2002/06/23 10:44:05 pouaite Exp $
  ChangeLog:
  $Log: http.h,v $
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

#include "global.h"

#ifndef HAVE_WINDOWS_H
# define SOCKET_ERROR -1
# define INVALID_SOCKET -1
# define SOCKET int
#else /* WIN32 Detected */
# ifdef __INSIDE_HTTP
#   define __USE_W32_SOCKETS
#   include "windows.h"
# else /* !__INSIDE_HTTP */
#   define SOCKET_ERROR -1
#   define INVALID_SOCKET 0
#   define SOCKET unsigned int
# endif
#endif

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

  char * post;
} HttpRequest;

void http_init();
char *http_error();
char *http_complete_error_info(); /* renvoie une chaine allou�, ATTENTION */


void http_request_init(HttpRequest *r);
void http_request_send(HttpRequest *r);
void http_request_close(HttpRequest *r);
int  http_read(HttpRequest *r, char *buff, int max_len);
int http_get_line(HttpRequest *r, char *s, int sz);
char *http_url_encode(char *string);

//int http_iread(SOCKET fd, char *buf, int len);
//int http_iwrite(SOCKET fd, char *buf, int len);

//void http_get(HttpRequest *r);
//SOCKET http_post_with_cookie(const char *host_name, int host_port, const char *host_path, 
//			     const char *proxy, const char *userpass, int proxy_port, 
//			     const char *user_agent, const unsigned char *referer, 
//			     const char *cookie, const unsigned char *post);
//int http_skip_header(SOCKET fd, char *last_modified, int last_modified_sz, int *chunk_encoding);

#endif
