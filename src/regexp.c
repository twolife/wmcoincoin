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
      "reste[^:]*:[^[:digit:]]*([[:digit:]]*)", NULL, "dd" },
    { pat_news,
      //"<!-- *NEWSBOX +[[:digit:]]+ *-->.*"
      //"class=\"newstitle\"[^>]*>.*<a[^>]*>([^<]*)</a>: <a +name=[^>]*>([^<]*)</a></td>.*"
      //"class=\"newsinfo\"[^>]*>.*Post.* +par +(.*)\\. +Approuvé +le +(.*) +<a[^>]*>\\[M\\]</a>.*"
      //"class=\"newstext\"[^>]*><a[^>]*>.*</a>(.*)</td>.*"
      //"class=\"newslink\"[^>]*>(.*)"
      //"class=\"[^\"]*\"[^>]*>.*"
      //"<!-- */ *NEWSBOX *-->",
      //       NULL,
      //      "ssssss" },

      //"<span class=\"newstext\"[^<]*<a[^>]*>([^<]*)</a>.*" // le titre
      ".*Approuvé +le +([^<]*).*"           // date
      "class=\"newstext\"[^>]*>(.*)</td>.*"  // texte de la news
      "class=\"newslink\"[^>]*>(.*)"                       // le bloc des liens
      "class=\"[^\"]*\"[^>]*>.*",
      NULL,
      "sss" }
  };

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

