#include <libintl.h>
#define _(String) gettext (String)

#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include "coincoin.h"
#include "http.h"
#include "regexp.h"

/* --------------- gestion des messages perso -------------- */

/*
  rcsid=$Id: messages.c,v 1.7 2002/10/16 20:41:45 pouaite Exp $

  ChangeLog:
  $Log: messages.c,v $
  Revision 1.7  2002/10/16 20:41:45  pouaite
  killall toto

  Revision 1.6  2002/08/29 00:15:53  pouaite
  cosmétique et capillotraction

  Revision 1.5  2002/08/22 00:10:14  pouaite
  prout

  Revision 1.4  2002/08/21 23:20:57  pouaite
  coin

  Revision 1.3  2002/08/21 20:22:16  pouaite
  fix compil

  Revision 1.2  2002/08/18 00:29:30  pouaite
  en travaux .. prière de porter le casque

  Revision 1.1  2002/08/17 18:33:39  pouaite
  grosse commition

*/

void 
site_msg_printf_messages(Site *site) {
  Message *n;
  int cnt;

  myprintf(_("\n------ %<YEL list of messages of %s> -------\n"
	   "num\tmsg_id\tunread\ttoo_old\n"), site->prefs->site_name);
  
  n = site->msg;
  cnt = 1;
  while (n) {
    printf("%d\t%d\t%d\t%d\n", cnt, n->mid, n->unreaded, n->tooold); 
    n = n->next; cnt++;
  }
}


/*
  efface un message (par ex. quand il est trop vieux et ne figure plus dans linuxfr/messages)
*/
static int
site_msg_delete_message(Site *site, int mid) {
  Message *m, *p;

  p = NULL;
  m = site->msg;
  while (m) {
    if (m->mid == mid) break;
    p = m;
    m = m->next;
  }
  if (m == NULL) {
    fprintf(stderr, _("[%s] QUACK ! Unable to destroy the message mid=%d\n"), 
	    site->prefs->site_name, mid);
    return 1;
  }
  if (p) {
    p->next = m->next;
  } else {
    site->msg = m->next;
  }
  free(m);
  return 0;
}

/* inserts a message (with sorting) */
static Message *
site_msg_insert_message(Site *site, int mid)
{
  Message *m, *p, *n;
  ALLOC_OBJ(m, Message);
  m->site = site;

  p = NULL; n = site->msg;
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
    site->msg = m;
  }
  m->next = n;
  m->mid = mid;
  return m;
}

/* renvoie null si le message n'est pas trouve */
static Message *
site_msg_find_mid(Site *site, int mid)
{
  Message *m;

  m = site->msg;
  while (m) {
    assert(m->mid > 0); /* piege a bugs, viens-là petit */
    if (m->mid == mid) break;
    m = m->next;
  }
  return m;
}

/*
  returns the first unreaded message
*/
Message *
site_msg_find_unreaded(Site *site)
{
  Message *m;
  
  m = site->msg;
  while (m) {
    if (m->unreaded) return m;
    m = m->next;
  }
  return NULL;
}

/*
  download & update private messages
*/
void
site_msg_dl_and_update(Site *site)
{
  HttpRequest r;
  char path[2048];

  if (site->prefs->user_cookie == NULL) return;
  
  BLAHBLAH(3,printf("site_msg_update_messages...\n"));

  pp_set_download_info(site->prefs->site_name, "updating messages");

  if ((Prefs.debug & 2) == 0) {
    snprintf(path, 2048, "%s%s/%s", (strlen(site->prefs->site_path) ? "/" : ""), site->prefs->site_path, site->prefs->path_messages);
  } else {
    snprintf(path, 2048, "%s/wmcoincoin/test/%s/messages.html", getenv("HOME"), site->prefs->site_name);
    myprintf(_("DEBUG: opening %<RED %s>\n"), path);
  }

  wmcc_init_http_request_with_cookie(&r, site->prefs, path);
  if (site->prefs->use_if_modified_since) { r.p_last_modified = &site->messages_last_modified; }
  http_request_send(&r);

  if (r.error == 0) {
    char *s, *p, *end=NULL;
    int msgcnt;
    int err;

    BLAHBLAH(2,printf(_("Download of '%s' looks OK.\n"), site->prefs->path_messages));
    err = 0;

    s = http_read_all(&r, site->prefs->path_messages);
    http_request_close(&r);
    
    if (s == NULL) { err = 1; goto ouups;}

    flag_updating_messagerie++;

    /* marque tous les message comme etant trop vieux */
    {
      Message *m;
      m = site->msg;
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
	  Message *m;
	  msgcnt++;
	  if ((m=site_msg_find_mid(site, mid)) == NULL) {
	    m = site_msg_insert_message(site, mid);
	    m->unreaded = 1;
	    m->tooold = 0;
	  } else {
	    m->tooold = 0;
	  }
	} else {
	  fprintf(stderr,_("Hem hem... Something's wrong with the messages\n"));
	}
      }
    } while (p);

    /* suppression des messages trop vieux */
    if (msgcnt > 0) {
      Message *m, *n, *p;
      m = site->msg; p = NULL;
      while (m) { 
	n = m->next;
	if (m->tooold) {
	  free(m);
	  if (p) {
	    p->next = n;
	  } else {
	    site->msg = n;
	  }
	} else {
	  p = m; 
	}
	m = n;
      }
    }

    /* premier lancement, on relit l'id du dernier message recu */
    if (site->messages_dl_cnt == 0 && msgcnt > 0) {
      FILE *f;
      f = open_site_file(site, "lastmessage");
      if (f) {
	int mid = -1;
	site->messages_dl_cnt = 1;
	while (fscanf(f, "%d", &mid) == 1) {
	  Message *m;
	  assert(mid > 0);
	  BLAHBLAH(2, printf("dejalu_mid=%d\n", mid));
	  m = site->msg; 
	  while (m) {
	    if (m->mid == mid) m->unreaded = 0;
	    m = m->next;
	  }
	}
	fclose(f);
      }
      
    }
    flag_updating_messagerie--;
    free(s);
  ouups:
    if (err) {
      myfprintf(stderr,_("site_msg_update_messages: %<RED Error while downloading> '%s' (err=%d)\n"), path, err);
    }
  } else {
    myfprintf(stderr, _("Error while downloading '%<YEL %s>' : %<RED %s>\n"), path, http_error());
  }
  pp_set_download_info(NULL,NULL);
}


void
site_msg_save_state(Site *site)
{
  char fname[2048];
  FILE *f;
  int nb_lus;
  Message  *m;
  
  site->messages_dl_cnt++;
  nb_lus = 0;
  for (m = site->msg; m ; m=m->next) nb_lus += (m->unreaded ? 0 : 1);
  
  /* si ce n'est pas le premier lancement, on met à jour le fichier */
  snprintf(fname,2048,"%s/.wmcoincoin/%s/lastmessage", getenv("HOME"), site->prefs->site_name);
  if (nb_lus && (f = open_wfile(fname))) {
    
    BLAHBLAH(2, printf(_("Opening '%s' for writing... OK\n"), fname));
    for (m = site->msg; m ; m=m->next) {
      if (m->unreaded == 0) {
	fprintf(f, "%d\n", m->mid);
      }
    }
    fclose(f);
  } else {
    BLAHBLAH(1, printf(_("Unable to open '%s' for writing !\n"), fname));
  }
}

/* detruit tous les messages */
void site_msg_destroy(Site *site) {
  while (site->msg) {
    site_msg_delete_message(site, site->msg->mid);
  }
}

