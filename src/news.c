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
  rcsid=$Id: news.c,v 1.8 2002/09/07 16:21:15 pouaite Exp $
  ChangeLog:
  $Log: news.c,v $
  Revision 1.8  2002/09/07 16:21:15  pouaite
  ça va releaser en douce

  Revision 1.7  2002/09/05 23:11:57  pouaite
  <blog>ce soir g mangé une omelette</blog>

  Revision 1.6  2002/08/29 00:15:53  pouaite
  cosmétique et capillotraction

  Revision 1.5  2002/08/21 23:20:57  pouaite
  coin

  Revision 1.4  2002/08/21 01:11:49  pouaite
  commit du soir, espoir

  Revision 1.3  2002/08/18 20:52:15  pouaite
  les locales des sites fonctionnent (c bon pour les news)

  Revision 1.2  2002/08/18 00:29:30  pouaite
  en travaux .. prière de porter le casque

  Revision 1.1  2002/08/17 18:33:39  pouaite
  grosse commition

  Revision 1.31  2002/06/26 22:19:49  pouaite
  ptit fix pour la tribune de f-cpu + patch de lordoric

  Revision 1.30  2002/06/23 22:26:01  pouaite
  bugfixes+support à deux francs des visuals pseudocolor

  Revision 1.29  2002/06/23 10:44:05  pouaite
  i18n-isation of the coincoin(kwakkwak), thanks to the incredible jjb !

  Revision 1.28  2002/05/27 18:39:14  pouaite
  trucs du week-end + patch de binny

  Revision 1.27  2002/05/20 22:24:36  pouaite
  10000ème bugfix de http.c.. quand je nettoie le code je rajoute des bugs, quand je bugfixe je cradouille le code

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

#include <libintl.h>
#define _(String) gettext (String)

#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include "coincoin.h"
#include "http.h"
#include "regexp.h"



/*
  fonctions générales de gestion des news 
*/




/*
  efface une news (auqnd elle est trop vieille)
*/
static int
site_news_delete_news(Site *site, id_type id) {
  News *n, *p;

  p = NULL;
  n = site->news;
  while (n) {
    if (id_type_eq(n->id,id)) break;
    p = n;
    n = n->next;
  }
  if (n == NULL) {
    fprintf(stderr, _("[%s] QUACK ! Unable to delete the news id=%d\n"), 
	    site->prefs->site_name, id_type_lid(id));
    return 1;
  }
  if (p) {
    p->next = n->next;
  } else {
   site->news = n->next;
  }

  if (n->titre) free(n->titre);
  if (n->txt) free(n->txt);
  if (n->auteur) free(n->auteur);
  if (n->topic) free(n->topic);
  if (n->mail) free(n->mail);
  if (n->url) free(n->url);
  free(n);
  site->news_updated = 1;
  flag_news_updated = 1;
  return 0;
}

static News *
site_news_insert_news(Site *site)
{
  News *n;
  ALLOC_OBJ(n, News);
  n->site = site;

  n->next = site->news;
  site->news = n;
  return n;
}

/* renvoie null si la news n'est pas trouvee */
News *
site_news_find_id(Site *site, id_type id)
{
  News *n;

  n = site->news;
  while (n) {
    assert(!id_type_is_invalid(n->id)); /* piege a bugs */
    if (id_type_eq(n->id,id)) break;
    n = n->next;
  }
  return n;
}

News *
site_news_find_prev(Site *site, id_type id)
{
  News *n;

  n = site->news;
  while (n) {
    if (n->next) {
      if (id_type_eq(n->next->id,id)) {
	return n;
      }
    } else {
      if (id_type_eq(site->news->id,id)) {
	return n;
      }
    }
    n = n->next;
  }
  return NULL;
}

News *
site_news_find_next(Site *site, id_type id) {
  News *n;

  n = site->news;
  while (n) {
    if (id_type_eq(n->id,id)) {
      if (n->next) {
	return n->next;
      } else return site->news;
    }
    n = n->next;
  }
  return NULL;
}

int
site_news_count(Site *dlfp)
{
  int cnt;
  News *n;

  cnt = 0;
  n = dlfp->news;
  while (n) {
    cnt++;
    n = n->next;
  }
  return cnt;
}


/*
  marque toutes les news comme déjà lues 
*/
void
site_news_unset_unreaded(Site *site)
{
  News *n;

  n = site->news;
  while (n) {
    site_newslues_add(site, id_type_lid(n->id));
    n = n->next;
  }
}


/*
  verifie si la news a dépassé la date de peremption */
static int
site_news_is_news_too_old(Site *site, char date[11])
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
  BLAHBLAH(3,myprintf(_("The date is y=%d , m=%d, d=%d\n"), d.tm_year, d.tm_mon, d.tm_mday));
  d.tm_mon--; /* pour mktime , janvier = 0, decembre = 11 ... c'est debile */
  d.tm_year -= 1900; /* ca aussi c'est debile */
  t = mktime(&d);
  t2 = time(NULL);

  BLAHBLAH(3,myprintf(_("-> converting to seconds: t=%ld , now=%ld\n"), t, t2));
  t = difftime(t2, t);
  ndays = (t / 3600 / 24);
  BLAHBLAH(3,myprintf(_("age=%d seconds == %d days\n"), t, ndays));
  return (ndays > site->prefs->news_max_nb_days);
}

/* elimine les news trop agees --
   cette fonction ne sert que pour les news qui ne sont plus listees
   dans le fichier de backend (short.php3) et qui sont devenues trop
   vieilles (plus de 'Prefs.news_max_nb_days' jours d'age)
*/
static
void
site_news_remove_old(Site *site)
{
  News *n, *nn;
  
  n = site->news;
  while (n) {
    /* on sauve le suivant au cas ou n est detruite */
    nn = n->next; 
    if (site_news_is_news_too_old(site, n->date)) {
      BLAHBLAH(2, printf(_("[%s] Destruction of news id=%d (very old, "
			   "date=%s, wasn't in %s anymore)\n"),
			 site->prefs->site_name, id_type_lid(n->id), 
			 n->date, site->prefs->path_news_backend));
      site_news_delete_news(site, n->id);
    }
    n = nn;
  }
}


static int
site_news_update_txt_(Site *site, News *n, int silent_error)
{
#define MAX_URL 100

  char URL[512];
  int err;
  HttpRequest r;

  char *date = NULL, *auteur = NULL, *section = NULL;
  char *texte = NULL, *liens = NULL;

  /* la table des liens (donnés en fin de news) */
  char *url_tab[MAX_URL];
  char *url_tab_desc[MAX_URL];
  int nb_url = 0;

  {
    char *s = str_printf("news #%d", id_type_lid(n->id));
    pp_set_download_info(site->prefs->site_name, s);
    free(s);
  }

  err = 1;

  /* -1 -> pour le threshold des commentaires
     7 -> pour le theme printable (ce qui evite les commentaires)
  */

  if ((Prefs.debug & 2) == 0) {
    snprintf(URL, 512, "%s%s",n->url, site->prefs->path_end_news_url);
    BLAHBLAH(1,printf("get %s\n",URL));
  } else {
    snprintf(URL, 512, "%s/wmcoincoin/test/%d,0,-1,6.html", 
	     getenv("HOME"), id_type_lid(n->id));
    myprintf(_("DEBUG: opening '%<RED %s>'\n"), URL);
  }
  wmcc_init_http_request(&r, site->prefs, URL);
  http_request_send(&r);

  if (r.error == 0) {
    char *s;
    char *p, *p2;

    err = 2;

    s = http_read_all(&r, n->url);
    http_request_close(&r);

    n->heure = 0;

    
    extract_news_txt(site->prefs, s, &date, &auteur, &section, &texte, &liens); /* fonction definie dans regexp.c */

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
      BLAHBLAH(2,myprintf(_("date: '%<BLU %s>' => time: %d\n"), date, n->heure));
    }
    
    n->auteur = strdup(auteur);
    n->topic = strdup(section);

    p = n->txt;
    while (*p) { if (((unsigned char)*p) <= ' ') *p = ' '; p++; }
    
    n->nb_comment = 0;
    BLAHBLAH(2,myprintf(_("We just got the news !!\n")));
    BLAHBLAH(3,myprintf(" --> %<YEL %s>\n", n->txt));
    p2++;
	    

    flag_news_updated = 1;
    err = 0; n->nb_comment = 0;

    free(s);
    
  } else {
    if (silent_error == 0)
      myfprintf(stderr, _("Error while downloading '%<YEL %s>' : %<RED %s>\n"), URL, http_error());
  }
 ouups1:
  if (err) {
    myfprintf(stderr,_("%<RED Error while downloading '%s' (err=%d)>\n"), URL, err);
    if (n->txt) { free(n->txt); n->txt = NULL; }
    if (n->auteur) { free(n->auteur); n->auteur = NULL; }
    if (n->topic) { free(n->topic); n->topic = NULL; }
  }
  pp_set_download_info(NULL,NULL);

  if (texte) free(texte);
  if (liens) free(liens);
  if (date) free(date);
  if (auteur) free(auteur);
  if (section) free(section);

  return err;
}


/*
  lecture du texte de la news

  hum.. [parental advisory]
  c'est pas joli-joli, mais comment veux-tu, comment veux-tu
*/

int
site_news_update_txt(Site *site, id_type id)
{
#define MAX_URL 100

  News *n;

  n = site_news_find_id(site, id);
  if (n == NULL) return 1;

  BLAHBLAH(1,myprintf(_("[%s] Updating text of the news: '%<cya %s>' (date='%10s', id %<cya %d>)\n"), site->prefs->site_name, n->titre, n->date, id_type_lid(n->id)));

  if (site_news_is_news_too_old(site, n->date)) {
    BLAHBLAH(1, myprintf(_("It's too old (date=%s, max age=%d days)\n"), n->date, site->prefs->news_max_nb_days));
    return 0;
  }

  return site_news_update_txt_(site, n, 0);
}

/*
static 
void site_check_hidden_news(Site *site) {
  int id_min, id_max;
  int i, nb_try = 3;

  if (site->nb_newslues == 0) return;
  do {
    int 
    id_max = -1; id_min = INT_MAX;
    for (i=0; i < site->nb_newslues; i++) {
      id_min = MIN(id_min, site->newslues[i]);
      id_max = MAX(id_max, site->newslues[i]);
    }
    if (id_min == id_max) return;
    
    if (id_min < id_max - 20) id_min = id_max - 10;
    id_max += 10;
    
    for (i=10; i > 0; i--) {
      int id;
      News *n =
      id = id_min + 1 + (rand() % (id_max-id_min-1));
      if (site_newslues_find(site, id) == 0) break;
    }
    if (i) {
      News *n;
      ALLOC_OBJ(n, News);
      n->titre = NULL;
    }

    nb_try --;
  } while (nb_try > 0 && nb_newslues != id_max - id_min + 1);
}
*/

/* debug only */
FILE *
open_site_file(Site *site, char *base_name)
{
  char fname[2048], rname[2048];
  FILE *fsave;
  int retry, tantpis;


  /* attention aux ames sensibles */
  snprintf(fname,2048,"%s/.wmcoincoin/%s/%s", getenv("HOME"), site->prefs->site_name, base_name);
  if ((fsave = fopen(fname, "rt")) == NULL) {
    printf("can't open '%s' [%s]\n", fname, strerror(errno));

    for (tantpis = 0, retry = 0; retry < 3; retry++) {
      if ((fsave = fopen(fname, "wt")) == NULL) {
	if (retry == 2) {
	  myprintf(_("Warning: unable to create file '%s' \n[error: '%<red %s>']\n"), fname,strerror(errno));
	  tantpis = 1; break;
	}
      } else { fclose(fsave); break; }
      snprintf(rname,2048,"%s/.wmcoincoin/%s", getenv("HOME"), site->prefs->site_name);
      if (mkdir(rname, -1)) {
	if (retry == 1) {
	  myprintf(_("Warning: unable to create directory '%s'\n[error: '%<red %s>']\n"), rname,strerror(errno));
	  tantpis = 1; break;
	}
	snprintf(rname,2048,"%s/.wmcoincoin", getenv("HOME"));
	if (mkdir(rname,-1)) {
	  myprintf(_("Warning: unable to create directory '%s'\n[error: '%<red %s>']\n"), rname, strerror(errno));
	  tantpis = 1; break;
	}
      }
    }
    
    fsave = fopen(fname, "rt");
  }
  return fsave;
}

void
site_newslues_add(Site *site, int lid)
{
  int n;
  if (site_newslues_find(site, lid)) return;
  n = MIN( MAX_NEWS_LUES-1,site->nb_newslues);
  memmove(site->newslues+1, site->newslues, n*sizeof(int));
  site->newslues[0] = lid;
  if (site->nb_newslues < MAX_NEWS_LUES) site->nb_newslues++;
}

int
site_newslues_find(Site *site, int lid)
{
  if (site->prefs->check_news) {
    int i;
    if (site->newslues_uptodate == 0) {
      site_news_restore_state(site);
    }    
    for (i=0; i < site->nb_newslues; i++)
      if (site->newslues[i] == lid) return 1;
  }
  return 0;
}


/*
  appelle lors de l'ajout d'un nouveau site:
  lit les id des news deja connues, pour eviter qu'elles
  ne soient resignalees comme etant de nouvelles news
*/
void
site_news_restore_state(Site *site)
{
  char s[2048];
  FILE *fsave;

  s[2047] = 0;

  if (site->prefs->check_news == 0) return;
   site->newslues_uptodate = 1;
  fsave = open_site_file(site, "newslues");
  if (fsave) {
    int lid;
    fgets(s, 2047, fsave); // on ouble la premiere ligne (=="#do not edit this file")
    while (fscanf(fsave, "%d\n", &lid)==1) {
      site_newslues_add(site, lid);
    }
    fclose(fsave);
  }
}

void
site_news_save_state(Site *site)
{
  char fname[2048];
  FILE *fsave;

  if (site->prefs->check_news == 0) return;
  /* attention aux ames sensibles */
  snprintf(fname,2048,"%s/.wmcoincoin/%s/newslues", getenv("HOME"), site->prefs->site_name);
  if ((fsave = fopen(fname, "wt"))) {
    int i;
    BLAHBLAH(1,printf("on sauve.. %s\n", site->prefs->site_name));
    fprintf(fsave, "#do not edit this file\n");
    for (i = 0; i < site->nb_newslues; i++)
      fprintf(fsave, "%d\n", site->newslues[i]);
    fclose(fsave);
  } else {
    BLAHBLAH(1,myprintf(_("Unable to open '%s' for writing\n(error: %s)\n"),
			fname, strerror(errno)));
  }
}


/*
  recupere le backend de news
  et met a jour la liste des news en consequence
*/
void
site_news_dl_and_update(Site *site)
{
  HttpRequest r;
  char path[2048];
  int transfert_ok = 0;

  BLAHBLAH(3,printf("[%s] site update_news...\n", site->prefs->site_name));

  /* on se protege */
  flag_updating_news++;
  pp_set_download_info(site->prefs->site_name, "d/l news backend");
  if ((Prefs.debug & 2) == 0) {
    snprintf(path, 2048, "%s%s/%s", (strlen(site->prefs->site_path) ? "/" : ""), 
	     site->prefs->site_path, site->prefs->path_news_backend); 
  } else {
    snprintf(path, 2048, "%s/wmcoincoin/test/%s/short.php3", getenv("HOME"), site->prefs->site_name); 
    myprintf(_("DEBUG: opening '%<RED %s>'\n"), path);
  }

  wmcc_init_http_request(&r, site->prefs, path);
  if (site->prefs->use_if_modified_since) r.p_last_modified = &site->news_backend_last_modified;
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
    id_type id;
    char s[512], url[512], base_url[512];
    int l_cnt;
    int news_err;

    ALLOW_X_LOOP;
    news_err = 0;

    BLAHBLAH(2,printf(_("[%s] Download looks OK\n"), site->prefs->site_name));
    l_cnt = -1;
    s[0] = 0;
    while (http_get_line(&r, s, 512)>0) {
      ALLOW_X_LOOP;

      /* faut sauter l'entete, PLOP! */
      if (strstr(s, "</channel>")) l_cnt = 0;
      if (l_cnt == -1) continue;

      if (strstr(s, news_item_sign)) { 
	if (l_cnt != 0) { news_err = 1; break; }
	l_cnt = 1; 
	id = id_type_invalid_id();
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
	News *n = NULL;

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
	  BLAHBLAH(1,myprintf(_("[%s] The '%s' backend doesn't look good...\n"), 
			      site->prefs->site_name, site->prefs->path_news_backend));
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

	id_type_set_lid(&id, atoi(s_id));
	id_type_set_sid(&id, site->site_id);

	if (id_type_is_invalid(id)) {
	  fprintf(stderr, _("[%s] Probleme in the news backend: line '%s' -> id='%s'=%d\n"),
		  site->prefs->site_name, url, s_id, id_type_lid(id));
	  news_err = 11; break;
	}

	flag_news_updated = 1;

	ALLOW_X_LOOP;

	if (site_news_is_news_too_old(site, date)) {
	  BLAHBLAH(1, printf(_("[%s] The news id=%d is too old (date=%s, max age=%d), "
			       "let's delete it\n"),
			     site->prefs->site_name, id_type_lid(id),
			     date,site->prefs->news_max_nb_days));
	  if (site_news_find_id(site,id)) {	    
	    site_news_delete_news(site, id);
	  }
	} else if (site_news_find_id(site,id) == NULL) {
	  ALLOW_X_LOOP;
	  n = site_news_insert_news(site); assert(n);
	  n->titre = strdup(title);
	  n->url = strdup(base_url);
	  n->txt = NULL;
	  n->auteur = NULL; 
	  n->topic = NULL;
	  n->mail = strdup("mailpic");
	  strncpy(n->date, date, 11); date[10] = 0;
	  n->id = id;
	  n->dl_nb_tries = 0;
	  site->news_updated = 1;
	  BLAHBLAH(2,printf(_(" . title='%s'\n"), title));
	  BLAHBLAH(2,printf(_(" . author='%s', mail='%s'\n"), n->auteur, n->mail));
	  BLAHBLAH(2,printf(_("** News ADDED\n")));
	  flag_news_updated = 1;
	}
      } /* nouvelle news ajoutée */
    } /* while http_get_line */
    if (news_err != 0) {
      myfprintf(stderr,_("%<RED OOOOPS!> %<grn site_updatenews> there has been an error (err=%d) when parsing the news backend.\n"), news_err);
    }
    http_request_close(&r);
    site->news_backend_dl_cnt++;
    transfert_ok = 1;
  } else {
    myfprintf(stderr, _("[%<YEL %s>] Error while downloading '%<YEL %s>' : %<RED %s>\n"), 
	      site->prefs->site_name, site->prefs->path_news_backend, http_error());
  }
  /* elimine les eventuelles news trop vielles, et qui ne sont plus
     dans short.php3 */
  site_news_remove_old(site);

  {
    News *n;
    for (n = site->news; n ; n = n->next) {
      if (n->txt == NULL && n->dl_nb_tries < 4) {
	n->dl_nb_tries++;
	ccqueue_push_newstxt_update(site->site_id, id_type_lid(n->id));
      }
    }
  }

  ALLOW_X_LOOP;
      
  if (site_news_count(site) == 0) {
    site->news_updated = 1; // pour forcer l'affiche d'un message particulier quand ttes les news sont trop vieilles 
  }
  pp_set_download_info(NULL, NULL);


  flag_updating_news--;
}

/* detruit toutes les news */
void site_news_destroy(Site *site) {
  while (site->news) {
    site_news_delete_news(site, site->news->id);
  }
}
