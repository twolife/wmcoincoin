/*
  rcsid=$Id: newswin.c,v 1.2 2001/12/02 18:34:54 pouaite Exp $
  ChangeLog:
  $Log: newswin.c,v $
  Revision 1.2  2001/12/02 18:34:54  pouaite
  ajout de tags cvs Id et Log un peu partout...

*/

#include <X11/xpm.h>
#include "coincoin.h"
#include "picohtml.h"
#include "../xpms/fleche.xpm"

/*
  ----------------------------------------------------------------------
  gestion de la fenetre d'affichage du texte des news
  ----------------------------------------------------------------------
*/

typedef struct _PHView {
  int x, y; /* dimensions et position de la zone d'affichage */
  int w, h;
  int decal; /* en pixels (qt� de scroll) */
  PicoHtml *ph;

  int ph_h; /* hauteur en pixels du contenu de ph */

  /* les items correpondant au premier et dernier mots affich�s
     (attention, ne sont valides qu'apres l'appel � newswin_draw_titlelist)

     (suppose un bon ordonnancement des htmlitems)
  */
  PicoHtmlItem *first, *last;
} PHView;

struct _Newswin {
  Window window;
  int nb_comment;
  int err;
  char err_msg[200];
  int win_width, win_height; /* dim de la fenetre, en pixels */
  int win_xpos, win_ypos; 

  int news_id;
  GC gc;

  PHView phv_titles;
  PHView phv_news;
  
  PicoHtml *ph_survol;

  unsigned long win_bgpixel, win_fgpixel;
  unsigned long titles_bgpixel, titles_fgpixel;

  struct struc_ztitles {
    int y, h, nid;
  } *ztitle;
  int nb_ztitle;

  int active_znum;
  Pixmap pix; /* un gros pixmap de la taille de la fenetre: c'est pas tr�s classe */
  Pixmap flechepix;
  //RImage *rimage_bordure;
};


/* construction du texte avec ses quelques attributs */
static void
newswin_parsetxt(Dock *dock, DLFP_news *n)
{
  Newswin *nw = dock->newswin;
  unsigned char *buff;

  //BLAHBLAH(2,printf("newswin_parsetxt\n"));
  if (n->txt == NULL) {
    buff = strdup("Le <b>texte de la news</b> n'a pas encore ete mis a jour...");
  } else {
    char p1[512];
    
    snprintf(p1, 512, "<B><a href=\"http://%s:%d%s%s\"><font color=#000080>%s</font></a></B><br>post� par "
	     "<b><font color=#800000>%s</font></b> le %s, section <b>%s</b><p>",
	     Prefs.site_root, Prefs.site_port, Prefs.site_path, 
	     n->url,
	     n->titre, n->auteur, n->date, n->topic);
    ALLOC_VEC(buff, strlen(p1) + strlen(n->txt) + 1, unsigned char);
    strcpy(buff, p1); strcat(buff, n->txt);

    /* on signale que la news aura deja ete lue */
    if (n->flag_unreaded == 1) {
      n->flag_unreaded = -1; /* demande de maj du fichier 'newslues'*/
    }
  }

  picohtml_parse(dock, nw->phv_news.ph, buff, nw->phv_news.w);

  free(buff);
}



static void
phview_draw(Dock *dock, Drawable d, PHView *phv, unsigned long bg_pixel, int title_mode)
{
  Newswin *nw = dock->newswin;
  PicoHtmlItem *it;
  XRectangle xr;
  
  XSetForeground(dock->display, nw->gc, bg_pixel);
  XFillRectangle(dock->display, d, nw->gc, phv->x-4, phv->y-2, phv->w+8, phv->h+4);

  if (title_mode && nw->active_znum != -1) {
    int y0, y1;
    assert(nw->active_znum < nw->nb_ztitle);
    y0 = MAX(0,nw->ztitle[nw->active_znum].y-phv->decal - 1);
    y1 = MIN(phv->h-1, nw->ztitle[nw->active_znum].y+nw->ztitle[nw->active_znum].h-1-phv->decal + 2);
    if (y1>=y0) {
      XSetForeground(dock->display, nw->gc, WhitePixel(dock->display, dock->screennum));//dock->light_pixel);
      XFillRectangle(dock->display, d, nw->gc, phv->x-4, phv->y + y0, phv->w+8, y1-y0 + 1);
    }
  }

  xr.x = phv->x; xr.y = phv->y; xr.width = phv->w; xr.height = phv->h;
  XSetClipRectangles(dock->display, nw->gc, 0, 0, &xr, 1, Unsorted); /* faut pas que �a bave sur les bord :) */

  it = phv->ph->txt;

  phv->first = NULL; 
  phv->last = NULL;
  while (it) {
    if (it->y > phv->decal && it->y-it->h < phv->decal + phv->h) {
      int x, y;
      XSetFont(dock->display, nw->gc, it->fn->fid);
      XSetForeground(dock->display, nw->gc, it->pixel);
      x = it->x + phv->x;
      y = it->y - phv->decal + phv->y;
      XDrawString(dock->display, d, nw->gc, x, y, it->s, strlen(it->s));
      if (phv->first == NULL) phv->first = it;
      phv->last = it;
    }
    it = it->next;
  }
  XSetClipMask(dock->display, nw->gc, None);
}

#define FLECHE_BW 21
#define FLECHE_BH 7
#define FLECHE_BX1 nw->phv_titles.x
#define FLECHE_BX2 nw->phv_news.x
#define FLECHE_BY1 1
#define FLECHE_BY2 nw->win_height - FLECHE_BH - 2

/* press_num = numero du bouton presse, dans l'ordre:

  0  2
  1  3

(-1) = pas de fleche pressee
*/
void
newswin_redraw_fleches(Dock *dock, Drawable d, int press_num)
{
  Newswin *nw = dock->newswin;
  int x0;
  
  XSetForeground(dock->display, nw->gc, nw->win_bgpixel);
  if (nw->phv_titles.decal > 0) {
    x0 = (press_num == 0) ?  FLECHE_BW : 0;
    XCopyArea(dock->display, nw->flechepix, d, nw->gc, x0, 0, FLECHE_BW, FLECHE_BH, FLECHE_BX1, FLECHE_BY1);
  } else {
    XFillRectangle(dock->display, d, nw->gc, FLECHE_BX1, FLECHE_BY1, FLECHE_BW, FLECHE_BH);
  }
  if (nw->phv_titles.decal + nw->phv_titles.h < nw->phv_titles.ph_h) {
    x0 = (press_num == 1) ?  FLECHE_BW : 0;
    XCopyArea(dock->display, nw->flechepix, d, nw->gc, x0, FLECHE_BH, FLECHE_BW, FLECHE_BH, FLECHE_BX1, FLECHE_BY2);
  } else {
    XFillRectangle(dock->display, d, nw->gc, FLECHE_BX1, FLECHE_BY2, FLECHE_BW, FLECHE_BH);
  }
  if (nw->phv_news.decal > 0) {
    x0 = (press_num == 2) ?  FLECHE_BW : 0;
    XCopyArea(dock->display, nw->flechepix, d, nw->gc, x0, 0, FLECHE_BW, FLECHE_BH, FLECHE_BX2, FLECHE_BY1);
  } else {
    XFillRectangle(dock->display, d, nw->gc, FLECHE_BX2, FLECHE_BY1, FLECHE_BW, FLECHE_BH);
  }
  if (nw->phv_news.decal + nw->phv_news.h < nw->phv_news.ph_h) {
    x0 = (press_num == 3) ?  FLECHE_BW : 0;
    XCopyArea(dock->display, nw->flechepix, d, nw->gc, x0, FLECHE_BH, FLECHE_BW, FLECHE_BH, FLECHE_BX2, FLECHE_BY2);
  } else {
    XFillRectangle(dock->display, d, nw->gc, FLECHE_BX2, FLECHE_BY2, FLECHE_BW, FLECHE_BH);
  }
}

void
newswin_draw(Dock *dock)
{
  Newswin *nw = dock->newswin;

  phview_draw(dock, nw->pix, &nw->phv_titles, nw->titles_bgpixel, 1);
  phview_draw(dock, nw->pix, &nw->phv_news, nw->win_bgpixel,0);
  newswin_redraw_fleches(dock, nw->pix, -1);
  XCopyArea(dock->display, nw->pix, nw->window, nw->gc, 0, 0, nw->win_width, nw->win_height, 0, 0);
}


/* renvoie les news dans un ordre descendant (bien sur c'est pas fait pour trier un milliard de news ..) */
DLFP_news *
get_news_sorted(DLFP *dlfp, DLFP_news *previous)
{
  DLFP_news *n, *best;
  
  
  n = dlfp->news; best = NULL;
  while (n) {
    int ok, cmp;

    /* on accepte de tester la news si c'est le premier appel (previous = NULL)
       ou bien si la news est plus ancienne que previous */
    ok = 0;
    if (previous == NULL) ok = 1;
    else if (n != previous) {
      cmp = strcmp(previous->date, n->date);
      if (cmp > 0 || (cmp == 0 && (previous->heure > n->heure || 
				   (previous->heure == n->heure && previous->id > n->id)))) ok = 1;
    }

    /* si on la teste, on la compare au meilleur candidat .. */
    if (ok) {
      if (best == NULL) best = n;
      else {
	cmp = strcmp(best->date, n->date);
	if (cmp < 0 || (cmp == 0 && (best->heure < n->heure ||
				     (best->heure == n->heure && best->id < n->id)))) best = n;
      }
    }
    n = n->next;
  }
  return best;
}


  /* met � jour les donn�es du contenu (nouvelles news, ou bien changement de la news affich�e) */
void
newswin_update_content(Dock *dock, DLFP *dlfp, int reset_decal)
{
  Newswin *nw = dock->newswin;

  DLFP_news *n;
  int bidon;

#define BUFFSZ 8192
  char buff[BUFFSZ];
  
  /* cr�ation de la liste des titres des news */
  if (flag_updating_news) {
    printf("maj des news en cours...\n");
    return;
  }

  if (!picohtml_isempty(nw->phv_titles.ph)) {
    picohtml_freetxt(nw->phv_titles.ph);
  }
  nw->phv_titles.ph_h = 0;
  n = NULL;
  buff[0] = 0;
  while ((n=get_news_sorted(dlfp,n)) != NULL) {
    int l;
    l = strlen(buff); assert(l <= BUFFSZ-1); if (l == BUFFSZ-1) break;
    snprintf(buff+l, BUFFSZ-l, "<!special=%d>%s%s%s<br>", n->id, 
	     n->flag_unreaded <= 0 ? "" : "<b>",
	     n->titre, 
	     n->flag_unreaded <= 0 ? "" : "</b>");
  }
  picohtml_parse(dock, nw->phv_titles.ph, buff, nw->phv_titles.w);
  picohtml_gettxtextent(nw->phv_titles.ph, &bidon, &nw->phv_titles.ph_h);

  /* dectection des positions y des diff�rents titres de news (c'est TRES porchou...) */
  if (nw->nb_ztitle > 0) {
    assert(nw->ztitle);
    free(nw->ztitle); nw->ztitle = NULL; nw->nb_ztitle = 0;
  }
  {
    PicoHtmlItem *it;
    nw->nb_ztitle = 0;
    it = nw->phv_titles.ph->txt;
    while (it) {
      if (it->special_attr != 0) {
	nw->ztitle = realloc(nw->ztitle, sizeof(struct struc_ztitles)*(nw->nb_ztitle+1)); assert(nw->ztitle); /* laid ! */
	nw->ztitle[nw->nb_ztitle].y = it->y - it->fn->ascent;
	nw->ztitle[nw->nb_ztitle].h = it->y + it->fn->descent;
	nw->ztitle[nw->nb_ztitle].nid = it->special_attr;
	while (it && it->special_attr == nw->ztitle[nw->nb_ztitle].nid) {
	  nw->ztitle[nw->nb_ztitle].h = it->fn->descent + it->y - nw->ztitle[nw->nb_ztitle].y;
	  it = it->next;
	}
	//	printf("ztitle[%d]: y=%d, h=%d, nid=%d\n", nw->nb_ztitle, nw->ztitle[nw->nb_ztitle].y, nw->ztitle[nw->nb_ztitle].h, nw->ztitle[nw->nb_ztitle].nid);
	
	if (nw->ztitle[nw->nb_ztitle].nid == nw->news_id) nw->active_znum = nw->nb_ztitle;
	nw->nb_ztitle++;
      } else it = it->next;
    }
  }


  nw->phv_news.ph_h = 0;
  if (!picohtml_isempty(nw->phv_news.ph)) {
    picohtml_freetxt(nw->phv_news.ph);
  }
  if (reset_decal) {
    nw->phv_news.decal = 0;
  }
  if (nw->news_id <= 0) {
    printf("pas de news...\n");
  } else {
    n = dlfp_find_news_id(dlfp, nw->news_id);
    if (n == NULL) {
      printf("cette news a �t� d�truite (id=%d)!?...\n",nw->news_id);
    } else {
      newswin_parsetxt(dock, n);
      picohtml_gettxtextent(nw->phv_news.ph, &bidon, &nw->phv_news.ph_h);
    }
  }
}

static void
newswin_adjust_size(Dock *dock)
{
  Newswin *nw = dock->newswin;

  nw->win_width = MIN(nw->win_width, 2000);
  nw->win_height = MIN(nw->win_height, 2000); /* ne PAS faire exploser le serveur X avec un pixmap 10000x10000 ... */

  nw->phv_titles.x = 10; nw->phv_titles.w = 170;
  nw->phv_titles.y = 11; nw->phv_titles.h = nw->win_height - 12 - nw->phv_titles.y;

  nw->phv_news.x = nw->phv_titles.x + nw->phv_titles.w + 10;
  nw->phv_news.w = nw->win_width - nw->phv_news.x - 10;
  nw->phv_news.y = 9;
  nw->phv_news.h = nw->win_height - nw->phv_news.y - 10;
  
  if (nw->pix != None) {
    XFreePixmap(dock->display, nw->pix);
  }
  nw->pix = XCreatePixmap(dock->display, nw->window, 
			  nw->win_width, nw->win_height, DefaultDepth(dock->display,dock->screennum));
  assert(nw->pix != None);
  XSetForeground(dock->display, nw->gc, nw->win_bgpixel);
  XFillRectangle(dock->display, nw->pix, nw->gc, 0, 0, nw->win_width, nw->win_height);
}

static void
newswin_createXWindow(Dock *dock)
{
  Newswin *nw = dock->newswin;

  XTextProperty window_title_property;
  char *window_title;
  XSizeHints* win_size_hints;
  XClassHint *class_hint;
  int rc;
  char s[512];
  int xpos, ypos;

  //  int iconx, icony;  /* position de wmcoincoin */



  /* dimensions de la fenetre cree */
  //  picohtml_gettxtextent(nw->ph, &nw->win_width, &nw->win_height);

  //  printf("%d h=%d\n",nw->win_width, nw->win_height);

  //  nw->win_width += NEWSWIN_HTML_X*2;
  //  nw->win_height += NEWSWIN_HTML_Y +20;

  //  nw->win_height = MIN(nw->win_height, HeightOfScreen(XScreenOfDisplay(dock->display, dock->screennum)));

  assert(nw->win_width>0 && nw->win_height>0);
  BLAHBLAH(2, printf("newswin_createXWindow: creation d'une fenetre de dimensions %d %d\n",  
		     nw->win_width, nw->win_height));

  /* maintenant on laisse le windowmanager positionner la fenetre */
  /*
  dock_get_icon_pos(dock, &iconx, &icony);


  nw->win_xpos = iconx - 128;
  nw->win_ypos = icony - nw->win_height;

  //  printf("winpos: %d %d, w=%d, h=%d\n", iconx, icony,nw->win_width,nw->win_height);

  if (iconx+64 > WidthOfScreen(XScreenOfDisplay(dock->display, dock->screennum)) - nw->win_width) {
    nw->win_xpos = iconx - nw->win_width;
  } else {
    nw->win_xpos = iconx + 64;
  }
  nw->win_ypos = icony;
  if (nw->win_ypos + nw->win_height > HeightOfScreen(XScreenOfDisplay(dock->display, dock->screennum))) {
    nw->win_ypos = HeightOfScreen(XScreenOfDisplay(dock->display, dock->screennum)) - nw->win_height;
  }

  nw->win_xpos = MAX(nw->win_xpos, 0); // situations pathologiques
  nw->win_width = MIN(nw->win_width, WidthOfScreen(XScreenOfDisplay(dock->display, dock->screennum)));
  nw->win_ypos = MAX(nw->win_ypos, 0);
  nw->win_height = MIN(nw->win_height, HeightOfScreen(XScreenOfDisplay(dock->display, dock->screennum)));
  */

  if (nw->win_xpos == -10000 && nw->win_ypos == -10000) {
    xpos = 0; ypos = 0; /* �a n'a d'effet que sur certain windowmanagers rustiques (genre pwm) */
  } else {
    xpos = nw->win_xpos;
    ypos = nw->win_ypos;
  }
  

  nw->window = XCreateSimpleWindow(dock->display, dock->rootwin, xpos, ypos, nw->win_width, nw->win_height, 0, 
				   nw->win_bgpixel, nw->win_bgpixel);

  XSelectInput(dock->display, nw->window, ButtonPressMask | ExposureMask | ButtonReleaseMask | PointerMotionMask | EnterWindowMask | LeaveWindowMask | StructureNotifyMask);

  snprintf(s, 512, "news de %s", Prefs.site_root);
  window_title = s;
  rc = XStringListToTextProperty(&window_title,1, &window_title_property); assert(rc);
  XSetWMName(dock->display, nw->window, &window_title_property);

  win_size_hints= XAllocSizeHints(); assert(win_size_hints);
  /* au premier lancement, la pos n'est pas connue (sauf si specifee
     dans les options ) */
  if (nw->win_xpos == -10000 && nw->win_ypos == -10000) {
    win_size_hints->flags = PSize | PMinSize | PMaxSize;
  } else {
    win_size_hints->flags = USPosition | PSize | PMinSize | PMaxSize;
  }
  win_size_hints->x = xpos; 
  win_size_hints->y = ypos;
  win_size_hints->min_width = 300;
  win_size_hints->min_height = 100;
  win_size_hints->max_width = 1280;
  win_size_hints->max_height = 1024;
  win_size_hints->base_width = nw->win_width;
  win_size_hints->base_height = nw->win_height;
  XSetWMNormalHints(dock->display, nw->window, win_size_hints);
  XFree(win_size_hints);

  class_hint = XAllocClassHint();
  class_hint->res_name = "news";
  sprintf(s, "wmcoincoin_%s", Prefs.site_root);
  class_hint->res_class = s;
  XSetClassHint(dock->display, nw->window, class_hint);
  XFree(class_hint);

  /* pour etre inform� de la fermeture de la fenetre demandee par le windowmanager */
  XSetWMProtocols(dock->display, nw->window, &dock->atom_WM_DELETE_WINDOW, 1);
  
  /* gc, fontes */
  nw->gc = XCreateGC(dock->display, nw->window, 0, NULL);
  XSetForeground(dock->display, nw->gc, nw->win_fgpixel);
  XSetBackground(dock->display, nw->gc, nw->win_bgpixel);
  XSetLineAttributes(dock->display, nw->gc, 1, LineSolid, CapButt, JoinBevel);
  XSetFillStyle(dock->display, nw->gc, FillSolid);
  //  XSetFont(dock->display, nw->gc, picohtml_get_fn_base(nw->ph)->fid);
  

  //  newswin_draw(dock, nw);
  XMapRaised(dock->display, nw->window);
}

void
newswin_show(Dock *dock, DLFP *dlfp, int id)
{
  Newswin *nw = dock->newswin;
  int create = 0;

  if (id != -2) nw->news_id = id; /* pas beau, c'est pour l'appel par clic droit (wmcoincoin.c ligne 1327)*/

  if (nw->window == None) {
    newswin_createXWindow(dock);
    create = 1;
  }
  newswin_adjust_size(dock);
  nw->active_znum = -1;
  assert(nw->window != None);
  newswin_update_content(dock, dlfp, 1);
  if (create == 0) newswin_draw(dock);
}

void
newswin_unmap(Dock *dock)
{
  Newswin *nw = dock->newswin;
  
  assert(nw->window != None);
  XDestroyWindow(dock->display, nw->window);
  nw->window = None;

  if (!picohtml_isempty(nw->phv_titles.ph)) 
    picohtml_freetxt(nw->phv_titles.ph);
  if (!picohtml_isempty(nw->phv_news.ph))
    picohtml_freetxt(nw->phv_news.ph);
  if (nw->nb_ztitle > 0) {
    assert(nw->ztitle);
    free(nw->ztitle); nw->ztitle = NULL; nw->nb_ztitle = 0;
  }
  if (nw->pix != None) {
    XFreePixmap(dock->display, nw->pix); nw->pix = None;
  }
}

/* renvoie le numero du titre de news survolle en (mx,my) */
int
newswin_get_znum(Dock *dock, int mx, int my)
{ 
  Newswin *nw = dock->newswin;
  int y, i;

  if (!IS_INSIDE(mx,my, nw->phv_titles.x, nw->phv_titles.y, 
		 nw->phv_titles.x + nw->phv_titles.w - 1,
		 nw->phv_titles.y + nw->phv_titles.h - 1)) return -1;

  y = my - nw->phv_titles.y + nw->phv_titles.decal;
  for (i=0; i < nw->nb_ztitle; i++) {
    if (y >= nw->ztitle[i].y && y < nw->ztitle[i].y + nw->ztitle[i].h) {
      return i; break;
    }
  }
  return -1;
}

PicoHtmlItem *
get_phi_survol(Dock *dock, int mx, int my)
{
  Newswin *nw = dock->newswin;
  PicoHtmlItem *pi;
  int x,y;

  if (!IS_INSIDE(mx,my, nw->phv_news.x, nw->phv_news.y, 
		 nw->phv_news.x + nw->phv_news.w - 1,
		 nw->phv_news.y + nw->phv_news.h - 1)) return NULL;
  
  pi = nw->phv_news.first;
  x = mx - nw->phv_news.x;
  y = my - nw->phv_news.y + nw->phv_news.decal;
  while (pi) {
    if (IS_INSIDE(x, y, pi->x, pi->y - pi->fn->ascent, pi->x + pi->w, pi->y + pi->fn->descent)) {
      return pi;
    }
    if (pi == nw->phv_news.last) break;
    pi = pi->next;
  }
  return NULL;
}

/* infos affich�es selon la position de la souris */
void
newswin_update_info(Dock *dock, DLFP *dlfp, int mx, int my) {
  Newswin *nw = dock->newswin;
  PicoHtmlItem *pi;
  char survol[1024];
  static int old_hash = 0;
  int hash;
  char *p;

  survol[0] = 0; 
  if (flag_updating_news) {
    strcpy(survol, "mise � jour des news en cours...");
  } 
  if (survol[0] == 0) {
    pi = get_phi_survol(dock, mx, my);
    if (pi) {
      if (pi->link_str) {
	//	printf("link: '%s'\n", pi->link_str);
	snprintf(survol, 1024, "URL: %s", pi->link_str);
      }
    }
  }
  if (survol[0] == 0) {
    int znum;
    DLFP_news *n;
    znum = newswin_get_znum(dock, mx, my);
    if (znum != -1) {
      n = dlfp_find_news_id(dlfp, nw->ztitle[znum].nid);
      if (n) {
	snprintf(survol, 1024, "id=<b>%d</b>, date : %s, approuv�e � %02d:%02d, %s", n->id, n->date, n->heure / 60, n->heure % 60,
		 (n->flag_unreaded <= 0) ? 
		 "(d�j� lue)":
		 "<b><font color=#8f0000>(nouvelle!)</font></b>");
      } else {
	snprintf(survol, 1024, "id=<b>%d</b>, contenu de la news non disponible (trop vieille ? download en cours ?)", nw->ztitle[znum].nid);
      }
    }
  }
  hash = 0;
  p = survol;
  while (*p) { hash += (((unsigned)*p) * 253) + 23; p++; }

  if (hash != old_hash) {
    old_hash = hash;

    newswin_draw(dock);
    if (strlen(survol)) {
      PicoHtml *ph = nw->ph_survol;
      int w,h;
      int x,y;

      //      printf("affichage de: %s\n", survol);
      picohtml_parse(dock, ph, survol, nw->phv_news.w);
      picohtml_gettxtextent(ph, &w, &h);

      x = nw->win_width - w - 4;
      y = nw->win_height - h - 2;

      XSetForeground(dock->display, nw->gc, WhitePixel(dock->display, dock->screennum));
      XFillRectangle(dock->display, nw->window, nw->gc, x, y, nw->win_width-x, nw->win_height-y);
      XSetForeground(dock->display, nw->gc, BlackPixel(dock->display, dock->screennum));
      XDrawRectangle(dock->display, nw->window, nw->gc, x, y, nw->win_width-x, nw->win_height-y);
      picohtml_render(dock, ph, nw->window, nw->gc, x+3, y-ph->fn_base->descent+1);
      picohtml_freetxt(ph);
    }
  }
}

void
newswin_handle_button_press(Dock *dock, XButtonEvent *event)
{
  Newswin *nw = dock->newswin;
  int mx,my;
  mx = event->x; my = event->y;
  if (IS_INSIDE(mx,my, FLECHE_BX1, FLECHE_BY1, 
		FLECHE_BX1+FLECHE_BW-1, FLECHE_BY1+FLECHE_BH-1)) {
    newswin_redraw_fleches(dock, nw->window, 0);
  }
  if (IS_INSIDE(mx,my, FLECHE_BX1, FLECHE_BY2, 
		FLECHE_BX1+FLECHE_BW-1, FLECHE_BY2+FLECHE_BH-1)) {
    newswin_redraw_fleches(dock, nw->window, 1);
  }
  if (IS_INSIDE(mx,my, FLECHE_BX2, FLECHE_BY1, 
		FLECHE_BX2+FLECHE_BW-1, FLECHE_BY1+FLECHE_BH-1)) {
    newswin_redraw_fleches(dock, nw->window, 2);
  }
  if (IS_INSIDE(mx,my, FLECHE_BX2, FLECHE_BY2, 
		FLECHE_BX2+FLECHE_BW-1, FLECHE_BY2+FLECHE_BH-1)) {
    newswin_redraw_fleches(dock, nw->window, 3);
  }
}

static void
newswin_scrollup(Dock *dock, PHView *phv, int q)
{
  phv->decal = MAX(0, phv->decal-q);
  newswin_draw(dock);
}

static void
newswin_scrolldown(Dock *dock, PHView *phv, int q)
{
  phv->decal = MIN(phv->decal + q, phv->ph_h +10 - phv->h);
  phv->decal = MAX(0, phv->decal);
  newswin_draw(dock);
}



void
newswin_handle_button_release(Dock *dock, DLFP *dlfp, XButtonEvent *event) {
  Newswin *nw = dock->newswin;
  int mx,my;
  int in_titles, in_news;
  PHView *phv = NULL;
  
  mx = event->x; my = event->y;

  in_titles = IS_INSIDE(mx,my, nw->phv_titles.x, nw->phv_titles.y, 
			nw->phv_titles.x + nw->phv_titles.w - 1,
			nw->phv_titles.y + nw->phv_titles.h - 1);
  in_news = IS_INSIDE(mx,my, nw->phv_news.x, nw->phv_news.y, 
			nw->phv_news.x + nw->phv_news.w - 1,
			nw->phv_news.y + nw->phv_news.h - 1);

  if (in_news) phv = &nw->phv_news;
  else if (in_titles) phv = &nw->phv_titles;

  if (event->button == Button4 && phv) {
    newswin_scrollup(dock, phv, 20);
  } else if (event->button == Button5 && phv) {
    newswin_scrolldown(dock, phv, 20);
  } else if (in_titles && event->button == Button1) {
    int znum;

    znum = newswin_get_znum(dock, mx, my);
    if (znum != -1) {
      newswin_show(dock, dlfp, nw->ztitle[znum].nid);      
    }
  } else if (in_news) {
    PicoHtmlItem *pi;
    pi = get_phi_survol(dock, mx, my);
    if (pi && pi->link_str) {
      if (event->button == Button1) {
	open_url(pi->link_str, 
		 nw->win_xpos + nw->phv_news.x + pi->x, 
		 nw->win_ypos + nw->phv_news.y + pi->y, 1);
      } else if (event->button == Button2) {
	open_url(pi->link_str, 
		 nw->win_xpos + nw->phv_news.x + pi->x, 
		 nw->win_ypos + nw->phv_news.y + pi->y, 2);
      } else if (event->button == Button3) {
	printf("copy! %s\n",pi->link_str);
	editw_cb_copy(dock, nw->window, pi->link_str, strlen(pi->link_str));
      }
    }
  }

  if (event->button == Button1 || event->button == Button3) {
    int q;
    q = event->button == Button1 ? 10 : (nw->win_height / 2);
    if (IS_INSIDE(mx,my, FLECHE_BX1, FLECHE_BY1, 
		  FLECHE_BX1+FLECHE_BW-1, FLECHE_BY1+FLECHE_BH-1)) {
      newswin_scrollup(dock, &nw->phv_titles, q);
    }
    if (IS_INSIDE(mx,my, FLECHE_BX1, FLECHE_BY2, 
		  FLECHE_BX1+FLECHE_BW-1, FLECHE_BY2+FLECHE_BH-1)) {
      newswin_scrolldown(dock, &nw->phv_titles, q);
    }
    if (IS_INSIDE(mx,my, FLECHE_BX2, FLECHE_BY1, 
		  FLECHE_BX2+FLECHE_BW-1, FLECHE_BY1+FLECHE_BH-1)) {
      newswin_scrollup(dock, &nw->phv_news, q);
    }
    if (IS_INSIDE(mx,my, FLECHE_BX2, FLECHE_BY2, 
		  FLECHE_BX2+FLECHE_BW-1, FLECHE_BY2+FLECHE_BH-1)) {
      newswin_scrolldown(dock, &nw->phv_news, q);
    }
  }
}

void
newswin_dispatch_event(Dock *dock, DLFP *dlfp, XEvent *event)
{
  Newswin *nw = dock->newswin;
  if (nw->window != None) {
    switch (event->type) {
    case DestroyNotify:
      BLAHBLAH(2,printf("newswin destroy !\n"));
      break;
    case ButtonPress:
      {
	newswin_handle_button_press(dock, &event->xbutton);
      } break;
    case ButtonRelease:
      {
	newswin_handle_button_release(dock, dlfp, &event->xbutton);
      } break;
    case MotionNotify:
      {
	newswin_update_info(dock, dlfp, event->xmotion.x, event->xmotion.y);
      } break;
    case EnterNotify:
    case LeaveNotify:
      {
	newswin_update_info(dock, dlfp, event->xcrossing.x, event->xcrossing.y);
      } break;
    case Expose:
      {
	XWindowAttributes wa;

	if (event->xexpose.count == 0) {
	  assert(nw->window != None);
	  
	  //flush_expose(nw->window);
	  XGetWindowAttributes(dock->display, nw->window, &wa);
	  /* on sauve la position de la fenetre (en prenant en compte les decorations du WM ) */
	  get_window_pos_with_decor(dock->display, nw->window, &nw->win_xpos, &nw->win_ypos);
	  if (wa.width != nw->win_width || wa.height != nw->win_height) {
	    nw->win_width = MAX(wa.width,80);
	    nw->win_height = MAX(wa.height,80);
	    newswin_adjust_size(dock);
	    newswin_update_content(dock, dlfp, 1);
	  }
	//	XMoveWindow(dock->display, nw->window, nw->win_xpos, nw->win_ypos);
	  newswin_draw(dock);
	}
      } break;
    case MapNotify:
      {
      } break;
    case SelectionRequest: 
      {
	editw_cb_handle_selectionrequest(dock, &(event->xselectionrequest));
      } break;
    case ClientMessage:
      {
	if (event->xclient.message_type == dock->atom_WM_PROTOCOLS
	    && event->xclient.format == 32 
	    && (Atom)event->xclient.data.l[0] == dock->atom_WM_DELETE_WINDOW) {
	  newswin_unmap(dock);
	}
      } break;
    }
  }
}

#define C2RGB(c) ((c) >> 16 & 0xff), ((c) >> 8 & 0xff), ((c) & 0xff)

void
newswin_build(Dock *dock)
{
  Newswin *nw;
  ALLOC_OBJ(nw,Newswin);
  nw->win_bgpixel = IRGB2PIXEL(Prefs.news_bgcolor);
  nw->win_fgpixel = IRGB2PIXEL(Prefs.news_fgcolor);
  nw->titles_bgpixel = IRGB2PIXEL(0xd0d0d0);
  nw->titles_fgpixel = IRGB2PIXEL(0x000080);

  nw->phv_news.ph = picohtml_create(dock, Prefs.news_fn_family, Prefs.news_fn_size, 1);
  picohtml_set_default_pixel_color(nw->phv_news.ph, nw->win_fgpixel);

  nw->phv_titles.ph = picohtml_create(dock, Prefs.news_fn_family, Prefs.news_fn_size, 1);
  picohtml_set_default_pixel_color(nw->phv_titles.ph, nw->titles_fgpixel);

  nw->ph_survol = picohtml_create(dock, Prefs.news_fn_family, Prefs.news_fn_size, 0);
  picohtml_set_parag_indent(nw->ph_survol, 0);
  picohtml_set_parag_skip(nw->ph_survol, 1.0);

  nw->win_width = Prefs.news_width;
  nw->win_height = Prefs.news_height;
  nw->win_xpos = Prefs.news_xpos;
  nw->win_ypos = Prefs.news_ypos;

  nw->window = None;
  nw->phv_news.decal = 0; nw->phv_titles.decal = 0;
  nw->nb_ztitle = 0; nw->ztitle = NULL;
  nw->active_znum = -1;
  nw->pix = None;
  nw->news_id = -1;
  {
    char s_xpm_bgcolor[30];
    /* on remplace la ligne de la couleur transparente par notre couleur de fond,
     c une ruse de sioux */
    snprintf(s_xpm_bgcolor, 30, " \tc #%06X", Prefs.news_bgcolor);

    fleche_xpm[1] = s_xpm_bgcolor;

    assert(XpmCreatePixmapFromData(dock->display, dock->rootwin, 
				   fleche_xpm, &nw->flechepix, NULL, NULL) == XpmSuccess);
  }


  dock->newswin = nw;

}

int
newswin_is_used(const Dock *dock) {
  const Newswin *nw = dock->newswin;
  return (nw->window != None);
}

Window
newswin_get_window(const Dock *dock) {
  const Newswin *nw = dock->newswin;
  return nw->window;
}

int
newswin_get_xpos(const Dock *dock) {
  const Newswin *nw = dock->newswin;
  return nw->win_xpos;
}

int
newswin_get_ypos(const Dock *dock) {
  const Newswin *nw = dock->newswin;
  return nw->win_ypos;
}
