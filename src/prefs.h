#ifndef __PREFS_H
#define __PREFS_H
#include <string.h>
#include <sys/types.h> /* obligatoire pour regex.h sous bsd */
#include <regex.h>
#include "options_list.h" /* liste des noms des options */
#include "keylist.h"

/* valeur absolument maximale de la longueur d'un message envoyé
   (selon les prefs par sites, des valeurs inférieure sont fixées */
#define MESSAGE_MAXMAX_LEN 1024
#define USERAGENT_MAXMAX_LEN 200

/* deux type de transparence: 
 - shading : le fond est obscurci (0=>transparence totale, 100=>opacité(noir) complète)
 - teinte  : le noir devient tint_black, le blanc prend la couleur tint_white,
             et les couleurs d'intensité intermédiaires sont interpolées entre ces deux valeurs (activé si shade == -1)
*/
typedef struct {
  enum {FULL_TRANSPARENCY, SHADING, TINTING} type;
  struct {
    unsigned long white, black;
  } tint;
  struct {
    int luminosite, assombrissement;
  } shade;
} TransparencyInfo;

/*
  pour stocker deux couleurs (une pour le mode normal, ou pour le mode transparent)
*/
typedef struct {
  unsigned opaque;
  unsigned transp;
} BiColor;

/*
  les regles de marquage visuel des messages (pour le trolloscope et les ua raccourcis du pinni)
*/
typedef struct _MiniUARule {
  char     *site_name;   /* si non-nul, doit matcher le site du message */
  char     *user_login;  /* si non-nul, doit matcher le login du posteur */
  char     *s_rgx;       /* la description originale de la regex */
  regex_t  *rgx;         /* si non-nul, l'ua doit matcher la regexp */
  char     *rua;         /* si non-nul, indique l'ua raccourcie de remplacement (peut
			    avoir des \1, \2 etc pour faire des truc avec rgx) */
  int      color;        /* si positif, indique la couleur imposée dans le trolloscope */
  int      symb;         /* si positif, indique la forme imposée dans le trolloscope */
  int      ua_terminal:1;     /* si c'est une regle terminale pour l'ua */
  int      color_terminal:1;  /* si c'est une regle terminale pour la couleur */
  int      symb_terminal:1;   /* si c'est une regle terminale pour le symbole */
  struct _MiniUARule *next;
} MiniUARule;

typedef struct _MiniUARules {
  MiniUARule *first;
} MiniUARules;

typedef struct _URLReplacement {
  char *key;
  char *repl;
  struct _URLReplacement *next;
} URLReplacement;

typedef struct _URLReplacements {
  URLReplacement *first;
} URLReplacements;


#define NB_PP_KEYWORD_CATEG 5


/* les preferences sont stockees dans ces structures */

typedef struct _SitePrefs {
  /* delai (en sec) et assez approximatif entre deux verif de la tribune */
  int board_check_delay;
  int news_check_delay;
  int board_backend_type; /* pour gerer les variations saisonnières du format de backend..
			   peut prendre 3 valeurs: 
			   1: les tags apparaissent sous la forme '<b>'
			   2: les tags sont du type '&lt;b;&gt'
			   3: le backend est en vrac, on a les deux types de tags :-/ 
			*/
  /* nb max de messages conserves en memoire */
  int board_max_msg;
  char *board_wiki_emulation; /* non nul => émulation des wiki-urls */
  /* on ne conserve que les news aui ont ete postees il y a 
     moins de 'news_max_nb_days' jours */
  int news_max_nb_days;
  /* user_agent (defaut: wmCoinCoin) */
  char *user_agent;
  char *proxy_auth_user; /* default: NULL */
  char *proxy_auth_pass; /* default: NULL */
  char *proxy_name; /* default: NULL */
  int   proxy_port; /* default: 1080 */

  /* pour forcer le download de myposts.php3 même si on n'a pas fourni le cookie */
  int force_fortune_retrieval; 

  /* indique si on veut utiliser un nom d'utilisateur (genre 'mmu>' ) */
  char *user_name;

  /* lg max des messages / useragents */
  int palmi_msg_max_len;
  int palmi_ua_max_len;

  char *site_root; /* par defaut: 'linuxfr.org' */
  char *site_path; /* par defaut: "" */
  int site_port; /* le port http (defaut 80) */
  char *path_board_backend; /* par defaut: "board/remote.xml" */
  char *path_news_backend; /* "short.php3" */
  char *path_end_news_url; /* ",0,-1,6.html" */
  char *path_board_add;  /* "board/add.php3" */
  char *path_myposts; /* users/myposts.php3?order=id" */
  char *path_messages; /* messages */
  char *user_cookie;
  char *user_login; /* le login (optionnel, peut etre NULL), utilisé pour la reconnaissance des messages que vous avez posté */

  /* quelques couleurs qui dépendent du site visité */
  BiColor pp_fgcolor, pp_tstamp_color, pp_useragent_color, 
    pp_login_color, pp_url_color, pp_visited_url_color, pp_strike_color, pp_trollscore_color;
  unsigned pp_bgcolor;
  int proxy_nocache; /* desactive le cache du proxy lors des requetes */
  int use_if_modified_since; /* utilisation de l'entete http 'If-Modified-Since' */
  enum { locEN=0, locFR=1 } locale; /* "localisation" du site (pour quelques mots-clefs + problèmes d'horloges AM/PM) */
  int use_AM_PM;
  char **all_names; /* liste des noms possibles du site (tous les alias) */
  int  nb_names;
  char *site_name; /* le nom court du site (premier argument de l'option 'site:'),
		      le pointeur pointe sur all_names[0] */
  long time_difference; /* decalage horaire du site */
  int check_news;
  int check_board;
  int check_comments;
  int check_messages;
} SitePrefs;

#define MAX_SITES 20 /* au-dela, faut vraiment songer à consulter */

typedef struct _GeneralPrefs{
  /* en cas d'inactivité les delais vont être progressivement augmentés
     max_refresh_delay est leur valeur maximale (en minutes) (0 => ralentissement désactivé) */
  int max_refresh_delay;
  /* de plus au bout d'un certain nombre de minutes, le coincoin cessera toute activité
     et passera en mode horloge */
  int switch_off_coincoin_delay;

  char *font_encoding; /* 'iso8859-1' ou mieux, 'iso8859-15' */

  /* message poste sur la tribune */
  char *coin_coin_message;

  /* fonte utilisee pour l'affichages des news */
  char *news_fn_family;
  int news_fn_size;

  int news_titles_fgcolor, news_emph_color;
  int news_bgcolor, news_fgcolor; /* couleur d'affichage de la fenetre des news */
  int news_titles_bgcolor;
  int news_xpos, news_ypos, news_width, news_height; /* dimensions,positions de cette fenetre */

  /* pour mise au point ... */
  int debug;
  int verbosity; /* 1, 2 ou 3 voire plus */

  int http_timeout; /* en secondes */

  char *gogole_search_url;


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
  char *dock_skin_pixmap;



  int default_trollo_speed; /* vitesse du trolloscope par defaut */

  char *browser_cmd; /* commande pour afficher une url dans un browser (le %s sera remplace par l'url) */
  char *browser2_cmd; /* le browser alternative (lancé par un clic milieu au lieu d'un clic gauche) */
  char *pp_fn_family; /* defaut : 'helvetica' */
  int pp_fn_size;
  int pp_start_in_transparency_mode;
  TransparencyInfo pp_transparency;

  int use_fake_real_transparency;

  BiColor sc_bg_color, sc_bg_light_color, sc_bg_dark_color, 
    sc_arrow_normal_color, sc_arrow_emphasized_color,
    sc_bar_color, sc_bar_light_color, sc_bar_dark_color;

  BiColor  pp_emph_color, 
    pp_sel_bgcolor, pp_popup_fgcolor, pp_popup_bgcolor,
    pp_my_msg_color, pp_answer_my_msg_color, 
    pp_keyword_color[NB_PP_KEYWORD_CATEG], pp_plopify_color,
    pp_buttonbar_bgcolor, pp_buttonbar_fgcolor,
    pp_buttonbar_msgcnt_color, pp_buttonbar_updlcnt_color,
    pp_buttonbar_progressbar_color;
  int pp_xpos, pp_ypos, pp_width, pp_height, pp_minibar_on;
  int pp_show_sec_mode, pp_html_mode, pp_nick_mode, pp_trollscore_mode, pp_fortune_mode;
  unsigned pp_fortune_bgcolor, pp_fortune_fgcolor;
  char *pp_fortune_fn_family;
  int pp_fortune_fn_size;
  int enable_troll_detector;


  /* preferences pour le spelchecker */
  int ew_do_spell;
  char* ew_spell_cmd;
  char* ew_spell_dict;
  char *post_cmd; /* commande exécutée après chaque nouveau post lu ($m=message, $l=login, $t=timestamp, $u=ua, $i=id, $s=troll_score) */
  /* Nom du fichier de scrinechote */
  char* board_scrinechote;

  KeyList *hilight_key_list; /* liste des mots clef declenchant la mise en valeur du post dans le pinnipede 
				attention c'est Mal, mais c'est le pinnipede qui ecrit dans cette liste..
			     */
  KeyList *plopify_key_list; /* version plopesque du kill-file, même remarque qu'au dessus */
  char **plop_words;
  unsigned nb_plop_words;
  int pinnipede_open_on_start; /* on décide que le pinnipede s'ouvre comme un grand tout seul quand on lance le coincoin */

  MiniUARules miniuarules;

  URLReplacements url_repl;

  int nb_sites;
  SitePrefs *site[MAX_SITES];
} GeneralPrefs;


/* et attention, on peut faire de l'antialiasing (hum..)
   # -> le pixel est éclairci (rgb*5/4)
   X -> le pixel est a la couleur complete
   x -> divise le rgb par 2
   : -> divise par 3
   . -> divise par 4
   c'est-y-pas cool ?
*/
#define NB_SYMBOLES 16
typedef struct _SymboleDef {
  char *s[5];
  char *name;
} SymboleDef;

#ifndef GLOBALS_HERE
extern SymboleDef symboles[NB_SYMBOLES];
#else
SymboleDef symboles[NB_SYMBOLES] = {{{"     ",
				      "     ",
				      "     ",
				      "     ",
				      "     "},"ignore"},
				    {{"xxxxX",
				      "xXXX#",
				      "xXXX#",
				      "xXXX#",
				      "X####"},"square"},
				    {{" ::: ",
				      ":X##X",
				      ":# :#",
				      ":#::#",
				      " X##."},"circ"},
				    {{"  x  ",
				      " :X: ",
				      " XXX ",
				      ":XXX:",
				      "XXXXX"},"dtri"},
				    {{"XXXXX",
				      ":XXX:",
				      " XXX ",
				      " :X: ",
				      "  x  "},"tri"},
				    {{"X:   ",
				      "XXX: ",
				      "XXXXX",
				      "XXX: ",
				      "X:   "},"ltri"},
				    {{"   :X",
				      " :XXX",
				      "XXXXX",
				      " :XXX",
				      "   XX"},"rtri"},
				    {{"   :X",
				      "  :X#",
				      " :X# ",
				      ":X#  ",
				      "X#   "},"antislash"},
				    {{"Xx   ",
				      "xXx  ",
				      " xXx ",
				      "  xXx",
				      "   xX"},"slash"}
,				    {{"X. .X",
				      ".X.X.",
				      " .X. ",
				      ".X.X.",
				      "X. .X"},"cross"},
				    {{"x    ",
				      "xX   ",
				      "xXX  ",
				      "xXXX ",
				      "X####"},"ldtri"},
				    {{"    X",
				      "   x#",
				      "  xX#",
				      " xXX#",
				      "X####"},"rdtri"},
				    {{"xxxxX",
				      "xXX# ",
				      "xX#  ",
				      "x#   ",
				      "X    "},"lutri"},
				    {{"xxxxX",
				      " XXX#",
				      "  XX#",
				      "   X#",
				      "    X"},"rutri"},
				    {{"  X  ",
				      "  X  ",
				      "XXXXX",
				      "  X  ",
				      "  X  "},"plus"},
				    {{"  x  ",
				      " xXX ",
				      "xXXX#",
				      " XX# ",
				      "  #  "}, "diamond"}};
#endif



#define FREE_STRING(x) { if (x) { free(x); x = NULL; }}
#define ASSIGN_STRING_VAL(x,v) { FREE_STRING(x); x = strdup(v); assert(x); }

char *string_to_miniuarule(unsigned char *str, MiniUARule *r);
void miniuarule_clear(MiniUARule *r);
void miniuarules_destroy(MiniUARules *urs, MiniUARule *ur);

/* remplit la structure avec les valeurs par défaut des preferences */
void wmcc_prefs_set_default(GeneralPrefs *p);
/* libere la mémoire allouée par les champs de la structure, *mais* pas la structure elle-même */
void wmcc_prefs_destroy(GeneralPrefs *p);
/* assigne une option dans les preferences, renvoie un message d'erreur si y'a un pb */
char *
wmcc_prefs_validate_option(GeneralPrefs *p, SitePrefs *sp, SitePrefs *global_sp, 
			   wmcc_options_id opt_num, unsigned char *arg);
/* lecture d'un fichier d'options, renvoie un message d'erreur si y'a un pb */
char *wmcc_prefs_read_options(GeneralPrefs *p, const char *filename);
SitePrefs * wmcc_prefs_find_site(GeneralPrefs *p, const char *name);
void wmcc_site_prefs_destroy(SitePrefs *p);
void wmcc_site_prefs_copy(SitePrefs *sp, const SitePrefs *src);
#endif
