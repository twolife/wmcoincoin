/*
  rcsid=$Id: raster.c,v 1.3 2002/03/10 16:07:10 pouaite Exp $
  ChangeLog:
  $Log: raster.c,v $
  Revision 1.3  2002/03/10 16:07:10  pouaite
  pseudo transp basique dans le pinnipede (en cours..)

  Revision 1.2  2001/12/02 18:34:54  pouaite
  ajout de tags cvs Id et Log un peu partout...

*/

#include "raster.h"
#include "coin_util.h"
/* 
   ces fonctions toutes nazes remplacent dorévanant la libwraster 

   le code est très simple parce qu'on envoie chier tout ce qui n'est pas truecolor
   et que la performance n'est pas un souci (cf les XPutPixel dans RGBAImage2Ximage ... 
   -> ceci dit, c'est pareil dans la libwraster: grosse déception...)

   le code reste néanmoins très inspiré par la libwraster

   les images sont stockées dans des tableaux, dans l'ordre (attention !!):
   [ligne][colonne] -> c.a.d. (y,x)
*/


/* transforme une valeur 0<=c<=255 en 
   une quantité suffisament décalée pour loger dans le mask
*/
static unsigned long 
foobarize(unsigned long c, unsigned long mask)
{
  int dec, len;
  
  dec = 0; /* decalage du masque */
  while ((mask & 1)==0) {
    dec++; mask >>= 1;
  }
  len = 0; /* nb de bits à 1 */
  while ((mask & 1)==1) {
    len++; mask >>= 1;
  }
  if (len < 8) {
    c >>= (8-len);
  } else if (len > 8) {
    c <<= (len-8);
  }
  c <<= dec;
  return c;
}

RGBAContext*
RGBACreateContext(Display *dpy, int screen_number)
{
  RGBAContext *context;
  XGCValues gcv;
  int i;

  ALLOC_OBJ(context, RGBAContext);
  
  context->dpy = dpy;
  
  context->screen_number = screen_number;
  
  context->visual = DefaultVisual(dpy, screen_number);
  context->depth = DefaultDepth(dpy, screen_number);
  context->cmap = DefaultColormap(dpy, screen_number);
  context->drawable = RootWindow(dpy, screen_number);
  context->black = BlackPixel(dpy, screen_number);
  context->white = WhitePixel(dpy, screen_number);
  context->vclass = context->visual->class;
    
  gcv.function = GXcopy;
  gcv.graphics_exposures = False;
  context->copy_gc = XCreateGC(dpy, context->drawable, GCFunction
			       |GCGraphicsExposures, &gcv);
  if (context->vclass == TrueColor || context->vclass == DirectColor) {
    /* calc offsets to create a TrueColor pixel */
    printf("le visual (depth=%d) est en %s, cool\n", context->depth, 
	   context->vclass == TrueColor ? "TrueColor" : "DirectColor");
  } else if (context->vclass == PseudoColor || context->vclass == StaticColor) {
    printf("waou, on est en pseudocolor (depth=%d)... dommage\n", context->depth);
    free(context); return NULL;
  } else if (context->vclass == GrayScale || context->vclass == StaticGray) {
    printf("incoryable, y'a même po de couleurs !\n");
    free(context); return NULL;
  } else return NULL;
  
  for (i=0; i < 256; i++) {
    context->rtable[i] = foobarize(i, context->visual->red_mask);
    context->gtable[i] = foobarize(i, context->visual->green_mask);
    context->btable[i] = foobarize(i, context->visual->blue_mask);
    //    printf("i=%02x: %lx %lx %lx\n", i, context->rtable[i], context->gtable[i], context->btable[i]);
  }

  {
    unsigned long mask;
    int nbits;

    context->rdecal = context->gdecal = context->bdecal = 0;
    mask = context->visual->red_mask; nbits = 0;
    while ((mask & 1) == 0) { context->rdecal++; mask >>= 1; }
    while ((mask & 1) == 1) { nbits++; mask >>= 1; }
    printf("rmask=%08lx, decal=%d, nbits=%d\n", 
	   context->visual->red_mask, context->rdecal, nbits);
    context->rdecal += (nbits-8);

    mask = context->visual->green_mask; nbits = 0;
    while ((mask & 1) == 0) { context->gdecal++; mask >>= 1; }
    while ((mask & 1) == 1) { nbits++; mask >>= 1; }
    printf("gmask=%08lx, decal=%d, nbits=%d\n", 
	   context->visual->green_mask, context->gdecal, nbits);
    context->gdecal += (nbits-8);

    mask = context->visual->blue_mask; nbits = 0;
    while ((mask & 1) == 0) { context->bdecal++; mask >>= 1; }
    while ((mask & 1) == 1) { nbits++; mask >>= 1; }
    printf("bmask=%08lx, decal=%d, nbits=%d\n", 
	   context->visual->blue_mask, context->bdecal, nbits);
    context->bdecal += (nbits-8);
  }
  return context;
}

RGBAImage*
RGBACreateImage(int width, int height)
{
  RGBAImage *image=NULL;
    
  assert(width>0 && height>0);

  image = malloc(sizeof(RGBAImage));
  if (!image) {
    return NULL;
  }

  image->w = width;
  image->h = height;
  assert(sizeof(RGBAPixel) == 4); /* je compte la dessus .. */
  ALLOC_ARR(image->data, height, width, RGBAPixel);
  return image;
}

void
RGBADestroyImage(RGBAImage *img)
{
  assert(img);
  free(img->data[0]); free(img->data); free(img);
}

XImage *
RGBAImage2XImage(RGBAContext *ctx, RGBAImage *rimg)
{
  XImage *ximg;
  int x,y;

  ximg = XCreateImage(ctx->dpy, ctx->visual, ctx->depth,
		      ZPixmap, 0, NULL, rimg->w, rimg->h, 8, 0);
  if (ximg == NULL) {
    return NULL;
  }
  ximg->data = malloc(ximg->bytes_per_line * rimg->h);
  if (ximg->data == NULL) {
    XDestroyImage(ximg); return NULL;
  }
  
  for (x=0; x < rimg->w; x++) {
    for (y=0; y < rimg->h; y++) {
      unsigned long pix;

      //      printf("x=%04d, y=%04d, rgb=%02x%02x%02x, %08lx %08lx\n", x,y,
      //	     rimg->data[y][x].rgba[0],rimg->data[y][x].rgba[1],rimg->data[y][x].rgba[2],
      //	     pix, ctx->white);
      pix = (ctx->rtable[rimg->data[y][x].rgba[0]] + 
	     ctx->gtable[rimg->data[y][x].rgba[1]] + 
	     ctx->btable[rimg->data[y][x].rgba[2]]);
      XPutPixel(ximg, x, y, pix);//ctx->white);
    }
  }
  return ximg;
}

Pixmap
RGBAImage2Pixmap(RGBAContext *ctx, RGBAImage *rimg)
{
  XImage *ximg;
  Pixmap pixmap;

  assert(ctx);
  assert(rimg);
  ximg = RGBAImage2XImage(ctx, rimg);
  if (ximg == NULL) return None;
  pixmap = XCreatePixmap(ctx->dpy, ctx->drawable, rimg->w,
			 rimg->h, ctx->depth);
  XPutImage(ctx->dpy, pixmap, ctx->copy_gc, ximg, 0, 0, 0, 0, rimg->w, rimg->h);
  XDestroyImage(ximg);
  return pixmap;
}
