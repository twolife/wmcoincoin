#ifndef _PINNIPEDE_H
#define _PINNIPEDE_H

#include <libintl.h>
#define _(String) gettext (String)

#include "coincoin.h"
#include "board_util.h"
#include "time.h"
#include "scrollcoin.h"
#include "coin_xutil.h"

/* chuis con, les bitfields c pas pour les chiens */
#define PWATTR_BD 1
#define PWATTR_IT 2
#define PWATTR_U  4
#define PWATTR_S  8
#define PWATTR_LNK 16
#define PWATTR_TSTAMP 32
//#define PWATTR_UA 64
#define PWATTR_NICK 128
#define PWATTR_REF 256 /* reference vers un autre post */
#define PWATTR_TMP_EMPH 512
#define PWATTR_HAS_INITIAL_SPACE 1024 /* indique si un espace doit etre insere entre ce mot et le
			     suivant (utilise par justif */
#define PWATTR_TROLLSCORE 2048
#define PWATTR_LOGIN 4096
#define PWATTR_TT  8192

typedef struct _PostVisual PostVisual;
typedef struct _PostWord PostWord;

/* une liste de mots avec leurs attributs */
struct _PostWord {
  unsigned char *w; /* non malloc�, stocke dans la m�me zone que cette structure */
  unsigned char *attr_s;
  unsigned short attr;
  short xpos, xwidth, ligne;
  struct _PostWord *next;
  struct _PostVisual *parent;
};

/* liste chain�e de posts */
struct _PostVisual {
  id_type id;         // message id + site id
  PostWord *first; /* la liste des mots */
  time_t tstamp;
  signed char sub_tstamp; /* sous numerotation quand plusieurs posts ont le m�me timestamp */
  int nblig BITFIELD(12); // nombre de lignes necessaire pour afficher ce message
  int ref_cnt BITFIELD(9); // compteur de references

  int is_my_message BITFIELD(1);
  int is_answer_to_me BITFIELD(1);
  int is_hilight_key BITFIELD(4);
  int is_skipped_id BITFIELD(1); /* non nul si le message (id-1) n'existe pas */
  int is_plopified BITFIELD(3);
  /* non nul si le message a �t� plopifi�
			  =1, le message apparait en gris, tags html enleves
			  =2, le message est plopifi� (mots remplac�s par plop, grouik..)
			  =3, le message est superplopifi� (message remplac� par 'plop')
			*/
  struct _PostVisual *next;
};

struct _PinnipedeFilter {
  int filter_mode;
  char *filter_name; /* contient le 'nom' du filtre) 
		      */
  char *ua;
  char *login;
  char *word;
  //  int hms[3]; /* filtre sur les ref au msg post� � l'heure indiqu�e dans hms */
  id_type *id; int nid; /* liste des id des messages affich�s dans le filtre de threads */
  int filter_boitakon;
  char visible_sites[MAX_SITES]; /* indique les sites affich�s
				    en simultan� (maj d'apr�s l'�tat des tabs du pinni) 
				    (ce filtre est tjs activ�, il ne d�pend pas de filter_mode)
				 */
};

typedef struct _PinnipedeLignesSel {
  int x0, x1;
  int trashed;
  PostWord *first_pw;
  PostWord *last_pw;
  int first_pw_pos, last_pw_pos;
} PinnipedeLignesSel;

typedef struct _PPMinib {
#define NB_MINIB 9
#define MINIB_H 10
#define MINIB_FN_W 6
#define MINIB_Y0 (pp->win_height - MINIB_H)
  enum { HELP, SCROLLBAR, /*REFRESH_TRIBUNE, REFRESH_NEWS,*/ UA, SECOND, TSCORE, FORTUNE, FILTER, PLOPIFY, TRANSPARENT } type;
  int x, y;
  int w, h;
  int clicked;
} PPMinib;
  
#define PPT_H 14

typedef struct _PinnipedeTab {
  Site *site;
  int selected;
  int x,y,w,h;
  int clicked;
} PinnipedeTab;

struct _Pinnipede {
  Window win;
  unsigned long win_bgpixel[MAX_SITES], timestamp_pixel[MAX_SITES], nick_pixel[MAX_SITES], 
    login_pixel[MAX_SITES], lnk_pixel[MAX_SITES], txt_pixel[MAX_SITES], strike_pixel[MAX_SITES], 
    trollscore_pixel[MAX_SITES],  emph_pixel, 
    popup_fgpixel, popup_bgpixel, sel_bgpixel,
    hilight_my_msg_pixel,hilight_answer_my_msg_pixel,hilight_keyword_pixel[NB_PP_KEYWORD_CATEG],
    plopify_pixel, 
    minib_pixel, minib_dark_pixel, minib_msgcnt_pixel, 
    minib_updlcnt_pixel, progress_bar_pixel;
  int mapped;
  int win_width, win_height, win_xpos, win_ypos;

  int zmsg_y1, zmsg_y2, zmsg_h; /* zone d'affichage des messages */
  XFontStruct *fn_base, *fn_it, *fn_bd, *fn_itbd, *fn_minib;
  int fn_h;
  //  Pixmap minipix;

  PostVisual *pv;

  int nb_lignes;
  PostWord **lignes;

  ScrollCoin *sc;

  PinnipedeLignesSel *lignes_sel; /* utilis� uniquement pendant les selections */
  int sel_anchor_x, sel_anchor_y;
  int sel_head_x, sel_head_y;
  int sel_l0, sel_l1;
  time_t time_sel; /* pour la deselection automatique quand il est *vraiment* temps de rafraichir le pinnipede */

  char *last_selected_text; /* stockage temporaire � usage interne au pinnipede */

  id_type id_base;
  int decal_base;
  id_type last_post_id; /* utilise pour savoir si la tribune a ete mise a jour.. */

  int colle_en_bas; /* pour savoir si on scrolle lors de nouveaux messages */

  //  int html_mode;
  int nick_mode; /* 0 : n'affiche rien, 
		    1:  affiche les useragent raccourcis, 
		    2: affiche les logins, 
		    3: affiche les deux, 
		    4: affiche useragent ou login */
  int nosec_mode; /* supprime les secondes sur les posts ou c'est possible */
  int trollscore_mode;
  int survol_hash; /* pour determiner (a peu pres) si on affcihe une nouvelle info de survol... (apprixmatif...) */
  int fortune_mode;
  int disable_plopify;

  int hilight_my_message_mode;
  int hilight_answer_to_me_mode;
  int hilight_key_mode;

  
  volatile int flag_board_updated;
  Pixmap lpix;

  PicoHtml *ph_fortune;
  int fortune_h, fortune_w;

  struct _PinnipedeFilter filter;

  Pixmap bg_pixmap;
  int transparency_mode;
  //  int selection_mode; /* non nul quand on est en train de selectionner du texte � copier dans le clipboard (en dragant avec la souris) */

  int use_minibar; /* les miniboutons sont-ils affich�s ? */
  PPMinib mb[NB_MINIB];
  int minib_pressed; /* numero du minibouton enfonce, -1 si aucun */
  int mb_min_width; /* largeur minimale demand�e par la minibar
		       (permet aux tabs de se placer avec la minibar, ou au-dessus) */
  int mb_buttonbar_width; /* largeur du bloc de boutons */
  int mb_x0;  /* position x de la minibar (non nul qd les tabs sont au m�me niveau, a gauche) */

  enum {PPT_UP, PPT_DOWN} tabs_pos; /* position des tabs */
  int nb_tabs; /* == nb de sites avec une tribune */
  PinnipedeTab *tabs; /* tableau de tabs (une par site avec tribune) */
  PinnipedeTab *active_tab;  
  int tabs_width;
};

int filter_msg_info(const board_msg_info *mi, const struct _PinnipedeFilter *filter);
int count_all_id_filtered(Boards *boards, struct _PinnipedeFilter *filter);
id_type get_nth_id_filtered(Boards *boards, struct _PinnipedeFilter *filter, int n);
id_type get_last_id_filtered(Boards *boards, struct _PinnipedeFilter *filter);
void pp_selection_unselect(Pinnipede *pp);
void pp_balloon_help(Dock *dock, int x, int y);
void pp_pv_destroy(Pinnipede *pp);
PostVisual *pp_pv_add(Pinnipede *pp, Boards *boards, id_type id);
void pp_refresh(Dock *dock, Drawable d, PostWord *pw_ref);
void pp_change_transparency_mode(Dock *dock, int on_off);
void pp_update_bg_pixmap(Dock *dock);
unsigned long pp_get_win_bgcolor(Dock *dock);
void pp_clear_win_area(Dock *dock, int x, int y, int w, int h);
//void pp_minib_initialize(Pinnipede *pp);
void pp_minib_refresh(Dock *dock);
void pp_minib_show(Dock *dock);
void pp_minib_hide(Dock *dock);
void pp_tabs_build(Dock *dock);
//void pp_tabs_set_position(Pinnipede *pp);
void pp_tabs_destroy(Pinnipede *pp);
void pp_tabs_refresh(Dock *dock);

void pp_scrollcoin_update_bounds(Dock *dock);
void pp_widgets_set_pos(Dock *dock);
void pp_widgets_refresh(Dock *dock);
int  pp_widgets_handle_button_press(Dock *dock, XButtonEvent *ev);
int  pp_widgets_handle_button_release(Dock *dock, XButtonEvent *event);
int  pp_widgets_handle_motion(Dock *dock, XMotionEvent *event);

void pp_update_fortune(Dock *dock);
void pp_refresh_fortune(Dock *dock, Drawable d);
void pp_update_content(Dock *dock, id_type id_base, int decal, int adjust, int update_scrollbar_bounds);



/* macros pour le calcul des differentes positions d'affichage des lignes */
#define LINEY0(l) (pp->zmsg_y2 - (pp->nb_lignes-l)*pp->fn_h-(pp->zmsg_h - pp->nb_lignes*pp->fn_h)/2)
#define LINEY1(l) (LINEY0(l)+pp->fn_h-1)
#define LINEBASE(l) (LINEY0(l) + pp->fn_base->ascent)

#endif
