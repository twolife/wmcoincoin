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
  rcsid=$Id: regexp.c,v 1.4 2001/12/17 00:18:04 pouaite Exp $
  ChangeLog:
  $Log: regexp.c,v $
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

patterns_t patterns[] =
  { 
    { pat_xp, "XP[^:]*:[^[:digit:]]*([[:digit:]]*)", NULL, "d"  },
    { pat_votes, 
      "([[:digit:]]*)[[:space:]]*votes[[:space:]]*.*"
      "reste[^:]*:[^[:digit:]]*([[:digit:]]*)", NULL, "dd" }
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

char *
strndup(const char *s, int n)
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
extract_news_txt(const char *s, char **p_date, char **p_auteur, char **p_section, char **p_txt, char **p_liens)
{
  const unsigned char *p, *p2=NULL;

  *p_date = *p_auteur = *p_section = *p_txt = *p_liens = NULL;

  
  p = after_substr(s, "class=\"newsinfo\"");
  p = after_substr(s, "Approuvé le ");
  if (p) {
    p2 = strchr(p, '<');
    if (p2) {
      *p_date = strndup(p, p2-p);
    }
  }

  //  printf("p_date = '%s'\n", *p_date);

  p = after_substr(s, "class=\"newsinfo\"");
  p = after_substr(s, "Posté par");
  if (p) {
    p2 = strstr(p, "Approuvé le ");
    if (p2) {
      *p_auteur = strndup(p, p2-p);
    }
  }
  if (*p_auteur == NULL) { *p_auteur = "???"; }

  /* recherche de la section */
  p = after_substr(s, "Topic:");
  if (p == NULL) p = after_substr(s, "Thème:"); /* actuellement (16/12/2001) c'est cette chaine qui est utilisee */
  if (p) {
    p = strchr(p, '>');
    if (p) {
      p++;
      p2 = strchr(p, '<');
      if (p2 && p2 - p < 100) {
	*p_section = strndup(p, p2-p);
      }
    }
  }
  if (*p_section == NULL) *p_section = strdup("???");

  p = after_substr(p, "class=\"newstext\"");
  p = after_substr(p, ">");  
  if (p) {
    p2 = strstr(p, "</td>");
    if (p2) {
      *p_txt = strndup(p, p2-p);
    }
  }

  //  printf("p_txt = '%s'\n", *p_txt);

  if (*p_txt) { /* si pas de txt , on ne s'acharne pas */
    p = after_substr(p2, "class=\"newslink\"");
    p = after_substr(p, ">");
    if (p) {
      p2 = strstr(p, "</td>");
      if (p2) {
	*p_liens = strndup(p, p2-p);
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
      printf("erreur regex: %d\n", res);
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
		       "La regexp [%s] ne matche pas avec la chaine [%s]\n",
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

