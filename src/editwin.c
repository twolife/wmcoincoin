/*

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

 */

/*
  rcsid=$Id: editwin.c,v 1.11 2002/03/01 00:27:40 pouaite Exp $
  ChangeLog:
  $Log: editwin.c,v $
  Revision 1.11  2002/03/01 00:27:40  pouaite
  trois fois rien

  Revision 1.10  2002/02/24 22:13:57  pouaite
  modifs pour la v2.3.5 (selection, scrollcoin, plopification, bugfixes)

  Revision 1.9  2002/01/19 19:56:09  pouaite
  petits crochets pour la mise en valeur de certains messages (cf changelog)

  Revision 1.8  2002/01/13 18:19:20  pouaite
  nouvelles suggestions pour clipouille (merci jjb!)

  Revision 1.7  2002/01/12 19:03:54  pouaite
  bugfix de picohtml et raccourci altgr-e pour le symbole euro (gruik)

  Revision 1.6  2002/01/12 17:29:08  pouaite
  support de l'iso8859-15 (euro..)

  Revision 1.5  2001/12/18 12:43:37  pouaite
  ajout de l'option de la fonte des ballons d'aide (pour mr. imr !) + bugfix d'une connerie assez naze dans la gestion du nom du fichier d'options (merci glandium de me l'avoir signalé)

  Revision 1.4  2001/12/16 20:28:45  pouaite
  bugfixes divers

  Revision 1.3  2001/12/16 16:46:12  pouaite
  Clippouille joins C0IN C0IN

  Revision 1.2  2001/12/02 18:34:54  pouaite
  ajout de tags cvs Id et Log un peu partout...

*/

/*
  ceci est le palmipede editor
*/


#include <X11/X.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xatom.h>

#include <X11/Xmd.h>
#include <X11/xpm.h>
#include <X11/keysym.h>
#include <X11/cursorfont.h>
#include <X11/extensions/shape.h>
#include "coin_util.h"
#include "coincoin.h"
#include "../xpms/editwin_minib.xpm"
#include "../xpms/clippy.xpm"
#include "spell_coin.h" 

#define FN_W 6
#define FN_H 11
#define FN_BASE_H 9
#define EW_FONT "-*-fixed-*--10-*"
#define EW_NCOL 60
#define EW_NROW 4

#define EW_TXT_XBORD_WIDTH 8
#define EW_TXT_Y0 2
#define EW_TXT_WIDTH  (EW_NCOL*FN_W)
#define EW_TXT_HEIGHT (EW_NROW*FN_H)

#define EW_SHAPE_SPACEX 14
#define EW_SHAPE_SPACEY 8
#define EW_WIDTH (EW_TXT_WIDTH + 2*EW_TXT_XBORD_WIDTH)
#define EW_HEIGHT 59
#define EW_SHAPED_WIDTH (EW_WIDTH + EW_SHAPE_SPACEX)
#define EW_SHAPED_HEIGHT (EW_HEIGHT + EW_SHAPE_SPACEY)
#define EW_XBORD (ew->dock_side==RIGHT ? 0 : EW_SHAPE_SPACEX)
#define EW_TXT_X0 (EW_XBORD+EW_TXT_XBORD_WIDTH)

#define EW_MAX_TOTAL_NROW 20

#define BT_CLOSE  0
#define BT_CHANGE 1
#define BT_ITAL   2
#define BT_BOLD   3
#define BT_STRIKE 4
#define BT_UNDERLINE 5
#define BT_UNDO 6
#define BT_CLEAR 7
#define BT_DEFAULTUA 8

#define NB_MINIBT 9

#define EWC_NORMAL 0
#define EWC_LONGWORD 1
#define EWC_URL 2
#define EWC_BALISE 3
#define EWC_SPELLWORD 4
#define EWC_NBATTR 5

#ifdef SWAP
#undef SWAP
#endif

#define SWAP(a,b,t) { t tmp; tmp = (t)a; a = (t)b; b = (t)tmp; }

/* utilise pour le copier coller */
typedef CARD32 Atom32;

typedef struct _MiniBouton {
  int pix_x, pix_y;
  int x,y,w,h;
  int visible;
  int enfonce;
} MiniBouton;

typedef enum {CUTPASTE, INSERT_CHAR, DELETE_CHAR, OTHER} EditWCommandClass;

struct _EditW {
  Window win;
  Pixmap pix;
  XFontStruct *fn;
  XIC input_context;
  unsigned char *buff;

  int  buff_sz;

  int  mapped;

  int buff_len;

  int sel_anchor, sel_head; /* premier et [dernier+1] numero de char de la selection ( a classer dans l'ordre)*/
  
  int y_scroll;
  int curs_x, curs_y;

  struct {
    unsigned char *buff;
    int y_scroll;
    int curs_x, curs_y;
  } undo;

  /* pour le word-wrap */
  int row_firstchar[EW_MAX_TOTAL_NROW+1]; 
  int nrow_used;

  unsigned long win_bgpixel, dark_pixel, light_pixel;
  unsigned long txt_fgpixel[EWC_NBATTR], txt_bgpixel;
  unsigned long cur_fgpixel, cur_bgpixel;
  unsigned long sel_fgpixel, sel_bgpixel;
  unsigned long fill_bgpixel;
  char *title;
  MiniBouton mini[NB_MINIBT];
  Pixmap minipix;

  enum {NOACTION, SORTIR, RENTRER} action;
  int action_step;

  int dock_x, dock_y;
  enum {LEFT,RIGHT} dock_side;

  int win_xpos, win_ypos;

  int buff_num; /* 0 -> dock->coin_coin_message,
		   1 -> dock->coin_coin_ueragent */

  /* utilise pour le undo */
  EditWCommandClass last_command;

  Pixmap clippy_pixmap;
  int clippy_w, clippy_h;
};

#define IS_SEP(c,nc) (((c) <=' ') || ((c)=='.') || ((c)==',')  || \
                      ((c) ==';') || ((c)==':') || ((c)=='!')  || ((c)=='?') || \
                      ((nc)=='(') || ((c)==')') || \
                      ((nc)=='{') || ((c)=='}') || \
                      ((nc)=='<') || ((c)=='>'))

#define IS_SEP2(c)   (((c) <=' ') || ((c)=='.') || ((c)==',')  || \
                      ((c) ==';') || ((c)==':') || ((c)=='!')  || ((c)=='?') || \
                      ((c)=='(') || ((c)==')') || \
                      ((c)=='{') || ((c)=='}') || \
                      ((c)=='<') || ((c)=='>'))


static unsigned char *cb_buffer = NULL;

int *head,*anch;


/* (x,y) = position dans le tableau EW_NCOL x EW_NROW */
static int
editw_xy2strpos(EditW *ew, int x, int y)
{
  int i;

  if (x < 0 || x >= EW_NCOL) {
    myprintf("%<MAG> RAH ENCORE CE BUG! x=%d y=%d curs_x=%d curs_y=%d,y_scroll=%d,sel_anchor=%d,sel_head=%d\n",
	     x,y,ew->curs_x,ew->curs_y,ew->y_scroll,ew->sel_anchor,ew->sel_head);
  }
    //  assert(x >= 0 && x < EW_NCOL);
  assert(y >= 0 && (y-ew->y_scroll) < EW_NROW);
  
  i = ew->row_firstchar[y] + x;
  if (i > ew->row_firstchar[y+1]) i =  ew->row_firstchar[y+1];
  
  return i;
}

static void
editw_strpos2xy(EditW *ew, int pos, int *x, int *y)
{
  *y = 0;
  assert(pos <= ew->buff_len);

  while (ew->row_firstchar[*y+1] < pos) (*y)++;
  (*x) = pos - ew->row_firstchar[*y];

  /* cas particulier: la ligne est de longueur EW_NCOL */
  if ((*x) == EW_NCOL) {
    *x = 0;
    (*y)++;
  }
  assert(*x < EW_NCOL);
}

static void editw_adjust_yscroll(EditW *ew) {
  if (ew->curs_y >= ew->y_scroll + EW_NROW) {
    ew->y_scroll =  ew->curs_y - (EW_NROW - 1);
  } else if (ew->curs_y < ew->y_scroll) {
    ew->y_scroll = ew->curs_y;
  }
  //  printf("yscroll=%d\n",ew->y_scroll);
}

static void editw_strpos2curs(EditW *ew, int pos)
{
  editw_strpos2xy(ew, pos, &ew->curs_x, &ew->curs_y);
  editw_adjust_yscroll(ew);
}

/* effectue le word-wrap en s'assurant que le curseur désigne toujours le bon caractère */
static void
editw_wordwrap(EditW *ew)
{
  int l, i, j, curs_pos;

  ew->buff_len = strlen(ew->buff);

  /* il faudra recalculer la pos du curseur si ca wrappe */
  curs_pos = editw_xy2strpos(ew, ew->curs_x, ew->curs_y);
  curs_pos = MIN(curs_pos, ew->buff_len);
  
  i = 0;
  ew->row_firstchar[0] = i;

  i = IS_SEP('e','g');
  ew->nrow_used = 0;
  l = 1;
  for (l=1; l < EW_MAX_TOTAL_NROW+1; l++) {
    if (i < ew->buff_len || i == 0) {
      /* nb de ligne utilisees pour l'affichage (avec cas pasticulier quand
	 la chaine est vide) */
      ew->nrow_used++; 
    }
    i += EW_NCOL;
    if (i >= ew->buff_len) {
      ew->row_firstchar[l] = ew->buff_len;
    } else {
      j = i-1;
      /* on essaye de couper la ligne precedente au bon endroit,
       en cherchant un separateur parmi les 15 dernier caracteres*/
      while (!IS_SEP(ew->buff[j], ew->buff[j+1])) {
	j--;
	if (j < 0) { j = 0; break; }
	if (j < i-15) break;
      }

      ew->row_firstchar[l] = ew->buff_len;
      if (ew->buff[j]) {
	if (IS_SEP(ew->buff[j], ew->buff[j+1])) {
	  i = ew->row_firstchar[l] = j+1;
	} else {
	  ew->row_firstchar[l] = i;
	}
      }
    }
    //    printf("row_firstchar[%d] = %d\n", l, ew->row_firstchar[l]);
  }

  /* cas particulier: la derniere ligne fait exactement EW_NCOL char
     donc on rajoute une ligne vide en dessous */
  if (ew->row_firstchar[ew->nrow_used] - ew->row_firstchar[ew->nrow_used-1] == EW_NCOL &&
      ew->nrow_used < EW_MAX_TOTAL_NROW) {
    assert(ew->buff[ew->row_firstchar[ew->nrow_used]] == 0);
    ew->nrow_used++;
    //    ew->row_firstchar[ew->nrow_used] = ew->row_firstchar[ew->nrow_used-1];
  }

  /* cas particulier, EW_MAX_TOTAL_NROW trop faible,
     ca ne devrait jamais arriver. au cas on, on tronque comme une bete */
  if (i < ew->buff_len) ew->buff[i] = 0;

  editw_strpos2curs(ew, curs_pos);
}

static
void editw_get_sel_bounds(EditW *ew, int *sel_start, int *sel_end)
{
  if (ew->sel_anchor <= ew->sel_head) {
    *sel_start = ew->sel_anchor;
    *sel_end = ew->sel_head;
  } else {
    *sel_end = ew->sel_anchor;//-1;
    *sel_start = ew->sel_head;
  }
}


int
editw_has_selection(EditW *ew) 
{
  int sel_start;
  int sel_end;

  editw_get_sel_bounds(ew, &sel_start, &sel_end);
  return (sel_end != sel_start);
}


static void
editw_unselect(EditW *ew)
{
  ew->sel_head = ew->sel_anchor = -1;
}

/* efface le cut-buffer */
static void
editw_cb_clear()
{
  if (cb_buffer) {
    free(cb_buffer); cb_buffer = NULL;
  }
}

/*
static char *cmd_str(EditWCommandClass cmd) {
  switch (cmd) {
  case CUTPASTE: return "CUTPASTE";
  case INSERT_CHAR: return "INSERT_CHAR";
  case DELETE_CHAR: return "DELETE_CHAR";
  case OTHER: return "OTHER";
  }
  return NULL;
  }*/
/* pour pouvoir faire un undo */
static void
editw_save_state(EditW *ew, EditWCommandClass cmd) {
  if ((cmd == CUTPASTE) ||
      (cmd == INSERT_CHAR && ew->last_command != INSERT_CHAR) ||
      (cmd == DELETE_CHAR && ew->last_command != DELETE_CHAR)) {
    //    printf("save STATE %s -> %s\n", cmd_str(ew->last_command), cmd_str(cmd));
    strcpy(ew->undo.buff, ew->buff);
    ew->undo.y_scroll = ew->y_scroll;
    ew->undo.curs_x = ew->curs_x;
    ew->undo.curs_y = ew->curs_y;
  }
  ew->last_command = cmd;
}

static void
editw_remove_txt(EditW *ew, int i0, int i1)
{
  if (i0 < i1) {
    assert(i1 <= (int)strlen(ew->buff));

    if (i1-i0 > 1) {
      editw_save_state(ew, CUTPASTE);
    } else {
      editw_save_state(ew, DELETE_CHAR);
    }

    //    printf("delete %d %d\n",i0,i1);
    memmove(ew->buff + i0, ew->buff+i1, strlen(ew->buff+i1)+1);
    editw_wordwrap(ew);
  }
}

static void
editw_erase_selection(EditW *ew)
{
  int i0, i1;
  
  if (editw_has_selection(ew)) {
    editw_get_sel_bounds(ew, &i0, &i1);
    editw_unselect(ew);
    editw_remove_txt(ew, i0, i1);
    editw_strpos2curs(ew, i0);
  }
}

/* efface tout le buffer si il n'y a pas de selection active */
static void
editw_erase(EditW *ew)
{
  if (editw_has_selection(ew)) {
    editw_erase_selection(ew);
  } else {
    editw_unselect(ew);
    editw_remove_txt(ew, 0, strlen(ew->buff));
    editw_strpos2curs(ew, 0);
  }
  { int x = ew->curs_x, y = ew->curs_y;
  if (x < 0 || x >= EW_NCOL) {
    myprintf("%<MAG> RAH ENCORE CE BUG! x=%d y=%d curs_x=%d curs_y=%d,y_scroll=%d,sel_anchor=%d,sel_head=%d\n",
	     x,y,ew->curs_x,ew->curs_y,ew->y_scroll,ew->sel_anchor,ew->sel_head);
  }}
}


/* renvoie 0 si pas de pb, longueur tronquee si tronquage */
int
editw_insert_string(EditW *ew, const unsigned char *s)
{
  int ls,lb,lrest,ldec;
  int tronq;

  if (strlen(s)>1) {
    editw_save_state(ew, CUTPASTE);
  } else {
    editw_save_state(ew, INSERT_CHAR);
  }

  editw_erase_selection(ew);

  lb = strlen(ew->buff);
  ls = strlen(s);
  lrest = ew->buff_sz-1-ls-lb;

  tronq = 0;

  if (lrest < 0) {
    tronq = -lrest;
    ls += lrest;
    lrest = 0;
  }
  assert(ls >= 0);
  //  printf("ls=%d",ls);
  if (ls > 0) {
    int i,j;
    i = editw_xy2strpos(ew, ew->curs_x, ew->curs_y);
    ldec = MIN(lb - i + 1, ew->buff_sz-1-i-ls); assert(ldec>=0);
    //    printf(" ldec=%d\n",ldec);
    if (ldec) {
      memmove(ew->buff + i + ls, ew->buff+i, ldec);
    }
    assert(ew->buff[ew->buff_sz-1] == 0);
    for (j=0; j < ls; j++) 
      ew->buff[i+j] = (s[j] >= ' ' ? s[j] : ' ');
    //    memmove(ew->buff + i, s, ls);
    editw_wordwrap(ew);
    editw_strpos2curs(ew, i+ls);
  }

  return tronq;
  /*
  l = MIN(strlen(s), ew->buff_sz-1-strlen(s)-strlen(ew->buff));
  if (l > 0) {
    int i;
    int l2;
    i = editw_xy2strpos(ew, ew->curs_x, ew->curs_y);
    l2 = MIN(strlen(ew->buff+i)+1, ew->buff_sz-1-l-i); 
    memmove(ew->buff + i + l, ew->buff+i, l2);
    assert(ew->buff[ew->buff_sz-1] == 0);
    memmove(ew->buff + i, s, l);
    //    printf("'%s' [%s]\n", ew->buff, s);
    editw_wordwrap(ew);
    editw_strpos2curs(ew, i+l);
    }*/
}



static void
editw_insert_char(EditW *ew, unsigned char c)
{
  char s[2];
  s[0] = c; s[1] = 0;
  editw_insert_string(ew, s);
}

/* X clipboard function -- stolen from wmpinboard  Copyright (C) 1998-2000 by Marco G"otze. */



/*
 * copies <text> to the X clipboard
 */
void
editw_cb_copy(Dock *dock, Window win, const char *text, int len)
{
  int l;

  if (!text) return;

  if (cb_buffer) free(cb_buffer);
  l = len < 0 ? (int)strlen(text) : len;
  cb_buffer = malloc(l+1); assert(cb_buffer);
  strncpy(cb_buffer, text, l);
  cb_buffer[l] = 0;

  XSetSelectionOwner(dock->display, XA_PRIMARY, win, CurrentTime);
  if (XGetSelectionOwner(dock->display, XA_PRIMARY) != win) {
    fprintf(stderr, "wmcoincoin: Failed to set XA_PRIMARY ownership.");
    XChangeProperty(dock->display, dock->rootwin, XA_CUT_BUFFER0,
		    XA_STRING, 8, PropModeReplace, cb_buffer, l);
  }
}


/*
 * responds to a SelectionRequest event
 * [once again, thanks to the RXVT source]
 */
void
editw_cb_handle_selectionrequest(Dock *dock, XSelectionRequestEvent *rq)
{
  XEvent ev;
  Atom32 target_list[2];
  static Atom xa_targets = None;

  if (xa_targets == None) xa_targets = XInternAtom(dock->display, "TARGETS", 0);

  ev.xselection.type = SelectionNotify;
  ev.xselection.property = None;
  ev.xselection.display = rq->display;
  ev.xselection.requestor = rq->requestor;
  ev.xselection.selection = rq->selection;
  ev.xselection.target = rq->target;
  ev.xselection.time = rq->time;

  if (rq->target == xa_targets) {
    target_list[0] = (Atom32) xa_targets;
    target_list[1] = (Atom32) XA_STRING;
    XChangeProperty(dock->display, rq->requestor, rq->property, rq->target,
      8*sizeof(target_list[0]), PropModeReplace,
      (unsigned char*) target_list,
      sizeof(target_list)/sizeof(target_list[0]));
    ev.xselection.property = rq->property;
  } else if (rq->target == XA_STRING) {
    XChangeProperty(dock->display, rq->requestor, rq->property, rq->target,
      8, PropModeReplace, cb_buffer, strlen((char*) cb_buffer));
      ev.xselection.property = rq->property;
  }
  XSendEvent(dock->display, rq->requestor, 0, 0, &ev);
}

/*
 * pastes the current contents of the clipboard into <note> at <pos>, inserting
 * or overwriting depending on <ins>, trying to word-wrap unless <raw>; moves
 * the cursor
 */
static void
editw_cb_paste_external(Dock *dock, EditW *ew, Window window, unsigned prop, int Delete)
{
  unsigned long bytes_after, nitems;
  unsigned char *data;
  Atom actual_type;
  int actual_fmt;

  data = NULL;
  if (prop == None) return;
  if ((XGetWindowProperty(dock->display, window, prop, 0, 64, Delete,
    AnyPropertyType, &actual_type, &actual_fmt, &nitems, &bytes_after,
    &data) != Success))
  {
    XFree(data);
    return;
  }
  if (nitems && data) {
    editw_unselect(ew);
    editw_insert_string(ew,data);
  }
  if (data)  XFree(data);
}

/*
 * handle's the user's request to paste text into a note
 */
static void
editw_cb_paste(Dock *dock, EditW *ew, int external_only)
{
  Atom prop;

  if (cb_buffer && external_only == 0) {
    //    printf("paste buffer\n");
    editw_unselect(ew);
    editw_insert_string(ew, cb_buffer); 
  } else if (XGetSelectionOwner(dock->display, XA_PRIMARY) == None) {
    //    printf("paste external\n");
    editw_cb_paste_external(dock, ew, dock->rootwin, XA_CUT_BUFFER0, 0);
  } else {
    //    printf("paste else\n");
    prop = XInternAtom(dock->display, "VT_SELECTION", 0);
    XConvertSelection(dock->display, XA_PRIMARY, XA_STRING, prop, ew->win, CurrentTime);
  }
}


/* end of stolen function from wmpinboard */



static void
editw_cut_selection(Dock *dock, EditW *ew)
{
  int i0, i1;
  
  if (editw_has_selection(ew)) {
    editw_get_sel_bounds(ew, &i0, &i1);
    editw_cb_copy(dock, ew->win, ew->buff+i0, i1-i0);
    editw_remove_txt(ew, i0, i1);
    editw_strpos2curs(ew, i0);
    editw_unselect(ew);
  }
}

/* renvoie le numero du minibouton contenant le pixel (x,y)
   -1 sinon
*/
static int
editw_xy2minibutton(EditW *ew, int x, int y)
{
  int i;
  for (i=0; i < NB_MINIBT; i++) {
    if (ew->mini[i].visible) {
      if (IS_INSIDE(x,y,ew->mini[i].x, ew->mini[i].y, 
		    ew->mini[i].x + ew->mini[i].w - 1, ew->mini[i].y + ew->mini[i].h - 1)) {
	return i;
      }
    }
  }
  return -1;
}

static int
editw_button_pressed(EditW *ew) {
  int i;
  for (i=0; i < NB_MINIBT; i++) {
    if (ew->mini[i].visible && ew->mini[i].enfonce) {
      return i;
    }
  }
  return -1;
}

#define IS_BLANK(i) (((unsigned char)i)<=' ')

/*
  defini les couleurs a afficher pour chaque caractere de ew->buff
   -> detecte les mots trops longs, les urls et les balises html
*/
static void
editw_colorize(EditW *ew, unsigned char *ctab)
{
  int word_start, word_end,word_len, i, j;
  ErrList spelled_faults;

  if( ew->buff_num!=0 ) { /* ainsi on evite de se taper 2 fois la boucle 
			     ( comme pour les versions precedentes ) */
    for( i=0; ew->buff[i]; i++)
      ctab[i] = EWC_NORMAL;
  } else {
    if( Prefs.ew_do_spell )
      spelled_faults = spellString(ew->buff);
    else
      spelled_faults = NULL;
    i = 0;
    do {
      /* isolation du premier/prochain mot de buf */
      for(; ew->buff[i] && IS_BLANK(ew->buff[i]); ++i)
	ctab[i] = EWC_NORMAL;
      if( ew->buff[i]==0 )
	break;
      word_start = i;
      for(; ew->buff[i] && !IS_BLANK(ew->buff[i]); ++i);
      word_end = i;
      word_len = word_end-word_start+1; assert(word_len>0);
      /* detection des URLs */
      if( strncmp(ew->buff+word_start, "https://", 8)==0 ||
	  strncmp(ew->buff+word_start, "http://", 7)==0 ||
	  strncmp(ew->buff+word_start, "ftp://", 6)==0 ) {
	for( j=word_start; j<word_end; ++j) 
	  ctab[j] = EWC_URL;
	/* detection des mots trops longs */
      } else if( word_len>31 ) {
	for( j=word_start; j<word_end; ++j)
	  ctab[j] = EWC_LONGWORD;
      } else {
	j = word_start;
	while( j<word_end ) {
	  if( !isalpha(ew->buff[j]) ) {
	    if( ew->buff[j]!='<' )
	      ctab[j] = EWC_NORMAL;
	    /* detection des balises connues */
	    else if ( strncmp(ew->buff+j, "<i>", 3)==0 ||
		      strncmp(ew->buff+j, "</i>", 4)==0 ||
		      strncmp(ew->buff+j, "<b>", 3)==0 ||
		      strncmp(ew->buff+j, "</b>", 4)==0 ||
		      strncmp(ew->buff+j, "<u>", 3)==0 ||
		      strncmp(ew->buff+j, "</u>", 4)==0 ||
		      strncmp(ew->buff+j, "<s>", 3)==0 ||
		      strncmp(ew->buff+j, "</s>", 4)==0 ) {
	      for(; ew->buff[j]!='>'; ++j)
		ctab[j] = EWC_BALISE;
	      ctab[j] = EWC_BALISE;
	    }
	    ++j;
	    /* Detection des mots pleins de fautes */
	  } else if( spelled_faults!=NULL && (unsigned)j==spelled_faults->offset ) {
	    for(; isalpha(ew->buff[j]); ++j)
	      ctab[j] = EWC_SPELLWORD;
	    spelled_faults = spelled_faults->next;
	  } else {
	    /* Cas ou on a un caratere alpha qui est dans un mot san fautes (?) */
	    ctab[j] = EWC_NORMAL;
	    ++j;
	  }
	}
      }
    } while(ew->buff[i]);
  }
}

static void
editw_draw_frame(Dock *dock, EditW *ew, Drawable d, GC *gc, int mono)
{  
#define EWDF_COLOR(p) { if (!mono) {XSetForeground(dock->display, *gc, (p)); } \
                        else XSetForeground(dock->display, *gc, 1); } 

#define GROUIK_NPTS 19
  static XPoint grouik[GROUIK_NPTS] = 
    {{200                        , EW_SHAPED_HEIGHT-1},
     {200                        , EW_HEIGHT-3},
     {200+7                      , EW_HEIGHT-3-7},
     {EW_WIDTH - 8               , EW_HEIGHT-3-7},
     {EW_WIDTH - 4               , EW_HEIGHT-3-7-4},
     {EW_WIDTH - 4               , 15},
     {EW_WIDTH - 4+4             , 11},
     {EW_WIDTH + 4               , 11},
     {EW_WIDTH + 4               , 28},
     {EW_SHAPED_WIDTH-3          , 28},

     {EW_SHAPED_WIDTH-3          , 2},
     {EW_SHAPED_WIDTH-1          , 2},
     {EW_SHAPED_WIDTH-1          , 61},
     {EW_SHAPED_WIDTH-3          , 61},


     {EW_SHAPED_WIDTH-3          , 36},
     {EW_WIDTH + 4               , 36},
     {EW_WIDTH + 4               , EW_SHAPED_HEIGHT-1-7},
     {EW_WIDTH + 4 - 7           , EW_SHAPED_HEIGHT-1},
     {200                        , EW_SHAPED_HEIGHT-1}};
  
  XPoint grouik2[GROUIK_NPTS];

  // 1 -> couleur claire, -1 ->sombre, 0 ->normal
  static char grouik_seg_side_left[GROUIK_NPTS-1] = 
    {-1,0,1,0,-1,0,1,1,1,-1,0,0,0,-1, -1,1,0,-1};
  static char grouik_seg_side_right[GROUIK_NPTS-1] = 
    {1,1,1,1,1,1,1,-1,1, 1,0,0,0,1, -1,-1,-1,-1};


  int i;


  /* cadres */
  EWDF_COLOR(ew->win_bgpixel);
  XFillRectangle(dock->display, d, *gc, 
		 EW_XBORD, 0, EW_XBORD+EW_WIDTH, EW_HEIGHT);

  /* partie claire */
  EWDF_COLOR(ew->light_pixel);
  XDrawLine(dock->display, d, *gc, EW_XBORD, 0, EW_XBORD+EW_WIDTH-1, 0);
  XDrawLine(dock->display, d, *gc, EW_XBORD, 0, EW_XBORD, EW_HEIGHT-1);

  XDrawLine(dock->display, d, *gc, 
  	    EW_TXT_X0, EW_TXT_Y0+EW_TXT_HEIGHT, EW_TXT_X0 + EW_TXT_WIDTH, EW_TXT_Y0+EW_TXT_HEIGHT);
  XDrawLine(dock->display, d, *gc, 
  	    EW_TXT_X0 + EW_TXT_WIDTH, EW_TXT_Y0, EW_TXT_X0 + EW_TXT_WIDTH, EW_TXT_Y0+EW_TXT_HEIGHT);

  XDrawLine(dock->display, d, *gc, EW_XBORD, 0, EW_XBORD, EW_HEIGHT);

  /* partie sombre */
  EWDF_COLOR(ew->dark_pixel);
  XDrawLine(dock->display, d, *gc, EW_XBORD, EW_HEIGHT-1, EW_XBORD+EW_WIDTH-1, EW_HEIGHT-1);
  XDrawLine(dock->display, d, *gc, EW_XBORD+EW_WIDTH-1, 0, EW_XBORD+EW_WIDTH-1, EW_HEIGHT-1);
  XDrawLine(dock->display, d, *gc, 
	    EW_TXT_X0-1, EW_TXT_Y0-1, EW_TXT_X0 + EW_TXT_WIDTH, EW_TXT_Y0-1);
  XDrawLine(dock->display, d, *gc, 
	    EW_TXT_X0-1, EW_TXT_Y0-1, EW_TXT_X0-1, EW_TXT_Y0+EW_TXT_HEIGHT);

  /* partie coloree normale */
  EWDF_COLOR(dock->bg_pixel);

  for (i=0; i < GROUIK_NPTS; i++) {
    if (ew->dock_side == RIGHT) {
      grouik2[i] = grouik[i];
    } else {
      grouik2[i].x = EW_SHAPED_WIDTH-1 - grouik[i].x;
      grouik2[i].y = grouik[i].y;
    }
  }
    
  XFillPolygon(dock->display, d, *gc, grouik2, GROUIK_NPTS, Complex, CoordModeOrigin);

  for (i=0; i < GROUIK_NPTS-1; i++) {
    int x0, y0, x1, y1;
    int v;
    x0 = grouik2[i].x; y0 = grouik2[i].y;
    x1 = grouik2[i+1].x; y1 = grouik2[i+1].y;

    v = (ew->dock_side == LEFT ? grouik_seg_side_left[i] : grouik_seg_side_right[i]);
    if (v == 1) { EWDF_COLOR(dock->light_pixel); }
    else if (v == -1) { EWDF_COLOR(dock->dark_pixel); }
    else { EWDF_COLOR(dock->bg_pixel); }
    XDrawLine(dock->display, d, *gc, x0, y0, x1, y1);
  }
  
}


/* s'assurer que le wordwrap a ete fait avec d'appeler cette fonction */
static void
editw_draw(Dock *dock, EditW *ew, Drawable d)
{
  int i;
  unsigned char *ctab;
  int charcnt;

  if (strlen(ew->buff)) {
    ctab = malloc(strlen(ew->buff));
  } else ctab = NULL;

  editw_colorize(ew, ctab);
  
  /* le texte */
  XSetFont(dock->display, dock->NormalGC, ew->fn->fid);
  
  XSetForeground(dock->display, dock->NormalGC, ew->txt_bgpixel);
  XFillRectangle(dock->display, d, dock->NormalGC, 
		 EW_TXT_X0, EW_TXT_Y0, EW_TXT_WIDTH, EW_TXT_HEIGHT);



  //  XSetForeground(dock->display, dock->NormalGC, ew->txt_fgpixel[EWC_NORMAL]);
  
  charcnt = ew->row_firstchar[ew->y_scroll];
  for (i = ew->y_scroll; i < EW_NROW+ew->y_scroll; i++) {
    int j, cnt;
    for (j=ew->row_firstchar[i], cnt=0; j < ew->row_firstchar[i+1]; j++, cnt++) {
      XSetForeground(dock->display, dock->NormalGC, ew->txt_fgpixel[ctab[j]]);
      XDrawString(dock->display, d, dock->NormalGC, 
		  EW_TXT_X0+cnt*FN_W, EW_TXT_Y0 + (i-ew->y_scroll)*FN_H + FN_BASE_H, ew->buff+j, 1);
    }

    if (i >= ew->nrow_used-1) charcnt += EW_NCOL; else charcnt += cnt;
    

    //    llast = ew->nrow_used + (ew->buff_sz - ew->row_firstchar[ew->nrow_used])/EW_NCOL
    if ((i < ew->nrow_used - 1 && cnt < EW_NCOL)
	|| charcnt > ew->buff_sz) {
      if (charcnt > ew->buff_sz) cnt = MAX(EW_NCOL - (charcnt - ew->buff_sz+1), 0);
      XSetForeground(dock->display, dock->NormalGC, ew->fill_bgpixel);
      XFillRectangle(dock->display, d, dock->NormalGC, 
		     EW_TXT_X0+FN_W*cnt, EW_TXT_Y0 + (i-ew->y_scroll)*FN_H, 
		     EW_TXT_WIDTH-(FN_W*cnt), FN_H);
    }
  }


  /* pas de selection active -> on affiche le curseur */
  if (!editw_has_selection(ew)) {
    int curs_idx;

    XSetForeground(dock->display, dock->NormalGC, ew->cur_bgpixel);
    XFillRectangle(dock->display, d, dock->NormalGC, 
		   EW_TXT_X0 + FN_W * ew->curs_x, 
		   EW_TXT_Y0 + FN_H * (ew->curs_y-ew->y_scroll), FN_W, FN_H);
    curs_idx = editw_xy2strpos(ew, ew->curs_x, ew->curs_y);
    XSetForeground(dock->display, dock->NormalGC, ew->cur_fgpixel);
    
    if (curs_idx < ew->row_firstchar[ew->curs_y+1]) {
      XDrawString(dock->display, d, dock->NormalGC, 
		   EW_TXT_X0 + FN_W * ew->curs_x, 
		  EW_TXT_Y0 + FN_H * (ew->curs_y-ew->y_scroll) + FN_BASE_H, 
		  &ew->buff[curs_idx], 1);
    }
  } else {
    /* affichage de la selection */
    int sx0, sy0, sx1, sy1, y, x0, x1;
    int sel_start, sel_end;


    editw_get_sel_bounds(ew, &sel_start, &sel_end);
    assert(sel_start < sel_end);
    editw_strpos2xy(ew, sel_start, &sx0, &sy0);
    editw_strpos2xy(ew, sel_end-1, &sx1, &sy1);

    /* printf("sel: (%d,%d) - (%d,%d), ew->y_scroll=%d\n", sx0,sy0,sx1,sy1, ew->y_scroll); */

    if (sy0 < ew->y_scroll) { sy0 = ew->y_scroll; sx0 = 0; }
    if (sy1 > ew->y_scroll + EW_NROW - 1) { sy1 = ew->y_scroll + EW_NROW - 1; sx1 = EW_NCOL; }
    for (y = sy0; y <= sy1; y++) {
      if (y == sy0) x0 = sx0; else x0 = 0;
      if (y == sy1) {
	x1 = sx1;
	x1 = MIN(x1, ew->row_firstchar[y+1]-ew->row_firstchar[y] - 1);
      } else x1 = ew->row_firstchar[y+1]-ew->row_firstchar[y] - 1;

      /* printf("y=%d, x0=%d, x1=%d, row first=%d row last=%d\n", y, x0, x1, ew->row_firstchar[y], ew->row_firstchar[y+1]); */
      XSetForeground(dock->display, dock->NormalGC, ew->sel_bgpixel);
      XFillRectangle(dock->display, d, dock->NormalGC, 
		     EW_TXT_X0 + FN_W * x0, EW_TXT_Y0 + FN_H * (y-ew->y_scroll), FN_W*(x1+1-x0), FN_H);

      XSetForeground(dock->display, dock->NormalGC, ew->sel_fgpixel);
      XDrawString(dock->display, d, dock->NormalGC, 
		   EW_TXT_X0 + FN_W * x0, EW_TXT_Y0 + FN_H * (y-ew->y_scroll) + FN_BASE_H, 
		  &ew->buff[ew->row_firstchar[y]+x0], x1 - x0 + 1);
    }
  }


  /* titre */
  {
    int tx;
    
    tx = (ew->dock_side == RIGHT ? EW_TXT_X0 + 10 : 220);

    XSetForeground(dock->display, dock->NormalGC, ew->light_pixel);
    XDrawString(dock->display, d, dock->NormalGC, 
		tx - 1, EW_HEIGHT - 3 -1, 
		ew->title, strlen(ew->title));
    XSetForeground(dock->display, dock->NormalGC, ew->dark_pixel);
    XDrawString(dock->display, d, dock->NormalGC, 
		tx, EW_HEIGHT - 3, 
		ew->title, strlen(ew->title));
  }
  /* miniboutons */
  for (i=0; i < NB_MINIBT; i++) {
    int x,y,w,h;
    x =  ew->mini[i].x; y = ew->mini[i].y;
    w =  ew->mini[i].w; h = ew->mini[i].h;

    if (ew->mini[i].visible) {
      XCopyArea(dock->display, ew->minipix, d, dock->NormalGC, ew->mini[i].pix_x, ew->mini[i].pix_y,
		w,h,x+ew->mini[i].enfonce,y+ew->mini[i].enfonce);
      
      if (ew->mini[i].enfonce) {
	XSetForeground(dock->display, dock->NormalGC, dock->dark_pixel);
	
	XDrawLine(dock->display, d, dock->NormalGC, x, y, x+w, y);
	XDrawLine(dock->display, d, dock->NormalGC, x, y, x, y+h);
      } else {
	XSetForeground(dock->display, dock->NormalGC, dock->dark_pixel);
	
	XDrawLine(dock->display, d, dock->NormalGC, x+w, y+1, x+w, y+h);
	XDrawLine(dock->display, d, dock->NormalGC, x+1, y+h, x+w, y+h);
      }
    }
  }
  free(ctab);
}

void
editw_show_pix(Dock *dock, EditW *ew)
{
  XCopyArea(dock->display, ew->pix, ew->win, dock->NormalGC, 0, 0, EW_SHAPED_WIDTH, EW_SHAPED_HEIGHT, 0, 0);
}

void
editw_refresh(Dock *dock, EditW *ew)
{
  editw_draw(dock, ew, ew->pix);
  editw_show_pix(dock, ew);
}

/* un seul niveau d'undo... */
void
editw_undo(Dock *dock, EditW *ew) {
  if (ew->undo.buff) {
    int i;
    for (i=0; i < ew->buff_sz; i++) {
      SWAP(ew->buff[i], ew->undo.buff[i], char);
    }
    SWAP(ew->y_scroll, ew->undo.y_scroll, int);
    SWAP(ew->curs_x, ew->undo.curs_x, int);
    SWAP(ew->curs_y, ew->undo.curs_y, int);


    editw_unselect(ew);
    editw_wordwrap(ew);
    editw_refresh(dock, ew);
  }
}

/* changement de buffer edite: message <-> useragent */
void
editw_select_buff(Dock *dock, EditW *ew, int user_agent_mode)
{
  int j,x,sgn;

  if (user_agent_mode == 1) {
    ew->buff_num = 1;
    ew->title = "palmipède editor: USERAGENT";
    ew->buff_sz = USERAGENT_MAX_LEN+1;
    ew->buff = dock->coin_coin_useragent;
  } else if (user_agent_mode == 0) {
    ew->buff_num = 0;
    ew->title = "palmipède editor: MESSAGE";
    ew->buff_sz = MESSAGE_MAX_LEN+1;
    ew->buff = dock->coin_coin_message;
  } else {
    assert(0);
  }

  ew->buff[ew->buff_sz-1] = 0; // securite

  ew->curs_x = 0; ew->curs_y = 0; ew->y_scroll = 0;
  ew->sel_anchor = 0;
  ew->sel_head = strlen(ew->buff);

  editw_wordwrap(ew);

  if (ew->dock_side == RIGHT) {
    x = EW_WIDTH-8;
    sgn = -1;
  } else {
    x = EW_XBORD+8-15;
    sgn = +1;
  }
  
  for (j=0; j < NB_MINIBT; j++) {
    static const int miniorder[NB_MINIBT] = {
      BT_CLOSE, BT_CHANGE, BT_UNDO, BT_CLEAR, 
      BT_STRIKE, BT_UNDERLINE, BT_ITAL, BT_BOLD, BT_DEFAULTUA};
    int i;
    
    i = miniorder[j];

    ew->mini[i].visible = 1;
    if ((user_agent_mode && i != BT_CLOSE && i != BT_CHANGE && i != BT_UNDO && i != BT_CLEAR && i != BT_DEFAULTUA) ||
	(user_agent_mode==0 && i == BT_DEFAULTUA)) {
      ew->mini[i].visible = 0;
    }
    ew->mini[i].enfonce = 0;

    if (ew->mini[i].visible)
      x += sgn*(ew->mini[i].w+1);
    ew->mini[i].x = x;
    ew->mini[i].y = EW_SHAPED_HEIGHT - 15;
    if (j == 1) x += sgn*15;
    if (j == 3) x += sgn*15;
  }

  if (ew->undo.buff) free(ew->undo.buff);
  ew->undo.buff = malloc(ew->buff_sz); 
  memset(ew->undo.buff, 0, ew->buff_sz);

  ew->last_command = OTHER;
  editw_save_state(ew, CUTPASTE);

  //editw_refresh(dock, ew);
  editw_draw_frame(dock, ew, ew->pix, &dock->NormalGC, 0);
  editw_draw(dock, ew, ew->pix);
  //  XMoveWindow(dock->display, ew->win, 1200, 70) ;
  editw_action(dock, ew);
}

/* declenche l'affichage de la fenetre */
void
editw_show(Dock *dock, EditW *ew, int user_agent_mode)
{
  XSetWindowAttributes wa;
  Pixmap shape_mask;
  GC monoGC;

  assert(ew->mapped == 0);
  ew->mapped = 1;

  dock_get_icon_pos(dock, &ew->dock_x, &ew->dock_y);
  if (ew->dock_x < EW_SHAPED_WIDTH) ew->dock_side = LEFT; else ew->dock_side = RIGHT;
  
  ew->win = XCreateSimpleWindow (
        dock->display, dock->rootwin, 0, 0, EW_SHAPED_WIDTH, EW_SHAPED_HEIGHT, 0,
        WhitePixel(dock->display, dock->screennum),
        ew->win_bgpixel);
  
  //wa.background_pixmap = ButtonBarImage ;
  wa.event_mask =
    ButtonPressMask | 
    ButtonReleaseMask | 
    ExposureMask | 
    PointerMotionMask | 
    StructureNotifyMask |
    KeyPressMask | 
    KeyReleaseMask | 
    EnterWindowMask | 
    LeaveWindowMask | 
    FocusChangeMask;
  //  if (! ClickOnly) wa.event_mask |= EnterWindowMask | LeaveWindowMask ;
  wa.override_redirect = True ;
  XChangeWindowAttributes (dock->display, ew->win,
			   //CWBackPixmap | 
			   CWEventMask | CWOverrideRedirect, &wa);

  /* cette ligne marchait pile poil sur mon qwerty, je la garde en reserve...
     en la reregardant je crois qu'elle etait debile, elle defibnit
     deux fois XNInputSyle avec deux valeurs diffrenetes...
    ew->input_context = 
        XCreateIC(dock->input_method, 
	      XNInputStyle, XIMPreeditNothing | XIMStatusNothing,
	      XNInputStyle, XIMStatusNothing,
	      XNClientWindow, ew->win,
	      NULL);
  */

  /* celle-ci est parfaite sur l'azerty...
    ew->input_context = 
    XCreateIC(dock->input_method, XNInputStyle, 1032,
	      XNClientWindow, ew->win, XNFocusWindow, ew->win, 0);*/
#ifndef sun
  ew->input_context = 
    XCreateIC(dock->input_method, XNInputStyle, XIMPreeditNothing | XIMStatusNothing,
	      XNClientWindow, ew->win, XNFocusWindow, ew->win, 0);

  if(ew->input_context == NULL){
    fprintf(stderr, "Warning : erreur dans XCreateIC() !\n");
  }
#endif

  /* cree le pixmap et le rempli de noir */
  ew->pix = XCreatePixmap(dock->display, ew->win, EW_SHAPED_WIDTH, EW_SHAPED_HEIGHT, 
			  DefaultDepth(dock->display,dock->screennum));
  XSetForeground(dock->display, dock->NormalGC, BlackPixel(dock->display,dock->screennum)); 
  XFillRectangle(dock->display, ew->pix, dock->NormalGC, 0, 0, EW_SHAPED_WIDTH, EW_SHAPED_HEIGHT);

  /* cree le mask, avec le GC-qui-va-bien et le met a 0 */
  shape_mask = XCreatePixmap(dock->display, dock->rootwin, EW_SHAPED_WIDTH, EW_SHAPED_HEIGHT, 1);
  monoGC = XCreateGC(dock->display, shape_mask, 0, NULL);
  XSetForeground(dock->display, monoGC, 0); 
  XFillRectangle(dock->display, shape_mask, monoGC, 0, 0, EW_SHAPED_WIDTH, EW_SHAPED_HEIGHT);

  editw_draw_frame(dock, ew, ew->pix, &dock->NormalGC, 0);
  editw_draw_frame(dock, ew, shape_mask, &monoGC, 1);
  XShapeCombineMask(dock->display, ew->win, ShapeBounding, 0, 0, shape_mask,
		    ShapeSet);
  XFreePixmap(dock->display, shape_mask);
  XFreeGC(dock->display, monoGC);

  /*
  {
    XRectangle xr[2];
    
    xr[0].x = 0; xr[0].y = 0; xr[0].width = EW_WIDTH; xr[0].height = EW_HEIGHT-10;
    xr[1].x = 100; xr[1].y = EW_HEIGHT-9; xr[1].width = EW_WIDTH-100; xr[1].height = 10;
    XShapeCombineRectangles(dock->display, ew->win, ShapeBounding, 0, 0, xr, 2, ShapeSet, Unsorted);
    }*/

  ew->action = SORTIR; ew->action_step = 0;

  /* user_agent_mode = -1 -> on ne change pas le buff_num,
     sinon on lui assigne user_agent_mode */
  editw_select_buff(dock, ew, (user_agent_mode == -1 ? ew->buff_num : user_agent_mode));

}


void
editw_unmap(Dock *dock, EditW *ew)
{
#ifndef sun
  XDestroyIC(ew->input_context);
#endif
  //  fprintf(stderr, "destroy! %lx\n", (unsigned long)ew->win);
  XDestroyWindow(dock->display, ew->win);
  //  fprintf(stderr, "destroy2!\n");

  XFreePixmap(dock->display, ew->pix);
  ew->win = None;
  ew->mapped = 0;
  ew->action = NOACTION; /* au cas ou on fait un unmap en pleine sortie */
  ew->buff = NULL; ew->buff_sz = 0;
  if (ew->undo.buff) { free(ew->undo.buff); ew->undo.buff = NULL; }
}

int editw_ismapped(EditW *ew) {
  return ew->mapped;
}

#define ACTION_NB_STEPS 15

double exp(double x);
/* animation de la fenetre quand elle rentre et sort .. */
void
editw_action(Dock *dock, EditW *ew)
{
  //  printf("%d %d\n", ew->action, ew->action_step);
  if (ew->action == SORTIR) {
    float f;
    int l;
    static int save_prev_l = 0;

    f = (ew->action_step+1) / ((float)ACTION_NB_STEPS);
    l = (int)(EW_SHAPED_WIDTH * f * f + .5);
    //    l = (EW_SHAPED_WIDTH * (ew->action_step+1)) / (ACTION_NB_STEPS);
    
    //    printf("l=%d\n",l);
    if (l == 0) {
      l = 2;
    }
    if (ew->action_step == 0) {
      /* traitement particulier quand le dock est a gauche de editw,
	 a cause des XShape...
      */
      if (ew->dock_side == LEFT) {
	XShapeOffsetShape(dock->display, ew->win, ShapeBounding, -EW_SHAPED_WIDTH+l, 0);
	save_prev_l = l;
      }
    }
    if (l > EW_SHAPED_WIDTH) l = EW_SHAPED_WIDTH;
    if (ew->dock_side == LEFT) {
      ew->win_xpos = ew->dock_x+64-2;
      ew->win_ypos = ew->dock_y;
      XShapeOffsetShape(dock->display, ew->win, ShapeBounding, l-save_prev_l, 0);
      save_prev_l = l;
      XMoveResizeWindow(dock->display, ew->win, ew->win_xpos, ew->win_ypos, l, EW_SHAPED_HEIGHT);


      XCopyArea(dock->display, ew->pix, ew->win, dock->NormalGC, EW_SHAPED_WIDTH-l, 0, l, EW_SHAPED_HEIGHT, 0, 0);
    } else {
      ew->win_xpos = ew->dock_x-l +2;
      ew->win_ypos = ew->dock_y;
      XMoveResizeWindow(dock->display, ew->win, ew->win_xpos, ew->win_ypos, l, EW_SHAPED_HEIGHT);
      XCopyArea(dock->display, ew->pix, ew->win, dock->NormalGC, 0, 0, l, EW_SHAPED_HEIGHT, 0, 0);
    }
    if (ew->action_step == 0) {
      XMapRaised(dock->display, ew->win);

    }

    ew->action_step ++;
    if (ew->action_step >= ACTION_NB_STEPS) {
      //      ew->action_step = 0;
      editw_set_kbfocus(dock, ew, 1);
      ew->action = NOACTION;
    }
  } else if (ew->action == RENTRER) {
    //    int l = (EW_SHAPED_WIDTH * (ew->action_step)) / (ACTION_NB_STEPS);
    float f;
    int l;
    static int save_prev_l = 0;

    f = (ew->action_step) / ((float)ACTION_NB_STEPS);
    l = (int)(EW_SHAPED_WIDTH * (1. - (1-f) * (1-f)) + .5);

    if (ew->action_step == ACTION_NB_STEPS) save_prev_l = l;

    if (l > 0) {
      if (ew->dock_side == LEFT) {
	ew->win_xpos = ew->dock_x+64-2;
	ew->win_ypos = ew->dock_y;
	XShapeOffsetShape(dock->display, ew->win, ShapeBounding, l-save_prev_l, 0);
	save_prev_l = l;
	XMoveResizeWindow(dock->display, ew->win, ew->win_xpos, ew->win_ypos, l, EW_SHAPED_HEIGHT);
	XCopyArea(dock->display, ew->pix, ew->win, dock->NormalGC, EW_SHAPED_WIDTH-l, 0, l, EW_SHAPED_HEIGHT, 0, 0);
      } else {
	ew->win_xpos = ew->dock_x-l +2;
	ew->win_ypos = ew->dock_y;
	XMoveResizeWindow(dock->display, ew->win,  ew->win_xpos, ew->win_ypos, l, EW_SHAPED_HEIGHT);
	XCopyArea(dock->display, ew->pix, ew->win, dock->NormalGC, 0, 0, l, EW_SHAPED_HEIGHT, 0, 0);
      }
    }
    ew->action_step --;

    if (ew->action_step == 0) {
      ew->action = NOACTION;
      editw_unmap(dock, ew);
    }
  }
}

void
editw_hide(Dock *dock, EditW *ew) {
  if (dock)
    ew->action = RENTRER;
  //  ew->action_step = 0;
}

/* initialisation */
EditW *
editw_build(Dock *dock)
{
  EditW *ew;

  ALLOC_OBJ(ew, EditW);
  ew->mapped = 0;
  ew->action = NOACTION;
  ew->input_context = NULL;
  ew->buff_num = 0;
  ew->win_xpos = ew->win_ypos = 0;
  ew->undo.buff = NULL;
  ew->last_command = OTHER;
  
  {
    char fn[512];
    snprintf(fn, 512, "%s-%s", EW_FONT, Prefs.font_encoding);
    ew->fn = XLoadQueryFont(dock->display, fn);
    if (!ew->fn) {
      myfprintf(stderr, "Impossible de charger la fonte %s \n", fn);
      exit(-1);
    }
  }
  
  ew->win_bgpixel = RGB2PIXEL(200,200,200);
  ew->dark_pixel = RGB2PIXEL(128, 128, 128);
  ew->light_pixel = RGB2PIXEL(230, 230, 230);
  ew->fill_bgpixel = RGB2PIXEL(230, 230, 230);
  /* couleurs du texte */
  ew->txt_fgpixel[EWC_NORMAL] = RGB2PIXEL(0,0,0);
  ew->txt_bgpixel = RGB2PIXEL(255,255,255);
  ew->txt_fgpixel[EWC_LONGWORD] = RGB2PIXEL(255,0,0);
  ew->txt_fgpixel[EWC_BALISE] = RGB2PIXEL(0, 127, 0);
  ew->txt_fgpixel[EWC_URL] = RGB2PIXEL(0, 0, 255);
  ew->txt_fgpixel[EWC_SPELLWORD] = RGB2PIXEL(200, 50, 100);
  ew->cur_bgpixel = RGB2PIXEL(255,0,0);
  ew->cur_fgpixel = RGB2PIXEL(255,255,255);
  ew->sel_bgpixel = RGB2PIXEL(255,215,0);
  ew->sel_fgpixel = RGB2PIXEL(0,0,0);

  ew->sel_anchor = ew->sel_head = -1;
  ew->y_scroll = 0;
  ew->curs_x = 0;
  ew->curs_y = 0;

  head = &ew->sel_head;
  anch = &ew->sel_anchor;


  {
    char s_xpm_bgcolor[30];

    /* on remplace la ligne de la couleur transparente par notre couleur de fond,
     c une ruse de sioux */
    snprintf(s_xpm_bgcolor, 30, " \tc #%06X", Prefs.bgcolor);

    editwin_minib_xpm[1] = s_xpm_bgcolor;

    assert(XpmCreatePixmapFromData(dock->display, dock->rootwin, 
				   editwin_minib_xpm, &ew->minipix, NULL, NULL) == XpmSuccess);

    
    snprintf(s_xpm_bgcolor, 30, " \tc #%06X", (255 << 16) + (231 << 8) + 186);
    clippy_xpm[1] = s_xpm_bgcolor;
    assert(XpmCreatePixmapFromData(dock->display, dock->rootwin, 
				   clippy_xpm, &ew->clippy_pixmap, NULL, NULL) == XpmSuccess);

    sscanf(clippy_xpm[0], "%d %d", &ew->clippy_w, &ew->clippy_h);
  }

  {
    static int bt_x[NB_MINIBT] = { 0, 12, 27, 41, 55, 69, 83, 97, 111};
    static int bt_w[NB_MINIBT] = {11, 15, 14, 14, 14, 14, 14, 14, 14};
    int i;

    for (i=0; i < NB_MINIBT; i++) {
      ew->mini[i].pix_x = bt_x[i]; ew->mini[i].pix_y = 0;
      ew->mini[i].w = bt_w[i]; ew->mini[i].h = 12;
    }
  }
  return ew;
}

static void
editw_move_cursor_rel(EditW *ew, int dec)
{
  int i;
  i = editw_xy2strpos(ew, ew->curs_x, ew->curs_y);
  i += dec;
  i = MIN(i, (int)strlen(ew->buff));
  i = MAX(i, 0);
  editw_strpos2curs(ew, i);
}

/*
 * (des)active le keyboard focus sur la fenetre
 */  
void 
editw_set_kbfocus(Dock *dock, EditW *ew, int get_it)
{
  if (get_it && ew->mapped)
    XSetInputFocus(dock->display, ew->win, RevertToPointerRoot, CurrentTime);
  else
    XSetInputFocus(dock->display, PointerRoot, RevertToNone, CurrentTime);
}

/* gestion de la selection pour les mouvements de curseur
   avec 'shift' enfoncee */
static void
editw_shift_move_start(EditW *ew, int shift_move)
{
  if (shift_move) {
    if (editw_has_selection(ew) == 0) {
      ew->sel_anchor = editw_xy2strpos(ew, ew->curs_x, ew->curs_y);
      ew->sel_head = ew->sel_anchor;
    }
  } else {
    editw_unselect(ew);
  }
}

static void
editw_shift_move_end(EditW *ew, int shift_move)
{
  if (shift_move) {
    ew->sel_head = editw_xy2strpos(ew, ew->curs_x, ew->curs_y);
    //    printf("sel anch=%d, head = %d\n", ew->sel_anchor, ew->sel_head);
  }
}

static int
editw_move_left(EditW *ew, int shift_move)
{
  editw_shift_move_start(ew, shift_move);
  if (ew->curs_x == 0) {
    if (ew->curs_y > 0) {
      ew->curs_y--; 
      /* MIN pour le cas particulier d'une ligne de NCOL char exactment */
      ew->curs_x = MIN(ew->row_firstchar[ew->curs_y+1] - ew->row_firstchar[ew->curs_y], EW_NCOL-1);
      editw_adjust_yscroll(ew);
    } else return 0;
  } else ew->curs_x--;
  editw_shift_move_end(ew, shift_move);
  return 1;
}

static int
editw_move_right(EditW *ew, int shift_move)
{
  editw_shift_move_start(ew, shift_move);
  if (ew->curs_x+1 > MIN(ew->row_firstchar[ew->curs_y+1] - ew->row_firstchar[ew->curs_y], EW_NCOL-1)) {
    if (ew->curs_y < ew->nrow_used-1) {
      ew->curs_y++; ew->curs_x = 0;
      editw_adjust_yscroll(ew);
    } else return 0;
  } else ew->curs_x++;
  editw_shift_move_end(ew, shift_move);
  return 1;
}
static void editw_move_up(EditW *ew, int shift_move) {
  editw_shift_move_start(ew, shift_move);
  if (ew->curs_y > 0) {
    ew->curs_y--;
    ew->curs_x = MIN(ew->curs_x, ew->row_firstchar[ew->curs_y+1] - ew->row_firstchar[ew->curs_y]);
    editw_adjust_yscroll(ew);
  }
  editw_shift_move_end(ew, shift_move);
}

static void editw_move_down(EditW *ew, int shift_move) {
  editw_shift_move_start(ew, shift_move);
  if (ew->curs_y < ew->nrow_used-1) {
    ew->curs_y++;
    ew->curs_x = MIN(ew->curs_x, ew->row_firstchar[ew->curs_y+1] - ew->row_firstchar[ew->curs_y]);
    editw_adjust_yscroll(ew);
  }
  editw_shift_move_end(ew, shift_move);
}

static void editw_move_next_word(EditW *ew, int shift_move) {
  int i;

  editw_shift_move_start(ew, shift_move);
  i =   editw_xy2strpos(ew, ew->curs_x, ew->curs_y);
  if (i == (int)strlen(ew->buff)) return;

  while (IS_SEP2(ew->buff[i]) && ew->buff[i]) i++;
  while (!IS_SEP2(ew->buff[i]) && ew->buff[i]) i++;

  editw_strpos2curs(ew, i);
  editw_shift_move_end(ew, shift_move);
}

static void editw_move_prev_word(EditW *ew, int shift_move) {
  int i;

  editw_shift_move_start(ew, shift_move);
  i = editw_xy2strpos(ew, ew->curs_x, ew->curs_y);
  if (i == 0) return;

  while (IS_SEP2(ew->buff[i]) && i) i--;
  while (!IS_SEP2(ew->buff[i]) && i) i--;
  editw_strpos2curs(ew, i);
  editw_shift_move_end(ew, shift_move);
}


static void editw_move_start_of_line(EditW *ew, int shift_move) {
  editw_shift_move_start(ew, shift_move);
  ew->curs_x = 0;
  editw_shift_move_end(ew, shift_move);
}

/* pas statique car utilisee par le pinnipede */
void editw_move_end_of_line(EditW *ew, int shift_move) {
  editw_shift_move_start(ew, shift_move);
  ew->curs_x = MIN(ew->row_firstchar[ew->curs_y+1] - ew->row_firstchar[ew->curs_y], EW_NCOL-1);
  editw_shift_move_end(ew, shift_move);
}

static void editw_move_start(EditW *ew, int shift_move) {
  editw_shift_move_start(ew, shift_move);
  ew->curs_x = 0; ew->curs_y = 0;
  editw_adjust_yscroll(ew);
  editw_shift_move_end(ew, shift_move);
}

static void editw_move_end(EditW *ew, int shift_move) {
  editw_shift_move_start(ew, shift_move);
  ew->curs_y = ew->nrow_used-1;
  ew->curs_x = MIN(ew->row_firstchar[ew->curs_y+1] - ew->row_firstchar[ew->curs_y], EW_NCOL-1);
  editw_adjust_yscroll(ew);
  editw_shift_move_end(ew, shift_move);
}

static void editw_del_char(EditW *ew, int backspace)
{
  if (editw_has_selection(ew)) {
    editw_erase_selection(ew);
  } else {
    int i;
    i = editw_xy2strpos(ew, ew->curs_x, ew->curs_y);
    if (!backspace) {
      if (i > 0) {
	editw_move_cursor_rel(ew, -1);
	editw_remove_txt(ew, i-1, i);
	//	editw_move_left(ew, 0);
      }
    } else if (i < (int)strlen(ew->buff)) {
      editw_remove_txt(ew, i, i+1);
    }
  }
  editw_unselect(ew);
  //  printf(ew->buff);
  //	editw_wordwrap(ew);
}

static void
editw_balise_ital(EditW *ew) {
  int i;
  i = editw_insert_string(ew, "<i></i>"); if (i<4) editw_move_cursor_rel(ew,-4+i);
}
static void
editw_balise_bold(EditW *ew) {
  int i;
  i = editw_insert_string(ew, "<b></b>"); if (i<4) editw_move_cursor_rel(ew,-4);
}
static void
editw_balise_strike(EditW *ew) {
  int i;
  i = editw_insert_string(ew, "<s></s>"); if (i<4) editw_move_cursor_rel(ew,-4);
}
static void
editw_balise_underline(EditW *ew) {
  int i;
  i = editw_insert_string(ew, "<u></u>"); if (i<4) editw_move_cursor_rel(ew,-4);
}

/*
static void
editw_balise_tt(EditW *ew) {
  int i;
  i = editw_insert_string(ew, "<tt></tt>"); if (i<5) editw_move_cursor_rel(ew,-5);
}
*/

#define FORWARD_KEY XSendEvent(dock->display, dock->rootwin, True, KeyPressMask, event); \
                    editw_set_kbfocus(dock, ew, 1);

static void
editw_handle_keypress(Dock *dock, EditW *ew, XEvent *event)
{
  KeySym ksym;
  int klen;
  unsigned char buff[4];
  static XComposeStatus compose_status = { 0, 0 };

  //printf("keypress: keycode=%d, state=%x\n", event->xkey.keycode, event->xkey.state);

  if ((event->xkey.state & 0xdfe0) || 
      (ew->input_context && XFilterEvent(event, None))) {
    //printf("forward key: \n");
    FORWARD_KEY;
    return;
  }

  klen = XLookupString(&event->xkey, (char*)buff, sizeof(buff), &ksym, &compose_status);
  //printf("klen=%2d %08x %c state=%08x\n", klen, ksym, ksym, event->xkey.state);
  if (ksym == 0x20ac) { /* vilain hack pour reconnaite l'euro (le klen == 0 !!) */
    editw_insert_char(ew, (unsigned char)'¤');
  } else if (event->xkey.state & Mod1Mask) {
    switch (ksym) {
    case 'I':
    case 'i': editw_balise_ital(ew); break;
    case 'B':
    case 'b': editw_balise_bold(ew); break;
    case 'U':
    case 'u': editw_balise_underline(ew); break;
    case 'S':
    case 's': editw_balise_strike(ew); break;
      /*
    case 'T':
    case 't': editw_balise_tt(ew); break;
      */
    default:
      FORWARD_KEY; break;
    }
  } else if (event->xkey.state & ControlMask) {
    switch (ksym) {
    case 'A':
    case 'a': editw_move_start_of_line(ew, 0); break;

    case 'E':
    case 'e': editw_move_end_of_line(ew, 0); break;

    case 'F':
    case 'f': editw_move_right(ew, 0); break;
      
    case 'B':
    case 'b': editw_move_left(ew, 0); break;
      
    case 'N':
    case 'n': editw_move_down(ew, 0); break;

    case 'P':
    case 'p': editw_move_up(ew, 0); break;

    case 'D':
    case 'd': editw_del_char(ew, 1); break;

    case 'U':
    case 'u': editw_unselect(ew);  editw_move_start(ew, 1); editw_cut_selection(dock, ew); break;

    case 'K':
    case 'k': editw_unselect(ew); editw_move_end(ew, 1); editw_cut_selection(dock, ew); break;

    case 'W':
    case 'w': editw_cut_selection(dock, ew); break;

    case 'Y':
    case 'y': editw_cb_paste(dock, ew, 0); break;

    case 'Z':
    case 'z':
    case '_': editw_undo(dock, ew); break;

    case XK_KP_Left:
    case XK_Left:
      editw_move_prev_word(ew, event->xkey.state & ShiftMask); break;

    case XK_KP_Right:
    case XK_Right:
      editw_move_next_word(ew, event->xkey.state & ShiftMask); break;

    case XK_KP_Home:
    case XK_Home: 
      editw_move_start(ew, event->xkey.state & ShiftMask); break;

    case XK_KP_End:  
    case XK_End:
      editw_move_end(ew, event->xkey.state & ShiftMask); break;

    case XK_Return:
    case XK_KP_Enter:
      //      printf("state %d\n", dock->door_state);
      if (dock->door_state != OPENING && dock->door_state != OPENED) {
	//printf("opening %d\n", dock->door_state);
	dock->door_state = OPENING;
      } else if (dock->door_state == OPENED || dock->door_state_step > 12) {
	dock->red_button_press_flag = 1;
	dock->post_anonyme = 1;
      }
      break;

    default:
      FORWARD_KEY; break;
    }
  } else {
    if (ew->input_context) {
      klen = XmbLookupString(ew->input_context, &event->xkey, (char*) buff, sizeof(buff), &ksym, 0);
    } else {
      klen = XLookupString(&event->xkey, (char*) buff, sizeof(buff), &ksym,
      			   0);
    }
    //    printf("(BIS) klen=%2d %08x %c\n", klen, ksym, ksym);

    switch (ksym) {
    case XK_KP_Left:
    case XK_Left:
      editw_move_left(ew, event->xkey.state & ShiftMask); break;

    case XK_KP_Right:
    case XK_Right:
      editw_move_right(ew, event->xkey.state & ShiftMask); break;

    case XK_KP_Up:
    case XK_Up:
      editw_move_up(ew, event->xkey.state & ShiftMask); break;

    case XK_KP_Down:
    case XK_Down:
      editw_move_down(ew, event->xkey.state & ShiftMask); break;

    case XK_Delete:
      if ((event->xkey.state & ShiftMask) == 0) {
	editw_del_char(ew,1); 
      } else {
	editw_cut_selection(dock, ew);
      } break;

    case XK_BackSpace:
      editw_del_char(ew,0); break;

    case XK_Insert:
      if (event->xkey.state & ShiftMask) {
	editw_cb_paste(dock, ew, 0);
      } break;
      
    case XK_KP_Home:
    case XK_Home: 
      editw_move_start_of_line(ew, event->xkey.state & ShiftMask); break;

    case XK_KP_End:  
    case XK_End:
      editw_move_end_of_line(ew, event->xkey.state & ShiftMask); break;

    case XK_Escape:
      editw_hide(dock, ew); break;

    case XK_Tab:
      editw_select_buff(dock, ew, 1-ew->buff_num); break;

    case XK_KP_0:
    case XK_KP_1:
    case XK_KP_2:
    case XK_KP_3:
    case XK_KP_4:
    case XK_KP_5:
    case XK_KP_6:
    case XK_KP_7:
    case XK_KP_8:
    case XK_KP_9: editw_insert_char(ew, (unsigned char)(ksym - XK_KP_0) + '0'); break;
    case XK_KP_Decimal: editw_insert_char(ew, '.'); break;
    case XK_KP_Subtract: editw_insert_char(ew, '-'); break;
    case XK_KP_Add: editw_insert_char(ew, '+'); break;
    case XK_KP_Divide: editw_insert_char(ew, '/'); break;
    case XK_KP_Multiply: editw_insert_char(ew, '*'); break;

    case XK_Return:
    case XK_KP_Enter:
      //      printf("state %d\n", dock->door_state);
      if (dock->door_state != OPENING && dock->door_state != OPENED) {
	//printf("opening %d\n", dock->door_state);
	dock->door_state = OPENING;
      } else if (dock->door_state == OPENED || dock->door_state_step > 12) {
	dock->red_button_press_flag = 1;
	dock->post_anonyme = 0;
      }
      break;
    default:
      if (ksym <= 0x00ff && (ksym & 0xff)) {
	//editw_insert_char(ew, (unsigned char)(ksym & 0xff));
	if (buff[0])
	  editw_insert_char(ew, (unsigned char)buff[0]);
      }
      break;
    }
    /*
    if (ksym > ' ' && ksym <= 0xff) {
      editw_insert_char(dock, ew, c);
    */
  }
  editw_refresh(dock, ew);
  //  printf("ksym=%04x klen=%d buff=%02x(%c) %02x %02x %02x\n",(unsigned)ksym,klen, buff[0], buff[0], buff[1],buff[2],buff[3]);
  
}

void
editw_handle_keyrelease(Dock *dock, EditW *ew, XEvent *event)
{
  KeySym ksym;
  int klen;
  unsigned char buff[4];

  klen = XLookupString(&event->xkey, (char*)buff, sizeof(buff), &ksym, NULL);
  if (ksym == XK_Return || ksym == XK_KP_Enter) {
    if (dock_red_button_check(dock)) {
      editw_hide(dock, ew);
    }
  }
}

void 
editw_handle_button_press(Dock *dock, EditW *ew, XButtonEvent *event)
{
  int mx,my;
  int b;

  mx = event->x; my = event->y;
  //  BLAHBLAH(2,printf("editw click click ! %d %d\n",x,y));

  if (event->button == Button1) {
    b = editw_xy2minibutton(ew, mx,my); 
    if (b>=0) { 
      ew->mini[b].enfonce = 1;
      editw_refresh(dock,ew);
      return;
    }
  }

  if (IS_INSIDE(mx,my,EW_TXT_X0, EW_TXT_Y0,EW_TXT_X0+ EW_TXT_WIDTH-1, EW_TXT_Y0+EW_TXT_HEIGHT-1)) {
    int x,y;
    
    x = (mx - EW_TXT_X0)/FN_W;
    y = (my - EW_TXT_Y0)/FN_H + ew->y_scroll;
    assert(x >= 0 && x < EW_NCOL);
    assert(y >= 0 && (y-ew->y_scroll) < EW_NROW);
    /* left clic */
    if (event->button == Button1) {
      if (y >= ew->nrow_used) {
	y = ew->nrow_used - 1;
	x = ew->buff_len - ew->row_firstchar[y];
      } else if (x >= ew->row_firstchar[y+1] - ew->row_firstchar[y]) {
	x = ew->row_firstchar[y+1]-ew->row_firstchar[y];
      }
      ew->curs_x = x; ew->curs_y = y;
      ew->sel_anchor = editw_xy2strpos(ew,x,y);
      ew->sel_head = ew->sel_anchor;
    
      /* middle clic */  
    } else if (event->button == Button2) {
      editw_cb_paste(dock, ew, 1);
    }
  }
}

static void
editw_handle_button_release(Dock *dock, EditW *ew, XButtonEvent *event)
{
  int b;

  b = editw_button_pressed(ew);
  if (b >= 0) {
    ew->mini[b].enfonce = 0;
    if (b == editw_xy2minibutton(ew, event->x, event->y)) {
      switch (b) {
      case BT_CLOSE: editw_hide(dock, ew); break;
      case BT_CHANGE: editw_select_buff(dock, ew, 1-ew->buff_num); break;
      case BT_ITAL: editw_balise_ital(ew); break;
      case BT_BOLD: editw_balise_bold(ew); break;
      case BT_UNDERLINE: editw_balise_underline(ew); break;
      case BT_STRIKE: editw_balise_strike(ew); break;
	//      case BT_TT: editw_balise_tt(ew); break;
      case BT_UNDO: editw_undo(dock,ew); break;
      case BT_DEFAULTUA: editw_erase(ew);
	editw_insert_string(ew, Prefs.user_agent);
	break;
      case BT_CLEAR: 
	/* patch de monsieur glandium [v2.22, 11/11/2001] (tres legerement reorganisé) */
 	{
	  editw_erase(ew);
	  /* insere le username si tout le buffer a été effacé */
 	  if (Prefs.user_name && strlen(ew->buff) == 0) {
	    char *buffer = NULL;
 	    buffer = malloc(strlen(Prefs.user_name) + 3);
 	    strcpy(buffer,Prefs.user_name);
 	    strcat(buffer, "> ");
	    editw_insert_string(ew, buffer);
	    free(buffer);
	  }
	}
	break;
      default: assert(0); break;
      }
    }
  } else {

    /* si on n'a pas relache un bouton .. */
    if (editw_has_selection(ew)) {
      int i0, i1;
      editw_get_sel_bounds(ew, &i0, &i1);
      editw_cb_copy(dock,ew->win,ew->buff + i0, i1-i0);
    }
  }

  editw_refresh(dock, ew);
}

static void 
editw_handle_motion(Dock *dock, EditW *ew, XMotionEvent *event)
{
  int mx,my;

  mx = event->x; my = event->y;
  //  BLAHBLAH(2,printf("editw click click ! %d %d\n",x,y));

  if ((event->state & Button1Mask) && editw_button_pressed(ew) == -1) {
    int x,y;
    int pos;

    //    printf("selection en cours..(%d,%d)",mx,my);
    mx = MAX(mx,EW_TXT_X0);
    mx = MIN(mx,EW_TXT_X0 + EW_TXT_WIDTH-1);
    my = MAX(my,EW_TXT_Y0);
    my = MIN(my,EW_TXT_Y0 + EW_TXT_HEIGHT-1);
    //    printf("-> (%d,%d)",mx,my);

    x = (mx - EW_TXT_X0)/FN_W;
    y = (my - EW_TXT_Y0)/FN_H + ew->y_scroll;
    //    printf("-> (%d,%d)\n",x,y);
    assert(x >= 0 && x < EW_NCOL);
    assert(y >= 0 && (y-ew->y_scroll) < EW_NROW);

    
    if (y >= ew->nrow_used) {
      y = ew->nrow_used - 1;
      x = ew->buff_len - ew->row_firstchar[y];
    } else if (x >= ew->row_firstchar[y+1] - ew->row_firstchar[y]) {
      x = ew->row_firstchar[y+1]-ew->row_firstchar[y];
    }
    pos = editw_xy2strpos(ew,x,y);

    if (pos != ew->sel_head && ew->sel_anchor != -1) {
      ew->sel_head = pos;
      editw_refresh(dock, ew);
    }
  }
}


void
editw_dispatch_event(Dock *dock, EditW *ew, XEvent *event)
{
  if (ew->action != NOACTION) return; /* animation encours */
  switch (event->type) {
  case DestroyNotify: 
    {
    } break;
  case ButtonPress:
    {
      editw_handle_button_press(dock, ew, &event->xbutton);
      
    } break;
  case ButtonRelease:
    {
      editw_handle_button_release(dock, ew, &event->xbutton);
      editw_refresh(dock, ew);
    } break;
  case MotionNotify:
    {
      editw_handle_motion(dock, ew, &event->xmotion);
    } break;
  case EnterNotify:
    {
      editw_set_kbfocus(dock, ew, 1);
    } break;
  case LeaveNotify:
    {
      editw_set_kbfocus(dock, ew, 0);
    } break;
  case Expose:
    {
      XCopyArea(dock->display, ew->pix, ew->win, dock->NormalGC, 
		event->xexpose.x, event->xexpose.y, 
		event->xexpose.width, event->xexpose.height,
		event->xexpose.x, event->xexpose.y);
	//editw_show_pix(dock, ew);
	//      printf("expose!\n");
    } break;
  case MapNotify:
    {
    } break;
  case KeyPress:
    {
      editw_handle_keypress(dock, ew, event);
    } break;
  case KeyRelease:
    {
      editw_handle_keyrelease(dock, ew, event);
    } break;
  case SelectionClear:
    {
      //      printf("selection clear\n");
      editw_unselect(ew); 
      editw_cb_clear();
    } break;
  case SelectionNotify:
    {
      //      printf("selection notify\n");
      editw_cb_paste_external(dock, ew, event->xselection.requestor,
			      event->xselection.property, 1);
      editw_refresh(dock, ew);
    } break;
  case SelectionRequest: 
    {
      //      printf("selection request\n");
      editw_cb_handle_selectionrequest(dock, &(event->xselectionrequest));
    } break;
  }
}

Window editw_get_win(EditW *ew) {
  return ew->win;
}

void editw_balloon_test(Dock *dock, EditW *ew, int x, int y) {
  const char *s[NB_MINIBT];
  int i;
  static int nb_suggest = 0;
  const char *suggestion[] = {
    "une histoire de clavier qui se blo, ça plait toujours :)",
    "une fine allusion a folop et mooby",
    "un bon vieux troll de distribs",
    "un contre-troll, par exemple: <i>y'en a marre des trolls de distribs!</i>",
    "quelque chose du genre <i>Debian r0x0r</i>",
    "<i>w1nch10tte ssuxX!</i>",
    "<i>C01N C01N is your M4ST3R<i>",
    "<i>grouik grouiiiiiik !</i>",
    "<i>vi sux0r, emacs roulaize</i>",
    "une question a la con qui se termine par <i>c'est normal?</i>",
    "une contrepetrie",
    "d'attendre 11:11:11 pour donner l'heure",
    "<i>Plop!</i>",
    "de vous faire passer pour un admin NT",
    "d'imiter le cri d'un animal",
    "un <i>Moment Nuttela</i>",
    "de faire une blague en base 13",
    "de parler de vos problèmes de digestion",
    "d'aller faire un tour sur bbspot et de poster une news bidon sur la tribune",
    "d'expliquer pourquoi votre iguane des sables n'utilise que bsd",
    "de vous faire passer pour un utilisateur de MacOS/X",
    "de lancer un troll orteaugrafique",
    "de faire de l'ascii-art",
    "d'aller prendre une douche", 
    "de relancer le débat 'nmap' vs 'telnet sur le port 25'",
    "14:00 chotomie ?",
    "14:00 plodocus",
    "d'ouvrir une bière",
    "d'aller prendre un cafè",
    "de poster un jeu de mot bien pourri",
    "demander où est la FAQ de la tribune",
    "de signaler que WindowMaker 0.70.0 is out!",
    "de lancer un troll sur les user-agents",
    "d'annoncer avec émotion que vous venez de passer moine",
    "de tenter un concours de trollomètre",
    "de convaincre les moules de lancer un manual DDOS",
    "de poster un lien sur un forum externe (hard-war, aufeminin...)",
    "d'arrêter de mouler sur la tribune",
    "de lancer un débat sur pbpg",
    "de demander pourquoi wmcoincoin n'affiche pas la tribune",
    "de demander à la cantonnade \"qu'est ce qu'une moule ?\"",
    "de dire que la tribune hard-war est revenue",
    "de donner la météo",
    "de dire qu'aucune news n'a été modérée depuis bien longtemps",
    "de demander ce que peuvent bien faire les modérateurs",
    "de vous renseigner sur la façon d'avoir le meilleur rendu graphique pour votre site web en flash",
    "de lancer un sondage sur la quantité de sel dans une motte de beurre",
    "d'aller vous promener dans la forêt",
    "de vous lamenter sur la triste loi de la nature, appliquée aux oryx et aux requins",
    "de prendre une photo",
    "de demander aux moules ce qu'elles utilisent comme langage de programmation",
    "<i>le COBOL, c'est de la merde !<i>",
    "un troll de langages de programmation",
    "14:00 phtérie ?",
    "<i>wolfenstein cd key</i>",
    "d'expliquer pourquoi vous n'irez pas passer vos vacances en Bretagne",
    "de disserter sur la fainéantise des fonctionnaires de ce pays",
    "de demander à quoi sert le gros bouton rouge",
    "de poster un lien vers une news sur Britney Spears",
    "de demander combien Poolpy a de ventouses",
    "de poster un script permettant de voter en continu pour un sondage en ligne à la con",
    "\\o/ Nartaaaaaaa !",
    "un moment Village People: \\o/ ^o^ o[ /o\\",
    NULL
  };
  char txt[512];

  if (nb_suggest == 0) {
    while (suggestion[nb_suggest])
      nb_suggest++;
    //printf("il y a %d suggest\n", nb_suggest);
  }

  i = rand() % nb_suggest;
  snprintf(txt, 512, "<p align=center><b>wmCoinCoin Agent</b></p>"
	   "mmmh, j'ai l'impression que vous ne savez pas quoi dire, alors si je puis me permettre, "
	   "je vous suggère <font color=blue>%s</font>", suggestion[i]);
  if (ew->buff_num == 0) {
    balloon_test_with_image(dock, x, y, ew->win_xpos, ew->win_ypos, 8000,
			    EW_TXT_X0, EW_TXT_Y0, EW_TXT_WIDTH-1, EW_TXT_HEIGHT-1,
			    txt, ew->clippy_pixmap, ew->clippy_w+8, ew->clippy_h);
  } else {
    balloon_test(dock, x, y, ew->win_xpos, ew->win_ypos, 10000,
		 EW_TXT_X0, EW_TXT_Y0, EW_TXT_WIDTH-1, EW_TXT_HEIGHT-1,
		 "Entrez le useragent");
  }

  s[BT_CLOSE] = "Replier le palmipede<p>raccourci: <b>escape</b>";
  s[BT_CHANGE] = (ew->buff_num == 0 ? "Editer le useragent" : "Editer le message<p>raccourci: <b>Tab</b>");
  s[BT_ITAL] = "Inserer les balises Italiques<p>raccourci: <b>Alt-I</b>";
  s[BT_BOLD] = "Inserer les balises Bold<p>raccourci: <b>Alt-B</b>";
  s[BT_STRIKE] = "Inserer les balises Strike<p>raccourci: <b>Alt-S</b>";
  s[BT_UNDERLINE] = "Inserer les balises Underline<p>raccourci: <b>Alt-U</b>";
  //  s[BT_TT] = "Inserer les balises TeleType<p>raccourci: <b>Alt-T</b>";
  s[BT_UNDO] = "Undo<p>raccourci: <b>Ctrl-Z</b> ou <b>Ctrl-_</b>";
  s[BT_CLEAR] = "Effacer";
  s[BT_DEFAULTUA] = "Restaurer le user-agent par defaut";
  for (i = 0; i < NB_MINIBT; i++) {
    if (ew->mini[i].visible) {
      balloon_test(dock, x, y, ew->win_xpos, ew->win_ypos, 0, 
		   ew->mini[i].x, ew->mini[i].y, 
		   ew->mini[i].w, ew->mini[i].h, s[i]);
    }
  }
}
