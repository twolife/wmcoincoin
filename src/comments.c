#include <libintl.h>
#define _(String) gettext (String)

#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include "coincoin.h"
#include "http.h"
#include "regexp.h"

/* --------------- gestion des commentaire -------------- */

/*
  rcsid=$Id: comments.c,v 1.4 2002/08/31 21:26:46 pouaite Exp $

  ChangeLog:
  $Log: comments.c,v $
  Revision 1.4  2002/08/31 21:26:46  pouaite
  ajout du wmccc

  Revision 1.3  2002/08/29 00:15:53  pouaite
  cosmétique et capillotraction

  Revision 1.2  2002/08/18 00:29:30  pouaite
  en travaux .. prière de porter le casque

  Revision 1.1  2002/08/17 18:33:39  pouaite
  grosse commition

*/

void
site_yc_printf_comments(Site *site) {
  Comment *n;
  int cnt;

  myprintf(_("\n------ %<YEL list of comments> -------\n"
	   "num\tnews_id\tcid\tnrep\told\tmodified\n"));
  
  n = site->com;
  cnt = 1;
  while (n) {
    printf("%d\t%d\t%d\t%d\t%d\t%d\n", cnt, n->news_id, n->com_id, 
	   n->nb_answers, n->old, n->modified);
    n = n->next; cnt++;
  }
}

/*
  efface un commentaire (auqnd elle est trop vieille)
*/
static int
site_yc_delete_comment(Site *site, int cid) {
  Comment *n, *p;

  p = NULL;
  n = site->com;
  while (n) {
    if (n->com_id == cid) break;
    p = n;
    n = n->next;
  }
  if (n == NULL) {
    fprintf(stderr, _("QUACK ! Unable to destroy the comment cid=%d\n"), cid);
    return 1;
  }
  if (p) {
    p->next = n->next;
  } else {
    site->com = n->next;
  }
  free(n);
  return 0;
}

static Comment *
site_yc_insert_comment(Site *site)
{
  Comment *n;
  ALLOC_OBJ(n, Comment);
  n->site = site;

  n->next = site->com;
  site->com = n;
  return n;
}

/* renvoie null si la news n'est pas trouvee */
static Comment *
site_yc_find_cid(Site *site, int cid)
{
  Comment *n;

  n = site->com;
  while (n) {
    assert(n->com_id > 0); /* piege a bugs */
    if (n->com_id == cid) break;
    n = n->next;
  }
  return n;
}

Comment *
site_yc_find_modified(Site *site)
{
  Comment *c;
  
  c = site->com;
  while (c) {
    if (c->modified) return c;
    c = c->next;
  }
  return NULL;
}

void
site_yc_clear_modified(Site *site)
{
  Comment *c;
  
  c = site->com;
  
  while (c) {
    c->modified = 0;
    c = c->next;
  }
}

/*
  download & update comments 
*/
void
site_yc_dl_and_update(Site *site)
{
  HttpRequest r;
  char path[2048];

  if (site->prefs->user_cookie == NULL && site->prefs->force_fortune_retrieval == 0) return;

  BLAHBLAH(3,printf("[%s] site_yc_update_comments...\n", site->prefs->site_name));

  if ((Prefs.debug & 2) == 0) {
    snprintf(path, 2048, "%s%s/%s", (strlen(site->prefs->site_path) ? "/" : ""), site->prefs->site_path, site->prefs->path_myposts);
  } else {
    snprintf(path, 2048, "%s/wmcoincoin/test/%s/posts.php3", getenv("HOME"),site->prefs->site_name);
    myprintf(_("DEBUG: opening %<RED %s>\n"), path);
  }
  
  pp_set_download_info(site->prefs->site_name, "updating comments");

  wmcc_init_http_request_with_cookie(&r, site->prefs, path);
  if (site->prefs->use_if_modified_since) { r.p_last_modified = &site->comments_last_modified; }
  http_request_send(&r);

  if (r.error == 0) {
    char *s, *p, *p2, *p3;
    int err;

    BLAHBLAH(2,printf(_("The download of '%s' looks OK.\n"), site->prefs->path_myposts));
    err = 0;

    /* on lit tout en un coup */
    s = http_read_all(&r, site->prefs->path_myposts);
    http_request_close(&r);
    
    if (s == NULL) { err = 1; goto ouups;}
    
    /* récuperation du cpu */
    p = strstr(s, "CPU</a>:");
    flag_updating_comments++;
    if (p) {
      site->CPU = atof(p+8);
      BLAHBLAH(1,printf("cpu=%f\n", site->CPU));
    } else site->CPU = -1.0;
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
      if (site->fortune) free(site->fortune);
	
      /*
      site->fortune = malloc(strlen(p) + 1 + strlen("<p align=center></p>"));
      sprintf(site->fortune, "<p align=center>%s</p>", p);
      */
      site->fortune = strdup(p);
      flag_updating_comments--;
      
      BLAHBLAH(1, myprintf("[%s] fortune: \"%<yel %s>\"\n", 
			   site->prefs->site_name, site->fortune));
    } else {
    fortune_erreur:
      BLAHBLAH(0,myprintf(_("[%<YEL %s>] Maybe the structure of '%s' has been modified, "
			    "as I can't find the fortune (err=%d)\n"), 
			  site->prefs->site_name, site->prefs->path_myposts, err));
    }

    /* si on est dans un cas de 'force_fortune_retrieval', on se barre maintenant */
    if (site->prefs->user_cookie == NULL) goto caroule;


    p = strstr(s,"class=\"logoinfo\"");
    if (p != NULL) {
      int xp;
      if (regexp_extract(p, pat_xp, &xp)) {
	if (site->xp != xp && site->xp_change_flag == 0 && site->xp > -100000) {
	  printf("debut flamo : %d %d\n", xp, site->xp);
	  site->xp_change_flag = 1;
	}
	if (site->xp != xp) {
	  site->xp_old = site->xp;
	  site->xp = xp;
	}
      }

      p = strstr(p, "<br");
      if (p != NULL)
	regexp_extract(p, pat_votes, &(site->votes_max), &(site->votes_cur));

      BLAHBLAH(1, myprintf("[%<YEL %s>] xp : %d, votes: %d/%d\n", 
			   site->prefs->site_name, site->xp, 
			   site->votes_cur, site->votes_max));
    } else {
      myprintf(_("[%<YEL %s>] Hmmm, how strange is '%s' ... there is no 'loginfo' field.\n"), 
	       site->prefs->site_name, site->prefs->path_myposts);
    }

    //modif DACODE 1.4
    //p = strstr(s,"?order=news_id"); // on se positionne bien...
    p = strstr(s,"/comments/thread.php"); // on se positionne bien...
    if (p == NULL) {
      if (strstr(s, _("not yet a poster???")) != NULL)
	goto ouups1;
      myprintf(_("[%<YEL %s>] '%s': oooops, looks like either (a) the cookie isn't valid anymore, or "
		 "(b) the structure of the page has changed, or (c) Obiwan Kenobi\n"), 
	       site->prefs->site_name, site->prefs->path_myposts);
      err = 2; goto ouups1;
    }

    flag_updating_comments++;

    /* avant tout, on marque tous les commentaires comme vieux */
    {
      Comment *c;
      c = site->com;
      while (c) {
	c->old = 1;
	c = c->next;
      }
    }

    /* lecture des lignes du tableau */
    do {
      int nid, cid,nbcom;
      Comment *c;

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
      BLAHBLAH(1,myprintf("NBCOM: '%.20s'->%<YEL %d>\n",p3 ? p3+9 : _("<error>"),nbcom));
      //      }
      p = p2;

      /* test du commentaire */
      c = site_yc_find_cid(site, cid);
      if (c) {
	if (c->nb_answers != nbcom) {
	  BLAHBLAH(1,myprintf(_("There has been %d/%d answers to the comment %d\n"), 
			      nbcom-c->nb_answers,c->nb_answers,cid));
	  c->modified = 1;                /* on signale que ce commentaire a été modifié */
	  site->comment_change_flag = 1;  /* mais c'est CE flag qui déclenche le flamometre 
					     (et il est remis à zéro dès que le flamometre a été déclenché)
					   */
	}
	c->nb_answers = nbcom;
	c->old = 0;
      } else {
	c = site_yc_insert_comment(site);
	c->news_id = nid;
	c->com_id = cid;
	c->nb_answers = nbcom;
	c->old = 0;
	c->modified = (nbcom == 0 || site->comments_dl_cnt ==0) ? 0 : 1;
	site->comment_change_flag = c->modified;  /* ce flag qui déclenche le flamometre */
	BLAHBLAH(1,myprintf(_("NEW COMMENT %d, with %d answers\n"),
			    cid, nbcom));
      }
    } while (p);

    /* suppression des commentaires qui ne sont plus listes dans myposts.php3 */
    {
      Comment *c,*n;
      c = site->com;
      while (c) {
	BLAHBLAH(1,printf(_("comment %d, nid=%d, nrep=%d, modified=%d,old=%d\n"),
	       c->com_id, c->news_id, c->nb_answers, c->modified, c->old));
	if (c->old) {
	  BLAHBLAH(1,myprintf(_("DESTROYING COMMENT %d (too old, isn't in '%s' anymore)\n"),
			      c->com_id, site->prefs->path_myposts));
	  n = c->next;
	  site_yc_delete_comment(site,c->com_id);
	  c = n;
	} else {
	  c = c->next;
	}
      }
    }
    site->comments_dl_cnt++;
    goto caroule;
  ouups2:
    myprintf(_("[%<YEL %s>] error in '%s', the html code must have changes... (err=%d)\n"),
	     site->prefs->site_name, site->prefs->path_myposts, err);
  caroule:
    flag_updating_comments--;
  ouups1:
    free(s);
  ouups:
    if (err) {
      myfprintf(stderr,_("[%<YEL %s>] site_yc_update_comments: %<RED Error "
			 "while downloading> '%s' (err=%d)\n"), 
		site->prefs->site_name, path, err);
    }
  } else {
    myfprintf(stderr, _("[%<YEL %s>] Error while downloading '%<YEL %s>' : %<RED %s>\n"), 
	      site->prefs->site_name, path, http_error());
  }
  pp_set_download_info(NULL,NULL);
}

/* detruit tous les commentaires */
void site_com_destroy(Site *site) {
  while (site->com) {
    site_yc_delete_comment(site, site->com->com_id);
  }
}
