#ifndef __PREFS_H
#define __PREFS_H
#include <string.h>
#include "options_list.h" /* liste des noms des options */

#define MESSAGE_MAX_LEN 255
#define USERAGENT_MAX_LEN 60

/* deux type de transparence: 
 - shading : le fond est obscurci (0=>transparence totale, 100=>opacité(noir) complète)
 - teinte  : le noir devient tint_black, le blanc prend la couleur tint_white,
             et les couleurs d'intensité intermédiaires sont interpolées entre ces deux valeurs (activé si shade == -1)
*/
typedef struct {
  int shading;
  unsigned long tint_white, tint_black;
} TransparencyInfo;

/*
  pour stocker deux couleurs (une pour le mode normal, ou pour le mode transparent)
*/
typedef struct {
  unsigned opaque;
  unsigned transp;
} BiColor;

/* les preferences sont stockees dans cette structure */

typedef struct _structPrefs{
  /* delai (en sec) et assez approximatif entre deux verif de la tribune */
  int dlfp_tribune_check_delay;
  int dlfp_news_check_delay;
  /* en cas d'inactivité les delais vont être progressivement augmentés
     max_refresh_delay est leur valeur maximale (en minutes) (0 => ralentissement désactivé) */
  int dlfp_max_refresh_delay;
  /* de plus au bout d'un certain nombre de minutes, le coincoin cessera toute activité
     et passera en mode horloge */
  int dlfp_switch_off_coincoin_delay;

  char *font_encoding; /* 'iso8859-1' ou mieux, 'iso8859-15' */

  /* nb max de messages conserves en memoire */
  int tribune_max_msg;

  int tribune_backend_type; /* pour gerer les variations saisonnières du format de backend..
			   peut prendre 3 valeurs: 
			   1: les tags apparaissent sous la forme '<b>'
			   2: les tags sont du type '&lt;b;&gt'
			   3: le backend est en vrac, on a les deux types de tags :-/ 
			*/
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
  int use_if_modified_since; /* utilisation de l'entete http 'If-Modified-Since' */
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
  int use_balloons;
  char *balloon_fn_family;
  int balloon_fn_size;
  int dock_xpos, dock_ypos;
  int start_in_boss_mode; /* demarrage en mode horloge */
  int dock_bgcolor, dock_fgcolor;
  char *dock_bgpixmap; /* nom du fichier xpm de fond (c)(tm)(r)kadreg :) */

  char *site_root; /* par defaut: 'linuxfr.org' */
  char *site_path; /* par defaut: "" */
  int site_port; /* le port http (defaut 80) */

  char *path_tribune_backend; /* par defaut: "board/remote.xml" */
  char *path_news_backend; /* "short.php3" */
  char *path_end_news_url; /* ",0,-1,6.html" */
  char *path_tribune_add;  /* "board/add.php3" */
  char *path_myposts; /* users/myposts.php3?order=id" */
  char *path_messages; /* messages */

  int default_trollo_speed; /* vitesse du trolloscope par defaut */
  char *user_cookie;
  char *user_login; /* le login (optionnel, peut etre NULL), utilisé pour la reconnaissance des messages que vous avez posté */

  char *browser_cmd; /* commande pour afficher une url dans un browser (le %s sera remplace par l'url) */
  char *browser2_cmd; /* le browser alternative (lancé par un clic milieu au lieu d'un clic gauche) */
  char *pp_fn_family; /* defaut : 'helvetica' */
  int pp_fn_size;
  unsigned pp_bgcolor;
  int pp_start_in_transparency_mode;
  TransparencyInfo pp_transparency;

  BiColor pp_fgcolor, pp_tstamp_color, pp_useragent_color, 
    pp_login_color, pp_url_color, pp_button_color, pp_emph_color, 
    pp_sel_bgcolor, pp_popup_fgcolor, pp_popup_bgcolor,
    pp_trollscore_color, pp_my_msg_color, pp_answer_my_msg_color, 
    pp_keyword_color, pp_plopify_color;
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

  char **plop_words; /* un tableau de string pour ranger les mots-plop (patch estian)*/
  unsigned  nb_plop_words;
} structPrefs;



#define FREE_STRING(x) { if (x) { free(x); x = NULL; }}
#define ASSIGN_STRING_VAL(x,v) { FREE_STRING(x); x = strdup(v); assert(x); }


/* remplit la structure avec les valeurs par défaut des preferences */
void wmcc_prefs_set_default(structPrefs *p);
/* libere la mémoire allouée par les champs de la structure, *mais* pas la structure elle-même */
void wmcc_prefs_destroy(structPrefs *p);
/* assigne une option dans les preferences, renvoie un message d'erreur si y'a un pb */
char *wmcc_prefs_validate_option(structPrefs *p, wmcc_options_id opt_num, unsigned char *arg);
/* lecture d'un fichier d'options, renvoie un message d'erreur si y'a un pb */
char *wmcc_prefs_read_options(structPrefs *p, const char *filename);
#endif
