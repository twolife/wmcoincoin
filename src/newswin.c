/*
  rcsid=$Id: newswin.c,v 1.17 2002/10/16 20:41:45 pouaite Exp $
  ChangeLog:
  $Log: newswin.c,v $
  Revision 1.17  2002/10/16 20:41:45  pouaite
  killall toto

  Revision 1.16  2002/10/15 23:17:28  pouaite
  rustinage à la truelle

  Revision 1.15  2002/08/21 23:20:57  pouaite
  coin

  Revision 1.14  2002/08/17 18:33:39  pouaite
  grosse commition

  Revision 1.13  2002/06/23 10:44:05  pouaite
  i18n-isation of the coincoin(kwakkwak), thanks to the incredible jjb !

  Revision 1.12  2002/06/01 17:54:04  pouaite
  nettoyage

  Revision 1.11  2002/05/27 18:39:14  pouaite
  trucs du week-end + patch de binny

  Revision 1.10  2002/05/19 01:14:59  pouaite
  bugfix du dernier bugfix.. ou pas.. chuis un peu trop fatigué pour faire des bugfix

  Revision 1.9  2002/04/01 01:39:38  pouaite
  grosse grosse commition (cf changelog)

  Revision 1.8  2002/03/24 23:26:38  pouaite
  patch de lordoric + bricoles à deux francs

  Revision 1.7  2002/03/21 22:53:07  pouaite
  ajout d'une icone pour la fenetre du pinnipede et des news

  Revision 1.6  2002/03/19 09:55:58  pouaite
  bugfixes compilation

  Revision 1.5  2002/03/05 21:04:28  pouaite
  bugfixes suite à l'upgrade de dlfp [et retour au comportement à l'ancienne du clic sur les horloges pour les moules ronchonnes]

  Revision 1.4  2002/03/03 10:10:04  pouaite
  bugfixes divers et variés

  Revision 1.3  2002/02/28 01:12:33  pouaite
  scrollcoin dans la fenetre des news

  Revision 1.2  2001/12/02 18:34:54  pouaite
  ajout de tags cvs Id et Log un peu partout...

*/

#include <libintl.h>
#define _(String) gettext (String)

#include "coincoin.h"
#include "picohtml.h"
#include "scrollcoin.h"
#include "coin_xutil.h"

#include "newswin.h"
#include "site.h"

/*
  ----------------------------------------------------------------------
  gestion de la fenetre d'affichage du texte des news
  ----------------------------------------------------------------------
*/

typedef struct _PHView {
  int x, y; /* dimensions et position de la zone d'affichage */
  int w, h;
  int decal; /* en pixels (qté de scroll) */
  PicoHtml *ph;

  int ph_h; /* hauteur en pixels du contenu de ph */

  /* les items correpondant au premier et dernier mots affichés
     (attention, ne sont valides qu'apres l'appel à newswin_draw_titlelist)

     (suppose un bon ordonnancement des htmlitems)
  */
  PicoHtmlItem *first, *last;
  ScrollCoin *sc;
} PHView;

struct _Newswin {
  Window window;
  int nb_comment;
  int err;
  char err_msg[200];
  int win_width, win_height; /* dim de la fenetre, en pixels */
  int win_xpos, win_ypos; 

  id_type id; /* news actuellrment affichee */
  char site_name[20];   /* nom du site de la news actuelle */


  GC gc;

  PHView phv_titles;
  PHView phv_news;
  
  PicoHtml *ph_survol;

  unsigned long win_bgpixel, win_fgpixel;
  unsigned long titles_bgpixel, titles_fgpixel[MAX_SITES], emph_pixel;

  struct struc_ztitles {
    int y, h;
    id_type nid;
    Site *site;
  } *ztitle;
  int nb_ztitle;
  int active_znum;
  Pixmap pix; /* un gros pixmap de la taille de la fenetre: c'est pas très classe */
};


/* construction du texte avec ses quelques attributs */
static void
newswin_parsetxt(Dock *dock, News *n)
{
  Newswin *nw = dock->newswin;
  unsigned char *buff;

  //BLAHBLAH(2,printf("newswin_parsetxt\n"));
  if (n->txt == NULL) {
    buff = strdup(_("The <b>text of the news</b> hasn't been updated yet..."));
  } else {
    char p1[512];
    Site *site;
    SitePrefs *sp;

    site = n->site;
    sp = (site ? site->prefs : NULL);
    snprintf(p1, 512, _("<B><a href=\"http://%s:%d%s%s\"><font color=#000080>%s</font></a></B><br>posted by "
	     "<b><font color=#800000>%s</font></b> on the %s, section <b>%s</b><p>"),
	     sp ? sp->site_root : "??", sp ? sp->site_port : 0, sp ? sp->site_path : "??", 
	     n->url,
	     n->titre, n->auteur, n->date, n->topic);
    ALLOC_VEC(buff, strlen(p1) + strlen(n->txt) + 1, unsigned char);
    strcpy(buff, p1); strcat(buff, n->txt);

    /* on signale que la news aura deja ete lue */
    site_newslues_add(site, id_type_lid(n->id));
  }

  if (n->url_path) {
    picohtml_set_url_path(nw->phv_news.ph, n->url_path);
  } else {
    picohtml_unset_url_path(nw->phv_news.ph);
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
      XSetForeground(dock->display, nw->gc, nw->emph_pixel);//dock->light_pixel);
      XFillRectangle(dock->display, d, nw->gc, phv->x-4, phv->y + y0, phv->w+8, y1-y0 + 1);
    }
  }

  xr.x = phv->x; xr.y = phv->y; xr.width = phv->w; xr.height = phv->h;
  XSetClipRectangles(dock->display, nw->gc, 0, 0, &xr, 1, Unsorted); /* faut pas que ça bave sur les bord :) */

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

  if (phv->sc) {
    //    printf("decal = %d\n", phv->decal);
    scrollcoin_setpos(phv->sc, phv->decal);
    scrollcoin_refresh(phv->sc, d, 1);
  }
}


void
newswin_draw(Dock *dock)
{
  Newswin *nw = dock->newswin;

  phview_draw(dock, nw->pix, &nw->phv_titles, nw->titles_bgpixel, 1);
  phview_draw(dock, nw->pix, &nw->phv_news, nw->win_bgpixel,0);
  XCopyArea(dock->display, nw->pix, nw->window, nw->gc, 0, 0, nw->win_width, nw->win_height, 0, 0);
}


/* renvoie les news dans un ordre descendant (bien sur c'est pas fait pour trier un milliard de news ..) */
static News *
get_news_sorted(Site *sl, News *previous, Site **psite)
{
  Site *site, *best_site = NULL;
  News *n, *best;
  
  site = sl;
  best = NULL;

  while (site) {
    n = site->news; 
    while (n) {
      int ok, cmp;
      
      /* on accepte de tester la news si c'est le premier appel (previous = NULL)
	 ou bien si la news est plus ancienne que previous */
      ok = 0;
      if (previous == NULL) ok = 1;
      else if (n != previous) {
	cmp = strcmp(previous->date, n->date);
	if (cmp > 0 || 
	    (cmp == 0 && (previous->heure > n->heure || 
			  (previous->heure == n->heure && 
			   id_type_lid(previous->id) > id_type_lid(n->id))))) ok = 1;
      }
      
      /* si on la teste, on la compare au meilleur candidat .. */
      if (ok) {
	if (best == NULL) { best = n; best_site = site; }
	else {
	  cmp = strcmp(best->date, n->date);
	  if (cmp < 0 || 
	      (cmp == 0 && (best->heure < n->heure ||
			    (best->heure == n->heure && 
			     id_type_lid(best->id) < id_type_lid(n->id))))) {
	    best = n;
	    best_site = site;
	  }
	}
      }
      n = n->next;
    }
    site = site->next;
  }
  if (psite) *psite = best_site;
  return best;
}

static void
newswin_adjust_scrollcoin(Dock *dock UNUSED, PHView *phv)
{
  if (phv->sc) {
    scrollcoin_setbounds(phv->sc, 0, MAX(10+phv->ph_h-phv->h,0));
    scrollcoin_resize(phv->sc, phv->x+phv->w+6, phv->y, phv->h);
  } else {
    phv->sc = scrollcoin_create(0, MAX(10+phv->ph_h-phv->h,0), 0, 
				phv->x+phv->w+6,
				phv->y, phv->h,0);
  }
}


  /* met à jour les données du contenu (nouvelles news, ou bien changement de la news affichée) */
void
newswin_update_content(Dock *dock, int reset_decal)
{
  Newswin *nw = dock->newswin;
  SiteList *slist = dock->sites;

  Site *site;
  News *n;
  int bidon;

#define BUFFSZ 8192
  char buff[BUFFSZ];
  
  /* création de la liste des titres des news */
  if (flag_updating_news) {
    printf(_("Updating the news...\n"));
    return;
  }

  if (!picohtml_isempty(nw->phv_titles.ph)) {
    picohtml_freetxt(nw->phv_titles.ph);
  }
  nw->phv_titles.ph_h = 0;
  n = NULL;
  buff[0] = 0;
  while ((n=get_news_sorted(slist->list,n, &site)) != NULL) {
    int l, unreaded;
    l = strlen(buff); assert(l <= BUFFSZ-1); if (l == BUFFSZ-1) break;
    unreaded = !site_newslues_find(n->site, id_type_lid(n->id));
    snprintf(buff+l, BUFFSZ-l, "<!special=%d>%s[%s] %s%s<br>", id_type_to_int(n->id), 
	     unreaded == 0 ? "" : "<b>",
	     site->prefs->site_name, n->titre, 
	     unreaded == 0 ? "" : "</b>");
  }
  picohtml_parse(dock, nw->phv_titles.ph, buff, nw->phv_titles.w);
  picohtml_gettxtextent(nw->phv_titles.ph, &bidon, &nw->phv_titles.ph_h);


  /* dectection des positions y des différents titres de news (c'est TRES porchou...) */
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
	nw->ztitle[nw->nb_ztitle].nid = int_to_id_type(it->special_attr);
	while (it && it->special_attr == id_type_to_int(nw->ztitle[nw->nb_ztitle].nid)) {
	  nw->ztitle[nw->nb_ztitle].h = it->fn->descent + it->y - nw->ztitle[nw->nb_ztitle].y;
	  it = it->next;
	}
	//	printf("ztitle[%d]: y=%d, h=%d, nid=%d\n", nw->nb_ztitle, nw->ztitle[nw->nb_ztitle].y, nw->ztitle[nw->nb_ztitle].h, nw->ztitle[nw->nb_ztitle].nid);
	
	if (!id_type_is_invalid(nw->id) &&
	    id_type_eq(nw->ztitle[nw->nb_ztitle].nid,nw->id)) {
	  nw->active_znum = nw->nb_ztitle;
	}
	nw->nb_ztitle++;
      } else {
	it = it->next;
      }
    }
  }

  if (nw->nb_ztitle > 0) { /* sinon, il peut y avoir un crash idiot dans l'assert de phwview_draw */
    nw->active_znum = MIN(nw->active_znum, nw->nb_ztitle);
  } else { nw->active_znum = -1; }

  nw->phv_news.ph_h = 0;
  if (!picohtml_isempty(nw->phv_news.ph)) {
    picohtml_freetxt(nw->phv_news.ph);
  }

  if (reset_decal) {
    nw->phv_news.decal = 0;
  }

  nw->phv_news.ph_h = 0;
  if (id_type_is_invalid(nw->id)) {
    printf(_("No news...\n"));
  } else {
    n = sl_find_news(slist, nw->id);
    if (n == NULL) {
      printf(_("This news was destroyed (site=%d,id=%d)!?...\n"),
	     id_type_sid(nw->id), id_type_lid(nw->id));
    } else {
      newswin_parsetxt(dock, n);
      picohtml_gettxtextent(nw->phv_news.ph, &bidon, &nw->phv_news.ph_h);
    }
  }
  newswin_adjust_scrollcoin(dock, &nw->phv_titles);
  newswin_adjust_scrollcoin(dock, &nw->phv_news);
}

static void
newswin_adjust_size(Dock *dock)
{
  Newswin *nw = dock->newswin;

  nw->win_width = MIN(nw->win_width, 2000);
  nw->win_height = MIN(nw->win_height, 2000); /* ne PAS faire exploser le serveur X avec un pixmap 10000x10000 ... */

  nw->phv_titles.x = 10; nw->phv_titles.w = 170;
  nw->phv_titles.y = 11; nw->phv_titles.h = nw->win_height - 12 - nw->phv_titles.y;

  nw->phv_news.x = nw->phv_titles.x + nw->phv_titles.w + 20;
  nw->phv_news.w = nw->win_width - nw->phv_news.x - 20;
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

  newswin_adjust_scrollcoin(dock, &nw->phv_titles);
  newswin_adjust_scrollcoin(dock, &nw->phv_news);
}

static void
newswin_createXWindow(Dock *dock)
{
  Newswin *nw = dock->newswin;

  XTextProperty window_title_property;
  char *window_title;
  XSizeHints* win_size_hints;
  XClassHint *class_hint;
  XWMHints *wm_hint;
  int rc;
  char s[512];
  int xpos, ypos;

  assert(nw->win_width>0 && nw->win_height>0);
  BLAHBLAH(2, printf(_("newswin_createXWindow: creating a window of dimensions %dx%d\n"),
		     nw->win_width, nw->win_height));

  if (nw->win_xpos == -10000 && nw->win_ypos == -10000) {
    xpos = 0; ypos = 0; /* ça n'a d'effet que sur certain windowmanagers rustiques (genre pwm) */
  } else {
    xpos = nw->win_xpos;
    ypos = nw->win_ypos;
  }
  

  nw->window = XCreateSimpleWindow(dock->display, dock->rootwin, xpos, ypos, nw->win_width, nw->win_height, 0, 
				   nw->win_bgpixel, nw->win_bgpixel);

  XSelectInput(dock->display, nw->window, 
	       ButtonPressMask | ExposureMask | ButtonReleaseMask | PointerMotionMask | 
	       EnterWindowMask | LeaveWindowMask | StructureNotifyMask);

  snprintf(s, 512, _("news from %s"), nw->site_name);
  window_title = s;

  /* nom de la fenetre (et de la fenetre iconifiée) */
  rc = XStringListToTextProperty(&window_title,1, &window_title_property); assert(rc);
  XSetWMName(dock->display, nw->window, &window_title_property);
  XSetWMIconName(dock->display, nw->window, &window_title_property);
  XFree(window_title_property.value);


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
  sprintf(s, "wmcoincoin_news");
  class_hint->res_class = s;
  XSetClassHint(dock->display, nw->window, class_hint);
  XFree(class_hint);

  wm_hint = XAllocWMHints(); assert(wm_hint);
  wm_hint->icon_pixmap = dock->wm_icon_pix;
  wm_hint->icon_mask = dock->wm_icon_mask;
  wm_hint->flags = IconPixmapHint | IconMaskHint;
  XSetWMHints(dock->display, nw->window, wm_hint);
  XFree(wm_hint);

  /* pour etre informé de la fermeture de la fenetre demandee par le windowmanager */
  XSetWMProtocols(dock->display, nw->window, &dock->atom_WM_DELETE_WINDOW, 1);
  
  /* gc, fontes */
  nw->gc = XCreateGC(dock->display, nw->window, 0, NULL);
  XSetForeground(dock->display, nw->gc, nw->win_fgpixel);
  XSetBackground(dock->display, nw->gc, nw->win_bgpixel);
  XSetLineAttributes(dock->display, nw->gc, 1, LineSolid, CapButt, JoinBevel);
  XSetFillStyle(dock->display, nw->gc, FillSolid);

  XMapRaised(dock->display, nw->window);
}

void
newswin_show(Dock *dock, id_type id)
{
  Newswin *nw = dock->newswin;
  int create = 0;

  if (!id_type_is_invalid(id)) {
    nw->id = id; /* pas beau, c'est pour l'appel par clic droit (wmcoincoin.c ligne 1327)*/
  }

  if (nw->window == None) {
    newswin_createXWindow(dock);
    create = 1;
  }
  newswin_adjust_size(dock);
  nw->active_znum = -1;
  assert(nw->window != None);
  newswin_update_content(dock, 1);
  if (create == 0) newswin_draw(dock);
}

void
newswin_unmap(Dock *dock)
{
  Newswin *nw = dock->newswin;
  
  assert(nw->window != None);
  XDestroyWindow(dock->display, nw->window);
  nw->window = None;

  XFreeGC(dock->display, nw->gc);

  if (!picohtml_isempty(nw->phv_titles.ph)) 
    picohtml_freetxt(nw->phv_titles.ph);
  if (nw->phv_titles.sc) {
    scrollcoin_destroy(nw->phv_titles.sc); nw->phv_titles.sc = NULL;
  }
  if (!picohtml_isempty(nw->phv_news.ph))
    picohtml_freetxt(nw->phv_news.ph);
  if (nw->phv_news.sc) {
    scrollcoin_destroy(nw->phv_news.sc); nw->phv_news.sc = NULL;
  }
  if (nw->nb_ztitle > 0) {
    assert(nw->ztitle);
    free(nw->ztitle); nw->ztitle = NULL; nw->nb_ztitle = 0;
  }
  if (nw->pix != None) {
    XFreePixmap(dock->display, nw->pix); nw->pix = None;
  }
}

/* renvoie le numero du titre de news survolle en (mx,my) */
static int
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

static PicoHtmlItem *
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

/* infos affichées selon la position de la souris */
void
newswin_update_info(Dock *dock, int mx, int my) {
  Newswin *nw = dock->newswin;
  PicoHtmlItem *pi;
  SiteList *slist = dock->sites;
  char survol[1024];
  static int old_hash = 0;
  int hash;
  char *p;

  survol[0] = 0; 
  if (flag_updating_news) {
    strcpy(survol, _("Updating the news..."));
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
    News *n;
    znum = newswin_get_znum(dock, mx, my);
    if (znum != -1) {
      id_type id = nw->ztitle[znum].nid;
      n = sl_find_news(slist, id);
      if (n) {
	snprintf(survol, 1024, _("id=<b>%d</b>, date: %s, approved on %02d:%02d, %s"), 
		 id_type_lid(n->id), n->date, n->heure / 60, n->heure % 60,
		 (site_newslues_find(n->site, id_type_lid(n->id))) ? 
		 _("(already read)"):
		 _("<b><font color=#8f0000>(new!)</font></b>"));
      } else {
	snprintf(survol, 1024, _("[%s] id=<b>%d</b>, contents of the news unavailable (too old ? download in progress ?)"), Prefs.site[id_type_sid(id)]->site_name, id_type_lid(id));
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

      XSetForeground(dock->display, nw->gc, nw->emph_pixel);
      XFillRectangle(dock->display, nw->window, nw->gc, x, y, nw->win_width-x, nw->win_height-y);
      XSetForeground(dock->display, nw->gc, BlackPixel(dock->display, dock->screennum));
      XDrawRectangle(dock->display, nw->window, nw->gc, x, y, nw->win_width-x, nw->win_height-y);
      picohtml_render(dock, ph, nw->window, nw->gc, x+3, y-ph->fn_base->descent+1);
      picohtml_freetxt(ph);
    }
  }
}

static void
newswin_handle_button_press(Dock *dock, XButtonEvent *event)
{
  Newswin *nw = dock->newswin;
  if (nw->phv_titles.sc && 
      scrollcoin_handle_button_press(nw->phv_titles.sc, event, nw->pix)) {
  } else if (nw->phv_news.sc && 
	     scrollcoin_handle_button_press(nw->phv_news.sc, event, nw->pix)) {
  }
}

static void
newswin_testscroll(Dock *dock)
{
  Newswin *nw = dock->newswin;
  int pos, redraw = 0;
  if (nw->phv_titles.sc && scrollcoin_read_requested_pos(nw->phv_titles.sc, &pos)) {
    if (nw->phv_titles.decal != pos) {
      //      printf("scrollup: decal <- %d\n", pos);
      nw->phv_titles.decal = pos; redraw = 1;
    }
  }
  if (nw->phv_news.sc && scrollcoin_read_requested_pos(nw->phv_news.sc, &pos)) {
    if (nw->phv_news.decal != pos) {
      nw->phv_news.decal = pos; redraw = 1;
    }
  }
  if (redraw) newswin_draw(dock);
}

static void
newswin_scrollup(Dock *dock, PHView *phv, int q)
{
  phv->decal = MAX(0, phv->decal-q);
  //  printf("scrollup: decal <- %d\n", phv->decal);
  newswin_draw(dock);
}

static void
newswin_scrolldown(Dock *dock, PHView *phv, int q)
{
  phv->decal = MIN(phv->decal + q, phv->ph_h +10 - phv->h);
  phv->decal = MAX(0, phv->decal);
  //  printf("scrolldown: decal <- %d\n", phv->decal);
  newswin_draw(dock);
}


static void
newswin_handle_button_release(Dock *dock, XButtonEvent *event) {
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

  if (nw->phv_titles.sc && 
      scrollcoin_handle_button_release(nw->phv_titles.sc, event, nw->pix)) {
  } else if (nw->phv_news.sc && 
	     scrollcoin_handle_button_release(nw->phv_news.sc, event, nw->pix)) {
  } else if (event->button == Button4 && phv) {
    newswin_scrollup(dock, phv, 20);
  } else if (event->button == Button5 && phv) {
    newswin_scrolldown(dock, phv, 20);
  } else if (in_titles && event->button == Button1) {
    int znum;

    znum = newswin_get_znum(dock, mx, my);
    if (znum != -1) {
      newswin_show(dock, nw->ztitle[znum].nid);
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
	//	printf("copy! %s\n",pi->link_str);
	editw_cb_copy(dock, nw->window, pi->link_str, strlen(pi->link_str));
      }
    }
  }
  /*
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
    }*/

  newswin_testscroll(dock);
}

void
newswin_dispatch_event(Dock *dock, XEvent *event)
{
  Newswin *nw = dock->newswin;

  if (flag_news_updated) { newswin_update_content(dock, 1); flag_news_updated = 0; }

  

  if (nw->window != None) {
    switch (event->type) {
    case DestroyNotify:
      BLAHBLAH(2,printf(_("newswin destroy !\n")));
      break;
    case ButtonPress:
      {
	newswin_handle_button_press(dock, &event->xbutton);
      } break;
    case ButtonRelease:
      {
	newswin_handle_button_release(dock, &event->xbutton);
      } break;
    case MotionNotify:
      {
	newswin_update_info(dock, event->xmotion.x, event->xmotion.y);
	if (nw->phv_titles.sc &&
	    scrollcoin_handle_motion(nw->phv_titles.sc, &event->xmotion, nw->pix)) {
	} else if (nw->phv_news.sc && 
		   scrollcoin_handle_motion(nw->phv_news.sc, &event->xmotion, nw->pix)) {
	}
	newswin_testscroll(dock);
      } break;
    case EnterNotify:
    case LeaveNotify:
      {
	newswin_update_info(dock, event->xcrossing.x, event->xcrossing.y);
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
	    newswin_update_content(dock, 1);
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
  nw->titles_bgpixel = IRGB2PIXEL(Prefs.news_titles_bgcolor);

  {
    int i;
    for (i=0; i < MAX_SITES; i++) {
      if (Prefs.site[i]) 
	nw->titles_fgpixel[i] = IRGB2PIXEL(Prefs.news_titles_fgcolor);
    }
  }
  nw->emph_pixel = IRGB2PIXEL(Prefs.news_emph_color);
  nw->phv_news.ph = picohtml_create(dock, Prefs.news_fn_family, Prefs.news_fn_size, 1);
  picohtml_set_default_pixel_color(nw->phv_news.ph, nw->win_fgpixel);

  nw->phv_news.sc = NULL;

  nw->phv_titles.ph = picohtml_create(dock, Prefs.news_fn_family, Prefs.news_fn_size, 1);
  picohtml_set_default_pixel_color(nw->phv_titles.ph, IRGB2PIXEL(0));

  nw->phv_titles.sc = NULL;

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
  nw->id = id_type_invalid_id();
  nw->site_name[0] = 0;
  dock->newswin = nw;
}

void
newswin_destroy(Dock *dock) {
  Newswin *nw = dock->newswin;
  if (newswin_is_used(dock)) newswin_unmap(dock);
  picohtml_destroy(dock->display, nw->phv_news.ph);
  picohtml_destroy(dock->display, nw->phv_titles.ph);
  picohtml_destroy(dock->display, nw->ph_survol);
  free(nw); dock->newswin = NULL;
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

/* sauvegarde de la position et des dimensions de la fenetre
   (appelé par wmcc_save_state de wmcoincoin.c) 
 */
void
newswin_save_state(Dock *dock, FILE *f) {
  const Newswin *nw = dock->newswin;

  fprintf(f, "%d %d %d %d %d\n", 
	  nw->window == None ? 0 : 1,
	  nw->win_xpos, nw->win_ypos, 
	  nw->win_width, nw->win_height);
}

void
newswin_restore_state(Dock *dock, FILE *f) {
  Newswin *nw = dock->newswin;
  int mapped, win_xpos, win_ypos, win_width, win_height;

  if (fscanf(f, "%d %d %d %d %d\n", 
	     &mapped,
	     &win_xpos, &win_ypos, 
	     &win_width, &win_height) == 5) {
    /* on vérifie qu'on n'a pas spécifié de préferences dans le fichier d'options */
    if (Prefs.news_xpos == -10000 && Prefs.news_ypos == -10000) {
      if (win_xpos != -10000) {
	nw->win_xpos = MAX(MIN(win_xpos,3000),-20);
	nw->win_ypos = MAX(MIN(win_ypos,3000),-20);
	nw->win_width = MAX(MIN(win_width,3000),50);
	nw->win_height = MAX(MIN(win_height,3000),50);
      }
      /*
	if (mapped) {
	  newswin_show(dock, &dock->slist, -2);
	}
      */
    }
  }
}
