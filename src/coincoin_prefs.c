
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

/*
  rcsid=$Id: coincoin_prefs.c,v 1.20 2002/03/10 22:45:36 pouaite Exp $
  ChangeLog:
  $Log: coincoin_prefs.c,v $
  Revision 1.20  2002/03/10 22:45:36  pouaite
  <mavie>dernier commit avant de passer la nuit dans le train</mavie> , spéciale dédicace à shift et à son patch ;)

  Revision 1.19  2002/03/09 19:45:52  pouaite
  microbugfix du plopifieur et ajout d'une macro PATCH_LEVEL

  Revision 1.18  2002/03/07 18:54:34  pouaite
  raaa .. fix login_color (jjb) patch plop_words (estian) et bidouille pour le chunk encoding (a tester)

  Revision 1.17  2002/02/24 22:13:56  pouaite
  modifs pour la v2.3.5 (selection, scrollcoin, plopification, bugfixes)

  Revision 1.16  2002/02/03 23:07:32  pouaite
  *** empty log message ***

  Revision 1.15  2002/02/02 23:49:17  pouaite
  plop

  Revision 1.14  2002/01/19 19:56:09  pouaite
  petits crochets pour la mise en valeur de certains messages (cf changelog)

  Revision 1.13  2002/01/18 19:45:58  pouaite
  petit oubli d'un fichier..

  Revision 1.12  2002/01/16 10:34:21  pouaite
  ptit patch glandium

  Revision 1.11  2002/01/16 00:35:26  pouaite
  debut de detection des reponse à nos message avec des couleurs hideuses et certainement plein de bugs moisis

  Revision 1.10  2002/01/14 23:54:06  pouaite
  reconnaissance des posts effectué par l'utilisateur du canard (à suivre...)

  Revision 1.9  2002/01/13 15:19:00  pouaite
  double patch: shift -> tribune.post_cmd et lordOric -> tribune.archive

  Revision 1.8  2002/01/12 17:29:08  pouaite
  support de l'iso8859-15 (euro..)

  Revision 1.7  2002/01/10 09:18:23  pouaite
  patch de jjb (ralentissement progressif des updates de la tribune en cas d'inactivité du coincoin)

  Revision 1.6  2001/12/18 12:43:37  pouaite
  ajout de l'option de la fonte des ballons d'aide (pour mr. imr !) + bugfix d'une connerie assez naze dans la gestion du nom du fichier d'options (merci glandium de me l'avoir signalé)

  Revision 1.5  2001/12/17 00:26:32  pouaite
  ultimes bugfixes avant la v2.3.2

  Revision 1.4  2001/12/16 20:28:45  pouaite
  bugfixes divers

  Revision 1.3  2001/12/02 18:34:54  pouaite
  ajout de tags cvs Id et Log un peu partout...

*/

#ifdef __CYGWIN__
#include <getopt.h>
#endif
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
unsigned char *
fget_line(unsigned char *s, int n, FILE *f)
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
/*
static void
option_set_max_refresh_delay (const char *optarg, 
                             const char *optname,
                            structPrefs *The_Prefs) {
  The_Prefs->dlfp_max_refresh_delay = atoi(optarg);
  if (The_Prefs->dlfp_max_refresh_delay && (The_Prefs->dlfp_max_refresh_delay <= 20 || The_Prefs->dlfp_max_refresh_delay>10000)) {
    fprintf(stderr, "valeur invalide pour l'option '%s': le temps au bout duquel on ne rafraîchit plus est exprime en minutes, et doit etre > 20 (ou nul pour désactiver cette fonctionnalité)\n", optname);
    exit(1);
    BLAHBLAH(1, myprintf("temps au bout duquel on ne rafraîchit plus les %<yel news> et la %<yel tribune> fixe a: %<YEL %d> minutes\n", The_Prefs->dlfp_max_refresh_delay));
  }
  }
*/

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
option_set_useragent(const char *optarg,
                     structPrefs *The_Prefs)
{
  if (optarg == NULL) The_Prefs->user_agent[0] = 0;
  else {
    struct utsname utsn;
    char *ua;
    const char *keys[] = {"$v", "$u", "$s", "$r", "$m"};
    const char *subs[] = {VERSION,"", ""  , ""  , ""};


    ua = str_substitute(optarg, "$v", VERSION);
    if (The_Prefs->user_name) {
      subs[1] = The_Prefs->user_name;
    }
    if (uname(&utsn) != -1) {
      subs[2] = utsn.sysname;
      subs[3] = utsn.release;
      subs[4] = utsn.machine;
    }
    ua = str_multi_substitute(optarg, keys, subs, 5);
    strncpy(The_Prefs->user_agent,ua,USERAGENT_MAX_LEN);
    The_Prefs->user_agent[USERAGENT_MAX_LEN] = 0;

    free(ua);
  }
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
option_set_user_login(const char  *optarg,
		      structPrefs *The_Prefs)
{
  if (The_Prefs->user_login) free(The_Prefs->user_login);
  if (optarg == NULL) The_Prefs->user_login = strdup("");
  The_Prefs->user_login = strdup(optarg);
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
option_use_if_modified_since (const char  *optarg,
		    structPrefs *The_Prefs)
{
  if (optarg == NULL || optarg[0] == 0) {
    The_Prefs->use_if_modified_since = 0;
  } else {
    The_Prefs->use_if_modified_since = !option_get_bool_val(optarg);
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

/* stocke une liste de mots-plops séparés par une virgule */
static void
option_plop_words(const char *optarg, structPrefs *p)
{
  char *s;
  char *sep = ",";
  char *prov;
  int i;

  s = strdup(optarg); assert(s); /* pour ne pas planter le strtok, il faut s'assurer que la chaine
				    est modifiable */

  /* libération de la mémoire précedemment allouée */
  if (p->plop_words) {
    for (i=0; p->plop_words[i]; i++) {
      free(p->plop_words[i]);
    }
    free(p->plop_words); p->plop_words = NULL;
  }

  /* comptage des mots */
  p->nb_plop_words = 0;
  for (i=0; s[i]; i++) { if (s[i]==*sep) p->nb_plop_words++; }

  if (p->nb_plop_words == 0) {
    myprintf("vous ètes prié de séparer les mots-plops par des %<YEL VIRGULES>, tous les autres caractères sont stockés dans les mots\n");
    exit(0);
  }

  p->plop_words=(char**)malloc((p->nb_plop_words+1)*sizeof(char *)); assert(p->plop_words);

  for(i=0;i<(int)p->nb_plop_words;i++) {
    if(i==0){
      prov = strtok(s,sep);
    } else {
      prov = strtok(NULL,sep);
    }
    assert(prov); /* sinon y'a un truc bizarre */
    p->plop_words[i]=strdup(prov);
  }
  p->plop_words[i] = NULL;
  
  free(s);
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
      TEST_OPTION("font.encoding:", 1) {
	if (The_Prefs->font_encoding) free(The_Prefs->font_encoding);
	The_Prefs->font_encoding = strdup(optarg); ok++;
      }
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
      TEST_OPTION("tribune.stop_refresh:", 1) {
	printf("tribune.stop_refresh: cette option a été remplacée par \n\t'tribunenews.max_refresh_delay (delai max entre deux mis à jour, en minutes)"
	       "\n  et\t 'tribunenews.switch_off_coincoin_delay (delay d'inactivité en minutes à partir duquel le coincoin arrête toute activité et passe en horloge)\n"
	       "ces deux options peuvent être désactivées en mettant leur valeurs à 0\n"
	       "par defaut, tribunenews.max_refresh_delay=30 (minutes) et tribunenews.switch_off_coincoin_delay=1440 (un jour)\n");
	exit(0);
	//	option_set_max_refresh_delay(optarg, optname, The_Prefs); ok++;
      }
      TEST_OPTION("tribunenews.max_refresh_delay:", 1) {
	sscanf(optarg, "%d", &The_Prefs->dlfp_max_refresh_delay); ok++;
      }
      TEST_OPTION("tribunenews.switch_off_coincoin_delay:", 1) {
	sscanf(optarg, "%d", &The_Prefs->dlfp_switch_off_coincoin_delay); ok++;
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
      TEST_OPTION("palmipede.userlogin:", 1) {
	option_set_user_login(optarg, The_Prefs); ok++;
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
      TEST_OPTION("dock.balloons.font_family:", 0) {
	if (The_Prefs->balloon_fn_family) free(The_Prefs->balloon_fn_family);
	The_Prefs->balloon_fn_family = strdup(optarg); ok++;
      }
      TEST_OPTION("dock.balloons.font_size:", 0) {
	The_Prefs->balloon_fn_size = atoi(optarg); ok++;
      }
      TEST_OPTION("dock.pos:",1) {
	option_dock_xpos(optarg, optname, The_Prefs); ok++;
      }
      TEST_OPTION("dock.start_in_boss_mode:",1) {
	The_Prefs->start_in_boss_mode = option_get_bool_val(optarg); ok++;
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
      TEST_OPTION("http.disable_if_modified_since:", 0) {
	option_use_if_modified_since(optarg,The_Prefs); ok++;	
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
      TEST_OPTION("pinnipede.hilight.my_msg_color:", 1) {
	sscanf(optarg, "%x", &The_Prefs->pp_my_msg_color); ok++;
      }
      TEST_OPTION("pinnipede.hilight.answer_my_msg_color:", 1) {
	sscanf(optarg, "%x", &The_Prefs->pp_answer_my_msg_color); ok++;
      }
      TEST_OPTION("pinnipede.hilight.keyword_color:", 1) {
	sscanf(optarg, "%x", &The_Prefs->pp_keyword_color); ok++;
      }
      TEST_OPTION("pinnipede.plopify_color:", 1) {
	sscanf(optarg, "%x", &The_Prefs->pp_plopify_color); ok++;
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
      TEST_OPTION("pinnipede.plop_words:",1){
	option_plop_words(optarg,The_Prefs);ok++;
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
      TEST_OPTION("tribune.post_cmd:", 1) {
	if (The_Prefs->post_cmd) free(The_Prefs->post_cmd);
	The_Prefs->post_cmd = strdup(optarg); ok++;
      }
      TEST_OPTION("tribune.archive:",1) {
	if (The_Prefs->tribune_scrinechote) free(The_Prefs->tribune_scrinechote);
	The_Prefs->tribune_scrinechote = strdup(optarg); assert(strlen(The_Prefs->tribune_scrinechote)>0); ok++;
      }
      if (ok == 0) {
	printf("[~/.wmcoincoin/options, ligne %d] c'est quoi ça : '%s' -- cette option est inconnue\n", options_linenum, s);
	exit(1);
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
    snprintf(s, sz, "wmCoinCoin/" VERSION "-" PATCH_LEVEL " (palmipede; %s %s %s)",
	     utsn.sysname, utsn.release, utsn.machine);
  } else {
    snprintf(s, sz, "wmCoinCoin/" VERSION "-" PATCH_LEVEL " (palmipede; ?)");
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
  The_Prefs->dlfp_news_check_delay = 300; /* 1 fois toutes les 5 min */

  The_Prefs->dlfp_max_refresh_delay = 30;   /* 30 minutes entre deux refresh au max */
  The_Prefs->dlfp_switch_off_coincoin_delay = 24*60; /* au bout d'un jour d'inactivité, le coincoin passe en horloge et arrête les refresh */
  The_Prefs->tribune_max_msg = 300;
  The_Prefs->debug = 0;
  The_Prefs->verbosity = 0;

  The_Prefs->font_encoding = strdup("iso8859-1");
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
  The_Prefs->use_if_modified_since = 1;
  The_Prefs->no_balloons = 0;
  The_Prefs->balloon_fn_family = strdup("helvetica");
  The_Prefs->balloon_fn_size = 10;
  The_Prefs->use_iconwin = 1; /* style windowmaker par defaut */
  The_Prefs->draw_border = 0; /* idem */
  The_Prefs->dock_xpos = The_Prefs->dock_ypos = 0;
  The_Prefs->start_in_boss_mode = 0;
  The_Prefs->app_name = argv[0];
  The_Prefs->site_root = strdup("linuxfr.org");
  The_Prefs->site_port = 80;
  The_Prefs->site_path = strdup("");
  The_Prefs->path_tribune_backend = strdup("board/remote.xml");
  The_Prefs->path_news_backend = strdup("backend.rss");
  The_Prefs->path_end_news_url = strdup(",0,-1,6.html");
  The_Prefs->path_tribune_add = strdup("board/add.php3");
  The_Prefs->path_myposts = strdup("users/posts.php3?order=id");
  The_Prefs->path_messages = strdup("messages/");
  The_Prefs->default_trollo_speed = 4;
  The_Prefs->user_cookie = NULL; 
  The_Prefs->user_login = NULL;
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
  The_Prefs->pp_my_msg_color = 0xf07000;
  The_Prefs->pp_answer_my_msg_color = 0xe0b080;
  The_Prefs->pp_keyword_color = 0x008080;
  The_Prefs->pp_plopify_color = 0xa0a0a0;

  The_Prefs->plop_words = NULL;
  option_plop_words("plop,grouik,gruiiik,glop,buurp,miaou,sluurpe,côôoot,pika,pikaaaa,ka-pika,"
		    "chuuu,prout,uuuurg,blob,ploop,pl0p,c0in,pouet,coin!,flebelebelblbll,blop,gloup", The_Prefs);
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

  The_Prefs->ew_do_spell = 0;                  /*Ca fonctionne (?)
						 donc je l'active par defaut
					       */
  The_Prefs->ew_spell_cmd = strdup("ispell");  /*ispell par defaut car present 
						 partout 
					       */
  The_Prefs->ew_spell_dict = strdup("french"); /* risque mais soyons chauvin */

  The_Prefs->post_cmd = NULL;
  The_Prefs->tribune_scrinechote = strdup("~/wmcc_tribune_shot.html");
  /* pas beau !!  préanalyse de la ligne de commande pour détecter l'option 'o' */
  {
    int i;
    for (i=1; i < argc-1; i++) {
      if (strcmp(argv[i], "-o") == 0) {
	assert(argv[i+1]);
	free(The_Prefs->options_file_name);
	The_Prefs->options_file_name = strdup(argv[i+1]); 
	myprintf("utilisation du fichier d'options '%<yel %s>'\n", argv[i+1]);
	break;
      }
    }
  }
}
