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

#ifndef COINCOIN_REGEXP_H
#define COINCOIN_REGEXP_H

#include <regex.h>
#include <stdarg.h>

typedef enum { pat_xp, pat_votes, pat_news, pat_news_no_sec } pat_type_t;

typedef struct _patterns_t
{
  pat_type_t id;
  const char *pattern;
  regex_t *regexp;
  const char *format;
} patterns_t;

/*
** Format can be:
**  d => decimal number (need int *)
**  s => string (need char **)
*/

extern patterns_t patterns[];

int regexp_extract(const char *str, pat_type_t pattern, ...);

#endif
