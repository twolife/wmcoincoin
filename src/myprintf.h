/*
  rcsid=$Id: myprintf.h,v 1.1 2001/11/27 19:17:09 pouaite Exp $

  ChangeLog:
  $Log: myprintf.h,v $
  Revision 1.1  2001/11/27 19:17:09  pouaite
  Initial revision

  Revision 1.2  1999/10/10 18:10:35  jules
  Ajout du ChangeLog dans tous les fichiers

*/
#ifndef __MY_PRINTF_H
#define __MY_PRINTF_H

#include <stdio.h>

#ifndef __GNUC__
#  define __attribute__(x) /*nothing*/
#endif

void myprintf_disable_ansi_codes();
void myprintf_disable_color();
void myprintf_enable_ansi_codes();
void myprintf_enable_color();
int  myfprintf(FILE *f, const char *fmt, ...); //__attribute__((format(printf,1,2)));
int  myprintf(const char *fmt, ...); //__attribute__((format(printf,1,2)));
int  mysprintf(char *s, const char *fmt, ...); //__attribute__((format(printf,1,2)));

#endif
