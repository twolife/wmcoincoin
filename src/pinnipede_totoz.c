#include <signal.h>
#include "pinnipede.h"
#include <X11/extensions/shape.h>

struct pp_totoz {
  Window win, subwin;
  int survol_hash;
  pid_t animate_pid;
};

void
pp_totoz_unmap(Dock *dock) {
  Pinnipede *pp = dock->pinnipede;
  if (pp->totoz->win != None) {
    XDestroyWindow(dock->display, pp->totoz->win);
    pp->totoz->win = None;
  }
  if (pp->totoz->animate_pid) {
    kill(pp->totoz->animate_pid, SIGKILL); /* animate est une grosse bouse pas capable de mourir comme un grand quand 
                                             sa fenetre est détruite. Je suis déçu déçu. C'est pourquoi il se prend 
                                             un gros SIGKILL dans la face. Dis bonjour au ramasse-zombie
                                          */
    pp->totoz->animate_pid = 0;
  }
}

void
pp_totoz_build(Dock *dock) {
  ALLOC_OBJ(dock->pinnipede->totoz, struct pp_totoz);
  pp_totoz_rebuild(dock);
}

void
pp_totoz_rebuild(Dock *dock) {
  Pinnipede *pp = dock->pinnipede;
  assert(pp->totoz);
  pp_totoz_unmap(dock);
  pp->totoz->win = None;
  pp->totoz->survol_hash = -42;
  pp->totoz->animate_pid = 0;
}

void
pp_totoz_destroy(Dock *dock) {
  Pinnipede *pp = dock->pinnipede;
  pp_totoz_unmap(dock);
  if (pp->totoz) {
    free(pp->totoz); pp->totoz = NULL;
  }
}

static void
pp_totoz_show(Dock *dock, int x_, int y_, int w, int h) {
  Pinnipede *pp = dock->pinnipede;
  XSetWindowAttributes wa;
  int x,y;
  pp_totoz_unmap(dock);

  x = MAX(x_ - w/2, 0);
  y = y_ - h - 20; /* dessus */
  if (y < 0) y = y_ + 20; /* dessous */
  if (y+h > pp->win_height) { /* à gauche ? */
    y = MAX(y_ - h/2, 0);
    x = x_ - w - 20;
    if (x < 0) x = x_ + 20;
    if (x+w > pp->win_width) { x = 0; }
  }

  pp->totoz->win = XCreateSimpleWindow (dock->display, pp->win, /*RootWindow(dock->display,dock->screennum), */
                                       x, y, w+2, h+2, 0,
                                       IRGB2PIXEL(0xffffff), IRGB2PIXEL(0xff0000));
  pp->totoz->subwin = XCreateSimpleWindow (dock->display, pp->totoz->win,
                                           1, 1, w, h, 0,
                                           IRGB2PIXEL(0xffffff), IRGB2PIXEL(0xffffff));
  wa.event_mask =
    ButtonPressMask | 
    ButtonReleaseMask | 
    ExposureMask;
  wa.override_redirect = False /*True*/ ;
  wa.save_under = True;
  XChangeWindowAttributes (dock->display, pp->totoz->win,
			   CWSaveUnder | CWEventMask | CWOverrideRedirect, &wa);
  /*XSetTransientForHint(dock->display, pp->totoz->win, pp->win);
  set_borderless_window_hints(dock->display, pp->totoz->win);
  set_window_class_hint(dock->display, pp->totoz->win, "wmcoincoin", "totoz");
  set_window_pos_hints(dock->display, pp->totoz->win, x, y);
  XSetWindowBorderWidth(dock->display, pp->totoz->win, 0);*/

  
  {
    Pixmap shape_mask = XCreatePixmap(dock->display, dock->rootwin, w+2, h+2, 1);
    GC monoGC = XCreateGC(dock->display, shape_mask, 0, NULL);
    XSetForeground(dock->display, monoGC, 0); 
    XFillRectangle(dock->display, shape_mask, monoGC, 0, 0, w+2, h+2);
    XSetForeground(dock->display, monoGC, 1); 
    XFillRectangle(dock->display, shape_mask, monoGC, 1, 1, w, h);
    XDrawLine(dock->display, shape_mask, monoGC, w-2, 0, w+1, 0);
    XDrawLine(dock->display, shape_mask, monoGC, w+1, 0, w+1, 4);
    XDrawLine(dock->display, shape_mask, monoGC, 0, 0, 4, 0);
    XDrawLine(dock->display, shape_mask, monoGC, 0, 0, 0, 4);
    XDrawLine(dock->display, shape_mask, monoGC, 0, h-2, 0, h+1);
    XDrawLine(dock->display, shape_mask, monoGC, 0, h+1, 4, h+1);
    XDrawLine(dock->display, shape_mask, monoGC, w+1, h-2, w+1, h+1);
    XDrawLine(dock->display, shape_mask, monoGC, w-2, h+1, w+1, h+1);
    XShapeCombineMask(dock->display, pp->totoz->win, ShapeBounding, 0, 0, shape_mask,
                      ShapeSet);
    XFreePixmap(dock->display, shape_mask);
    XFreeGC(dock->display, monoGC);
  }

  XMapRaised(dock->display, pp->totoz->win);
  XMapWindow(dock->display, pp->totoz->subwin);
}

int 
pp_totoz_dispatch_event(Dock *dock, XEvent *event)
{
  Pinnipede *pp = dock->pinnipede;
  if (pp->totoz->win != None && event->xany.window == pp->totoz->win) {
    printf("totoz event\n");
    switch (event->type) {
      //    case ButtonPress:
    case ButtonRelease:
      {
	pp_totoz_unmap(dock);
      } break;
    }
    return 1;
  }
  return 0;
}

/*
  entree: "[:totoz plop]"
  sortie: "totoz%20plop"
*/
static char *
pp_totoz_realfname(unsigned char *name) {
  int i;
  unsigned char *realname;
  unsigned char *s = strdup(name);
  assert(s);
  assert(s[0] == '[' && s[1] == ':' && s[strlen(name)-1] == ']');
  s[strlen(s)-1] = 0;
  for (i=0; s[i]; ++i) if (s[i] < ' ') s[i] = ' ';
  realname = http_url_encode(s+2,0);  assert(realname);
  free(s);
  return realname;
}

static void
pp_totoz_request(Dock *dock, char *imgname, int x, int y) {
  Pinnipede *pp = dock->pinnipede;
  char *urlencod_img = NULL;
  char fname[2048], s[512], info[1024], realname[512], mimetype[100];
  FILE *f;
  int use_gifview = 0;
  int tw,th;
  urlencod_img = pp_totoz_realfname(imgname);
  snprintf(fname,2048,"%s/.wmcoincoin/totoz/%s.desc", getenv("HOME"), urlencod_img);
  f = fopen(fname, "rt");

  printf("pp_totoz_request : %s\n", fname);
  /* record de concentration de gotos ! */
  if (f == NULL) { 
    snprintf(info,1024, "This picture [%s] has not been downloaded, use the popup menu (right clic) to get it (and wait a little..)", urlencod_img);
    pp_popup_show_txt(dock, info);
    printf("file %s not opened: %s\n", fname, strerror(errno)); 
    goto dommage_eliane;
  }
  if (fgets(s, 512,f) == NULL) { pp_popup_show_txt(dock, "grunt ?"); goto dommage_eliane; }
  fclose(f); f = NULL;
  if (strncmp(s, "NOTFOUND", 8) == 0) {
    snprintf(info,1024, "desolai but according to [~/.wmcoincoin/totoz/%s.desc] this picture does not exist (like the cabale)", urlencod_img);
    pp_popup_show_txt(dock, info);
    goto dommage_eliane;
  }
  if (sscanf(s, " %511s %dx%d %99s", realname, &tw, &th, mimetype) != 4) { pp_popup_show_txt(dock, "uuurg ?"); goto dommage_eliane; }
  if (strcasecmp(mimetype,"image/gif") == 0) use_gifview = 1;

  //  printf("ouéééé on va afficher '%s' : %dx%d (survol_hash=%08x)\n", s, tw,th, pp->totoz->survol_hash);

  snprintf(info,1024, "%s [%s], size %dx%d, using %s", realname, mimetype, tw,th, use_gifview ? "gifview" : "animate");
  pp_popup_show_txt(dock, info);

  pp_totoz_show(dock, x, y, tw, th);
  /* rajouter un check anti-fork bomb */
  switch ((pp->totoz->animate_pid = fork())) {
  case -1: /* arrrrg */
    fprintf(stderr, _("Fork failed...(%s)\n"), strerror(errno)); pp->totoz->animate_pid = 0; break;
  case 0: {
    char *swin = str_printf("0x%08lx",(unsigned long)pp->totoz->subwin); assert(swin);
    char *sfile= str_printf("%s/.wmcoincoin/totoz/%s",getenv("HOME"),realname); assert(sfile);
    sfile = shell_quote(sfile); assert(sfile);  /* et une deuxième couche, a priori parfaitement inutile */
    //    int ret = execlp("/usr/bin/animate", "/usr/bin/animate", "-virtual-pixel", "Constant", "-background", "white", "-window", swin, sfile, NULL, NULL);
    int ret;
    if (use_gifview) {
      BLAHBLAH(0,myprintf("exec gifview +e -a --bg white -w %s %<YEL %s>\n", swin, sfile));
      ret = execlp("/usr/bin/gifview", "/usr/bin/gifview", "+e", "-a", "--bg", "white", "-w", swin, sfile, NULL, NULL);
    } else {
      BLAHBLAH(0,myprintf("exec animate -window %s %<YEL %s>\n", swin, sfile));
      ret = execlp("/usr/bin/animate", "/usr/bin/animate", 
                   "-background", "white", "-window", swin, sfile, NULL, NULL);
    }
    
    if( ret==-1 ) {
      fprintf(stderr, _("Exec failed...(%s)\n"), strerror(errno));
    }
    exit(ret);
  } break;
  default:
    break;
  }

 dommage_eliane:  
  printf("dommage eliane\n");
  if (f) fclose(f);
  free(urlencod_img);
}

void
pp_check_totoz(Dock *dock, PostWord *pw, int x_root, int y_root) {
  Pinnipede *pp = dock->pinnipede;
  int survol_hash = 0;
  if (pw) survol_hash = str_hache(pw->w,50);
  if (pw && (pw->attr & PWATTR_TOTOZ))
    printf("%s : %d %d %08lx\n", pw->w, pp->totoz->survol_hash, pp->survol_hash, pp->totoz->win);
  if (pp->totoz->survol_hash != survol_hash) {
    if (pp->totoz->win != None) {
      pp_totoz_unmap(dock); pp->totoz->survol_hash = 0;
    }
    if (pw && (pw->attr & PWATTR_TOTOZ)) {
      pp_totoz_request(dock, pw->w, x_root, y_root);
    }
  }
  pp->totoz->survol_hash = survol_hash;
}

void
pp_totoz_download(Dock *dock, unsigned char *imgname) {
  char *realname = pp_totoz_realfname(imgname);
  char *cmd = NULL;
  cmd = str_printf("%s/wmcoincoin/wmcc/scripts/wmcoincoin-totoz-get %s&", getenv("HOME"), realname);
  BLAHBLAH(0, myprintf("Trying to get a new picture [%s] ! executing '%<GRN %s>'\n", realname, cmd));
  system(cmd);
  free(cmd);
  free(realname);
}
