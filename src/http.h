/*
  rcsid=$Id: http.h,v 1.4 2002/02/02 23:49:17 pouaite Exp $
  ChangeLog:
  $Log: http.h,v $
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
#else // WIN32 Detected
# ifdef __INSIDE_HTTP
#   define __USE_W32_SOCKETS
#   include "windows.h"
# else // !__INSIDE_HTTP
#   define SOCKET_ERROR -1
#   define INVALID_SOCKET 0
#   define SOCKET unsigned int
# endif
#endif

void http_init();
char *http_error();
char *http_complete_error_info(); /* renvoie une chaine alloué, ATTENTION */

int http_iread(SOCKET fd, char *buf, int len);
int http_iwrite(SOCKET fd, char *buf, int len);
int http_get_line(char *s, int sz, SOCKET fd);
char *http_url_encode(char *string);
SOCKET http_get(const char *host_name, int host_port, const char *host_path, 
		const char *proxy, const char *userpass, int proxy_port, 
		const char *user_agent, char *last_modified, int last_modified_sz);
SOCKET http_get_with_cookie(const char *host_name, int host_port, const char *host_path, 
			    const char *proxy, const char *userpass, int proxy_port, 
			    const char *user_agent, const char *cookie, 
			    char *last_modified, int last_modified_sz);
SOCKET http_post_with_cookie(const char *host_name, int host_port, const char *host_path, 
			     const char *proxy, const char *userpass, int proxy_port, 
			     const char *user_agent, const unsigned char *referer, 
			     const char *cookie, const unsigned char *post);
int http_skip_header(SOCKET fd, char *last_modified, int last_modified_sz);
int http_close (SOCKET fd);

#endif
