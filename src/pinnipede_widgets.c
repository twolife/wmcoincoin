#include "pinnipede.h"


/* --------------------------- gestion des tabs ------------------- */
/* a appeler apres update fortune et modif minib*/
static void
pp_tabs_set_pos(Pinnipede *pp)
{
  int cnt;
  
  int tab_max_w,tab_min_w, tab_w, tabs_min_w=-1, tabs_max_w=-1, tabs_w=-1;
  int x,y;
  
  pp->zmsg_y1 = pp->fortune_h == 0 ? 3 : pp->fortune_h;
  pp->zmsg_y2 = pp->win_height - 1 - (pp->use_minibar ? MINIB_H : 0);
  pp->zmsg_h = pp->zmsg_y2-pp->zmsg_y1+1;

  if (pp->nb_tabs == 0 || pp->use_minibar == 0) return;

  tab_min_w = 40;
  tab_max_w = 100;
  tabs_min_w = pp->nb_tabs * tab_min_w;
  tabs_max_w = pp->nb_tabs * tab_max_w;
  tab_w = MIN(MAX(pp->win_width / pp->nb_tabs, tab_min_w), tab_max_w);
  tabs_w = pp->nb_tabs * tab_w;

  pp->mb_x0 = 0;
  
  x = 0;
  if (pp->tabs_pos == PPT_DOWN) {
    y = pp->win_height - PPT_H;
    
    if (pp->use_minibar && tabs_min_w + pp->mb_min_width > pp->win_width) {
      y -= MINIB_H;
    } else {
      /* "reduction" de la largeur pour loger dans la minibar */
      tab_w = MIN((pp->win_width - pp->mb_min_width) / pp->nb_tabs, tab_max_w);
      tabs_w = pp->nb_tabs * tab_w;
      pp->mb_x0 = tabs_w;
    }
    pp->zmsg_y2 = y-1;
  } else {
    y = 0;
    pp->zmsg_y1 += PPT_H;
  }

  for (cnt = 0; cnt < pp->nb_tabs; cnt++) {
    PinnipedeTab *pt = &pp->tabs[cnt];
    pt->x = x;
    pt->y = y;
    pt->w = tab_w;
    pt->h = PPT_H;
    x += pt->w;
  }
  pp->tabs_width = MIN(x, pp->win_width);

  pp->zmsg_h = pp->zmsg_y2-pp->zmsg_y1+1;
}

void
pp_tabs_set_visible_sites(Pinnipede *pp) {
  int i;
  for (i=0; i < MAX_SITES; i++) pp->filter.visible_sites[i] = 0;
  for (i=0; i < pp->nb_tabs; i++) {
    pp->filter.visible_sites[pp->tabs[i].site->site_id] = pp->tabs[i].selected; 
  }
}

/* a appeler apres update fortune */
void
pp_tabs_build(Dock *dock) {
  Pinnipede *pp = dock->pinnipede;
  Site *s;
  PinnipedeTab *pt;

  pp->tabs_pos = PPT_DOWN;
  pp->nb_tabs = 0; 
  for (s = dock->sites->list; s; s = s->next) {
    if (s->board) {
      pp->nb_tabs++;
    }
  }

  printf("nb_tabs = %d\n",pp->nb_tabs);

  ALLOC_VEC(pp->tabs, pp->nb_tabs, PinnipedeTab);
  for (s = dock->sites->list, pt = pp->tabs; s; s = s->next) {
    if (s->board) {
      pt->site = s;
      pt->selected = 1;
      pt->clicked = 0;
      pt++;
    }
  }
  pp->active_tab = (pp->nb_tabs) ? &pp->tabs[0] : NULL;

  pp_tabs_set_pos(pp);
  pp_tabs_set_visible_sites(pp);
}

void
pp_tabs_destroy(Pinnipede *pp)
{
  free(pp->tabs); pp->tabs = NULL; pp->nb_tabs = 0;
  pp->active_tab = NULL;
}

void
pp_tabs_refresh(Dock *dock)
{
  Pinnipede *pp = dock->pinnipede;

  if (pp->nb_tabs == 0) return;
  XSetFont(dock->display, dock->NormalGC, pp->fn_minib->fid);

  /* dessin general */
  //  XSetForeground(dock->display, dock->NormalGC, pp->minib_pixel);

  /* dessin des lignes */
  {
    int y1, y2,i;
    y1 = 0; y2 = pp->tabs[0].h - 1;
    if (pp->tabs_pos == PPT_DOWN) {
      int tmp = y1; y1 = y2; y2 = tmp; 
    }


    
    XSetForeground(dock->display, dock->NormalGC, pp->minib_pixel);
    XFillRectangle(dock->display, pp->lpix, dock->NormalGC, 
		   0, 0, pp->win_width, pp->fn_h);
    

    XSetForeground(dock->display, dock->NormalGC, pp->minib_dark_pixel);
    XDrawLine(dock->display, pp->lpix, dock->NormalGC, 0, 0, pp->tabs_width, 0);
    for (i=0; i < pp->nb_tabs; i++) {
      PinnipedeTab *pt = pp->tabs+i;
      int x = pt->x, w = pt->w, h=pt->h;
      unsigned long bar_pixel = pp->progress_bar_pixel;
      Board *board = pt->site->board;

      XSetForeground(dock->display, dock->NormalGC, pp->minib_dark_pixel);
      XDrawLine(dock->display, pp->lpix, dock->NormalGC, pt->x+pt->w-1, 
		0, pt->x+pt->w-1, PPT_H-1);
      
      if (pt->site->board->enabled) {
	XSetForeground(dock->display, dock->NormalGC, pp->minib_dark_pixel);
	XDrawLine(dock->display, pp->lpix, dock->NormalGC, x+w-6, h-6, x+w-2, h-1);
	XDrawLine(dock->display, pp->lpix, dock->NormalGC, x+w-6, h-1, x+w-2, h-6);
      }

      XSetForeground(dock->display, dock->NormalGC, bar_pixel);
      if (board->board_refresh_delay > 0) {
	int zw = ((w-7)*MIN(board->board_refresh_cnt,board->board_refresh_delay))/board->board_refresh_delay;
	zw = (w-7 - zw);
	//	printf("zw=%d %d %d %d\n",zw,board->board_refresh_cnt,board->board_refresh_delay, pt->w);
	if (zw > 0) {
	  XFillRectangle(dock->display, pp->lpix, dock->NormalGC, x+1, h-4, zw, 3);
	}
      }

      {
	const char *t = pp->tabs[i].site->prefs->site_name;
	int tw = strlen(t) * MINIB_FN_W;
	int tx = pt->x + (pt->w - tw)/2;
	int ty = pp->fn_minib->ascent+1 + pp->tabs[i].clicked*2;
	if (!pt->selected) {
	  XSetForeground(dock->display, dock->NormalGC, IRGB2PIXEL(0x808080));
	} else {
	  if (pt == pp->active_tab) {
	    XSetForeground(dock->display, dock->NormalGC, IRGB2PIXEL(0x80ff80));
	    XDrawString(dock->display, pp->lpix, dock->NormalGC, tx+1, ty+1, t, strlen(t));
	  }
	  XSetForeground(dock->display, dock->NormalGC, IRGB2PIXEL(0x006000));
	}
	XDrawString(dock->display, pp->lpix, dock->NormalGC, tx, ty, t, strlen(t));
      }
    }
  }
  /* et hop */
  XCopyArea(dock->display, pp->lpix, pp->win, dock->NormalGC,
	    0,0,pp->tabs_width, PPT_H, pp->tabs[0].x,pp->tabs[0].y);
  //  printf("tabs_w = %d, mb_w = %d\n", pp->tabs_width, pp->mb_buttonbar_width);
}

static PinnipedeTab *
pp_tabs_at_xy(Dock *dock, int x, int y) {
  int i;
  Pinnipede *pp = dock->pinnipede;
  for (i=0; i < pp->nb_tabs; i++) 
    if (x >= pp->tabs[i].x && y >= pp->tabs[i].y && 
	x <= pp->tabs[i].x + pp->tabs[i].w-1 && 
	y <= pp->tabs[i].y + pp->tabs[i].h-1)
      return pp->tabs+i;
  return NULL;
}
static PinnipedeTab *
pp_tabs_pressed_tab(Dock *dock)
{
  Pinnipede *pp = dock->pinnipede;
  int i;

  for (i=0; i < pp->nb_tabs; i++) {
    if (pp->tabs[i].clicked)
      return &pp->tabs[i];
  }
  return NULL;
}

static int
pp_tabs_handle_button_press(Dock *dock, XButtonEvent *ev) {
  //  Pinnipede *pp = dock->pinnipede;
  PinnipedeTab *pt;
  pt = pp_tabs_at_xy(dock, ev->x, ev->y);
  if (pt) {
    pt->clicked = 1;
    pp_minib_refresh(dock);
    return 1;
  } else return 0;
}


static int
pp_tabs_handle_button_release(Dock *dock, XButtonEvent *event)
{
  Pinnipede *pp = dock->pinnipede;
  PinnipedeTab *pt;
  int i,ret = 0, need_refresh=0;
  pt = pp_tabs_at_xy(dock, event->x, event->y);
  if (pt && pt->clicked == 1) {
    Board *board = pt->site->board;
    if (event->button == Button1) {
      if (event->x > pt->x + pt->w - 6 && event->y > pt->y + pt->h - 6) {
	board->enabled = 1-board->enabled;
      } else {
	if (pt != pp->active_tab) {
	  if (pt->selected) {
	    pp->active_tab = pt;
	  } else pt->selected = 1;
	} else {
	  int all_active = 1;
	  for (i=0; i < pp->nb_tabs; i++) if (pp->tabs[i].selected == 0) all_active = 0;
	  if (all_active) {
	    for (i=0; i < pp->nb_tabs; i++) 
	      pp->tabs[i].selected = (pp->tabs+i == pt);
	  } else {
	    for (i=0; i < pp->nb_tabs; i++) 
	      pp->tabs[i].selected = 1;
	  }
	}
      }
    } else if (event->button == Button2) {
      board->update_request = 1;
    } else if (event->button == Button3) {
      if (pt->selected == 0) pt->selected = 1;
    }

    pp_tabs_set_visible_sites(pp);

    pp_pv_destroy(pp);
    //    pp_update_content(dock, pp->id_base, pp->decal_base,0,1);
    pp_update_content(dock, get_last_id_filtered(dock->sites->boards, &pp->filter), 100,0,1);
    pp_refresh(dock, pp->win, NULL);
    ret = 1;
  }
  for (i=0; i < pp->nb_tabs; i++) {
    if (pp->tabs[i].clicked) need_refresh = 1;
    pp->tabs[i].clicked = 0;
  }

  if (need_refresh) pp_tabs_refresh(dock);
  return ret;
}


/* ------------------- gestion de la barre de boutons -------------------------- */
static void
pp_minib_set_pos(Pinnipede *pp)
{
  int i;

  int x;

  x = pp->win_width; //(pp->sc ? SC_W : 0);
  i = 0;
  pp->mb[i].type = HELP;            pp->mb[i].w = SC_W-1; x -= pp->mb[i].w; pp->mb[i].x = x; i++;
  pp->mb[i].type = SCROLLBAR;       pp->mb[i].w = 12; x -= pp->mb[i].w; pp->mb[i].x = x; i++;
  pp->mb[i].type = TRANSPARENT;     pp->mb[i].w = 12; x -= pp->mb[i].w; pp->mb[i].x = x; i++;
  pp->mb[i].type = UA;              pp->mb[i].w = 12; x -= pp->mb[i].w; pp->mb[i].x = x; i++;
  pp->mb[i].type = SECOND;          pp->mb[i].w = 12; x -= pp->mb[i].w; pp->mb[i].x = x; i++;
  pp->mb[i].type = TSCORE;          pp->mb[i].w = 12; x -= pp->mb[i].w; pp->mb[i].x = x; i++;
  pp->mb[i].type = FORTUNE;         pp->mb[i].w = 12; x -= pp->mb[i].w; pp->mb[i].x = x; i++;
  pp->mb[i].type = FILTER;          pp->mb[i].w = 12; x -= pp->mb[i].w; pp->mb[i].x = x; i++;
  pp->mb[i].type = PLOPIFY;         pp->mb[i].w = 12; x -= pp->mb[i].w; pp->mb[i].x = x; i++;
  //  pp->mb[i].type = REFRESH_NEWS;    pp->mb[i].w = 60; x -= pp->mb[i].w; pp->mb[i].x = x; i++;
  //  pp->mb[i].type = REFRESH_TRIBUNE; pp->mb[i].w = 60; x -= pp->mb[i].w; pp->mb[i].x = x; i++;


  pp->mb_buttonbar_width = pp->win_width - x;
  assert(i == NB_MINIB);

  for (i=0; i < NB_MINIB; i++) {
    pp->mb[i].clicked = 0;
    pp->mb[i].y = pp->win_height - MINIB_H;
    pp->mb[i].h = MINIB_H;
  }
  pp->mb_min_width = MIN(pp->win_width, 200 + pp->mb_buttonbar_width);
}


/* a nice piece of shit, if i can say */
void
pp_minib_refresh(Dock *dock)
{
  Pinnipede *pp = dock->pinnipede;
  Boards *boards = dock->sites->boards;
  SitePrefs *main_prefs; 
  Site *main_site;
  int i,x,y;

  int x_minib;

  main_prefs = pp->active_tab->site->prefs;
  main_site = pp->active_tab->site;

  if (pp->use_minibar == 0) return;

  /* dessin general */
  XSetForeground(dock->display, dock->NormalGC, pp->minib_pixel);
  XFillRectangle(dock->display, pp->lpix, dock->NormalGC, 0, 1, pp->win_width, MINIB_H-1);
  XSetForeground(dock->display, dock->NormalGC, pp->minib_dark_pixel);
  XDrawLine(dock->display, pp->lpix, dock->NormalGC, 0, 0, pp->win_width, 0);

  x_minib = pp->mb[0].x;
  for (i=0; i < NB_MINIB; i++) x_minib = MIN(x_minib, pp->mb[i].x);

  XSetFont(dock->display, dock->NormalGC, pp->fn_minib->fid);
  if (pp->filter.filter_mode) {
    char s_filtre[70];
    
    if (pp->filter.filter_name) {
      snprintf(s_filtre, 60, "%.40s [match:%d msg]", 
	       pp->filter.filter_name, count_all_id_filtered(boards, &pp->filter));
    } else {
      snprintf(s_filtre, 60, _("FILTER UNDEFINED"));
    }
    XSetForeground(dock->display, dock->NormalGC, pp->minib_dark_pixel);
    XDrawString(dock->display, pp->lpix, dock->NormalGC, 5+pp->mb_x0, pp->fn_minib->ascent+1, s_filtre, strlen(s_filtre));
  } else {
    
    /* affichage de la charge du serveur de dlfp */

    char s_cpu[20];
    char s_xp[20], s_votes[20], s_nb_messages[20], s_http_stats[40];
    int x, w;
    s_xp[0] = 0; s_cpu[0] = 0; s_votes[0] = 0; s_nb_messages[0] = 0;
    if (main_prefs->user_cookie || main_prefs->force_fortune_retrieval) {
      if (flag_updating_comments == 0) {
	/*
	  
	le CPU a disparu (ouuiiiinnn!)
	if (dock->dlfp->CPU != -1.0) {
	snprintf(s_cpu, 20, "cpu:%1.2f", dock->dlfp->CPU);
	} else snprintf(s_cpu, 20, "cpu: ??");
	*/
	if (main_site->xp >= 0) {
	  snprintf(s_xp, 20, "xp:%d", main_site->xp);
	  snprintf(s_votes, 20, "[%d/%d]", main_site->votes_cur, main_site->votes_max);
	}
      }
    }
    snprintf(s_nb_messages, 20, "%d msg", count_all_id_filtered(boards, &pp->filter));
    snprintf(s_http_stats, 40, "UP:%d, DL:%d", global_http_upload_cnt, global_http_download_cnt);
    XSetForeground(dock->display, dock->NormalGC, BlackPixel(dock->display, dock->screennum));
    
    x = 5+pp->mb_x0;
    w = MINIB_FN_W*strlen(s_cpu);
    if (x+w < x_minib) {
      XDrawString(dock->display, pp->lpix, dock->NormalGC, x, pp->fn_minib->ascent+1, s_cpu, strlen(s_cpu));
    }
    x += w + 6;
    w = MINIB_FN_W*strlen(s_xp);
    if (strlen(s_xp)) {
      if (x+w < x_minib) {
	XDrawString(dock->display, pp->lpix, dock->NormalGC, x, pp->fn_minib->ascent+1, s_xp, strlen(s_xp));
      }
    }
    x += w + 6;
    w = MINIB_FN_W*strlen(s_votes);
    if (strlen(s_votes)) {
      if (x+w < x_minib) {
	XDrawString(dock->display, pp->lpix, dock->NormalGC, x, pp->fn_minib->ascent+1, s_votes, strlen(s_votes));
      }
    }
    x += w + 6;
    w = MINIB_FN_W*strlen(s_nb_messages);
    if (strlen(s_nb_messages)) {
      if (x+w < x_minib) {
	XSetForeground(dock->display, dock->NormalGC, pp->minib_msgcnt_pixel);	
	XDrawString(dock->display, pp->lpix, dock->NormalGC, x, pp->fn_minib->ascent+1, s_nb_messages, strlen(s_nb_messages));
      }
    }
    x += w + 6;
    w = MINIB_FN_W*strlen(s_http_stats);
    if (strlen(s_http_stats)) {
      if (x+w < x_minib) {
	XSetForeground(dock->display, dock->NormalGC, pp->minib_updlcnt_pixel);
	XDrawString(dock->display, pp->lpix, dock->NormalGC, x, pp->fn_minib->ascent+1, s_http_stats, strlen(s_http_stats));
      }
    }

  }

  /* dessin des boutons */
  for (i=0;i < NB_MINIB; i++) {
    int xc;

    x = pp->mb[i].x;
    xc = x + pp->mb[i].w / 2;
    y = 1;

    XSetForeground(dock->display, dock->NormalGC, pp->minib_dark_pixel);
    XDrawRectangle(dock->display, pp->lpix, dock->NormalGC, x, 0, pp->mb[i].w, pp->mb[i].h);


/*     if (pp->mb[i].clicked && pp->mb[i].type != REFRESH_TRIBUNE && pp->mb[i].type != REFRESH_NEWS) { */
      XSetForeground(dock->display, dock->NormalGC, pp->minib_dark_pixel);
      XFillRectangle(dock->display, pp->lpix, dock->NormalGC, x, 1, pp->mb[i].w, pp->mb[i].h);
/*     }     */
    switch (pp->mb[i].type) {
    case SCROLLBAR:
      {
	XSetForeground(dock->display, dock->NormalGC, pp->minib_dark_pixel);
	XDrawLine(dock->display, pp->lpix, dock->NormalGC, xc, 2, xc, 8);
	XDrawLine(dock->display, pp->lpix, dock->NormalGC, xc-1, 3, xc-1, 7);
	XDrawLine(dock->display, pp->lpix, dock->NormalGC, xc-2, 4, xc-2, 6);
	XDrawLine(dock->display, pp->lpix, dock->NormalGC, xc+1, 3, xc+1, 7);
	XDrawLine(dock->display, pp->lpix, dock->NormalGC, xc+2, 4, xc+2, 6);
      } break;
    case TRANSPARENT:
      {
	int j;
	for (j=0; j < 7; j++) {
	  XSetForeground(dock->display, dock->NormalGC, RGB2PIXEL((j*40),(6-j)*30,0));
	  XDrawLine(dock->display, pp->lpix, dock->NormalGC, xc+j-3, 3, xc+j-3, 7);
	}
      } break;
      /*    case REFRESH_TRIBUNE:
      {
	char *s;

	s = "board";
	if (pp->mb[i].clicked == 0) {
	  int w, draw_bar = 0;
	  unsigned long bar_pixel = pp->minib_progress_bar_pixel;

	  if (dock->wmcc_state_info == WMCC_SENDING_COINCOIN) { s = "post.."; draw_bar = 0; }
	  else if (dock->wmcc_state_info == WMCC_UPDATING_BOARD) { s = "updating"; draw_bar = 0; }
	  else if (dock->tribune_update_request) { s = "queued"; draw_bar = 0; }
	  else { draw_bar = 1; if (dock->wmcc_state_info != WMCC_IDLE) bar_pixel = IRGB2PIXEL(0xffc0c0); }
	  
	  if (draw_bar) {
	    XSetForeground(dock->display, dock->NormalGC, bar_pixel);
	    if (dock->tribune_refresh_delay > 0) {
	      w = ((pp->mb[i].w-2)*MIN(dock->tribune_refresh_cnt,dock->tribune_refresh_delay))/dock->tribune_refresh_delay;
	      w = (pp->mb[i].w-2 - w);
	      //	      printf("cnt: %d delai: %d w=%d\n", dock->tribune_refresh_cnt, dock->tribune_refresh_delay, w);
	      if (w > 0) {
		XFillRectangle(dock->display, pp->lpix, dock->NormalGC, x+1, 1, w, pp->mb[i].h-1);
	      }
	    }
	  }
	}
	XSetForeground(dock->display, dock->NormalGC, IRGB2PIXEL(0x303030));
	XDrawString(dock->display, pp->lpix, dock->NormalGC, xc-(MINIB_FN_W*strlen(s))/2, pp->fn_minib->ascent+1, s, strlen(s));
      } break;

    case REFRESH_NEWS:
      {
	char *s;

	s = "news/msg";
	if (pp->mb[i].clicked == 0) {
	  int w, draw_bar = 0;
	  unsigned long bar_pixel = pp->minib_progress_bar_pixel;
	  
	  if (dock->wmcc_state_info == WMCC_UPDATING_NEWS) { s = "dl news"; draw_bar = 0; }
	  else if (dock->wmcc_state_info == WMCC_UPDATING_COMMENTS) { s = "dl com"; draw_bar = 0; }
	  else if (dock->wmcc_state_info == WMCC_UPDATING_MESSAGES) { s = "dl msg"; draw_bar = 0; }
	  else if (dock->news_update_request) { s = "queued"; draw_bar = 0; }
	  else { draw_bar = 1; if (dock->wmcc_state_info != WMCC_IDLE) bar_pixel = IRGB2PIXEL(0xffc0c0); }
	  
	  if (draw_bar) {
	    XSetForeground(dock->display, dock->NormalGC, bar_pixel);
	    if (dock->news_refresh_delay > 0) {
	      w = ((pp->mb[i].w-2)*MIN(dock->news_refresh_cnt,dock->news_refresh_delay))/dock->news_refresh_delay;
	      w = (pp->mb[i].w-2 - w);
	      //	      printf("news : cnt: %d delai: %d w=%d\n", dock->news_refresh_cnt, dock->news_refresh_delay, w);
	      if (w > 0) {
		XFillRectangle(dock->display, pp->lpix, dock->NormalGC, x+1, 1, w, pp->mb[i].h-1);
	      }
	    }
	  }
	}
	XSetForeground(dock->display, dock->NormalGC, IRGB2PIXEL(0x303030));
	XDrawString(dock->display, pp->lpix, dock->NormalGC, xc-(MINIB_FN_W*strlen(s))/2, pp->fn_minib->ascent+1, s, strlen(s));
      } break;*/
    case HELP:
      {
	XDrawString(dock->display, pp->lpix, dock->NormalGC, xc-MINIB_FN_W/2+1, pp->fn_minib->ascent+1, "?", 1);
      } break;
    case SECOND:
      {
	int rx, rw, ry, rh;

	rx = x + 3; ry  = y+2; rw = pp->mb[i].w-6; rh = pp->mb[i].h-6;
	XSetForeground(dock->display, dock->NormalGC, pp->timestamp_pixel[main_site->site_id]);
	
	if (pp->nosec_mode) {
	  XFillRectangle(dock->display, pp->lpix, dock->NormalGC, rx, ry, rw+1, rh+1);
	} else {
	  XDrawRectangle(dock->display, pp->lpix, dock->NormalGC, rx, ry, rw, rh);
	}
      } break;
    case TSCORE:
      {
	int rx, rw, ry, rh;

	rx = x + 3; ry  = y+2; rw = pp->mb[i].w-6; rh = pp->mb[i].h-6;
	XSetForeground(dock->display, dock->NormalGC, pp->trollscore_pixel[main_site->site_id]);
	
	if (pp->trollscore_mode) {
	  XFillRectangle(dock->display, pp->lpix, dock->NormalGC, rx, ry, rw+1, rh+1);
	} else {
	  XDrawRectangle(dock->display, pp->lpix, dock->NormalGC, rx, ry, rw, rh);
	}
      } break;
    case FORTUNE:
      {
	int rx, rw, ry, rh;

	rx = x + 3; ry  = y+2; rw = pp->mb[i].w-6; rh = pp->mb[i].h-6;
	XSetForeground(dock->display, dock->NormalGC, IRGB2PIXEL(0x8f8060));
	
	if (pp->fortune_mode) {
	  XFillRectangle(dock->display, pp->lpix, dock->NormalGC, rx, ry, rw+1, rh+1);
	} else {
	  XDrawRectangle(dock->display, pp->lpix, dock->NormalGC, rx, ry, rw, rh);
	}
      } break;
    case FILTER:
      {
	int rx, rw, ry, rh;

	rx = x + 3; ry  = y+2; rw = pp->mb[i].w-6; rh = pp->mb[i].h-6;
	XSetForeground(dock->display, dock->NormalGC, pp->lnk_pixel[main_site->site_id]);
	
	if (pp->filter.filter_mode) {
	  XFillRectangle(dock->display, pp->lpix, dock->NormalGC, rx, ry, rw+1, rh+1);
	} else {
	  XDrawRectangle(dock->display, pp->lpix, dock->NormalGC, rx, ry, rw, rh);
	}
      } break;
    case PLOPIFY:
      {
	int rx, rw, ry, rh;

	rx = x + 3; ry  = y+2; rw = pp->mb[i].w-6; rh = pp->mb[i].h-6;
	XSetForeground(dock->display, dock->NormalGC, pp->plopify_pixel);
	
	if (pp->disable_plopify) {
	  XFillRectangle(dock->display, pp->lpix, dock->NormalGC, rx, ry, rw+1, rh+1);
	} else {
	  XDrawRectangle(dock->display, pp->lpix, dock->NormalGC, rx, ry, rw, rh);
	}
      } break;
    case UA:
      {
	int rx, rw, ry, rh;

	rx = x + 3; ry  = y+2; rw = pp->mb[i].w-6; rh = pp->mb[i].h-6;
	XSetForeground(dock->display, dock->NormalGC, pp->nick_pixel[main_site->site_id]);
	
	XDrawRectangle(dock->display, pp->lpix, dock->NormalGC, rx, ry, rw, rh);
	if (pp->nick_mode == 3) {
	  XFillRectangle(dock->display, pp->lpix, dock->NormalGC, rx, ry, rw, rh);
	} else if (pp->nick_mode == 1) {
	  XFillRectangle(dock->display, pp->lpix, dock->NormalGC, rx,ry,xc-rx+1,rh);
	} else if (pp->nick_mode == 2) {
	  XFillRectangle(dock->display, pp->lpix, dock->NormalGC, xc,ry,xc-rx+1,rh);
	} else if (pp->nick_mode == 4) {
	  XDrawLine(dock->display, pp->lpix, dock->NormalGC, xc, ry, xc, ry+rh-1);
	}
      } break;
    default:
      abort(); break;
    }
  }

  /* et hop */
  XCopyArea(dock->display, pp->lpix, pp->win, dock->NormalGC,
	    pp->mb_x0,0,pp->win_width - pp->mb_x0, MINIB_H, 
	    pp->mb_x0, MINIB_Y0);

}


static PPMinib *
pp_minib_get_button(Dock *dock, int x, int y)
{
  Pinnipede *pp = dock->pinnipede;
  int i;

  for (i=0; i < NB_MINIB; i++) {
    if (x >= pp->mb[i].x && x < pp->mb[i].x+pp->mb[i].w && 
	y >= pp->mb[i].y && y < pp->mb[i].y+pp->mb[i].h) {
      return &pp->mb[i];
    }
  }
  return NULL;
}

static PPMinib *
pp_minib_pressed_button(Dock *dock)
{
  Pinnipede *pp = dock->pinnipede;
  int i;

  for (i=0; i < NB_MINIB; i++) {
    if (pp->mb[i].clicked)
      return &pp->mb[i];
  }
  return NULL;
}

void
pp_minib_show(Dock *dock) 
{
  Pinnipede *pp = dock->pinnipede;
  pp->use_minibar = 1;
  pp_tabs_set_pos(pp);
}

void 
pp_minib_hide(Dock *dock) 
{
  Pinnipede *pp = dock->pinnipede;
  pp->use_minibar = 0;
  pp_tabs_set_pos(pp);
}



static int
pp_minib_handle_button_press(Dock *dock, XButtonEvent *ev) {
  PPMinib *mb;
  mb = pp_minib_get_button(dock, ev->x, ev->y);
  if (mb) {
    mb->clicked = 1;
    pp_minib_refresh(dock);
    return 1;
  } else return 0;
}


static int
pp_minib_handle_button_release(Dock *dock, XButtonEvent *event)
{
  Pinnipede *pp = dock->pinnipede;

  PPMinib *mb;
  int i, need_refresh, ret = 0;

  SitePrefs *main_prefs = pp->active_tab->site->prefs;

  pp_selection_unselect(pp);


  mb = pp_minib_get_button(dock, event->x, event->y);
  if (event->type == ButtonRelease  && mb && mb->clicked == 1) {
    switch (mb->type) {
      
/*     case REFRESH_TRIBUNE: */
/*       { */
/* 	dock->board_update_request = 1; pp_minib_refresh(dock); */
/*       } break; */

/*     case REFRESH_NEWS: */
/*       { */
/* 	dock->news_update_request = 1; pp_minib_refresh(dock); */
/*       } break; */
      
    case HELP:
      {
	pp_minib_refresh(dock);
	pp_balloon_help(dock, mb->x-20, mb->y-20);
      } break;
    case UA:
      {
	pp->nick_mode = (pp->nick_mode + 1) % 5;
	pp_pv_destroy(pp);
	pp_update_content(dock, pp->id_base, pp->decal_base,0,1);
	pp_refresh(dock, pp->win, NULL);	    
      } break;
    case SECOND:
      {
	pp->nosec_mode = (1-pp->nosec_mode);
	pp_pv_destroy(pp);
	pp_update_content(dock, pp->id_base, pp->decal_base,0,1);
	pp_refresh(dock, pp->win, NULL);	    
      } break;
    case FILTER:
      {
	pp->filter.filter_mode = (1-pp->filter.filter_mode);
	/* reset du scroll (necessaire, faut etre que le post 'id_base' 
	   soit bien affiché par le filtre) */
	if (pp->filter.filter_mode) pp->id_base = id_type_invalid_id(); 

	pp_pv_destroy(pp);
	pp_update_content(dock, pp->id_base, pp->decal_base,0,1);
	pp_refresh(dock, pp->win, NULL);	    	    
      } break;
    case PLOPIFY:
      {
	pp->disable_plopify = (1-pp->disable_plopify);
	pp->filter.filter_boitakon = 1-pp->disable_plopify;

	pp_pv_destroy(pp);
	pp_update_content(dock, pp->id_base, pp->decal_base,0,1);
	pp_refresh(dock, pp->win, NULL);	    	    
      } break;
    case TSCORE:
      {
	if (Prefs.enable_troll_detector) {
	  pp->trollscore_mode = (1-pp->trollscore_mode);
	  pp_pv_destroy(pp);
	  pp_update_content(dock, pp->id_base, pp->decal_base,0,1);
	  pp_refresh(dock, pp->win, NULL);	    	    
	} else {
	  msgbox_show(dock, _("Don't click on this button, the troll_detector is deactivated (see the option 'tribune.enable_troll_detector')."));
	}
      } break;
    case FORTUNE:
      {
	if (main_prefs->user_cookie || main_prefs->force_fortune_retrieval) {
	  pp->fortune_mode = (1-pp->fortune_mode);
	  pp_pv_destroy(pp);
	  pp_update_content(dock, pp->id_base, pp->decal_base,0,1);
	  pp_refresh(dock, pp->win, NULL);	    	    	
	} else {
	  msgbox_show(dock, _("Don't click on this button if you don't have given a cookie to wmcoincoin, nor activated the 'http.force_fortune_retrieval' option."));
	}
      } break;
    case SCROLLBAR:
      {
	if (pp->sc) { scrollcoin_destroy(pp->sc); pp->sc = NULL; }
	else { pp->sc = scrollcoin_create(1,1,1,pp->win_width-SC_W+1, 0, 
					  pp->win_height-20, pp->transparency_mode); }
	pp_pv_destroy(pp);
	pp_update_content(dock, pp->id_base, pp->decal_base,0,1);
	pp_refresh(dock, pp->win, NULL);
      } break;
    case TRANSPARENT:
      {
	if (Prefs.use_fake_real_transparency) {
	  pp_unmap(dock); XFlush(dock->display); 
	  usleep(150); /* pour laisser le temps aux autres applis de se refresher
			  on atteint des sommets de laideur
			  pas sur que c'était une bonne idée cette option use_fake_real_transparency
		       */
	}
	pp_change_transparency_mode(dock, 1-pp->transparency_mode);
	if (Prefs.use_fake_real_transparency) 
	  pp_show(dock);
	else {
	  pp_update_bg_pixmap(dock);
	}
	pp_refresh(dock, pp->win, NULL);
      } break;
    default:
      assert(0); 
    }
    ret = 1;
  }
  for (i=0, need_refresh=0; i < NB_MINIB; i++) {
    if (pp->mb[i].clicked) {
      pp->mb[i].clicked = 0; need_refresh++;
    }
  }
  if (need_refresh) { pp_minib_refresh(dock); }
  return ret;
}




/* ---------------------- gestion des fortunes ------------------- */

/* redessine la fortune */
void
pp_refresh_fortune(Dock *dock, Drawable d)
{
  Pinnipede *pp = dock->pinnipede;
  Pixmap fpix;

  if (pp->fortune_h > 0) {
    int x;
    assert(pp->fortune_h < 10000); /* tout de meme */

    fpix = XCreatePixmap(dock->display, pp->win, pp->win_width, pp->fortune_h, 
			 DefaultDepth(dock->display,dock->screennum));
    XSetForeground(dock->display, dock->NormalGC, IRGB2PIXEL(Prefs.pp_fortune_bgcolor));
    XFillRectangle(dock->display, fpix, dock->NormalGC, 0, 0, pp->win_width, pp->fortune_h);
    XSetForeground(dock->display, dock->NormalGC, RGB2PIXEL(128,128,128));
    XDrawLine(dock->display, fpix, dock->NormalGC, 0, pp->fortune_h-1, pp->win_width, pp->fortune_h-1);

    assert(!picohtml_isempty(pp->ph_fortune));

    x = (pp->win_width - pp->fortune_w)/2;
    picohtml_render(dock, pp->ph_fortune, fpix, dock->NormalGC, x, 0);
    XCopyArea(dock->display, fpix, d, dock->NormalGC, 0, 0, pp->win_width, pp->fortune_h, 0, 0);
    XFreePixmap(dock->display, fpix);
  } else {
  /* nettoyage ligne du haut */
    assert(LINEY0(0)>0);
    pp_clear_win_area(dock, 0, 0, pp->win_width, LINEY0(0));
  }
}

/* a appeler quand la fortune est changée */
void
pp_update_fortune(Dock *dock)
{
  Pinnipede *pp = dock->pinnipede;
  char * fortune = pp->active_tab->site->fortune;
  
  assert(flag_updating_comments == 0);
  if (!picohtml_isempty(pp->ph_fortune)) {
    picohtml_freetxt(pp->ph_fortune);
  }
  pp->fortune_h = 0; /* quand pp->fortune_h != 0 => il y a une fortune à afficher */
  pp->fortune_w = 0;


  if (pp->fortune_mode && fortune) {
    if (fortune == NULL) fortune = "pas de fortune pour l'instant...<br>";
    picohtml_parse(dock, pp->ph_fortune, fortune, pp->win_width - 6);
    picohtml_gettxtextent(pp->ph_fortune, &pp->fortune_w, &pp->fortune_h);
    if (!picohtml_isempty(pp->ph_fortune)) { /* on s'arrête si la fortune est vide (s == "" par ex..)
						(ça peut arriver et ça declenche le assert(!isempty) de refresh_fortune) */
      pp->fortune_h += 3; 
      pp->fortune_h = MIN(pp->fortune_h, pp->win_height/2); /* faut pas exagerer */
    }
  }
}


void
pp_check_balloons(Dock *dock, int x, int y)
{
  Pinnipede *pp = dock->pinnipede;
  int i;

  if (pp->use_minibar) {
    for (i=0; i < NB_MINIB; i++) {
      char *msg = NULL;
      switch (pp->mb[i].type) {
      case HELP: msg = _("Bring some help"); break;
      case SCROLLBAR: msg = _("Bring/hide the scrollcoin"); break;
      case TRANSPARENT: msg = _("Activate/deactivate the pseudo-transparency"); break;
      case UA: msg = _("Change the display mode of the logins/useragents (5 different modes)"); break;
      case SECOND: msg = _("Bring/hide the seconds (when there are less than two messages in the same minute)"); break;
      case TSCORE: msg = _("Bring/hide the troll score (the numbers on the left of some messages)"); break;
      case FORTUNE: msg = _("Bring/hide the fortune (if appropriate)"); break;
      case FILTER: msg = _("Activate/deactivate the <b>filter</b>. To filter the messages, do a <font color=#0000ff>ctrl+left clic</font> on a word/login/useragent or a clock (to display a thread). To remove the filter, just click here"); break;
      case PLOPIFY: msg = _("Change the plopification type (beware, you will also see the messages in the boitakon!). <p> To plopify a message, <font color=#0000ff>shift+right click</font> on a word/login/useragent/clock (or the zone on the left of the clock to plopify a thread). To unplopify (or let someone out of the boitakon), just click on the same place.<br> To access to the superplopification, do a <font color=#0000ff>Mod1+shift+right click</font><br> To put a login/ua/etc. in the <b>boitakon</b>(tm), you have to use the mega combo <font color=#0000ff>Ctrl+Mod4+Mod1+shift+right click</font>."); break;
/*       case REFRESH_NEWS: msg = _("Click here to force the refresh of the news, messages, fortune and XP"); break; */
/*       case REFRESH_TRIBUNE: msg = _("Click here to force the refresh of the board"); break; */
      default: assert(0);
      }
      balloon_test(dock, x, y, pp->win_xpos, pp->win_ypos-15, 0, 
		   pp->mb[i].x, MINIB_Y0, 
		   pp->mb[i].w, MINIB_H, msg);
    }
  }
}

/* ---------------------------------------- scrollcoin ----------------------- */

/* lecture de la scrollbar, avec un refresh legerement differé pour éviter de trop charger... */
void
pp_check_scroll_pos(Dock *dock)
{
  Pinnipede *pp = dock->pinnipede;
  Boards *boards = dock->sites->boards;
  
  static int refresh_requested = 0;
  static int refresh_nb_delayed = 0;
  int new_pos;
 
  if (pp->sc == NULL) return;
  if (scrollcoin_read_requested_pos(pp->sc, &new_pos)) {
    id_type id;
    /* petit bricolage pas beau pour que ça affiche completement le premier post: 
       update_content rescrollera juste ce qu'il faut */
    if (new_pos == scrollcoin_get_vmin(pp->sc)) new_pos--;

    id = get_nth_id_filtered(boards, &pp->filter, new_pos);
    //    myprintf("scroll pos = %<MAG %d>, --> id_base = %d\n", new_pos, id);
    

    pp_update_content(dock, id, 0,0,0);
    refresh_requested = 2;
  }
  if (refresh_requested == 1 || refresh_nb_delayed >= 0) {
    pp_refresh(dock, pp->win, NULL);
    refresh_nb_delayed = 0; 
    refresh_requested = 0;
  } else if (refresh_requested > 0) {
    refresh_requested--; refresh_nb_delayed++;
  }
}

/* positionnenement de la scrollbar */
static void
pp_scrollcoin_move_resize(Dock *dock)
{
  Pinnipede *pp = dock->pinnipede;
  /*  int y;
  y = LINEY0(0);
  scrollcoin_resize(pp->sc, pp->win_width - SC_W+1, y, pp->win_height - y - (pp->use_minibar ? MINIB_H-1 : 0));*/
  scrollcoin_resize(pp->sc, pp->win_width - SC_W+1, pp->zmsg_y1, pp->zmsg_h);
}


/*
  ça tient du vilain bricolage.. mais bon: la scrollbar doit connaitre ses bornes
  le chiffre qu'elle renvoie correspond à l'id du message affiché sur la dernière ligne du pinnipede
  
  le problème est alors la borne vmin: c'est l'id du dernier message de la première page,
  ce qui demande un certain nombre de manip pour le determiner

  (si scroll_coin avait travaillé par ligne plutot que par id, ça aurait été encore pire)
*/

void
pp_scrollcoin_update_bounds(Dock *dock)
{
  Pinnipede *pp = dock->pinnipede;
  Boards *boards = dock->sites->boards;
  int lcnt;
  int vmin, vmax;
  board_msg_info *mi;

  if (pp->sc == NULL) return;
  
  //  myprintf("%<YEL pp_scrollcoin_update_bounds>\n");
  vmax = count_all_id_filtered(boards, &pp->filter);
  vmin = 0;
  lcnt = 0;

  mi = boards->first;
  while (lcnt < pp->nb_lignes && mi) {
    if (filter_msg_info(mi,&pp->filter)) {
      PostVisual *pv;
      pv = pp_pv_add(pp, boards, mi->id); 
      if (pv) { lcnt += pv->nblig; vmin++; }
    }
    mi = mi->g_next;
  }
  scrollcoin_setbounds(pp->sc, vmin, vmax);
  pp_scrollcoin_move_resize(dock);
}

/* ---------------------- fonctions qui regroupent tout ------------------*/

void
pp_widgets_set_pos(Dock *dock)
{
  Pinnipede *pp = dock->pinnipede;
  pp_minib_set_pos(pp);
  pp_tabs_set_pos(pp);
  if (pp->sc) { pp_scrollcoin_move_resize(dock); }
}

void
pp_widgets_refresh(Dock *dock)
{
  Pinnipede *pp = dock->pinnipede;
  if (pp->use_minibar) {
    pp_tabs_refresh(dock);
    pp_minib_refresh(dock);
  }
  if (pp->sc) { scrollcoin_refresh(pp->sc, pp->win, 1); }
}

int
pp_widgets_handle_button_press(Dock *dock, XButtonEvent *event)
{
  Pinnipede *pp = dock->pinnipede;
  if (pp->sc && scrollcoin_handle_button_press(pp->sc, event, pp->win))
    return 1;
  else if (pp->use_minibar) {  
    if (pp_minib_handle_button_press(dock,event)) return 1;
    else if (pp_tabs_handle_button_press(dock,event)) return 1;
  }
  return 0;
}

int
pp_widgets_handle_button_release(Dock *dock, XButtonEvent *event)
{
  Pinnipede *pp = dock->pinnipede;
  if (pp->sc && scrollcoin_handle_button_release(pp->sc, event, pp->win)) {
    pp_check_scroll_pos(dock); return 1;
  } else if (pp->use_minibar) {
    if (pp_minib_handle_button_release(dock,event)) { return 1;}
    else if (pp_tabs_handle_button_release(dock,event)) { return 1; }
  }
  return 0;
}

int 
pp_widgets_handle_motion(Dock *dock, XMotionEvent *event)
{
  Pinnipede *pp = dock->pinnipede;
  if (pp->sc && scrollcoin_handle_motion(pp->sc, event, pp->win)) {
    pp_check_scroll_pos(dock); return 1;
  } else if (pp_minib_pressed_button(dock)) {
	/* ben oui, on s'en fout */
    return 1;
  } else if (pp_tabs_pressed_tab(dock)) {
    return 1;
  } return 0;
}
