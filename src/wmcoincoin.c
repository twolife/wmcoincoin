
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
  rcsid=$Id: wmcoincoin.c,v 1.3 2001/12/02 18:07:03 pouaite Exp $
  ChangeLog:
  $Log: wmcoincoin.c,v $
  Revision 1.3  2001/12/02 18:07:03  pouaite
  amélioration du support de ispell avec un vilain hack + correction(..pas sur..) d'un bug de clignotement du flammometre

*/

/*
  avertissement au lecteur...
  c'est un bordel monstrueux, ce fichier est BEAUCOUP TROP GROS
*/

#define GLOBALS_HERE
//#include <getopt.h>
#include <signal.h>
#include <time.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "coincoin.h"
#include <regex.h>
#include <X11/xpm.h>
#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <X11/cursorfont.h>
#include <X11/extensions/shape.h>
#include <locale.h>
#include "spell_coin.h"

#ifdef __CYGWIN__
#include <pthread.h>
#endif

#include <sys/utsname.h> /* pour la fonction uname */

#include "coincoin_prefs.h"
#include "coin_util.h"
#include "http.h"

/* quelques images */
#include "../xpms/coin.xpm"
#include "../xpms/porte.h"
#include "../xpms/leds.h"
//#include "../xpms/letters.xpm"
#include "../xpms/weekday.xpm"
#include "../xpms/month.xpm"
#include "../xpms/clock.xpm"
#include "../xpms/date.xpm"
#include "../xpms/led.xpm"


#define MAX_NEWSTITLES_LEN 512
#define MAX_MSGINFO_LEN 300


/* des vieilles variables globales qui feraient mieux de rentrer dans une jolie structure */

//Pixmap pixmap_letters;


/* ça c'est le tableau du trolloscope */
TL_item **tribune_load; //[TRIBUNE_LOAD_WIDTH][TRIBUNE_LOAD_HEIGHT];
/* tribune_resolution: 5(faible) ou 2(moyenne) ou 1(hires!)*/
int tribune_resolution = 5; /* de retour, à la demande de monsieur 'The Original Palmipède' */
int tribune_load_bgr=0, tribune_load_bgg=0, tribune_load_bgb = 0; /* couleur de fond du trolloscope (oui c'est tout naze) */

int tribune_load_clign_step = -1; // -1 -> pas de clignot, -2 -> arret demande

structPrefs Prefs;


int char2bitmap[256];
  
//float trollo_rate = 0.; /* nb de message sur la tribune / minute */


int opened_cnt; /* ça c'est de la bonne vieille variable qui date de la v0.9...*/

Dock *dock;

/*
   la fonction de la honte ...

   ouverture d'une url dans un browser externe (avec une commande du type 'mozilla url &', executée par le shell)

   url est une chaine de carateres (terminee par un zero), de longueur
   raisonnable, mais susceptible de contenir absolument n'importe quels caratères.
   Le principe est d'échapper tous ceux qui sont susceptible d'etre interpretes par le shell.



extrait de RATIONALE FOR SECURITY FILTER, d.wheeler


les arguments bidons sont:
  ballon_{x,y} : position d'affichage du ballon d'aide quand aucun browser externe n'a été défini
  browser_num = 1 ou 2 -> utilisation de la commande Prefs.browser_cmd ou de la commande
  alternative Prefs.browser2_cmd
*/
void
open_url(const unsigned char *url, int balloon_x, int balloon_y, int browser_num)
{
  char hex[16] = "0123456789ABCDEF";
#define CMDSZ 4096
  char s[CMDSZ];
#define SECSZ 2048
  char secure_url[SECSZ];
  int i,j;
  char *bcmd;

  if (url == NULL) return;

  bcmd = (browser_num == 1) ? Prefs.browser_cmd : Prefs.browser2_cmd;

  /* maintenant on est facho */
  if (strncasecmp(url, "http://",7) != 0 &&
      strncasecmp(url, "ftp://", 6) != 0 &&
      strncasecmp(url, "https://",8) != 0) {
    snprintf(s, CMDSZ, "cette url PUE !: <b>%s</b> (ni ftp://, ni http://)<br>"
           "il faudra la lancer à la main (copier dans le clipboard avec clic droit etc..)", url);
    if (balloon_x != -1 && balloon_y != -1) {
      balloon_show(dock, balloon_x, balloon_y, 10, 10, s, 300);
    } else {
      fprintf(stderr, "wmcoincoin: %s\n", s);
    }
    return;
  }

  if (bcmd == NULL) {
    snprintf(s, CMDSZ, "vous n'avez pas défini le browser externe dans ~/.wmcoincoin/options (%s), inutile de cliquer comme un fou sur les <b>[url]</b>...",
	     (browser_num == 1) ? "http.browser: lacommande" : "http.browser2: lacommande");
    if (balloon_x > 0 && balloon_y > 0) {
      balloon_show(dock, balloon_x, balloon_y, 10, 10, s, 300);
    } else {
      fprintf(stderr, "wmcoincoin: %s\n", s);
    }
    return;
  }


      /* d'apres d.wheeler (et la rfc 1738,2396), une url est formée de caractères
       - réservés:                                ; / ? : @ & = + $ ,
       - non réservés:    A-Z, a-z, 0-9, et | | | | | | - _ . ! ~ * ' ( )

       tous les caractères qui ne sont pas 'réservés' peuvent être hexencodés en %XX.

       toujours selon lui, la liste des caractères interprétables par le shell est
       (http://www.dwheeler.com/secure-programs/Secure-Programs-HOWTO/limit-call) :
       & ; ` ' \ " | * ? ~ < > ^ ( ) [ ] { } $ \n \r


       Les virgules et les parenthèses fermantes dans une url sont susceptible d'etre mal interpretées
       par netscape quand on lui passe l'url en ligne de commande avec nestcape -remote openURL(http://blahblah, ddfdjh)
      */

  /*
     FILTRE DEUX-EN-UN:
     on hex-encode tous les caractère qui ne sont pas les suivants:
     ; / ? : @ & = + $ # % A-Z a-z 0-9 - _ . , et tout ce qui est inférieur à 0x20 est giclé

     ceci dit, on pourrait aussi refuser toute url contenant des caractère devant être hexencodés...

     puis, les charactères non alphanumériques qui n'ont pas été hexencodés sont backslashés pour ne surtout pas
     être interpretés par /bin/sh
  */
  for (i=0, j=0; url[i] && j < SECSZ-3; i++) {
    if (url[i] > ' ') { /* tout ce qui est ' ', '\r' '\n' '\t' passe à la trappe */
      int alnum;
      alnum = ((url[i]>='a' && url[i] <= 'z') ||
             (url[i]>='A' && url[i] <= 'Z') ||
             (url[i]>='0' && url[i] <= '9')) ? 1 : 0;
      /* remarque: on hexencode un caractère réservé, la virgule, mais ce n'est apparament pas génant */
      if (alnum || strchr(";/?:@&=+$#%-_.", url[i])) {
      if (alnum == 0 && url[i] != '.' && url[i] != '_' && url[i] != '/' && url[i] != ':') {
        secure_url[j++] = '\\'; // on insere le backslash
      }
      secure_url[j++] = url[i];
      } else {
      secure_url[j++] = '%';
      secure_url[j++] = hex[(url[i]>>4) & 0x0f];
      secure_url[j++] = hex[(url[i]   ) & 0x0f];
      }
    }
  }
  secure_url[j] = 0;

  assert(j>0 && j < SECSZ); //rhoo

  /*
    donc maintenant on a une jolie url toute propre, et qui *marche* contrairement
     à la bouillie que j'avais fait dans coincoin 2.01
  */

  snprintf(s, CMDSZ, bcmd, secure_url);

  myfprintf(stderr, "Execution de \"%<YEL %s>\"\n", s);
  /*
    system fait appel à /bin/sh , mais on a backslashé tous les caractères embetants
  */
  system(s);
}

/* remplacé par RGB2PIXEL
unsigned long
pixel_color(Dock *dock, unsigned char r, unsigned char g, unsigned char b)
{
  XColor c;
  c.red = r*256; c.green = g*256; c.blue = b*256;
  c.flags = DoRed | DoGreen | DoBlue;
  if (XAllocColor(dock->display, DefaultColormap(dock->display, dock->screennum), &c) == BadColor) {
    return BlackPixel(dock->display, dock->screennum);
  } else {
    return c.pixel;
  }
}
*/

void
dock_get_icon_pos(Dock *dock, int *iconx, int *icony)
{
  Window junkwin;
  XWindowAttributes wa;

  /* lecture de la position de wmcoincoin */
  if (!XGetWindowAttributes(dock->display, DOCK_WIN(dock), &wa))
    fprintf(stderr, "Can't get window attributes.\n");
  XTranslateCoordinates (dock->display, DOCK_WIN(dock), wa.root, 
			 -wa.border_width,
			 -wa.border_width,
			 iconx, icony, &junkwin);
}

static void
update_pix_tribune_load(Dock *dock, DLFP_tribune *trib)
{
  tribune_msg_info *it;
  int tnow;
  int i,j;
  RGBAImage *img;

  /* nombre de minutes regroupees dans une meme colonne du graphique */
  int col_nb_min = 0;
  int trib_nrow, trib_ncol;

  if (flag_updating_tribune) return;
  //  assert(flag_tribune_updated == 1);
  dock->tribune_updatable = 0;

  if (tribune_resolution == 5) {
    col_nb_min = 1*dock->trolloscope_speed;
  } else if (tribune_resolution == 2) {
    col_nb_min = 2*dock->trolloscope_speed;
  } else if (tribune_resolution == 1) {
    col_nb_min = 4*dock->trolloscope_speed;
  } else assert(0);

  trib_nrow = (TRIBUNE_LOAD_HEIGHT + tribune_resolution - 1) / tribune_resolution;
  trib_ncol = (TRIBUNE_LOAD_WIDTH  + tribune_resolution - 1) / tribune_resolution;

  for (i=0; i < trib_nrow; i++) {
    for (j=0; j < trib_ncol; j++) {
      tribune_load[i][j].id = -1;
      tribune_load[i][j].tatouage = NULL;
    }
  }

  it = trib->msg;

  tnow = (tribune_get_time_now(trib)/60 + col_nb_min - 1)/col_nb_min;

  /* construction de tribune_load */
  while (it) {
    if (it->tatouage) {
      int age;

      //      age = tribune_get_msg_age(trib, it) / 60 / col_nb_min;
      age = (tnow - (it->timestamp/60 + col_nb_min - 1)/col_nb_min + ((24*60)/col_nb_min))%((24*60)/col_nb_min);
      BLAHBLAH(4, myprintf("id=%<YEL %d>, age=%<RED %d> ts=%d, col_nb_min=%d, tnow=%d\n", it->id, age,it->timestamp,col_nb_min,tnow));
      assert(age >= 0);
      if (age < trib_ncol) {
	/* on empile les message sur la pile d'age 'age' (je suis clair?)*/
	i = 0;
	while (tribune_load[i][age].id != -1) {
	  i++; if (i == trib_nrow) break;
	}
	if (i < trib_nrow) {
	  tribune_load[i][age].id = it->id;
	  tribune_load[i][age].tatouage = it->tatouage;
	}
      }
    }
    it = it->next;
  }

  img = RGBACreateImage(TRIBUNE_LOAD_WIDTH, TRIBUNE_LOAD_HEIGHT); assert(img);
  /*
  for (i=0; i < TRIBUNE_LOAD_HEIGHT; i++) {
    for (j=0; j < TRIBUNE_LOAD_WIDTH; j++) {
      img->data[(i*TRIBUNE_LOAD_WIDTH  + j)*3 + 0] = tribune_load_bgr; 
      img->data[(i*TRIBUNE_LOAD_WIDTH  + j)*3 + 1] = tribune_load_bgg; 
      img->data[(i*TRIBUNE_LOAD_WIDTH  + j)*3 + 2] = tribune_load_bgg; 
    }
    }*/
  for (i=0; i < trib_nrow; i++) {
    for (j=0; j < trib_ncol; j++) {
      int r,g,b,symb;

      if (tribune_load[i][j].id >= 0 && tribune_load[i][j].tatouage) {
	r = tribune_load[i][j].tatouage->R;
	g = tribune_load[i][j].tatouage->G;
	b = tribune_load[i][j].tatouage->B;
	symb = tribune_load[i][j].tatouage->symb;
      } else { 
	r = tribune_load_bgr; g = tribune_load_bgg; b = tribune_load_bgb; symb = 0;
	tribune_load[i][j].id = 0;
	tribune_load[i][j].tatouage = NULL;
      }
      BLAHBLAH(5, myprintf("[%<YEL %1d>%<RED %02x>] ", symb, (r+g+b)%256));
      if (tribune_resolution == 5) {
	int ii,jj;
	for (ii=0; ii < 5; ii++) {
	  for (jj=0; jj < 5; jj++) {
	    if (i*5+ii < TRIBUNE_LOAD_HEIGHT &&  TRIBUNE_LOAD_WIDTH-1-(j*5+jj) >= 0) {
	      RGBAPixel *pixel;
	      pixel = &img->data[ TRIBUNE_LOAD_HEIGHT-1-(i*5+ii) ][ TRIBUNE_LOAD_WIDTH-1-(j*5+jj) ];
	      switch (symboles[symb].s[ii][jj]) {
	      case ' ': 
		pixel->rgba[0] = tribune_load_bgr; 
		pixel->rgba[1] = tribune_load_bgg; 
		pixel->rgba[2] = tribune_load_bgb; 
		break;
	      case '.': 
		pixel->rgba[0] = (r/2); 
		pixel->rgba[1] = (g/2); 
		pixel->rgba[2]=(b/2); break;
	      case ':': pixel->rgba[0] = ((r*2)/3); pixel->rgba[1] = ((g*2)/3); pixel->rgba[2]=((b*2)/3); break;
	      case 'x': pixel->rgba[0] = ((r*3)/4); pixel->rgba[1] = ((g*3)/4); pixel->rgba[2]=((b*3)/4); break;
	      case 'X': pixel->rgba[0] = r; pixel->rgba[1] = g; pixel->rgba[2]=b; break;
	      case '#': pixel->rgba[0] = MIN((r*5)/4,255); pixel->rgba[1] = MIN((g*5)/4,255); pixel->rgba[2]=MIN((b*5)/4,255); break;
	      default: assert(1==0); /* autant dire que j'ai pas envie que ca arrive */
	      }
	    }
	  }
	}
      } else if(tribune_resolution == 2) {
	int ii,jj;
	for (ii=0; ii < 2; ii++) {
	  for (jj=0; jj < 2; jj++) {
	    if (i*2+ii < TRIBUNE_LOAD_HEIGHT && TRIBUNE_LOAD_WIDTH-1-(j*2+jj) >= 0) {
	      RGBAPixel *pixel;
	      pixel = &img->data[ TRIBUNE_LOAD_HEIGHT-1-(i*2+ii) ][ TRIBUNE_LOAD_WIDTH-1-(j*2+jj) ];
	      pixel->rgba[0] = r; pixel->rgba[1] = g; pixel->rgba[2] = b;
	    }
	  }
	}           
      } else if (tribune_resolution == 1) {
	RGBAPixel *pixel;
	pixel = &img->data[ TRIBUNE_LOAD_HEIGHT-1-i ][ trib_ncol-1-j ];
	pixel->rgba[0] = r; pixel->rgba[1] = g; pixel->rgba[2] = b;
      }
    }
    BLAHBLAH(5,myprintf("\n"));
  }

  if (dock->tl_item_survol) { 
    /* ce fut un bug idiot */
    if (dock->tl_item_survol->id <= 0) {
      dock->tl_item_survol = NULL;
      dock->tl_item_clicked = 0;
    }
  }

  /* et voila ! y'a plus qu'a fabriquer le pixmap */
  XFreePixmap(dock->display, dock->pix_tribune_load);
  dock->pix_tribune_load = RGBAImage2Pixmap(dock->rgba_context, img);
  assert(dock->pix_tribune_load != None);
  RGBADestroyImage(img);
  dock->tribune_updatable = 1;
}


int
is_empty(char *s) {
  int i;
  if (s == NULL) return 1;
  else if (strlen(s) == 0) return 1;
  else {
    i = 0;
    while (s[i] && s[i] <= ' ') i++;
    return (i == (int)strlen(s));
  }
}

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
int
read_rcfile_nocleanup(DLFP_tribune *trib, char *err_buff, int err_buff_sz)
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

  BLAHBLAH(1, printf("lecture rcfile '%s'\n", filename));
  f = fopen(filename, "rt");
  linenum = 0;
  ruleprev = NULL;
  if (f) {
    char *s_shape, *s_coul, *s_regex, *s_name;
    do {
      s[0] = 0; /* pour la derniere ligne */
      fget_line(s, 1024, f);
      linenum++;
      s_shape = strtok(s, delim); 
      if (is_empty(s_shape)) continue;
      if (s_shape[0] == '#') continue;
      
      s_coul = strtok(NULL,delim);
      if (is_empty(s_coul)) {
	snprintf(err_buff, err_buff_sz, "erreur dans '%s', ligne '%d',\nunexpected end of line, color expected\n", filename, linenum);
	return 1;
      }
      s_regex = strtok(NULL, delim);
      if (is_empty(s_regex)) {
	snprintf(err_buff, err_buff_sz, "erreur dans '%s', ligne '%d',\nunexpected end of line, regex expected\n", filename, linenum);
	return 1;
      }
      s_name = strtok(NULL, delim);
      if (is_empty(s_name)) {
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
      BLAHBLAH(2, myprintf("un tatouage de plus: shape = '%<MAG %s>', coul[r=%<RED %02x>, g=%<GRN %02x>, b=%<BLU %02x>], regex='%<MAG %s>'\n", s_shape, coulr, coulg, coulb, s_regex));
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
int
read_rcfile(DLFP_tribune *trib, char *err_buff, int err_buff_sz)
{
  int err;
  trib->rules = NULL;
  err = read_rcfile_nocleanup(trib, err_buff, err_buff_sz);
  if (err) {
    DLFP_trib_load_rule_destroy(trib->rules);
    trib->rules = NULL;
  }
  return err;
}


int
reread_rcfile(DLFP *dlfp) {
  char errbuff[1024];
  DLFP_trib_load_rule *old_rules;
  int err;
  
  strcpy(errbuff, "Erreur pendant la lecture de <tt>.wmcoincoin/useragents</tt>\n");
  old_rules = dlfp->tribune.rules; dlfp->tribune.rules = NULL;
  err = read_rcfile(&dlfp->tribune, errbuff, 1024);
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
    update_pix_tribune_load(dock, &dlfp->tribune);
  }
  return err;
}

int
read_initial_rcfile(DLFP *dlfp) {
  char errbuff[1024];
  int err;
  
  err = read_rcfile(&dlfp->tribune, errbuff, 1024);
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
    //    char s[1024];
    msgbox_show_modal(dock, errbuff);
    /*
      snprintf(s, 1024, "xmessage \"%s\", errbuff);
    system(s);
    */
  }
  return err;
}



void
dlfp_checkout(Dock *dock, DLFP *dlfp)
{
  if (flag_updating_news == 0) {
    if (dlfp->updated) {
      DLFP_news *n;
      int pos, pos0;
      dlfp->updated = 0;
      if (dlfp_count_news(dlfp) == 0) {
	sprintf(dock->newstitles, "..NO NEWS");
	memset(dock->newstitles_id, 0, sizeof(dock->newstitles_id[0]) * MAX_NEWSTITLES_LEN);
	dock->newstitles_pos = 0; 
	dock->newstitles_char_dec = 4;
      } else {
	pos = 0; 
	dock->newstitles[0] = 0;      
	n = dlfp->news;
	while (n) {
	  int i;
	  static char *separ = " ... ";
	  pos0 = pos;
	  //	  printf("ajout de '%s'\n",n->titre);
	  for (i=0; i < (int)strlen(n->titre); i++) {
	    if (pos >= MAX_NEWSTITLES_LEN-1) break;
	    dock->newstitles[pos] = n->titre[i];
	    pos++;
	  }
	  for (i=0; i < (int)strlen(separ); i++) {
	    if (pos >= MAX_NEWSTITLES_LEN-1) break;
	    dock->newstitles[pos] = separ[i];
	    pos++;
	  }
	  dock->newstitles[pos] = 0;
	  
	  for (i=pos0; i <= pos; i++) {
	    dock->newstitles_id[i] = n->id;
	  }
	  n = n->next;
	}
	dock->newstitles_pos = 0; 
	dock->newstitles_char_dec = 0;
      }
    }
  }
}

void
checkout_tribune(Dock *dock, DLFP *dlfp)
{
  if (flag_updating_tribune == 0) {
    if (flag_tribune_updated) {
      int i;
      for (i=0; i < 5; i++) dock->tribune_time[i] = dlfp->tribune.last_post_time[i];
      dock->tribune_time[5] = 0;

      dlfp_tribune_get_trollo_rate(&dlfp->tribune, &dock->trib_trollo_rate, &dock->trib_trollo_score);
      BLAHBLAH(2,myprintf("%<red checkout_tribune>: %<WHT trollo_rate <- %f, trollo_score <- %f>\n", dock->trib_trollo_rate, dock->trib_trollo_score));

      assert(dock->trib_trollo_rate < 10000);

      update_pix_tribune_load(dock, &dlfp->tribune);

      pp_set_tribune_updated(dock);

      flag_tribune_updated = 0;
    }
  }
}

void
textout_msg(Dock *dock, unsigned char *msg, int x, int y, int w)
{
  int cx, cnt, dec;
  XRectangle xr;

  xr.x = x; xr.y = y; xr.width = w; xr.height = 11;
  XSetForeground(dock->display, dock->NormalGC, IRGB2PIXEL(Prefs.fgcolor));
  XSetFont(dock->display, dock->NormalGC, dock->fixed_font->fid);
  XSetClipRectangles(dock->display, dock->NormalGC, 0, 0, &xr, 1, Unsorted); /* faut pas que ça bave sur les bord :) */
  if (dock->flag_survol_trollo) {
    unsigned char minimsg[10];
    int tscore = (int)(dock->trib_trollo_score*3);
    if (tscore < 1000) {
      snprintf(minimsg, 10, "troll:%3d", tscore);
    } else {
      snprintf(minimsg, 10, "trl:%5d", tscore);
    }
    cx = x + (56-strlen(minimsg)*DOCK_FIXED_FONT_W)/2; /* super centrage..*/
    XDrawString(dock->display, dock->coinpix, dock->NormalGC, cx, y+dock->fixed_font->ascent+1, minimsg, strlen(minimsg));

  } else if (dock->view_id_in_newstitles == 0) {
    dec = dock->newstitles_char_dec;
    cx = x - dec;
    cnt = dock->newstitles_pos;
    //  if (msg[cnt] == 0) return;
    do {
      XDrawString(dock->display, dock->coinpix, dock->NormalGC, cx, y+dock->fixed_font->ascent+1, &msg[cnt], 1);
      //      XCopyArea(dock->display, pixmap_letters, dock->coinpix, dock->and_GC, char2bitmap[msg[cnt]]+dec,0,MIN(cx+6, x+w)-MAX(cx,x),12,MAX(cx,x),y);
      cx += 6;
      cnt++; if (msg[cnt] == 0) cnt = 0;
      dec = 0;
    } while (cx < x+w);
  } else {
    unsigned char minimsg[10];
    minimsg[0] = 0;
    if (dock->view_id_timer_cnt % 40 > 15) {
      snprintf(minimsg, 10, "id=%d", dock->view_id_in_newstitles); 
    } else if (dock->tl_item_survol) {
      tribune_msg_info *mi;
      mi = tribune_find_id(&dock->dlfp->tribune, dock->tl_item_survol->id);
      if (mi) {
	struct tm t;
	localtime_r(&mi->timestamp, &t);
	//snprintf(minimsg, 10, "%02d:%02d", (int)((mi->timestamp/3600)%24), (int)((mi->timestamp/60)%60));
	snprintf(minimsg, 10, "%02d:%02d:%02d", t.tm_hour, t.tm_min, t.tm_sec);
      } else {
	strcpy(minimsg, "??:?? BUG");
      }
    }
    if (strlen(minimsg)) {
      cx = x + (56-strlen(minimsg)*DOCK_FIXED_FONT_W)/2; /* super centrage..*/
      
      XDrawString(dock->display, dock->coinpix, dock->NormalGC, cx, y+dock->fixed_font->ascent+1, minimsg, strlen(minimsg));
    }
  }
  XSetClipMask(dock->display, dock->NormalGC, None);
}

void
textout_msginfo(Dock *dock, int x, int y)
{
  int cx, cnt;
  char c;
  XRectangle xr;

  if (strlen(dock->msginfo)==0) return;

  xr.x = x; xr.y = y; xr.width = 56; xr.height = 11;
  XSetFont(dock->display, dock->NormalGC, dock->fixed_font->fid);
  XSetForeground(dock->display, dock->NormalGC, IRGB2PIXEL(Prefs.fgcolor));
  XSetClipRectangles(dock->display, dock->NormalGC, 0, 0, &xr, 1, Unsorted); /* faut pas que ça bave sur les bord :) */

  if (dock->msginfo_defil) {
    int dec;
    dec = (dock->msginfo_defil - 1) % 6;
    cx = x - dec;
    cnt = (dock->msginfo_defil - 1) / 6;
    do {
      cnt = cnt % (strlen(dock->msginfo) + 3);
      
      if (cnt < (int)strlen(dock->msginfo)) c = dock->msginfo[cnt]; else c = '.';

      XDrawString(dock->display, dock->coinpix, dock->NormalGC, cx, y+dock->fixed_font->ascent+1, &c, 1);

      //      XCopyArea(dock->display, pixmap_letters, dock->coinpix, dock->and_GC, char2bitmap[c]+dec,0,MIN(cx+6, x+56)-MAX(cx,x),12,MAX(cx,x),y);
      cx += DOCK_FIXED_FONT_W;
      cnt++;
      dec = 0;
    } while (cx < x+56);
  } else {
    cx = x + MAX((56 - (int)strlen(dock->msginfo)*6)/2, 0);
    assert(cx < 200);
    cnt = 0;
    do {
      if (cnt < (int)strlen(dock->msginfo)) c = dock->msginfo[cnt]; else c = ' ';

      XDrawString(dock->display, dock->coinpix, dock->NormalGC, cx, y+dock->fixed_font->ascent+1, &c, 1);
    
      //      XCopyArea(dock->display, pixmap_letters, dock->coinpix, dock->and_GC, char2bitmap[c],0,MIN(cx+6, x+56)-MAX(cx,x),12,MAX(cx,x),y);
      cx += DOCK_FIXED_FONT_W;
      cnt++;
    } while (cx < x+56);
  }
  XSetClipMask(dock->display, dock->NormalGC, None);
}

void
led_create(Led *l, int x, int y)
{
  assert(l);
  l->xpos = x;
  l->ypos = y;
  l->coul[0] = OFF; l->coul[1] = OFF; l->coul[2] = OFF;
  l->delay[0] = 12; l->delay[1] = 1; l->delay[2] = 10;
  l->cnt = 0;
  l->colcnt = 0;
}

void
led_update(Led *l)
{
  l->cnt++;
  if (l->cnt <= l->delay[0]) {
    l->colcnt = 0;
  } else if (l->cnt <= l->delay[0]+l->delay[1]) {
    l->colcnt = 1;
  } else if (l->cnt <= l->delay[0]+l->delay[1]+l->delay[2]) {
    l->colcnt = 2;
  } else if (l->cnt <= l->delay[0]+l->delay[1]*2+l->delay[2]) {
    l->colcnt = 1;
  } else {
    l->cnt = l->cnt % ( l->delay[0]+l->delay[1]+l->delay[2]);
    l->colcnt = 0;
  }
}


void 
led_color(Led *l, LedColor c1, LedColor c2, LedColor c3)
{
  l->coul[0] = c1; l->coul[1] = c2; l->coul[2] = c3;
}

void
leds_create(Leds *leds)
{
  assert(leds);

  leds->pixmap = pixmap_create_from_raw(dock->rgba_context, leds_image.width, leds_image.height, leds_image.bytes_per_pixel, leds_image.pixel_data);
  leds->mask = alpha2pixmap_mask(dock->display, dock->coinpix, leds_image.width, leds_image.height, leds_image.pixel_data, 255);
  led_create(&leds->led[0], 2, 16);  leds->led[0].cnt = 5;  leds->led[0].delay[0] = 12;
  led_create(&leds->led[1], 13, 16); leds->led[1].cnt = 10; leds->led[1].delay[0] =  4; leds->led[1].delay[1] =  2; leds->led[1].delay[2] =  2;
  led_create(&leds->led[2], 24, 16); leds->led[2].cnt = 15; leds->led[2].delay[0] = 15;
  led_create(&leds->led[3], 35, 16); leds->led[3].cnt = 20; leds->led[3].delay[0] =  5;
  led_create(&leds->led[4], 50, 18); leds->led[4].cnt = 0;  leds->led[4].delay[0] = 20;
  leds->led[4].coul[0] = OFF   ;
  leds->led[4].coul[1] = BIGREDLIGHT;
  leds->led[4].coul[2] = BIGRED;
}

void leds_update(Leds *l)
{
  int i;
  for (i=0; i < 5; i++)
    led_update(&l->led[i]);
}

void
leds_draw(Leds *l, int state_step)
{
  int i,x;
  assert(l);
  for (i=0; i < 4; i++) {
    if (l->led[i].coul[l->led[i].colcnt] != OFF)
      XCopyArea(dock->display, l->pixmap, dock->coinpix, dock->NormalGC, 
		(l->led[i].coul[l->led[i].colcnt]-1)*9, 0, 9, 4, 
		l->led[i].xpos, l->led[i].ypos - MIN(state_step,13));
  }

  i = 4;
  if (l->led[i].coul[l->led[i].colcnt] == OFF) x = 63;
  else if (l->led[i].coul[l->led[i].colcnt] == BIGREDLIGHT) x = 74;
  else x = 85;

  if (x == 85) {
    XCopyArea(dock->display, l->pixmap, dock->coinpix, dock->NormalGC, 
	      x, 2, 11, 3, l->led[i].xpos, l->led[i].ypos + MIN(state_step,38)+2);
    XCopyArea(dock->display, l->pixmap, dock->coinpix, dock->NormalGC, 
	      x+1, 1, 10, 1, l->led[i].xpos+1, l->led[i].ypos + MIN(state_step,38)+1);
    XCopyArea(dock->display, l->pixmap, dock->coinpix, dock->NormalGC, 
	      x+2, 0, 9, 1, l->led[i].xpos+2, l->led[i].ypos + MIN(state_step,38));
  } else {
    XCopyArea(dock->display, l->pixmap, dock->coinpix, dock->NormalGC, 
	      x+2, 1, 8, 1, l->led[i].xpos+2, 1+l->led[i].ypos + MIN(state_step,38));
    XCopyArea(dock->display, l->pixmap, dock->coinpix, dock->NormalGC, 
	      x+1, 2, 9, 2, l->led[i].xpos+1, 2+l->led[i].ypos + MIN(state_step,38));
  }
}



void
refresh_msginfo(Dock *dock)
{
  if (dock->tl_item_survol) {
    if (dock->tl_item_clicked) {
      tribune_msg_info *mi;

      if (flag_updating_tribune == 0) {
	dock->tribune_updatable = 0;
      
	mi = tribune_find_id(&dock->dlfp->tribune, dock->tl_item_survol->id);
	if (mi) {
	  snprintf(dock->msginfo, MAX_MSGINFO_LEN, "    %s",
		   mi->useragent);
	  dock->msginfo[MAX_MSGINFO_LEN-1]=0;
	} else {
	  strcpy(dock->msginfo, "bug?");
	}
	dock->tribune_updatable = 1;
      } else strcpy(dock->msginfo, "plz wait");
    } else {
      if (flag_updating_tribune == 0) {
	dock->tribune_updatable = 0;
	strncpy(dock->msginfo, dock->tl_item_survol->tatouage->name, MAX_MSGINFO_LEN);
	dock->msginfo[MAX_MSGINFO_LEN-1]=0;
	if (strcmp(dock->msginfo, "?")==0 && flag_updating_tribune == 0) {
	  tribune_msg_info *mi;
	  mi = tribune_find_id(&dock->dlfp->tribune, dock->tl_item_survol->id);
	  if (mi) {
	    make_short_name_from_ua(mi->useragent, dock->msginfo, MAX_MSGINFO_LEN);
	  }
	}
	dock->tribune_updatable = 1;
      } else strcpy(dock->msginfo, "plz wait");
    }
  } else {
    if (dock->flag_survol_trollo) {
      sprintf(dock->msginfo, "level:%3d", (int)(dock->trib_trollo_rate*3));
    } else if (dock->flag_survol_led1) {
      sprintf(dock->msginfo, "vit.def:%d",dock->trolloscope_speed);
    } else if (flag_updating_tribune == 0) {
      dock->tribune_updatable = 0;
      sprintf(dock->msginfo, "%s+%02d%s", dock->tribune_time,
	      dock->dlfp->tribune.nbsec_since_last_msg,
	      dock->dlfp->tribune.nbsec_since_last_msg < 100 ? "s" : "");
      dock->tribune_updatable = 1;
    }  else {
      //      strcpy(dock->msginfo, "updating");
    }
  }
}

void
dock_refresh_normal(Dock *dock)
{
  refresh_msginfo(dock);
  switch (dock->door_state) {
  case OPENED:
  case CLOSING:
  case OPENING:
    {
      /* l'image de fond */
      XCopyArea(dock->display, dock->coinpix, dock->coinpix, dock->NormalGC, 64, 0, 64, 64, 0, 0);
      /* le bouton */
      XCopyArea(dock->display, dock->coinpix, dock->coinpix, dock->or_GC, 
		128+(dock->red_button_press_state%3)*32, (dock->red_button_press_state/3)*31, 31, 30, 16, 16);


      XSetClipMask(dock->display, dock->NormalGC, dock->mask_porte_haut);
      XSetClipOrigin(dock->display, dock->NormalGC, 0, -MIN(dock->door_state_step,13));
      XCopyArea(dock->display, dock->pix_porte, dock->coinpix, dock->NormalGC, 
		0, MIN(dock->door_state_step,13), 64, 64, 0, 0);


      XSetClipMask(dock->display, dock->NormalGC, None);
      textout_msg(dock, dock->newstitles, 3, 3-dock->door_state_step, 57);



      XSetClipMask(dock->display, dock->NormalGC, dock->mask_porte_bas);
      XSetClipOrigin(dock->display, dock->NormalGC, 0, dock->door_state_step);
      XCopyArea(dock->display, dock->pix_porte, dock->coinpix, dock->NormalGC, 
		0, 0, 64, 64-dock->door_state_step, 0, dock->door_state_step);
      XSetClipMask(dock->display, dock->NormalGC, None);
      //      textout_simple(tribune_time, 3, 49 + state_step, 5);

      if (dock->door_state_step < TRIBUNE_LOAD_HEIGHT) {
	XCopyArea(dock->display, dock->pix_tribune_load, dock->coinpix, dock->NormalGC, 
		  0, 0, TRIBUNE_LOAD_WIDTH, TRIBUNE_LOAD_HEIGHT-dock->door_state_step, TRIBUNE_LOAD_X, TRIBUNE_LOAD_Y+dock->door_state_step);
      }

      XCopyArea(dock->display, dock->pix_porte, dock->coinpix, dock->NormalGC, 
		0, 47, 64, 64-47+1, 0, 47+MAX(0,dock->door_state_step - TRIBUNE_LOAD_HEIGHT + 5));

      textout_msginfo(dock, 4, 49+MAX(0,dock->door_state_step - TRIBUNE_LOAD_HEIGHT + 5));
      
      leds_draw(&dock->leds, dock->door_state_step);

    } break;
  case CLOSED:
    {
      XCopyArea(dock->display, dock->pix_porte, dock->coinpix, dock->NormalGC, 0, 0, 64, 64, 0, 0);
      textout_msg(dock, dock->newstitles, 3, 3, 57);
      //      XCopyArea(dock->display, dock->pix_porte_bas, dock->coinpix, dock->or_GC, 0, 0, 64, 64, 0, 0);
      textout_msginfo(dock, 4, 49+MAX(0,dock->door_state_step - TRIBUNE_LOAD_HEIGHT + 5));

      XCopyArea(dock->display, dock->pix_tribune_load, dock->coinpix, dock->NormalGC, 0, 0, 
		TRIBUNE_LOAD_WIDTH, TRIBUNE_LOAD_HEIGHT, TRIBUNE_LOAD_X, TRIBUNE_LOAD_Y);

      leds_draw(&dock->leds, dock->door_state_step);



    } break;
  default:
    {
      fprintf(stderr, "coin ?.... COIN ?? couic !\n"); assert(0);
    }
  }
}

static void
dock_refresh_horloge_mode(Dock *dock)
{
//  int w;
//  char s[20];
  struct tm tm;
  time_t tnow;

  XCopyArea(dock->display, dock->clockpix, dock->coinpix, dock->NormalGC,
	    0,0, 64, 64, 0,0);
//  XSetForeground(dock->display, dock->NormalGC, IRGB2PIXEL(Prefs.bgcolor));
//  XFillRectangle(dock->display, dock->coinpix, dock->NormalGC, 2, 2, 60, 60);
//  XSetForeground(dock->display, dock->NormalGC, IRGB2PIXEL(Prefs.fgcolor));
//  XSetFont(dock->display, dock->NormalGC, dock->fixed_font->fid);

  tnow = time(NULL);
  if (dock->dlfp->tribune.last_post_id > 0) {
    time_t decal, ttribune;

    decal = difftime(tnow, dock->dlfp->tribune.local_time_last_check);
    ttribune = dock->dlfp->tribune.last_post_timestamp + decal + dock->dlfp->tribune.nbsec_since_last_msg;
    localtime_r(&ttribune, &tm);
//    snprintf(s, 20, "%02d:%02d:%02d", tm.tm_hour, tm.tm_min, tm.tm_sec);
  } else {
//    snprintf(s, 20, "...");
    localtime_r(&tnow, &tm);
  }
  
  XCopyArea(dock->display, dock->led, dock->coinpix, dock->NormalGC, 9 * (tm.tm_hour / 10), 0, 9, 11, 9, 6); 
  XCopyArea(dock->display, dock->led, dock->coinpix, dock->NormalGC, 9 * (tm.tm_hour % 10), 0, 9, 11, 18, 6); 
  if (tm.tm_sec % 2) {
    XCopyArea(dock->display, dock->led, dock->coinpix, dock->NormalGC, 90, 0, 5, 11, 27, 6); 
  }
  XCopyArea(dock->display, dock->led, dock->coinpix, dock->NormalGC, 9 * (tm.tm_min / 10), 0, 9, 11, 32, 6); 
  XCopyArea(dock->display, dock->led, dock->coinpix, dock->NormalGC, 9 * (tm.tm_min % 10), 0, 9, 11, 41, 6); 
  XCopyArea(dock->display, dock->month, dock->coinpix, dock->NormalGC, 0, 6 * tm.tm_mon, 22, 6, 18, 48);
  XCopyArea(dock->display, dock->weekday, dock->coinpix, dock->NormalGC, 0, 6 * ((tm.tm_wday +6) % 7), 20, 6, 21, 24);
  if (tm.tm_mday > 9) {
    XCopyArea(dock->display, dock->date, dock->coinpix, dock->NormalGC, 9 * (tm.tm_mday / 10), 0, 8, 13, 22, 33);
    XCopyArea(dock->display, dock->date, dock->coinpix, dock->NormalGC, 9 * (tm.tm_mday % 10), 0, 8, 13, 31, 33);
  } else {
    XCopyArea(dock->display, dock->date, dock->coinpix, dock->NormalGC, 9 * tm.tm_mday, 0, 8, 13, 26, 33);
  }
//  w = XTextWidth(dock->fixed_font, s, strlen(s));
//  XDrawString(dock->display, dock->coinpix, dock->NormalGC, 32-w/2, 32, s, strlen(s));
}


/* 
   poste le message sur la tribune -- en tant que fonction 'lente' 
   cette fonction est executée par la boucle principale
 */
void
exec_coin_coin(Dock *dock)
{
  SOCKET fd;
  char s[2048], referer[2048], path[2048];
  char *urlencod_msg;
  char cookie[200];

  BLAHBLAH(1, myprintf("message posté: '%<YEL %s>\n", dock->coin_coin_message));
  BLAHBLAH(1, myprintf("   (useragent: '%<CYA %s>\n", dock->coin_coin_useragent));
  flag_sending_coin_coin = 1;
  urlencod_msg = http_url_encode(dock->coin_coin_message); assert(urlencod_msg);
  snprintf(s, 2048, "message=%s", urlencod_msg);
  free(urlencod_msg);
  snprintf(referer, 2048,"http://%s:%d/%s/", Prefs.site_root, Prefs.site_port, Prefs.site_path);
  snprintf(path, 2048, "%s%s/%s", strlen(Prefs.site_path) ? "/" : "", Prefs.site_path, Prefs.path_tribune_add);
  if (Prefs.user_cookie && dock->post_anonyme == 0) { /* merci glandium !! */
     snprintf(cookie, 200, "session_id=%s", Prefs.user_cookie);
  } else cookie[0] = 0;
  fd = http_post_with_cookie(Prefs.site_root, Prefs.site_port, path,
			     Prefs.proxy_name, Prefs.proxy_auth, Prefs.proxy_port, dock->coin_coin_useragent,
			     referer, cookie, s);


  if (fd != INVALID_SOCKET) {
    int got;
    
    if (http_skip_header(fd) < 0) {
      /* si la reponse n'est pas un 302 Found */
      snprintf(s, 2048, "Damned ! y'a une pouille dans le cotage<p>%s", http_error());
      msgbox_show(dock, s);
    } else {
      int l;
      BLAHBLAH(1, myprintf(" --> OK, message envoye avec succes\n"));

      strcpy(s, "Ouups, il y a sans doute eu un petit probleme, <b>LinuxFr</b> a répondu:<p>");
      l = strlen(s);
      /* test si la reponse est du style 'pas 2 msg a la suite',
	 'vous etes blackliste', ou 'mot interdit', ou autre .. */
      if ((got=http_iread(fd, s+l, 2047-l))>0) {
	s[l+got] = 0;
      //      myprintf("REPONSE RECUE: '%<YEL %s>'\n", s);
	if (got) {
	  msgbox_show(dock, s);
	}
      }
    }
    http_close(fd);

  } else {
    snprintf(s, 2048, "Erreur pendant l'envoi du message: <p><b>%s</b>\n", http_error());
    msgbox_show(dock, s);
  }
  flag_sending_coin_coin = 0;
}

static void
check_cursor_shape(Dock *dock, int x, int y)
{
  if (x >= TRIBUNE_LOAD_X && x < TRIBUNE_LOAD_X+TRIBUNE_LOAD_WIDTH &&
      y >= TRIBUNE_LOAD_Y && y < TRIBUNE_LOAD_Y+TRIBUNE_LOAD_HEIGHT) {
    if (dock->flag_trib_load_cursor == 0) {
      XDefineCursor(dock->display, DOCK_WIN(dock), dock->trib_load_cursor);
      XSync(dock->display, False);
      dock->flag_trib_load_cursor = 1;
    }
  } else if (dock->flag_trib_load_cursor == 1) {
    XUndefineCursor(dock->display, DOCK_WIN(dock));
    XSync(dock->display, False);
    dock->flag_trib_load_cursor = 0;
  }
}

static void
dock_handle_motion_notify(Dock *dock, int x, int y)
{
  int oldid;

  if (dock->tl_item_survol) {
    oldid = dock->tl_item_survol->id;
  } else oldid = -1;
  
  dock->tl_item_survol = NULL;
  check_cursor_shape(dock, x,y);
  if (dock->door_state == CLOSED) {
    /* 
       survol de la tribune_load ? 
	*/
	if (x >= TRIBUNE_LOAD_X && x < TRIBUNE_LOAD_X+TRIBUNE_LOAD_WIDTH &&
	    y >= TRIBUNE_LOAD_Y && y < TRIBUNE_LOAD_Y+TRIBUNE_LOAD_HEIGHT) {
	  int i,j;
	  

	  j = (TRIBUNE_LOAD_WIDTH + TRIBUNE_LOAD_X - 1 - x) / tribune_resolution;
	  i = (TRIBUNE_LOAD_HEIGHT + TRIBUNE_LOAD_Y - 1 - y) / tribune_resolution;

	  //	  printf("\r%d, %d %d %d", event->xmotion.x, event->xmotion.y, i, j); fflush(stdout);
	  
	  if (tribune_load[i][j].id > 0) {
	    dock->tl_item_survol = &tribune_load[i][j];
	    if (tribune_load[i][j].id != oldid) {
	      dock->tl_item_clicked = 0;
	      dock->msginfo_defil = 0;
	    }
	    dock->view_id_in_newstitles = dock->tl_item_survol->id;
	    dock->view_id_timer_cnt = 0;
	    //	    myprintf("i = %d, j=%d, id = %d, nom = %s\n", i,j, tribune_load[i][j].id,tribune_load[i][j].tatouage->name);
	  } else {
	    dock->tl_item_clicked = 0;
	    dock->msginfo_defil = 0;
	    dock->view_id_in_newstitles = 0;
	  }
	} else {
	  dock->tl_item_clicked = 0;
	  dock->msginfo_defil = 0;
	  dock->view_id_in_newstitles = 0;
	}
	if (IS_INSIDE(x,y,50,18,60,22)) {
	  /* survol du trollometre */
	  dock->flag_survol_trollo = 1;
	} else {
	  dock->flag_survol_trollo = 0;
	}
	if (IS_INSIDE(x,y,dock->leds.led[1].xpos,dock->leds.led[1].ypos - MIN(dock->door_state_step,13),
		      dock->leds.led[1].xpos+8, dock->leds.led[1].ypos +3 - MIN(dock->door_state_step,13))) {
	  /* survol de la led 1 */
	  //	  printf("survol led1\n");
	  dock->flag_survol_led1 = 1;
	} else {
	  dock->flag_survol_led1 = 0;
	}
      }
      //      printf("DEBUG MOTION: item survol %p)\n", dock->tl_item_survol);
}

/* renvoie 1 si le bouton rouge a ete suffisament enfonce */
int
dock_red_button_check(Dock *dock) {
  if (dock->red_button_press_flag) {
    /* si on a appuye assez fort ... */
    if (dock->red_button_press_state == 5) {
      BLAHBLAH(1,printf("Coin !\n"));
      dock->coin_coin_request = 1;
      //	  exec_coin_coin();
    }
    
    dock->red_button_press_flag = -1;
    
    //    if (dock->door_state != CLOSED) dock->door_state = CLOSING;
  }
  return dock->coin_coin_request;
}

void
dock_show_tribune_frequentation(Dock *dock)
{
  char s[2048], s_xp[512], sv_xp[10], sv_xp_old[50];
  int ua_cnt1, ua_cnt2, ua_cnt3, ua_cnt4;
  int msg_cnt1, msg_cnt2, msg_cnt3, msg_cnt4;


  tribune_frequentation(&dock->dlfp->tribune, 10, &ua_cnt1, &msg_cnt1);
  tribune_frequentation(&dock->dlfp->tribune, 30, &ua_cnt2, &msg_cnt2);
  tribune_frequentation(&dock->dlfp->tribune, 120, &ua_cnt3, &msg_cnt3);
  tribune_frequentation(&dock->dlfp->tribune, 8*60, &ua_cnt4, &msg_cnt4);

  if (Prefs.user_cookie) {
    if (dock->dlfp->xp > -1000) {
      snprintf(sv_xp,10,"%d", dock->dlfp->xp);
    } else snprintf(sv_xp, 10, "???");
    if (dock->dlfp->xp_old > -1000) {
      snprintf(sv_xp_old,50,"(ancienne valeur: %d)",dock->dlfp->xp_old);
    } else sv_xp_old[0] = 0;

    snprintf(s_xp, 512, "Vous avez <b><font color=blue>%s</font></b>"
	     " XP %s  [votes:%d/%d]<br><br>", 
	     sv_xp, sv_xp_old, dock->dlfp->votes_cur, dock->dlfp->votes_max);
  } else {
    s_xp[0] = 0;
    //    snprintf(s_xp, 512, "<p align=center>(vous n'etes pas loggé)<br>");
  }
  
  snprintf(s, 2048, 
	   "%s"
	   "<p align=center><b>Fréquentation de la Tribune</b><br>"
	   "<i>(estimation basée sur les useragents)</i></p><br>"
	   "depuis:<br>"
	   ".<tab><i>10 minutes</i>: <tab><font color=blue>%d</font><tab> personnes ont posté <tab><font color=blue>%d</font> messages<br>"
	   ".<tab><i>30 minutes</i>: <tab><font color=blue>%d</font><tab> personnes ont posté <tab><font color=blue>%d</font> messages<br>"
	   ".<tab><i> 2 heures </i>: <tab><tab><font color=blue>%d</font><tab> personnes ont posté <tab><font color=blue>%d</font> messages<br>"
	   ".<tab><i> 8 heures </i>: <tab><tab><font color=blue>%d</font><tab> personnes ont posté <tab><font color=blue>%d</font> messages<br>",
	   s_xp, ua_cnt1, msg_cnt1, ua_cnt2, msg_cnt2, ua_cnt3, msg_cnt3, ua_cnt4, msg_cnt4);

  msgbox_show(dock, s);
}

void
dock_set_horloge_mode(Dock *dock) {
  if (Prefs.draw_border == 0) {
    XShapeCombineMask(dock->display, dock->win, ShapeBounding, 
		      0, 0, dock->clock_pixmask, ShapeSet);
    XShapeCombineMask(dock->display, dock->iconwin, ShapeBounding,
		      0, 0, dock->clock_pixmask, ShapeSet);
  }
  dock->horloge_mode = 1;
}

void
dock_unset_horloge_mode(Dock *dock) {
  if (Prefs.draw_border == 0) {
    XShapeCombineMask(dock->display, dock->win, ShapeBounding, 
	  	      0, 0, dock->coin_pixmask, ShapeSet);
    XShapeCombineMask(dock->display, dock->iconwin, ShapeBounding,
		      0, 0, dock->coin_pixmask, ShapeSet);
  }
  dock->horloge_mode = 0;  
}

void
dock_handle_button_press(Dock *dock, XButtonEvent *xbevent)
{
  int x,y;
  x = xbevent->x; y = xbevent->y;


  /* gestion d'un bug curieux de KDE/swallow:
	 un LeaveNotify est genere quand on clique sur le dock...
	 du coup ca reinitialise plein de choses.. */
  dock_handle_motion_notify(dock, x, y);

  //      printf("DEBUG: item survol %p)\n", dock->tl_item_survol);

  /* Ctrl-Clic1 -> passage en mode horloge */
  if (xbevent->button == Button1 && (xbevent->state & ControlMask)) {
    dock_set_horloge_mode(dock);
  } else if (xbevent->button == Button1) {
    if (IS_INSIDE(x,y,20,20,40,40) && 
	(dock->door_state == OPENED || (dock->door_state_step>12))) {
      dock->red_button_press_flag = 1;
    } else if (IS_INSIDE(x,y,50,18,60,22) && 
	       (dock->door_state == CLOSED)) {
      /* clic sur le trollometre */
      dock->door_state = OPENING;

    } else if (IS_INSIDE(x,y,2,2,59,13) && 
	       (dock->door_state == CLOSED)) {
      int id;
      /* clic gauche sur la zone des news défilantes -> on affiche la news et on 'raise' la fenetre */
      BLAHBLAH(4,printf("newswin_show\n"));
      if (strlen(dock->newstitles)) {
	int pos;
	pos = (dock->newstitles_pos + ((xbevent->x - 7)+3)/6) % strlen(dock->newstitles);
	id = dock->newstitles_id[pos];
      } else { id = -1; }
      newswin_show(dock, dock->dlfp, id); XRaiseWindow(dock->display, newswin_get_window(dock));
    } else if (IS_INSIDE(x,y,dock->leds.led[0].xpos,dock->leds.led[0].ypos - MIN(dock->door_state_step,13),
			 dock->leds.led[0].xpos+8, dock->leds.led[0].ypos +3 - MIN(dock->door_state_step,13))) {
      /*
	clic bouton 1 sur la 1ere led ->
	voir la derniere erreur http
      */
      if (flag_http_error) {
	msgbox_show(dock, http_error());
      } else msgbox_show(dock, "no problemo");
    }else if (IS_INSIDE(x,y,dock->leds.led[1].xpos,dock->leds.led[1].ypos - MIN(dock->door_state_step,13),
			dock->leds.led[1].xpos+8, dock->leds.led[1].ypos +3 - MIN(dock->door_state_step,13))) {
      /*
	clic bouton 1 sur la 2eme led ->
	accelere le trolloscope
      */
      dock->trolloscope_speed /= 2;
      if (dock->trolloscope_speed <= 0) dock->trolloscope_speed = 1;
      update_pix_tribune_load(dock, &dock->dlfp->tribune);
    } else if (IS_INSIDE(x,y,dock->leds.led[2].xpos,dock->leds.led[2].ypos - MIN(dock->door_state_step,13),
			 dock->leds.led[2].xpos+8, dock->leds.led[2].ypos +3 - MIN(dock->door_state_step,13))) {
      /*
	click bouton 1 sur la 3eme led -> 
	voir les news non lues
      */
      int id;
      id = dlfp_unreaded_news_id(dock->dlfp, 1);
      if (id > 0) {
	newswin_show(dock, dock->dlfp, id);
      }
    } else if (IS_INSIDE(x,y,dock->leds.led[3].xpos,dock->leds.led[3].ypos - MIN(dock->door_state_step,13),
			 dock->leds.led[3].xpos+8, dock->leds.led[3].ypos +3 - MIN(dock->door_state_step,13))
	       && flag_updating_messagerie == 0) {
      /* clic gauche sur la 4eme led -> voir les nouveaux messages (avec le browser 1)*/
      DLFP_message *m;
      m = dlfp_msg_find_unreaded(dock->dlfp);
      if (m) {
	char url[2048];
	snprintf(url, 2048, "http://%s:%d/%s%smessages/view.php3?id=%d", 
		 Prefs.site_root, Prefs.site_port, Prefs.site_path, strlen(Prefs.site_path) ? "/" : "", 
		 m->mid);
	open_url(url, x, y, 1);
	m->unreaded = 0;
      } else {
	msgbox_show(dock, "pas de nouveaux messages");
      }
    } else if (IS_INSIDE(x,y,TRIBUNE_LOAD_X, TRIBUNE_LOAD_Y,
			 TRIBUNE_LOAD_X+TRIBUNE_LOAD_WIDTH-1,TRIBUNE_LOAD_Y+TRIBUNE_LOAD_HEIGHT-1) &&
	       dock->door_state == CLOSED) {
      /* 
	 click bouton 1 sur un element de tribune_load valide 
	 on va faire defiler son useragent en bas, et faire
	 clignoter en alternance son id et l'heure dans la zone
	 de texte du haut

	 SI tribune_load clignote (detection de nouveau commentaire), on l'efface
      */
	  
      if (flag_updating_comments == 0) {
	DLFP_comment *com;
	if ((com = dlfp_yc_find_modified(dock->dlfp,NULL))) {
	  char url[1024];
	  snprintf(url, 1024, "http://%s:%d/%s%scomments/thread.php3?news_id=%d&com_id=%d",
		   Prefs.site_root, Prefs.site_port, Prefs.site_path, strlen(Prefs.site_path) ? "/" : "", 
		   com->news_id, com->com_id);
	  open_url(url, -1, -1, 1);
	  com->modified = 0;
	  dlfp_yc_clear_modified(dock->dlfp);
	  if (dlfp_yc_find_modified(dock->dlfp,NULL) == NULL)
	    tribune_load_clign_step = -2;
	} else if (dock->dlfp->xp_clign_cnt >= 0) {
	  dock->dlfp->xp_clign_cnt = -1;
	  tribune_load_clign_step = -2;
	}
      }
      if (dock->tl_item_survol) {
	dock->tl_item_clicked = 1;
	dock->msginfo_defil = 1;
      }
    } else if (IS_INSIDE(x,y,3,49,3+57,49+12) && 
	       (dock->door_state_step <= TRIBUNE_LOAD_HEIGHT)) {
      if (!editw_ismapped(dock->editw)) {
	editw_show(dock, dock->editw, 0);
      } else {
	editw_hide(dock, dock->editw);
      }
      //msgbox_show(dock, "<b>essai de msgbox</b>");
      //	  balloon_show(dock, 1600, 0, 1,1, "essai<br>je suis <b>bien</b><i>content</i> c'est vraiment surper djkdhfds dgjkfdgjgjkfdg dfgjkhfd");
    }

    /********************************************* BOUTON DROIT ****************************/
  } else if (xbevent->button == Button3) {
    if (IS_INSIDE(x,y,2,2,59,13) && 
	(dock->door_state == CLOSED)) {
      /* clic bouton droite sur la zone défilante -> fermeture la fenetre des news */
      if (newswin_is_used(dock)) {
	newswin_unmap(dock);
      } else newswin_show(dock, dock->dlfp, -2);
    } else if (IS_INSIDE(x,y,TRIBUNE_LOAD_X, TRIBUNE_LOAD_Y,
			 TRIBUNE_LOAD_X+TRIBUNE_LOAD_WIDTH-1,TRIBUNE_LOAD_Y+TRIBUNE_LOAD_HEIGHT-1) &&
	       dock->door_state == CLOSED) {
      /* 
	     clic bouton de droite dans tribune_load
	     -> le comportement a été modifié, le chagement de resolution est crétin et inutile
	     ->nouveau comportement:
         	     SI tribune_load clignote (detection de nouveau commentaire), on l'efface
	  */
	  
      if (flag_updating_comments == 0) {
	if (dlfp_yc_find_modified(dock->dlfp,NULL)) {
	  dlfp_yc_clear_modified(dock->dlfp);
	  tribune_load_clign_step = -2;
	}
      }
    } else if (IS_INSIDE(x,y,dock->leds.led[1].xpos,dock->leds.led[1].ypos - MIN(dock->door_state_step,13),
			 dock->leds.led[1].xpos+8, dock->leds.led[1].ypos +3 - MIN(dock->door_state_step,13))) {
      /*
	    click bouton droite sur la 2eme led -> 
	    ralentir le trolloscope
	  */
      dock->trolloscope_speed *= 2;
      if (dock->trolloscope_speed <= 0 || dock->trolloscope_speed > 128) dock->trolloscope_speed = 128;
      update_pix_tribune_load(dock, &dock->dlfp->tribune);
	  
    }  else if (IS_INSIDE(x,y,dock->leds.led[2].xpos,dock->leds.led[2].ypos - MIN(dock->door_state_step,13),
			  dock->leds.led[2].xpos+8, dock->leds.led[2].ypos +3 - MIN(dock->door_state_step,13))) {
      /*
	    click bouton droite sur la 3eme led -> 
	     eteindre la diode
	  */
      if (flag_updating_news == 0) {
	dlfp_unset_unreaded_news(dock->dlfp);
	if (newswin_is_used(dock)) { newswin_update_content(dock, dock->dlfp, 0); newswin_draw(dock); }
      }
    } else if (IS_INSIDE(x,y,dock->leds.led[3].xpos,dock->leds.led[3].ypos - MIN(dock->door_state_step,13),
			 dock->leds.led[3].xpos+8, dock->leds.led[3].ypos +3 - MIN(dock->door_state_step,13))
	       && flag_updating_messagerie == 0) {
      /* clic droite sur la 4eme led -> annuler les nouveaux messages */
      DLFP_message *m;
      while ((m = dlfp_msg_find_unreaded(dock->dlfp))) m->unreaded = 0;
    } else  if (IS_INSIDE(x,y,50,18,60,22) && 
		(dock->door_state == CLOSED)) {
      /* bouton 3 sur le trollometre:
	     on montre la frequentation de la tribune */
      dock_show_tribune_frequentation(dock);
    } else if (IS_INSIDE(x,y,3,49,3+57,49+12) && 
	       (dock->door_state_step <= TRIBUNE_LOAD_HEIGHT)) {
      /* montre le pinnipede teletype */
      if (!pp_ismapped(dock)) {
	if (flag_updating_tribune == 0) {
	  pp_show(dock, &dock->dlfp->tribune);
	}
      } else {
	pp_unmap(dock);
      }
    }

    /************************** BOUTON MILIEU ****************************************************/
  } else if (xbevent->button == Button2) {
    /*
	  demande de rafraichissement de la tribune ?
	  (bouton milieu dans la tribune_load)
	*/
    if (IS_INSIDE(x,y,2,2,59,13) && 
	(dock->door_state == CLOSED)) {
      dock->news_update_request = 1;
    } else if (IS_INSIDE(x,y,TRIBUNE_LOAD_X, TRIBUNE_LOAD_Y,
			 TRIBUNE_LOAD_X+TRIBUNE_LOAD_WIDTH-1,TRIBUNE_LOAD_Y+TRIBUNE_LOAD_HEIGHT-1) &&
	       dock->door_state == CLOSED) {
      /* rafraichissement des news */
      dock->tribune_update_request = 1;
    } else if (IS_INSIDE(x,y,dock->leds.led[1].xpos,dock->leds.led[1].ypos - MIN(dock->door_state_step,13),
			 dock->leds.led[1].xpos+8, dock->leds.led[1].ypos +3 - MIN(dock->door_state_step,13))) {
      /* clic milieu sur la deuxieme led -> changement de la resolution du trolloscope */
      switch (tribune_resolution) {
      case 5: tribune_resolution = 2; break;
      case 2: tribune_resolution = 1; break;
      case 1: tribune_resolution = 5; break;
      default: assert(0); break;	  
      }	  
      dock->tl_item_clicked = 0;
      dock->msginfo_defil = 0;
      update_pix_tribune_load(dock, &dock->dlfp->tribune);
    } else if (IS_INSIDE(x,y,dock->leds.led[3].xpos,dock->leds.led[3].ypos - MIN(dock->door_state_step,13),
			 dock->leds.led[3].xpos+8, dock->leds.led[3].ypos +3 - MIN(dock->door_state_step,13))
	       && flag_updating_messagerie == 0) {
      /* clic milieu sur la 4eme led -> voir les nouveaux messages (avec le browser 2)*/
      DLFP_message *m;
      m = dlfp_msg_find_unreaded(dock->dlfp);
      if (m) {
	char url[2048];
	snprintf(url, 2048, "http://%s:%d/%s%smessages/view.php3?id=%d", 
		 Prefs.site_root, Prefs.site_port, Prefs.site_path, strlen(Prefs.site_path) ? "/" : "", 
		 m->mid);
	open_url(url, x, y, 2);
	m->unreaded = 0;
      } else {
	msgbox_show(dock, "pas de nouveaux messages");
      }
    } else if (IS_INSIDE(x,y,50,18,60,22) && 
	       (dock->door_state == CLOSED)) {
      /* bouton milieu sur le trollometre:
	     relire le .wmcoincoin/useragents
	  */
      int err;
      err = reread_rcfile(dock->dlfp);
      BLAHBLAH(2, printf("relecture RCfile -> code d'erreur renvoye: %d\n", err));
    }
  }
}

void
dock_handle_button_press_horloge_mode(Dock *dock, XButtonEvent *xbevent)
{
  int x,y;
  x = xbevent->x; y = xbevent->y;
  
  if (xbevent->button == Button1 && (xbevent->state & ControlMask)) {
    dock_unset_horloge_mode(dock);
  }
}

void
dock_dispatch_event(Dock *dock, XEvent *event)
{
  switch (event->type) {
  case DestroyNotify: /* OH MON DIEU ! ils ont tué coincoin ! */
    {
      XCloseDisplay(dock->display);
    
      exit(0);
    } break;
  case ButtonPress:
    {
      if (dock->horloge_mode == 0) {
	dock_handle_button_press(dock, &event->xbutton);
      } else {
	dock_handle_button_press_horloge_mode(dock, &event->xbutton);
      }
    } break;
  case ButtonRelease:
    {
      if (dock->horloge_mode == 0) dock_red_button_check(dock);
    } break;
  case MotionNotify:
    {
      if (dock->horloge_mode == 0) dock_handle_motion_notify(dock, event->xmotion.x, event->xmotion.y);
    } break;
  case EnterNotify:
    {
      int x,y;
      x = event->xcrossing.x;
      y = event->xcrossing.y;
      if (dock->horloge_mode == 0) check_cursor_shape(dock, x, y);
    } break;
  case LeaveNotify:
    {
      //printf("DEBUG: leave %p ->send_event=%d\n", dock->tl_item_survol, event->xcrossing.send_event);

      check_cursor_shape(dock, -1,-1);
      dock->tl_item_survol = NULL;
      dock->tl_item_clicked = 0;
      dock->view_id_in_newstitles = 0;
      dock->flag_survol_trollo = 0;
      dock->flag_survol_led1 = 0;

      if (dock->door_state == OPENED || dock->door_state == OPENING) dock->door_state = CLOSING;
    } break;
  }
}

void
set_leds_state(Dock *dock)
{
  int trollo_hrate;

  if (flag_troll_braining) {
    led_color(&dock->leds.led[0], VIOLET, VIOLET, VIOLET);
  } else if (flag_http_transfert) {
    led_color(&dock->leds.led[0], BLUE, BLUE, BLUE);
  } else if (flag_http_error) {
    led_color(&dock->leds.led[0], OFF, OFF, RED);
  } else if (flag_updating_news || flag_updating_tribune) {
    led_color(&dock->leds.led[0], CYAN, CYAN, CYAN);
  } else {
    led_color(&dock->leds.led[0], OFF, OFF, OFF);
  }

  /*
  if (dock->door_state == OPENING || dock->door_state == CLOSING) {
    led_color(&dock->leds.led[1], YELLOW, YELLOW, YELLOW);
  } else {
    if (dock->door_state == OPENED) {
      led_color(&dock->leds.led[1], OFF, YELLOW, YELLOW);
    } else {
      led_color(&dock->leds.led[1], OFF, OFF, OFF);
    }
  }
  */
  if (flag_sending_coin_coin) {
    led_color(&dock->leds.led[1], YELLOW, YELLOW, YELLOW);
  } else if (dock->coin_coin_request > 0) {
    led_color(&dock->leds.led[1], OFF, YELLOW, YELLOW);    
  } else if (dock->coin_coin_request < 0) {
    led_color(&dock->leds.led[1], GREEN, GREEN, GREEN);
  } else {
    led_color(&dock->leds.led[1], OFF, OFF, OFF);
  }

  if (dlfp_unreaded_news_id(dock->dlfp, 1)>0) {
    led_color(&dock->leds.led[2], OFF, GREENLIGHT, GREEN);
  } else {
    led_color(&dock->leds.led[2], OFF, OFF, OFF);
  }


  if (flag_updating_messagerie) {
    led_color(&dock->leds.led[3], VIOLET, VIOLET, VIOLET);
  } else if (dlfp_msg_find_unreaded(dock->dlfp)) {
    led_color(&dock->leds.led[3], OFF, GREENLIGHT, GREEN);
  } else {
    led_color(&dock->leds.led[3], OFF, OFF, OFF);
  }

  /* rapidite du clignotement du trollometre */
  trollo_hrate = ((int)(dock->trib_trollo_rate*60));
  if (trollo_hrate < 10) {
    /* moins de 10 message / heure -> trollo OFF */
    if (dock->leds.led[4].delay[2] != 0) {
      BLAHBLAH(1, myprintf("%<YEL Troll-o-metre -> passage au niveau> %<GRN 0.00> (max=1.) !!!\n"));
      BLAHBLAH(1, myprintf("%<YEL> trollo_hrate = %d\n", trollo_hrate));
    }
    dock->leds.led[4].delay[1] = 0;
    dock->leds.led[4].delay[2] = 0;
  } else {
    /* moins de 3msg/5min */
    if (trollo_hrate < 36) {
      dock->leds.led[4].delay[0] = 65;
      dock->leds.led[4].delay[1] = 1;
      dock->leds.led[4].delay[2] = 1;
      /*1 msg/min */
    } else if (trollo_hrate < 60) {
      dock->leds.led[4].delay[0] = 30;
      dock->leds.led[4].delay[1] = 1;
      dock->leds.led[4].delay[2] = 1;
      /*2 msg/min */
    } else if (trollo_hrate < 120) {
      dock->leds.led[4].delay[0] = 16;
      dock->leds.led[4].delay[1] = 2;
      dock->leds.led[4].delay[2] = 2;
      /*3 msg/min */
    } else if (trollo_hrate < 180) {
      dock->leds.led[4].delay[0] = 8;
      dock->leds.led[4].delay[1] = 2;
      dock->leds.led[4].delay[2] = 2;
      /*+ de 3 msg/min */
    } else if (trollo_hrate < 60) {
      dock->leds.led[4].delay[0] = 2;
      dock->leds.led[4].delay[1] = 2;
      dock->leds.led[4].delay[2] = 2;
    }
  }
}

/*
il parait que ce type de chose est Mal(tm)
int 
flush_expose(Window w) {
  XEvent dummy;
  int i=0;

  while (XCheckTypedWindowEvent(dock->display, w, Expose, &dummy))
    i++;
  return i;
}
*/

/* declenchement des ballons d'aide...*/


void check_balloons(Dock *dock) 
{
#ifndef DISABLE_BALLOONS
  if (dock->mouse_cnt >= 2000 && dock->mouse_win != None && !balloon_ismapped(dock)) {
    int x,y;

    x = dock->mouse_x; y = dock->mouse_y;
    if (dock->mouse_win == dock->win || dock->mouse_win == dock->iconwin) {
      int iconx, icony;
      dock_get_icon_pos(dock, &iconx, &icony);

      if (dock->door_state == CLOSED && dock->horloge_mode == 0) {
	char s[2048];
	snprintf(s,2048,"<p align=center><b>Titres des news de <font color=blue>%s</font></b></p>"
		 "<font color=blue><tt>Click Gauche</tt></font><tab>: afficher l'article qui défile<br>"
		 "<font color=blue><tt>Click Milieu</tt></font><tab>: mise à jour immédiate de la liste des news<br>"
		 "<font color=blue><tt>Click Droite</tt></font><tab>: ouvrir/fermer la fenêtre des news<br>",
		 Prefs.site_root);
	balloon_test(dock,x,y,iconx,icony,0,3,3,57,11,
		     s);
	balloon_test(dock,x,y,iconx,icony,0,TRIBUNE_LOAD_X,TRIBUNE_LOAD_Y,TRIBUNE_LOAD_WIDTH,TRIBUNE_LOAD_HEIGHT,
		     "<p align=center><b>Ceci est un <font color=#a00000><i>Trolloscope</i></font> professionel</b></p>"
		     "Il affiche des symboles correspondant aux user-agents des derniers messages postés sur la tribune."
		     "Les associations useragent->(couleur,symbole) sont définies dans le fichier <tt>~/.wmcoincoin/useragents</tt><br>"
		     "<font color=blue><tt>Click Gauche</tt></font><tab>: affiche un useragent<br>"
		     "<font color=blue><tt>Click Milieu</tt></font><tab>: mise à jour immédiate<br>"
		     "remarque: si vous avez prêté votre cookie d'authentification à wmCoinCoin et que celui-ci "
		     "clignote en rouge, il vous indique qu'on a répondu à un de vos commentaire (non anonyme, bien sur) "
		     "Pour voir les réponses: <font color=blue><tt>Click Gauche</tt></font> (à condition d'avoir configuré le browser utilisé par "
		     "wmcoincoin). Pour arrêter le clignotement, un <font color=blue><tt>Click Droite</tt></font> suffit. Le clignotement vert "
		     "indique un changement de la valeur de vos XP");
	balloon_test(dock,x,y,iconx,icony,0,dock->leds.led[0].xpos, dock->leds.led[0].ypos, 9, 4,
		     "Lorsque cette led est bleue, un <b>transfert http</b> depuis linuxfr est en cours."
		     "Un clignotement <b><font color=red>rouge</font></b> indique un probleme au cours du dernier transfert<br>"
		     "<font color=blue><tt>Click Gauche</tt></font>: affiche dernier message d'erreur reçu<br>");
	balloon_test(dock,x,y,iconx,icony,0,dock->leds.led[1].xpos, dock->leds.led[1].ypos, 9, 4,
		     "Cette led clignote lorsque vous venez de poster un message sur la tribune, et que celui est en attente<br>."
		     "Elle est bleue lorsque le message est en cours d'envoi, et verte une demi-seconde après son envoi.<br>"
		     "Un clic sur cette led permet de changer de la vitesse de defilement du trolloscope:<br>"
		     "<font color=blue><tt>Click Gauche</tt></font><tab>: moins vite<br>"
		     "<font color=blue><tt>Click Droit</tt></font><tab>: plus vite<br>"
		     "<font color=blue><tt>Click Milieu</tt></font><tab>: changer la résolution du trolloscope");
	balloon_test(dock,x,y,iconx,icony,0,dock->leds.led[2].xpos, dock->leds.led[2].ypos, 9, 4,
		     "Quand cette led clignote en vert, un nouvelle article est disponible<br>"
		     "<font color=blue><tt>Click Gauche</tt></font><tab>: voir l'article<br>"
		     "<font color=blue><tt>Click Droit</tt></font><tab>: marquer tous les articles comme lus");
	balloon_test(dock,x,y,iconx,icony,0,dock->leds.led[3].xpos, dock->leds.led[3].ypos, 9, 4,
		     "Quand cette led clignote en vert, vous venez de recevoir un nouveau message<br>"
		     "<font color=blue><tt>Click Gauche</tt></font>: voir le nouveau message avec le browser externe 1<br>"
		     "<font color=blue><tt>Click Milieu</tt></font>: voir le nouveau message avec le browser externe 2<br>"
		     "<font color=blue><tt>Click Droite</tt></font>: annuler les nouveaux messages");
	balloon_test(dock,x,y,iconx,icony,0,dock->leds.led[4].xpos, dock->leds.led[4].ypos, 11, 5,
		     "Quand le curseur de la souris est au-dessus cette led, le niveau du <font color=#a00000><i>Trollometre</i></font> est affiché<br>"
		     "<font color=blue><tt>Click Gauche</tt></font><tab>: INVOQUER <b>C01N C01N</b> !<br>"
		     "<font color=blue><tt>Click Droit</tt></font><tab>: voir les stats de fréquentation de la tribune, ainsi que vos XP/votes (si vous avez fourni votre cookie d'authentification)<br>"
		     "<font color=blue><tt>Click Milieu</tt></font>: demande de relecture de <tt>~/.wmcoincoin/useragents</tt>");
	balloon_test(dock,x,y,iconx,icony,0,3,49,57,12,
		     "Heure du dernier message reçu sur la tribune, ainsi que le nombre de secondes qui se sont écoulées depuis<br>"
		     "<font color=blue><tt>Click Gauche</tt></font>: montrer/cacher le <b>palmipède editor</b><br>"
		     "<font color=blue><tt>Click Droit</tt></font>: montrer/cacher le <b>pinnipède teletype</b>");

      } else if (dock->door_state == OPENED && dock->horloge_mode == 0) {
	balloon_test(dock,x,y,iconx,icony,4000,31,30,16,16,
		 "<b><i>DON'T PANIC</i></b>");
      }
    } else if (newswin_is_used(dock) && dock->mouse_win == newswin_get_window(dock)) {
    } else if (editw_ismapped(dock->editw) && dock->mouse_win == editw_get_win(dock->editw)) {
      editw_balloon_test(dock, dock->editw, x, y);
    }
  }
#endif
}


void
wmcoincoin_dispatch_events(Dock *dock)
{
  XEvent event;

  while (XPending(dock->display)) {
    XNextEvent(dock->display, &event);
    
    /* des que la souris bouge, le ballon disparait */
    balloon_check_event(dock, &event);
    
    /* gestion de la souris (pour le declenchement des ballons) 
       + on ramene le focus dans editw (quand elle est active) */
    switch (event.type) {
    case EnterNotify: 
      {
	dock->mouse_x = event.xcrossing.x;
	dock->mouse_y = event.xcrossing.y;
	dock->mouse_cnt = 0;
	dock->mouse_win = event.xany.window;
	if (editw_ismapped(dock->editw))
	  editw_set_kbfocus(dock, dock->editw, 1);
      } break;
    case MotionNotify:
      {
	dock->mouse_x = event.xmotion.x;
	dock->mouse_y = event.xmotion.y;
	dock->mouse_cnt = 0;
	dock->mouse_win = event.xany.window;
      } break;
    case LeaveNotify:
      {
	dock->mouse_x = -1;
	dock->mouse_y = -1;
	dock->mouse_cnt = 0;
	dock->mouse_win = None;
      } break;
    case ButtonPress:
    case ButtonRelease:
    case KeyPress:
      {
	dock->mouse_cnt = 0;
      } break;
    }

    /* attention le champ window n'est pas utilise pour les evenement du clavier 
       gare au bug */
    if (event.xany.window == dock->iconwin || event.xany.window == dock->win) {
      dock_dispatch_event(dock, &event);
    } else if (event.xany.window == editw_get_win(dock->editw) && event.xany.window) {
      editw_dispatch_event(dock, dock->editw, &event);
    } else if (msgbox_ismapped(dock) && event.xany.window == msgbox_get_win(dock)) {
      msgbox_dispatch_event(dock, &event);
    } else if (event.xany.window == pp_get_win(dock) && event.xany.window) {
      pp_dispatch_event(dock, &dock->dlfp->tribune, &event);
    } else if (newswin_is_used(dock)) {
      if (event.xany.window == newswin_get_window(dock)) {
	newswin_dispatch_event(dock, dock->dlfp, &event);
      }
    }
  }
}

/* appelee ~25 fois par seconde */
void
timer_signal(int signum) {

  if (signum == SIGUSR1) {
    flag_discretion_request = 1;
    //printf("sigusr1\n"); 
#   ifdef SIGNAUX_A_LANCIENNE // mais maintenant on utilise sigaction
#   ifdef _XOPEN_SOURCE
    assert(signal(SIGUSR1, timer_signal) != SIG_ERR);
#   endif
#   endif
    return;
  } else if (signum == SIGUSR2) {
    flag_discretion_request = -1;
    //    printf("sigusr2\n"); 
#   ifdef SIGNAUX_A_LANCIENNE // mais maintenant on utilise sigaction
#   ifdef _XOPEN_SOURCE
    assert(signal(SIGUSR2, timer_signal) != SIG_ERR);
#   endif
#   endif
    return;
  } else if (signum != SIGALRM) {
    return;
  }

#ifdef SIGNAUX_A_LANCIENNE // mais maintenant on utilise sigaction
#ifdef _XOPEN_SOURCE
  assert(signal(SIGALRM, timer_signal) != SIG_ERR);
#endif
#endif

  X_loop_request++; wmcc_tic_cnt++;
}

/* la boucle principale (appelée 25 fois par seconde, mais uniquement aux moments 
   propices (cad pas au milieu d'un malloc..)) */
void X_loop()
{
  static int no_reentrant = 0;
  int save_errno;
  static int timer_cnt = 0;

  if (no_reentrant) {
    // apres reflexion ça peut arriver (en placant allow_x_loop dans des fonctions succeptible d'etre
    // appellees depuis network_thread et X_loop, dans ce cas, hop
    //printf ("REENTRANT !!!!! je vais me mordre les ***i*les\n"); exit(1); /* ça ne doit JAMAIS arriver */
    return;
  }
  no_reentrant = 1;

  X_loop_request = 0;

  save_errno = errno; /* eh oui, si on appelle X_loop dans une periode délicate, il vaut 
			 mieux ne pas trop toucher à cette variable... */
  
  timer_cnt++;

  dock->view_id_timer_cnt++;
  dock->mouse_cnt+=40;

  if (dock->coin_coin_request < 0) dock->coin_coin_request++; /* pour éteindre la led[1] apres un cours délai */

  /* verifie si il y a des ballons d'aide a afficher */
  if (Prefs.no_balloons == 0) check_balloons(dock);
  
  editw_action(dock, dock->editw); /* animation du palmipede si necessaire */

  /* lectures des evenements */
  wmcoincoin_dispatch_events(dock);

  timer_cnt++;

  if (timer_cnt % 10 == 0) {
    check_if_should_kill_ispell();
  }

  if (timer_cnt % 1 == 0) {
    if (dock->horloge_mode == 0) {
      dock_refresh_normal(dock);
    } else {
      dock_refresh_horloge_mode(dock);
    }

    /* gestion des animations du dock */
    if (dock->door_state == OPENING) {
      dock->door_state_step++;
      if (dock->door_state_step > 25+13) {
	dock->door_state_step = 25+13;
	dock->door_state = OPENED;
	opened_cnt = 0;
      }
    } else if (dock->door_state == OPENED) {
      if (opened_cnt++ > 200) {
	dock->door_state = CLOSING;
      }
    } else if (dock->door_state == CLOSING) {
      dock->door_state_step --;
      if (dock->door_state_step <= 0) {
	dock->door_state_step = 0;
	dock->door_state = CLOSED;
	dock->red_button_press_flag = -1;
      }
    }
    if (dock->red_button_press_flag > 0) {
      dock->red_button_press_state++;
      if (dock->red_button_press_state >= 6) dock->red_button_press_state = 5;
    } else if (dock->red_button_press_flag < 0) {
      dock->red_button_press_state--;
      if (dock->red_button_press_state <= 0) { 
	dock->red_button_press_state = 0;
	dock->red_button_press_flag = 0;
	if (dock->door_state != CLOSED) dock->door_state = CLOSING;
      }
    }
    
    set_leds_state(dock);
    leds_update(&dock->leds);

    /* clignotement qui signale l'ajout de nouveaux commentaires */
    if (flag_updating_comments == 0) {
      int xp_chg, cmt_chg;
      
      xp_chg = (dock->dlfp->xp_clign_cnt>=0);
      cmt_chg = (dlfp_yc_find_modified(dock->dlfp,NULL) != NULL);
      if (cmt_chg || xp_chg)
	{
	  tribune_load_clign_step++;
	  if (tribune_load_clign_step > 50) {
	    tribune_load_clign_step = 0;
	  }
	  if (cmt_chg) {
	    tribune_load_bgr = (25 - ABS(tribune_load_clign_step-25))*9;
	  }
	  if (xp_chg) {
	    tribune_load_bgg = (25 - ABS(tribune_load_clign_step-25))*9;
	  }
	  update_pix_tribune_load(dock, &dock->dlfp->tribune);

	  if (dock->dlfp->xp_clign_cnt >=0) {
	    dock->dlfp->xp_clign_cnt--;
	    if (dock->dlfp->xp_clign_cnt == -1) {
	      tribune_load_bgg = 0;
	    }
	  }
	  //	  if (tribune_load_clign_step == 0) {
	  //	  }
	} 
      if (tribune_load_clign_step == -2) {
	  tribune_load_clign_step = -1;
	  tribune_load_bgr = 0;
	  tribune_load_bgg = 0;
	  dock->dlfp->xp_clign_cnt = -1;
	  update_pix_tribune_load(dock, &dock->dlfp->tribune);
      }
    }


    dlfp_checkout(dock, dock->dlfp);
    checkout_tribune(dock, dock->dlfp);

    /* affichage effectif du coincoin */
    XCopyArea(dock->display, dock->coinpix, DOCK_WIN(dock), dock->NormalGC,
	      0,0, 64, 64, 0,0);

    //    if (timer_cnt % 2048 == 0) reread_messages();
    
    if (strlen(dock->newstitles)>10) {
      dock->newstitles_char_dec+=2;
      if (dock->newstitles_char_dec >= 6) {
	dock->newstitles_char_dec = 0;      
	dock->newstitles_pos++; if (dock->newstitles_pos >= (int)strlen(dock->newstitles)) {
	  dock->newstitles_pos = 0;
	}
      }
    }
    
    if (dock->tl_item_clicked && dock->msginfo_defil > 0) {
      dock->msginfo_defil+=3;
    }

    /* faut-il autoscroller le pinnipede teletype ? */
    pp_check_tribune_updated(dock, &dock->dlfp->tribune);

    /* update la fenetre des news si necessaire */
    if (newswin_is_used(dock)) {
      if (flag_news_updated && flag_updating_news == 0) {
	newswin_update_content(dock, dock->dlfp, 0);
	newswin_draw(dock);
	flag_news_updated = 0;
      } else if (flag_updating_news) {
	newswin_update_info(dock, dock->dlfp, 0, 0); /* juste pour afficher 'maj de news en cours' */
      }
    }
  }

  /* le chef est-il dans le bureau ? */
  if (flag_discretion_request == +1) {
    
    if ((dock->discretion_saved_state.newswin_used = newswin_is_used(dock))) {
      newswin_unmap(dock);
    }
    
    if ((dock->discretion_saved_state.palmipede_used = editw_ismapped(dock->editw))) {
      editw_unmap(dock, dock->editw);
    }

    if ((dock->discretion_saved_state.pinnipede_used = pp_ismapped(dock))) {
      pp_unmap(dock);
    }
    balloon_hide(dock); msgbox_hide(dock);
    flag_discretion_request = 0;
    dock->discretion_saved_state.last_sig_is_usr1 = 1;
    dock_set_horloge_mode(dock); /* arf arf le gros camouflage ! */
  } else if (flag_discretion_request == -1) { /* ahhh, il vient de sortir alors :) */
    if (dock->discretion_saved_state.last_sig_is_usr1) { /* on vient de tout cacher ? */
      if (dock->discretion_saved_state.newswin_used && 
	  newswin_is_used(dock)==0) {
	newswin_show(dock, dock->dlfp, -2);
      }
      if (dock->discretion_saved_state.palmipede_used &&
	  editw_ismapped(dock->editw) == 0) {
	editw_show(dock, dock->editw,0);
      }
      if (dock->discretion_saved_state.pinnipede_used && 
	  pp_ismapped(dock)==0) {
	pp_show(dock, &dock->dlfp->tribune);
      }
    } else { /* si on ne vient pas de tout cacher, on considère qu'il faut "raise" les fenetres */
      if (newswin_is_used(dock)) XRaiseWindow(dock->display, newswin_get_window(dock));
      if (editw_ismapped(dock->editw)) XRaiseWindow(dock->display, editw_get_win(dock->editw));
      if (pp_ismapped(dock)) XRaiseWindow(dock->display, pp_get_win(dock));
    }
    flag_discretion_request = 0;
    dock->discretion_saved_state.last_sig_is_usr1 = 0;
  }

  /* si ispell vient de se finir, on essaye de réafficher le palmipede pour prendre en compte les erreurs ... */
  if (editw_ismapped(dock->editw) && flag_spell_finished) {
    if (flag_spell_finished == 1) {
      editw_refresh(dock, dock->editw);
      flag_spell_finished = 0; /* remise a zero APRES le redraw histoire de ne pas relancer un ispell en plein milieu du redraw */
    } else {
      flag_spell_finished--; /* y'a un petit delai entre la fin de la correction et 
				son affichage, c'est juste pour éviter un clignotement trop
				chiant quand on tape
			     */
    }
  }

  no_reentrant =  0;

  errno = save_errno;
}


/* je l'ai pompe honteuseument dans le classique 'wmgeneral.c' ... 
 */
void createXBMfromXPM(char *xbm, char **xpm, int sx, int sy) {

  int i,j,k;
  int width, height, numcol, depth;
  int zero=0;
  unsigned char	bwrite;
  int bcount;
  int curpixel;

  sscanf(*xpm, "%d %d %d %d", &width, &height, &numcol, &depth);

  for (k=0; k!=depth; k++) {
    zero <<=8;
    zero |= xpm[1][k];
  }

  for (i=numcol+1; i < numcol+sy+1; i++) {
    bcount = 0;
    bwrite = 0;
    for (j=0; j<sx*depth; j+=depth) {
      bwrite >>= 1;

      curpixel=0;
      for (k=0; k!=depth; k++) {
	curpixel <<=8;
	curpixel |= xpm[i][j+k];
      }

      if ( curpixel != zero ) {
	bwrite += 128;
      }
      bcount++;
      if (bcount == 8) {
	*xbm = bwrite;
	xbm++;
	bcount = 0;
	bwrite = 0;
      }
    }
  }
}

/* x initialization crap */
void initx(Dock *dock, int argc, char **argv) {
  int i;

  /* some x structs */
  XSizeHints xsh;
  XWMHints *xwmh;
  XClassHint xch;
  XGCValues xgcv;
  XTextProperty xtp;
  char coin_mask[64*64/8];
  char clock_mask[64*64/8];
	
  /* connect to default display */
  dock->display = XOpenDisplay(getenv("DISPLAY"));
  if(!dock->display) {
    fprintf(stderr, "Couldn't connect to display\n");
    exit(1);
  }
  
  // je veux pas que atof("1,1") renvoie 1.1 et atof("1.1") 
  // renvoie 1 !!! (comme c'est naze les locales des fois..)
  //  setlocale(LC_ALL, "");
  setlocale(LC_CTYPE, ""); 
  XSetLocaleModifiers("@im=none");
  dock->input_method = XOpenIM(dock->display, NULL, NULL, NULL);
  if (!dock->input_method) {
    printf("Erreur ! echec de XOpenIM(), ca pue !\n");
    exit(1);
  }

  /* get screen and root window */
  dock->screennum = DefaultScreen(dock->display);
  dock->rootwin = RootWindow(dock->display, dock->screennum);

  dock->rgba_context = RGBACreateContext(dock->display, dock->screennum);    
  
  /* set size hints 64 x 64 */
  xsh.flags = USSize | USPosition;
  xsh.width = 64;
  xsh.height = 64;
  if (Prefs.draw_border == 0) {
    XWMGeometry(dock->display, dock->screennum, "64x64+0+0", NULL, 0,
		&xsh, &xsh.x, &xsh.y, &xsh.width, &xsh.height, &i);
  } else {
    xsh.x = Prefs.dock_xpos;
    xsh.y = Prefs.dock_ypos;
  }

  /* create the application window */
  dock->win = XCreateSimpleWindow(dock->display, dock->rootwin,
				  xsh.x, xsh.y, xsh.width, xsh.height, 0,
				  BlackPixel(dock->display, dock->screennum),
				  WhitePixel(dock->display, dock->screennum));
  
  if(!dock->win) {
    fprintf(stderr, "Couldn't create window\n");
    exit(1);
  }
  
  /* create icon window */
  dock->iconwin = XCreateSimpleWindow(dock->display, dock->rootwin,
				      xsh.x, xsh.y, xsh.width, xsh.height, 0,
				      BlackPixel(dock->display, dock->screennum),
				      WhitePixel(dock->display, dock->screennum));
  
  if(!dock->iconwin) {
    fprintf(stderr, "Couldn't create icon window\n");
    exit(1);
  }



  /* load interface pixmap */
  if(XpmCreatePixmapFromData(dock->display, dock->rootwin, coin_xpm,
			     &dock->coinpix, NULL, NULL)!=XpmSuccess) {
    fprintf(stderr, "Not engough free colorcells (you need at least a 15bit display)\n");
    exit(1);
  }
  
  createXBMfromXPM(coin_mask, coin_xpm, 64, 64);
  
  dock->coin_pixmask = XCreateBitmapFromData(dock->display, dock->win, 
					     coin_mask, 64, 64);

  if(XpmCreatePixmapFromData(dock->display, dock->rootwin, clock_xpm,
			     &dock->clockpix, NULL, NULL)!=XpmSuccess) {
    fprintf(stderr, "Not engough free colorcells (you need at least a 15bit display)\n");
    exit(1);
  }
  
  createXBMfromXPM(clock_mask, clock_xpm, 64, 64);
  
  dock->clock_pixmask = XCreateBitmapFromData(dock->display, dock->win, 
					     clock_mask, 64, 64);
  
  if(XpmCreatePixmapFromData(dock->display, dock->rootwin, led_xpm,
			     &dock->led, NULL, NULL)!=XpmSuccess) {
    fprintf(stderr, "Not engough free colorcells (you need at least a 15bit display)\n");
    exit(1);
  }
  
  if(XpmCreatePixmapFromData(dock->display, dock->rootwin, month_xpm,
			     &dock->month, NULL, NULL)!=XpmSuccess) {
    fprintf(stderr, "Not engough free colorcells (you need at least a 15bit display)\n");
    exit(1);
  }
  
  if(XpmCreatePixmapFromData(dock->display, dock->rootwin, date_xpm,
			     &dock->date, NULL, NULL)!=XpmSuccess) {
    fprintf(stderr, "Not engough free colorcells (you need at least a 15bit display)\n");
    exit(1);
  }
  
  if(XpmCreatePixmapFromData(dock->display, dock->rootwin, weekday_xpm,
			     &dock->weekday, NULL, NULL)!=XpmSuccess) {
    fprintf(stderr, "Not engough free colorcells (you need at least a 15bit display)\n");
    exit(1);
  }
  
  if(XpmCreatePixmapFromData(dock->display, dock->rootwin, month_xpm,
			     &dock->month, NULL, NULL)!=XpmSuccess) {
    fprintf(stderr, "Not engough free colorcells (you need at least a 15bit display)\n");
    exit(1);
  }
  
  if (Prefs.draw_border == 0) {
    /* setup shaped window */
    XShapeCombineMask(dock->display, dock->win, ShapeBounding, 
		      0, 0, dock->coin_pixmask, ShapeSet);
    XShapeCombineMask(dock->display, dock->iconwin, ShapeBounding,
		      0, 0, dock->coin_pixmask, ShapeSet);
  

    /* set window manager hints */
    xwmh = XAllocWMHints();
    xwmh->flags = WindowGroupHint | IconWindowHint | StateHint;
    xwmh->icon_window = dock->iconwin;
    xwmh->window_group = dock->win;
    xwmh->initial_state = WithdrawnState;
    XSetWMHints(dock->display, dock->win, xwmh);

  } else {
    XSetWindowAttributes wattr;
    wattr.override_redirect = True;
  
    XChangeWindowAttributes(dock->display, dock->win, CWOverrideRedirect, &wattr);
  }


  /* set class hints */


  xch.res_name = Prefs.app_name; 
  xch.res_class = Prefs.app_name; /* ca chie avec KDE:
				    quand KDE sauve la session, il sauve ce champ et execute ce nom
				    a la session suivante, donc: 
				    * rec_class doit etre le nom de l'executable
				    * et la ligne de commande, je la mets ou ?

				    pour l'instant, pas de sauvegarde de session kde..
				    */
  XSetClassHint(dock->display, dock->win, &xch);
  
  /* set size hints */
  XSetWMNormalHints(dock->display, dock->win, &xsh);
  
  /* tell window manager app name */
  if(!XStringListToTextProperty(&Prefs.app_name, 1, &xtp)) {
    fprintf(stderr, "Couldn't create text property\n");
    exit(1);
  }
  XSetWMName(dock->display, dock->win, &xtp);
  
  /* les trois couleurs du dock */
  {
    int r,g,b;
    r = (Prefs.bgcolor>>16)&0xff; g = (Prefs.bgcolor>>8)&0xff; b = Prefs.bgcolor &0xff;

    /* ces valeurs ne sont pas utilisees pour le dock lui-meme, qui passe par l'intermediaire
       de l'horrible rcreate_image_from_raw_with_tint et compagnie
       mais pour l'editwin qui est nettement mieux ecrite ainsi que pour msgbox... */
    dock->bg_pixel   = RGB2PIXEL(r,g,b);
    dock->light_pixel = RGB2PIXEL(MIN(255, (r*170)/128), MIN(255,(g*170)/128), MIN(255,(b*170)/128));
    dock->dark_pixel = RGB2PIXEL(MIN(255, (r*97)/128), MIN(255,(g*97)/128), MIN(255,(b*97)/128));
  }
  
  /* create a graphics context */
  xgcv.foreground = BlackPixel(dock->display, dock->screennum);
  xgcv.background = BlackPixel(dock->display, dock->screennum);

  dock->NormalGC = XCreateGC(dock->display, dock->win, GCForeground | GCBackground, &xgcv);
  if(!dock->NormalGC) {
    fprintf(stderr, "Couldn't create graphics context\n");
    exit(1);
  }
  
  /* select events to catch */
  XSelectInput(dock->display, dock->win,
	       ExposureMask |
	       ButtonPressMask |
	       ButtonReleaseMask |
	       PointerMotionMask |
	       EnterWindowMask | 
	       LeaveWindowMask |
	       StructureNotifyMask);
  XSelectInput(dock->display, dock->iconwin,
	       ExposureMask |
	       ButtonPressMask |
	       ButtonReleaseMask |
	       PointerMotionMask |
	       EnterWindowMask | 
	       LeaveWindowMask |
	       StructureNotifyMask);
  
  /* set the command line for restarting */
  XSetCommand(dock->display, dock->win, argv, argc);
  
  /* map the main window */
  XMapWindow(dock->display, dock->win);

  /* affiche l'image d'initialisation (oh comme c'est userfriendly ! ) */
  XSync(dock->display, True);
  XCopyArea(dock->display, dock->coinpix, DOCK_WIN(dock), dock->NormalGC, 0, 0, 64, 64, 0, 0);
  XFlush(dock->display);
}

/* ----------------------------------- */
/*        Main Network thread          */
/* ----------------------------------- */
void *Network_Thread (void *arg) {
  static int update_cnt = 0;

  arg = arg;
  strcpy(dock->newstitles, "transfert en cours...");

  while (1) {
    if (dock->coin_coin_request > 0) {
      //      printf("coincoin request\n");
      exec_coin_coin(dock); ALLOW_X_LOOP;
      dock->coin_coin_request = -50; /* on va le repasser progressivement à zero (pour permettre à la led
					de s'éteindre progressivement) */

      dock->tribune_update_request = 2; /* va falloir mettre la tribune à jour */
    }
    //    printf("%d\n", dock->tribune_update_request);
    if (dock->tribune_update_request > 1)
      dock->tribune_update_request--;
    if (dock->news_update_request > 1)
      dock->news_update_request--;

    if (update_cnt % (25*Prefs.dlfp_news_check_delay) == 10 ||
	dock->news_update_request == 1) {
      dlfp_updatenews(dock->dlfp); ALLOW_X_LOOP;
      dock->news_update_request = 0;
    }
    
    if (update_cnt % (25*Prefs.dlfp_tribune_check_delay) == 5 || 
	dock->tribune_update_request == 1) {
      if (dock->tribune_updatable) {
	//	printf("update\n");
	dlfp_updatetribune(dock->dlfp); 
	//if (flag_tribune_updated) pp_set_tribune_updated(dock); -> deplace dans checkout_tribune
	ALLOW_X_LOOP;
	dock->tribune_update_request = 0;
      } else {
	dock->tribune_update_request = 1;
      }
    }
    if (Prefs.ew_do_spell) ispell_run_background(Prefs.ew_spell_cmd, Prefs.ew_spell_dict);
#ifdef __CYGWIN__
    usleep(40000); 
#else
    ALLOW_X_LOOP;
    pause(); 
    ALLOW_X_LOOP;
#endif
    update_cnt++;
  }
}

/* (c)(tm)(r) kadreg qui n'aime pas le jaune (pourtant moi j'aime bien le jaune) */
void
prepare_pixmap_porte(Dock *dock)
{
  Pixmap bg_pixmap;
  /* debut modif kadreg */
  /* chargement de pix_porte suivant si pixmap de fond */

  dock->pix_porte = None;
  if (Prefs.bgpixmap) {
    XpmAttributes attribs;
    int res;
    attribs.valuemask = 0;
    res = XpmReadFileToPixmap(dock->display, dock->rootwin, 
				  Prefs.bgpixmap,
				  &bg_pixmap, NULL, &attribs);
    if ((res != XpmSuccess) || (attribs.width != 64) || (attribs.height != 64)) {
      myprintf("Erreur en chargeant le fichier : %<YEL %s>\n", Prefs.bgpixmap);
    } else {
      RGBAImage *rgba_porte;
      XImage *XiPixPixmap;
      int i, j;

      rgba_porte = rimage_create_from_raw(porte_image.width, porte_image.height,porte_image.bytes_per_pixel,porte_image.pixel_data);

      XiPixPixmap= XGetImage (dock->display, 
			      bg_pixmap, 
			      0, 0, 64, 64, AllPlanes, ZPixmap);
      XFreePixmap(dock->display, bg_pixmap);

      
      for (i=0; i< (int)rgba_porte->w; i++) {
	for (j=0; j< (int)rgba_porte->h; j++) {
	  XColor color;
	  unsigned char r, g, b;
	  int shade;

	  color.pixel = XGetPixel (XiPixPixmap, i, j);
	  XQueryColor (dock->display, 
		       DefaultColormap (dock->display, dock->screennum) , 
		       &color);

	  /* on ramene les composantes de 16bits à 8bits */
	  r = color.red >> 8;
	  g = color.green >> 8;
	  b = color.blue >> 8;

	  /* selon le niveau de gris de rgba_porte */
	  shade = rgba_porte->data[j][i].rgba[0] - 0x7F; /* shade = 0 pour les couleurs non modifiees, ~92 pour les 
							   bords clairs et  ~ -60 pour les bords sombres, voir xpms/porte.xcf */
	  if (shade > 0) {
	    r = MIN(r*1.3, 255.0);
	    g = MIN(g*1.3, 255.0);
	    b = MIN(b*1.3, 255.0);
	  } else if (shade == -127) {
	    r = g = b = 0;
	  } else if (shade < 0) {
	    r = MAX(r/1.3, 0.0); /* le MAX ne sert a rien, je sais */
	    g = MAX(g/1.3, 0.0);
	    b = MAX(b/1.3, 0.0);
	  }
	  rgba_porte->data[j][i].rgba[0] = r;
	  rgba_porte->data[j][i].rgba[1] = g;
	  rgba_porte->data[j][i].rgba[2] = b;
	}
      }

      dock->pix_porte = RGBAImage2Pixmap(dock->rgba_context, rgba_porte);
      //XpmWriteFileFromPixmap (dock->display, "pixporte.xpm", dock->pix_porte, 0, NULL);
      RGBADestroyImage(rgba_porte);
      XDestroyImage (XiPixPixmap);
    }
  }
  /* pouille dans le cottage ? */
  if (dock->pix_porte == None) {
      /* et hop, on sauve les meubles */
    dock->pix_porte = pixmap_create_from_raw_with_tint(dock->rgba_context, 
						       porte_image.width, 
						       porte_image.height, 
						       porte_image.bytes_per_pixel, 
						       porte_image.pixel_data, 
						       Prefs.bgcolor);
    
  }
  /* fin modif kadreg */
}

static void
install_sighandlers()
{
  /* installation du gestionnaire de signal ALARM
     c'est lui qui permet à coincoin de continuer à clignoter pendant
     que des download sont en cours... ça n'est pas très orthodoxe comme fonctionnement,
     mais, avec quelques protections basiques, ça marche très bien, au moins sous linux
  */
#ifdef SIGNAUX_A_LANCIENNE
  assert(signal(SIGALRM, timer_signal) != SIG_ERR); /* le comportement de certains OS (solaris par ex. differe
					  differe de linux (si _XOPEN_SOURCE est défini,
					  le gestionnaire de signal n'est pas automatiquement réinstallé
					  après le déclenchement du signal ... y'a un bugfix dans X_loop,
					  mais c'est mieux d'utiliser les signaux posix (sigaction..)
				       */
  assert(signal(SIGUSR1, timer_signal) != SIG_ERR);
  assert(signal(SIGUSR2, timer_signal) != SIG_ERR);
#else
  {
    struct sigaction action;
    action.sa_handler = timer_signal;
    sigemptyset(&(action.sa_mask));
    action.sa_flags = 0;
    if (sigaction(SIGALRM, &action, NULL) != 0) {
     fprintf(stderr,"sigaction: erreur %d (%s)\n essayez de recompiler en faisant un #define SIGNAUX_A_LANCIENNE...\n",
         errno, strerror(errno));
     exit(1);
    }
    if (sigaction(SIGUSR1, &action, NULL) != 0) {
     fprintf(stderr,"sigaction: erreur %d (%s)\n essayez de recompiler en faisant un #define SIGNAUX_A_LANCIENNE...\n",
         errno, strerror(errno));
     exit(1);
    }
    if (sigaction(SIGUSR2, &action, NULL) != 0) {
     fprintf(stderr,"sigaction: erreur %d (%s)\n essayez de recompiler en faisant un #define SIGNAUX_A_LANCIENNE...\n",
         errno, strerror(errno));
     exit(1);
    }

  }
#endif
}


/* fonction non utilisee */
void
block_sigalrm(int bloque)
{
#ifdef SIGNAUX_A_LANCIENNE
#else
  {
    sigset_t sset;
    sigemptyset(&sset);
    sigaddset(&sset, SIGALRM);
    
    sigprocmask(bloque ? SIG_BLOCK : SIG_UNBLOCK, &sset, NULL);
  }
#endif
}



int
main(int argc, char **argv)
{
#ifdef __CYGWIN__
  pthread_t net_thread;
#endif
  srand(time(NULL));
  ALLOC_OBJ(dock, Dock);

  myprintf("%<GRN wmc2> v.%<WHT " VERSION "> [ compile le " __DATE__ " ]\n");

  init_default_prefs (argc, argv, &Prefs);
  check_wmcoincoin_dir();
  read_coincoin_options (&Prefs); // lecture de '.wmcoincoin/options'
  parse_cmdline(argc, argv, &Prefs);

  http_init();

  if (Prefs.debug & 1) {
    _Xdebug = 1; /* oblige la synchronisation */
  }

  ALLOC_VEC(dock->newstitles,MAX_NEWSTITLES_LEN, unsigned char);
  ALLOC_VEC(dock->newstitles_id,MAX_NEWSTITLES_LEN, int);
  ALLOC_VEC(dock->msginfo,MAX_MSGINFO_LEN, unsigned char);
  /* le trolloscope */
  ALLOC_ARR(tribune_load, TRIBUNE_LOAD_HEIGHT, TRIBUNE_LOAD_WIDTH, TL_item);

  /* recopie du message et du useragent des preferences */
  if (Prefs.user_name) {
    snprintf(dock->coin_coin_message, MESSAGE_MAX_LEN, "%s> %s", Prefs.user_name, Prefs.coin_coin_message);
  } else {
    strncpy(dock->coin_coin_message, Prefs.coin_coin_message, MESSAGE_MAX_LEN); 
  }
  dock->coin_coin_message[MESSAGE_MAX_LEN] = 0;
  free(Prefs.coin_coin_message); Prefs.coin_coin_message = dock->coin_coin_message; /* pas beau */

  strncpy(dock->coin_coin_useragent, Prefs.user_agent, USERAGENT_MAX_LEN); 
  dock->coin_coin_useragent[USERAGENT_MAX_LEN] = 0;
  /* pour les http_get, on utilisera Prefs.user_agent, qui est non modifiable
     pour les http_post, on utilisera dock->coin_coin_useragent */

  dock->tl_item_survol = NULL;
  dock->news_update_request = 0;
  dock->tribune_update_request = 0;
  dock->tribune_updatable = 1;
  dock->coin_coin_request = 0;

  dock->view_id_in_newstitles = 0;
  dock->flag_survol_trollo = 0;
  dock->flag_survol_led1 = 0;
  dock->flag_trib_load_cursor = 0;
  dock->mouse_x = 0; 
  dock->mouse_y = 0;
  dock->mouse_cnt = 0;
  dock->mouse_win = None;
  dock->trolloscope_speed = Prefs.default_trollo_speed;
  dock->discretion_saved_state.newswin_used = 0;
  dock->discretion_saved_state.palmipede_used = 0;
  dock->discretion_saved_state.pinnipede_used = 0;
  dock->discretion_saved_state.last_sig_is_usr1 = 0;
  dock->horloge_mode = 0;
  dock->trib_trollo_rate = 0; dock->trib_trollo_score = 0;
  dock->dlfp = dlfp_create();

  {
    char mask[64*64];
    
    XGCValues xgcv;
    
    createXBMfromXPM(mask, coin_xpm, 64, 64);
    
    initx(dock,argc, argv);


    prepare_pixmap_porte(dock);
    //dock->pix_porte = pixmap_create_from_raw_with_tint(dock->rgba_context, porte_image.width, porte_image.height, porte_image.bytes_per_pixel, porte_image.pixel_data, Prefs.bgcolor);
	
    //    exit(0);
    //dock->pix_porte = pixmap_create_from_raw_with_tint(dock->rcontext, porte_image.width, porte_image.height, porte_image.bytes_per_pixel, porte_image.pixel_data, 0x808080);

    /* bouh comme c vilain */
    dock->mask_porte_haut = alpha2pixmap_mask(dock->display, dock->coinpix, 64, 64, porte_image.pixel_data, 255);
    dock->mask_porte_bas = alpha2pixmap_mask(dock->display, dock->coinpix, 64, 64, porte_image.pixel_data, 230);

    leds_create(&dock->leds);

    dock->fixed_font = XLoadQueryFont(dock->display, DOCK_FIXED_FONT);
    if (!dock->fixed_font) {
      myfprintf(stderr, "Impossible de charger la fonte " DOCK_FIXED_FONT "\n");
      exit(-1);
    }

    //    assert(XpmCreatePixmapFromData(dock->display, dock->coinpix, letters_xpm, &pixmap_letters, NULL, NULL) == XpmSuccess);
    //    assert(XpmCreatePixmapFromData(dock->display, dock->coinpix, leds_xpm, &pixmap_led, NULL, NULL) == XpmSuccess);
    
    dock->pix_tribune_load = XCreatePixmap(dock->display, dock->coinpix, TRIBUNE_LOAD_WIDTH, TRIBUNE_LOAD_HEIGHT, DefaultDepth(dock->display,dock->screennum));
    XFillRectangle(dock->display, dock->pix_tribune_load, dock->NormalGC, 0, 0, TRIBUNE_LOAD_WIDTH, TRIBUNE_LOAD_HEIGHT);

    xgcv.function = GXor;
    xgcv.graphics_exposures = 0;
    dock->or_GC = XCreateGC(dock->display, dock->coinpix, GCFunction| GCGraphicsExposures, &xgcv);

    xgcv.function = GXand;
    xgcv.graphics_exposures = 0;
    dock->and_GC = XCreateGC(dock->display, dock->coinpix, GCFunction| GCGraphicsExposures, &xgcv);
  }
  /* selection du curseur affiche qd on survole la trib_load */
  //  dock->trib_load_cursor = XCreateFontCursor(dock->display, XC_target);
  //dock->trib_load_cursor = XCreateFontCursor(dock->display, XC_plus);
  dock->trib_load_cursor = XCreateFontCursor(dock->display, XC_tcross);

  dock->atom_WM_DELETE_WINDOW = XInternAtom(dock->display, "WM_DELETE_WINDOW", False);
  dock->atom_WM_PROTOCOLS = XInternAtom(dock->display, "WM_PROTOCOLS", False);

  newswin_build(dock);
  balloon_build(dock);
  msgbox_build(dock);
  dock->editw = editw_build(dock);
  pp_build(dock);
  

  if (read_initial_rcfile(dock->dlfp) != 0) /* a faire APRES msgbox_build */
    return 1;
  

  dock->door_state = CLOSED; dock->door_state_step = 0;
  dock->red_button_press_flag = 0; dock->red_button_press_state = 0;
  dock->post_anonyme = 0;

  dock->newstitles_pos = 0; 
  dock->newstitles_char_dec = 0;
  dock->newstitles[0] = 0;
  memset(dock->tribune_time, 0, 6);

  dock_refresh_normal(dock);

  install_sighandlers();

  /* launching the network update thread */

#ifdef __CYGWIN__ 
  pthread_create (&net_thread, NULL, Network_Thread, NULL);

  while (1) {
    X_loop ();
    usleep (40000);
  }
#else
  {
    struct itimerval the_timer;
    the_timer.it_interval.tv_sec = 0;
    the_timer.it_interval.tv_usec = WMCC_TIMER_DELAY_MS * 1000; /* microsecondes */
    the_timer.it_value = the_timer.it_interval;

    setitimer(ITIMER_REAL, &the_timer, NULL);
  }
  Network_Thread(NULL);
#endif  
  return 0;
}
