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
  rcsid=$Id: coincoin_news.c,v 1.26 2002/05/19 01:14:57 pouaite Exp $
  ChangeLog:
  $Log: coincoin_news.c,v $
  Revision 1.26  2002/05/19 01:14:57  pouaite
  bugfix du dernier bugfix.. ou pas.. chuis un peu trop fatigué pour faire des bugfix

  Revision 1.25  2002/05/18 21:29:41  pouaite
  PAN PAN le vilain bug

  Revision 1.24  2002/05/15 09:55:13  pouaite
  fix http.c

  Revision 1.23  2002/05/12 22:06:27  pouaite
  grosses modifs dans http.c

  Revision 1.22  2002/04/09 00:28:19  pouaite
  quelques modifs faites dans un état d'hébétude avancé /!\ travaux en cours /!\

  Revision 1.21  2002/04/01 01:39:38  pouaite
  grosse grosse commition (cf changelog)

  Revision 1.20  2002/03/28 11:21:57  pouaite
  bugfix crash messagerie (si l'utilisateur ne l'a jamais utilisee) -> merci bplessis

  Revision 1.19  2002/03/27 23:27:10  pouaite
  tjs des bugfixes (pour gerer des posts qui peuvent atteindre 10ko !), en parallele de la v2.3.6-5

  Revision 1.18  2002/03/03 10:10:04  pouaite
  bugfixes divers et variés

  Revision 1.17  2002/02/27 00:32:19  pouaite
  modifs velues

  Revision 1.16  2002/02/24 22:13:56  pouaite
  modifs pour la v2.3.5 (selection, scrollcoin, plopification, bugfixes)

  Revision 1.15  2002/02/02 23:49:17  pouaite
  plop

  Revision 1.14  2002/01/31 23:45:00  pouaite
  plop

  Revision 1.13  2002/01/20 20:53:22  pouaite
  bugfix configure.in && http_win.c pour cygwin + 2-3 petis trucs

  Revision 1.12  2002/01/19 19:56:09  pouaite
  petits crochets pour la mise en valeur de certains messages (cf changelog)

  Revision 1.11  2002/01/14 23:54:06  pouaite
  reconnaissance des posts effectué par l'utilisateur du canard (à suivre...)

  Revision 1.10  2002/01/13 17:14:34  pouaite
  préparation pour dacode 1.4

  Revision 1.9  2002/01/13 15:19:00  pouaite
  double patch: shift -> tribune.post_cmd et lordOric -> tribune.archive

  Revision 1.8  2002/01/10 09:31:40  pouaite
  j'avais oublie un fichier.. dernier morceau du pacth de glandium...

  Revision 1.7  2001/12/17 16:01:33  pouaite
  fix suite à un petit changement dans le /backend.rdf

  Revision 1.6  2001/12/17 00:18:04  pouaite
  changement du format du backend -> on utilise desormais le /backend.rdf

  Revision 1.5  2001/12/16 21:51:22  pouaite
  *** empty log message ***

  Revision 1.4  2001/12/16 20:28:45  pouaite
  bugfixes divers

  Revision 1.3  2001/12/02 19:04:42  pouaite
  suppression de messages de debug...

  Revision 1.2  2001/12/02 18:24:16  pouaite
  modif (virage) des regexp pour le texte des news, trop chiant à maintenir, du coup ça devrait marcher à nouveau sous bsd

*/

#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include "coincoin.h"
#include "http.h"
#include "regexp.h"

/* fonction a-la-con: lecture de toutes les données en mémoire.. */
unsigned char *
gros_read(HttpRequest *r, char *what)
{
  unsigned char *s;
  int bchunk = 1024;
  int bsize;

  /* on lit tout en un coup */
  bsize = bchunk;
  s = malloc(bsize+1); strcpy(s,"plop! c'est raté");
  if (s) {
    int got;
    int bi;
    bi = 0;
    
    /* attention : les ames sensible pourraient etre choques
       par la brutalite de ce qui va suivre ... */
    while ((got=http_read(r, s+bi, bchunk)) > 0 && r->error == 0) {
      bi += got;
      s[bi] = 0;
      bsize += bchunk;
      if (bsize > 300000) {
	  BLAHBLAH(0, myprintf("%s: c'est trop gros (bsize=%d!) on coupe\n",
			       what, bsize));
	  break;
      }
      s = realloc(s, bsize+1);
      if (!s) break;
    }
    if (got == -1) {
      fprintf(stderr, "probleme pendant la lecture de %s: %s\n", what, http_error());
      if (s) free(s);
      s = NULL;
    } else {
      s[bi] = 0;
    }
    BLAHBLAH(0, myprintf("%s, lu: %<mag %s>\n", what, s));
  }
  return s;
}

/*
  traitement DLFP

  initialisation des news et de la tribune (oui pas propre, l'init de la tribune aurait sa place
  dans coincoin_tribune.c ..)
*/

DLFP *
dlfp_create()
{
  DLFP *dlfp;
  dlfp = (DLFP*) calloc(1, sizeof(DLFP));

  dlfp->updated = 0;
  dlfp->news = NULL;

  /* init du log des messages */
  dlfp->tribune.msg = NULL;
  dlfp->tribune.last_post_id = -1;
  dlfp->tribune.last_post_timestamp = 0;
  dlfp->tribune.nbsec_since_last_msg = 0;
  dlfp->tribune.local_time_last_check = time(NULL);

  dlfp->tribune.just_posted_anonymous = 0;
  dlfp->com = NULL;
  dlfp->xp_old = -1000;
  dlfp->xp = -1000;
  dlfp->xp_change_flag = 0;
  dlfp->comment_change_flag = 0;
  dlfp->votes_max = -1;
  dlfp->votes_cur = -1;

  dlfp->fortune = NULL; dlfp->CPU = -1.00;
  
  dlfp->msg = NULL;

  strncpy(dlfp->tribune.last_post_time, "xx:xx", 5);
  return dlfp;
}


/*
  efface une news (auqnd elle est trop vieille)
*/
int
dlfp_delete_news(DLFP *dlfp, int id) {
  DLFP_news *n, *p;

  p = NULL;
  n = dlfp->news;
  while (n) {
    if (n->id == id) break;
    p = n;
    n = n->next;
  }
  if (n == NULL) {
    fprintf(stderr, "COIN ! impossible de detruire la news id=%d\n", id);
    return 1;
  }
  if (p) {
    p->next = n->next;
  } else {
    dlfp->news = n->next;
  }

  if (n->titre) free(n->titre);
  if (n->txt) free(n->txt);
  if (n->auteur) free(n->auteur);
  if (n->topic) free(n->topic);
  if (n->mail) free(n->mail);
  if (n->url) free(n->url);
  free(n);
  dlfp->updated = 1;
  flag_news_updated = 1;
  return 0;
}

DLFP_news *
dlfp_insert_news(DLFP *dlfp)
{
  DLFP_news *n;
  ALLOC_OBJ(n, DLFP_news);
  n->next = dlfp->news;
  dlfp->news = n;
  return n;
}

/* renvoie null si la news n'est pas trouvee */
DLFP_news *
dlfp_find_news_id(DLFP *dlfp, int id)
{
  DLFP_news *n;

  n = dlfp->news;
  while (n) {
    assert(n->id > 0); /* piege a bugs */
    if (n->id == id) break;
    n = n->next;
  }
  return n;
}

DLFP_news *
dlfp_find_prev_news(DLFP *dlfp, int id)
{
  DLFP_news *n;

  n = dlfp->news;
  while (n) {
    if (n->next) {
      if (n->next->id == id) {
	return n;
      }
    } else {
      if (dlfp->news->id == id) {
	return n;
      }
    }
    n = n->next;
  }
  return NULL;
}

DLFP_news *
dlfp_find_next_news(DLFP *dlfp, int id) {
  DLFP_news *n;

  n = dlfp->news;
  while (n) {
    if (n->id == id) {
      if (n->next) {
	return n->next;
      } else return dlfp->news;
    }
    n = n->next;
  }
  return NULL;
}

int
dlfp_count_news(DLFP *dlfp)
{
  int cnt;
  DLFP_news *n;

  cnt = 0;
  n = dlfp->news;
  while (n) {
    cnt++;
    n = n->next;
  }
  return cnt;
}

/* 
   renvoie l'id de la ieme news non encore lue
*/
int
dlfp_unreaded_news_id(DLFP *dlfp, int i)
{
  DLFP_news *n;
  int cnt;

  n = dlfp->news;
  cnt = 0;
  while (n) {
    if (n->flag_unreaded == 1) {
      cnt++;
      if (cnt == i) return n->id;
    }
    n = n->next;
  }

  return -1;
}

void
dlfp_unset_unreaded_news(DLFP *dlfp)
{
  DLFP_news *n;

  n = dlfp->news;
  while (n) {
    if (n->flag_unreaded == 1) {
      n->flag_unreaded = -1;
    }
    n = n->next;
  }
}


static
int
dlfp_is_news_too_old(char date[11])
{
  struct tm d;
  time_t t, t2;
  int ndays;
  char date2[11]; 

  assert(date[10] == 0);

  strcpy(date2, date); 
  assert(date2[4] == '/'); date2[4] = 0; 
  assert(date2[7] == '/'); date2[7] = 0;
  d.tm_sec = 0; d.tm_min = 0; d.tm_hour = 0; d.tm_isdst = 0;
  //  printf("%s %s %s\n", date2, date2+5, date2+8);
  assert(sscanf(date2, "%d", &d.tm_year) == 1);
  assert(sscanf(date2+5, "%d",&d.tm_mon) == 1);
  assert(sscanf(date2+8, "%d",&d.tm_mday) == 1);
  assert(d.tm_year >= 2001); 
  assert(d.tm_mon >= 1 && d.tm_mon <= 12); 
  assert(d.tm_mday >= 1 && d.tm_mday <= 31);
  BLAHBLAH(3,myprintf("la date est y=%d , m=%d, d=%d\n", d.tm_year, d.tm_mon, d.tm_mday));
  d.tm_mon--; /* pour mktime , janvier = 0, decembre = 11 ... c'est debile */
  d.tm_year -= 1900; /* ca aussi c'est debile */
  t = mktime(&d);
  t2 = time(NULL);

  BLAHBLAH(3,myprintf("-> conversion en nb de secondes: t=%ld , maintenant=%ld\n", t, t2));
  t = difftime(t2, t);
  ndays = (t / 3600 / 24);
  BLAHBLAH(3,myprintf("age=%d secondes == %d jours\n", t, ndays));
  return (ndays > Prefs.news_max_nb_days);
}

/* elimine les news trop agees --
   cette fonction ne sert que pour les news qui ne sont plus listees
   dans le fichier de backend (short.php3) et qui sont devenues trop
   vieilles (plus de 'Prefs.news_max_nb_days' jours d'age)
*/
static
void
dlfp_remove_old_news(DLFP *dlfp)
{
  DLFP_news *n, *nn;
  
  n = dlfp->news;
  while (n) {
    /* on sauve le suivant au cas ou n est detruite */
    nn = n->next; 
    if (dlfp_is_news_too_old(n->date)) {
      BLAHBLAH(2, printf("Destruction de la news id=%d (tres vielle, date=%s, n'etait plus dans %s)\n",n->id, n->date, Prefs.path_news_backend));
      dlfp_delete_news(dlfp, n->id);
    }
    n = nn;
  }
}

static
int
dlfp_updatenews_txt(DLFP *dlfp, int id)
{
#define MAX_URL 100

  DLFP_news *n;
  char URL[512];
  int err;
  HttpRequest r;

  char *date = NULL, *auteur = NULL, *section = NULL;
  char *texte = NULL, *liens = NULL;

  /* la table des liens (donnés en fin de news) */
  char *url_tab[MAX_URL];
  char *url_tab_desc[MAX_URL];
  int nb_url = 0;

  err = 1;
  n = dlfp_find_news_id(dlfp, id);
  if (n == NULL) return 1;



  BLAHBLAH(1,myprintf("Update du texte de la news: '%<cya %s>' (date='%10s', id %<cya %d>)\n", n->titre, n->date, n->id));

  if (dlfp_is_news_too_old(n->date)) {
    BLAHBLAH(1, myprintf("Elle est trop vieille (date=%s, age max=%d jours)\n", n->date, Prefs.news_max_nb_days));
    return 0;
  }

  /* -1 -> pour le threshold des commentaires
     7 -> pour le theme printable (ce qui evite les commentaires)
  */

  if ((Prefs.debug & 2) == 0) {
    snprintf(URL, 512, "%s%s",n->url, Prefs.path_end_news_url);
    BLAHBLAH(1,printf("get %s\n",URL));
  } else {
    snprintf(URL, 512, "%s/wmcoincoin/test/%d,0,-1,6.html", getenv("HOME"), n->id);
    myprintf("DEBUG: ouverture de '%<RED %s>'\n", URL);
  }
  wmcc_init_http_request(&r, URL);
  http_request_send(&r);

  if (r.error == 0) {
    char *s;
    char *p, *p2;

    err = 2;

    s = gros_read(&r, n->url);
    http_request_close(&r);

    n->heure = 0;

    
    extract_news_txt(s, &date, &auteur, &section, &texte, &liens); /* fonction definie dans regexp.c */

    /*    fprintf(stderr, "\n--\n%s\n--\n", p);*/
    /*
    {
      int res;
      res = regexp_extract(s, pat_news, &date,
			   &texte, &liens);
      if (!res) {
	printf("fuck!\n");
      }
      }
*/
    
    if (texte == NULL) { err = 7; goto ouups1; }

    //myprintf("liens: '%<BLU %s>'\n", liens);
    
    p2 = liens;
    while (p2) {
      char *p3;


      url_tab[nb_url] = NULL;
      /* bourrin .. au moindre problème on laisse tomber */

      /* essai 1 : y'a t-il un onmouseover ? (pour chopper le vrai lien) */
      p3 = after_substr(p2, " onmouseover=\"javascript: window.status='");
      if (p3) {
	url_tab[nb_url] = p3;
	p3 = strchr(p3, '\'');
	if (p3) {
	  *p3 = 0; p3++;
	}
      }

      /* essai 2, il y a juste un href= */
      if (url_tab[nb_url] == NULL) {
	p3 = after_substr(p2, "<a href=\"");
	if (p3 == NULL) goto stop_url;
	url_tab[nb_url] = p3;
	p3 = strchr(p3, '"');
	if (p3 == NULL) goto stop_url;
	*p3 = 0; p3++;
      }

      /* chope la descriptuion de l'url */
      p3 = strstr(p3, ">");
      if (p3 == NULL) goto stop_url;
      url_tab_desc[nb_url] = p3+1;
      p3 = strstr(p3, "<");
      if (p3 == NULL) goto stop_url;
      *p3 = 0; p3++;
      //      printf("LINK='%s' , DESC='%s'\n", url_tab[nb_url], url_tab_desc[nb_url]);
      nb_url++;
      p2 = p3;
    }

  stop_url:      
    if (nb_url > 0) {
      /* reformatage de la liste des urls, c'est aussi un peu bourrin */
#define URL_BSIZE 8192
      char url_buff[URL_BSIZE];
      int i;
      int l = 0;
      strcpy(url_buff, "<p align=center>");
      url_buff[URL_BSIZE-1] = 0;
      for (i=0; i < nb_url; i++) {
	l = strlen(url_buff);
	if (l < URL_BSIZE-1) {
	  snprintf(url_buff+l, URL_BSIZE-l, "<a href=\"%s\">%s</a>%s", url_tab[i], url_tab_desc[i],
		   i != nb_url-1 ? "<br>" : "</p>");
	  if (strlen(url_buff) >= URL_BSIZE-1) {
	    url_buff[l] = 0; /* on tronque, tant pis pour l'url qui deborde */
	  }
	}
      }
      n->txt = malloc(strlen(texte) + strlen(url_buff) + 1);
      strcpy(n->txt, texte);
      strcat(n->txt, url_buff);
    } else n->txt = strdup(texte);

    if (date) {
      p = strstr(date, "à");
      if (p) {
	while (!isdigit((unsigned)*p) && *p) p++;
	n->heure = atoi(p) * 60;
	p += 2;
	while (!isdigit((unsigned)*p) && *p) p++;      
	n->heure += atoi(p);     
      }
      BLAHBLAH(2,myprintf("date: '%<BLU %s>' => heure: %d\n", date, n->heure));
    }
    
    n->auteur = strdup(auteur);
    n->topic = strdup(section);

    p = n->txt;
    while (*p) { if (((unsigned char)*p) <= ' ') *p = ' '; p++; }
    
    n->nb_comment = 0;
    BLAHBLAH(2,myprintf("on vient de chopper la news !!\n"));
    BLAHBLAH(3,myprintf(" --> %<YEL %s>\n", n->txt));
    p2++;
	    


    err = 0; n->nb_comment = 0;

    free(s);
    
  } else {
    myfprintf(stderr, "erreur pendant le d/l de '%<YEL %s>' : %<RED %s>\n", URL, http_error());
  }
 ouups1:
  if (err) {
    myfprintf(stderr,"%<RED Erreur lors du rapatriement> de '%s' (err=%d)\n", URL, err);
  }
  
  if (texte) free(texte);
  if (liens) free(liens);
  if (date) free(date);
  if (auteur) free(auteur);
  if (section) free(section);

  return err;
}


FILE *
open_site_file(char *base_name)
{
  char fname[2048], rname[2048];
  FILE *fsave;
  int retry, tantpis;


  /* attention aux ames sensibles */
  snprintf(fname,2048,"%s/.wmcoincoin/%s/%s", getenv("HOME"), Prefs.site_root, base_name);
  if ((fsave = fopen(fname, "rt")) == NULL) {
    for (tantpis = 0, retry = 0; retry < 3; retry++) {
      if ((fsave = fopen(fname, "wt")) == NULL) {
	if (retry == 2) {
	  myprintf("impossible de creer le fichier '%s' \n[erreur: '%<red %s>']\n,"
		   "c'est pas grave mais c'est dommage\n", fname,strerror(errno));
	  tantpis = 1; break;
	}
      } else { fclose(fsave); break; }
      snprintf(rname,2048,"%s/.wmcoincoin/%s", getenv("HOME"), Prefs.site_root);
      if (mkdir(rname, -1)) {
	if (retry == 1) {
	  myprintf("impossible de creer le repertoire '%s'\n[erreur: '%<red %s>']\n,"
		   "c'est pas grave mais c'est dommage\n", rname,strerror(errno));
	  tantpis = 1; break;
	}
	snprintf(rname,2048,"%s/.wmcoincoin", getenv("HOME"));
	if (mkdir(rname,-1)) {
	  myprintf("impossible de creer le repertoire '%s'\n[erreur: '%<red %s>']\n,"
		   "c'est pas grave mais c'est dommage\n", rname, strerror(errno));
	  tantpis = 1; break;
	}
      }
    }
    
    fsave = fopen(fname, "rt");
  }
  return fsave;
}

/*
  appelle au demarrage de coincoin:
  lit les id des news deja connues, pour eviter qu'elles
  ne soient resignalees comme etant de nouvelles news
*/
static void
dlfp_get_newslues(DLFP *dlfp)
{
  char s[2048];
  FILE *fsave;

  s[2047] = 0;

  fsave = open_site_file("newslues");
  if (fsave) {
    int id;
    DLFP_news *n;

    n = dlfp->news;
    while (n) {
      n->flag_unreaded = 1; /* on est au premier lancement, toutes les news
			       on ete marquees non lues precedement pour 
			       empecher la diode de clignoter betement,
			       si maintenant on s'apercoit qu'il y en a de nouvelles
			       on, on les declare ici */
      n = n->next;
    }

    fgets(s, 2047, fsave); // on ouble la premiere ligne (=="#do not edit this file")
    while (fgets(s,2047,fsave)) {
      id = atoi(s);
      n = dlfp_find_news_id(dlfp, id);
      if (n) {
	n->flag_unreaded = 0;
      }
    }
    fclose(fsave);
  }
}

static void
dlfp_update_newslues(DLFP *dlfp)
{
  char fname[2048];
  FILE *fsave;
  int need_update;
  DLFP_news *n;
  

  n = dlfp->news;
  need_update = 0;
  while (n) {
    assert(n->id > 0); /* piege a bugs */
    //    myprintf("update_newslues: n->id = %d, n->flag_unreaded=%d n->title=%<yel %s>\n", n->id, n->flag_unreaded, n->titre);
    if (n->flag_unreaded == -1) { 
      n->flag_unreaded = 0;
      need_update = 1;
    }
    n = n->next;
  }

  //  printf("update_newslues: need_update=%d\n", need_update);

  if (need_update == 0) return;

  /* attention aux ames sensibles */
  snprintf(fname,2048,"%s/.wmcoincoin/%s/newslues", getenv("HOME"), Prefs.site_root);
  if ((fsave = fopen(fname, "wt"))) {
    fprintf(fsave, "#do not edit this file\n");
    n = dlfp->news;
    while (n) {
      assert(n->id > 0); /* piege a bugs */
      if (n->flag_unreaded == 0) { 
	fprintf(fsave, "%d\n", n->id);
      }
      n = n->next;
    }
    fclose(fsave);
  } else {
    BLAHBLAH(1,myprintf("impossible d'ouvrir '%s' en ecriture\n(erreur: %s)\n",
			fname, strerror(errno)));
  }
}


/*
  recupere http://linuxfr.org/short.php3
  et met a jour la liste des news en consequence
*/
void
dlfp_updatenews(DLFP *dlfp)
{
  HttpRequest r;
  static int first_run = 1;
  char path[2048];
  static int transfert_cnt = 0; /* incrémenté a chaque transfert réussi */

  static char *news_last_modified = NULL;


  BLAHBLAH(3,printf("dlfp update_news...\n"));

  /* on se protege */
  flag_updating_news++;

  if ((Prefs.debug & 2) == 0) {
    snprintf(path, 2048, "%s%s/%s", (strlen(Prefs.site_path) ? "/" : ""), Prefs.site_path, Prefs.path_news_backend); 
  } else {
    snprintf(path, 2048, "%s/wmcoincoin/test/short.php3", getenv("HOME")); 
    myprintf("DEBUG: ouverture de '%<RED %s>'\n", path);
  }

  wmcc_init_http_request(&r, path);
  if (Prefs.use_if_modified_since) r.p_last_modified = &news_last_modified;
  http_request_send(&r);

  if (r.error == 0) {
    const char *news_item_sign = "<item>";
    const char *news_item_endsign = "</item>";
    const char *news_title_sign = "<title>";
    const char *news_title_endsign = "</title>";
    const char *news_url_sign = "<link>";
    const char *news_url_endsign = "</link>";

    char title[512];
    char s_id[10];
    char date[11];
    int id;
    char s[512], url[512], base_url[512];
    int l_cnt;
    int news_err;

    ALLOW_X_LOOP;
    news_err = 0;

    BLAHBLAH(2,printf("le transfert semble ok\n"));
    l_cnt = -1;
    s[0] = 0;
    while (http_get_line(&r, s, 512)>0) {
      ALLOW_X_LOOP;

      /* faut sauter l'entete */
      if (strstr(s, "</channel>")) l_cnt = 0;
      if (l_cnt == -1) continue;

      if (strstr(s, news_item_sign)) { 
	if (l_cnt != 0) { news_err = 1; break; }
	l_cnt = 1; 
	id = -1;
	url[0] = 0; s_id[0] = 0; date[0] = 0; title[0] = 0; base_url[0] = 0;
      } else if (strstr(s, news_title_sign)) {
	char *p;
	if (l_cnt != 1) { news_err = 2; break; }
	l_cnt = 2;
	p = strstr(s, news_title_endsign);
	if (p == NULL) { news_err = 3; break; }
	*p = 0;
	p = strstr(s, news_title_sign) + strlen(news_title_sign);
	convert_to_ascii(title, p, 512);
      } else if (strstr(s, news_url_sign)) {
	char *p;
	if (l_cnt != 2) { news_err = 4; break; }
	l_cnt = 3;
	p = strstr(s, news_url_endsign);
	if (p == NULL) { news_err = 5; break; }
	*p = 0;
	p = strstr(s, news_url_sign) + strlen(news_url_sign);
	strncpy(url, p, 512); url[511] = 0;
      } else if (strstr(s, news_item_endsign)) {

	char base_url[512];
	char *p;
	int i;
	DLFP_news *n = NULL;

	memset(s, 0, 512); /* anti bug temporaire */
	if (l_cnt != 3) { news_err = 6; break; }
	l_cnt = 0;

	/* on recupere l'url en enlevant le debut, la fin, ainsi que le numero de port */
	if (!strstr(url,"http://") && !strstr(url,"https://") && url[0] == '/') {	/* on gere aussi le cas où l'url ne contient pas http://... mais juste /2001/11/... */
	  news_err = 8; break;
	}	  

	
	if (url[0] != '/') {
	  p = strchr(url+7,'/');
	  while (*p == '/') p++;
	} else {
	  p = url+1;
	}

	assert(p); 
	assert(p>url);

	base_url[0] = '/';
	strncpy(base_url+1, p, 512); base_url[511]=0;

	/* suppression du numero de port si on le trouve dans l'url (A TESTER) */
	p = strstr(base_url, ":");
	if (p) {
	  char *pp;
	  pp = p;
	  p++;
	  while (*p <= '9' && *p >= '0' && *p) p++;
	  memmove(pp, p, strlen(p));
	}
	    
	p = strstr(base_url, ",");
	if (p && p != base_url) {
	  *p = 0;
	} else {
	  BLAHBLAH(1,myprintf("le backend '%s' n'a pas l'air tres orthodoxe...\n", Prefs.path_news_backend));
	  news_err = 7; break;
	}

	/* recupere l'id */
	i = p-base_url-1;
	while (i > 0 && base_url[i] != '/') i--;
	if (i == 0) {
	  news_err = 9;
	  p = NULL; break;
	} else {
	  strncpy(s_id, base_url+i+1, 9); s_id[9] = 0;
	}
	
	/* on enchaine pour recuperer la date */
	if (i > 10) {
	  strncpy(date, base_url+i-10, 10); date[10] = 0;
	} else {
	  p = NULL; news_err = 10; break;
	}

	id = atoi(s_id);
	if (id <= 0) {
	  fprintf(stderr, "probleme dans le backend des news -> ligne url news: '%s' -> id='%s'=%d\n",url, s_id, id);
	  news_err = 11; break;
	}

	flag_news_updated = 1;

	ALLOW_X_LOOP;

	if (dlfp_is_news_too_old(date)) {
	  BLAHBLAH(1, printf("La news id=%d est trop vieille (date=%s, age max=%d), on la vire\n",id,date,Prefs.news_max_nb_days));
	  if (dlfp_find_news_id(dlfp,id)) {
	    dlfp_delete_news(dlfp, id);
	  }
	} else if ((n=dlfp_find_news_id(dlfp,id))) {
	  BLAHBLAH(1,printf("** Cette news est deja enregistree, pas de probleme\n"));
	  if (n->txt == NULL) {
	    BLAHBLAH(1,printf("** Par contre, son texte n'a pas encore ete mis a jour on va reessayer... problème?\n"));
	    dlfp_updatenews_txt(dlfp, id);
	  }
	} else {
	  ALLOW_X_LOOP;
	  n = dlfp_insert_news(dlfp); assert(n);
	  n->titre = strdup(title);
	  n->url = strdup(base_url);
	  n->txt = NULL;
	  n->auteur = NULL; 
	  n->topic = NULL;
	  n->mail = strdup("mailpic");
	  strncpy(n->date, date, 11); date[10] = 0;
	  n->id = id;
	  n->flag_unreaded = 1-first_run;
	  dlfp->updated = 1;
	  BLAHBLAH(2,printf(" . titre='%s'\n", title));
	  BLAHBLAH(2,printf(" . auteur='%s', mail='%s'\n", n->auteur, n->mail));
	  BLAHBLAH(2,printf("** News AJOUTEE\n"));
	  dlfp_updatenews_txt(dlfp, id);
	  flag_news_updated = 1;
	    //if (dlfp_updatenews_txt(dlfp, id) != 0) return;

	    //	    if (dlfp_count_news(dlfp) > 4) break;
	    //	    break;
	    //	    dlfp_updatenews_txt(dlfp, 2293);
	}
      } /* nouvelle news ajoutée */
    } /* while http_get_line */
    if (news_err != 0) {
      myfprintf(stderr,"%<RED OUUUPS!> %<grn dlfp_updatenews> il y a eu une erreur (err=%d) dans le parsage du backend des news\n", news_err);
    }
    http_request_close(&r);
    transfert_cnt++;
  } else {
    myfprintf(stderr, "erreur lors du transfert de '%<YEL %s>' : %<RED %s>\n", Prefs.path_news_backend, http_error());
  }
  /* elimine les eventuelles news trop vielles, et qui ne sont plus
     dans short.php3 */
  dlfp_remove_old_news(dlfp);

  ALLOW_X_LOOP;
      
  if (dlfp_count_news(dlfp) == 0) {
    dlfp->updated = 1; // pour forcer l'affiche d'un message particulier quand ttes les news sont trop vieilles 
  }

  if (first_run && transfert_cnt) {
    BLAHBLAH(2,myprintf("recherche des news lues lors de la derniere session de wmcoincoin\n"));
    dlfp_get_newslues(dlfp);
    first_run = 0;
  } else if (first_run == 0) {
    BLAHBLAH(2,myprintf("maj du fichier de sauvegarde des numeros de news deja lues\n"));
    if (transfert_cnt >= 1) {
      dlfp_update_newslues(dlfp);
    }
  }

  flag_updating_news--;
}


/* gestion des commentaire ... */

/*
  efface un commentaire (auqnd elle est trop vieille)
*/
static int
dlfp_yc_delete_comment(DLFP *dlfp, int cid) {
  DLFP_comment *n, *p;

  p = NULL;
  n = dlfp->com;
  while (n) {
    if (n->com_id == cid) break;
    p = n;
    n = n->next;
  }
  if (n == NULL) {
    fprintf(stderr, "COIN ! impossible de detruire le commentaire cid=%d\n", cid);
    return 1;
  }
  if (p) {
    p->next = n->next;
  } else {
    dlfp->com = n->next;
  }
  free(n);
  return 0;
}

static DLFP_comment *
dlfp_yc_insert_comment(DLFP *dlfp)
{
  DLFP_comment *n;
  ALLOC_OBJ(n, DLFP_comment);
  n->next = dlfp->com;
  dlfp->com = n;
  return n;
}

/* renvoie null si la news n'est pas trouvee */
static DLFP_comment *
dlfp_yc_find_cid(DLFP *dlfp, int cid)
{
  DLFP_comment *n;

  n = dlfp->com;
  while (n) {
    assert(n->com_id > 0); /* piege a bugs */
    if (n->com_id == cid) break;
    n = n->next;
  }
  return n;
}

DLFP_comment *
dlfp_yc_find_modified(DLFP *dlfp, DLFP_comment *prev)
{
  DLFP_comment *c;
  
  c = dlfp->com;
  if (prev) c = prev->next;
  
  while (c) {
    if (c->modified) return c;
    c = c->next;
  }
  return NULL;
}

void
dlfp_yc_clear_modified(DLFP *dlfp)
{
  DLFP_comment *c;
  
  c = dlfp->com;
  
  while (c) {
    c->modified = 0;
    c = c->next;
  }
}

void
dlfp_yc_update_comments(DLFP *dlfp)
{
  HttpRequest r;
  char path[2048];
  static int first_run = 1;
  static char *comments_last_modified = NULL;

  if (Prefs.user_cookie == NULL && Prefs.force_fortune_retrieval == 0) return;

  BLAHBLAH(3,printf("dlfp_yc_update_comments...\n"));

  if ((Prefs.debug & 2) == 0) {
    snprintf(path, 2048, "%s%s/%s", (strlen(Prefs.site_path) ? "/" : ""), Prefs.site_path, Prefs.path_myposts);
  } else {
    snprintf(path, 2048, "%s/wmcoincoin/test/posts.php3", getenv("HOME"));
  }


  wmcc_init_http_request_with_cookie(&r, path);
  if (Prefs.use_if_modified_since) { r.p_last_modified = &comments_last_modified; }
  http_request_send(&r);

  if (r.error == 0) {
    char *s, *p, *p2, *p3;
    int err;

    BLAHBLAH(2,printf("le transfert de '%s' semble ok\n", Prefs.path_myposts));
    err = 0;

    /* on lit tout en un coup */
    s = gros_read(&r, Prefs.path_myposts);
    http_request_close(&r);
    
    if (s == NULL) { err = 1; goto ouups;}
    
    /* récuperation du cpu */
    p = strstr(s, "CPU</a>:");
    flag_updating_comments++;
    if (p) {
      dlfp->CPU = atof(p+8);
      BLAHBLAH(1,printf("cpu=%f\n", dlfp->CPU));
    } else dlfp->CPU = -1.0;
    flag_updating_comments--;

    /* recuperation de la fortune */
    p = strstr(s, "</td><td class=\"newstext\"");
    if (p) {
      p += 20;
      while (*p && *p != '>') p++;
      if (*p == 0) {err = 2; goto fortune_erreur;}
      p++;
      p2 = strstr(p, "</td>");
      if (p2 == NULL) {err = 3; goto fortune_erreur;}
      *p2 = 0; /* on tronque brutalement, ça ne va pas gener la suite puisqu'on est tout à la fin de la page */
      if (p2-p > 10000) { p[10000] = 0; } /* les fortune de 10ko, non merci */

      flag_updating_comments++;
      if (dlfp->fortune) free(dlfp->fortune);
	
      /*
      dlfp->fortune = malloc(strlen(p) + 1 + strlen("<p align=center></p>"));
      sprintf(dlfp->fortune, "<p align=center>%s</p>", p);
      */
      dlfp->fortune = strdup(p);
      flag_updating_comments--;
      
      BLAHBLAH(1, myprintf("fortune: \"%<yel %s>\"\n", dlfp->fortune));
    } else {
    fortune_erreur:
      BLAHBLAH(0,myprintf("la structure de '%s' a été modifiée ?... je ne trouve pas la fortune (err=%d)\n", Prefs.path_myposts, err));
    }

    /* si on est dans un cas de 'force_fortune_retrieval', on se barre maintenant */
    if (Prefs.user_cookie == NULL) goto caroule;


    p = strstr(s,"class=\"logoinfo\"");
    if (p != NULL) {
      int xp;
      if (regexp_extract(p, pat_xp, &xp)) {
	if (dlfp->xp != xp && dlfp->xp_change_flag == 0 && dlfp->xp > -1000) {
	  dlfp->xp_change_flag = 1;
	}
	if (dlfp->xp != xp) {
	  dlfp->xp_old = dlfp->xp;
	  dlfp->xp = xp;
	}
      }

      p = strstr(p, "<br");
      if (p != NULL)
	regexp_extract(p, pat_votes, &(dlfp->votes_max), &(dlfp->votes_cur));
    } else {
      printf("mmmh, bizarre la page '%s' ... pas de champ 'loginfo' ..\n", Prefs.path_myposts);
    }

    //modif DACODE 1.4
    //p = strstr(s,"?order=news_id"); // on se positionne bien...
    p = strstr(s,"/comments/thread.php"); // on se positionne bien...
    if (p == NULL) {
      if (strstr(s, "Vous n'avez encore rien posté ???") != NULL)
	goto ouups1;
      myprintf("'%s': ouuups, soit (a) le cookie n'est plus valide, soit (b) la structure de la page a changé, soit (c) obiwan kenobi\n", Prefs.path_myposts);
      err = 2; goto ouups1;
    }
    
    //modif DACODE 1.4
    /* err = 3;
    p = strstr(p, "<tr>"); // debut du tableau
    if (p == NULL) goto ouups1;
    err++;
    */

    flag_updating_comments = 1;

    /* avant tout, on marque tous les commentaires comme vieux */
    {
      DLFP_comment *c;
      c = dlfp->com;
      while (c) {
	c->old = 1;
	c = c->next;
      }
    }

    /* lecture des lignes du tableau */
    do {
      int nid, cid,nbcom;
      DLFP_comment *c;

      nid = -1; cid = -1; nbcom = -1;
      p = strstr(p, "news_id=");
      if (p == NULL) break; /* fin des commentaires */
      nid = atoi(p+8);
      p = strstr(p, "com_id=");
      if (p == NULL) {err = 4; goto ouups2;}
      cid = atoi(p+7);
      // maintenant on saute les trois colonnes suivantes
      p = strstr(p, "<td class=\"boxtext\">");
      if (p == NULL) {err = 5; goto ouups2;}
      p = strstr(p+1, "<td class=\"boxtext\">");
      if (p == NULL) {err = 6; goto ouups2;}
      p = strstr(p+1, "<td class=\"boxtext\">");
      if (p == NULL) {err = 7; goto ouups2;}

      p2 = strstr(p, "</td>"); // on repere la fin de la colonne
      if (p2 == NULL) {err = 8; goto ouups2;}
      //p3 = p2; while (p3 > p && *p3 != '>') p3--; /* on remonte au tag fermant precedant */
      p3 = strstr(p, "after=1\">");
      if (p3 == NULL || p3 > p2) {
      	nbcom = 0;
      } else {
	nbcom = atoi(p3+9);
      }
      BLAHBLAH(1,myprintf("NBCOM: '%.20s'->%<YEL %d>\n",p3 ? p3+9 : "<erreur>",nbcom));
      //      }
      p = p2;

      /* test du commentaire */
      c = dlfp_yc_find_cid(dlfp, cid);
      if (c) {
	if (c->nb_answers != nbcom) {
	  BLAHBLAH(1,myprintf("il y a eu %d/%d reponses au commentaire %d\n", 
			      nbcom-c->nb_answers,c->nb_answers,cid));
	  c->modified = 1;                /* on signale que ce commentaire a été modifié */
	  dlfp->comment_change_flag = 1;  /* mais c'est CE flag qui déclenche le flamometre 
					     (et il est remis à zéro dès que le flamometre a été déclenché)
					   */
	}
	c->nb_answers = nbcom;
	c->old = 0;
      } else {
	c = dlfp_yc_insert_comment(dlfp);
	c->news_id = nid;
	c->com_id = cid;
	c->nb_answers = nbcom;
	c->old = 0;
	c->modified = (nbcom == 0 || first_run ==1) ? 0 : 1;
	dlfp->comment_change_flag = c->modified;  /* ce flag qui déclenche le flamometre */
	BLAHBLAH(1,myprintf("NOUVEAU COMMENTAIRE %d, avec %d reponses\n",
			    cid, nbcom));
      }
    } while (p);

    /* suppression des commentaires qui ne sont plus listes dans myposts.php3 */
    {
      DLFP_comment *c,*n;
      c = dlfp->com;
      while (c) {
	BLAHBLAH(1,printf("commentaire %d, nid=%d, nrep=%d, modified=%d,old=%d\n",
	       c->com_id, c->news_id, c->nb_answers, c->modified, c->old));
	if (c->old) {
	  BLAHBLAH(1,myprintf("DESTRUCTION DU COMMENTAIRE %d (trop vieux, n'est plus liste dans '%s')\n",
			      c->com_id, Prefs.path_myposts));
	  n = c->next;
	  dlfp_yc_delete_comment(dlfp,c->com_id);
	  c = n;
	} else {
	  c = c->next;
	}
      }
    }
    first_run = 0;
    goto caroule;
  ouups2:
    myprintf("erreur dans '%s', le code html a du changer... (err=%d)\n",Prefs.path_myposts, err);
  caroule:
    flag_updating_comments = 0;
  ouups1:
    free(s);
  ouups:
    if (err) {
      myfprintf(stderr,"dlfp_yc_update_comments: %<RED Erreur lors du rapatriement> de '%s' (err=%d)\n", path, err);
    }
  } else {
    myfprintf(stderr, "erreur pendant la récupération de '%<YEL %s>' : %<RED %s>\n", path, http_error());
  }
}


/* gestion des messages */

/*
  efface un message (par ex. quand il est trop vieux et ne figure plus dans linuxfr/messages)
*/
#ifdef TEST_MEMLEAK
static int
dlfp_msg_delete_message(DLFP *dlfp, int mid) {
  DLFP_message *m, *p;

  p = NULL;
  m = dlfp->msg;
  while (m) {
    if (m->mid == mid) break;
    p = m;
    m = m->next;
  }
  if (m == NULL) {
    fprintf(stderr, "COIN ! impossible de detruire le message mid=%d\n", mid);
    return 1;
  }
  if (p) {
    p->next = m->next;
  } else {
    dlfp->msg = m->next;
  }
  free(m);
  return 0;
}
#endif

/* insere (en classant) */
static DLFP_message *
dlfp_msg_insert_message(DLFP *dlfp, int mid)
{
  DLFP_message *m, *p, *n;
  ALLOC_OBJ(m, DLFP_message);

  p = NULL; n = dlfp->msg;
  while (n) {
    if (n->mid > mid) {
      break;
    }
    assert(n->mid != mid); /* sinon ça sent le bug */
    p = n;
    n = n->next;
  }
  
  if (p) {
    p->next = m;
  } else {
    dlfp->msg = m;
  }
  m->next = n;
  m->mid = mid;
  return m;
}

/* renvoie null si le message n'est pas trouve */
static DLFP_message *
dlfp_msg_find_mid(DLFP *dlfp, int mid)
{
  DLFP_message *m;

  m = dlfp->msg;
  while (m) {
    assert(m->mid > 0); /* piege a bugs */
    if (m->mid == mid) break;
    m = m->next;
  }
  return m;
}

DLFP_message *
dlfp_msg_find_unreaded(DLFP *dlfp)
{
  DLFP_message *m;
  
  m = dlfp->msg;
  while (m) {
    if (m->unreaded) return m;
    m = m->next;
  }
  return NULL;
}

void
dlfp_msg_update_messages(DLFP *dlfp)
{
  HttpRequest r;
  char path[2048];
  static int first_run = 1;
  static char *messages_last_modified = NULL;

  if (Prefs.user_cookie == NULL) return;
  
  BLAHBLAH(3,printf("dlfp_msg_update_messages...\n"));

  if ((Prefs.debug & 2) == 0) {
    snprintf(path, 2048, "%s%s/%s", (strlen(Prefs.site_path) ? "/" : ""), Prefs.site_path, Prefs.path_messages);
  } else {
    snprintf(path, 2048, "%s/wmcoincoin/test/messages.html", getenv("HOME"));
    myprintf("DEBUG: ouverture de %<RED %s>\n", path);
  }

  wmcc_init_http_request_with_cookie(&r, path);
  if (Prefs.use_if_modified_since) { r.p_last_modified = &messages_last_modified; }
  http_request_send(&r);

  if (r.error == 0) {
    char *s, *p, *end=NULL;
    int msgcnt;
    int err;

    BLAHBLAH(2,printf("le transfert de '%s' semble ok\n", Prefs.path_messages));
    err = 0;

    s = gros_read(&r, Prefs.path_messages);
    http_request_close(&r);
    
    if (s == NULL) { err = 1; goto ouups;}

    flag_updating_messagerie = 1;

    /* marque tous les message comme etant trop vieux */
    {
      DLFP_message *m;
      m = dlfp->msg;
      while (m) { m->tooold = 1; m = m->next; }
    }

    /* ajout des nouveaux messages */
    p = s;
    msgcnt = 0;
    do {
      const char *sign = "messages/view.php3?id=";
      p = strstr(p+1, sign); /* p+1 pour pas tourner en rond comme un çon ..*/
      if (end == NULL && p) { /* pour éviter de clignoter lorsqu'on vient d'envoyer un message
			    on tronque à la fin de la premiere table, ce qui évite de lire
			    la liste des messages que l'on a envoyé, et donc qu'ils soient
			    signalés comme de nouveau messages */
	end = strstr(p, "</table");
	if (end) *end = 0;
      }
      if (p) {
	int mid;

	mid = atoi(p + strlen(sign));
	if (mid > 0) {
	  DLFP_message *m;
	  msgcnt++;
	  if ((m=dlfp_msg_find_mid(dlfp, mid)) == NULL) {
	    m = dlfp_msg_insert_message(dlfp, mid);
	    m->unreaded = 1;
	    m->tooold = 0;
	  } else {
	    m->tooold = 0;
	  }
	} else {
	  fprintf(stderr,"heum heum... y'a une pouille dans les messages\n");
	}
      }
    } while (p);

    /* suppression des messages trop vieux */
    if (msgcnt > 0) {
      DLFP_message *m, *n, *p;
      m = dlfp->msg; p = NULL;
      while (m) { 
	n = m->next;
	if (m->tooold) {
	  free(m);
	  if (p) {
	    p->next = n;
	  } else {
	    dlfp->msg = n;
	  }
	} else {
	  p = m; 
	}
	m = n;
      }
    }

    /* premier lancement, on relit l'id du dernier message recu */
    if (first_run && msgcnt > 0) {
      FILE *f;
      f = open_site_file("lastmessage");
      if (f) {
	int mid = -1;
	first_run = 0;
	while (fscanf(f, "%d", &mid) == 1) {
	  DLFP_message *m;
	  assert(mid > 0);
	  BLAHBLAH(2, printf("dejalu_mid=%d\n", mid));
	  m = dlfp->msg; 
	  while (m) {
	    if (m->mid == mid) m->unreaded = 0;
	    m = m->next;
	  }
	}
	fclose(f);
      }
      
    } else if (msgcnt > 0) {
      /* si ce n'est pas le premier lancement, on met à jour le fichier */
      char fname[2048];
      FILE *f;
      snprintf(fname,2048,"%s/.wmcoincoin/%s/lastmessage", getenv("HOME"), Prefs.site_root);
      if ((f = fopen(fname, "wt"))) {
	DLFP_message *m;

	BLAHBLAH(2, printf("ouverture en ecriture de '%s' ok..\n", fname));
	m = dlfp->msg; 
	while (m) {
	  if (m->unreaded == 0) {
	    fprintf(f, "%d\n", m->mid);
	  }
	  m = m->next;
	}
	fclose(f);
      } else {
	BLAHBLAH(1, printf("impossible d'ouvrir en ecriture '%s' arg..\n", fname));
      }
    }
    flag_updating_messagerie = 0;
    free(s);
  ouups:
    if (err) {
      myfprintf(stderr,"dlfp_msg_update_messages: %<RED Erreur lors du rapatriement> de '%s' (err=%d)\n", path, err);
    }
  } else {
    myfprintf(stderr, "erreur pendant la récupération de '%<YEL %s>' : %<RED %s>\n", path, http_error());
  }
}

#ifdef TEST_MEMLEAK
void DLFP_trib_load_rule_destroy(DLFP_trib_load_rule *r);

void
dlfp_destroy(DLFP *dlfp)
{
  tribune_msg_info *mi;
  while (dlfp->news) {
    dlfp_delete_news(dlfp, dlfp->news->id);
  }
  while (dlfp->msg) {
    dlfp_msg_delete_message(dlfp, dlfp->msg->mid);
  }
  while (dlfp->com) {
    dlfp_yc_delete_comment(dlfp, dlfp->com->com_id);
  }
  if (dlfp->fortune) {
    free(dlfp->fortune); dlfp->fortune = NULL;
  }
  DLFP_trib_load_rule_destroy(dlfp->tribune.rules);

  mi = dlfp->tribune.msg;
  while (mi) {
    tribune_msg_info *nmi;
    nmi = mi->next;
    if (mi->refs) free(mi->refs);
    free(mi);
    mi = nmi;
  }
  dlfp->tribune.msg = NULL;
  free(dlfp);
}
#endif
