
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
  rcsid=$Id: wmcoincoin.c,v 1.13 2002/01/16 21:27:35 pouaite Exp $
  ChangeLog:
  $Log: wmcoincoin.c,v $
  Revision 1.13  2002/01/16 21:27:35  pouaite
  gros coup de balai dans wmcoincoin.c qui s'est du coup splitt� en trois: wmcoincoin.c, dock.c et useragents_file.c

  Revision 1.12  2002/01/16 10:34:21  pouaite
  ptit patch glandium

  Revision 1.11  2002/01/15 15:30:17  pouaite
  rien d'interessant

  Revision 1.10  2002/01/14 23:54:06  pouaite
  reconnaissance des posts effectu� par l'utilisateur du canard (� suivre...)

  Revision 1.9  2002/01/12 17:29:08  pouaite
  support de l'iso8859-15 (euro..)

  Revision 1.8  2002/01/10 09:18:23  pouaite
  patch de jjb (ralentissement progressif des updates de la tribune en cas d'inactivit� du coincoin)

  Revision 1.7  2002/01/10 09:03:06  pouaite
  integration du patch de glandium (requetes http/1.1 avec header 'If-Modified-Since' --> coincoin plus gentil avec dacode)

  Revision 1.6  2001/12/17 17:58:58  pouaite
  bugfix plutot gruik du flamometre qui partait en nouille

  Revision 1.5  2001/12/16 01:43:33  pouaite
  filtrage des posts, meilleure gestion des posts multiples

  Revision 1.4  2001/12/06 22:41:11  pouaite
  patch de glandium (bugfix du mode asclock)

  Revision 1.3  2001/12/02 18:07:03  pouaite
  am�lioration du support de ispell avec un vilain hack + correction(..pas sur..) d'un bug de clignotement du flammometre

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
//#include <sys/types.h>
#include <sys/stat.h>
//#include <regex.h>
#include <X11/xpm.h>
#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <X11/cursorfont.h>
#include <X11/extensions/shape.h>
#include <locale.h>
#include "coincoin.h"
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
#include "../xpms/weekday.xpm"
#include "../xpms/month.xpm"
#include "../xpms/clock.xpm"
#include "../xpms/date.xpm"
#include "../xpms/led.xpm"



/* des vieilles variables globales qui feraient mieux de rentrer dans une jolie structure */

int temps_depuis_dernier_event = 0;


structPrefs Prefs;

int opened_cnt; /* �a c'est de la bonne vieille variable qui date de la v0.9...*/

Dock *dock;

/*
   la fonction de la honte ...

   ouverture d'une url dans un browser externe (avec une commande du type 'mozilla url &', execut�e par le shell)

   url est une chaine de carateres (termin�e par un zero), de longueur
   raisonnable, mais susceptible de contenir absolument n'importe quels caract�res.
   Le principe est d'�chapper tous ceux qui sont susceptible d'etre interpretes par le shell.



extrait de RATIONALE FOR SECURITY FILTER, d.wheeler


les arguments bidons sont:
  ballon_{x,y} : position d'affichage du ballon d'aide quand aucun browser externe n'a �t� d�fini
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
           "il faudra la lancer � la main (copier dans le clipboard avec clic droit etc..)", url);
    if (balloon_x != -1 && balloon_y != -1) {
      balloon_show(dock, balloon_x, balloon_y, 10, 10, s, 300);
    } else {
      fprintf(stderr, "wmcoincoin: %s\n", s);
    }
    return;
  }

  if (bcmd == NULL) {
    snprintf(s, CMDSZ, "vous n'avez pas d�fini le browser externe dans ~/.wmcoincoin/options (%s), inutile de cliquer comme un fou sur les <b>[url]</b>...",
	     (browser_num == 1) ? "http.browser: lacommande" : "http.browser2: lacommande");
    if (balloon_x > 0 && balloon_y > 0) {
      balloon_show(dock, balloon_x, balloon_y, 10, 10, s, 300);
    } else {
      fprintf(stderr, "wmcoincoin: %s\n", s);
    }
    return;
  }


      /* d'apres d.wheeler (et la rfc 1738,2396), une url est form�e de caract�res
       - r�serv�s:                                ; / ? : @ & = + $ ,
       - non r�serv�s:    A-Z, a-z, 0-9, et | | | | | | - _ . ! ~ * ' ( )

       tous les caract�res qui ne sont pas 'r�serv�s' peuvent �tre hexencod�s en %XX.

       toujours selon lui, la liste des caract�res interpr�tables par le shell est
       (http://www.dwheeler.com/secure-programs/Secure-Programs-HOWTO/limit-call) :
       & ; ` ' \ " | * ? ~ < > ^ ( ) [ ] { } $ \n \r


       Les virgules et les parenth�ses fermantes dans une url sont susceptible d'etre mal interpret�es
       par netscape quand on lui passe l'url en ligne de commande avec nestcape -remote openURL(http://blahblah, ddfdjh)
      */

  /*
     FILTRE DEUX-EN-UN:
     on hex-encode tous les caract�re qui ne sont pas les suivants:
     ; / ? : @ & = + $ # % A-Z a-z 0-9 - _ . , et tout ce qui est inf�rieur � 0x20 est gicl�

     ceci dit, on pourrait aussi refuser toute url contenant des caract�re devant �tre hexencod�s...

     puis, les charact�res non alphanum�riques qui n'ont pas �t� hexencod�s sont backslash�s pour ne surtout pas
     �tre interpret�s par /bin/sh
  */
  for (i=0, j=0; url[i] && j < SECSZ-3; i++) {
    if (url[i] > ' ') { /* tout ce qui est ' ', '\r' '\n' '\t' passe � la trappe */
      int alnum;
      alnum = ((url[i]>='a' && url[i] <= 'z') ||
             (url[i]>='A' && url[i] <= 'Z') ||
             (url[i]>='0' && url[i] <= '9')) ? 1 : 0;
      /* remarque: on hexencode un caract�re r�serv�, la virgule, mais ce n'est apparament pas g�nant */
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
     � la bouillie que j'avais fait dans coincoin 2.01
  */

  snprintf(s, CMDSZ, bcmd, secure_url);

  myfprintf(stderr, "Execution de \"%<YEL %s>\"\n", s);
  /*
    system fait appel � /bin/sh , mais on a backslash� tous les caract�res embetants
  */
  system(s);
}

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


/* d�clenche la mise � jour du trolloscope, du pinnipede etc.. */
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

      dock_update_pix_trolloscope(dock, &dlfp->tribune);

      pp_set_tribune_updated(dock);

      flag_tribune_updated = 0;
    }
  }
}

/* 
   poste le message sur la tribune -- en tant que fonction 'lente' 
   cette fonction est execut�e par la boucle principale
 */
void
exec_coin_coin(Dock *dock)
{
  SOCKET fd;
  char s[2048], referer[2048], path[2048];
  char *urlencod_msg;
  char cookie[200];

  BLAHBLAH(1, myprintf("message post�: '%<YEL %s>\n", dock->real_coin_coin_message));
  BLAHBLAH(1, myprintf("   (useragent: '%<CYA %s>\n", dock->real_coin_coin_useragent));
  flag_sending_coin_coin = 1;
  urlencod_msg = http_url_encode(dock->real_coin_coin_message); assert(urlencod_msg);
  snprintf(s, 2048, "message=%s", urlencod_msg);
  free(urlencod_msg);
  snprintf(referer, 2048,"http://%s:%d/%s/", Prefs.site_root, Prefs.site_port, Prefs.site_path);
  snprintf(path, 2048, "%s%s/%s", strlen(Prefs.site_path) ? "/" : "", Prefs.site_path, Prefs.path_tribune_add);
  if (Prefs.user_cookie && dock->post_anonyme == 0) { /* merci glandium !! */
     snprintf(cookie, 200, "session_id=%s", Prefs.user_cookie);
  } else cookie[0] = 0;
  fd = http_post_with_cookie(Prefs.site_root, Prefs.site_port, path,
			     Prefs.proxy_name, Prefs.proxy_auth, Prefs.proxy_port, dock->real_coin_coin_useragent,
			     referer, cookie, s);


  if (fd != INVALID_SOCKET) {
    int got;
    
    if (http_skip_header(fd, NULL) < 0) {
      /* si la reponse n'est pas un 302 Found */
      snprintf(s, 2048, "Damned ! y'a une pouille dans le cotage<p>%s", http_error());
      msgbox_show(dock, s);
    } else {
      int l;
      BLAHBLAH(1, myprintf(" --> OK, message envoye avec succes\n"));

      strcpy(s, "Ouups, il y a sans doute eu un petit probleme, <b>LinuxFr</b> a r�pondu:<p>");
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

    /* pour la reconnaissance des messages de ceux qui sont g�n�ralement authentifi�s et se lachent en anonyme de temps � autre */
    if (dock->post_anonyme) { dock->dlfp->tribune.just_posted_anonymous = 1; }

  } else {
    snprintf(s, 2048, "Erreur pendant l'envoi du message: <p><b>%s</b>\n", http_error());
    msgbox_show(dock, s);
  }
  flag_sending_coin_coin = 0;
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
		 "<font color=blue><tt>Click Gauche</tt></font><tab>: afficher l'article qui d�file<br>"
		 "<font color=blue><tt>Click Milieu</tt></font><tab>: mise � jour imm�diate de la liste des news<br>"
		 "<font color=blue><tt>Click Droite</tt></font><tab>: ouvrir/fermer la fen�tre des news<br>",
		 Prefs.site_root);
	balloon_test(dock,x,y,iconx,icony,0,3,3,57,11,
		     s);
	balloon_test(dock,x,y,iconx,icony,0,TROLLOSCOPE_X,TROLLOSCOPE_Y,TROLLOSCOPE_WIDTH,TROLLOSCOPE_HEIGHT,
		     "<p align=center><b>Ceci est un <font color=#a00000><i>Trolloscope</i></font> professionel</b></p>"
		     "Il affiche des symboles correspondant aux user-agents des derniers messages post�s sur la tribune."
		     "Les associations useragent->(couleur,symbole) sont d�finies dans le fichier <tt>~/.wmcoincoin/useragents</tt><br>"
		     "<font color=blue><tt>Click Gauche</tt></font><tab>: affiche un useragent<br>"
		     "<font color=blue><tt>Click Milieu</tt></font><tab>: mise � jour imm�diate<br>"
		     "remarque: si vous avez pr�t� votre cookie d'authentification � wmCoinCoin et que celui-ci "
		     "clignote en rouge, il vous indique qu'on a r�pondu � un de vos commentaire (non anonyme, bien sur) "
		     "Pour voir les r�ponses: <font color=blue><tt>Click Gauche</tt></font> (� condition d'avoir configur� le browser utilis� par "
		     "wmcoincoin). Pour arr�ter le clignotement, un <font color=blue><tt>Click Droite</tt></font> suffit. Le clignotement vert "
		     "indique un changement de la valeur de vos XP");
	balloon_test(dock,x,y,iconx,icony,0,dock->leds.led[0].xpos, dock->leds.led[0].ypos, 9, 4,
		     "Lorsque cette led est bleue, un <b>transfert http</b> depuis linuxfr est en cours."
		     "Un clignotement <b><font color=red>rouge</font></b> indique un probleme au cours du dernier transfert<br>"
		     "<font color=blue><tt>Click Gauche</tt></font>: affiche dernier message d'erreur re�u<br>");
	balloon_test(dock,x,y,iconx,icony,0,dock->leds.led[1].xpos, dock->leds.led[1].ypos, 9, 4,
		     "Cette led clignote lorsque vous venez de poster un message sur la tribune, et que celui est en attente<br>."
		     "Elle est bleue lorsque le message est en cours d'envoi, et verte une demi-seconde apr�s son envoi.<br>"
		     "Un clic sur cette led permet de changer de la vitesse de defilement du trolloscope:<br>"
		     "<font color=blue><tt>Click Gauche</tt></font><tab>: moins vite<br>"
		     "<font color=blue><tt>Click Droit</tt></font><tab>: plus vite<br>"
		     "<font color=blue><tt>Click Milieu</tt></font><tab>: changer la r�solution du trolloscope");
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
		     "Quand le curseur de la souris est au-dessus cette led, le niveau du <font color=#a00000><i>Trollometre</i></font> est affich�<br>"
		     "<font color=blue><tt>Click Gauche</tt></font><tab>: INVOQUER <b>C01N C01N</b> !<br>"
		     "<font color=blue><tt>Click Droit</tt></font><tab>: voir les stats de fr�quentation de la tribune, ainsi que vos XP/votes (si vous avez fourni votre cookie d'authentification)<br>"
		     "<font color=blue><tt>Click Milieu</tt></font>: demande de relecture de <tt>~/.wmcoincoin/useragents</tt>");
	balloon_test(dock,x,y,iconx,icony,0,3,49,57,12,
		     "Heure du dernier message re�u sur la tribune, ainsi que le nombre de secondes qui se sont �coul�es depuis<br>"
		     "<font color=blue><tt>Click Gauche</tt></font>: montrer/cacher le <b>palmip�de editor</b><br>"
		     "<font color=blue><tt>Click Droit</tt></font>: montrer/cacher le <b>pinnip�de teletype</b>");

      } else if (dock->door_state == OPENED && dock->horloge_mode == 0) {
	balloon_test(dock,x,y,iconx,icony,2000,31,30,16,16,
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
       + on ramene le focus dans editw (quand elle est active)
       + on remet � 0 le temps depuis le dernier d�placement */
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
	temps_depuis_dernier_event = 0;
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

/* la boucle principale (appel�e 25 fois par seconde, mais uniquement aux moments 
   propices (cad pas au milieu d'un malloc..)) */
void X_loop()
{
  static int no_reentrant = 0;
  int save_errno;
  static int timer_cnt = 0;

  if (no_reentrant) {
    // apres reflexion �a peut arriver (en placant allow_x_loop dans des fonctions succeptible d'etre
    // appellees depuis network_thread et X_loop, dans ce cas, hop
    //printf ("REENTRANT !!!!! je vais me mordre les ***i*les\n"); exit(1); /* �a ne doit JAMAIS arriver */
    return;
  }
  no_reentrant = 1;

  X_loop_request = 0;

  save_errno = errno; /* eh oui, si on appelle X_loop dans une periode d�licate, il vaut 
			 mieux ne pas trop toucher � cette variable... */
  
  timer_cnt++;

  dock->view_id_timer_cnt++;
  dock->mouse_cnt+=40;

  if (dock->coin_coin_request < 0) dock->coin_coin_request++; /* pour �teindre la led[1] apres un cours d�lai */

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
    
    dock_leds_set_state(dock);
    dock_leds_update(&dock->leds);

    /* clignotement qui signale l'ajout de nouveaux commentaires */
    if (flag_updating_comments == 0) {
      int xp_chg, cmt_chg;
      
      xp_chg = (dock->dlfp->xp_clign_cnt>=0);
      cmt_chg = (dlfp_yc_find_modified(dock->dlfp,NULL) != NULL);
      if (cmt_chg || xp_chg)
	{
	  dock->trolloscope_clign_step++;
	  if (dock->trolloscope_clign_step > 50) {
	    dock->trolloscope_clign_step = 0;
	  }
	  if (cmt_chg) {
	    dock->trolloscope_bgr = (25 - ABS(dock->trolloscope_clign_step-25))*9;
	  }
	  if (xp_chg) {
	    dock->trolloscope_bgg = (25 - ABS(dock->trolloscope_clign_step-25))*9;
	  }
	  dock_update_pix_trolloscope(dock, &dock->dlfp->tribune);

	  if (dock->dlfp->xp_clign_cnt >=0) {
	    dock->dlfp->xp_clign_cnt--;
	    if (dock->dlfp->xp_clign_cnt == -1) {
	      dock->trolloscope_bgg = 0;
	    }
	  }
	  //	  if (trolloscope_clign_step == 0) {
	  //	  }
	} 
      if (dock->trolloscope_clign_step == -2) {
	  dock->trolloscope_clign_step = -1;
	  dock->trolloscope_bgr = 0;
	  dock->trolloscope_bgg = 0;
	  dock->dlfp->xp_clign_cnt = -1;
	  dock_update_pix_trolloscope(dock, &dock->dlfp->tribune);
      }
    }


    dock_checkout_newstitles(dock, dock->dlfp);
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
    } else { /* si on ne vient pas de tout cacher, on consid�re qu'il faut "raise" les fenetres */
      if (newswin_is_used(dock)) XRaiseWindow(dock->display, newswin_get_window(dock));
      if (editw_ismapped(dock->editw)) XRaiseWindow(dock->display, editw_get_win(dock->editw));
      if (pp_ismapped(dock)) XRaiseWindow(dock->display, pp_get_win(dock));
    }
    flag_discretion_request = 0;
    dock->discretion_saved_state.last_sig_is_usr1 = 0;
  }

  /* si ispell vient de se finir, on essaye de r�afficher le palmipede pour prendre en compte les erreurs ... */
  if (editw_ismapped(dock->editw) && flag_spell_finished) {
    if (flag_spell_finished == 1) {
      editw_refresh(dock, dock->editw);
      flag_spell_finished = 0; /* remise a zero APRES le redraw histoire de ne pas relancer un ispell en plein milieu du redraw */
    } else {
      flag_spell_finished--; /* y'a un petit delai entre la fin de la correction et 
				son affichage, c'est juste pour �viter un clignotement trop
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



static int faut_il_rafraichir(int count,int delay, int offset)
{
  if(count<25*delay) // On a d�j� rafra�chi r�cemment
    return 0;
  if(Prefs.dlfp_max_refresh_delay == 0)
    return 1;
  if(temps_depuis_dernier_event > 1500*Prefs.dlfp_max_refresh_delay) // minutes
    return 0;
  if(temps_depuis_dernier_event > 150*Prefs.dlfp_max_refresh_delay)
    {
      if(temps_depuis_dernier_event % (125*delay) == offset)
	return 1;
      else
	return 0;
    }
    // Exemple : au bout de 2 heures on arr�te tout => au bout de 12 minutes on commence � ralentir (5 fois moins de refresh).

  if (temps_depuis_dernier_event % (25*delay) == offset)
    return 1;
  return 0;
}



/* ----------------------------------- */
/*        Main Network thread          */
/* ----------------------------------- */
void *Network_Thread (void *arg) {
  int compteur_news = 1000000;
  int compteur_tribune = 1000000;

  arg = arg;
  strcpy(dock->newstitles, "transfert en cours...");

  while (1) {
    if (dock->coin_coin_request > 0) {
      //      printf("coincoin request\n");
      exec_coin_coin(dock); ALLOW_X_LOOP;
      dock->coin_coin_request = -50; /* on va le repasser progressivement � zero (pour permettre � la led
					de s'�teindre progressivement) */

      dock->tribune_update_request = 2; /* va falloir mettre la tribune � jour */
    }
    //    printf("%d\n", dock->tribune_update_request);
    if (dock->tribune_update_request > 1)
      dock->tribune_update_request--;
    if (dock->news_update_request > 1)
      dock->news_update_request--;

    if (faut_il_rafraichir(compteur_news,Prefs.dlfp_news_check_delay,10) ||
	dock->news_update_request == 1) {
      compteur_news=0;
      dlfp_updatenews(dock->dlfp); ALLOW_X_LOOP;
      dock->news_update_request = 0;
    }
    
    if (faut_il_rafraichir(compteur_tribune,Prefs.dlfp_tribune_check_delay,5) || 
	dock->tribune_update_request == 1) {
      compteur_tribune=0;
      if (dock->tribune_updatable) {
	//	printf("update\n");
	dlfp_tribune_update(dock->dlfp, dock->real_coin_coin_useragent); 
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
    compteur_news++;
    compteur_tribune++;
    temps_depuis_dernier_event++;
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

	  /* on ramene les composantes de 16bits � 8bits */
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
     c'est lui qui permet � coincoin de continuer � clignoter pendant
     que des download sont en cours... �a n'est pas tr�s orthodoxe comme fonctionnement,
     mais, avec quelques protections basiques, �a marche tr�s bien, au moins sous linux
  */
#ifdef SIGNAUX_A_LANCIENNE
  assert(signal(SIGALRM, timer_signal) != SIG_ERR); /* le comportement de certains OS (solaris par ex. differe
					  differe de linux (si _XOPEN_SOURCE est d�fini,
					  le gestionnaire de signal n'est pas automatiquement r�install�
					  apr�s le d�clenchement du signal ... y'a un bugfix dans X_loop,
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
  ALLOC_ARR(dock->trolloscope, TROLLOSCOPE_HEIGHT, TROLLOSCOPE_WIDTH, TL_item);

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
  strncpy(dock->real_coin_coin_useragent, Prefs.user_agent, USERAGENT_MAX_LEN); 
  dock->real_coin_coin_useragent[USERAGENT_MAX_LEN] = 0;
  /* pour les http_get, on utilisera Prefs.user_agent, qui est non modifiable
     pour les http_post, on utilisera dock->coin_coin_useragent */

  dock->tl_item_survol = NULL;
  dock->trolloscope_resolution = 5;
  dock->trolloscope_bgr = dock->trolloscope_bgb = dock->trolloscope_bgg = 0;
  dock->trolloscope_clign_step = -1;

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

    dock_leds_create(dock, &dock->leds);

    {
      char fn[512];
      snprintf(fn, 512, "%s-%s", DOCK_FIXED_FONT, Prefs.font_encoding);
      dock->fixed_font = XLoadQueryFont(dock->display, fn);
      if (!dock->fixed_font) {
	myfprintf(stderr, "Impossible de charger la fonte %s\n", fn);
	exit(-1);
      }
    }

    //    assert(XpmCreatePixmapFromData(dock->display, dock->coinpix, letters_xpm, &pixmap_letters, NULL, NULL) == XpmSuccess);
    //    assert(XpmCreatePixmapFromData(dock->display, dock->coinpix, leds_xpm, &pixmap_led, NULL, NULL) == XpmSuccess);
    
    dock->pix_trolloscope = XCreatePixmap(dock->display, dock->coinpix, TROLLOSCOPE_WIDTH, TROLLOSCOPE_HEIGHT, DefaultDepth(dock->display,dock->screennum));
    XFillRectangle(dock->display, dock->pix_trolloscope, dock->NormalGC, 0, 0, TROLLOSCOPE_WIDTH, TROLLOSCOPE_HEIGHT);

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
  

  if (useragents_file_read_initial(dock, dock->dlfp) != 0) /* a faire APRES msgbox_build */
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
