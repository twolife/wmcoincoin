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
  lecture du fichier ~/.wmcoincoin/useragents
  c'est des choses qui ne sont plus très utiles maintenant ( à part pour les couleurs du trolloscope..)
  Ce fichier a été crée pour désengorger wmcoincoin.c

  rcsid=$Id: useragents_file.c,v 1.5 2002/04/13 11:55:19 pouaite Exp $
  ChangeLog:
  $Log: useragents_file.c,v $
  Revision 1.5  2002/04/13 11:55:19  pouaite
  fix kde3 + deux trois conneries

  Revision 1.4  2002/04/01 01:39:38  pouaite
  grosse grosse commition (cf changelog)

  Revision 1.3  2002/03/03 10:10:04  pouaite
  bugfixes divers et variés

  Revision 1.2  2002/02/02 23:49:17  pouaite
  plop

  Revision 1.1  2002/01/16 21:27:35  pouaite
  gros coup de balai dans wmcoincoin.c qui s'est du coup splitté en trois: wmcoincoin.c, dock.c et useragents_file.c

*/

#include <sys/types.h>
#include <regex.h>
#include "coincoin.h"

/*
  destruction de la chaine de regles -- utile en cas d'erreur
*/
void
DLFP_trib_load_rule_destroy(DLFP_trib_load_rule *r)
{
  DLFP_trib_load_rule *nr;
  if (r == NULL) return;
  while (r) {
    nr = r->next;
    free(r->name);
    regfree(&r->rule);
    free(r);
    r = nr;
  }
}

/* 
   le genre de fonction extremement penible a ecrire ... 
   c'est la version sans cleanup -> elle ne libere pas la memoire en cas d'echec
*/
static int
useragents_file_read_nocleanup(DLFP_tribune *trib, char *err_buff, int err_buff_sz)
{
  char filename[1024];
  char s[1024];
  FILE *f;
  int linenum;
  char *delim = "\t";
  int symb, coulr, coulg, coulb;
  DLFP_trib_load_rule *rule, *ruleprev;
  int err;

  assert(getenv("HOME"));
  snprintf(filename, 1024, "%s/.wmcoincoin/useragents", getenv("HOME"));

  BLAHBLAH(1, printf("lecture useragents_file '%s'\n", filename));
  f = fopen(filename, "rt");
  linenum = 0;
  ruleprev = NULL;
  if (f) {
    char *s_shape, *s_coul, *s_regex, *s_name;
    do {
      s[0] = 0; /* pour la derniere ligne */
      fgets(s, 1024, f); str_trim(s);
      linenum++;
      s_shape = strtok(s, delim); 
      if (str_is_empty(s_shape)) continue;
      if (s_shape[0] == '#') continue;
      
      s_coul = strtok(NULL,delim);
      if (str_is_empty(s_coul)) {
	snprintf(err_buff, err_buff_sz, "erreur dans '%s', ligne '%d',\nunexpected end of line, color expected\n", filename, linenum);
	return 1;
      }
      s_regex = strtok(NULL, delim);
      if (str_is_empty(s_regex)) {
	snprintf(err_buff, err_buff_sz, "erreur dans '%s', ligne '%d',\nunexpected end of line, regex expected\n", filename, linenum);
	return 1;
      }
      s_name = strtok(NULL, delim);
      if (str_is_empty(s_name)) {
	snprintf(err_buff, err_buff_sz, "erreur dans '%s', ligne '%d',\nunexpected end of line, rule name expected\n", filename, linenum);
	return 1;
      }
      
      for (symb=0; symb < NB_SYMBOLES; symb++) {
	BLAHBLAH(4,myprintf("'%s' == '%s'\n", s_shape, symboles[symb].name));
	if (strcasecmp(s_shape, symboles[symb].name) == 0) break;
      }
      if (symb == NB_SYMBOLES) {
	snprintf(err_buff, err_buff_sz, "erreur dans '%s', ligne '%d',\nje ne connais pas le symbole '%s'\n", filename, linenum, s_shape);
	return 1;
      }
      
      if (sscanf(s_coul, "%02x%02x%02x", &coulr, &coulg, &coulb) != 3) {
	snprintf(err_buff, err_buff_sz, "erreur dans '%s', ligne '%d',\nles couleurs doivent etre des nombre hexadecimaux sur 6\ncaracteres et je trouve ca:'%s'\n. c'est un nombre héxadecimal ca ? hein ?\n", filename, linenum, s_coul);
	return 1;
      }
      
      ALLOC_OBJ(rule, DLFP_trib_load_rule);
      if (ruleprev == NULL) {
	trib->rules = rule;
      } else {
	ruleprev->next = rule;
      }
      ruleprev = rule;

      rule->R = coulr; rule->G = coulg; rule->B = coulb; 
      rule->symb = symb; 
      rule->next = NULL;
      rule->name = strdup(s_name);

      /* et maintenant le plus coton, compilation de la regex */
      if ((err = regcomp(&rule->rule, s_regex, REG_EXTENDED | REG_NOSUB)) != 0) {
	int regex_errbuffsz;
	char *regex_errbuf;
	snprintf(err_buff, err_buff_sz, "erreur dans '%s', ligne '%d', dans la regex '%s'\n", filename, linenum, s_regex);
	/* cf man regex */
	regex_errbuffsz = regerror(err, &rule->rule, 0, 0);
	regex_errbuf = calloc(regex_errbuffsz+1, sizeof(char));
	regerror(err, &rule->rule, regex_errbuf, regex_errbuffsz);
	snprintf(err_buff, err_buff_sz, "%sregerror nous dit: '%s'\n", err_buff, regex_errbuf);
	free(regex_errbuf);
	return 1;
      }
      BLAHBLAH(3, myprintf("un tatouage de plus: shape = '%<MAG %s>', coul[r=%<RED %02x>, g=%<GRN %02x>, b=%<BLU %02x>], regex='%<MAG %s>'\n", s_shape, coulr, coulg, coulb, s_regex));
    } while (!feof(f));
    fclose(f);
  } else {
    snprintf(err_buff, 1024, "Le fichier <tt>%s/.wmcoincoin/useragents</tt> n'existe pas ou n'est pas lisible..\n", getenv("HOME"));
    return 2;
  }
  return 0;
}

/*
  version avec cleanup
*/
static int
useragents_file_read(DLFP_tribune *trib, char *err_buff, int err_buff_sz)
{
  int err;
  trib->rules = NULL;
  err = useragents_file_read_nocleanup(trib, err_buff, err_buff_sz);
  if (err) {
    DLFP_trib_load_rule_destroy(trib->rules);
    trib->rules = NULL;
  }
  return err;
}


int
useragents_file_reread(Dock *dock, DLFP *dlfp) {
  char errbuff[1024];
  DLFP_trib_load_rule *old_rules;
  int err;
  
  strcpy(errbuff, "Erreur pendant la lecture de <tt>.wmcoincoin/useragents</tt>\n");
  old_rules = dlfp->tribune.rules; dlfp->tribune.rules = NULL;
  err = useragents_file_read(&dlfp->tribune, errbuff, 1024);
  if (err) {
    msgbox_show(dock, errbuff);
    /*
    char s[1024];
    snprintf(s, 1024, "xmessage -geometry 600x200 \"%s\"", errbuff);
    system(s);
    */
    dlfp->tribune.rules = old_rules;
  } else {
    tribune_msg_info *it;
    DLFP_trib_load_rule_destroy(old_rules);

    it = dlfp->tribune.msg;
    BLAHBLAH(2, printf("retatouage en cours..\n"));
    while (it) {
      it->tatouage = NULL;
      tribune_tatouage(&dlfp->tribune, it);
      it = it->next;
    }
    /* ca c'est pas beau, mais les structure TL_item font reference
       a un tatouage par pointeur interpose...*/
    dock_update_pix_trolloscope(dock, &dlfp->tribune);
  }
  return err;
}

int
useragents_file_read_initial(Dock *dock, DLFP *dlfp) {
  char errbuff[1024];
  int err;
  
  err = useragents_file_read(&dlfp->tribune, errbuff, 1024);
  if (err == 2) {
    char s[1024];
    FILE *f;

    snprintf(s, 1024, "%s<p>Un fichier par défaut va être crée dans <tt>~/.wmcoincoin/useragents</tt>, "
	     "n'hesitez pas à l'editer, puis relancez wmCoinCoin<p>"
	     "<b>Remarque</b>: si vous aviez déjà customisé le fichier <tt>~/.wmCoinCoinrc</tt>"
	     "(utilisé par les version 1.x de wmcoincoin), renommez le en <tt>~/.wmcoincoin/useragents</tt><p>"
	     "... cliquez dans cette fenêtre pour continuer<br>", errbuff);
    msgbox_show_modal(dock, s);
    
    snprintf(s, 1024, "%s/.wmcoincoin/useragents", getenv("HOME"));
    f = fopen(s, "wt");
    if (f == NULL) { 
      msgbox_show_modal(dock, "IMPOSSIBLE DE CREER <tt>.wmcoincoin/useragents</tt><p><p>Si c'est comme ça, je m'en vais"); return 1; 
    }
    
#include "defaultuseragents.h"

    fclose(f);
    return 1;
  } else if (err == 1) {
    msgbox_show_modal(dock, errbuff);
  }
  return err;
}

