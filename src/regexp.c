/*
    wmCoinCoin, the stupidest WindowMaker dock applet
    Copyright (C) 2001  Julien Pommier
    This is file (C) 2001 Gaël Le Mignot <kilobug@freesurf.fr>

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

 */
/*
  rcsid=$Id: regexp.c,v 1.10 2002/08/18 20:52:15 pouaite Exp $
  ChangeLog:
  $Log: regexp.c,v $
  Revision 1.10  2002/08/18 20:52:15  pouaite
  les locales des sites fonctionnent (c bon pour les news)

  Revision 1.9  2002/06/23 22:26:01  pouaite
  bugfixes+support à deux francs des visuals pseudocolor

  Revision 1.8  2002/06/23 10:44:05  pouaite
  i18n-isation of the coincoin(kwakkwak), thanks to the incredible jjb !

  Revision 1.7  2002/05/27 18:39:14  pouaite
  trucs du week-end + patch de binny

  Revision 1.6  2002/03/03 10:10:04  pouaite
  bugfixes divers et variés

  Revision 1.5  2001/12/17 22:59:26  pouaite
  bugfix débile qui empeche le plantage du coincoin quand /backend.rdf est invalide

  Revision 1.4  2001/12/17 00:18:04  pouaite
  changement du format du backend -> on utilise desormais le /backend.rdf

  Revision 1.3  2001/12/02 20:29:31  pouaite
  rajout de 'options' et 'useragents' dans le Makefile.am ...

  Revision 1.2  2001/12/02 18:24:16  pouaite
  modif (virage) des regexp pour le texte des news, trop chiant à maintenir, du coup ça devrait marcher à nouveau sous bsd

*/

#include <stdlib.h>
#include <string.h>
#include "general.h"
#include "regexp.h"
#include "coincoin.h"

#include <libintl.h>
#define _(String) gettext (String)

patterns_t patterns[] =
  { 
    { pat_xp, "XP[^:]*:[^[:digit:]]*(-?[[:digit:]]*)", NULL, "d"  },
    { pat_votes, 
      "([[:digit:]]*)[[:space:]]*votes[[:space:]]*.*"
      "reste[^:]*:[^[0-9\\-]]*(-?[[:digit:]]*)", NULL, "dd" }
    /*    
	  
    pat_news est remplace par la fonction ci-dessous (j'arrive pas a avoir une regex qui marche
    bien -> cad qui fonctionne dans tous les cas et qui est rapide -> en particulier gros pb sur
    openbsd



    { pat_news,
      //"<!-- *NEWSBOX +[[:digit:]]+ *-->.*"
      //"class=\"newstitle\"[^>]*>.*<a[^>]*>([^<]*)</a>: <a +name=[^>]*>([^<]*)</a></td>.*"
      //"class=\"newsinfo\"[^>]*>.*Post.* +par +(.*)\\. +Approuvé +le +(.*) +<a[^>]*>\\[M\\]</a>.*"
      //"class=\"newstext\"[^>]*><a[^>]*>.*</a>(.*)</td>.*"
      //"class=\"newslink\"[^>]*>(.*)"
      //"class=\"[^\"]*\"[^>]*>.*"
      //       NULL,
      //      "ssssss" },

      //"<span class=\"newstext\"[^<]*<a[^>]*>([^<]*)</a>.*" // le titre
      ".*Approuvé +le +([^<]*).*"           // date
      "class=\"newstext\"[^>]*>(.+)</td>.*"  // texte de la news
      "class=\"newslink\"[^>]*>([^[.</td>.]]+).*"                       // le bloc des liens
      //      "class=\"[^\"]*\"[^>]*>.*"
      ,NULL,
      "sss" }*/
  };


const char *
site_locale_str(SitePrefs *sp, const char *s) {
  struct {
    const char *ref;
    const char *trad[2];
  } traduc[] = {{"Approved on ", {NULL, "Approuvé le "}}, 
		{"Posted by"   , {NULL, "Posté par"}},
		{"Topic:"      , {NULL, NULL}},
		{"Theme:"      , {NULL, "Thème:"}},
		{NULL,           {NULL,NULL}}};
  int i = 0;
  while (traduc[i].ref) {
    if (strcmp(s, traduc[i].ref) == 0) {
      const char *t = traduc[i].trad[sp->locale];
      if (t == NULL) return s; else return t;
    }
    i++;
  }
  assert(0);		
}

char *
mystrndup(const char *s, int n)
{
  char *p;

  p = malloc(n+1);
  strncpy(p, s, n); p[n] = 0;
  return p;
}

char*
after_substr(const char *s, const char *substr)
{
  const char *p;

  if (s == NULL) return NULL;
  p = strstr(s, substr);
  if (p) {
    return (char *) p+strlen(substr);
  } else {
    return NULL;
  }
}


/* remplace pat_news */
void
extract_news_txt(SitePrefs *sp, const char *s, char **p_date, char **p_auteur, char **p_section, char **p_txt, char **p_liens)
{
  const unsigned char *p, *p2=NULL;

  *p_date = *p_auteur = *p_section = *p_txt = *p_liens = NULL;

  
  p = after_substr(s, "class=\"newsinfo\"");
  p = after_substr(s, site_locale_str(sp, "Approved on "));
  if (p) {
    p2 = strchr(p, '<');
    if (p2) {
      *p_date = mystrndup(p, p2-p);
    }
  }

  //  printf("p_date = '%s'\n", *p_date);

  p = after_substr(s, "class=\"newsinfo\"");
  p = after_substr(s, site_locale_str(sp, "Posted by"));
  if (p) {
    p2 = strstr(p, site_locale_str(sp, "Approved on "));
    if (p2) {
      *p_auteur = mystrndup(p, p2-p);
    }
  }
  if (*p_auteur == NULL) { *p_auteur = strdup("???"); }

  /* recherche de la section */
  p2 = p;
  p = after_substr(s, site_locale_str(sp, "Topic:"));
  if (p == NULL) p = after_substr(s, site_locale_str(sp, "Theme:")); /* actuellement (16/12/2001) c'est cette chaine qui est utilisee */
  if (p) {
    p = strchr(p, '>');
    if (p) {
      p++;
      p2 = strchr(p, '<');
      if (p2 && p2 - p < 100) {
	*p_section = mystrndup(p, p2-p);
      }
    }
  }
  if (*p_section == NULL) *p_section = strdup("???");

  p = after_substr(p2, "class=\"newstext\"");
  p = after_substr(p, ">");  
  if (p) {
    p2 = strstr(p, "</td>");
    if (p2) {
      *p_txt = mystrndup(p, p2-p);
    }
  }

  //  printf("p_txt = '%s'\n", *p_txt);

  if (*p_txt) { /* si pas de txt , on ne s'acharne pas */
    p = after_substr(p2, "class=\"newslink\"");
    p = after_substr(p, ">");
    if (p) {
      p2 = strstr(p, "</td>");
      if (p2) {
	*p_liens = mystrndup(p, p2-p);
      }
    }
  }

  //  printf("p_liens = '%s'\n", *p_liens);


}

int 
regexp_extract(const char *str, pat_type_t pattern, ...)
{
  int res, i;
  regmatch_t *match = NULL;
  int nb;
  va_list ap;
  int *ip;
  char **sp;
  char *s = NULL;

  g_return_val_if_fail(str != NULL, FALSE);
  g_return_val_if_fail(patterns[pattern].id == pattern, FALSE);

  /* Compilation de la regexp */
  if (patterns[pattern].regexp == NULL) {
    patterns[pattern].regexp = malloc(sizeof(*(patterns[pattern].regexp)));
    g_return_val_if_fail(patterns[pattern].regexp != NULL, FALSE);
    res = regcomp(patterns[pattern].regexp, patterns[pattern].pattern,
		  REG_EXTENDED | REG_ICASE);
    if (res) {
      printf(_("regexp error: %d\n"), res);
      exit(1);
    }
    g_return_val_if_fail(res == 0, FALSE);    
  }
  
  /* Utilisation de la regexp */
  nb = (patterns[pattern].format) ? strlen(patterns[pattern].format) : 0;
  match = malloc(sizeof(*match) * (nb + 1));
  g_return_val_if_fail(match != NULL, FALSE);
  res = regexec(patterns[pattern].regexp, str, nb + 1, match, 0);
  if (res != 0) {
    BLAHBLAH(1,fprintf(stderr, 
		       _("The regexp [%s] doesn't match the string [%s]\n"),
		       patterns[pattern].pattern, str));
    free(match);
    return FALSE;
  }  
  
  /* Récupération du résultat */
  va_start(ap, pattern);
  for (i = 0; patterns[pattern].format[i]; i++) {
    
    s = malloc(match[i + 1].rm_eo - match[i + 1].rm_so + 2);
    g_return_val_if_fail(s != NULL, FALSE);
    strncpy(s, str + match[i + 1].rm_so, match[i + 1].rm_eo - match[i + 1].rm_so + 1);
    s[match[i + 1].rm_eo - match[i + 1].rm_so] = 0;
    
    switch (patterns[pattern].format[i]) {
    case 'd':
      ip = va_arg(ap, int *);
      g_return_val_if_fail(ip != NULL, FALSE);
      *ip = atoi(s);
      free(s);
      break;
    case 's':
      sp = va_arg(ap, char **);
      g_return_val_if_fail(sp != NULL, FALSE);
      *sp = s;
      break;
    default:
      free(s);
      g_return_val_if_reached(FALSE);
    }
  }
  free(match);
  va_end(ap);

  return TRUE;
}

