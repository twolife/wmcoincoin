/*
    wmCoinCoin, the stupidest WindowMaker dock applet
    Copyright (C) 2001  Julien Pommier

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
  contient les fonction gérant l'affichage de l'applet
  ainsi que les évenements

  rcsid=$Id: dock.c,v 1.14 2002/05/27 18:39:14 pouaite Exp $
  ChangeLog:
  $Log: dock.c,v $
  Revision 1.14  2002/05/27 18:39:14  pouaite
  trucs du week-end + patch de binny

  Revision 1.13  2002/05/19 01:14:58  pouaite
  bugfix du dernier bugfix.. ou pas.. chuis un peu trop fatigué pour faire des bugfix

  Revision 1.12  2002/04/26 08:55:27  pouaite
  fix pour le mode horloge avec l'option '-w'/dock.iconwin (merci gle!)

  Revision 1.11  2002/04/13 11:55:19  pouaite
  fix kde3 + deux trois conneries

  Revision 1.10  2002/04/09 00:28:19  pouaite
  quelques modifs faites dans un état d'hébétude avancé /!\ travaux en cours /!\

  Revision 1.9  2002/04/04 20:42:51  pouaite
  replop

  Revision 1.8  2002/04/01 22:56:03  pouaite
  la pseudo-transparence du pinni, bugfixes divers, option tribune.backend_type

  Revision 1.7  2002/04/01 01:39:38  pouaite
  grosse grosse commition (cf changelog)

  Revision 1.6  2002/03/19 09:55:58  pouaite
  bugfixes compilation

  Revision 1.5  2002/03/09 19:45:52  pouaite
  microbugfix du plopifieur et ajout d'une macro PATCH_LEVEL

  Revision 1.4  2002/02/24 22:13:56  pouaite
  modifs pour la v2.3.5 (selection, scrollcoin, plopification, bugfixes)

  Revision 1.3  2002/02/02 23:49:17  pouaite
  plop

  Revision 1.2  2002/01/31 23:45:00  pouaite
  plop

  Revision 1.1  2002/01/16 21:27:35  pouaite
  gros coup de balai dans wmcoincoin.c qui s'est du coup splitté en trois: wmcoincoin.c, dock.c et useragents_file.c

*/

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/keysym.h>
#include <X11/cursorfont.h>
#include <X11/extensions/shape.h>
#include <time.h>
#include <sys/time.h>
#include "coincoin.h"
#include "coin_xutil.h"
#include "http.h"

/* image */
#include "../xpms/leds.h"
#include "../xpms/porte.h"

/* au max un defilement toutes les 15 secondes */
#define TROLLO_MAX_SPEED 15

void
dock_update_pix_trolloscope(Dock *dock, DLFP_tribune *trib)
{
  tribune_msg_info *it;
  int tnow;
  int i,j;
  RGBAImage *img;
  int trolloscope_bgr, trolloscope_bgg, trolloscope_bgb;
  
  

  /* nombre de secondes regroupees dans une meme colonne du graphique */
  int col_nb_sec = 0;
  int trib_nrow, trib_ncol;

  if (flag_updating_tribune) return;

  dock->tribune_updatable = 0; /* ça c'est des vieux restes de l'époque des threads.. A VIRER UN DE CES QUATRES */

  /* couleur de fond du trolloscope */
  trolloscope_bgr = (280*(dock->flamometre.comment_change_decnt % FLAMOMETRE_COMMENT_CLIGN_SPEED))/(FLAMOMETRE_COMMENT_CLIGN_SPEED-1);
  trolloscope_bgg = (280*(dock->flamometre.xp_change_decnt % FLAMOMETRE_XP_CLIGN_SPEED))/(FLAMOMETRE_XP_CLIGN_SPEED-1);
  trolloscope_bgb = (280*(dock->flamometre.tribune_answer_decnt % FLAMOMETRE_TRIB_CLIGN_SPEED))/(FLAMOMETRE_TRIB_CLIGN_SPEED-1);
  if (trolloscope_bgr>255) { trolloscope_bgr = ((280-trolloscope_bgr)*255)/(280-255); }
  if (trolloscope_bgg>255) { trolloscope_bgg = ((280-trolloscope_bgg)*255)/(280-255); }
  if (trolloscope_bgb>255) { trolloscope_bgb = ((280-trolloscope_bgb)*255)/(280-255); }

  if (dock->trolloscope_resolution == 5) {
    col_nb_sec = 1*dock->trolloscope_speed*TROLLO_MAX_SPEED;
  } else if (dock->trolloscope_resolution == 2) {
    col_nb_sec = 2*dock->trolloscope_speed*TROLLO_MAX_SPEED;
  } else if (dock->trolloscope_resolution == 1) {
    col_nb_sec = 4*dock->trolloscope_speed*TROLLO_MAX_SPEED;
  } else assert(0);

  trib_nrow = (TROLLOSCOPE_HEIGHT + dock->trolloscope_resolution - 1) / dock->trolloscope_resolution;
  trib_ncol = (TROLLOSCOPE_WIDTH  + dock->trolloscope_resolution - 1) / dock->trolloscope_resolution;

  for (i=0; i < trib_nrow; i++) {
    for (j=0; j < trib_ncol; j++) {
      dock->trolloscope[i][j].id = -1;
      dock->trolloscope[i][j].tatouage = NULL;
    }
  }

  it = trib->msg;

  tnow = (tribune_get_time_now(trib) + col_nb_sec - 1)/col_nb_sec;

  /* construction de trolloscope */
  while (it) {
    if (it->tatouage) {
      int age;

      /* age = tribune_get_msg_age(trib, it) / 60 / col_nb_min; */
      age = (tnow - (it->timestamp + col_nb_sec - 1)/col_nb_sec + ((24*60*60)/col_nb_sec))%((24*60*60)/col_nb_sec);
      BLAHBLAH(4, myprintf("id=%<YEL %d>, age=%<RED %d> ts=%d, col_nb_sec=%d, tnow=%d\n", it->id, age,it->timestamp,col_nb_sec,tnow));
      assert(age >= 0);
      if (age < trib_ncol) {
	/* on empile les message sur la pile d'age 'age' (je suis clair?)*/
	i = 0;
	while (dock->trolloscope[i][age].id != -1) {
	  i++; if (i == trib_nrow) break;
	}
	if (i < trib_nrow) {
	  dock->trolloscope[i][age].id = it->id;
	  dock->trolloscope[i][age].tatouage = it->tatouage;
	}
      }
    }
    it = it->next;
  }

  img = RGBACreateImage(TROLLOSCOPE_WIDTH, TROLLOSCOPE_HEIGHT); assert(img);
  /*
  for (i=0; i < TROLLOSCOPE_HEIGHT; i++) {
    for (j=0; j < TROLLOSCOPE_WIDTH; j++) {
      img->data[(i*TROLLOSCOPE_WIDTH  + j)*3 + 0] = trolloscope_bgr; 
      img->data[(i*TROLLOSCOPE_WIDTH  + j)*3 + 1] = trolloscope_bgg; 
      img->data[(i*TROLLOSCOPE_WIDTH  + j)*3 + 2] = trolloscope_bgg; 
    }
    }*/
  for (i=0; i < trib_nrow; i++) {
    for (j=0; j < trib_ncol; j++) {
      int r,g,b,symb;

      if (dock->trolloscope[i][j].id >= 0 && dock->trolloscope[i][j].tatouage) {
	r = dock->trolloscope[i][j].tatouage->R;
	g = dock->trolloscope[i][j].tatouage->G;
	b = dock->trolloscope[i][j].tatouage->B;
	symb = dock->trolloscope[i][j].tatouage->symb;
      } else { 
	r = trolloscope_bgr; g = trolloscope_bgg; b = trolloscope_bgb; symb = 0;
	dock->trolloscope[i][j].id = 0;
	dock->trolloscope[i][j].tatouage = NULL;
      }
      BLAHBLAH(5, myprintf("[%<YEL %1d>%<RED %02x>] ", symb, (r+g+b)%256));
      if (dock->trolloscope_resolution == 5) {
	int ii,jj;
	for (ii=0; ii < 5; ii++) {
	  for (jj=0; jj < 5; jj++) {
	    if (i*5+ii < TROLLOSCOPE_HEIGHT &&  TROLLOSCOPE_WIDTH-1-(j*5+jj) >= 0) {
	      RGBAPixel *pixel;
	      pixel = &img->data[ TROLLOSCOPE_HEIGHT-1-(i*5+ii) ][ TROLLOSCOPE_WIDTH-1-(j*5+jj) ];
	      switch (symboles[symb].s[ii][jj]) {
	      case ' ': 
		pixel->rgba[0] = trolloscope_bgr; 
		pixel->rgba[1] = trolloscope_bgg; 
		pixel->rgba[2] = trolloscope_bgb; 
		break;
	      case '.': 
		pixel->rgba[0] = (r/2); 
		pixel->rgba[1] = (g/2); 
		pixel->rgba[2]=(b/2); break;
	      case ':': pixel->rgba[0] = ((r*2)/3); pixel->rgba[1] = ((g*2)/3); pixel->rgba[2]=((b*2)/3); break;
	      case 'x': pixel->rgba[0] = ((r*3)/4); pixel->rgba[1] = ((g*3)/4); pixel->rgba[2]=((b*3)/4); break;
	      case 'X': pixel->rgba[0] = r; pixel->rgba[1] = g; pixel->rgba[2]=b; break;
	      case '#': pixel->rgba[0] = MIN((r*5)/4,255); pixel->rgba[1] = MIN((g*5)/4,255); pixel->rgba[2]=MIN((b*5)/4,255); break;
	      default: assert(1==0); /* autant dire que j'ai pas envie que ca arrive */
	      }
	    }
	  }
	}
      } else if(dock->trolloscope_resolution == 2) {
	int ii,jj;
	for (ii=0; ii < 2; ii++) {
	  for (jj=0; jj < 2; jj++) {
	    if (i*2+ii < TROLLOSCOPE_HEIGHT && TROLLOSCOPE_WIDTH-1-(j*2+jj) >= 0) {
	      RGBAPixel *pixel;
	      pixel = &img->data[ TROLLOSCOPE_HEIGHT-1-(i*2+ii) ][ TROLLOSCOPE_WIDTH-1-(j*2+jj) ];
	      pixel->rgba[0] = r; pixel->rgba[1] = g; pixel->rgba[2] = b;
	    }
	  }
	}           
      } else if (dock->trolloscope_resolution == 1) {
	RGBAPixel *pixel;
	pixel = &img->data[ TROLLOSCOPE_HEIGHT-1-i ][ trib_ncol-1-j ];
	pixel->rgba[0] = r; pixel->rgba[1] = g; pixel->rgba[2] = b;
      }
    }
    BLAHBLAH(5,myprintf("\n"));
  }

  if (dock->tl_item_survol) { 
    /* ce fut un bug idiot */
    if (dock->tl_item_survol->id <= 0) {
      dock->tl_item_survol = NULL;
      dock->tl_item_clicked = 0;
    }
  }

  /* et voila ! y'a plus qu'a fabriquer le pixmap */
  XFreePixmap(dock->display, dock->pix_trolloscope);
  dock->pix_trolloscope = RGBAImage2Pixmap(dock->rgba_context, img);
  assert(dock->pix_trolloscope != None);
  RGBADestroyImage(img);
  dock->tribune_updatable = 1;
}

static void
textout_msg(Dock *dock, unsigned char *msg, int x, int y, int w)
{
  int cx, cnt, dec;
  XRectangle xr;

  xr.x = x; xr.y = y; xr.width = w; xr.height = 11;
  XSetForeground(dock->display, dock->NormalGC, IRGB2PIXEL(Prefs.dock_fgcolor));
  XSetFont(dock->display, dock->NormalGC, dock->fixed_font->fid);
  XSetClipRectangles(dock->display, dock->NormalGC, 0, 0, &xr, 1, Unsorted); /* faut pas que ça bave sur les bord :) */
  if (dock->flag_survol_trollo) {
    unsigned char minimsg[10];
    int tscore = (int)(dock->trib_trollo_score*3);
    if (tscore < 1000) {
      snprintf(minimsg, 10, "troll:%3d", tscore);
    } else {
      snprintf(minimsg, 10, "trl:%5d", tscore);
    }
    cx = x + (56-strlen(minimsg)*DOCK_FIXED_FONT_W)/2; /* super centrage..*/
    XDrawString(dock->display, dock->coinpix, dock->NormalGC, cx, y+dock->fixed_font->ascent+1, minimsg, strlen(minimsg));

  } else if (dock->view_id_in_newstitles == 0) {
    dec = dock->newstitles_char_dec;
    cx = x - dec;
    cnt = dock->newstitles_pos;
    /* if (msg[cnt] == 0) return; */
    do {
      XDrawString(dock->display, dock->coinpix, dock->NormalGC, cx, y+dock->fixed_font->ascent+1, &msg[cnt], 1);
      /* XCopyArea(dock->display, pixmap_letters, dock->coinpix, dock->and_GC, char2bitmap[msg[cnt]]+dec,0,MIN(cx+6, x+w)-MAX(cx,x),12,MAX(cx,x),y); */
      cx += 6;
      cnt++; if (msg[cnt] == 0) cnt = 0;
      dec = 0;
    } while (cx < x+w);
  } else {
    unsigned char minimsg[10];
    minimsg[0] = 0;
    if (dock->view_id_timer_cnt % 40 > 15) {
      snprintf(minimsg, 10, "id=%d", dock->view_id_in_newstitles); 
    } else if (dock->tl_item_survol) {
      tribune_msg_info *mi;
      mi = tribune_find_id(&dock->dlfp->tribune, dock->tl_item_survol->id);
      if (mi) {
	struct tm *t;

	/* remarque c'est plus rentrant (depuis la v2.3.5) mais on s'en bat les ouilles
	   puisque coincoin n'est pas multithreadé */
	t = localtime(&mi->timestamp);
	/* snprintf(minimsg, 10, "%02d:%02d", (int)((mi->timestamp/3600)%24), (int)((mi->timestamp/60)%60)); */
	snprintf(minimsg, 10, "%02d:%02d:%02d", t->tm_hour, t->tm_min, t->tm_sec);
      } else {
	strcpy(minimsg, "??:?? BUG");
      }
    }
    if (strlen(minimsg)) {
      cx = x + (56-strlen(minimsg)*DOCK_FIXED_FONT_W)/2; /* super centrage..*/
      
      XDrawString(dock->display, dock->coinpix, dock->NormalGC, cx, y+dock->fixed_font->ascent+1, minimsg, strlen(minimsg));
    }
  }
  XSetClipMask(dock->display, dock->NormalGC, None);
}

static void
textout_msginfo(Dock *dock, int x, int y)
{
  int cx, cnt;
  char c;
  XRectangle xr;

  if (strlen(dock->msginfo)==0) return;

  xr.x = x; xr.y = y; xr.width = 56; xr.height = 11;
  XSetFont(dock->display, dock->NormalGC, dock->fixed_font->fid);
  XSetForeground(dock->display, dock->NormalGC, IRGB2PIXEL(Prefs.dock_fgcolor));
  XSetClipRectangles(dock->display, dock->NormalGC, 0, 0, &xr, 1, Unsorted); /* faut pas que ça bave sur les bord :) */

  if (dock->msginfo_defil) {
    int dec;
    dec = (dock->msginfo_defil - 1) % 6;
    cx = x - dec;
    cnt = (dock->msginfo_defil - 1) / 6;
    do {
      cnt = cnt % (strlen(dock->msginfo) + 3);
      
      if (cnt < (int)strlen(dock->msginfo)) c = dock->msginfo[cnt]; else c = '.';

      XDrawString(dock->display, dock->coinpix, dock->NormalGC, cx, y+dock->fixed_font->ascent+1, &c, 1);

      /*       XCopyArea(dock->display, pixmap_letters, dock->coinpix, dock->and_GC, char2bitmap[c]+dec,0,MIN(cx+6, x+56)-MAX(cx,x),12,MAX(cx,x),y); */
      cx += DOCK_FIXED_FONT_W;
      cnt++;
      dec = 0;
    } while (cx < x+56);
  } else {
    cx = x + MAX((56 - (int)strlen(dock->msginfo)*6)/2, 0);
    assert(cx < 200);
    cnt = 0;
    do {
      if (cnt < (int)strlen(dock->msginfo)) c = dock->msginfo[cnt]; else c = ' ';

      XDrawString(dock->display, dock->coinpix, dock->NormalGC, cx, y+dock->fixed_font->ascent+1, &c, 1);
    
      /* XCopyArea(dock->display, pixmap_letters, dock->coinpix, dock->and_GC, char2bitmap[c],0,MIN(cx+6, x+56)-MAX(cx,x),12,MAX(cx,x),y); */
      cx += DOCK_FIXED_FONT_W;
      cnt++;
    } while (cx < x+56);
  }
  XSetClipMask(dock->display, dock->NormalGC, None);
}


/* mise à jour du titre défilant de l'applet selon l'arrivage de news */
void
dock_checkout_newstitles(Dock *dock, DLFP *dlfp)
{
  if (flag_updating_news == 0) {
    if (dlfp->updated) {
      DLFP_news *n;
      int pos, pos0;
      dlfp->updated = 0;

      if (dlfp_count_news(dlfp) == 0) {
	sprintf(dock->newstitles, "..NO NEWS");
	memset(dock->newstitles_id, 0, sizeof(dock->newstitles_id[0]) * MAX_NEWSTITLES_LEN);
	dock->newstitles_pos = 0; 
	dock->newstitles_char_dec = 4;
      } else {
	pos = 0; 
	dock->newstitles[0] = 0;      
	n = dlfp->news;
	while (n) {
	  int i;
	  static char *separ = " ... ";
	  pos0 = pos;
	  for (i=0; i < (int)strlen(n->titre); i++) {
	    if (pos >= MAX_NEWSTITLES_LEN-1) break;
	    dock->newstitles[pos] = n->titre[i];
	    pos++;
	  }
	  for (i=0; i < (int)strlen(separ); i++) {
	    if (pos >= MAX_NEWSTITLES_LEN-1) break;
	    dock->newstitles[pos] = separ[i];
	    pos++;
	  }
	  dock->newstitles[pos] = 0;
	  
	  for (i=pos0; i <= pos; i++) {
	    dock->newstitles_id[i] = n->id;
	  }
	  n = n->next;
	}
	dock->newstitles_pos = 0; 
	dock->newstitles_char_dec = 0;
      }
    }
  }
}

static void
led_create(Led *l, int x, int y)
{
  assert(l);
  l->xpos = x;
  l->ypos = y;
  l->coul[0] = OFF; l->coul[1] = OFF; l->coul[2] = OFF;
  l->delay[0] = 12; l->delay[1] = 1; l->delay[2] = 10;
  l->cnt = 0;
  l->colcnt = 0;
}

static void
led_update(Led *l)
{
  l->cnt++;
  if (l->cnt <= l->delay[0]) {
    l->colcnt = 0;
  } else if (l->cnt <= l->delay[0]+l->delay[1]) {
    l->colcnt = 1;
  } else if (l->cnt <= l->delay[0]+l->delay[1]+l->delay[2]) {
    l->colcnt = 2;
  } else if (l->cnt <= l->delay[0]+l->delay[1]*2+l->delay[2]) {
    l->colcnt = 1;
  } else {
    l->cnt = l->cnt % ( l->delay[0]+l->delay[1]+l->delay[2]);
    l->colcnt = 0;
  }
}


static void 
led_color(Led *l, LedColor c1, LedColor c2, LedColor c3)
{
  l->coul[0] = c1; l->coul[1] = c2; l->coul[2] = c3;
}

void
dock_leds_create(Dock *dock, Leds *leds)
{
  assert(leds);

  leds->pixmap = pixmap_create_from_raw(dock->rgba_context, leds_image.width, leds_image.height, leds_image.bytes_per_pixel, leds_image.pixel_data);
  leds->mask = alpha2pixmap_mask(dock->display, dock->coinpix, leds_image.width, leds_image.height, leds_image.pixel_data, 255);
  led_create(&leds->led[0], 2, 16);  leds->led[0].cnt = 5;  leds->led[0].delay[0] = 12;
  led_create(&leds->led[1], 13, 16); leds->led[1].cnt = 10; leds->led[1].delay[0] =  4; leds->led[1].delay[1] =  2; leds->led[1].delay[2] =  2;
  led_create(&leds->led[2], 24, 16); leds->led[2].cnt = 15; leds->led[2].delay[0] = 15;
  led_create(&leds->led[3], 35, 16); leds->led[3].cnt = 20; leds->led[3].delay[0] =  5;
  led_create(&leds->led[4], 50, 18); leds->led[4].cnt = 0;  leds->led[4].delay[0] = 20;
  leds->led[4].coul[0] = OFF   ;
  leds->led[4].coul[1] = BIGREDLIGHT;
  leds->led[4].coul[2] = BIGRED;
}

/* decremente les compteurs de clignotement */
void 
dock_leds_update(Leds *l)
{
  int i;
  for (i=0; i < 5; i++)
    led_update(&l->led[i]);
}

static void
dock_leds_draw(Dock *dock, Leds *l, int state_step)
{
  int i,x;
  assert(l);
  for (i=0; i < 4; i++) {
    if (l->led[i].coul[l->led[i].colcnt] != OFF)
      XCopyArea(dock->display, l->pixmap, dock->coinpix, dock->NormalGC, 
		(l->led[i].coul[l->led[i].colcnt]-1)*9, 0, 9, 4, 
		l->led[i].xpos, l->led[i].ypos - MIN(state_step,13));
  }

  i = 4;
  if (l->led[i].coul[l->led[i].colcnt] == OFF) x = 63;
  else if (l->led[i].coul[l->led[i].colcnt] == BIGREDLIGHT) x = 74;
  else x = 85;

  if (x == 85) {
    XCopyArea(dock->display, l->pixmap, dock->coinpix, dock->NormalGC, 
	      x, 2, 11, 3, l->led[i].xpos, l->led[i].ypos + MIN(state_step,38)+2);
    XCopyArea(dock->display, l->pixmap, dock->coinpix, dock->NormalGC, 
	      x+1, 1, 10, 1, l->led[i].xpos+1, l->led[i].ypos + MIN(state_step,38)+1);
    XCopyArea(dock->display, l->pixmap, dock->coinpix, dock->NormalGC, 
	      x+2, 0, 9, 1, l->led[i].xpos+2, l->led[i].ypos + MIN(state_step,38));
  } else {
    XCopyArea(dock->display, l->pixmap, dock->coinpix, dock->NormalGC, 
	      x+2, 1, 8, 1, l->led[i].xpos+2, 1+l->led[i].ypos + MIN(state_step,38));
    XCopyArea(dock->display, l->pixmap, dock->coinpix, dock->NormalGC, 
	      x+1, 2, 9, 2, l->led[i].xpos+1, 2+l->led[i].ypos + MIN(state_step,38));
  }
}


static void
refresh_msginfo(Dock *dock)
{
  if (dock->tl_item_survol) {
    if (dock->tl_item_clicked) {
      tribune_msg_info *mi;

      if (flag_updating_tribune == 0) {
	dock->tribune_updatable = 0;
      
	mi = tribune_find_id(&dock->dlfp->tribune, dock->tl_item_survol->id);
	if (mi) {
	  snprintf(dock->msginfo, MAX_MSGINFO_LEN, "    %s",
		   mi->useragent);
	  dock->msginfo[MAX_MSGINFO_LEN-1]=0;
	} else {
	  strcpy(dock->msginfo, "bug?");
	}
	dock->tribune_updatable = 1;
      } else strcpy(dock->msginfo, "plz wait");
    } else {
      if (flag_updating_tribune == 0) {
	dock->tribune_updatable = 0;
	strncpy(dock->msginfo, dock->tl_item_survol->tatouage->name, MAX_MSGINFO_LEN);
	dock->msginfo[MAX_MSGINFO_LEN-1]=0;
	if (strcmp(dock->msginfo, "?")==0 && flag_updating_tribune == 0) {
	  tribune_msg_info *mi;
	  mi = tribune_find_id(&dock->dlfp->tribune, dock->tl_item_survol->id);
	  if (mi) {
	    make_short_name_from_ua(mi->useragent, dock->msginfo, MAX_MSGINFO_LEN);
	  }
	}
	dock->tribune_updatable = 1;
      } else strcpy(dock->msginfo, "plz wait");
    }
  } else {
    if (dock->flag_survol_trollo) {
      sprintf(dock->msginfo, "level:%3d", (int)(dock->trib_trollo_rate*3));
    } else if (dock->flag_survol_led1) {
      sprintf(dock->msginfo, "vit.def:%d",dock->trolloscope_speed);
    } else if (flag_updating_tribune == 0) {
      dock->tribune_updatable = 0;
      sprintf(dock->msginfo, "%s+%02d%s", dock->tribune_time,
	      dock->dlfp->tribune.nbsec_since_last_msg,
	      dock->dlfp->tribune.nbsec_since_last_msg < 100 ? "s" : "");
      dock->tribune_updatable = 1;
    }  else {
      /* strcpy(dock->msginfo, "updating"); */
    }
  }
}

/* redessine l'applet (en mode normal, cad pas en mode horloge) */
void
dock_refresh_normal(Dock *dock)
{
  if ((Prefs.debug & 4) && ((wmcc_tic_cnt % 25) != 0 )) return;

  refresh_msginfo(dock);
  switch (dock->door_state) {
  case OPENED:
  case CLOSING:
  case OPENING:
    {
      /* l'image de fond */
      XCopyArea(dock->display, dock->coinpix, dock->coinpix, dock->NormalGC, 64, 0, 64, 64, 0, 0);
      /* le bouton */
      XCopyArea(dock->display, dock->coinpix, dock->coinpix, dock->or_GC, 
		128+(dock->red_button_press_state%3)*32, (dock->red_button_press_state/3)*31, 31, 30, 16, 16);


      XSetClipMask(dock->display, dock->NormalGC, dock->mask_porte_haut);
      XSetClipOrigin(dock->display, dock->NormalGC, 0, -MIN(dock->door_state_step,13));
      XCopyArea(dock->display, dock->pix_porte, dock->coinpix, dock->NormalGC, 
		0, MIN(dock->door_state_step,13), 64, 64, 0, 0);


      XSetClipMask(dock->display, dock->NormalGC, None);
      textout_msg(dock, dock->newstitles, 3, 3-dock->door_state_step, 57);



      XSetClipMask(dock->display, dock->NormalGC, dock->mask_porte_bas);
      XSetClipOrigin(dock->display, dock->NormalGC, 0, dock->door_state_step);
      XCopyArea(dock->display, dock->pix_porte, dock->coinpix, dock->NormalGC, 
		0, 0, 64, 64-dock->door_state_step, 0, dock->door_state_step);
      XSetClipMask(dock->display, dock->NormalGC, None);

      if (dock->door_state_step < TROLLOSCOPE_HEIGHT) {
	XCopyArea(dock->display, dock->pix_trolloscope, dock->coinpix, dock->NormalGC, 
		  0, 0, TROLLOSCOPE_WIDTH, TROLLOSCOPE_HEIGHT-dock->door_state_step, TROLLOSCOPE_X, TROLLOSCOPE_Y+dock->door_state_step);
      }

      XCopyArea(dock->display, dock->pix_porte, dock->coinpix, dock->NormalGC, 
		0, 47, 64, 64-47+1, 0, 47+MAX(0,dock->door_state_step - TROLLOSCOPE_HEIGHT + 5));

      textout_msginfo(dock, 4, 49+MAX(0,dock->door_state_step - TROLLOSCOPE_HEIGHT + 5));
      
      dock_leds_draw(dock, &dock->leds, dock->door_state_step);

    } break;
  case CLOSED:
    {
      XCopyArea(dock->display, dock->pix_porte, dock->coinpix, dock->NormalGC, 0, 0, 64, 64, 0, 0);
      textout_msg(dock, dock->newstitles, 3, 3, 57);

      textout_msginfo(dock, 4, 49+MAX(0,dock->door_state_step - TROLLOSCOPE_HEIGHT + 5));

      XCopyArea(dock->display, dock->pix_trolloscope, dock->coinpix, dock->NormalGC, 0, 0, 
		TROLLOSCOPE_WIDTH, TROLLOSCOPE_HEIGHT, TROLLOSCOPE_X, TROLLOSCOPE_Y);

      dock_leds_draw(dock, &dock->leds, dock->door_state_step);



    } break;
  default:
    {
      fprintf(stderr, "coin ?.... COIN ?? couic !\n"); assert(0);
    }
  }
}

/* redessine l'applet en mode horloge */
void
dock_refresh_horloge_mode(Dock *dock)
{
  struct tm *tm;
  time_t tnow;

  XCopyArea(dock->display, dock->clockpix, dock->coinpix, dock->NormalGC,
	    0,0, 64, 64, 0,0);

  tnow = time(NULL);
  if (dock->dlfp->tribune.last_post_id > 0) {
    time_t decal, ttribune;

    decal = difftime(tnow, dock->dlfp->tribune.local_time_last_check);
    ttribune = dock->dlfp->tribune.last_post_timestamp + decal + dock->dlfp->tribune.nbsec_since_last_msg;
    tm = localtime(&ttribune);
  } else {
    tm = localtime(&tnow);
  }
  
  XCopyArea(dock->display, dock->led, dock->coinpix, dock->NormalGC, 9 * (tm->tm_hour / 10), 0, 9, 11, 9, 6); 
  XCopyArea(dock->display, dock->led, dock->coinpix, dock->NormalGC, 9 * (tm->tm_hour % 10), 0, 9, 11, 18, 6); 
  if (tm->tm_sec % 2) {
    XCopyArea(dock->display, dock->led, dock->coinpix, dock->NormalGC, 90, 0, 5, 11, 27, 6); 
  }
  XCopyArea(dock->display, dock->led, dock->coinpix, dock->NormalGC, 9 * (tm->tm_min / 10), 0, 9, 11, 32, 6); 
  XCopyArea(dock->display, dock->led, dock->coinpix, dock->NormalGC, 9 * (tm->tm_min % 10), 0, 9, 11, 41, 6); 
  XCopyArea(dock->display, dock->month, dock->coinpix, dock->NormalGC, 0, 6 * tm->tm_mon, 22, 6, 18, 48);
  XCopyArea(dock->display, dock->weekday, dock->coinpix, dock->NormalGC, 0, 6 * ((tm->tm_wday +6) % 7), 20, 6, 21, 24);
  if (tm->tm_mday > 9) {
    XCopyArea(dock->display, dock->date, dock->coinpix, dock->NormalGC, 9 * (tm->tm_mday / 10), 0, 9, 13, 22, 33);
    XCopyArea(dock->display, dock->date, dock->coinpix, dock->NormalGC, 9 * (tm->tm_mday % 10), 0, 9, 13, 31, 33);
  } else {
    XCopyArea(dock->display, dock->date, dock->coinpix, dock->NormalGC, 9 * tm->tm_mday, 0, 9, 13, 26, 33);
  }
}

/* active/desactive le clignotement (et la couleur) des leds */
void
dock_leds_set_state(Dock *dock)
{
  int trollo_hrate;

  if (flag_troll_braining) {
    led_color(&dock->leds.led[0], VIOLET, VIOLET, VIOLET);
  } else if (flag_http_transfert) {
    led_color(&dock->leds.led[0], BLUE, BLUE, BLUE);
  } else if (flag_http_error) {
    led_color(&dock->leds.led[0], OFF, OFF, RED);
  } else if (flag_updating_news || flag_updating_tribune) {
    led_color(&dock->leds.led[0], CYAN, CYAN, CYAN);
  } else {
    led_color(&dock->leds.led[0], OFF, OFF, OFF);
  }

  if (flag_sending_coin_coin) {
    led_color(&dock->leds.led[1], YELLOW, YELLOW, YELLOW);
  } else if (dock->coin_coin_request > 0) {
    led_color(&dock->leds.led[1], OFF, YELLOW, YELLOW);    
  } else if (dock->coin_coin_request < 0) {
    led_color(&dock->leds.led[1], GREEN, GREEN, GREEN);
  } else {
    led_color(&dock->leds.led[1], OFF, OFF, OFF);
  }

  if (dlfp_unreaded_news_id(dock->dlfp, 1)>0) {
    led_color(&dock->leds.led[2], OFF, GREENLIGHT, GREEN);
  } else {
    led_color(&dock->leds.led[2], OFF, OFF, OFF);
  }


  if (flag_updating_messagerie) {
    led_color(&dock->leds.led[3], VIOLET, VIOLET, VIOLET);
  } else if (dlfp_msg_find_unreaded(dock->dlfp)) {
    led_color(&dock->leds.led[3], OFF, GREENLIGHT, GREEN);
  } else {
    led_color(&dock->leds.led[3], OFF, OFF, OFF);
  }

  /* rapidite du clignotement du trollometre */
  trollo_hrate = ((int)(dock->trib_trollo_rate*60));
  if (trollo_hrate < 10) {
    /* moins de 10 message / heure -> trollo OFF */
    if (dock->leds.led[4].delay[2] != 0) {
      BLAHBLAH(1, myprintf("%<YEL Troll-o-metre -> passage au niveau> %<GRN 0.00> (max=1.) !!!\n"));
      BLAHBLAH(1, myprintf("%<YEL> trollo_hrate = %d\n", trollo_hrate));
    }
    dock->leds.led[4].delay[1] = 0;
    dock->leds.led[4].delay[2] = 0;
  } else {
    /* moins de 3msg/5min */
    if (trollo_hrate < 36) {
      dock->leds.led[4].delay[0] = 65;
      dock->leds.led[4].delay[1] = 1;
      dock->leds.led[4].delay[2] = 1;
      /*1 msg/min */
    } else if (trollo_hrate < 60) {
      dock->leds.led[4].delay[0] = 30;
      dock->leds.led[4].delay[1] = 1;
      dock->leds.led[4].delay[2] = 1;
      /*2 msg/min */
    } else if (trollo_hrate < 120) {
      dock->leds.led[4].delay[0] = 16;
      dock->leds.led[4].delay[1] = 2;
      dock->leds.led[4].delay[2] = 2;
      /*3 msg/min */
    } else if (trollo_hrate < 180) {
      dock->leds.led[4].delay[0] = 8;
      dock->leds.led[4].delay[1] = 2;
      dock->leds.led[4].delay[2] = 2;
      /*+ de 3 msg/min */
    } else if (trollo_hrate < 60) {
      dock->leds.led[4].delay[0] = 2;
      dock->leds.led[4].delay[1] = 2;
      dock->leds.led[4].delay[2] = 2;
    }
  }
}


/* met le curseur en forme de croix que il survole le trolloscope */
static void
check_cursor_shape(Dock *dock, int x, int y)
{
  if (x >= TROLLOSCOPE_X && x < TROLLOSCOPE_X+TROLLOSCOPE_WIDTH &&
      y >= TROLLOSCOPE_Y && y < TROLLOSCOPE_Y+TROLLOSCOPE_HEIGHT) {
    if (dock->flag_trib_load_cursor == 0) {
      XDefineCursor(dock->display, DOCK_WIN(dock), dock->trib_load_cursor);
      XSync(dock->display, False);
      dock->flag_trib_load_cursor = 1;
    }
  } else if (dock->flag_trib_load_cursor == 1) {
    XUndefineCursor(dock->display, DOCK_WIN(dock));
    XSync(dock->display, False);
    dock->flag_trib_load_cursor = 0;
  }
}

static void
dock_handle_motion_notify(Dock *dock, int x, int y)
{
  int oldid;

  if (dock->tl_item_survol) {
    oldid = dock->tl_item_survol->id;
  } else oldid = -1;
  
  dock->tl_item_survol = NULL;
  check_cursor_shape(dock, x,y);
  if (dock->door_state == CLOSED) {
    /* 
       survol de la trolloscope ? 
    */
    if (x >= TROLLOSCOPE_X && x < TROLLOSCOPE_X+TROLLOSCOPE_WIDTH &&
	y >= TROLLOSCOPE_Y && y < TROLLOSCOPE_Y+TROLLOSCOPE_HEIGHT) {
      int i,j;
      
      
      j = (TROLLOSCOPE_WIDTH + TROLLOSCOPE_X - 1 - x) / dock->trolloscope_resolution;
      i = (TROLLOSCOPE_HEIGHT + TROLLOSCOPE_Y - 1 - y) / dock->trolloscope_resolution;
      
      if (dock->trolloscope[i][j].id > 0) {
	dock->tl_item_survol = &dock->trolloscope[i][j];
	if (dock->trolloscope[i][j].id != oldid) {
	  dock->tl_item_clicked = 0;
	  dock->msginfo_defil = 0;
	}
	dock->view_id_in_newstitles = dock->tl_item_survol->id;
	dock->view_id_timer_cnt = 0;
	/* myprintf("i = %d, j=%d, id = %d, nom = %s\n", i,j, dock->trolloscope[i][j].id,dock->trolloscope[i][j].tatouage->name); */
      } else {
	dock->tl_item_clicked = 0;
	dock->msginfo_defil = 0;
	dock->view_id_in_newstitles = 0;
      }
    } else {
      dock->tl_item_clicked = 0;
      dock->msginfo_defil = 0;
      dock->view_id_in_newstitles = 0;
    }
    if (IS_INSIDE(x,y,50,18,60,22)) {
      /* survol du trollometre */
      dock->flag_survol_trollo = 1;
    } else {
      dock->flag_survol_trollo = 0;
    }
    if (IS_INSIDE(x,y,dock->leds.led[1].xpos,dock->leds.led[1].ypos - MIN(dock->door_state_step,13),
		  dock->leds.led[1].xpos+8, dock->leds.led[1].ypos +3 - MIN(dock->door_state_step,13))) {
      /* survol de la led 1 */
      dock->flag_survol_led1 = 1;
    } else {
      dock->flag_survol_led1 = 0;
    }
  }
}

/* renvoie 1 si le bouton rouge a ete suffisament enfonce */
int
dock_red_button_check(Dock *dock) {
  if (dock->red_button_press_flag) {
    /* si on a appuye assez fort ... */
    if (dock->red_button_press_state == 5) {
      BLAHBLAH(1,printf("Coin !\n"));

      if (flag_sending_coin_coin == 0) { /* petite precaution */
	/* On utilise real_coin_coin_message pour éviter un bug si on modifier
	   le message entre l'appuie du bouton rouge et exec_coin_coin */
	strncpy(dock->real_coin_coin_message, dock->coin_coin_message, 
		MESSAGE_MAX_LEN);
	strncpy(dock->real_coin_coin_useragent, dock->coin_coin_useragent, 
		USERAGENT_MAX_LEN);
      
	dock->coin_coin_request = 1;
      }
    }
    
    dock->red_button_press_flag = -1;
  }
  return dock->coin_coin_request;
}

/* statistique à la noix */
static void
dock_show_tribune_frequentation(Dock *dock)
{
  char s[2048], s_xp[512], sv_xp[10], sv_xp_old[50];
  int ua_cnt1, ua_cnt2, ua_cnt3, ua_cnt4;
  int msg_cnt1, msg_cnt2, msg_cnt3, msg_cnt4;
  int my_msg_cnt1, my_msg_cnt2, my_msg_cnt3, my_msg_cnt4;


  tribune_frequentation(&dock->dlfp->tribune, 10, &ua_cnt1, &msg_cnt1, &my_msg_cnt1);
  tribune_frequentation(&dock->dlfp->tribune, 30, &ua_cnt2, &msg_cnt2, &my_msg_cnt2);
  tribune_frequentation(&dock->dlfp->tribune, 120, &ua_cnt3, &msg_cnt3, &my_msg_cnt3);
  tribune_frequentation(&dock->dlfp->tribune, 8*60, &ua_cnt4, &msg_cnt4, &my_msg_cnt4);

  if (Prefs.user_cookie) {
    if (dock->dlfp->xp > -1000) {
      snprintf(sv_xp,10,"%d", dock->dlfp->xp);
    } else snprintf(sv_xp, 10, "???");
    if (dock->dlfp->xp_old > -1000) {
      snprintf(sv_xp_old,50,"(ancienne valeur: %d)",dock->dlfp->xp_old);
    } else sv_xp_old[0] = 0;

    snprintf(s_xp, 512, "Vous avez <b><font color=blue>%s</font></b>"
	     " XP %s  [votes:%d/%d]<br><br>", 
	     sv_xp, sv_xp_old, dock->dlfp->votes_cur, dock->dlfp->votes_max);
  } else {
    s_xp[0] = 0;
  }
  
  snprintf(s, 2048, 
	   "vous utilisez wmc² v.<font color=blue>" VERSION "</font> [ compilé le " __DATE__ " ]<p>"
	   "%s"
	   "<p align=center><b>Fréquentation de la Tribune</b><br>"
	   "<i>(estimation basée sur les useragents et les logins)</i></p><br>"
	   "depuis:<br>"
	   ".<tab><i>10 minutes</i>: <tab><font color=blue>%d</font><tab> personnes ont posté <tab><font color=blue>%d</font> messages (vous:%d)<br>"
	   ".<tab><i>30 minutes</i>: <tab><font color=blue>%d</font><tab> personnes ont posté <tab><font color=blue>%d</font> messages (vous:%d)<br>"
	   ".<tab><i> 2 heures </i>: <tab><tab><font color=blue>%d</font><tab> personnes ont posté <tab><font color=blue>%d</font> messages (vous:%d)<br>"
	   ".<tab><i> 8 heures </i>: <tab><tab><font color=blue>%d</font><tab> personnes ont posté <tab><font color=blue>%d</font> messages (vous:%d)<br>",
	   s_xp, ua_cnt1, msg_cnt1, my_msg_cnt1, 
	   ua_cnt2, msg_cnt2, my_msg_cnt2, 
	   ua_cnt3, msg_cnt3, my_msg_cnt3,
	   ua_cnt4, msg_cnt4, my_msg_cnt4);

  msgbox_show(dock, s);
}



/* (c)(tm)(r) kadreg qui n'aime pas le jaune (pourtant moi j'aime bien le jaune) */
char *
dock_build_pixmap_porte(Dock *dock)
{
  Pixmap bg_pixmap;
  /* debut modif kadreg */
  /* chargement de pix_porte suivant si pixmap de fond */

  dock->pix_porte = None;
  if (Prefs.dock_bgpixmap) {
    int w, h;
    bg_pixmap = RGBACreatePixmapFromXpmFile(dock->rgba_context, Prefs.dock_bgpixmap, &w, &h);
    if ((bg_pixmap == None) || (w != 64) || (h != 64)) {
      return str_printf("Erreur en chargeant le fichier : '%s' [xpm de 64x64 pixels svp]", Prefs.dock_bgpixmap);
    } else {
      RGBAImage *rgba_porte;
      XImage *XiPixPixmap;
      int i, j;

      rgba_porte = rimage_create_from_raw(porte_image.width, porte_image.height,
					  porte_image.bytes_per_pixel,porte_image.pixel_data);

      XiPixPixmap= XGetImage (dock->display, 
			      bg_pixmap, 
			      0, 0, 64, 64, AllPlanes, ZPixmap);
      XFreePixmap(dock->display, bg_pixmap);

      
      for (i=0; i< (int)rgba_porte->w; i++) {
	for (j=0; j< (int)rgba_porte->h; j++) {
	  unsigned char r, g, b;
	  unsigned long pixel;
	  int shade;

	  pixel = XGetPixel (XiPixPixmap, i, j);
	  
	  /* on ramene les composantes de 16bits à 8bits */
	  r = PIXEL2R(dock->rgba_context, pixel);
	  g = PIXEL2G(dock->rgba_context, pixel);
	  b = PIXEL2B(dock->rgba_context, pixel);

	  /* selon le niveau de gris de rgba_porte */
	  shade = rgba_porte->data[j][i].rgba[0] - 0x7F; /* shade = 0 pour les couleurs non modifiees, ~92 pour les 
							   bords clairs et  ~ -60 pour les bords sombres, voir xpms/porte.xcf */
	  if (shade > 0) {
	    r = MIN(r*1.3, 255.0);
	    g = MIN(g*1.3, 255.0);
	    b = MIN(b*1.3, 255.0);
	  } else if (shade == -127) {
	    r = g = b = 0;
	  } else if (shade < 0) {
	    r = MAX(r/1.3, 0.0); /* le MAX ne sert a rien, je sais */
	    g = MAX(g/1.3, 0.0);
	    b = MAX(b/1.3, 0.0);
	  }
	  rgba_porte->data[j][i].rgba[0] = r;
	  rgba_porte->data[j][i].rgba[1] = g;
	  rgba_porte->data[j][i].rgba[2] = b;
	}
      }

      dock->pix_porte = RGBAImage2Pixmap(dock->rgba_context, rgba_porte);
      RGBADestroyImage(rgba_porte);
      XDestroyImage (XiPixPixmap);
    }
  }
  /* pouille dans le cottage ? */
  if (dock->pix_porte == None) {
      /* et hop, on sauve les meubles */
    dock->pix_porte = pixmap_create_from_raw_with_tint(dock->rgba_context, 
						       porte_image.width, 
						       porte_image.height, 
						       porte_image.bytes_per_pixel, 
						       porte_image.pixel_data, 
						       Prefs.dock_bgcolor);
    
  }
  /* bouh comme c vilain */
  if (dock->mask_porte_haut == None) {
    dock->mask_porte_haut = alpha2pixmap_mask(dock->display, dock->coinpix, 64, 64, porte_image.pixel_data, 255);
    dock->mask_porte_bas = alpha2pixmap_mask(dock->display, dock->coinpix, 64, 64, porte_image.pixel_data, 230);
  }
  /* fin modif kadreg */

  /* les trois couleurs du dock */
  {
    int r,g,b;
    r = (Prefs.dock_bgcolor>>16)&0xff; g = (Prefs.dock_bgcolor>>8)&0xff; b = Prefs.dock_bgcolor &0xff;

    /* ces valeurs ne sont pas utilisees pour le dock lui-meme, qui passe par l'intermediaire
       de l'horrible rcreate_image_from_raw_with_tint et compagnie
       mais pour l'editwin qui est nettement mieux ecrite ainsi que pour msgbox... */
    dock->bg_pixel   = RGB2PIXEL(r,g,b);
    dock->light_pixel = RGB2PIXEL(MIN(255, (r*170)/128), MIN(255,(g*170)/128), MIN(255,(b*170)/128));
    dock->dark_pixel = RGB2PIXEL(MIN(255, (r*97)/128), MIN(255,(g*97)/128), MIN(255,(b*97)/128));
  }
  
  return NULL;
}


void
dock_set_horloge_mode(Dock *dock) {
  if (Prefs.draw_border == 0) {
    XShapeCombineMask(dock->display, DOCK_WIN(dock), ShapeBounding, 
		      0, 0, dock->clock_pixmask, ShapeSet);
  }
  dock->horloge_mode = 1;
}

static void
dock_unset_horloge_mode(Dock *dock) {
  if (Prefs.draw_border == 0) {
    XShapeCombineMask(dock->display, DOCK_WIN(dock), ShapeBounding, 
	  	      0, 0, dock->coin_pixmask, ShapeSet);
  }
  dock->horloge_mode = 0;  
}

/*
  bouse
*/
static void
dock_handle_button_press(Dock *dock, XButtonEvent *xbevent)
{
  int x,y;
  x = xbevent->x; y = xbevent->y;


  /* gestion d'un bug curieux de KDE/swallow:
	 un LeaveNotify est genere quand on clique sur le dock...
	 du coup ca reinitialise plein de choses.. */
  dock_handle_motion_notify(dock, x, y);

  /* Ctrl-Clic1 -> passage en mode horloge */
  if (xbevent->button == Button1 && (xbevent->state & ControlMask)) {
    dock_set_horloge_mode(dock);
  } else if (xbevent->button == Button1) {
    if (IS_INSIDE(x,y,20,20,40,40) && 
	(dock->door_state == OPENED || (dock->door_state_step>12))) {
      dock->red_button_press_flag = 1;
    } else if (IS_INSIDE(x,y,50,18,60,22) && 
	       (dock->door_state == CLOSED)) {
      /* clic sur le trollometre */
      dock->door_state = OPENING;

    } else if (IS_INSIDE(x,y,2,2,59,13) && 
	       (dock->door_state == CLOSED)) {
      int id;
      /* clic gauche sur la zone des news défilantes -> on affiche la news et on 'raise' la fenetre */
      BLAHBLAH(4,printf("newswin_show\n"));
      if (strlen(dock->newstitles)) {
	int pos;
	pos = (dock->newstitles_pos + ((xbevent->x - 7)+3)/6) % strlen(dock->newstitles);
	id = dock->newstitles_id[pos];
      } else { id = -1; }
      newswin_show(dock, dock->dlfp, id); XRaiseWindow(dock->display, newswin_get_window(dock));
    } else if (IS_INSIDE(x,y,dock->leds.led[0].xpos,dock->leds.led[0].ypos - MIN(dock->door_state_step,13),
			 dock->leds.led[0].xpos+8, dock->leds.led[0].ypos +3 - MIN(dock->door_state_step,13))) {
      /*
	clic bouton 1 sur la 1ere led ->
	voir la derniere erreur http
      */
      char *err_msg;
      err_msg = http_complete_error_info();
      msgbox_show(dock, err_msg);
      free(err_msg);
    }else if (IS_INSIDE(x,y,dock->leds.led[1].xpos,dock->leds.led[1].ypos - MIN(dock->door_state_step,13),
			dock->leds.led[1].xpos+8, dock->leds.led[1].ypos +3 - MIN(dock->door_state_step,13))) {
      /*
	clic bouton 1 sur la 2eme led ->
	accelere le trolloscope
      */
      dock->trolloscope_speed /= 2;
      if (dock->trolloscope_speed <= 0) dock->trolloscope_speed = 1;
      dock_update_pix_trolloscope(dock, &dock->dlfp->tribune);
    } else if (IS_INSIDE(x,y,dock->leds.led[2].xpos,dock->leds.led[2].ypos - MIN(dock->door_state_step,13),
			 dock->leds.led[2].xpos+8, dock->leds.led[2].ypos +3 - MIN(dock->door_state_step,13))) {
      /*
	click bouton 1 sur la 3eme led -> 
	voir les news non lues
      */
      int id;
      id = dlfp_unreaded_news_id(dock->dlfp, 1);
      if (id > 0) {
	newswin_show(dock, dock->dlfp, id);
      }
    } else if (IS_INSIDE(x,y,dock->leds.led[3].xpos,dock->leds.led[3].ypos - MIN(dock->door_state_step,13),
			 dock->leds.led[3].xpos+8, dock->leds.led[3].ypos +3 - MIN(dock->door_state_step,13))
	       && flag_updating_messagerie == 0) {
      /* clic gauche sur la 4eme led -> voir les nouveaux messages (avec le browser 1)*/
      DLFP_message *m;
      m = dlfp_msg_find_unreaded(dock->dlfp);
      if (m) {
	char url[2048];
	snprintf(url, 2048, "http://%s:%d/%s%smessages/view.php3?id=%d", 
		 Prefs.site_root, Prefs.site_port, Prefs.site_path, strlen(Prefs.site_path) ? "/" : "", 
		 m->mid);
	open_url(url, x, y, 1);
	m->unreaded = 0;
      } else {
	msgbox_show(dock, "pas de nouveaux messages");
      }
    } else if (IS_INSIDE(x,y,TROLLOSCOPE_X, TROLLOSCOPE_Y,
			 TROLLOSCOPE_X+TROLLOSCOPE_WIDTH-1,TROLLOSCOPE_Y+TROLLOSCOPE_HEIGHT-1) &&
	       dock->door_state == CLOSED) {
      /* 
	 click bouton 1 sur un element de trolloscope valide 
	 on va faire defiler son useragent en bas, et faire
	 clignoter en alternance son id et l'heure dans la zone
	 de texte du haut

	 SI trolloscope clignote (detection de nouveau commentaire), on l'efface
      */


      /* pour le debuggage, ça peut tj servir .. */
      BLAHBLAH(1, dlfp_yc_printf_comments(dock->dlfp));
      BLAHBLAH(1, dlfp_msg_printf_messages(dock->dlfp));


      if (flag_updating_comments == 0) {
	DLFP_comment *com;
	if ((com = dlfp_yc_find_modified(dock->dlfp,NULL))) {
	  char url[1024];
	  snprintf(url, 1024, "http://%s:%d/%s%scomments/thread.php3?news_id=%d&com_id=%d&res_type=1",
		   Prefs.site_root, Prefs.site_port, Prefs.site_path, strlen(Prefs.site_path) ? "/" : "", 
		   com->news_id, com->com_id);
	  open_url(url, -1, -1, 1);
	  com->modified = 0;
	  dlfp_yc_clear_modified(dock->dlfp);
	  if (dlfp_yc_find_modified(dock->dlfp,NULL) == NULL) {
	    dock->flamometre.comment_change_decnt = 1;
	  }
	} else if (dock->flamometre.comment_change_decnt) { /* bizarre mais bon .. */
	  dock->flamometre.comment_change_decnt = 1; /* ta gueule , en quelque sorte */
	} else if (dock->flamometre.xp_change_decnt) {
	  dock->flamometre.xp_change_decnt = 1;
	} else if (dock->flamometre.tribune_answer_decnt) {
	  dock->flamometre.tribune_answer_decnt = 1;
	}
      }

      if (dock->tl_item_survol) {
	dock->tl_item_clicked = 1;
	dock->msginfo_defil = 1;
      }
    } else if (IS_INSIDE(x,y,3,49,3+57,49+12) && 
	       (dock->door_state_step <= TROLLOSCOPE_HEIGHT)) {
      if (!editw_ismapped(dock->editw)) {
	editw_show(dock, dock->editw, 0);
      } else {
	editw_hide(dock, dock->editw);
      }
    }

    /********************************************* BOUTON DROIT ****************************/
  } else if (xbevent->button == Button3) {
    if (IS_INSIDE(x,y,2,2,59,13) && 
	(dock->door_state == CLOSED)) {
      /* clic bouton droite sur la zone défilante -> fermeture la fenetre des news */
      if (newswin_is_used(dock)) {
	newswin_unmap(dock);
      } else newswin_show(dock, dock->dlfp, -2);
    } else if (IS_INSIDE(x,y,TROLLOSCOPE_X, TROLLOSCOPE_Y,
			 TROLLOSCOPE_X+TROLLOSCOPE_WIDTH-1,TROLLOSCOPE_Y+TROLLOSCOPE_HEIGHT-1) &&
	       dock->door_state == CLOSED) {
      /* 
	 SI trolloscope clignote (detection de nouveau commentaire), on l'efface
      */
	  
      if (flag_updating_comments == 0) {
	if (dlfp_yc_find_modified(dock->dlfp,NULL)) {
	  dlfp_yc_clear_modified(dock->dlfp);
	  dock->flamometre.comment_change_decnt = 1;
	}
      }
    } else if (IS_INSIDE(x,y,dock->leds.led[1].xpos,dock->leds.led[1].ypos - MIN(dock->door_state_step,13),
			 dock->leds.led[1].xpos+8, dock->leds.led[1].ypos +3 - MIN(dock->door_state_step,13))) {
      /*
	    click bouton droite sur la 2eme led -> 
	    ralentir le trolloscope
	  */
      dock->trolloscope_speed *= 2;
      if (dock->trolloscope_speed <= 0 || dock->trolloscope_speed > 128) dock->trolloscope_speed = 128;
      dock_update_pix_trolloscope(dock, &dock->dlfp->tribune);
	  
    }  else if (IS_INSIDE(x,y,dock->leds.led[2].xpos,dock->leds.led[2].ypos - MIN(dock->door_state_step,13),
			  dock->leds.led[2].xpos+8, dock->leds.led[2].ypos +3 - MIN(dock->door_state_step,13))) {
      /*
	    click bouton droite sur la 3eme led -> 
	     eteindre la diode
	  */
      if (flag_updating_news == 0) {
	dlfp_unset_unreaded_news(dock->dlfp);
	if (newswin_is_used(dock)) { newswin_update_content(dock, dock->dlfp, 0); newswin_draw(dock); }
      }
    } else if (IS_INSIDE(x,y,dock->leds.led[3].xpos,dock->leds.led[3].ypos - MIN(dock->door_state_step,13),
			 dock->leds.led[3].xpos+8, dock->leds.led[3].ypos +3 - MIN(dock->door_state_step,13))
	       && flag_updating_messagerie == 0) {
      /* clic droite sur la 4eme led -> annuler les nouveaux messages */
      DLFP_message *m;
      while ((m = dlfp_msg_find_unreaded(dock->dlfp))) m->unreaded = 0;
    } else  if (IS_INSIDE(x,y,50,18,60,22) && 
		(dock->door_state == CLOSED)) {
      /* bouton 3 sur le trollometre:
	     on montre la frequentation de la tribune */
      dock_show_tribune_frequentation(dock);
    } else if (IS_INSIDE(x,y,3,49,3+57,49+12) && 
	       (dock->door_state_step <= TROLLOSCOPE_HEIGHT)) {
      /* montre le pinnipede teletype */
      if (!pp_ismapped(dock)) {
	if (flag_updating_tribune == 0) {
	  pp_show(dock, &dock->dlfp->tribune);
	}
      } else {
	pp_unmap(dock);
      }
    }

    /************************** BOUTON MILIEU ****************************************************/
  } else if (xbevent->button == Button2) {
    /*
	  demande de rafraichissement de la tribune ?
	  (bouton milieu dans la trolloscope)
	*/
    if (IS_INSIDE(x,y,2,2,59,13) && 
	(dock->door_state == CLOSED)) {
      dock->news_update_request = 1;
    } else if (IS_INSIDE(x,y,TROLLOSCOPE_X, TROLLOSCOPE_Y,
			 TROLLOSCOPE_X+TROLLOSCOPE_WIDTH-1,TROLLOSCOPE_Y+TROLLOSCOPE_HEIGHT-1) &&
	       dock->door_state == CLOSED) {
      /* rafraichissement des news */
      dock->tribune_update_request = 1;
    } else if (IS_INSIDE(x,y,dock->leds.led[1].xpos,dock->leds.led[1].ypos - MIN(dock->door_state_step,13),
			 dock->leds.led[1].xpos+8, dock->leds.led[1].ypos +3 - MIN(dock->door_state_step,13))) {
      /* clic milieu sur la deuxieme led -> changement de la resolution du trolloscope */
      switch (dock->trolloscope_resolution) {
      case 5: dock->trolloscope_resolution = 2; break;
      case 2: dock->trolloscope_resolution = 1; break;
      case 1: dock->trolloscope_resolution = 5; break;
      default: assert(0); break;	  
      }	  
      dock->tl_item_clicked = 0;
      dock->msginfo_defil = 0;
      dock_update_pix_trolloscope(dock, &dock->dlfp->tribune);
    } else if (IS_INSIDE(x,y,dock->leds.led[3].xpos,dock->leds.led[3].ypos - MIN(dock->door_state_step,13),
			 dock->leds.led[3].xpos+8, dock->leds.led[3].ypos +3 - MIN(dock->door_state_step,13))
	       && flag_updating_messagerie == 0) {
      /* clic milieu sur la 4eme led -> voir les nouveaux messages (avec le browser 2)*/
      DLFP_message *m;
      m = dlfp_msg_find_unreaded(dock->dlfp);
      if (m) {
	char url[2048];
	snprintf(url, 2048, "http://%s:%d/%s%smessages/view.php3?id=%d", 
		 Prefs.site_root, Prefs.site_port, Prefs.site_path, strlen(Prefs.site_path) ? "/" : "", 
		 m->mid);
	open_url(url, x, y, 2);
	m->unreaded = 0;
      } else {
	msgbox_show(dock, "pas de nouveaux messages");
      }
    } else if (IS_INSIDE(x,y,50,18,60,22) && 
	       (dock->door_state == CLOSED)) {
      if ((xbevent->state & ShiftMask) == 0) {
	/* 
	   bouton du milieu sur le trollometre:
	   relire le fichier d'options
	 */
	wmcc_prefs_relecture(dock);	
      } else {
	/* shift+bouton milieu sur le trollometre:
	   relire le .wmcoincoin/useragents
	*/
	int err;
	err = useragents_file_reread(dock, dock->dlfp);
	BLAHBLAH(2, printf("relecture RCfile -> code d'erreur renvoye: %d\n", err));
      }
    }
  }
}

static void
dock_handle_button_press_horloge_mode(Dock *dock, XButtonEvent *xbevent)
{
  int x,y;
  x = xbevent->x; y = xbevent->y;
  
  if (xbevent->button == Button1 && (xbevent->state & ControlMask)) {
    dock_unset_horloge_mode(dock);
  }
}

void
dock_dispatch_event(Dock *dock, XEvent *event)
{
  switch (event->type) {
  case DestroyNotify: /* OH MON DIEU ! ils ont tué coincoin ! */
    {
      XCloseDisplay(dock->display);
    
      exit(0);
    } break;
  case ButtonPress:
    {
      if (dock->horloge_mode == 0) {
	dock_handle_button_press(dock, &event->xbutton);
      } else {
	dock_handle_button_press_horloge_mode(dock, &event->xbutton);
      }
    } break;
  case ButtonRelease:
    {
      if (dock->horloge_mode == 0) dock_red_button_check(dock);
    } break;
  case MotionNotify:
    {
      if (dock->horloge_mode == 0) dock_handle_motion_notify(dock, event->xmotion.x, event->xmotion.y);
    } break;
  case EnterNotify:
    {
      int x,y;
      x = event->xcrossing.x;
      y = event->xcrossing.y;
      if (dock->horloge_mode == 0) check_cursor_shape(dock, x, y);
    } break;
  case LeaveNotify:
    {
      check_cursor_shape(dock, -1,-1);
      dock->tl_item_survol = NULL;
      dock->tl_item_clicked = 0;
      dock->view_id_in_newstitles = 0;
      dock->flag_survol_trollo = 0;
      dock->flag_survol_led1 = 0;

      if (dock->door_state == OPENED || dock->door_state == OPENING) dock->door_state = CLOSING;
    } break;
  }
}
