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
#endif
