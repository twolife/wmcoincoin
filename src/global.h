
/*
  rcsid=$Id: global.h,v 1.20 2002/04/09 00:28:19 pouaite Exp $
  ChangeLog:
  $Log: global.h,v $
  Revision 1.20  2002/04/09 00:28:19  pouaite
  quelques modifs faites dans un état d'hébétude avancé /!\ travaux en cours /!\

  Revision 1.19  2002/04/01 01:39:38  pouaite
  grosse grosse commition (cf changelog)

  Revision 1.18  2002/03/18 22:46:49  pouaite
  1 ou 2 bugfix mineurs, et surtout suppression de la dependance avec la libXpm .. un premier pas vers wmc² en 8bits

  Revision 1.17  2002/03/07 18:54:34  pouaite
  raaa .. fix login_color (jjb) patch plop_words (estian) et bidouille pour le chunk encoding (a tester)

  Revision 1.16  2002/03/03 10:10:04  pouaite
  bugfixes divers et variés

  Revision 1.15  2002/02/26 22:02:07  pouaite
  bugfix gruikissime pour les pbs de lag sous cygwin

  Revision 1.14  2002/02/26 09:18:23  pouaite
  bugfixes divers

  Revision 1.13  2002/02/24 22:13:57  pouaite
  modifs pour la v2.3.5 (selection, scrollcoin, plopification, bugfixes)

  Revision 1.12  2002/02/02 23:49:17  pouaite
  plop

  Revision 1.11  2002/01/20 02:17:13  pouaite
  modifs d'ordre esthetique (!) sans grand interet

  Revision 1.10  2002/01/19 19:56:09  pouaite
  petits crochets pour la mise en valeur de certains messages (cf changelog)

  Revision 1.9  2002/01/16 00:35:26  pouaite
  debut de detection des reponse à nos message avec des couleurs hideuses et certainement plein de bugs moisis

  Revision 1.8  2002/01/14 23:54:06  pouaite
  reconnaissance des posts effectué par l'utilisateur du canard (à suivre...)

  Revision 1.7  2002/01/13 15:19:00  pouaite
  double patch: shift -> tribune.post_cmd et lordOric -> tribune.archive

  Revision 1.6  2002/01/12 17:29:08  pouaite
  support de l'iso8859-15 (euro..)

  Revision 1.5  2002/01/10 09:18:23  pouaite
  patch de jjb (ralentissement progressif des updates de la tribune en cas d'inactivité du coincoin)

  Revision 1.4  2001/12/18 12:43:37  pouaite
  ajout de l'option de la fonte des ballons d'aide (pour mr. imr !) + bugfix d'une connerie assez naze dans la gestion du nom du fichier d'options (merci glandium de me l'avoir signalé)

  Revision 1.3  2001/12/16 01:43:33  pouaite
  filtrage des posts, meilleure gestion des posts multiples

  Revision 1.2  2001/12/02 18:34:54  pouaite
  ajout de tags cvs Id et Log un peu partout...

*/

#ifndef GLOBAL_H
#define GLOBAL_H

#include "config.h"
#include "prefs.h"
#ifdef TEST_MEMLEAK
#include "mpatrol.h"
#endif

#define USERNAME_MAX_LEN 60 // lg max du username affiché dans la tribune, pour les personnes loggées

#ifdef __GNUC__
# define UNUSED __attribute((unused))
#else
# define UNUSED
#endif

#ifdef GLOBALS_HERE
# define DECL_GLOB(x) x;
# define DECL_GLOB_INIT(x,y) x = y
#else
# define DECL_GLOB(x) extern x
# define DECL_GLOB_INIT(x,y) extern x
#endif

#ifndef NO_BLAHBLAH
# define BLAHBLAH(n,x) if (Prefs.verbosity >= n) { x; fflush(stdout); }
#else
# define BLAHBLAH(n,x)
#endif

#ifndef NO_BITFIELDS
#  define BITFIELD(n) :n
#else
#  define BITFIELD(n) 
#endif 

#define WMCC_TIMER_DELAY_MS 40 /* un bip toutes les 40 millisecondes */


/* variables communes ici: */

#ifdef USE_VALGRIND
# define NOSIGNALS
# include <sys/times.h>
  DECL_GLOB_INIT(volatile clock_t last_call_X_loop, 0);
# define VALGRINDCHK { struct tms buf; clock_t t = times(&buf); if (t-last_call_X_loop > 3) { last_call_X_loop = t; X_loop_request++; wmcc_tic_cnt++; }}
#else
# define VALGRINDCHK
#endif

/*
  il faut des protections pour les appels systèmes non reentrant :-( (malloc...)
  (la cause des bugs bizarres du "mur vert" ?)
*/
void X_loop();
void ispell_run_background(const char* spellCmd, const char* spellDict);
#define ALLOW_ISPELL if (Prefs.ew_do_spell) {ispell_run_background(Prefs.ew_spell_cmd, Prefs.ew_spell_dict);}
#define ALLOW_X_LOOP VALGRINDCHK; if (X_loop_request) { if (X_loop_request > 1 && Prefs.verbosity) { printf("%s, ligne %d : X_loop_request=%d!\n", __FILE__, __LINE__, X_loop_request); }X_loop(); }
#define ALLOW_X_LOOP_MSG(m) VALGRINDCHK; if (X_loop_request) { if (X_loop_request > 1 && Prefs.verbosity) { printf(m " : X_loop_request=%d!\n", X_loop_request); }  X_loop(); }

/* très très laid, voir wmcoincoin.c/Timer_Thread */
#ifdef __CYGWIN__
# define CYGWIN_ENABLE_THREAD_X_LOOP flag_cygwin_x_loop_in_thread = 1;
# define CYGWIN_DISABLE_THREAD_X_LOOP flag_cygwin_x_loop_in_thread = 0;
DECL_GLOB_INIT(volatile int flag_cygwin_x_loop_in_thread,0);
#else
# define CYGWIN_ENABLE_THREAD_X_LOOP 
# define CYGWIN_DISABLE_THREAD_X_LOOP 
#endif


DECL_GLOB(structPrefs Prefs);
DECL_GLOB_INIT(volatile int X_loop_request, 0);

/* +1 = demande au coincoin de fermer ses fenetres,
   -1 = demande au coincoin de se rouvrir
   0  = ça roule
*/
DECL_GLOB_INIT(volatile int flag_discretion_request, 0);
/*
  non nul quand on met a jours les donnees:
   -> updates des news, update tribune
*/
DECL_GLOB_INIT(volatile int flag_updating_news, 0);
DECL_GLOB_INIT(volatile int flag_updating_messagerie, 0);
DECL_GLOB_INIT(volatile int flag_news_updated, 0);
DECL_GLOB_INIT(volatile int flag_updating_comments, 0); /* utilisé aussi pour la maj de la fortune et du CPU */
DECL_GLOB_INIT(volatile int flag_updating_tribune, 0);
DECL_GLOB_INIT(volatile int flag_tribune_answer_to_me, 0); /* clignotement bleu du flamometre quand une reponse à un de nos posts sur la tribune vient d'arriver */
DECL_GLOB_INIT(volatile int flag_sending_coin_coin, 0); /* utilisé pour savoir si on est en train de poster un message */
DECL_GLOB_INIT(volatile int flag_troll_braining,0); /* pour indiquer sur la led si on est dans 'troll_detector' */
DECL_GLOB_INIT(volatile int flag_tribune_updated, 0);
DECL_GLOB_INIT(volatile int flag_http_transfert, 0);
DECL_GLOB_INIT(volatile int flag_http_error, 0);
DECL_GLOB_INIT(volatile int flag_spell_request, 0); /* pour le palmipede (ça commence à puer le vilain hack... mais bon, je veux pas de threads alors j'assume) */
DECL_GLOB_INIT(volatile int flag_spell_finished, 0);
DECL_GLOB_INIT(volatile int wmcc_tic_cnt,0);

DECL_GLOB_INIT(unsigned global_http_upload_cnt,0);
DECL_GLOB_INIT(unsigned global_http_download_cnt,0);
DECL_GLOB_INIT(char *options_file_name, NULL); /* le nom du fichier d'options (par defaut: 'options') */

DECL_GLOB_INIT(int flag_changed_http_params, 0); /* positionné à 1 quand les prefs viennent
						    d'être relues, et que l'adresse du site
						    a été changée (ou le proxy) -> pour
						    forcer le gethostbyname */
DECL_GLOB_INIT(char *app_useragent, NULL);

#endif

