/*
  rcsid=$Id: http_unix.c,v 1.14 2002/06/23 10:44:05 pouaite Exp $
  ChangeLog:
  $Log: http_unix.c,v $
  Revision 1.14  2002/06/23 10:44:05  pouaite
  i18n-isation of the coincoin(kwakkwak), thanks to the incredible jjb !

  Revision 1.12  2002/06/02 12:37:36  pouaite
  fix gethostbyname --> version 2.3.8b

  Revision 1.11  2002/06/01 17:54:04  pouaite
  nettoyage

  Revision 1.10  2002/05/13 08:01:11  pouaite
  bugfix (à tester) compil sous cygwin

  Revision 1.9  2002/05/12 22:06:27  pouaite
  grosses modifs dans http.c

  Revision 1.8  2002/03/03 10:10:04  pouaite
  bugfixes divers et variés

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
  printf(_("Basic connect (can BLOCK THE COINCOIN)...\n"));

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
	//	printf("%d %d\n", errno, EADDRINUSE);
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
      printf(_("Connection timed out (timeout=%d sec)!\n"), timeout_secs);
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


int http_close(SOCKET fd) {
  do { close (fd); } while (errno == EINTR);
  return 0;
}

/*
  un bon gros fork pour ne plus se figer pendant le gethostbyname

  ça a l'air de bien marcher
*/
int
gethostbyname_nonbloq(const char *hostname, unsigned char addr[65]) {
  pid_t pid;
  struct hostent *phost;
  int tube[2];

  BLAHBLAH(VERBOSE_LVL,fprintf(stderr, _("Welcome to the forked gethostbyname, everything is experimental, beware the zombies\n")));
  memset(addr, 0, 65);

  if (pipe(tube) == -1) {
    fprintf(stderr, _("Broken pipe: %s\n"), strerror(errno)); return -1;
  }
  switch ((pid = fork())) {
  case -1:
    fprintf(stderr, _("Ooooops, it looks like there is a throng of zombies out there\nthe fork failed: %s"), strerror(errno));
    return -1;
    break;

  case 0: { /* fiston */
    int n;
    if (close(tube[0]) == -1) {
      fprintf(stderr, _("son: pipe full (%s)\n"), strerror(errno)); close(tube[1]); exit(-1);
    }

    BLAHBLAH(VERBOSE_LVL,fprintf(stderr, _("son: gethostbyname going on...\n")));
    phost=gethostbyname(hostname);
    BLAHBLAH(VERBOSE_LVL,fprintf(stderr, _("son: gethostbyname finished.\n")));
    if( phost != NULL ) {
      unsigned char l;
      assert(phost->h_length < 128); /* faut pas pousser grand mère */
      l = (unsigned char)phost->h_length;
      errno = 0;
      n = write( tube[1], &l, 1); BLAHBLAH(VERBOSE_LVL,printf("write len %d [%s]\n", n, strerror(errno)));
      n = write( tube[1], phost->h_addr_list[0] , phost->h_length); BLAHBLAH(VERBOSE_LVL,printf("write adr %d [%s]\n", n, strerror(errno)));
    } else {
      fprintf(stderr, _("son: gethostbyname on '%s' failed.\n"), hostname);
    }
    exit(phost == NULL );
    break;
  }
  default: { /* pôpa */
    int cnt, got, len, cstat;
    time_t time_debut;
    if (close(tube[1]) == -1) {
      fprintf(stderr, _("daddy: pipe full (%s), what will do now ?\n"), strerror(errno)); close(tube[0]);
    }
    
    //fcntl( tube[0] , F_SETFD , fcntl( tube[0] , F_GETFD ) | O_NONBLOCK );
    cnt = 0; len = -1;
    time_debut = time(NULL);
    while (1) {
      int retval;

      ALLOW_X_LOOP_MSG("daddy listens to his son"); ALLOW_ISPELL;

      retval = http_select_fd(tube[0], 0, 10000, 0);
      BLAHBLAH(4,fprintf(stderr, "select : retval = %d %s\n", retval, (retval == -1) ? strerror(errno) : "ok"));

      /* tube prêt ? */
      if (retval > 0) {
	got = read(tube[0], addr+cnt, (len == -1 ? 65 : len+1-cnt)); 
	
	/* problème : got = 0 peut vouloir dire que 
	   (a) fiston n'a pas encore écrit dans le tube
	   (b) fiston {a écrit|a raté son gethostbyname} et fermé le tube
	*/

	if (got == 0) {
	  BLAHBLAH(VERBOSE_LVL, fprintf(stderr,_("The son got the pipe full again!\n")));
	  break;
	} else if (got == -1) {
	  BLAHBLAH(VERBOSE_LVL, fprintf(stderr,_("What a fucking pipe! %s\n"), strerror(errno))); break;
	}
	cnt += got;
	if (cnt >= 1) len = addr[0];
      } else if (retval == 0) { /* rien a lire pour l'instant */
	BLAHBLAH(4,fprintf(stderr, _("select .. awaiting\n")));
      } else if (errno == EINTR) {
	BLAHBLAH(4,fprintf(stderr, _("select .. interrupted\n")));
      } else {
	BLAHBLAH(4,fprintf(stderr, _("select .. problem : %s\n"), strerror(errno)));
      }
      /*
      if (difftime(time(NULL),time_debut) > 4) {
	fprintf(stderr, "fiston est trop lent, ou bien il s'est vautré, qu'il crève !\n");
	break;
      }
      */
    } /* while ((got == -1 && (errno == EAGAIN || errno == EINTR)) ||
         	 len == -1 || cnt < len+1); */

    close(tube[0]);

    while (waitpid(pid,&cstat,WNOHANG) == 0) {
      usleep(10000);
      ALLOW_X_LOOP_MSG("return from the forked gethostbyname"); ALLOW_ISPELL;
      printf(_("We're waiting for the son... Come here boy !\n"));
    }

    if (len == -1 || cnt < len+1) {
      fprintf(stderr, _("Daddy: I couldn't read from my son :-/ (len=%d, cnt=%d) (lasterr=%s)\n"), len, cnt, strerror(errno));
      return -1;
    }
    break;
  }
  }
  return 0;  /*   \o/    */
}
