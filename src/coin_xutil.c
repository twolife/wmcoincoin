/*
  coin_xutil : diverses fonctions complémentaires à raster.c pour la manip des images

  rcsid=$Id: coin_xutil.c,v 1.4 2002/04/02 22:29:28 pouaite Exp $

  ChangeLog:
  $Log: coin_xutil.c,v $
  Revision 1.4  2002/04/02 22:29:28  pouaite
  bugfixes transparence

  Revision 1.3  2002/04/01 23:04:11  pouaite
  fix compilation avec gcc 2.9x

  Revision 1.2  2002/04/01 22:56:03  pouaite
  la pseudo-transparence du pinni, bugfixes divers, option tribune.backend_type

  Revision 1.1  2002/04/01 01:39:38  pouaite
  grosse grosse commition (cf changelog)

*/

#include <X11/Xatom.h>
#include <X11/Xmd.h>
#include <string.h>
#include "coin_xutil.h"


RGBAImage *
rimage_create_from_raw(int w, int h, int bpp, const unsigned char *data)
{
  RGBAImage *rimg;
  int x,y;

  rimg = RGBACreateImage(w,h);
  for (y=0; y < h; y++) {
    for (x=0; x < w; x++) {
      int pos;
      pos = (y*w+x)*bpp;
      rimg->data[y][x].rgba[0] = data[pos];
      rimg->data[y][x].rgba[1] = data[pos+1];
      rimg->data[y][x].rgba[2] = data[pos+2];
      if (bpp == 4) {
	rimg->data[y][x].rgba[3] = data[pos+3];
      } else {
	rimg->data[y][x].rgba[3] = 0;
      }
    }
  }
  return rimg;
}

Pixmap
pixmap_create_from_raw(RGBAContext *rc, int w, int h, int bpp, const unsigned char *data)
{
  RGBAImage *rimg;
  Pixmap p;

  rimg = rimage_create_from_raw(w,h,bpp,data);
  p = RGBAImage2Pixmap(rc, rimg);
  RGBADestroyImage(rimg);
  return p;
}

RGBAImage *
rimage_create_from_raw_with_tint(int w, int h, int bpp, const unsigned char *data, int rgb)
{
  RGBAImage *rimg;
  int r,g,b;
  int i;

  r = (rgb >> 16) & 0xff; g = (rgb >> 8) & 0xff; b = rgb & 0xff;
  //  printf("rimage_create_from_raw_with_tint: rgb=%06x r=%d g=%d b=%d\n", rgb, r, g, b);
  rimg = RGBACreateImage(w,h);
  
  for (i=0; i < w*h; i++) {
    float v;
    v =  (data[i*bpp] + data[i*bpp+1] + data[i*bpp+2])/(3.*128);
    v = 1. + (v-1.)/2.5; // les couleurs de la porte sont un peu trop contrastees a mon gout
    rimg->data[0][i].rgba[0] = (unsigned char)MIN(v*r, 255);
    rimg->data[0][i].rgba[1] = (unsigned char)MIN(v*g, 255);
    rimg->data[0][i].rgba[2] = (unsigned char)MIN(v*b, 255);
    rimg->data[0][i].rgba[3] = 0;
  }

  /* c'est equivalent à */
  /*
  for (y=0; y < h; y++) {
    for (x=0; x < w; x++) {
      int pos;
      float v;
      pos = (y*w+x)*bpp;
      v =  (data[pos] + data[pos+1] + data[pos+2])/(3.*128);
      v = 1. + (v-1.)/2.5; // les couleurs de la porte sont un peu trop contrastees a mon gout
      rimg->data[y][x].rgba[0] = (unsigned char)MIN(v*r, 255);
      rimg->data[y][x].rgba[1] = (unsigned char)MIN(v*g, 255);
      rimg->data[y][x].rgba[2] = (unsigned char)MIN(v*b, 255);
      if (bpp == 4) {
	rimg->data[y][x].rgba[3] = data[pos+3];
      } else {
	rimg->data[y][x].rgba[3] = 0;
      }
    }
    }*/
  return rimg;
}

Pixmap
pixmap_create_from_raw_with_tint(RGBAContext *rc, int w, int h, int bpp, const unsigned char *data, int rgb)
{
  RGBAImage *rimg;
  Pixmap p;
  rimg = rimage_create_from_raw_with_tint(w,h,bpp,data,rgb);
  //rimg = rimage_create_from_raw(w,h,bpp,data);
  p = RGBAImage2Pixmap(rc, rimg);
  RGBADestroyImage(rimg);
  return p;
}

/* conversion du channel alpha d'une image RGBA (32bits) en un masque binaire
   utilise pour separer les deux morceaux de la porte

   TRES TRES largement inspire de createXBMfromXPM dans wmgeneral (sauf que moi
   je mets des commentaires ;-)
*/
void
alpha2mask(int w, int h, unsigned char *bitmap, const unsigned char *rgba, unsigned char mask_val)
{
  int i,j;
  unsigned char bits;
  unsigned int nbits;

  for (i=0; i < h; i++) {
    nbits = 0; bits = 0;
    for (j=0; j < w; j++) {
      bits >>= 1;
      if (rgba[i*w*4 + j*4 + 3] == mask_val) {
	bits |= 0x80;
      }
      //printf("%1d", rgba[i*w*4 + j*4 + 3] == mask_val);
      nbits++;
      if (nbits == 8) {
	*bitmap = bits;
	bitmap++; nbits = 0; bits = 0;
      }
    }
    //    printf("\n");
  }
}

Pixmap
alpha2pixmap_mask(Display *dpy, Window win, int w, int h, const unsigned char *rgba, unsigned char mask_val)
{
  unsigned char *bitmap;
  Pixmap p;

  ALLOC_VEC(bitmap, h*((w+7)/8), unsigned char);
  alpha2mask(w, h, bitmap, rgba, mask_val);
  p = XCreateBitmapFromData(dpy, win, bitmap, w, h);
  FREE_VEC(bitmap);
  return p;
}

/* sert à colorier le pixmap des lettres -> le noir devient la color 'tint' 

a virer: c'est nul, ca alloue de la memoire et c'est trop lourd de changer la couleur du texte defilant dans le dock
*/
void
colorize_black_pixmap(char **pixtxt, int tint)
{
  int w,h,ncolor;
  int i;

  assert(sscanf(pixtxt[0], "%d %d %d", &w, &h, &ncolor) == 3);
  for (i=1; i <= ncolor; i++) {
    int l,r,g,b;
    if (strstr(pixtxt[i], "None") == NULL) {
      char *s;
      s = strdup(pixtxt[i]);

      l = strlen(s); assert(l > 8);
      assert(sscanf(s+l-6, "%2x%2x%2x", &r, &g, &b));
      s[l-6] = '0';
      r = ((0xff-r)*((tint >> 16) & 0xff)) / 0xff;
      g = ((0xff-g)*((tint >>  8) & 0xff)) / 0xff;
      b = ((0xff-b)*((tint      ) & 0xff)) / 0xff;
      snprintf(s+l-6, 7, "%02x%02x%02x", r, g, b);
      
      pixtxt[i] = s;
    }
  }
}

 
void 
get_window_pos_with_decor(Display *display, Window base_win, int *screen_x, int *screen_y)
{
  Window child_win;
  Window parent_win;
  Window root_win;
  Window* child_windows;
  int num_child_windows;
  XWindowAttributes win_attr;
  Window win;
  
  *screen_x = -1; *screen_y = -1;
  win = base_win;
  
  while (XQueryTree(display, win,
		    &root_win,
		    &parent_win,
		    &child_windows, &num_child_windows)) {
    if (child_windows) XFree(child_windows);
    if (parent_win == root_win) break;
    win = parent_win;
  }
  
  //  printf("win = %lx, parent=%lx, root=%lx\n", win, parent_win, root_win);
  if (parent_win) {
    XGetWindowAttributes(display, win, &win_attr);
    XTranslateCoordinates(display, win, root_win,
			  -win_attr.border_width, -win_attr.border_width, screen_x, screen_y,
			  &child_win);
  } 
}

void 
get_window_pos_without_decor(Display *display, Window root_win, Window win, int *screen_x, int *screen_y)
{
  Window child_win;
  XWindowAttributes win_attr;

  XGetWindowAttributes(display, win, &win_attr);
  XTranslateCoordinates(display, win, root_win,
			0, 0, screen_x, screen_y,
			&child_win);
}

/* volée dans les sources de aterm :) 
   maintenant, faut voir dans quelles conditions cette fonction marche 
   (avec wmaker/wmsetbg ça marche en tout cas)

   race condition possible si le rootpixmap est changé entre le moment où cette fonction
   est appelée et le moment où on utilise le pixmap...
*/
Pixmap
get_rootwin_pixmap(const RGBAContext *rc)
{
  Atom id;
  Pixmap currentRootPixmap = None;
  id = XInternAtom (rc->dpy, "_XROOTPMAP_ID", True);

  if (id != None) {
    Atom act_type;
    int act_format;
    unsigned long nitems, bytes_after;
    unsigned char *prop = NULL;
    
    /*fprintf(stderr, "\n aterm GetRootPixmap(): root pixmap is set");    		    */
    if (XGetWindowProperty (rc->dpy, rc->drawable, id, 0, 1, False, XA_PIXMAP,
			    &act_type, &act_format, &nitems, &bytes_after,
			    &prop) == Success)
      {
	if (prop)
	  {
	    currentRootPixmap = *((Pixmap *) prop);
	    XFree (prop);
	  }
      }
  }
  return currentRootPixmap ;
}

/* une de mes macros les plus laides :) */
#define SELECT_SHADE_OR_TINT(_c1,_c2) { if (ti->type == SHADING) { _c1; \
          r = (r*shade_coef)/256 + shade_inc; \
          g = (g*shade_coef)/256 + shade_inc; \
          b = (b*shade_coef)/256 + shade_inc; \
          _c2; } else { int l; _c1; \
          l = (r*76)/256 + (g*150)/256 + (b*30)/256; \
          r = (l*wr + (256-l)*br)/256; \
	  g = (l*wg + (256-l)*bg)/256; \
	  b = (l*wb + (256-l)*bb)/256; _c2; } }

//#define TRANSFO(x,m,d) ((d) > 0 ? (((x)&(m))>>(unsigned)(d)) : (((x)&(m))<<(unsigned)(-(d))))

void
shade_XImage(const RGBAContext *rc, XImage *ximg, TransparencyInfo *ti)
{
  int wr=0,wg=0,wb=0,br=0,bg=0,bb=0; 
  int x, y;
  int shade_coef=0, shade_inc=0;

  if (ti->type == TINTING) {
    wr = (ti->tint.white & 0xff0000)>>16; 
    wg = (ti->tint.white & 0x00ff00)>>8; 
    wb = (ti->tint.white & 0x0000ff);
    br = (ti->tint.black & 0xff0000)>>16; 
    bg = (ti->tint.black & 0x00ff00)>>8; 
    bb = (ti->tint.black & 0x0000ff);
  } else if (ti->type == SHADING) {
  //  int l_tint = (tr*76)/256 + (tg*150)/256 + (tb*30)/256;
    shade_inc = ((ti->shade.luminosite)*256)/100;
    shade_coef = ((ti->shade.assombrissement-ti->shade.luminosite)*256)/100;
  }
  if (ximg->bits_per_pixel == 16 || ximg->bits_per_pixel == 15) {
    CARD16 *p = (CARD16*)ximg->data;
    SELECT_SHADE_OR_TINT(
			 for (y=0; y < ximg->height; y++) {
			   for (x=0; x < ximg->width; x++) {
			     int r = PIXEL2R(rc,p[x]);
			     int g = PIXEL2G(rc,p[x]);
			     int b = PIXEL2B(rc,p[x]);
				,;
				p[x] = rc->rtable[r] + rc->gtable[g] + rc->btable[b];
			   }
			   p += ximg->bytes_per_line/2;
			 }
			 );
  } else if (ximg->bits_per_pixel == 24) {
    unsigned char *p = (unsigned char*) ximg->data;
    SELECT_SHADE_OR_TINT(
			 for (y=0; y < ximg->height; y++) {
			   for (x=0; x < ximg->width; x++) {
			     int r = p[x*3];
			     int g = p[x*3+1];
			     int b = p[x*3+2];
			     ,;
			     p[x*3] = r; p[x*3+1] = g; p[x*3+2] = b;
			   }
			   p += ximg->bytes_per_line;
			 }
			 );
    
  } else if (ximg->bits_per_pixel == 32) {
    CARD32 *p = (CARD32*) ximg->data;
    SELECT_SHADE_OR_TINT(
			 for (y=0; y < ximg->height; y++) {
			   for (x=0; x < ximg->width; x++) {
			     int r = (p[x] & 0xff0000)>>16;
			     int g = (p[x] & 0x00ff00)>>8;
			     int b = (p[x] & 0x0000ff);;
			     ,;
			     p[x] = (p[x] & 0xff000000) + (r << 16) + (g << 8) + b;
			   }
			   p += ximg->bytes_per_line/4;
			 }
			 );
  } else {
    printf("shade_ximage non supporté (bitsperpix=%d) !! bizarre\n",ximg->bits_per_pixel);
  }
}

int x11_error_occured = 0;

int x_error_handler_bidon(Display *dpy, XErrorEvent *err)
{
  char errmsg[80]; 
  XGetErrorText(dpy, err->error_code, errmsg, 80);
  fprintf(stderr, "X11 error detectee dans une zone à risque:\n  %s\n", errmsg);
  fprintf(stderr, "  Protocol request: %d\n", err->request_code);
  fprintf(stderr, "  Resource ID:      0x%lx\n", err->resourceid);
  fprintf(stderr, " --> THE SHOW MUST GO ON!\n");
  x11_error_occured = 1;
  return 0;
}

/* obligé de gérer les erreurs de manière un peu cavalière, car le root pixmap peut être détruit
   à tout bout de champ etc.. */
Pixmap
extract_root_pixmap_and_shade(const RGBAContext *rc, int x, int y, int w, int h, TransparencyInfo *ti)
{
  Pixmap root_pix, shade_pix;
  XImage *ximg;
  

  if (rc->depth < 15) return None; /* pas de pseudotransp sur les visual non truecolor */

  root_pix = get_rootwin_pixmap(rc);
  if (root_pix == None) return None;

  x11_error_occured = 0;

  /* transparence pure, ça va vite */
  if (ti->type == FULL_TRANSPARENCY) {
    shade_pix = XCreatePixmap(rc->dpy, rc->drawable, 
			      w, h, rc->depth); assert(shade_pix != None);
    XSetErrorHandler(x_error_handler_bidon);
    XCopyArea(rc->dpy, root_pix, shade_pix, rc->copy_gc, 
	      x, y, w, h, 0, 0);
    XSync(rc->dpy,0);
    XSetErrorHandler(NULL); if (x11_error_occured) { XFreePixmap(rc->dpy, shade_pix); shade_pix = None; };
    return shade_pix;
  }

  /* shade/tinte, il faut retravailler l'image */

  x11_error_occured = 0; XSetErrorHandler(x_error_handler_bidon);
  ximg = XGetImage(rc->dpy, root_pix, x, y, w, h, 
		   AllPlanes, ZPixmap); 
  XSetErrorHandler(NULL); if (x11_error_occured || ximg == NULL) return None;

  shade_XImage(rc, ximg, ti);
  shade_pix = XCreatePixmap(rc->dpy, rc->drawable, 
			    w, h, rc->depth); assert(shade_pix != None);
  XPutImage(rc->dpy, shade_pix, rc->copy_gc, ximg, 0, 0, 0, 0, w, h);
  XDestroyImage(ximg);
  return shade_pix;
}

