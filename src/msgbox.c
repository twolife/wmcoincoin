/*
    wmCoinCoin, a braindamaged WindowMaker dock applet

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
  rcsid=$Id: msgbox.c,v 1.7 2003/01/19 18:52:23 pouaite Exp $
  ChangeLog:
  $Log: msgbox.c,v $
  Revision 1.7  2003/01/19 18:52:23  pouaite
  patch gle (couleur de fond du palmi)

  Revision 1.6  2002/08/17 18:33:39  pouaite
  grosse commition

  Revision 1.5  2002/06/23 10:44:05  pouaite
  i18n-isation of the coincoin(kwakkwak), thanks to the incredible jjb !

  Revision 1.4  2002/04/01 01:39:38  pouaite
  grosse grosse commition (cf changelog)

  Revision 1.3  2001/12/18 12:43:37  pouaite
  ajout de l'option de la fonte des ballons d'aide (pour mr. imr !) + bugfix d'une connerie assez naze dans la gestion du nom du fichier d'options (merci glandium de me l'avoir signalé)

  Revision 1.2  2001/12/02 18:34:54  pouaite
  ajout de tags cvs Id et Log un peu partout...

*/


#include <libintl.h>
#define _(String) gettext (String)


#include <X11/Xlib.h>
#include "coincoin.h"
#include "coin_xutil.h"
#include "dock.h"

struct _MsgBox {
  Window win;
  PicoHtml *ph;
  int w,h;
  int mapped;
  unsigned long bg_pixel, dark_pixel, light_pixel, bg_title_pixel;
};


#define TOP	0
#define BOTTOM	1
#define LEFT	0
#define RIGHT	2

#define 	SPACE	12


static void
msgbox_refresh(Dock *dock, Drawable d)
{
  MsgBox *m = dock->msgbox;
  const char *title;
  int tw;

  title = str_printf("%s " VERSION,_("MESSAGE FROM WMCOINCOIN"));

  if (m->mapped == 0) return;
  //printf("refresh!\n");
  
  tw = XTextWidth(picohtml_get_fn_bold(m->ph), title, strlen(title));
 

  XSetForeground(dock->display, dock->NormalGC, m->bg_pixel);
  XFillRectangle(dock->display, d, dock->NormalGC, 1, 13, m->w-1, m->h-1);

  XSetForeground(dock->display, dock->NormalGC, m->bg_title_pixel);
  XFillRectangle(dock->display, d, dock->NormalGC, 1, 1, m->w-2, 12);
  XSetForeground(dock->display, dock->NormalGC, WhitePixel(dock->display, dock->screennum));
  XSetFont(dock->display, dock->NormalGC, picohtml_get_fn_bold(m->ph)->fid);
  XDrawString(dock->display, d, dock->NormalGC, m->w/2-tw/2, 11, title, strlen(title)); 

  XSetForeground(dock->display, dock->NormalGC, m->dark_pixel);
  XDrawLine(dock->display, d, dock->NormalGC, 0, m->h-1, m->w-1, m->h-1);
  XDrawLine(dock->display, d, dock->NormalGC, m->w-1, 0, m->w-1, m->h-1);
  XSetForeground(dock->display, dock->NormalGC, m->light_pixel);
  XDrawLine(dock->display, d, dock->NormalGC, 0, 0, m->w-1, 0);
  XDrawLine(dock->display, d, dock->NormalGC, 0, 0, 0, m->h-1);
  picohtml_render(dock, m->ph, d, dock->NormalGC, 5, 13);
  free(title);
}


void
msgbox_build(Dock *dock)
{
  MsgBox *m;

  ALLOC_OBJ(m, MsgBox);

  m->bg_pixel = dock->bg_pixel;
  m->dark_pixel = dock->dark_pixel;
  m->light_pixel = dock->light_pixel;
  m->bg_title_pixel = RGB2PIXEL(0, 50, 200);
  m->mapped = 0;
  m->ph = NULL;
  dock->msgbox = m;
}

void
msgbox_show(Dock *dock, char *text)
{
  XSetWindowAttributes wa;
  MsgBox *m;
  //  int scr_width, scr_height;
  int iconx, icony, mx, my;

  m = dock->msgbox;
  
  //  fprintf(stderr, "MSGBOX: %s\n", text);
  if (m->mapped) {
    msgbox_hide(dock);
  }

  
  assert(m->ph == NULL);
  m->ph = picohtml_create(dock, Prefs.balloon_fn_family, Prefs.balloon_fn_size, 0);
  picohtml_set_parag_skip(m->ph, 1.0);
  picohtml_set_line_skip(m->ph, 1.0);
  picohtml_set_parag_indent(m->ph, 0);
  picohtml_set_tabul_skip(m->ph, 10);

  m->w = 300;
  picohtml_parse(dock, m->ph, text, m->w);
  picohtml_gettxtextent(m->ph, &m->w, &m->h);

  m->w = MAX(m->w, 150);

  m->h += 20; m->w += 10;
  //  scr_width = WidthOfScreen(XScreenOfDisplay(dock->display, dock->screennum));
  //  scr_height = HeightOfScreen(XScreenOfDisplay(dock->display, dock->screennum));

  dock_get_icon_pos(dock, &iconx, &icony);

  if (iconx < m->w+10) {
    mx = iconx + 70;
  } else {
    mx = iconx - m->w - 10;
  }
  if (icony < m->h+10) {
    my = icony + 70;
  } else {
    my = icony - m->h - 10;
  }

  /* creation fenetre (une bonne fois pour toutes) */
  m->win = XCreateSimpleWindow (dock->display, RootWindow(dock->display,dock->screennum), 
				mx, my, m->w, m->h, 0,
				m->dark_pixel,
				m->bg_pixel);
  
  //wa.background_pixmap = ButtonBarImage ;
  wa.event_mask =
    ExposureMask | 
    ButtonPressMask | 
    ButtonReleaseMask;
  wa.override_redirect = True ;
  wa.save_under = True;
  XChangeWindowAttributes (dock->display, m->win,
			   CWSaveUnder | CWEventMask | CWOverrideRedirect, &wa);


  m->mapped = 1;
  XMapRaised(dock->display, m->win);
  //  msgbox_refresh(dock);
}

void
msgbox_hide(Dock *dock)
{
  MsgBox *m = dock->msgbox;

  BLAHBLAH(3, printf(_("close msgbox\n")));
  if (m->mapped) {
    m->mapped = 0;
    
    picohtml_destroy(dock->display, m->ph); m->ph = NULL;
    XDestroyWindow(dock->display, m->win);
    m->win = None;
  }
}

int 
msgbox_ismapped(Dock *dock)
{
  return dock->msgbox->mapped;
}

Window
msgbox_get_win(Dock *dock)
{
  return dock->msgbox->win;
}


void 
msgbox_dispatch_event(Dock *dock, XEvent *event)
{
  MsgBox *m = dock->msgbox;

  if (m->mapped) {
    switch (event->type) {
    case Expose:
      {
	if (event->xexpose.count == 0) {
	  msgbox_refresh(dock, m->win);
	}
      } break;
      //    case ButtonPress:
    case ButtonRelease:
      {
	msgbox_hide(dock);
      } break;
    }
  }
}

void
msgbox_show_modal(Dock *dock, char *text)
{
  MsgBox *m = dock->msgbox;

  XEvent event;
  msgbox_show(dock, text);
  while (m->mapped) {
    XNextEvent(dock->display, &event);
    if (event.xany.window == m->win) {
      msgbox_dispatch_event(dock, &event);
    }
  }
}
