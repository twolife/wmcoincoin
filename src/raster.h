/*
  rcsid=$Id: raster.h,v 1.4 2002/03/18 22:46:49 pouaite Exp $
  ChangeLog:
  $Log: raster.h,v $
  Revision 1.4  2002/03/18 22:46:49  pouaite
  1 ou 2 bugfix mineurs, et surtout suppression de la dependance avec la libXpm .. un premier pas vers wmc² en 8bits

  Revision 1.3  2002/03/10 16:07:10  pouaite
  pseudo transp basique dans le pinnipede (en cours..)

  Revision 1.2  2001/12/02 18:34:54  pouaite
  ajout de tags cvs Id et Log un peu partout...

*/

#ifndef RASTER_H__
#define RASTER_H__

#include <X11/X.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>

typedef struct RGBAContext {
  Display *dpy;
  int screen_number;
  Colormap cmap;
  
  GC copy_gc;
  
  Visual *visual;
  int depth;
  Window drawable;		       /* window to pass for XCreatePixmap().*/
				       /* generally = root */
  int vclass;
  
  unsigned long black;
  unsigned long white;

  unsigned long rtable[256], gtable[256], btable[256];

  /* decalage à appliquer sur un pixel (apres l'avoir masque avec visual->red_mask etc) pour se retrouver avec une valeur entre 0 et 255 */
  int rdecal, gdecal, bdecal;
  int rmask, gmask, bmask;
} RGBAContext;


typedef union RGBAPixel_ {
  unsigned char rgba[4];
  unsigned int v;
} RGBAPixel;

typedef struct {
  int w, h;
  RGBAPixel **data;
} RGBAImage;

RGBAContext* RGBACreateContext(Display *dpy, int screen_number);
RGBAImage* RGBACreateImage(int width, int height);
void RGBADestroyImage(RGBAImage *img);
XImage *RGBAImage2XImage(RGBAContext *ctx, RGBAImage *rimg);
Pixmap RGBAImage2Pixmap(RGBAContext *ctx, RGBAImage *rimg);
Pixmap RGBACreatePixmapFromXpmData(RGBAContext *ctx, char **xpm);
Pixmap RGBACreatePixmapFromXpmFile(RGBAContext *ctx, char *xpm_file, int *w, int *h);
#endif
