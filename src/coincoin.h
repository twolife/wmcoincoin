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
#include "http.h"
#include "global.h"
#include "raster.h"
#include "coin_util.h"
#include "myprintf.h"

#ifndef WMCCDATADIR
#define WMCCDATADIR "."
#endif

/* tentative de compilation avec _XOPEN_SOURCE, y'a plein de warnings */
#ifdef _XOPEN_SOURCE
#ifndef _GNU_SOURCE
int strcasecmp(const char *s1, const char *s2);
int strncasecmp(const char *s1, const char *s2, size_t n);
char *strdup(const char *s);
int snprintf(char *str, size_t size, const char *format, ...);
void usleep(unsigned long usec);
#endif
#endif

#define APPNAME "wmcoincoin"

typedef struct _PicoHtmlItem PicoHtmlItem;
typedef struct _PicoHtml PicoHtml;

typedef long long int64;
typedef unsigned long long uint64;

typedef struct _Newswin Newswin;
typedef struct _Comment Comment;
typedef struct _News News;
typedef struct _Message Message;
typedef struct _Board Board;
typedef struct _Boards Boards;
typedef struct _Site Site;
typedef struct _SiteList SiteList;

struct _News {
  unsigned char *titre;
  unsigned char *txt;
  unsigned char *auteur;
  unsigned char *topic;
  unsigned char *mail;
  unsigned char *url; /* tronquee (cad moins le http://, le port, et le ,0,-1,7.html) */

  char date[11];
  int heure; /* en nombre de minutes depuis minuit */
  id_type id;
  int nb_comment; /* pas tr�s utile... */
  int dl_nb_tries; /* nombre d'essais de d/l du texte de la news */
  struct _News *next;
  Site *site;
};

#define trollo_log_extent 5 /* minutes */

/* #define TRIBUNE_MAX_MSG 400 */ /* nb max de messages gard�s en m�moire */

/* la tribune load affiche les messages sur les 15 dernieres minutes */
#define TROLLOSCOPE_NB_MINUTES 11
/* sur une minute, elle peut stocker 5 messages simultanes */
#define TROLLOSCOPE_MAX_SIMUL 5

#define TROLLOSCOPE_WIDTH TROLLOSCOPE_NB_MINUTES*5
/* le -2 c fait expres -> quand il y a + de 5 msg dans
   la meme minute, on voit le 5eme sans pouvoir l'identifier vraiment
   c mieux que de prendre une hauteur de exactement 4 formes */
#define TROLLOSCOPE_HEIGHT TROLLOSCOPE_MAX_SIMUL*5-4
#define TROLLOSCOPE_X 4
#define TROLLOSCOPE_Y 25


struct _Comment {
  int news_id;
  int com_id;
  int nb_answers;
  int old;
  int modified;
  struct _Comment *next;
  Site *site;
};

struct _Message {
  int mid;
  int unreaded, tooold;
  struct _Message *next;
  Site *site;
};

#define MINIUA_SZ 20
typedef struct _MiniUA {
  unsigned char R,G,B;
  unsigned char symb; /* numero du symbole */
  char name[MINIUA_SZ];
} MiniUA;

typedef struct _board_msg_info board_msg_info;

typedef struct _board_msg_ref {
  unsigned int h:5;
  unsigned int m:8;
  int s:9;    /* si positif, les secondes sont indiqu�es dans la ref */
  int num:5;  /* si positif, le sous-num�ro (pour les post multiples dans une m�me seconde) est indiqu� dans la ref */
  unsigned int nbmi:6; /* nb de messages consecutifs point�s (g�n�ralement 1 sauf si la ref d�signe plusieurs messages, forcement consecutifs)
			  0 => ref non determinee (ipot, mauvaise reference, le message n'existe plus..)
			  
			  _ATTENTION_: quand on boucle sur cette serie de message, il faut
			  utiliser le pointeur 'intra-site' mi->next et pas 
			  le pointeur 'cross-sites' mi->g_next
		       */
  board_msg_info *mi; /* non malloc�, forc�ment, et mis � jour lors de la destruction des messages trop vieux 
			  peut �tre NULL (ipot, message effac�..)

			  remarque: les refs DOIVENT �tre vers des messages du passe (ipot interdit)
			  c'est necessaire au bon fonctionnement de la construction des threads du pinnipede
			*/
} board_msg_ref;



/* ne pas d�placer ce genre de structure apr�s son allocation
   --> pas de realloc la dessus !
   (� cause des board_msg_ref qui les relient entre elles)
*/
struct _board_msg_info {
  id_type id;
  time_t timestamp;
  signed char sub_timestamp; /* sous numerotation quand plusieurs posts ont le m�me timestamp 
			 (-1 -> pas (encore) d'autre post avec le meme tstamp)
		       */
  /* (year-2000)|month|day|h|m|s */
  signed char hmsf[4]; /* heure, minute, seconde + flag d'affichage des secondes (1 == secondes necessaires)  */
  char *useragent; /* pointe dans la zone m�moire allou�e pour board_msg_info -> ne pas faire de free(useragent) !!! */
  char *msg; /* pointe dans la zone m�moire allou�e pour board_msg_info -> ne pas faire de free(msg) !!! */
  char *login; /* non malloc�, comme useragent, msg etc..*/
  struct _board_msg_info *next;

  /* on a aussi une structure d'arbre qui se colle l� dessus
     (la structure de liste tri�e reste pour des raisons historiques ..) */
  struct _board_msg_info *left;
  struct _board_msg_info *right;

  /*
    describes how the message will appear in the trolloscope 
     also contains a hopefully useful short name describing the useragent
  */
  MiniUA miniua;

  /* le niveau de trollitude du post (cf troll_detector.c) */
  int troll_score BITFIELD(13); 
  int is_my_message BITFIELD(1);
  int is_answer_to_me BITFIELD(1);

  /* utilis� par board_key_list_test_thread pour �viter de r�curser comme un ouf */
  int bidouille_qui_pue BITFIELD(1); 
  int in_boitakon BITFIELD(1); /* le niveau ultime de la plopification */
  short nb_refs BITFIELD(15);
  board_msg_ref *refs; /* pointeur malloc�, indique la liste des messages point�s par celui ci */

  /* pointeurs inter-sites: le point de depart est dans la structure boards,
     et ils permettent de parcourir les messages de tous les sites, dans l'ordre */
  struct _board_msg_info *g_next, *g_prev;
};

struct _Board {
  unsigned char last_post_time[5];

  int last_post_timestamp; /* en secondes */
  int last_post_id;
  int last_post_id_prev;

  /* log des 'last_post_id' au cours des 'nb_trollo_log' derniers check de la board,
     utilise pour calculer le nb moyen de messages postes / seconde */
  board_msg_info *msg;

  /* valeur du wmcc_tic_cnt lors du dernier check positif */
  int wmcc_tic_cnt_last_check;

  /* nombre de secondes ecoulees depuis que le dernier message a ete recu */
  int nbsec_since_last_msg;

  /* date a laquelle le dernier check a ete fait
     (c'est pas redondant, je part du principe que l'horloge locale
     et celle de linuxfr ne sont pas synchronisees */
  time_t local_time_last_check, local_time_last_check_old, local_time_last_check_end;

  /* fourchette d'�cart de temps */
  time_t time_shift_min, time_shift_max;

  time_t time_shift; /* time@localhost - time@remotehost */

  /* regles de reconnaissance des useragent (par regex)
     ce qui permet de leur assigner des couleurs/formes differentes */
  //  board_load_rule *rules;

  int just_posted_anonymous; /* positionn� si on vient juste d'envoyer un message en anonyme
				(pour aider la reconnaissance de nos messages) */
  
  board_msg_info *mi_tree_root; /* rooh un arbre binaire ..
				     c'est utilis� par board_find_id */
  char *last_modified; /* pour les requetes http */
  struct _Site *site;
  Boards *boards; /* pointeur vers la structure multi-site 
		     il est juste l� par commodit�
		   */
  int flag_answer_to_me;

  /* stocke le useragent (potentiellement modifi�) */
  unsigned char coin_coin_useragent[USERAGENT_MAXMAX_LEN+1];

  /* compteurs mis � jour dans Net_loop (25 fois/sec) */
  int board_refresh_cnt, board_refresh_delay;

  volatile int auto_refresh; /* refreshs auto activ� desactiv� par la ptite croix en bas � droite du tab */
};

typedef struct _SiteNameHash {
  int hash;
  int sid;
} SiteNameHash;

struct _Boards {
  board_msg_info *first;
  board_msg_info *last;
  Board *btab[MAX_SITES];

  int nb_aliases;
  SiteNameHash *aliases;
};

struct _Site {
  int news_updated;

  News *news;
  Board *board;
  Comment *com;
  Message *msg;

  char *news_backend_last_modified; /* a init en NULL */
  int news_backend_dl_cnt; 
  char *messages_last_modified; /* a init en NULL */
  int messages_dl_cnt;
  char *comments_last_modified;
  int comments_dl_cnt;

#define MAX_NEWS_LUES 100
  int newslues[MAX_NEWS_LUES];
  int nb_newslues;
  int newslues_uptodate;

  /* les xp & fortunes & votes ne sont valides que si monitor_com != 0 */
  int xp, xp_old;
  /* xp_change_flag est active quand on detecte un changement dans les xp pendant la 
     lecture de myposts.php3
     (raz d�s que la flamometre commence � clignoter) */
  int xp_change_flag; 

  /* ce flag d�clenche le flamometre 
     (et il est remis � z�ro d�s que le flamometre a �t� d�clench�) */
  int comment_change_flag;   

  char *fortune; /* la fortune recuperee sur myposts.php3 */
  float CPU;     /* la charge cpu recuperee sur myposts.php3 */
  int votes_max, votes_cur;

  /* compteurs mis � jour dans Net_loop (25 fois/sec) */
  int news_refresh_cnt, news_refresh_delay;

  SitePrefs *prefs;
  struct _Site *next;

  int site_id; /*
		 un num�ro unique au site,
		  susceptible de changer a chaque rechargement des prefs
		  (ajout ou suppression de sites)
		  (ce numero est compris entre 0 et MAX_SITES-1, et represente l'indice
		  du pointeur 'prefs' dans la structure globale Prefs.site[] )
	       */
};

struct _SiteList {
  Site *list;
  Boards *boards;
};



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



#define DOCK_WIN(d) ((Prefs.use_iconwin ? (d)->iconwin : (d)->win))


typedef struct _TL_item {
  id_type id;
  unsigned char R,G,B,symb;
} TL_item;

typedef struct _Balloon Balloon;
typedef struct _EditW EditW;
typedef struct _MsgBox MsgBox;
typedef struct _Pinnipede Pinnipede;

#define DOCK_FIXED_FONT "-*-fixed-*--10-*"
#define DOCK_FIXED_FONT_W 6


#define MAX_NEWSTITLES_LEN 512 /* taille de dock->newstitles */
#define MAX_MSGINFO_LEN 300 /* taille de dock->msginfo */

/* periode du clignotement (25-> 1 par sec) */
#define FLAMOMETRE_COMMENT_CLIGN_SPEED 32
#define FLAMOMETRE_XP_CLIGN_SPEED      32
#define FLAMOMETRE_TRIB_CLIGN_SPEED    16
/* duree de clignotement (en secondes) */
#define FLAMOMETRE_COMMENT_DUREE 3600000  /* c'est long, tr�s long */
#define FLAMOMETRE_XP_DUREE      900      /* 1/4 d'heure */
#define FLAMOMETRE_TRIB_DUREE    15

typedef struct _Dock {
  Pixmap pix_porte, mask_porte_haut, mask_porte_bas;
  Leds leds;
  /* le pixmap du load de la board (il n'est regenere que
     quand la tribune a ete modifie, c'est quand meme plus cool
     que le faire a chaque refresh_dock */
  Pixmap pix_trolloscope;

  XFontStruct *fixed_font;
  
  SiteList *sites; /* toutes les donn�es !! */

  /*
    le message defilant en haut 
  */
  int nb_newstitles;
  unsigned char *newstitles;
  /* permet de lier un caractere du message a l'ID d'une news */
  id_type *newstitles_id;
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
     est au dessus d'un message dans la zone de trolloscope
     (NULL sinon)
  */
  TL_item *tl_item_survol;
  int tl_item_clicked;

  /* �a c'est le tableau du trolloscope */
  TL_item **trolloscope;
  /* trolloscope_resolution: 5(faible) ou 2(moyenne) ou 1(hires!)*/
  int trolloscope_resolution; /* de retour, � la demande de monsieur 'The Original Palmip�de' */
  
  struct {
    int xp_change_decnt;
    int comment_change_decnt;
    int board_answer_decnt;
  } flamometre;

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

  Atom atom_WM_DELETE_WINDOW; /* oh les bon gros atomes  */
  Atom atom_WM_PROTOCOLS;

  /* si non nul, on voit l'id du msg designe par tl_item_clicked, 
     --> active par un click sur un symbole dans trolloscope 
    (contient l'id, ce n'est pas un flag 0/1) */
  id_type view_id_in_newstitles;
  int view_id_timer_cnt;

  unsigned char coin_coin_message[MESSAGE_MAXMAX_LEN+1];
  int coin_coin_site_id;
  int coin_coin_sent_decnt;

  Cursor trib_load_cursor;
  int flag_trib_load_cursor;

  GC NormalGC, or_GC, and_GC;
  Display *display;

  Window rootwin;

  Window iconwin,win;

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

  int trolloscope_speed; /* vitesse de defilement du trolloscope (1,2,4 ou 8), defaut:2 */

  struct {
    int newswin_used;
    int palmipede_used;
    int pinnipede_used;

    int last_sig_is_usr1;
  } discretion_saved_state;

  int horloge_mode;

  float trib_trollo_rate, trib_trollo_score;

  Pixmap wm_icon_pix, wm_icon_mask; /* icone utilis�e par le windowmanager (pour le pinnipede et la fenetre des news) */
  pid_t wmccc_pid;
} Dock;

typedef enum { Q_PREFS_UPDATE, Q_BOARD_POST, Q_BOARD_UPDATE, Q_COMMENTS_UPDATE, Q_MESSAGES_UPDATE, 
	       Q_NEWSLST_UPDATE, Q_NEWSTXT_UPDATE } ccqueue_elt_type;
typedef struct _ccqueue_elt {
  ccqueue_elt_type what;
  int  sid;

  char *ua;
  char *msg;
  int  nid;
  struct _ccqueue_elt *next;
} ccqueue_elt;


/* wmcoincoin.c */
void open_url(const unsigned char *url, int balloon_x, int balloon_y, int browser_num);
void wmcc_init_http_request(HttpRequest *r, SitePrefs *sp, char *url_path);
void wmcc_init_http_request_with_cookie(HttpRequest *r, SitePrefs *sp, char *url_path);
void block_sigalrm(int bloque);
int launch_wmccc(Dock *dock);
void exec_coin_coin(Dock *dock, int sid, const char *ua, const char *msg);
void wmcc_save_or_restore_state(Dock *dock, int do_restore);

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

/* cc_queue.c */
void ccqueue_build();
void ccqueue_push_prefs_update(int whatfile);
void ccqueue_push_board_post(int sid, char *ua, char *msg);
void ccqueue_push_board_update(int sid);
void ccqueue_push_comments_update(int sid);
void ccqueue_push_messages_update(int sid);
void ccqueue_push_newslst_update(int sid);
void ccqueue_push_newstxt_update(int sid, int nid);
int ccqueue_state();
void ccqueue_print();
const ccqueue_elt *ccqueue_doing_what();
ccqueue_elt* ccqueue_find_next(ccqueue_elt_type what, int sid, ccqueue_elt *q);
ccqueue_elt* ccqueue_find(ccqueue_elt_type what, int sid);
void ccqueue_loop(Dock *dock);

/* useragents_file.c */
int useragents_file_reread(Dock *dock, Site*dlfp);
int useragents_file_read_initial(Dock *dock, Site*dlfp);


/* news.c */
void site_newslues_add(Site *site, int lid);
int site_newslues_find(Site *site, int lid);
void site_news_restore_state(Site *site);
void site_news_save_state(Site *site);
void site_news_dl_and_update(Site* dlfp);
int site_news_update_txt(Site *site, id_type id);
Site* site_news_create();
void site_news_destroy(Site *dlfp);
News *site_news_find_id(Site *dlfp, id_type id);
News *site_news_find_prev(Site *dlfp, id_type id);
News *site_news_find_next(Site *dlfp, id_type id);
void site_news_unset_unreaded(Site *site);
int site_news_count(Site *dlfp);
FILE * open_site_file(Site *site, char *base_name);
/* comments.c */
void site_yc_printf_comments(Site *dlfp);
Comment *site_yc_find_modified(Site *dlfp);
void site_yc_clear_modified(Site *dlfp);
void site_yc_dl_and_update(Site *dlfp);
void site_com_destroy(Site *site);
/* messages.c */
void site_msg_printf_messages(Site *dlfp);
Message *site_msg_find_unreaded(Site *dlfp);
void site_msg_dl_and_update(Site *dlfp);
void site_msg_destroy(Site *site);
void site_msg_save_state(Site *site);
/* palmipede.c  */
void editw_show(Dock *dock, SitePrefs *sp, int useragent_mode);
void editw_hide(Dock *dock, EditW *ew); /* rentrer le palmipede */
void editw_unmap(Dock *dock, EditW *ew); /* cacher immediatement le palmipede */
void editw_reload_colors(Dock *dock, EditW *ew);
void editw_build(Dock *dock);
void editw_rebuild(Dock *dock);
void editw_set_kbfocus(Dock *dock, EditW *ew, int get_it);
void editw_dispatch_event(Dock *dock, EditW *ew, XEvent *event);
Window editw_get_win(EditW *ew);
int editw_get_site_id(Dock *dock);
void editw_action(Dock *dock, EditW *ew);
int editw_ismapped(EditW *ew);
void editw_balloon_test(Dock *dock, EditW *ew, int x, int y);
void editw_refresh(Dock *dock, EditW *ew);
void editw_erase(EditW *ew);
int editw_insert_string(EditW *ew, const unsigned char *s); /* utilise par le pinnipede */
void editw_move_end_of_line(EditW *ew, int shift_move); /* utilise par le pinnipede */

/* deux fonction d'utilite generale, en depis de leur nom */
void editw_cb_copy(Dock *dock, Window win, const char *text, int len);
void editw_cb_handle_selectionrequest(Dock *dock, XSelectionRequestEvent *rq);



/* balloon.c */
void balloon_build(Dock *dock);
void balloon_destroy(Dock *dock);
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
void pp_rebuild(Dock *dock);
void pp_destroy(Dock *dock);
void pp_show(Dock *dock);
void pp_unmap(Dock *dock);
int pp_ismapped(Dock *dock);
void pp_dispatch_event(Dock *dock, XEvent *event);
void pp_minib_dispatch_event(Dock *dock, Board *trib, XEvent *event);
Window pp_get_win(Dock *dock);
void pp_check_board_updated(Dock *dock);
void pp_animate(Dock *dock);
void pp_set_board_updated(Dock *dock);
void pp_set_prefs_colors(Dock *dock);
void pp_check_balloons(Dock *dock, int x, int y);
void pp_set_ua_filter(Dock *dock, char *ua);
void pp_set_word_filter(Dock *dock, char *word);
void pp_save_state(Dock *dock, FILE *f);
void pp_restore_state(Dock *dock, FILE *f);
Site *pp_tabs_get_main_site(Dock *dock);
void pp_set_download_info(char *site, char *what);

/* prefs_gestion.c */
char *check_install_data_file(char *data_file_name, char *dot_wmcc_file_name);
void wmcc_prefs_initialize(int argc, char **argv, GeneralPrefs *p);
void wmcc_prefs_relecture(Dock *dock, int whichfile);
char *get_wmcc_options_filename();
char *get_wmcc_tmp_options_filename();

/* troll_detector.c */
void troll_detector(board_msg_info *mi);


/* board.c */
Board *board_create(Site *site, Boards *boards);
void board_tatouage(Board *trib, board_msg_info *it);
/* renvoie l'age du message, en secondes */
time_t board_get_msg_age(const Board *trib, const board_msg_info *it);
void board_frequentation(const Board *trib, int nb_minutes, int *ua_cnt, int *msg_cnt, int *my_msg_cnt);
void board_get_trollo_rate(const Board *trib, float *trate, float *tscore);
void boards_update_boitakon(Boards *boards);
void board_update(Board* board);
void board_destroy(Board *board);
#endif
