#include <limits.h>
#include "coincoin.h"
#include "site.h"

static void
sl_build_site_names_hash(SiteList *sl)
{
  Site *site;
  int cnt;
  Boards *bds = sl->boards; 

  if (bds->aliases) free(bds->aliases);
  bds->nb_aliases = 0;
  for (site = sl->list; site; site = site->next) {
    bds->nb_aliases += site->prefs->nb_names;    
  }
  if (bds->nb_aliases == 0) {
    bds->aliases = NULL;
    return;
  } 
  ALLOC_VEC(bds->aliases, bds->nb_aliases, SiteNameHash);
  
  cnt = 0;
  for (site = sl->list; site; site = site->next) {
    int i;
    for (i=0; i < site->prefs->nb_names; i++) {
      bds->aliases[cnt].hash = str_hache_nocase(site->prefs->all_names[i], 
					       strlen(site->prefs->all_names[i]));
      bds->aliases[cnt].sid = site->site_id;
      cnt ++;
    }
  }
  assert(cnt == bds->nb_aliases);
}

void
boards_init_sites(SiteList *slist) {
  Site *s;
  int i;
  for (i=0; i < MAX_SITES; i++)
    slist->boards->btab[i] = NULL;
  for (s = slist->list; s; s=s->next) {
    slist->boards->btab[s->site_id] = s->board;
  }
  sl_build_site_names_hash(slist);
}

static Boards *
boards_create() {
  Boards *b;
  ALLOC_OBJ(b, Boards);
  b->first = NULL;
  b->last = NULL;
  return b;
}

void
boards_destroy(Boards *b) {
  free(b);
}



/* init de la liste de sites */
SiteList *
sl_create() {
  SiteList *sl;
  int i;
  ALLOC_OBJ(sl, SiteList);
  sl->boards = boards_create(sl);
  sl->list = NULL;
  for (i = 0; i < MAX_SITES; i++) {
    if (Prefs.site[i]) {
      sl_insert_new_site(sl, Prefs.site[i]);
    }
  }
  boards_init_sites(sl);
  return sl;
}

static void
sl_set_site_id(Site *site)
{
  int i;
  i = 0;
  while (i < MAX_SITES && Prefs.site[i] != site->prefs) {
    i++;
  }
  assert(i < MAX_SITES); /* or you can shoot yourself in the nose */
  site->site_id = i;
}

/* insertion d'un nouveau site */
void
sl_insert_new_site(SiteList *sl, SitePrefs *sp)
{
  Site *site, *pp;

  ALLOC_OBJ(site, Site);
  site->prefs = sp;
  
  site->news_updated = 0;
  site->news = NULL;
  if (sp->check_board) site->board = board_create(site, sl->boards);
  else site->board = NULL;
  site->com = NULL;
  site->msg = NULL;

  site->news_backend_last_modified = NULL;
  site->news_backend_dl_cnt = 0;
  site->nb_newslues = 0;
  site->newslues_uptodate = 0;
  
  site->messages_last_modified = NULL;
  site->messages_dl_cnt = 0;
  site->comments_last_modified = NULL;
  site->comments_dl_cnt = 0;
  
  site->news_update_request = 0;

  site->xp = site->xp_old = 0;
  site->xp_change_flag = 0;
  site->comment_change_flag = 0;
  
  site->fortune = NULL;
  site->CPU = -1.00;
  site->votes_max = -1;
  site->votes_cur = -1;
  site->site_id = -1;
  site->news_refresh_delay = sp->news_check_delay*(1000/WMCC_TIMER_DELAY_MS);
  site->news_refresh_cnt = site->news_refresh_delay-100; /* juste pour que le premier check se fasse après
							    celui de la tribune */

  sl_set_site_id(site);
 
  /* inserts at end of list */
  pp = sl->list; while (pp && pp->next) pp = pp->next;
  if (pp) pp->next = site; else sl->list = site;

  boards_init_sites(sl);
}

/* suppression d'un site */
void
sl_delete_site(SiteList *sl, Site *site)
{
  Site *p, *pp;

  p = sl->list; pp = NULL;
  while (p && p != site) { pp = p; p = p->next; }
  assert(p);
  p = p->next;
  if (pp) {
    pp->next = p;
  } else {
    sl->list = p;
  }
  site_news_destroy(site);
  site_com_destroy(site);
  site_msg_destroy(site);
  if (site->board) board_destroy(site->board);
  if (site->news_backend_last_modified) free(site->news_backend_last_modified);
  if (site->messages_last_modified) free(site->messages_last_modified);
  if (site->comments_last_modified) free(site->comments_last_modified);
  if (site->fortune) free(site->fortune);

  free(site);
  boards_init_sites(sl);
}

Site*
sl_find_site_by_name(SiteList *sl, char *site_name)
{
  Site *p;
  p = sl->list;
  while (p && strcmp(site_name, p->prefs->site_name)) {
    p = p->next;
  }
  return p;
}


Site*
sl_find_site_id(SiteList *sl, int sid) {
  Site *s;
  s = sl->list;
  while (s) {
    if (s->site_id == sid) return s;
    s = s->next;
  }
  return NULL;
}

News*
sl_find_news(SiteList *sl, id_type id) {
  Site *s;

  s = sl_find_site_id(sl, id_type_sid(id));
  if (s) {
    return site_news_find_id(s, id);
  }
  return NULL;
}

News*
sl_get_nth_unreaded_news(SiteList *sl, int i)
{
  Site *s;
  int cnt = 0;
  for (s = sl->list; s; s = s->next) {
    News *n;    
    for (n = s->news; n; n = n->next) {
      if (site_newslues_find(s, id_type_lid(n->id)) == 0) {
	cnt++;
        if (cnt == i) return n;
      }
    }
  }
  return NULL;
}

/*
  renvoi un site dont le xp ont changé
*/
Site*
sl_find_xp_change(SiteList *sl) {
  Site *s;
  s = sl->list;
  while (s) {
    if (s->xp_change_flag && s->prefs->user_cookie && s->prefs->check_comments) 
      return s;
    s = s->next;
  }
  return NULL;
}

/*
  renvoi un site dont le comment_change_flag est activé
*/
Site*
sl_find_comment_change(SiteList *sl) {
  Site *s;
  s = sl->list;
  while (s) {
    if (s->comment_change_flag && s->prefs->user_cookie && s->prefs->check_comments) 
      return s;
    s = s->next;
  }
  return NULL;
}

/*
  renvoi un site sur la tribune duquel vous venez d'avoir une réponse à 
  votre message Ô combien interessant
*/
Site*
sl_find_board_answer_to_me(SiteList *sl) {
  Site *s;
  s = sl->list;
  while (s) {
    if (s->prefs->check_board) {
      assert(s->board);
      if (s->board->flag_answer_to_me)
	return s;
    }
    s = s->next;
  }
  return NULL;
}

Message*
sl_find_unreaded_msg(SiteList *sl)
{
  Site *s;
  for (s = sl->list; s; s = s->next) {
    Message *m;
    if ((m = site_msg_find_unreaded(s)))
      return m;
  }
  return NULL;
}

Comment *
sl_find_modified_comment(SiteList *sl)
{
  Site *s;
  for (s = sl->list; s; s = s->next) {
    Comment *c;
    if ((c = site_yc_find_modified(s)))
      return c;
  }
  return NULL;
}

void 
site_save_state(Dock *dock UNUSED, FILE *f, Site *site) {
  long tmin=LONG_MIN,tmax=LONG_MAX,t=0;
  if (site->board) {
    tmin = site->board->time_shift_min;
    tmax = site->board->time_shift_max;
    t = site->board->time_shift;
    myprintf("%<yel site_save_state> %10s : tmin -> %<cya %3ld> tmax -> %<cya %3ld> t -> %<CYA %3ld>\n",
	     site->prefs->site_name, tmin, tmax, t);
  }
  fprintf(f, "%ld %ld %ld", tmin, tmax, t);
}

void 
site_restore_state(Dock *dock UNUSED, FILE *f, Site *site) {
  long tmin,tmax,t;
  if (fscanf(f, "%ld %ld %ld\n",&tmin, &tmax, &t) != 3) return;
  if (tmin <= tmax && tmin <= t && t <= tmax && site->board) {
    myprintf("%<yel site_restore_state> %10s : tmin <- %<cya %3ld> tmax <- %<cya %3ld> t <- %<CYA %3ld>\n",
	     site->prefs->site_name, tmin, tmax, t);
    site->board->time_shift_min = tmin;
    site->board->time_shift_max = tmax;
    site->board->time_shift = t;
  }
}
