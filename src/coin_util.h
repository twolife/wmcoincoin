#ifndef COINCOIN_UTIL
#define COINCOIN_UTIL
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include "raster.h"

#ifndef MAX
# define MAX(x,y) ((x)>(y)?(x):(y))
#endif
#ifndef MIN
# define MIN(x,y) ((x)<(y)?(x):(y))
#endif
#ifndef SQR
#  define SQR(a) ((a)*(a))
#endif
#ifndef ABS
#  define ABS(a) ((a)>0?(a):-(a))
#endif
#define ALLOC_OBJ(p, t) { p = (t*) calloc(1, sizeof(t)); assert(p);}
#define FREE_OBJ(p) free(p);
#define ALLOC_VEC(p, n, t) { p = (t*) calloc(n, sizeof(t)); assert(p); }
#define SET_VEC(p, v, i0, i1) { int _i; \
                  for (_i = (i0); _i <= (i1); _i++) p[_i] = v; }
#define FREE_VEC(p) free(p);

/* alloc de tableau, p[0] est le pointeur de base, p[ni] est NULL */
#define ALLOC_ARR(p, ni, nj, t) { int _i; \
               p = (t**) calloc(ni+1, sizeof(t*)); assert(p); \
               p[0] = (t*) calloc((ni)*(nj), sizeof(t)); assert(p[0]); \
               for (_i = 1; _i < ni; _i++) { p[_i] = p[0]+((nj)*_i); } p[ni] = (t*)NULL;}
#define FREE_ARR(p) { free(p[0]); free(p); }
#define BASE_ARR(p) (p[0])

#ifndef NO_BLAHBLAH
# define BLAHBLAH(n,x) if (Prefs.verbosity >= n) { x; fflush(stdout); }
#else
# define BLAHBLAH(n,x)
#endif

#define MARK myprintf("%<YEL ******************************************>\n");

/* deux macros fort pratiques !! */
#define RGB2PIXEL(r,g,b) (dock->rgba_context->rtable[r] + \
                          dock->rgba_context->gtable[g] + \
                          dock->rgba_context->btable[b])
#define IRGB2PIXEL(rgb) (dock->rgba_context->rtable[(rgb>>16) & 0xff] + \
                         dock->rgba_context->gtable[(rgb>> 8) & 0xff] + \
                         dock->rgba_context->btable[(rgb    ) & 0xff])

RGBAImage * rimage_create_from_raw(int w, int h, int bpp, const unsigned char *data);
Pixmap pixmap_create_from_raw(RGBAContext *rc, int w, int h, int bpp, const unsigned char *data);
RGBAImage * rimage_create_from_raw_with_tint(int w, int h, int bpp, const unsigned char *data, int rgb);
Pixmap pixmap_create_from_raw_with_tint(RGBAContext *rc, int w, int h, int bpp, const unsigned char *data, int rgb);
void alpha2mask(int w, int h, unsigned char *bitmap, const unsigned char *rgba, unsigned char mask_val);
Pixmap alpha2pixmap_mask(Display *dpy, Window win, int w, int h, const unsigned char *rgba, unsigned char mask_val);
void colorize_black_pixmap(char **pixtxt, int tint);
/* construit un 'nom' à partir des premiers mots du useragent */
void make_short_name_from_ua(const unsigned char *ua, unsigned char *name, int name_sz);
void get_window_pos_with_decor(Display *display, Window win, int *screen_x, int *screen_y);
int convert_to_ascii(char *dest, const char *src, int dest_sz, int with_bug_amp);
char *str_substitute(const char *src, const char *key, const char *substitution);
char *shell_quote(const char *src);
#endif
