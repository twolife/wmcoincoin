#ifndef COINCOIN_H
#define COINCOIN_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <assert.h>
#include <time.h>
#include <regex.h>
#include <ctype.h>
#include "global.h"
#include "coin_util.h"
#include "myprintf.h"


/* tentative de compilation avec _XOPEN_SOURCE, y'a plein de warnings */
#ifdef _XOPEN_SOURCE
int strcasecmp(const char *s1, const char *s2);
int strncasecmp(const char *s1, const char *s2, size_t n);
char *strdup(const char *s);
int snprintf(char *str, size_t size, const char *format, ...);
void bzero(void *s, size_t n);
void usleep(unsigned long usec);
#endif

#define IS_INSIDE(x, y, xmin, ymin, xmax, ymax) ((x) >= (xmin) && (x) <= (xmax) && (y) >= (ymin) && (y) <= (ymax))

#define APPNAME "wmcoincoin"
#define APP_USERAGENT "wmCoinCoin/" VERSION

typedef struct _PicoHtmlItem PicoHtmlItem;
typedef struct _PicoHtml PicoHtml;

typedef long long int64;
typedef unsigned long long uint64;

typedef struct _Newswin Newswin;

typedef struct _DLFP_news {

  unsigned char *titre;
  unsigned char *txt;
  unsigned char *auteur;
  unsigned char *topic;
  unsigned char *mail;
  unsigned char *url; /* tronquee (cad moins le http://, le port, et le ,0,-1,7.html) */

  char date[11];
  int heure; /* en nombre de minutes depuis minuit */
  int id;
  int nb_comment; /* pas très utile... */
  struct _DLFP_news *next;

  /* =1 si la news n'a pas encore ete lue */
  /* =0 si elle a ete lue, et que le fichier de
     sauvegarde des news lues est a jour */
  /* -1 si elle vient d'etre lue, mais qu'il
     faut mettre a jour le fichier */
  int flag_unreaded;
} DLFP_news;

#define trollo_log_extent 5 /* minutes */

//#define TRIBUNE_MAX_MSG 400 /* nb max de messages gardés en mémoire */

/* la tribune load affiche les messages sur les 15 dernieres minutes */
#define TRIBUNE_LOAD_NB_MINUTES 11
/* sur une minute, elle peut stocker 5 messages simultanes */
#define TRIBUNE_LOAD_MAX_SIMUL 5

#define TRIBUNE_LOAD_WIDTH TRIBUNE_LOAD_NB_MINUTES*5
/* le -2 c fait expres -> quand il y a + de 5 msg dans
   la meme minute, on voit le 5eme sans pouvoir l'identifier vraiment
   c mieux que de prendre une hauteur de exactement 4 formes */
#define TRIBUNE_LOAD_HEIGHT TRIBUNE_LOAD_MAX_SIMUL*5-4
#define TRIBUNE_LOAD_X 4
#define TRIBUNE_LOAD_Y 25


typedef struct _DLFP_trib_load_rule {
  unsigned R,G,B;
  int symb; /* numero du symbole */
  regex_t rule; /* compile avec REG_NOSUB (important, cf man page)*/
  struct _DLFP_trib_load_rule *next;

  char *name; /* nom de la regle  */
} DLFP_trib_load_rule;


typedef struct _tribune_msg_info {

  int id;
  /* (year-2000)|month|day|h|m|s */
  time_t timestamp;
  char sub_timestamp; /* sous numerotation quand plusieurs posts ont le même timestamp 
			 (-1 -> pas (encore) d'autre post avec le meme tstamp)
		      */

  char hmsf[4]; /* heure, minute, seconde + flag d'affichage des secondes (1 == secondes necessaires)  */
  char *useragent; /* pointe dans la zone mémoire allouée pour tribune_msg_info -> ne pas faire de free(useragent) !!! */
  char *msg; /* pointe dans la zone mémoire allouée pour tribune_msg_info -> ne pas faire de free(msg) !!! */
  char *login; /* nouveau !! */
  struct _tribune_msg_info *next;

  /* tatouage pointe sur la regle que satisfie 'useragent' 
     c'est update_pix_trib_load qui s'en occupe

     peut valoir etre NULL (non identifie, ne sera pas affiche dans tribune_load 
     mais attention, l'allocation ne se fait pas ici, le pointeur designe
     juste un element de 'tribune.rules'
  */
  DLFP_trib_load_rule *tatouage;
  short troll_score; /* le niveau de trollitude du post (cf troll_detector.c) */
} tribune_msg_info;



typedef struct _DLFP_tribune {
  unsigned char last_post_time[5];

  int last_post_timestamp; /* en secondes */
  int last_post_id;

  /* log des 'last_post_id' au cours des 'nb_trollo_log' derniers check de la tribune,
     utilise pour calculer le nb moyen de messages postes / seconde */
  tribune_msg_info *msg;

  /* nombre de secondes ecoulees depuis que le dernier message a ete recu */
  int nbsec_since_last_msg;

  /* date a laquelle le dernier check a ete fait
     (c'est pas redondant, je part du principe que l'horloge locale
     et celle de linuxfr ne sont pas synchronisees */
  time_t local_time_last_check;

  /* regles de reconnaissance des useragent (par regex)
     ce qui permet de leur assigner des couleurs/formes differentes */
  DLFP_trib_load_rule *rules;
} DLFP_tribune;

typedef struct _DLFP_comment {
  int news_id;
  int com_id;
  int nb_answers;
  int old;
  int modified;
  struct _DLFP_comment *next;
} DLFP_comment;

typedef struct _DLFP_message {
  int mid;
  int unreaded, tooold;
  struct _DLFP_message *next;
} DLFP_message;

typedef struct _DLFP {
  int updated;
  DLFP_news *news;
  DLFP_tribune tribune;
  DLFP_comment *com;
  DLFP_message *msg;

  int xp, xp_old, xp_clign_cnt; /* cnt = -1 -> arrete, sinon il est decremente au fur et a mesure du clignotement 
				   il est active quand on detecte un changement dans les xp pendant la 
				   lecture de myposts.php3
				*/

  char *fortune; /* la fortune recuperee sur myposts.php3 */
  float CPU;     /* la charge cpu recuperee sur myposts.php3 */
  int votes_max, votes_cur;
} DLFP;

typedef enum {OFF=0, BLUE=1, GREENLIGHT=2, YELLOW=3, VIOLET=4, CYAN=5, RED=6, GREEN=7,BIGREDLIGHT=7, BIGRED=8} LedColor;

typedef struct _Led {
  LedColor coul[3];
  int delay[3];
  
  int xpos, ypos;

  int cnt, colcnt;
  
} Led;

typedef struct Leds {
  Led led[5];
  Pixmap pixmap, mask;
} Leds;


typedef struct _SymboleDef {
  char *s[5];
  char *name;
} SymboleDef;


#define NB_SYMBOLES 16


/* et attention, on peut faire de l'antialiasing (hum..)
   # -> le pixel est éclairci (rgb*5/4)
   X -> le pixel est a la couleur complete
   x -> divise le rgb par 2
   : -> divise par 3
   . -> divise par 4
   c'est-y-pas cool ?
*/
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
				      "X####"},"carre"},
				    {{" ::: ",
				      ":X##X",
				      ":# :#",
				      ":#::#",
				      " X##."},"trou"},
				    {{"  x  ",
				      " :X: ",
				      " XXX ",
				      ":XXX:",
				      "XXXXX"},"trianglebas"},
				    {{"XXXXX",
				      ":XXX:",
				      " XXX ",
				      " :X: ",
				      "  x  "},"triangle"},
				    {{"X:   ",
				      "XXX: ",
				      "XXXXX",
				      "XXX: ",
				      "X:   "},"trianglegauche"},
				    {{"   :X",
				      " :XXX",
				      "XXXXX",
				      " :XXX",
				      "   XX"},"triangledroite"},
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
				      "X. .X"},"croix"},
				    {{"x    ",
				      "xX   ",
				      "xXX  ",
				      "xXXX ",
				      "X####"},"coin11"},
				    {{"    X",
				      "   x#",
				      "  xX#",
				      " xXX#",
				      "X####"},"coin12"},
				    {{"xxxxX",
				      "xXX# ",
				      "xX#  ",
				      "x#   ",
				      "X    "},"coin21"},
				    {{"xxxxX",
				      " XXX#",
				      "  XX#",
				      "   X#",
				      "    X"},"coin22"},
				    {{"  X  ",
				      "  X  ",
				      "XXXXX",
				      "  X  ",
				      "  X  "},"plus"},
				    {{"  x  ",
				      " xXX ",
				      "xXXX#",
				      " XX# ",
				      "  #  "}, "losange"}};
#endif

#define DOCK_WIN(d) ((Prefs.use_iconwin ? (d)->iconwin : (d)->win))


typedef struct _TL_item {
  int id;
  DLFP_trib_load_rule *tatouage; /* un peu redondant, on pourrait se contenter de id */
} TL_item;

typedef struct _Balloon Balloon;
typedef struct _EditW EditW;
typedef struct _MsgBox MsgBox;
typedef struct _Pinnipede Pinnipede;

#define DOCK_FIXED_FONT "-*-fixed-*--10-*-iso8859-1"
#define DOCK_FIXED_FONT_W 6

typedef struct _Dock {
  Pixmap pix_porte, mask_porte_haut, mask_porte_bas;
  Leds leds;
  /* le pixmap du load de la tribune (il n'est regenere que
     quand la tribune a ete modifie, c'est quand meme plus cool
     que le faire a chaque refresh_dock */
  Pixmap pix_tribune_load;

  XFontStruct *fixed_font;
  
  DLFP *dlfp; /* toutes les données !! */

  /*
    le message defilant en haut 
  */
  int nb_newstitles;
  unsigned char *newstitles;
  /* permet de lier un caractere du message a l'ID d'une news */
  int *newstitles_id;
  int newstitles_pos, newstitles_char_dec;

  /* ouverture,fermeture,enfoncage de bouton..*/
  enum {OPENING, CLOSING, OPENED, CLOSED} door_state;
  int door_state_step;

  /* press_flag : 0: pas presse, 
     +1 en cours d'enfoncements, -1 en cours de desenfoncement
     press_state: niveau d'enfoncement (max=5)
  */
  int red_button_press_flag, red_button_press_state;

  int post_anonyme;

  /* le message a caractere informatif en bas */
  unsigned char *msginfo;

  /* pointe vers la structure tl_item si la souris
     est au dessus d'un message dans la zone de tribune_load
     (NULL sinon)
  */
  TL_item *tl_item_survol;
  int tl_item_clicked;
  /* si non nul, c'est le compteur de defilement*/
  int msginfo_defil;

  int flag_survol_trollo; /* qd la souris est audessus du trollometre on affiche sa valeur */
  int flag_survol_led1; /* qd la souris est audessus de la led1, on affiche la vitesse de defilement du trolloscope */

  /* position du pointeur & comptage du nombre de millisecondes depuis le dernier mvt */
  int mouse_x, mouse_y, mouse_cnt;
  Window mouse_win;

  /* InputMethod -> pour gerer les carac. acceptues sur 
     clavier qwerty, par exemple.. */
  XIM input_method;

  Atom atom_WM_DELETE_WINDOW; // oh les bon gros atomes
  Atom atom_WM_PROTOCOLS;

  /* si non nul, on voit l'id du msg designe par tl_item_clicked, 
     --> active par un click sur un symbole dans tribune_load 
    (contient l'id, ce n'est pas un flag 0/1) */
  int view_id_in_newstitles;
  int view_id_timer_cnt;

  char tribune_time[6];

  /* nul => update non demandée
     1 => update immediate
     2 => update dans 40millesecondes
     3 =>  "       "  80 "
     etc...     
  */
  volatile int tribune_update_request;
  volatile int tribune_updatable;
  volatile int news_update_request;
  volatile int coin_coin_request;

  Cursor trib_load_cursor;
  int flag_trib_load_cursor;

  GC NormalGC, or_GC, and_GC;
  Display *display;

  Window rootwin;

  Window iconwin,win;
  //  Window used_win; /* designe soit iconwin, soit win selon que l'option '-w' a ete utilisee ou non */


  Window msgwin;
  Pixmap pix_msgwin;
  int msgwin_visible;
  GC msgwin_GC;

  /* les trois couleurs de base du dock */
  unsigned long bg_pixel,light_pixel,dark_pixel;

  int screennum;
  Pixmap coinpix, coin_pixmask;
  Pixmap clockpix, clock_pixmask;
  Pixmap led, month, date, weekday;

  RGBAContext *rgba_context;

  Newswin* newswin;

  Balloon *balloon;

  EditW *editw;

  Pinnipede *pinnipede;

  MsgBox *msgbox;

  unsigned char coin_coin_message[MESSAGE_MAX_LEN+1];
  unsigned char real_coin_coin_message[MESSAGE_MAX_LEN+1];
  unsigned char coin_coin_useragent[USERAGENT_MAX_LEN+1];
  unsigned char real_coin_coin_useragent[USERAGENT_MAX_LEN+1];

  int trolloscope_speed; /* vitesse de defilement du trolloscope (1,2,4 ou 8), defaut:2 */

  struct {
    int newswin_used;
    int palmipede_used;
    int pinnipede_used;

    int last_sig_is_usr1;
  } discretion_saved_state;

  int horloge_mode;

  float trib_trollo_rate, trib_trollo_score;
} Dock;


/* wmcoincoin.c */
void open_url(const unsigned char *url, int balloon_x, int balloon_y, int browser_num);
char* http_transfert(char *URL);
char *fget_line(char *s, int n, FILE *f);
//unsigned long pixel_color(Dock *dock, unsigned char r, unsigned char g, unsigned char b);
void dock_get_icon_pos(Dock *dock, int *iconx, int *icony);
/* renvoie 1 si le bouton rouge a ete suffisament enfonce */
int dock_red_button_check(Dock *dock);
//int flush_expose(Window w);
void block_sigalrm(int bloque);

/* picohtml.c */
void picohtml_parse(Dock *dock, PicoHtml *ph, const char *buff, int width);
void picohtml_gettxtextent(PicoHtml *ph, int *width, int *height);
void picohtml_render(Dock *dock, PicoHtml *ph, Drawable d, GC gc, int x, int y);
void picohtml_freetxt(PicoHtml *ph);
int  picohtml_isempty(PicoHtml *ph);
void picohtml_set_parag_indent(PicoHtml *ph, int parag_indent);
void picohtml_set_parag_skip(PicoHtml *ph, float parag_skip);
void picohtml_set_line_skip(PicoHtml *ph, float line_skip);
void picohtml_set_tabul_skip(PicoHtml *ph, int tabul_skip);
XFontStruct *picohtml_get_fn_base(PicoHtml *ph);
XFontStruct *picohtml_get_fn_bold(PicoHtml *ph);
PicoHtml *picohtml_create(Dock *dock, char *base_family, int base_size, int white_txt);
void picohtml_destroy(Display *display, PicoHtml *ph);
void picohtml_set_default_pixel_color(PicoHtml *ph, unsigned long pix);

/* coincoin_tribune.c */
tribune_msg_info *tribune_find_id(const DLFP_tribune *trib, int id);
tribune_msg_info *tribune_find_previous(const DLFP_tribune *trib, tribune_msg_info *mi);
void tribune_tatouage(DLFP_tribune *trib, tribune_msg_info *it);
/* renvoie l'age du message, en secondes */
time_t tribune_get_msg_age(const DLFP_tribune *trib, const tribune_msg_info *it);
/* renvoie l'estimation de l'heure actuelle sur la tribune -- en MINUTES */
time_t tribune_get_time_now(const DLFP_tribune *trib);
void tribune_frequentation(const DLFP_tribune *trib, int nb_minutes, int *ua_cnt, int *msg_cnt);
void dlfp_tribune_get_trollo_rate(const DLFP_tribune *trib, float *trate, float *tscore);
void dlfp_updatetribune(DLFP *dlfp);

/* coincoin_news.c */
void dlfp_updatenews(DLFP *dlfp);
DLFP *dlfp_create();
int dlfp_delete_news(DLFP *dlfp, int id);
DLFP_news *dlfp_insert_news(DLFP *dlfp);
DLFP_news *dlfp_find_news_id(DLFP *dlfp, int id);
DLFP_news *dlfp_find_prev_news(DLFP *dlfp, int id);
DLFP_news *dlfp_find_next_news(DLFP *dlfp, int id);
int dlfp_count_news(DLFP *dlfp);
DLFP_comment *dlfp_yc_find_modified(DLFP *dlfp, DLFP_comment *prev);
void dlfp_yc_clear_modified(DLFP *dlfp);
void dlfp_yc_update_comments(DLFP *dlfp);
DLFP_message *dlfp_msg_find_unreaded(DLFP *dlfp);
void dlfp_msg_update_messages(DLFP *dlfp);

/* newswin.c */
void newswin_show(Dock *dock, DLFP *dlfp, int id);
void newswin_unmap(Dock *dock);
void newswin_dispatch_event(Dock *dock, DLFP *dlfp, XEvent *event);
void newswin_build(Dock *dock);
int newswin_is_used(const Dock *dock);
Window newswin_get_window(const Dock *dock);
int newswin_get_xpos(const Dock *dock);
int newswin_get_ypos(const Dock *dock);
void newswin_draw(Dock *dock);
void newswin_update_content(Dock *dock, DLFP *dlfp, int reset_decal);
void newswin_update_info(Dock *dock, DLFP *dlfp, int mx, int my);
/* 
   renvoie l'id de la ieme news non encore lue
   (-1 si elle n'existe pas)
*/
int dlfp_unreaded_news_id(DLFP *dlfp, int i);
void dlfp_unset_unreaded_news(DLFP *dlfp);

/* editwin.c (le palmipede) */
void editw_show(Dock *dock, EditW *ew, int useragent_mode);
void editw_hide(Dock *dock, EditW *ew); /* rentrer le palmipede */
void editw_unmap(Dock *dock, EditW *ew); /* cacher immediatement le palmipede */
EditW *editw_build(Dock *dock);
void editw_select_buff(Dock *dock, EditW *ew, int user_agent_mode);
void editw_set_kbfocus(Dock *dock, EditW *ew, int get_it);
void editw_dispatch_event(Dock *dock, EditW *ew, XEvent *event);
Window editw_get_win(EditW *ew);
void editw_action(Dock *dock, EditW *ew);
int editw_ismapped(EditW *ew);
void editw_balloon_test(Dock *dock, EditW *ew, int x, int y);
void editw_refresh(Dock *dock, EditW *ew);
int editw_insert_string(EditW *ew, const unsigned char *s); /* utilise par le pinnipede */
void editw_move_end_of_line(EditW *ew, int shift_move); /* utilise par le pinnipede */

/* deux fonction d'utilite generale, en depis de leur nom */
void editw_cb_copy(Dock *dock, Window win, const char *text, int len);
void editw_cb_handle_selectionrequest(Dock *dock, XSelectionRequestEvent *rq);



/* balloon.c */
void balloon_build(Dock *dock);
void balloon_hide(Dock *dock);
void balloon_show(Dock *dock, int x, int y, int h, int w, const char *text, int bwidth);
void balloon_show_with_image(Dock *dock, int x, int y, int h, int w, const char *text, int bwidth, Pixmap image, int img_w, int img_h);
int balloon_ismapped(Dock *dock);
void balloon_check_event(Dock *dock, XEvent *event);

int balloon_test(Dock *dock, int x, int y, int win_xpos, int win_ypos, int bcnt, int bx, int by, int bw, int bh, const char *btxt);
int balloon_test_with_image(Dock *dock, int x, int y, int winx, int winy, int bcnt, int bx, int by, int bw, int bh, const char *btxt, Pixmap image, int img_w, int img_h);

/* msgbox.c */
void msgbox_dispatch_event(Dock *dock, XEvent *event);
Window msgbox_get_win(Dock *dock);
int msgbox_ismapped(Dock *dock);
void msgbox_hide(Dock *dock);
void msgbox_show(Dock *dock, char *text);
void msgbox_show_modal(Dock *dock, char *text);
void msgbox_build(Dock *dock);

/* pinnipede.c */
void pp_build(Dock *dock);
void pp_show(Dock *dock, DLFP_tribune *trib);
void pp_unmap(Dock *dock);
int pp_ismapped(Dock *dock);
void pp_dispatch_event(Dock *dock, DLFP_tribune *trib, XEvent *event);
void pp_minib_dispatch_event(Dock *dock, DLFP_tribune *trib, XEvent *event);
Window pp_get_win(Dock *dock);
void pp_check_tribune_updated(Dock *dock, DLFP_tribune *trib);
void pp_set_tribune_updated(Dock *dock);

/* troll_detector.c */
void troll_detector(tribune_msg_info *mi);


#endif
