
/*
    wmCoinCoin, the stupidest WindowMaker dock applet
    Copyright (C) 2001  Julien Pommier

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


//#include <getopt.h>
#include <sys/stat.h>
#include <sys/utsname.h> /* pour la fonction uname */
#include <errno.h>
#include <strings.h>
#include <string.h>
#include <unistd.h>
#include "global.h"
#include "coin_util.h"
#include "myprintf.h"

#define MAX_NEWSTITLES_LEN 512
#define MAX_MSGINFO_LEN 300

structPrefs Prefs;

int options_linenum = 0;

/* fgets qui vire les \n et les blancs du début */
char *
fget_line(char *s, int n, FILE *f)
{
  int i;
  assert(s);
  fgets(s,n,f);
  i = strlen(s)-1;
  while (i >= 0) {
    if (s[i] < ' ') s[i] = 0;
    else break;
    i--;
  }

  /* enleve les blancs du debut */
  i = 0;
  while ((s[i] == ' ' || s[i] == '\t') && s[i]) i++;
  if (s[i] == 0) s[0] = 0;
  else {
    memmove(s, s+i, strlen(s)-i + 1);
  }
  return s;
}

/*************************************************
 gestion des options
***********************************************************/
static int
option_get_bool_val(const char *optarg) {
  assert(optarg);
  if (strcmp(optarg, "1")==0) return 1;
  else if (strcasecmp(optarg, "on")==0) return 1;
  else if (strcasecmp(optarg, "yes")==0) return 1;
  else if (strcasecmp(optarg, "true")==0) return 1;
  else return 0;
}

static void
option_string_val(const char  *optarg, 
		  const char  *optname,
		  char **dest)
{
  assert(dest);
  if (*dest) free(*dest);
  if (optarg == NULL) {
    myprintf("erreur pour l'option %s, on attend une chaine de caractère\n", optname);
    exit(1);
  }
  *dest = strdup(optarg);
  //  printf("option '%s' <- '%s'\n", optname, optarg);
  assert(*dest);
}

static void
option_set_tribune_check_delay (const char  *optarg, 
                                const char  *optname,
                                structPrefs *The_Prefs) {
  The_Prefs->dlfp_tribune_check_delay = atoi(optarg);
  if (The_Prefs->dlfp_tribune_check_delay <= 10 || The_Prefs->dlfp_tribune_check_delay>10000) {
    fprintf(stderr, "valeur invalide pour l'option '%s': le delai entre deux check de la tribune est exprime en secondes, et doit etre entre 10 et 10000\n", optname);
    exit(1);
    BLAHBLAH(1, myprintf("delai entre deux check de %<yel la tribune> fixe a: %<YEL %d> secondes\n", The_Prefs->dlfp_tribune_check_delay));
  }
}

static void
option_set_news_check_delay (const char *optarg, 
                             const char *optname,
                            structPrefs *The_Prefs) {
  The_Prefs->dlfp_news_check_delay = atoi(optarg);
  if (The_Prefs->dlfp_news_check_delay <= 60 || The_Prefs->dlfp_news_check_delay>10000) {
    fprintf(stderr, "valeur invalide pour l'option '%s': le delai entre deux check des news est exprime en seconde, et doit etre > 60\n", optname);
    exit(1);
    BLAHBLAH(1, myprintf("delai entre deux check des %<yel news> fixe a: %<YEL %d> secondes\n", The_Prefs->dlfp_news_check_delay));
  }
}

static void
option_set_news_font_family (const char  *optarg,
                             structPrefs *The_Prefs) {
  if (The_Prefs->news_fn_family) free(The_Prefs->news_fn_family);
  The_Prefs->news_fn_family = strdup(optarg);
}

static void
option_set_news_font_size (const char *optarg,
                           structPrefs *The_Prefs) {
  The_Prefs->news_fn_size = atoi(optarg); 
  assert(The_Prefs->news_fn_size > 0 && The_Prefs->news_fn_size < 30);
}

static void
option_set_news_bgcolor(const char  *optarg,
			structPrefs *The_Prefs)
{
  if (sscanf(optarg, "%x", &The_Prefs->news_bgcolor)!=1) {
    fprintf(stderr, "news.bg_color: mauvaise couleur de fond (elle doit etre au format hexadecimal, du gende '1eda01'\n");
    exit(1);
  }
}

static void
option_set_news_fgcolor(const char  *optarg,
                   structPrefs *The_Prefs)
{
  if (sscanf(optarg, "%x", &The_Prefs->news_fgcolor)!=1) {
    fprintf(stderr, "news.fg_color: mauvaise couleur (elle doit etre au format hexadecimal, du gende '1eda01'\n");
    exit(1);
  }
}

static void
option_news_pos(const char  *_optarg, 
		const char  *optname,
		structPrefs *The_Prefs)
{
  char *s;
  char optarg[200];
  strncpy(optarg, _optarg,200); optarg[199] = 0;
  if ((s = strchr(optarg, ':')) != NULL) {
    *s = 0;
    The_Prefs->news_xpos = atoi(optarg);
    The_Prefs->news_ypos = atoi(s+1);
  } else {
    myfprintf(stderr, "option '%s' invalide, on attendait deux nombre x et y au format x:y\n", optname);
    exit(1);
  }
}

static void
option_news_size(const char  *_optarg, 
		 const char  *optname,
		 structPrefs *The_Prefs)
{
  char *s;
  char optarg[200];
  strncpy(optarg, _optarg,200); optarg[199] = 0;
  if ((s = strchr(optarg, ':')) != NULL) {
    *s = 0;
    The_Prefs->news_width = atoi(optarg);
    The_Prefs->news_height = atoi(s+1);
  } else {
    myfprintf(stderr, "option '%s' invalide, on attendait deux nombres width et height au format width:height\n", optname);
    exit(1);
  }
}

static void 
perform_substitution(char *str, const char *from, const char *to)
{
  char *index = str;
  int tlen = strlen(to);
  int flen = strlen(from);
  int offs, size;
  
  while ((index = strstr(index, from))) {
    offs = index - str;
    size = USERAGENT_MAX_LEN - offs - MAX(tlen, flen);
    if (size > 0)
      memmove(index + tlen, index + flen, size);
    size = MIN(tlen, USERAGENT_MAX_LEN - offs);
    memmove(index, to, tlen);
    str[USERAGENT_MAX_LEN] = 0;
  }
}


static void
option_set_useragent(const char *optarg,
                     structPrefs *The_Prefs)
{
  if (optarg == NULL) The_Prefs->user_agent[0] = 0;
  else {
	struct utsname utsn;
  
	strncpy(The_Prefs->user_agent,optarg,USERAGENT_MAX_LEN); 

	perform_substitution(The_Prefs->user_agent, "$v", VERSION);
	if (The_Prefs->user_name) {
	  perform_substitution(The_Prefs->user_agent, "$u", The_Prefs->user_name);
 	}
	if (uname(&utsn) != -1) {
	  perform_substitution(The_Prefs->user_agent, "$s", utsn.sysname);
	  perform_substitution(The_Prefs->user_agent, "$r", utsn.release);
	  perform_substitution(The_Prefs->user_agent, "$m", utsn.machine);
	}
  }
  The_Prefs->user_agent[USERAGENT_MAX_LEN] = 0;
}

static void
option_set_message (const char  *optarg,
                    structPrefs *The_Prefs)
{
  if (The_Prefs->coin_coin_message) free(The_Prefs->coin_coin_message);
  if (optarg == NULL) The_Prefs->coin_coin_message = strdup("");
  The_Prefs->coin_coin_message = strdup(optarg);
}

static void
option_set_user_name(const char  *optarg,
                    structPrefs *The_Prefs)
{
  if (The_Prefs->user_name) free(The_Prefs->user_name);
  if (optarg == NULL) The_Prefs->user_name = strdup("");
  The_Prefs->user_name = strdup(optarg);
}

static void
option_set_bgcolor(const char  *optarg,
                   structPrefs *The_Prefs)
{
  if (sscanf(optarg, "%x", &The_Prefs->bgcolor)!=1) {
    fprintf(stderr, "mauvaise couleur de fond (elle doit etre au format hexadecimal, du gende '1eda01'\n");
    exit(1);
  }
}

static void
option_set_fgcolor(const char  *optarg,
                   structPrefs *The_Prefs)
{
  if (sscanf(optarg, "%x", &The_Prefs->fgcolor)!=1) {
    fprintf(stderr, "mauvaise couleur de texte de l'applet (elle doit etre au format hexadecimal, du gende '1eda01'\n");
    exit(1);
  }
}

/* debut modif kadreg */
void
option_set_bgpixmap (const char  *optarg,
 		     structPrefs *The_Prefs)
{
   if (The_Prefs->bgpixmap) free(The_Prefs->bgpixmap);
   The_Prefs->bgpixmap = strdup(optarg);
}
/* fin modif kadreg */

static void
option_set_proxy(const char  *optarg,
                 structPrefs *The_Prefs)
{
  char *s, *S;
  
  if (The_Prefs->proxy_name) free(The_Prefs->proxy_name); 
  if (optarg == NULL || strlen(optarg) == 0) {
    fprintf(stderr, "désactivation du proxy");
    The_Prefs->proxy_name = NULL;
  } else {
    S = strdup(optarg);
    if ((s = strchr(S, ':')) != NULL) {
      *s = 0;
      The_Prefs->proxy_name = strdup(S);
      The_Prefs->proxy_port = atoi(s+1); assert(The_Prefs->proxy_port>0);
    } else {
      The_Prefs->proxy_name = strdup(S);	  
    }
    BLAHBLAH(2, myprintf("PROXY: %<YEL %s>:%<GRN %d>\n", 
                         The_Prefs->proxy_name, 
                         The_Prefs->proxy_port));
    free(S);
  }
}

static void
option_set_auth (const char  *optarg,
                 structPrefs *The_Prefs) {
  if (The_Prefs->proxy_auth) free(The_Prefs->proxy_auth); 
  if (optarg || strlen(optarg)) 
    The_Prefs->proxy_auth = strdup(optarg);
  else 
    The_Prefs->proxy_auth = NULL;
}

static void
option_proxy_nocache (const char  *optarg,
		    structPrefs *The_Prefs)
{
  if (optarg == NULL || optarg[0] == 0) {
    The_Prefs->proxy_nocache = 1;
  } else {
    The_Prefs->proxy_nocache = option_get_bool_val(optarg);
  }
}

static void
option_use_iconwin (const char  *optarg,
		    structPrefs *The_Prefs)
{
  if (optarg == NULL || optarg[0] == 0) {
    The_Prefs->use_iconwin = 0;
  } else {
    The_Prefs->use_iconwin = option_get_bool_val(optarg);
  }
}

static void
option_draw_border (const char  *optarg,
		    structPrefs *The_Prefs)
{
  if (optarg == NULL || optarg[0] == 0) {
    The_Prefs->draw_border = 0;
  } else {
    The_Prefs->draw_border = option_get_bool_val(optarg);
  }
}

static void
option_use_balloons (const char  *optarg,
		     structPrefs *The_Prefs)
{
  if (optarg == NULL || optarg[0] == 0) {
    The_Prefs->no_balloons = 0;
  } else {
    The_Prefs->no_balloons = (option_get_bool_val(optarg) ? 0 : 1);
  }
}

static void
option_dock_xpos (const char  *_optarg, 
		  const char  *optname,
		  structPrefs *The_Prefs)
{
  char *s;
  char optarg[200];
  strncpy(optarg, _optarg,200); optarg[199] = 0;
  if ((s = strchr(optarg, ':')) != NULL) {
    *s = 0;
    The_Prefs->dock_xpos = atoi(optarg);
    The_Prefs->dock_ypos = atoi(s+1);
  } else {
    myfprintf(stderr, "option '%s' invalide, on attendait deux nombre x et y au format x:y\n", optname);
    exit(1);
  }
}

static void
option_site_root (const char  *optarg,
		  structPrefs *The_Prefs)
{
  char *s;
  char *p,*p2;

  assert(optarg);
  if (The_Prefs->site_root) free(The_Prefs->site_root);
  The_Prefs->site_root = NULL;
  if (The_Prefs->site_path) free(The_Prefs->site_path);
  The_Prefs->site_path = NULL;

  s = strdup(optarg); // pour pouvoir ecrire dedans
  p = s;
  if (strstr(p,"http://")) p+=7;
  
  /* cherche numero de port et le nom du site */
  if ((p2 = strchr(p, ':'))) {
    *p2 = 0;
    The_Prefs->site_root = strdup(p);
    p2++; p = p2;
    
    while (*p2 >= '0' && *p2 <= '9' && *p2) p2++;
    The_Prefs->site_port = atoi(p);
    p = p2;
    while (*p == '/') p++;
  } else {
    p2 = strchr(p,'/');
    if (p2) {
      *p2 = 0;
      p2++;
    } else {
      p2 = p + strlen(p);
    }
    The_Prefs->site_root = strdup(p);
    p = p2;
  }
  /* recupere le path */
  if (strlen(p)) {
    p2 = p + strlen(p) - 1;
    while (p2 > p && *p2 == '/') { *p2 = 0; p2--;}
  }
  The_Prefs->site_path = strdup(p);
  myprintf("le site principal est : %<YEL http://%s>:%<GRN %d>/%<YEL %s>%s\n", 
	   The_Prefs->site_root, The_Prefs->site_port, The_Prefs->site_path,strlen(The_Prefs->site_path) ? "/" : "");
  free(s);
}

static void
option_cookie (const char  *optarg,
	       structPrefs *The_Prefs)
{
  if (The_Prefs->user_cookie) free(The_Prefs->user_cookie);
  The_Prefs->user_cookie = strdup(optarg);
}

static void
option_force_fortune(const char  *optarg,
		     structPrefs *The_Prefs)
{
  if (optarg == NULL || optarg[0] == 0) {
    The_Prefs->force_fortune_retrieval = 1;
  } else {
    The_Prefs->force_fortune_retrieval = option_get_bool_val(optarg);
  }
}


static void
option_pinnipede_pos(const char  *_optarg, 
		     const char  *optname,
		     structPrefs *The_Prefs)
{
  char *s;
  char optarg[200];
  strncpy(optarg, _optarg,200); optarg[199] = 0;
  if ((s = strchr(optarg, ':')) != NULL) {
    *s = 0;
    The_Prefs->pp_xpos = atoi(optarg);
    The_Prefs->pp_ypos = atoi(s+1);
  } else {
    myfprintf(stderr, "option '%s' invalide, on attendait deux nombre x et y au format x:y\n", optname);
    exit(1);
  }
}

static void
option_pinnipede_size(const char  *_optarg, 
		      const char  *optname,
		      structPrefs *The_Prefs)
{
  char *s;
  char optarg[200];
  strncpy(optarg, _optarg,200); optarg[199] = 0;
  if ((s = strchr(optarg, ':')) != NULL) {
    *s = 0;
    The_Prefs->pp_width = atoi(optarg);
    The_Prefs->pp_height = atoi(s+1);
  } else {
    myfprintf(stderr, "option '%s' invalide, on attendait deux nombres width et height au format width:height\n", optname);
    exit(1);
  }
}

static void
option_pinnipede_minibar(const char  *optarg,
			 structPrefs *The_Prefs)
{
  if (optarg == NULL || optarg[0] == 0) {
    The_Prefs->pp_minibar_on = 1;
  } else {
    The_Prefs->pp_minibar_on = option_get_bool_val(optarg);
  }
}

static void
option_pinnipede_nick_mode(const char  *optarg,
			 structPrefs *The_Prefs)
{
  if (optarg == NULL || optarg[0] == 0) {
    The_Prefs->pp_nick_mode = 1;
  } else {
    The_Prefs->pp_nick_mode = atoi(optarg); assert(The_Prefs->pp_nick_mode < 5 && The_Prefs->pp_nick_mode>=0);
  }
}

static void
option_pinnipede_show_tags(const char  *optarg,
			   structPrefs *The_Prefs)
{
  if (optarg == NULL || optarg[0] == 0) {
    The_Prefs->pp_html_mode = 0;
  } else {
    The_Prefs->pp_html_mode = !option_get_bool_val(optarg);
  }
}

static void
option_pinnipede_show_seconds(const char  *optarg,
			      structPrefs *The_Prefs)
{
  if (optarg == NULL || optarg[0] == 0) {
    The_Prefs->pp_nosec_mode = 0;
  } else {
    The_Prefs->pp_nosec_mode = !option_get_bool_val(optarg);
  }
}

static void
option_pinnipede_show_trollscore(const char  *optarg,
				 structPrefs *The_Prefs)
{
  if (optarg == NULL || optarg[0] == 0) {
    The_Prefs->pp_trollscore_mode = 0;
  } else {
    The_Prefs->pp_trollscore_mode = option_get_bool_val(optarg);
  }
}

static void
option_pinnipede_show_fortune(const char  *optarg,
			      structPrefs *The_Prefs)
{
  if (optarg == NULL || optarg[0] == 0) {
    The_Prefs->pp_fortune_mode = 0;
  } else {
    The_Prefs->pp_fortune_mode = option_get_bool_val(optarg);
  }
}

static void
option_pinnipede_fortune_font_family(const char  *optarg,
                             structPrefs *The_Prefs) {
  if (The_Prefs->pp_fortune_fn_family) free(The_Prefs->pp_fortune_fn_family);
  The_Prefs->pp_fortune_fn_family = strdup(optarg);
}

static void
option_pinnipede_fortune_font_size(const char *optarg,
                           structPrefs *The_Prefs) {
  The_Prefs->pp_fortune_fn_size = atoi(optarg); 
  assert(The_Prefs->pp_fortune_fn_size > 0 && The_Prefs->pp_fortune_fn_size < 30);
}

static void
option_browser(const char *optarg,const char *optname,
	       structPrefs *The_Prefs, int browser_num)
{
  int i, pc;
  char *s;
  char **bcmd = NULL;
  int ok = 0;

  if (browser_num == 1) bcmd = &The_Prefs->browser_cmd; else bcmd = &The_Prefs->browser2_cmd;
  if (*bcmd) free(*bcmd);
  *bcmd = strdup(optarg);
  s = *bcmd; assert(s);
  i = 0; pc = 0;
  /* verifie que la seule occurence de '%' est bien suivie d'un 's' */
  while (s[i]) {
    if (pc == '%') {
      if (s[i] != 's') {
	ok = 0; break;
      } else ok++;
    }
    pc = s[i]; i++;
  }
  if (ok != 1) {
    myfprintf(stderr, "%s (ligne %d) : il doit y avoir %<YEL une et une seule> "
	      "occurence du charactère '%%', \net il doit être suivi d'un 's' (c'est ce qui "
	      "sera remplacé par l'url)", optname, options_linenum);
    exit(1);
  }
  if (strstr(s, "'%s'")) {
    myfprintf(stderr, "%s (ligne %d) : %<YEL warning>: '%%s' == Mal, %%s == Bien (toutes les urls sont généreusement escapées)\n");
  }
}

static void
option_editwin_spell_enable(const char  *optarg,
			    structPrefs *The_Prefs)
{
  if (optarg == NULL || optarg[0] == 0) {
    The_Prefs->ew_do_spell = 1;
  } else {
    The_Prefs->ew_do_spell = (option_get_bool_val(optarg) ? 1 : 0);
  }
}

static void 
option_editwin_spell_cmd(const char *optarg, structPrefs *The_Prefs)
{
  free(The_Prefs->ew_spell_cmd);
  The_Prefs->ew_spell_cmd = strdup(optarg);
  assert(strlen(The_Prefs->ew_spell_cmd)>0);
}

static void
option_editwin_spell_lang(const char *optarg, structPrefs *The_Prefs)
{
  free(The_Prefs->ew_spell_dict);
  The_Prefs->ew_spell_dict = strdup(optarg);
  assert(strlen(The_Prefs->ew_spell_dict)>0);
}

void
parse_cmdline(int argc, char **argv, structPrefs *The_Prefs)
{
  int i;

  /* tant que le 'kicker' de KDE ne sera pas capable de sauvegarder
     les options de ligne de commande d'une session a l'autre... */
  if (strstr(argv[0], "wmcoincoin-kde")) {
    myprintf("vous avec invoqué wmcoincoin-kde : %<YEL Mode KDE activé>\n");
    The_Prefs->use_iconwin = 0;
  }

  while ((i = getopt(argc, argv, "hd:D:f:F:tT:u:m:v:p:b:g::P:A:wBHx:r:s:C:X:c:o:")) !=-1) {
    switch (i) {
    case 'h':
      {
        myprintf("%<GRN wmCoinCoin v" VERSION "> compile le %<GRN " __DATE__ ">\n");
	myprintf("Home page: %<BLU http:/hules.free.fr/wmcoincoin>\n");

	myprintf("Dock applet pour WindowMaker -- affiche les news de %<GRN D>%<CYA a>%<GRN L>%<CYA inux>%<GRN F>%<CYA rench>%<GRN P>%<CYA age> (%<BLU http://linuxfr.org>)\n");
	myprintf("options (la plupart de ces options peuvent être activées dans le fichier '~/.wmcoincoin/options'):\n");
	myprintf(" %<GRN -h>\t\t: mouaif...\n");
	myprintf(" %<GRN -d> %<CYA n>\t\t: fixe le delai entre deux checks de la tribune a %<CYA n> secondes (defaut %<grn %d>)\n", The_Prefs->dlfp_tribune_check_delay);
	myprintf(" %<GRN -D> %<CYA n>\t\t: fixe le delai entre deux checks des news a %<CYA n> secondes (default %<grn %d>)\n", The_Prefs->dlfp_news_check_delay);
	myprintf(" %<GRN -f> %<CYA fn>\t\t: utiliser la fonte de famille %<CYA fn> pour l'affiche des news\n\t\t  (ex: fn = %<grn %s> (defaut), ou fn = clean)\n", The_Prefs->news_fn_family);
	myprintf(" %<GRN -F> %<CYA n>\t\t: utiliser une taille de fonte de %<CYA n> pixels pour l'affiche des news (defaut = %<grn %d>)\n", The_Prefs->news_fn_size);
	myprintf(" %<GRN -u> %<CYA ua>\t\t: change le user-agent (defaut: %<grn %s>)\n", The_Prefs->user_agent);
	myprintf(" %<GRN -P> %<CYA proxy:port>\t: utilise un proxy\n");
	myprintf(" %<GRN -A> %<CYA username:password>: authentification pour le proxy, si necessaire\n");
	myprintf(" %<GRN -m> %<CYA msg>\t\t: change le message posté sur la tribune (defaut: '%<grn %s>')\n", The_Prefs->coin_coin_message);
	myprintf(" %<GRN -v> %<CYA n>\t\t: verbosité (defaut %<grn %d>) ,%<CYA n>=0,1,2,3\n", The_Prefs->verbosity);
	myprintf(" %<GRN -p> %<CYA n>\t\t: ne s'interesse qu'aux news postees il y a moins de %<CYA n> jours (defaut: %<grn %d> jours)\n", The_Prefs->news_max_nb_days);
	myprintf(" %<GRN -b> %<CYA hexcoul>\t: couleur de fond, en RGB hexa (par defaut: %<grn %06x>, un magnifique(!) jaune)\n", The_Prefs->bgcolor);
	myprintf(" %<GRN -c> %<CYA hexcoul>\t: couleur du texte de l'applet, en RGB hexa (par defaut: %<grn %06x>)\n", The_Prefs->fgcolor);
	myprintf(" %<GRN -X> %<CYA file.xpm>\t: Fichier pixmap à mettre en fond du dock (fichier.xpm de 64x64)\n");
	myprintf(" %<GRN -w> \t\t: utilise le mode windowed plutot que le mode icone\n");
	myprintf("\t\tcette option est nécessaire pour utiliser wmcoincoin avec le kicker de KDE\n");
	myprintf("\t\tmais comme le kicker de KDE 2.1 a un bug qui l'empeche de sauver les options\n");
	myprintf("\t\tde la ligne de commande d'une session a l'autre, le mieux est d'invoquer\n");
	myprintf("\t\twmcoincoin sous le nom %<YEL wmcoincoin-kde> (c'est un bete lien symbolique)\n");
	myprintf("\t\tet l'option -w sera automatiquement activée...\n");
	myprintf(" %<GRN -B> \t\t: ajoute la bordure de wmcoincoin (pour windowmanager sans dock)\n");
	myprintf(" %<GRN -x> %<CYA x:y>\t\t: essaye de positionner wmcoincoin en (x,y), cette option\n"
		 "\t\tn'est utile que si le windowmanager n'a pas de dock (utiliser avec l'option %<GRN -B>)\n");
#ifndef DISABLE_BALLOONS
	myprintf(" %<GRN -H> \t\t: desactive les ballons d'aide\n");	
#endif
	myprintf(" %<GRN -r> %<CYA url>\t\t: se connecte sur un autre site dacode que linuxfr (defaut %<grn http://linuxfr.org:80>)\n");
	myprintf(" %<GRN -s> %<CYA v>\t\t: vitesse de défilement du trolloscope (1 = rapide, 100 = 1 tres lent)\n(defaut %<grn %d>)\n", The_Prefs->default_trollo_speed);
	myprintf(" %<GRN -C> %<CYA cookie>\t: indique la valeur de votre cookie 'session_id' (il faut\n"
		 "\t\têtre authentifié). Cela permet à wmCoinCoin de détecter quand il y a une nouvelle\n"
		 "\t\tréponse à un de vos commentaires -- cette option est tout à fait facultative\n");
	myprintf(" %<GRN -o> %<CYA fichier>\t: indique le nom du fichier d'options à utiliser dans le rep ~/.wmcoincoin (defaut '%<grn %s>')\n", The_Prefs->options_file_name);
	exit(0);
      } break;
    case 'd': option_set_tribune_check_delay(optarg, "-d", The_Prefs); break;
    case 'D': option_set_news_check_delay(optarg, "-D", The_Prefs); break;
    case 'f': option_set_news_font_family(optarg,The_Prefs); break;
    case 'F': option_set_news_font_size(optarg,The_Prefs); break;
    case 't': 
    case 'T': printf("les options -t et -T sont obsolètes\n"); exit(0); break;
    case 'u': option_set_useragent(optarg,The_Prefs); break;
    case 'm': option_set_message(optarg,The_Prefs); break;
    case 'v':
      {
	The_Prefs->verbosity = atoi(optarg); assert(The_Prefs->verbosity >= 0 && The_Prefs->verbosity <= 5);
      } break;
    case 'p':
      {
	The_Prefs->news_max_nb_days = atoi(optarg);
	assert(The_Prefs->news_max_nb_days > 0 && The_Prefs->news_max_nb_days < 100);
      } break;
    case 'b': option_set_bgcolor(optarg,The_Prefs); break;
    case 'c': option_set_fgcolor(optarg,The_Prefs); break;
    case 'X': option_set_bgpixmap(optarg,The_Prefs); break;
    case 'g':
      {
	if (optarg == NULL) {
	  The_Prefs->debug = 1;
	} else The_Prefs->debug = atoi(optarg);
	
	myprintf("Activation du mode %<YEL debug> secret (dbg=%d)!\n", The_Prefs->debug);
      } break;
    case 'P': option_set_proxy(optarg,The_Prefs); break;
    case 'A': option_set_auth(optarg,The_Prefs); break;
    case 'w': option_use_iconwin(NULL,The_Prefs); break;
    case 'B': option_draw_border("1",The_Prefs); break;
    case 'H': option_use_balloons("1",The_Prefs); break;
    case 'x': option_dock_xpos(optarg, "-x",The_Prefs); break;
    case 'r': option_site_root(optarg,The_Prefs); break;
    case 's':
      {
	The_Prefs->default_trollo_speed = 1 << (atoi(optarg)-1);
	//	printf("%s -> %d\n",optarg, The_Prefs->default_trollo_speed);
	if (The_Prefs->default_trollo_speed < 1 || The_Prefs->default_trollo_speed > 128) {
	  myprintf("la vitesse doit se situer en 1 et 8\n");
	  exit(1);
	}
      } break;
    case 'C' : option_cookie(optarg,The_Prefs); break;
    case 'o': break; /* cette option est traitée dans init_default_prefs (cad AVANT la lecture du fichier d'options) */
    case ':':
    case '?':
    default:
      exit(0); break;   
    }
  }
}


int
read_coincoin_options_test_opt (const char *opt_name, 
                                char *s, 
                                char **opt_val, 
                                int need_opt_val)
{
  int lopt = strlen(opt_name);
  
  *opt_val = NULL;
  if (strncasecmp(s, opt_name, lopt) == 0) {
    while (s[lopt] == ' ' || s[lopt] == '\t') lopt++;
    if (s[lopt]) *opt_val = s+lopt;
    else if (need_opt_val) {
      fprintf(stderr, "erreur sur l'option '%s' (ligne %d) : il faut specifier une valeur\n", s, options_linenum);
      return 0;
    }
    return 1;
  } else {
    return 0;
  }
}


#define TEST_OPTION(OPTION, NEED_ARG) optname = OPTION; if (read_coincoin_options_test_opt(optname, s, &optarg, NEED_ARG))


void
read_coincoin_options (structPrefs *The_Prefs)
{
  char fname[1024];
  FILE *f;

  snprintf(fname, 1024, "%s/.wmcoincoin/%s", getenv("HOME"), The_Prefs->options_file_name);
  f = fopen(fname, "rt");
  if (f) {
    char s[1024];
    char *optarg;

    options_linenum = 0;
    do {
      const char *optname;
      int ok;
      s[0] = 0; /* pour la derniere ligne */
      fget_line(s, 1024, f); options_linenum++;
      if (s[0] == '#') continue;
      if (strlen(s) == 0) continue;
      
      BLAHBLAH(1,printf("traitement de: '%s'\n", s));
      ok = 0;
      TEST_OPTION("tribune.delay:", 1) {
	option_set_tribune_check_delay(optarg, optname, The_Prefs); ok++;
      }
      TEST_OPTION("tribune.max_messages:", 1) {
	The_Prefs->tribune_max_msg = atoi(optarg);
	assert(The_Prefs->tribune_max_msg>0 && The_Prefs->tribune_max_msg < 100000);
	ok++;
      }
      TEST_OPTION("tribune.troll_detector:", 1) {
	The_Prefs->enable_troll_detector = option_get_bool_val(optarg); ok++;
      }
      TEST_OPTION("news.delay:", 1) {
	option_set_news_check_delay(optarg,optname,The_Prefs); ok++;
      }
      TEST_OPTION("news.font_family:", 1) {
	option_set_news_font_family(optarg,The_Prefs); ok++;
      }
      TEST_OPTION("news.font_size:", 1) {
	option_set_news_font_size(optarg,The_Prefs); ok++;
      }
      TEST_OPTION("news.use_transparency:", 0) {
	printf("news.use_transparency: cette option est obsolète (cause:trop pourrie), veuillez la virer de votre ~/.wmcoincoin/options\n");
	exit(0);
      }
      TEST_OPTION("news.transparency_coef:", 1) {
	printf("news.transparency_coef: cette option est obsolète (cause:trop naze), veuillez la virer de votre ~/.wmcoincoin/options\n");
	exit(0);
      }
      TEST_OPTION("news.bg_color:", 1) {
	option_set_news_bgcolor(optarg,The_Prefs); ok++;
      }
      TEST_OPTION("news.fg_color:", 1) {
	option_set_news_fgcolor(optarg,The_Prefs); ok++;
      }
      TEST_OPTION("news.location:", 1) {
        option_news_pos(optarg, optname,The_Prefs); ok++;
      }
      TEST_OPTION("news.dimensions:", 1) {
        option_news_size(optarg, optname,The_Prefs); ok++;
      }
      TEST_OPTION("palmipede.useragent:", 1) {
	option_set_useragent(optarg,The_Prefs); ok++;
      }
      TEST_OPTION("palmipede.default_message:", 1) {
	option_set_message(optarg,The_Prefs); ok++;
      }
      TEST_OPTION("palmipede.username:", 1) {
	option_set_user_name(optarg, The_Prefs); ok++;
      }
      TEST_OPTION("palmipede.autoprompt:", 1) { /* pour compatibilité avec les ver < 2.23 */
	option_set_user_name(optarg, The_Prefs); ok++;
      }
      TEST_OPTION("dock.bg_color:", 1) {
	option_set_bgcolor(optarg,The_Prefs); ok++;
      }
      TEST_OPTION("dock.fg_color:", 1) {
	option_set_fgcolor(optarg,The_Prefs); ok++;
      }
      TEST_OPTION("dock.bg_pixmap:", 1) {
 	option_set_bgpixmap(optarg,The_Prefs); ok++;
      }
      TEST_OPTION("http.proxy:", 1) {
	option_set_proxy(optarg,The_Prefs); ok++;
      }
      TEST_OPTION("http.proxy_auth:", 1) {
	option_set_auth(optarg,The_Prefs); ok++;
      }
      TEST_OPTION("dock.iconwin:", 0) {
	option_use_iconwin(optarg,The_Prefs); ok++;
      }
      TEST_OPTION("dock.draw_border:", 0) {
	option_draw_border(optarg,The_Prefs); ok++;
      }
      TEST_OPTION("dock.use_balloons:", 0) {
	option_use_balloons(optarg,The_Prefs); ok++;
      }
      TEST_OPTION("dock.pos:",1) {
	option_dock_xpos(optarg, optname, The_Prefs); ok++;
      }
      TEST_OPTION("http.site_url:", 1) {
	option_site_root(optarg,The_Prefs); ok++;
      }

      TEST_OPTION("http.path_tribune_backend:", 1) {
	option_string_val(optarg, optname, &The_Prefs->path_tribune_backend); ok++;
      }
      TEST_OPTION("http.path_news_backend:", 1) {
	option_string_val(optarg, optname, &The_Prefs->path_news_backend); ok++;
      }
      TEST_OPTION("http.path_end_news_url:", 1) {
	option_string_val(optarg, optname, &The_Prefs->path_end_news_url); ok++;
      }
      TEST_OPTION("http.path_tribune_add:", 1) {
	option_string_val(optarg, optname, &The_Prefs->path_tribune_add); ok++;
      }
      TEST_OPTION("http.path_myposts:", 1) {
	option_string_val(optarg, optname, &The_Prefs->path_myposts); ok++;
      }
      TEST_OPTION("http.path_messages:", 1) {
	option_string_val(optarg, optname, &The_Prefs->path_messages); ok++;
      }

      TEST_OPTION("http.cookie:",1) {
	option_cookie(optarg,The_Prefs); ok++;
      }
      TEST_OPTION("http.force_fortune_retrieval:",1) {
	option_force_fortune(optarg,The_Prefs); ok++;
      }
      TEST_OPTION("http.proxy_use_nocache:",1) {
	option_proxy_nocache(optarg,The_Prefs); ok++;
      }
      TEST_OPTION("pinnipede.font_family:", 1) {
	if (The_Prefs->pp_fn_family) free(The_Prefs->pp_fn_family);
	The_Prefs->pp_fn_family = strdup(optarg); ok++;
      }
      TEST_OPTION("pinnipede.font_size:", 1) {
	The_Prefs->pp_fn_size = atoi(optarg); ok++;
      }
      TEST_OPTION("http.browser:",  1) {
	option_browser(optarg,optname,The_Prefs,1); ok++;
      }
      TEST_OPTION("http.browser2:",  1) {
	option_browser(optarg,optname,The_Prefs,2); ok++;
      }
      TEST_OPTION("pinnipede.bg_color:", 1) {
	sscanf(optarg, "%x", &The_Prefs->pp_bgcolor); ok++;
      }
      TEST_OPTION("pinnipede.fg_color:", 1) {
	sscanf(optarg, "%x", &The_Prefs->pp_fgcolor); ok++;
      }
      TEST_OPTION("pinnipede.clock_color:", 1) {
	sscanf(optarg, "%x", &The_Prefs->pp_tstamp_color); ok++;
      }
      TEST_OPTION("pinnipede.useragent_color:", 1) {
	sscanf(optarg, "%x", &The_Prefs->pp_useragent_color); ok++;
      }
      TEST_OPTION("pinnipede.login_color:", 1) {
	sscanf(optarg, "%x", &The_Prefs->pp_login_color); ok++;
      }
      TEST_OPTION("pinnipede.url_color:", 1) {
	sscanf(optarg, "%x", &The_Prefs->pp_url_color); ok++;
      }
      TEST_OPTION("pinnipede.button_color:", 1) {
	sscanf(optarg, "%x", &The_Prefs->pp_button_color); ok++;
      }
      TEST_OPTION("pinnipede.emph_color:", 1) {
	sscanf(optarg, "%x", &The_Prefs->pp_emph_color); ok++;
      }
      TEST_OPTION("pinnipede.fortune_bgcolor:", 1) {
	sscanf(optarg, "%x", &The_Prefs->pp_fortune_bgcolor); ok++;
      }
      TEST_OPTION("pinnipede.fortune_fgcolor:", 1) {
	sscanf(optarg, "%x", &The_Prefs->pp_fortune_fgcolor); ok++;
      }
      TEST_OPTION("pinnipede.trollscore_color:", 1) {
	sscanf(optarg, "%x", &The_Prefs->pp_trollscore_color); ok++;
      }
      TEST_OPTION("pinnipede.location:", 1) {
        option_pinnipede_pos(optarg, optname,The_Prefs); ok++;
      }
      TEST_OPTION("pinnipede.dimensions:", 1) {
        option_pinnipede_size(optarg, optname,The_Prefs); ok++;
      }
      TEST_OPTION("pinnipede.buttons:", 1) {
        option_pinnipede_minibar(optarg,The_Prefs); ok++;
      }
      TEST_OPTION("pinnipede.nick_mode:", 1) {
        option_pinnipede_nick_mode(optarg,The_Prefs); ok++;
      }
      TEST_OPTION("pinnipede.show_tags:", 1) {
        option_pinnipede_show_tags(optarg,The_Prefs); ok++;
      }
      TEST_OPTION("pinnipede.show_seconds:", 1) {
        option_pinnipede_show_seconds(optarg,The_Prefs); ok++;
      }
      TEST_OPTION("pinnipede.show_troll_score:", 1) {
        option_pinnipede_show_trollscore(optarg,The_Prefs); ok++;
      }
      TEST_OPTION("pinnipede.show_fortune:", 1) {
        option_pinnipede_show_fortune(optarg,The_Prefs); ok++;
      }
      TEST_OPTION("pinnipede.fortune_font_family:", 1) {
	option_pinnipede_fortune_font_family(optarg,The_Prefs); ok++;
      }
      TEST_OPTION("pinnipede.fortune_font_size:", 1) {
	option_pinnipede_fortune_font_size(optarg,The_Prefs); ok++;
      }
      TEST_OPTION("spell.enable:", 0) {
	option_editwin_spell_enable(optarg,The_Prefs); ok++;
      }
      TEST_OPTION("spell.cmd:", 1) {
	option_editwin_spell_cmd(optarg, The_Prefs); ok++;
      }
      TEST_OPTION("spell.dict:", 1) {
	option_editwin_spell_lang(optarg, The_Prefs); ok++;
      }

      if (ok == 0) {
	printf("[~/.wmcoincoin/options, ligne %d] c'est quoi ça : '%s' -- cette option est inconnue\n", options_linenum, s);
//	exit(1);
      }
    } while (!feof(f));
    fclose(f);
  } else {
    myfprintf(stderr, "Creation de '%<GRN %s>', %<YEL n'hésitez pas à l'éditer>\n", fname);
    f = fopen(fname, "wt");
    if (f) {
#include "defaultoptionsfile.h"
      fclose(f);
    } else {
      fprintf(stderr, "impossible de le creer (!!), tant pis\n");
    }
  }  
}

void
coincoin_default_useragent(char *s, int sz)
{
  struct utsname utsn;
  
  if (uname(&utsn) != -1) {
    snprintf(s, sz, "wmCoinCoin/" VERSION " (palmipede; %s %s %s)",
	     utsn.sysname, utsn.release, utsn.machine);
  } else {
    snprintf(s, sz, "wmCoinCoin/" VERSION " (palmipede; ?)");
  }
}

void
check_wmcoincoin_dir()
{
  char rpath[2048];
  struct stat stbuf;
  int is_ok;

  snprintf(rpath,2048,"%s/.wmcoincoin", getenv("HOME"));
  is_ok = 1;
  if (stat(rpath, &stbuf) == 0) {
    if (!S_ISDIR(stbuf.st_mode)) {
      fprintf(stderr, "le fichier '%s' n'est pas un répertoire !!! supprimez le, svp\n", rpath);
      is_ok = 0;
    }
  } else {
    if (errno == ENOENT) {
      myfprintf(stderr, "création de '%<YEL %s>'..\n", rpath);
      if (mkdir(rpath,-1)) {
	myfprintf(stderr, "impossible de creer le repertoire '%s'\n[erreur: '%<red %s>']\n",
		 rpath, strerror(errno));
	is_ok = 0;
      }
    } else {
      fprintf(stderr, "Il y a quelque chose de bizarre avec le répertoire '%s' [err=%s]\n", rpath, strerror(errno));
      is_ok = 0;
    }
  }
  if (is_ok == 0) {
    exit(1);
  }
}

  /* initialisation des preferences */

void init_default_prefs (int argc, char **argv, structPrefs *The_Prefs)
{
  The_Prefs->options_file_name = strdup("options");

  The_Prefs->dlfp_tribune_check_delay = 30; /* 2 fois par minute */
  The_Prefs->tribune_max_msg = 300;
  The_Prefs->dlfp_news_check_delay = 300; /* 1 fois toutes les 5 min */
  The_Prefs->debug = 0;
  The_Prefs->verbosity = 0;
  The_Prefs->news_max_nb_days = 1;
  The_Prefs->news_fn_family =  strdup("helvetica");
  The_Prefs->news_fn_size = 12;
  The_Prefs->news_bgcolor = 0xdae6e6;
  The_Prefs->news_fgcolor = 0x000000;
  The_Prefs->news_xpos = -10000;
  The_Prefs->news_ypos = -10000;
  The_Prefs->news_width = 600;
  The_Prefs->news_height = 300;

  The_Prefs->user_agent = malloc(USERAGENT_MAX_LEN+1);
  coincoin_default_useragent(The_Prefs->user_agent, USERAGENT_MAX_LEN+1);
  The_Prefs->coin_coin_message = strdup("coin ! coin !");
  The_Prefs->user_name = NULL;
  The_Prefs->bgcolor = (255L<<16) + (193L<<8) + 44; /* un joli jaune (je trouve) (NDKad : y'a que toi)*/
  The_Prefs->fgcolor = 0x000000;
  The_Prefs->bgpixmap = NULL;
  The_Prefs->http_timeout = 40;
  The_Prefs->proxy_auth = NULL;
  The_Prefs->proxy_name = NULL;
  The_Prefs->proxy_port = 1080;/* meme valeur par defaut que curl ... */
  The_Prefs->proxy_nocache = 0;
  The_Prefs->no_balloons = 0;
  The_Prefs->use_iconwin = 1; /* style windowmaker par defaut */
  The_Prefs->draw_border = 0; /* idem */
  The_Prefs->dock_xpos = The_Prefs->dock_ypos = 0;
  The_Prefs->app_name = argv[0];
  The_Prefs->site_root = strdup("linuxfr.org");
  The_Prefs->site_port = 80;
  The_Prefs->site_path = strdup("");
  The_Prefs->path_tribune_backend = strdup("board/remote.xml");
  The_Prefs->path_news_backend = strdup("short.php3");
  The_Prefs->path_end_news_url = strdup(",0,-1,6.html");
  The_Prefs->path_tribune_add = strdup("board/add.php3");
  The_Prefs->path_myposts = strdup("users/myposts.php3?order=id");
  The_Prefs->path_messages = strdup("messages/");
  The_Prefs->default_trollo_speed = 2;
  The_Prefs->user_cookie = NULL; 
  The_Prefs->force_fortune_retrieval = 0;
  The_Prefs->browser_cmd = NULL;
  The_Prefs->browser2_cmd = NULL;
  The_Prefs->enable_troll_detector = 1;
  The_Prefs->pp_fn_family = strdup("helvetica");
  The_Prefs->pp_fn_size = 12;
  The_Prefs->pp_bgcolor = 0xdae6e6;
  The_Prefs->pp_fgcolor = 0x303030;
  The_Prefs->pp_tstamp_color = 0x004000;
  The_Prefs->pp_useragent_color = 0x800000;
  The_Prefs->pp_login_color = 0xff0000;
  The_Prefs->pp_url_color = 0x0000ff;
  The_Prefs->pp_trollscore_color = 0xff0000;
  The_Prefs->pp_button_color = 0xdae6e6;
  The_Prefs->pp_emph_color = 0xffffff;
  The_Prefs->pp_xpos = -10000;
  The_Prefs->pp_ypos = -10000;
  The_Prefs->pp_width = 300;
  The_Prefs->pp_height = 455;
  The_Prefs->pp_minibar_on = 1;
  The_Prefs->pp_nosec_mode = 1;
  The_Prefs->pp_html_mode = 1;
  The_Prefs->pp_nick_mode = 4;
  The_Prefs->pp_trollscore_mode = 1;           
  The_Prefs->pp_fortune_mode = 1;
  The_Prefs->pp_fortune_bgcolor = 0xb0b0b0;
  The_Prefs->pp_fortune_fgcolor = 0x000000;
  The_Prefs->pp_fortune_fn_family = strdup("helvetica");
  The_Prefs->pp_fortune_fn_size = 10;

  The_Prefs->ew_do_spell = 1;                  /*Ca fonctionne (?)
						 donc je l'active par defaut
					       */
  The_Prefs->ew_spell_cmd = strdup("ispell");  /*ispell par defaut car present 
						 partout 
					       */
  The_Prefs->ew_spell_dict = strdup("french"); /* risque mais soyons chauvin */

  /* pas beau !!  préanalyse de la ligne de commande pour détecter l'option 'o' */
  {
    int i;
    for (i=1; i < argc-1; i++) {
      if (strcmp(argv[i], "-o") == 0) {
	assert(argv[i+1]);
	strcpy(The_Prefs->options_file_name, argv[i+1]); 
	myprintf("utilisation du fichier d'options '%<yel %s>'\n", argv[i+1]);
	break;
      }
    }
  }
}
