/*
  rcsid=$Id: http_unix.c,v 1.7 2002/02/26 09:18:23 pouaite Exp $
  ChangeLog:
  $Log: http_unix.c,v $
  Revision 1.7  2002/02/26 09:18:23  pouaite
  bugfixes divers

  Revision 1.6  2002/02/02 23:49:17  pouaite
  plop

  Revision 1.5  2002/01/20 20:53:22  pouaite
  bugfix configure.in && http_win.c pour cygwin + 2-3 petis trucs

  Revision 1.4  2002/01/20 00:37:06  pouaite
  bugfix qui permet d'utiliser l'option 'http.proxy_use_nocache:' sur les horribles proxy transparents

  Revision 1.3  2002/01/10 09:03:06  pouaite
  integration du patch de glandium (requetes http/1.1 avec header 'If-Modified-Since' --> coincoin plus gentil avec dacode)

  Revision 1.2  2001/12/02 18:26:06  pouaite
  modif http (affreux hack pr ispell + http.c fait maintenant un #include de http_unix/win.c )

*/




/* 
   connection sans gestion de timeout ... pour les OS à moelle ? 
*/
int
net_tcp_connect(int fd, struct sockaddr_in *sock)
{
  int ret;
  assert(fd >= 0);
  printf("connect basique (susceptible de BLOQUER LE COINCOIN)..\n");

  do {
    ALLOW_X_LOOP; ALLOW_ISPELL;
  block_sigalrm(1);
    ret = connect (fd, (struct sockaddr *) sock, sizeof (struct sockaddr_in));
  block_sigalrm(0);
  ALLOW_X_LOOP; ALLOW_ISPELL;
//    printf("connect: ret=%d, errno=%d (%s)\n", ret, errno, strerror(errno));
  } while (errno == EINTR);
  return ret;
}

/* vole dans une mailing liste (je sais plus laquelle) , ça n'a pas l'air ultra-portable */
static int
net_tcp_connect_with_timeout (int fd, struct sockaddr_in *sock, int timeout_secs)
{
  struct timeval timeout;
  fd_set write_fds;

  assert(fd >= 0);
  
  /*
   * Set the socket to be non-blocking so that connect()
   * doesn't block.
   */
  if (fcntl(fd, F_SETFL, O_NONBLOCK) < 0)
    return -1;
  ALLOW_X_LOOP; ALLOW_ISPELL;
  /*
   * Setup the connection timeout.
   */
  //printf("timeout=%d sec\n", timeout_secs);
  timeout.tv_sec = timeout_secs;
  timeout.tv_usec = 0;
  
  while (1) {
    
    /*
     * Try to connect.
     */
    if (connect (fd, (struct sockaddr *) sock,
		 sizeof (struct sockaddr_in)) < 0) {
      ALLOW_X_LOOP; ALLOW_ISPELL;
      /* le test sur EISCONN special BSD -> bsd connecte plus vite que l'éclair? */
      if (errno == EISCONN) goto cassos;
      if (errno != EAGAIN && errno != EINPROGRESS) {
	printf("%d %d\n", errno, EADDRINUSE);
	perror ("net_tcp_connect_with_timeout: connect");
	return -1;
      }
      
    } else {
      //fprintf (stderr, "Connected succesfully!\n");
      return 0;
    }

    /*
     * We couldn't connect, so we select on the fd and
     * wait for the timer to run out, or for the fd to be
     * ready.
     */
    FD_ZERO (&write_fds);
    FD_SET (fd, &write_fds);

    while (select (getdtablesize (), NULL, &write_fds, NULL, &timeout) < 0) {
      ALLOW_X_LOOP; ALLOW_ISPELL;
      if (errno != EINTR) {
	perror ("net_tcp_connect_with_timeout: select");
	return -1;
      }
    }
    if (timeout.tv_sec == 0 && timeout.tv_usec == 0) {
      printf("Connection timed out (timeout=%d sec)!\n", timeout_secs);
      return -1;
    }
    ALLOW_X_LOOP; ALLOW_ISPELL;
  }
cassos:
  fcntl(fd, F_SETFL, fcntl(fd, F_GETFL) & ~O_NONBLOCK);
  ALLOW_X_LOOP; ALLOW_ISPELL;
  return 0;
}

void http_init() {
  global_http_download_cnt = 0;
  global_http_upload_cnt = 0;
}

int http_close (SOCKET fd) {
  do { close (fd); } while (errno == EINTR);
  return 0;
}
