/*
  rcsid=$Id: coin_util.c,v 1.22 2002/04/13 11:55:19 pouaite Exp $
  ChangeLog:
  $Log: coin_util.c,v $
  Revision 1.22  2002/04/13 11:55:19  pouaite
  fix kde3 + deux trois conneries

  Revision 1.21  2002/04/10 18:13:33  pouaite
  bugfix ppc

  Revision 1.20  2002/04/01 22:56:03  pouaite
  la pseudo-transparence du pinni, bugfixes divers, option tribune.backend_type

  Revision 1.19  2002/04/01 01:39:38  pouaite
  grosse grosse commition (cf changelog)

  Revision 1.18  2002/03/27 23:27:10  pouaite
  tjs des bugfixes (pour gerer des posts qui peuvent atteindre 10ko !), en parallele de la v2.3.6-5

  Revision 1.17  2002/03/27 21:22:29  pouaite
  microbugfix

  Revision 1.16  2002/03/27 20:45:06  pouaite
  deuxième vague de bugfix

  Revision 1.15  2002/03/27 19:02:04  pouaite
  bugfix pour le nouveau format du backend

  Revision 1.14  2002/03/21 22:53:07  pouaite
  ajout d'une icone pour la fenetre du pinnipede et des news

  Revision 1.13  2002/03/03 10:10:04  pouaite
  bugfixes divers et variés

  Revision 1.12  2002/03/01 00:27:40  pouaite
  trois fois rien

  Revision 1.11  2002/02/27 00:32:19  pouaite
  modifs velues

  Revision 1.10  2002/02/24 22:13:56  pouaite
  modifs pour la v2.3.5 (selection, scrollcoin, plopification, bugfixes)

  Revision 1.9  2002/01/16 21:27:35  pouaite
  gros coup de balai dans wmcoincoin.c qui s'est du coup splitté en trois: wmcoincoin.c, dock.c et useragents_file.c

  Revision 1.8  2002/01/16 00:35:26  pouaite
  debut de detection des reponse à nos message avec des couleurs hideuses et certainement plein de bugs moisis

  Revision 1.7  2002/01/14 23:54:06  pouaite
  reconnaissance des posts effectué par l'utilisateur du canard (à suivre...)

  Revision 1.6  2002/01/13 20:02:51  pouaite
  j'ai honte

  Revision 1.5  2002/01/13 15:19:00  pouaite
  double patch: shift -> tribune.post_cmd et lordOric -> tribune.archive

  Revision 1.4  2002/01/12 17:29:08  pouaite
  support de l'iso8859-15 (euro..)

  Revision 1.3  2001/12/21 12:21:23  pouaite
  pico bugfix

  Revision 1.2  2001/12/02 18:29:46  pouaite
  à la demande des décideurs de tous poils, gestion (toute naze...) de l'EURO !

*/

#include <string.h>
#include <ctype.h>
#include <stdarg.h>
#include "coin_util.h"


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


/*
  with_bug_amp : le remote.rdf a été deux fois interprete par une moulinette,
   resultat , les 'é' soit devenus des '&eacute;' puis des '&amp;eacute;'

   remarque: on peut avoir dest == src, on utilise alors une chaine temporaire

   renvoie la longueur de la chaine apres conversion

   cette fonction est utilisee par picohtml.c et coincoin_tribune.c
*/
int
convert_to_ascii(char *dest, const char *_src, int dest_sz)
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
	     {"shy;"   , "­"},
	     {"reg;"   , "®"},
	     {"macr;"  , "¯"},
	     {"deg;"   , "°"}, // 0xb0
	     {"plusmn;", "±"},
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
	     {"ETH;"   , "Ğ"}, // 0xd0
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
	     {"Yacute;", "İ"},
	     {"THORN;" , "Ş"},
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
	     {"eth;"   , "ğ"}, // 0xf0
	     {"ntilde;", "ñ"},
	     {"ograve;", "ò"},
	     {"oacute;", "ó"},
	     {"ocirc;" , "ô"},
	     {"otilde;", "õ"},
	     {"ouml;"  , "ö"},
	     {"divide;", "÷"},
	     {"oslash;", "ø"},
	     {"uacute;", "ú"},
	     {"ugrave;", "ù"},
	     {"ucirc;" , "û"},
	     {"uuml;"  , "ü"}, 
	     {"yacute;", "ı"}, // 0xfd
	     {"thorn;" , "ş"}, // 0xfe
	     {"yuml;"  , "ÿ"}, // 0xff
	     //   {"Scaron;",{352,0}},
	     //	     {"scaron;",{353,0}},
	     {"trade;", "(tm)"}, // non iso8859-1
	     {"euro;", "¤"},   // il faut iso8859-15 pour que ça fasse le bon char
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
      int found;

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
    } else if ((unsigned char)src[is] == 0x80 && id < dest_sz-2) { // cas particulier pour l'odieux EURO (encodage windows) 
      dest[id++] = '¤';
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

int
mark_html_tags(char *s, int sz) {
  char *s2 = strdup(s);
  int i,j;

  for (i=0,j=0; i < sz && s2[i]; i++) {
    if ((s2[i] == '<' || s2[i] == '>') && j < sz-2) {
      s[j++] = '\t';
      s[j++] = s2[i];
    } else if (j < sz-1) {
      s[j++] = s2[i];
    }
  }
  assert(j < sz); assert(i <= sz);
  s[j] = 0;
  free(s2);
  return j;
}


/*
  verifie si la chaine est vide (cad si elle ne contient que des caractères non imprimables 
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


/* recherche la première occurence d'une des chaines 'keys' dans 'src' et renvoie un pointeur vers
   cette occurence, ainsi que le numéro de la 'keys' trouvée

   bien sûr c'est pas optimal du tout, mais ON S'EN FOUT(tm)

   et oui, effectivement, 'str_multi_str' est un nom à la con
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

/* renvoie une chaine (allouée correctement) contenant la substitution de toutes les occurences de
   'key' dans 'src' par 'substitution' (key et substition sont des tableaux de chaines de
   caractères, car pour faire plusieurs substitutions, mieux vaut les effectuer simultanement que
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

  /* et là PAF ! */
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
  assert(p_dest - dest == dest_sz-1); /* capote à bugs */
  return dest;
}

char *
str_substitute(const char *src, const char *key, const char *substitution) {
  return str_multi_substitute(src, &key, &substitution, 1);
}

/* quotage pour les commandes externes.. à priori c'est comme pour open_url
   mais bon.. je me refuse à donner la moindre garantie sur la sécurité 

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
  assert(i == dest_sz-1); /* kapeaute à beugue */
  return dest;
}

/* 
   fonction de hachage à la con (vraiment!) 
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
  for (i=0, j=0; i < max_len && s[i]; i++) {
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
  unsigned char *trans_accents  = "éèëêÊËÉÈàâáäÀÂÁÄûüùÙçÇîïíìÏÎÍÌôóòõÔÓÒÕñ";
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

/* un printf pas très fin, mais avec allocation dynamique..
   c'est pratique ces ptites choses */
char *
str_printf(const char *fmt, ...)
{
  va_list ap;
  char *s;
  int s_sz;

  s_sz = 10; /* a reaugmenter des que la fonction est validee : */
  s = malloc(s_sz); assert(s);
  while (1) {
    int ret;
    va_start(ap, fmt);
    ret = vsnprintf(s, s_sz, fmt, ap);
    va_end(ap);
    if (ret == -1 || ret >= s_sz-1) {
      s_sz *= 2;
      assert(s_sz < 100000);
      s = realloc(s, s_sz); assert(s);
    } else 
      break;
  }
  s = realloc(s, strlen(s)+1); assert(s);
  return s;
}

/* lecture d'une ligne d'un fichier, avec allocation dynamique */
char *
str_fget_line(FILE *f)
{
  int i,c;
  char *s;
  int s_sz;

  s_sz = 100; s = malloc(s_sz); assert(s);
  i = 0;
  while ((c = fgetc(f)) > 0) {
    if (c >= ' ' || c == '\t') {
      s[i++] = c;
      if (i == s_sz) { 
	s_sz *= 2; assert(s_sz < 100000);
	s = realloc(s, s_sz); assert(s);
      }
    }
    if (c == '\n') break;
  }
  s[i] = 0; assert(i < s_sz);
  s = realloc(s, strlen(s)+1); assert(s);
  return s;
}

/* enleve les espaces au début et à la fin */
void
str_trim(unsigned char *s) {
  int i,j;

  if (s == NULL) return;
  j = strlen(s)-1;
  while (j>=0 && s[j] <= ' ') s[j--] = 0;

  i = 0; 
  while (s[i] && s[i] <= ' ') i++;
  if (i<=j) {
    memmove(s, s+i, j+2-i);
  }
}

char *
str_preencode_for_http(const char *in)
{
  const char *keys[] = {" ", "+"  , ":"  ,"@"  };
  const char *subs[] = {"+", "%2B", "%3A","%40"};
  char *s;
  
  if (in == NULL) return NULL;
  s = str_multi_substitute(in, keys, subs, 2);
  return s;
}
