#ifndef COINCOIN_PREFS_H
#define COINCOIN_PREFS_H

#include "global.h"

char* fget_line(char *s, int n, FILE *f);

void init_default_prefs (int argc, char **argv, structPrefs *The_Prefs);
void check_wmcoincoin_dir(void);
void read_coincoin_options (structPrefs *The_Prefs);
void parse_cmdline (int argc, char **argv, structPrefs *The_Prefs);

#endif
