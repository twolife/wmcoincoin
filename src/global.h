
/*
  rcsid=$Id: global.h,v 1.7 2002/01/13 15:19:00 pouaite Exp $
  ChangeLog:
  $Log: global.h,v $
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

#define MESSAGE_MAX_LEN 255
#define USERAGENT_MAX_LEN 60
#define USERNAME_MAX_LEN 60 // lg max du username affiché dans la tribune, pour les personnes loggées


#ifdef GLOBALS_HERE
# define DECL_GLOB(x) x;
# define DECL_GLOB_INIT(x,y) x = y
#else
# define DECL_GLOB(x) extern x
# define DECL_GLOB_INIT(x,y) extern x
#endif

#define WMCC_TIMER_DELAY_MS 40 /* un bip toutes les 40 millisecondes */

/* les preferences sont stockees dans cette structure */

typedef struct _structPrefs{
  /* delai (en sec) et assez approximatif entre deux verif de la tribune */
  int dlfp_tribune_check_delay;
  int dlfp_news_check_delay;
  /* Au bout de combien de temps on arrête lesdites verifications (en min)
     Note : la fréquence diminue progressivement.*/
  int dlfp_max_refresh_delay;

  char *font_encoding; /* 'iso8859-1' ou mieux, 'iso8859-15' */

  /* nb max de messages conserves en memoire */
  int tribune_max_msg;
  /* fonte utilisee pour l'affichages des news */
  char *news_fn_family;
  int news_fn_size;

  /* on ne conserve que les news aui ont ete postees il y a 
     moins de 'news_max_nb_days' jours */
  int news_max_nb_days;
  int news_bgcolor, news_fgcolor; /* couleur d'affichage de la fenetre des news */
  int news_xpos, news_ypos, news_width, news_height; /* dimensions,positions de cette fenetre */

  /* pour mise au point ... */
  int debug;
  int verbosity; /* 1, 2 ou 3 voire plus */

  int http_timeout; /* en secondes */
  int proxy_nocache; /* desactive le cache du proxy lors des requetes */

  /* user_agent (defaut: wmCoinCoin) */
  char *user_agent;

  char *proxy_auth; /* default: NULL */
  char *proxy_name; /* default: NULL */
  int   proxy_port; /* default: 1080 */

  /* message poste sur la tribune */
  char *coin_coin_message;

  /* indique si on veut utiliser un nom d'utilisateur (genre 'mmu>' ) */
  char *user_name;

  /* pour savoir si on utilise l'icone (style windowmaker) ou bien la fenetre (style swallow kde) */
  int use_iconwin; 

  /* indique si la bordure est a la charge du windowmanager (ie si le WM a un dock:
     WMaker, KDE..) ou si wmcoincoin doit dessiner lui meme sa bordure */
  int draw_border;
  int no_balloons;
  char *balloon_fn_family;
  int balloon_fn_size;
  int dock_xpos, dock_ypos;
  int bgcolor, fgcolor;
  char *bgpixmap; /* nom du fichier xpm de fond (c)(tm)(r)kadreg :) */
  char *app_name; /* pointeur vers le nom sous lequel a ete invoque
		    wmcoincoin (pour reconnaitre quand on l'appelle
		    sous le nom wmcoincoin-kde */

  char *site_root; /* par defaut: 'linuxfr.org' */
  char *site_path; /* par defaut: "" */
  int site_port; /* le port http (defaut 80) */

  char *options_file_name; /* le nom du fichier d'options (par defaut: 'options') */

  char *path_tribune_backend; /* par defaut: "board/remote.xml" */
  char *path_news_backend; /* "short.php3" */
  char *path_end_news_url; /* ",0,-1,6.html" */
  char *path_tribune_add;  /* "board/add.php3" */
  char *path_myposts; /* users/myposts.php3?order=id" */
  char *path_messages; /* messages */

  char *site_path_remote;
  char *site_theme_num;

  int default_trollo_speed; /* vitesse du trolloscope par defaut */
  char *user_cookie;

  char *browser_cmd; /* commande pour afficher une url dans un browser (le %s sera remplace par l'url) */
  char *browser2_cmd; /* le browser alternative (lancé par un clic milieu au lieu d'un clic gauche) */
  char *pp_fn_family; /* defaut : 'helvetica' */
  int pp_fn_size;
  unsigned pp_bgcolor, pp_fgcolor, pp_tstamp_color, pp_useragent_color, pp_login_color,
    pp_url_color, pp_button_color, pp_emph_color, pp_trollscore_color;
  int pp_xpos, pp_ypos, pp_width, pp_height, pp_minibar_on;
  int pp_nosec_mode, pp_html_mode, pp_nick_mode, pp_trollscore_mode, pp_fortune_mode;
  unsigned pp_fortune_bgcolor, pp_fortune_fgcolor;
  char *pp_fortune_fn_family;
  int pp_fortune_fn_size;
  int enable_troll_detector;
  int force_fortune_retrieval; /* pour forcer le download de myposts.php3 même si on n'a pas fourni le cookie */

  /* preferences pour le spelchecker */
  int ew_do_spell;
  char* ew_spell_cmd;
  char* ew_spell_dict;
  char *post_cmd; /* commande exécutée après chaque nouveau post lu ($m=message, $l=login, $t=timestamp, $u=ua, $i=id, $s=troll_score) */
  /* Nom du fichier de scrinechote */
  char* tribune_scrinechote;
} structPrefs;

/* variables communes ici: */

#ifndef __CYGWIN__
/*
  il faut des protections pour les appels systèmes non reentrant :-( (malloc...)
  (la cause des bugs bizarres du "mur vert" ?)
*/
void X_loop();
void ispell_run_background(const char* spellCmd, const char* spellDict);
#define ALLOW_ISPELL if (Prefs.ew_do_spell) {ispell_run_background(Prefs.ew_spell_cmd, Prefs.ew_spell_dict);}
#define ALLOW_X_LOOP if (X_loop_request) { if (X_loop_request > 1 && Prefs.verbosity) { printf("%s, ligne %d : X_loop_request=%d!\n", __FILE__, __LINE__, X_loop_request); }X_loop(); }
#define ALLOW_X_LOOP_MSG(m) if (X_loop_request) { if (X_loop_request > 1 && Prefs.verbosity) { printf(m " : X_loop_request=%d!\n", X_loop_request); }  X_loop(); }
#else /* utilise les pthreads, pas besoin de protection */
#define ALLOW_X_LOOP
#define ALLOW_X_LOOP_MSG(m)
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
DECL_GLOB_INIT(volatile int flag_sending_coin_coin, 0); /* utilisé pour savoir si on est en train de poster un message */
DECL_GLOB_INIT(volatile int flag_troll_braining,0); /* pour indiquer sur la led si on est dans 'troll_detector' */
DECL_GLOB_INIT(volatile int flag_tribune_updated, 0);
DECL_GLOB_INIT(volatile int flag_http_transfert, 0);
DECL_GLOB_INIT(volatile int flag_http_error, 0);
DECL_GLOB_INIT(volatile int flag_spell_request, 0); /* pour le palmipede (ça commence à puer le vilain hack... mais bon, je veux pas de threads alors j'assume) */
DECL_GLOB_INIT(volatile int flag_spell_finished, 0);
DECL_GLOB_INIT(volatile int wmcc_tic_cnt,0);

#endif

