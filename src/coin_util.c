/*
  rcsid=$Id: coin_util.c,v 1.2 2001/12/02 18:29:46 pouaite Exp $
  ChangeLog:
  $Log: coin_util.c,v $
  Revision 1.2  2001/12/02 18:29:46  pouaite
  à la demande des décideurs de tous poils, gestion (toute naze...) de l'EURO !

*/

#include <string.h>
#include <ctype.h>
#include "coin_util.h"
#include "raster.h"



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

/* construit un 'nom' à partir des premiers mots du useragent */
void
make_short_name_from_ua(const unsigned char *ua, unsigned char *name, int name_sz)
{
  if (ua == NULL || strlen(ua) == 0) {
    strncpy(name, "[vide]", name_sz); name[name_sz-1] = 0;
    return;
  }
  strncpy(name, ua, name_sz); name[name_sz-1] = 0;
  if ((int)strlen(name) >= name_sz-1) {
    int i;
    /* essaye de ramener l'useragent à une longueur correcte sans tronquer de mot */
    i = name_sz-2;
    while (i && isalnum((unsigned char)name[i])) {
      i--;
    }
    while (i>1 && !isalnum((unsigned char)name[i-1])) {
      i--;
    }
    
    if (i > 5) {
      name[i] = 0;
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
    XFree(child_windows);
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


/*
  with_bug_amp : le remote.rdf a été deux fois interprete par une moulinette,
   resultat , les 'é' soit devenus des '&eacute;' puis des '&amp;eacute;'

   remarque: on peut avoir dest == src, on utilise alors une chaine temporaire

   renvoie la longueur de la chaine apres conversion

   cette fonction est utilisee par picohtml.c et coincoin_tribune.c
*/
int
convert_to_ascii(char *dest, const char *_src, int dest_sz, int with_bug_amp)
{
  int id, is;

  const char *src;
  static const struct {
    char *sign;
    char *c;
  } tab[] = {{"amp;", "&"},
	     {"quot;", "\"",},
	     {"gt;",">",},
	     {"lt;","<",},
	     {"acute;", "\'"},

	     {"nbsp;"  , " "},  // 0xa0 /* ouais bon, du coup il va etre breakable l'espace.. */
	     {"iexcl;" , "¡"},
	     {"cent;"  , "¢"},
             {"pound;" , "£"},
             {"curren;", "¤"},  // 0xa4
	     {"yen;"   , "¥"},
	     {"brvbar;", "¦"},
	     {"sect;"  , "§"},
             {"uml;"   , "¨"},
	     {"copy;"  , "©"},
	     {"ordf;"  , "ª"},
	     {"laquo;" , "«"},
	     {"not;"   , "¬"},
	     //	     {"shy;"   , "????"}, // 0xad soft hyphen ... non visible ?

	     {"reg;"   , "®"},
	     {"macr;"  , "¯"},
	     {"deg;"   , "°"}, // 0xb0
	     {"plusnm;", "±"},
	     {"sup2;"  , "²"},
	     {"sup3;"  , "³"},
	     {"acute;" , "´"},
	     {"micro;" , "µ"},
	     {"para;"  , "¶"},
	     {"middot;", "·"},
             {"cedil;" , "¸"},
	     {"sup1;"  , "¹"},
	     {"ordm;"  , "º"},
	     {"raquo;", "»"},

	     {"frac14;", "¼"},
	     {"frac12;", "½"},
	     {"frac34;", "¾"},
	     {"iquest;", "¿"},

	     {"Agrave;", "À"}, // 0xc0
	     {"Aacute;", "Á"},
	     {"Acirc;" , "Â"},
	     {"Atilde;", "Ã"},
	     {"Auml;"  , "Ä"},
	     {"Aring;" , "Å"},
	     {"AElig;" , "Æ"},
	     {"Ccedil;", "Ç"},
	     {"Egrave;", "È"},
	     {"Eacute;", "É"},
	     {"Ecirc;" , "Ê"},
	     {"Euml;"  , "Ë"}, 
	     {"Igrave;", "Ì"},
	     {"Iacute;", "Í"},
	     {"Icirc;" , "Î"},
	     {"Iuml;"  , "Ï"},
	     {"ETH;"   , "Ð"}, // 0xd0
	     {"Ntilde;", "Ñ"},
	     {"Ograve;", "Ò"},
	     {"Oacute;", "Ó"},
	     {"Ocirc;" , "Ô"},
	     {"Otilde;", "Õ"},
	     {"Ouml;"  , "Ö"},
	     {"times;" , "×"},
	     {"Oslash;", "Ø"},
	     {"Ugrave;", "Ù"},
	     {"Uacute;", "Ú"},
	     {"Ucirc;" , "Û"},
	     {"Uuml;"  , "Ü"},
	     {"Yacute;", "Ý"},
	     {"THORN;" , "Þ"},
	     {"szlig;" , "ß"},
	     {"agrave;", "à"}, // 0xe0
	     {"aacute;", "á"},
	     {"acirc;" , "â"},
	     {"atilde;", "ã"},
	     {"auml;"  , "ä"},
	     {"aring;" , "å"},
	     {"aelig;" , "æ"},
	     {"ccedil;", "ç"},
	     {"egrave;", "è"},
	     {"eacute;", "é"},
	     {"ecirc;" , "ê"},
	     {"euml;"  , "ë"},
	     {"igrave;", "ì"},
	     {"iacute;", "í"},
	     {"icirc;" , "î"},
	     {"iuml;"  , "ï"},
	     {"eth;"   , "ð"}, // 0xf0
	     {"ntilde;", "ñ"},
	     {"ograve;", "ò"},
	     {"oacute;", "ó"},
	     {"ocirc;" , "ô"},
	     {"otilde;", "õ"},
	     {"ouml;"  , "ö"},

	     {"oslash;", "ø"},
	     {"uacute;", "ú"},
	     {"ugrave;", "ù"},
	     {"ucirc;" , "û"},
	     {"uuml;"  , "ü"}, 
	     {"yacute;", "ý"}, // 0xfd
	     {"thorn;" , "þ"}, // 0xfe
	     {"yuml;"  , "ÿ"}, // 0xff
	     //   {"Scaron;",{352,0}},
	     //	     {"scaron;",{353,0}},
	     {"trade;", "(tm)"}, // non iso8859-1
	     {"euro;", "EUR"},   // non iso8859-1
	     {"OElig;" , "OE"},
	     {"oelig;" , "oe"},
	     {NULL, "*"}};


  /* detection du cas ou les chaines se supperposent */
  if (_src == dest ) {
    src = strdup(_src); assert(src);
  } else {
    src = _src;
  }

  id = 0; is = 0;
  while (id < dest_sz-1 && src[is]) {
    if (src[is] == '&') {
      int i;
      int is_bug;
      int found;

      is_bug = 0;
      /* y'a un bug dans remopte.php, on trouve des trucs du genre '&amp;quot;' au lieu de '&quot' */
      if (with_bug_amp) {
	if (strncmp(src+is, "&amp;", 5) == 0) {
	  is_bug = 1; is += 4;
	}
      }

      i = 0; found = -1;
      while (tab[i].sign) {
	if (strncmp(tab[i].sign, src+is+1, strlen(tab[i].sign))==0) {
	  is += strlen(tab[i].sign)+1;
	  found = i;
	  break;
	}
	i++;
      }
      if (found == -1) {
	dest[id++] = '&'; is++;
      } else {
	int j;
	j = 0;
	while (id < dest_sz-1 && tab[i].c[j]) {
	  dest[id++] = tab[i].c[j++];
	}
      }
    } else if ((unsigned char)src[is] == 0x80 && id < dest_sz-4) { /* cas particulier pour l'odieux EURO (encodage windows) */
      dest[id++] = 'E';
      dest[id++] = 'U';
      dest[id++] = 'R';
      is++;
    } else {
      dest[id] = src[is];
      id++; is++;
    }
  }
  dest[id] = 0;

  if (_src == dest) {
    free((char*)src);
  }
  return id;
}

