#ifndef COIN_XUTIL_H
#define COIN_XUTIL_H

#include "raster.h"
#include "coin_util.h"
#include "prefs.h"

/* deux macros fort pratiques !! */
#define RGB2PIXEL(r,g,b) _RGB2PIXEL(dock->rgba_context,r,g,b)
#define IRGB2PIXEL(rgb) _IRGB2PIXEL(dock->rgba_context,rgb)

#define PIXEL2R(rc, pixel) (((pixel>>rc->r_shift_left)<<rc->r_shift_right)&0xff)
#define PIXEL2G(rc, pixel) (((pixel>>rc->g_shift_left)<<rc->g_shift_right)&0xff)
#define PIXEL2B(rc, pixel) (((pixel>>rc->b_shift_left)<<rc->b_shift_right)&0xff)

RGBAImage * rimage_create_from_raw(int w, int h, int bpp, const unsigned char *data);
Pixmap pixmap_create_from_raw(RGBAContext *rc, int w, int h, int bpp, const unsigned char *data);
RGBAImage * rimage_create_from_raw_with_tint(int w, int h, int bpp, const unsigned char *data, int rgb);
Pixmap pixmap_create_from_raw_with_tint(RGBAContext *rc, int w, int h, int bpp, const unsigned char *data, int rgb);
void alpha2mask(int w, int h, unsigned char *bitmap, const unsigned char *rgba, unsigned char mask_val);
Pixmap alpha2pixmap_mask(Display *dpy, Window win, int w, int h, const unsigned char *rgba, unsigned char mask_val);
void colorize_black_pixmap(char **pixtxt, int tint);
void get_window_pos_with_decor(Display *display, Window win, int *screen_x, int *screen_y);
void get_window_pos_without_decor(Display *display, Window root_win, Window win, int *screen_x, int *screen_y);
Pixmap get_rootwin_pixmap(const RGBAContext *rc);
void shade_XImage(const RGBAContext *rc, XImage *ximg, TransparencyInfo *tr);
Pixmap extract_root_pixmap_and_shade(const RGBAContext *rc, int x, int y, int w, int h, TransparencyInfo *tr, int use_fake_real_transparency);
#endif
