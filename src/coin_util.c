/*
  rcsid=$Id: coin_util.c,v 1.14 2002/03/21 22:53:07 pouaite Exp $
  ChangeLog:
  $Log: coin_util.c,v $
  Revision 1.14  2002/03/21 22:53:07  pouaite
  ajout d'une icone pour la fenetre du pinnipede et des news

  Revision 1.13  2002/03/03 10:10:04  pouaite
  bugfixes divers et variÈs

  Revision 1.12  2002/03/01 00:27:40  pouaite
  trois fois rien

  Revision 1.11  2002/02/27 00:32:19  pouaite
  modifs velues

  Revision 1.10  2002/02/24 22:13:56  pouaite
  modifs pour la v2.3.5 (selection, scrollcoin, plopification, bugfixes)

  Revision 1.9  2002/01/16 21:27:35  pouaite
  gros coup de balai dans wmcoincoin.c qui s'est du coup splittÈ en trois: wmcoincoin.c, dock.c et useragents_file.c

  Revision 1.8  2002/01/16 00:35:26  pouaite
  debut de detection des reponse ‡ nos message avec des couleurs hideuses et certainement plein de bugs moisis

  Revision 1.7  2002/01/14 23:54:06  pouaite
  reconnaissance des posts effectuÈ par l'utilisateur du canard (‡ suivre...)

  Revision 1.6  2002/01/13 20:02:51  pouaite
  j'ai honte

  Revision 1.5  2002/01/13 15:19:00  pouaite
  double patch: shift -> tribune.post_cmd et lordOric -> tribune.archive

  Revision 1.4  2002/01/12 17:29:08  pouaite
  support de l'iso8859-15 (euro..)

  Revision 1.3  2001/12/21 12:21:23  pouaite
  pico bugfix

  Revision 1.2  2001/12/02 18:29:46  pouaite
  ‡ la demande des dÈcideurs de tous poils, gestion (toute naze...) de l'EURO !

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

  /* c'est equivalent ‡ */
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

/* sert ‡ colorier le pixmap des lettres -> le noir devient la color 'tint' 

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

/* construit un 'nom' ‡ partir des premiers mots du useragent */
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
    /* essaye de ramener l'useragent ‡ une longueur correcte sans tronquer de mot */
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

/*
  with_bug_amp : le remote.rdf a ÈtÈ deux fois interprete par une moulinette,
   resultat , les 'È' soit devenus des '&eacute;' puis des '&amp;eacute;'

   remarque: on peut avoir dest == src, on utilise alors une chaine temporaire

   renvoie la longueur de la chaine apres conversion

   cette fonction est utilisee par picohtml.c et coincoin_tribune.c
*/
int
convert_to_ascii(char *dest, const char *_src, int dest_sz, int with_bug_amp, int special_encode_ltgt)
{
  int id, is;

  const char *src;
  static const struct {
    char *sign;
    char *c;
  } tab[] = {{"lt;", "\t<"}, /* les deux premier sont utilisÈs si 'special_encode_lt_gt' non nul */
	     {"gt;", "\t>"},
	     {"amp;", "&"},
	     {"quot;", "\"",},
	     {"gt;",">",},
	     {"lt;","<",},
	     {"acute;", "\'"},

	     {"nbsp;"  , " "},  // 0xa0 /* ouais bon, du coup il va etre breakable l'espace.. */
	     {"iexcl;" , "°"},
	     {"cent;"  , "¢"},
             {"pound;" , "£"},
             {"curren;", "§"},  // 0xa4
	     {"yen;"   , "•"},
	     {"brvbar;", "¶"},
	     {"sect;"  , "ß"},
             {"uml;"   , "®"},
	     {"copy;"  , "©"},
	     {"ordf;"  , "™"},
	     {"laquo;" , "´"},
	     {"not;"   , "¨"},
	     {"shy;"   , "≠"},
	     {"reg;"   , "Æ"},
	     {"macr;"  , "Ø"},
	     {"deg;"   , "∞"}, // 0xb0
	     {"plusmn;", "±"},
	     {"sup2;"  , "≤"},
	     {"sup3;"  , "≥"},
	     {"acute;" , "¥"},
	     {"micro;" , "µ"},
	     {"para;"  , "∂"},
	     {"middot;", "∑"},
             {"cedil;" , "∏"},
	     {"sup1;"  , "π"},
	     {"ordm;"  , "∫"},
	     {"raquo;", "ª"},

	     {"frac14;", "º"},
	     {"frac12;", "Ω"},
	     {"frac34;", "æ"},
	     {"iquest;", "ø"},

	     {"Agrave;", "¿"}, // 0xc0
	     {"Aacute;", "¡"},
	     {"Acirc;" , "¬"},
	     {"Atilde;", "√"},
	     {"Auml;"  , "ƒ"},
	     {"Aring;" , "≈"},
	     {"AElig;" , "∆"},
	     {"Ccedil;", "«"},
	     {"Egrave;", "»"},
	     {"Eacute;", "…"},
	     {"Ecirc;" , " "},
	     {"Euml;"  , "À"}, 
	     {"Igrave;", "Ã"},
	     {"Iacute;", "Õ"},
	     {"Icirc;" , "Œ"},
	     {"Iuml;"  , "œ"},
	     {"ETH;"   , "–"}, // 0xd0
	     {"Ntilde;", "—"},
	     {"Ograve;", "“"},
	     {"Oacute;", "”"},
	     {"Ocirc;" , "‘"},
	     {"Otilde;", "’"},
	     {"Ouml;"  , "÷"},
	     {"times;" , "◊"},
	     {"Oslash;", "ÿ"},
	     {"Ugrave;", "Ÿ"},
	     {"Uacute;", "⁄"},
	     {"Ucirc;" , "€"},
	     {"Uuml;"  , "‹"},
	     {"Yacute;", "›"},
	     {"THORN;" , "ﬁ"},
	     {"szlig;" , "ﬂ"},
	     {"agrave;", "‡"}, // 0xe0
	     {"aacute;", "·"},
	     {"acirc;" , "‚"},
	     {"atilde;", "„"},
	     {"auml;"  , "‰"},
	     {"aring;" , "Â"},
	     {"aelig;" , "Ê"},
	     {"ccedil;", "Á"},
	     {"egrave;", "Ë"},
	     {"eacute;", "È"},
	     {"ecirc;" , "Í"},
	     {"euml;"  , "Î"},
	     {"igrave;", "Ï"},
	     {"iacute;", "Ì"},
	     {"icirc;" , "Ó"},
	     {"iuml;"  , "Ô"},
	     {"eth;"   , ""}, // 0xf0
	     {"ntilde;", "Ò"},
	     {"ograve;", "Ú"},
	     {"oacute;", "Û"},
	     {"ocirc;" , "Ù"},
	     {"otilde;", "ı"},
	     {"ouml;"  , "ˆ"},
	     {"divide;", "˜"},
	     {"oslash;", "¯"},
	     {"uacute;", "˙"},
	     {"ugrave;", "˘"},
	     {"ucirc;" , "˚"},
	     {"uuml;"  , "¸"}, 
	     {"yacute;", "˝"}, // 0xfd
	     {"thorn;" , "˛"}, // 0xfe
	     {"yuml;"  , "ˇ"}, // 0xff
	     //   {"Scaron;",{352,0}},
	     //	     {"scaron;",{353,0}},
	     {"trade;", "(tm)"}, // non iso8859-1
	     {"euro;", "§"},   // il faut iso8859-15 pour que Áa fasse le bon char
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
      /* y'a un bug dans remopte.php, on trouve des trucs du genre '&amp;quot;' au lieu de '&quot' 
	 en fait c'est pas un bug, mais je gËre Áa comme un puerco
       */
      if (with_bug_amp) {
	if (strncmp(src+is, "&amp;", 5) == 0) {
	  is_bug = 1; is += 4;
	}
      }

      i = (special_encode_ltgt && is_bug == 0) ? 0 : 2; found = -1;
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
      /*    } else if ((unsigned char)src[is] == 0x80 && id < dest_sz-4) { // cas particulier pour l'odieux EURO (encodage windows) 
      dest[id++] = 'E';
      dest[id++] = 'U';
      dest[id++] = 'R';
      is++;*/
    } else if ((unsigned char)src[is] == 0x80 && id < dest_sz-2) { // cas particulier pour l'odieux EURO (encodage windows) 
      dest[id++] = '§';
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

/*
  verifie si la chaine est vide (cad si elle ne contient que des caractËres non imprimables 
*/
int
str_is_empty(const char *s) {
  int i;
  if (s == NULL) return 1;
  else if (strlen(s) == 0) return 1;
  else {
    i = 0;
    while (s[i] && s[i] <= ' ') i++;
    return (i == (int)strlen(s));
  }
}


/* recherche la premiËre occurence d'une des chaines 'keys' dans 'src' et renvoie un pointeur vers
   cette occurence, ainsi que le numÈro de la 'keys' trouvÈe

   bien s˚r c'est pas optimal du tout, mais ON S'EN FOUT(tm)

   et oui, effectivement, 'str_multi_str' est un nom ‡ la con
*/
char *
str_multi_str(const char *src, const char **keys, int nb_keys, int *key_idx)
{
  int i;
  const char *res;

  assert(key_idx);
  *key_idx = 0;
  res = NULL;
  for (i=0; i < nb_keys; i++) {
    const char *p;
    p = strstr(src, keys[i]);
    if (p && (res==NULL || p < res)) { res = p; *key_idx = i; }
  }
  return (char*)res;
}

/* renvoie une chaine (allouÈe correctement) contenant la substitution de toutes les occurences de
   'key' dans 'src' par 'substitution' (key et substition sont des tableaux de chaines de
   caractËres, car pour faire plusieurs substitutions, mieux vaut les effectuer simultanement que
   les enchainer pour eviter les effets de bords
*/
char *
str_multi_substitute(const char *src, const char **keys, const char **substitutions, int nkeys)
{
  const char *p, *p_key;
  char *dest, *p_dest;
  int dest_sz, p_len,j;

  if (src == NULL) return NULL;

  /* calcul de la longueur de la destination.. */
  p = src;
  dest_sz = strlen(src)+1;

  while ((p_key=str_multi_str(p, keys, nkeys, &j))) {
    dest_sz += (strlen(substitutions[j]) - strlen(keys[j]));
    p = p_key+strlen(keys[j]);
  }

  dest = malloc(dest_sz);

  /* et l‡ PAF ! */
  p = src;
  p_dest = dest;
  while ((p_key=str_multi_str(p, keys, nkeys, &j))) {
    memcpy(p_dest, p, p_key-p);
    p_dest += p_key-p;
    memcpy(p_dest, substitutions[j], strlen(substitutions[j]));
    p_dest += strlen(substitutions[j]);
    p = p_key + strlen(keys[j]);
  }
  p_len = strlen(p);
  if (p_len) {
    memcpy(p_dest, p, p_len); p_dest += p_len;
  }
  *p_dest = 0;
  assert(p_dest - dest == dest_sz-1); /* capote ‡ bugs */
  return dest;
}

char *
str_substitute(const char *src, const char *key, const char *substitution) {
  return str_multi_substitute(src, &key, &substitution, 1);
}

/* quotage pour les commandes externes.. ‡ priori c'est comme pour open_url
   mais bon.. je me refuse ‡ donner la moindre garantie sur la sÈcuritÈ 

   be aware
*/
char *
shell_quote(const char *src)
{
  char *quote = "&;`'\\\"|*?~<>^()[]{}$ ";
  int i,dest_sz;
  const char *p;
  char *dest;

  if (src == NULL || strlen(src) == 0) return strdup("");

  dest_sz = strlen(src)+1;
  for (p=src; *p; p++) {
    if (strchr(quote, *p)) dest_sz+=1;
  }
  dest = malloc(dest_sz);

  for (p=src, i=0; *p; p++) {
    if (strchr(quote, *p)) {
      dest[i++] = '\\';
    }
    if (*p>=0 && *p < ' ') {
      dest[i++] = ' ';
    } else {
      dest[i++] = *p;
    }
  }
  dest[i] = 0;
  assert(i == dest_sz-1); /* kapeaute ‡ beugue */
  return dest;
}

/* 
   fonction de hachage ‡ la con (vraiment!) 
*/

int
str_hache(const unsigned char *s, int max_len)
{
  unsigned char v[4];
  const unsigned char *p;
  int i, j;

  assert(s);
  v[0] = 0xAB; v[1] = 0x13; v[2] = 0x9A; v[3] = 0x12;
  p = s;
  for (i=0, j=0; i < max_len; i++) {
    unsigned char c;
    c = ((p[i])<<j) + ((p[i]) >> (8-j));
    v[j] ^= c;
    j++; if (j == 4) j = 0;
  }
  return CVINT(v[0],v[1],v[2],v[3]);
}

unsigned char char_trans[256];
static int char_trans_init = 0;

static void 
init_char_trans()
{
  unsigned char *trans_accents  = "ÈËÎÍ À…»‡‚·‰¿¬¡ƒ˚¸˘ŸÁ«ÓÔÌÏœŒÕÃÙÛÚı‘”“’Ò";
  unsigned char *trans_accents2 = "eeeeeeeeaaaaaaaauuuucciiiiiiiioooooooon";
  int i;

  for (i=0; i < 256; i++) {
    unsigned char *p;
    if ((p=strchr(trans_accents, i))) {
      char_trans[i] = trans_accents2[(p - trans_accents)];
      } else if (i < (unsigned char)'A' || i > (unsigned char)'Z') {
	char_trans[i] = i;
      } else {
	char_trans[i] = i + 'a' - 'A';
      }
  }
  char_trans_init = 1;
}

unsigned char
chr_noaccent_tolower(unsigned char c)
{
  if (char_trans_init == 0) init_char_trans();
  return char_trans[c];
}

void
str_noaccent_tolower(unsigned char *s)
{
  int i;
  if (s == NULL) return;
  if (char_trans_init == 0) init_char_trans();
  i = 0; while(s[i]) {
    s[i] = char_trans[s[i]]; i++;
  }
}

unsigned char *
str_noaccent_casestr(const unsigned char *meule, const unsigned char *aiguille)
{
  unsigned char *res;
  unsigned char *m = strdup(meule);
  unsigned char *a = strdup(aiguille);

  str_noaccent_tolower(m);
  str_noaccent_tolower(a);
  res = strstr(m, a);
  free(a); free(m);
  return res;
}
