/*
  coin_xutil : diverses fonctions complémentaires à raster.c pour la manip des images

  rcsid=$Id: coin_xutil.c,v 1.1 2002/04/01 01:39:38 pouaite Exp $

  ChangeLog:
  $Log: coin_xutil.c,v $
  Revision 1.1  2002/04/01 01:39:38  pouaite
  grosse grosse commition (cf changelog)

*/

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
