#ifndef COIN_XUTIL_H
#define COIN_XUTIL_H

#include "raster.h"
#include "coin_util.h"

/* deux macros fort pratiques !! */
#define RGB2PIXEL(r,g,b) (dock->rgba_context->rtable[r] + \
                          dock->rgba_context->gtable[g] + \
                          dock->rgba_context->btable[b])
#define IRGB2PIXEL(rgb) (dock->rgba_context->rtable[(rgb>>16) & 0xff] + \
                         dock->rgba_context->gtable[(rgb>> 8) & 0xff] + \
                         dock->rgba_context->btable[(rgb    ) & 0xff])

#define _RGB2PIXEL(c,r,g,b) (c->rtable[r] + \
                             c->gtable[g] + \
                             c->btable[b])
#define _IRGB2PIXEL(c,rgb) (c->rtable[(rgb>>16) & 0xff] + \
                            c->gtable[(rgb>> 8) & 0xff] + \
                            c->btable[(rgb    ) & 0xff])

RGBAImage * rimage_create_from_raw(int w, int h, int bpp, const unsigned char *data);
Pixmap pixmap_create_from_raw(RGBAContext *rc, int w, int h, int bpp, const unsigned char *data);
RGBAImage * rimage_create_from_raw_with_tint(int w, int h, int bpp, const unsigned char *data, int rgb);
Pixmap pixmap_create_from_raw_with_tint(RGBAContext *rc, int w, int h, int bpp, const unsigned char *data, int rgb);
void alpha2mask(int w, int h, unsigned char *bitmap, const unsigned char *rgba, unsigned char mask_val);
Pixmap alpha2pixmap_mask(Display *dpy, Window win, int w, int h, const unsigned char *rgba, unsigned char mask_val);
void colorize_black_pixmap(char **pixtxt, int tint);
void get_window_pos_with_decor(Display *display, Window win, int *screen_x, int *screen_y);
#endif
