/*
  rcsid=$Id: raster.c,v 1.12 2002/05/11 22:00:20 pouaite Exp $
  ChangeLog:
  $Log: raster.c,v $
  Revision 1.12  2002/05/11 22:00:20  pouaite
  bugfix lecture des xpms en niveaux de gris

  Revision 1.11  2002/04/15 19:56:38  pouaite
  v2.3.7a

  Revision 1.10  2002/04/13 11:55:19  pouaite
  fix kde3 + deux trois conneries

  Revision 1.9  2002/04/10 22:53:44  pouaite
  un commit et au lit

  Revision 1.8  2002/04/02 22:29:29  pouaite
  bugfixes transparence

  Revision 1.7  2002/04/01 01:39:38  pouaite
  grosse grosse commition (cf changelog)

  Revision 1.6  2002/03/21 22:53:07  pouaite
  ajout d'une icone pour la fenetre du pinnipede et des news

  Revision 1.5  2002/03/19 09:55:58  pouaite
  bugfixes compilation

  Revision 1.4  2002/03/18 22:46:49  pouaite
  1 ou 2 bugfix mineurs, et surtout suppression de la dependance avec la libXpm .. un premier pas vers wmc� en 8bits

  Revision 1.3  2002/03/10 16:07:10  pouaite
  pseudo transp basique dans le pinnipede (en cours..)

  Revision 1.2  2001/12/02 18:34:54  pouaite
  ajout de tags cvs Id et Log un peu partout...

*/
#include <assert.h>
#include <string.h>
#include <errno.h>
#include "raster.h"
#include "coin_util.h"
/* 
   ces fonctions toutes nazes remplacent dor�vanant la libwraster 

   le code est tr�s simple parce qu'on envoie chier tout ce qui n'est pas truecolor
   et que la performance n'est pas un souci (cf les XPutPixel dans RGBAImage2Ximage ... 
   -> ceci dit, c'est pareil dans la libwraster: grosse d�ception...)

   le code reste n�anmoins tr�s inspir� par la libwraster

   les images sont stock�es dans des tableaux, dans l'ordre (attention !!):
   [ligne][colonne] -> c.a.d. (y,x)
*/


/* transforme une valeur 0<=c<=255 en 
   une quantit� suffisament d�cal�e pour loger dans le mask
*/
static unsigned long 
foobarize(unsigned long c, unsigned long mask)
{
  int dec, len;
  
  dec = 0; /* decalage du masque */
  while ((mask & 1)==0) {
    dec++; mask >>= 1;
  }
  len = 0; /* nb de bits � 1 */
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
    printf("waou, on est en pseudocolor (depth=%d)...\n", context->depth);
    free(context); return NULL;
  } else if (context->vclass == GrayScale || context->vclass == StaticGray) {
    printf("incoryable, y'a m�me po de couleurs !\n");
    free(context); return NULL;
  } else return NULL;
  
  for (i=0; i < 256; i++) {
    context->rtable[i] = foobarize(i, context->visual->red_mask);
    context->gtable[i] = foobarize(i, context->visual->green_mask);
    context->btable[i] = foobarize(i, context->visual->blue_mask);
    /*    printf("i=%02x: %lx %lx %lx\n", i, context->rtable[i], context->gtable[i], context->btable[i]); */
  }

  {
    unsigned long mask;
    int nbits, decal;

    context->r_shift_left = context->g_shift_left = context->b_shift_left = 0;
    context->r_shift_right = context->g_shift_right = context->b_shift_right = 0;

    mask = context->visual->red_mask; nbits = 0; decal = 0;
    while ((mask & 1) == 0) { decal++; mask >>= 1; }
    while ((mask & 1) == 1) { nbits++; mask >>= 1; }
    printf("rmask=%08lx, decal=%d, nbits=%d\n", 
	   context->visual->red_mask, decal, nbits);
    context->r_shift_left = decal + (nbits-8);
    if (decal + nbits - 8 < 0) { 
      context->r_shift_right = -context->r_shift_left;
      context->r_shift_left  = 0;
    }

    mask = context->visual->green_mask; nbits = 0; decal = 0;
    while ((mask & 1) == 0) { decal++; mask >>= 1; }
    while ((mask & 1) == 1) { nbits++; mask >>= 1; }
    printf("gmask=%08lx, decal=%d, nbits=%d\n", 
	   context->visual->green_mask, decal, nbits);
    context->g_shift_left = decal + (nbits-8);
    if (decal + nbits - 8 < 0) { 
      context->g_shift_right = -context->g_shift_left;
      context->g_shift_left  = 0;
    }

    mask = context->visual->blue_mask; nbits = 0; decal = 0;
    while ((mask & 1) == 0) { decal++; mask >>= 1; }
    while ((mask & 1) == 1) { nbits++; mask >>= 1; }
    printf("bmask=%08lx, decal=%d, nbits=%d\n", 
	   context->visual->blue_mask, decal, nbits);
    context->b_shift_left = decal + (nbits-8);
    if (decal + nbits - 8 < 0) { 
      context->b_shift_right = -context->b_shift_left;
      context->b_shift_left  = 0;
    }
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
  ximg->data = calloc(ximg->bytes_per_line * rimg->h, 1);
  if (ximg->data == NULL) {
    XDestroyImage(ximg); return NULL;
  }
  
  for (x=0; x < rimg->w; x++) {
    for (y=0; y < rimg->h; y++) {
      unsigned long pix;

      /*      printf("x=%04d, y=%04d, rgb=%02x%02x%02x, %08lx %08lx\n", x,y,
	      rimg->data[y][x].rgba[0],rimg->data[y][x].rgba[1],rimg->data[y][x].rgba[2],
	      pix, ctx->white); */
      pix = (ctx->rtable[rimg->data[y][x].rgba[0]] + 
	     ctx->gtable[rimg->data[y][x].rgba[1]] + 
	     ctx->btable[rimg->data[y][x].rgba[2]]);
      XPutPixel(ximg, x, y, pix);
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

/*
  dans la cat�gorie des fonctions � ne pas appeler trop souvent, 
  ni sur des wallpaper 1600x1200 , voici un clone de XpmCreatePixmapFromData ... 

  Update: coup�e en deux
*/
RGBAImage *
RGBACreateRImgFromXpmData(RGBAContext *rc, char **xpm)
{
  int w,h,ncolor,cpp;
  RGBAImage *rimg;

  struct xpm_color {
    char char_color[4];
    int r,g,b,a;
  } *col_tab;
  
  int i, rgb;
  int err = 0;

  assert(sscanf(xpm[0], "%d %d %d %d", &w, &h, &ncolor, &cpp)==4);
  assert(w > 0); assert(h>0); assert(ncolor>0); assert(cpp>0 && cpp <= 4);
  
  ALLOC_VEC(col_tab, ncolor, struct xpm_color);
  for (i=0; i < ncolor; i++) {
    char *s;
    s = xpm[i+1];
    strncpy(col_tab[i].char_color, s, cpp); /* copie les caracteres identifiant la couleur */

    s+= cpp;
    if (!(*s == '\t' || *s == ' ')) { err = 1; goto ralala; }
    s++;
    if (*s != 'c' && *s != 'g') {err = 2; goto ralala;} /* seulement les fichiers xpms en couleur ou en niveau de gris (en gris les couleurs sont qd m�me sous la forme #rrggbb, avec rr=gg=bb)  */

    s++; if (!(*s == ' ')) { err=3; goto ralala; }
    s++;
    if (strcasecmp(s, "None") != 0) {
      if (*s != '#') {
	XColor xc;
	if (XParseColor(rc->dpy, DefaultColormap(rc->dpy, rc->screen_number), s, &xc)) {
	  fprintf(stderr, "couleur inconnue dans le .xpm : '%s'\n'", s);
	  exit(0);
	}
	rgb = ((xc.red>>8) << 16) + ((xc.green>>8) << 8) + (xc.blue>>8);
      } else {
	if (!(sscanf(s+1, "%x", &rgb)==1)) {err = 4; goto ralala; }
      }
      col_tab[i].r = (rgb & 0xff0000) >> 16;
      col_tab[i].g = (rgb & 0x00ff00) >> 8;
      col_tab[i].b = (rgb & 0x0000ff);
      col_tab[i].a = 255;
      
      /*printf("couleur %d: %x [s='%s']\n", i, rgb, xpm[i+1]);*/
    } else {
      col_tab[i].r = 0;
      col_tab[i].g = 0;
      col_tab[i].b = 0;
      col_tab[i].a = 0;
      
    }
  }

  rimg = RGBACreateImage(w,h); assert(rimg);
  for (i=0; i < h; i++) {
    char *s; 
    int j;
    s = xpm[i+1+ncolor];
    for (j=0; j < w; j++) {
      int k;

      /* recherche ultra-bourrine de la couleur, pas le temps de finasser ;) */
      for (k = 0; k < ncolor; k++) {
	if (memcmp(s+j*cpp, col_tab[k].char_color, cpp) == 0) {
	  break;
	}
      }
      assert(k < ncolor);
      
      /*   printf("%d %d --> [%02x,%02x,%02x]\n", i, j, col_tab[k].r, col_tab[k].g, col_tab[k].b);*/
      rimg->data[i][j].rgba[0] = col_tab[k].r;
      rimg->data[i][j].rgba[1] = col_tab[k].g;
      rimg->data[i][j].rgba[2] = col_tab[k].b;
      rimg->data[i][j].rgba[3] = col_tab[k].a;
    }
  }

  free(col_tab);  
  return rimg;
 ralala:
  fprintf(stderr, "dommage, vous avez trouv� une pouille dans le 'parseur' de .xpm\n VOTRE fichier .xpm n'est pas conforme � MON standard xpm\nerr=%d, la ligne qui pose probl�me est: '%s'\n En le r��crivant avec un autre logiciel �a devrait mieux marcher, mais le mieux c'est sans doute d'envoyer un bug report [ c0in@altern.org ]\n", err, xpm[i+1]);
  exit(1);
  return NULL;
}

Pixmap
RGBACreatePixmapFromXpmData(RGBAContext *ctx, char **xpm)
{
  RGBAImage *rimg;
  Pixmap pix;

  rimg = RGBACreateRImgFromXpmData(ctx, xpm);
  pix = RGBAImage2Pixmap(ctx, rimg);
  RGBADestroyImage(rimg);
  return pix;
}

/* toujours plus con , voici...*/
Pixmap
RGBACreatePixmapFromXpmFile(RGBAContext *ctx, char *xpm_file, int *w, int *h)
{
  FILE *f;
#define LEN_MAX 4096
#define NLIG_MAX 2048

  char *l_tab[NLIG_MAX]; /* beurk */
  char l[LEN_MAX];       /* eeeerk */
  int lcnt;
  Pixmap pix;

  *w = 0; *h = 0;
  f = fopen(xpm_file, "r"); if (f == NULL) {
    fprintf(stderr, "impossible d'ouvrir '%s' : %s\n", xpm_file, strerror(errno));
    return None;
  }
  
  lcnt = 0;
  do {
    if (ferror(f)) {
      fprintf(stderr, "erreur pendant la lecture de '%s' !? [%s]\n", xpm_file, strerror(errno));
    }

    l[0] = 0; fgets(l, LEN_MAX, f);
    
    /* on ne prend en compte QUE LES LIGNES QUI COMMENCE PAR '"' et QUI SE TERMINENT PAR '",' */
    
    if (l[0] == '"') {
      int i;
      i = strlen(l); assert(i > 2);
      
      i--;
      while (l[i] != '"') i--; /* oui, j'ai pas envie de finasser */
      if (i == 0) {
	fprintf(stderr, "la ligne '%s' est bizarre\n", l); return None;
      }
      l[i] = 0;

      /*      printf("ajout de la ligne: '%s'\n", l+1); */
      l_tab[lcnt] = strdup(l+1);
      lcnt++;
    }
  } while (!feof(f));
  fclose(f);
  
  assert(lcnt > 0);
  sscanf(l_tab[0], "%d %d", w, h);
  pix = RGBACreatePixmapFromXpmData(ctx, l_tab);

  while (lcnt > 0) {
    lcnt--;
    free(l_tab[lcnt]);
  }
  return pix;
}
