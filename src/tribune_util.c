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
  fonctions diverses sur la tribune

  rcsid=$Id: tribune_util.c,v 1.25 2002/06/23 10:44:05 pouaite Exp $
  ChangeLog:
  $Log: tribune_util.c,v $
  Revision 1.25  2002/06/23 10:44:05  pouaite
  i18n-isation of the coincoin(kwakkwak), thanks to the incredible jjb !

  Revision 1.23  2002/05/28 23:22:58  pouaite
  ptit fix

  Revision 1.22  2002/05/28 20:11:55  pouaite
  modif pr un pinnipede + fluide qd il y a bcp de messages stockés + tribune sur plusieurs jours

  Revision 1.21  2002/04/26 04:45:51  pouaite
  reconnaissance des horloges suivies de 3 pts de suspension

  Revision 1.20  2002/04/24 19:44:00  pouaite
  option pinnipede.use_AM_PM pour les horloges à l'anglaise comme sur http://woof.lu

  Revision 1.19  2002/04/13 11:55:19  pouaite
  fix kde3 + deux trois conneries

  Revision 1.18  2002/04/11 23:16:54  pouaite
  boitakon mega combo

  Revision 1.17  2002/04/10 22:53:44  pouaite
  un commit et au lit

  Revision 1.16  2002/04/09 23:38:29  pouaite
  boitakon et son cortège de bugfixes

  Revision 1.15  2002/04/09 00:28:19  pouaite
  quelques modifs faites dans un état d'hébétude avancé /!\ travaux en cours /!\

  Revision 1.14  2002/04/03 20:15:11  pouaite
  plop

  Revision 1.13  2002/04/01 22:56:03  pouaite
  la pseudo-transparence du pinni, bugfixes divers, option tribune.backend_type

  Revision 1.12  2002/03/27 20:45:06  pouaite
  deuxième vague de bugfix

  Revision 1.11  2002/03/09 00:25:12  pouaite
  coin coin

  Revision 1.10  2002/03/08 23:53:40  pouaite
  derniers bugfixes pour la v2.3.6

  Revision 1.9  2002/03/03 11:58:55  pouaite
  bugfix du crash avec les posts autoreferants (gniiii)

  Revision 1.8  2002/02/27 00:32:19  pouaite
  modifs velues

  Revision 1.7  2002/02/25 01:36:58  pouaite
  bugfixes pour la compilation

  Revision 1.6  2002/02/24 22:13:57  pouaite
  modifs pour la v2.3.5 (selection, scrollcoin, plopification, bugfixes)

  Revision 1.5  2002/01/31 23:45:00  pouaite
  plop

  Revision 1.4  2002/01/20 20:53:22  pouaite
  bugfix configure.in && http_win.c pour cygwin + 2-3 petis trucs

  Revision 1.3  2002/01/20 02:17:13  pouaite
  modifs d'ordre esthetique (!) sans grand interet

  Revision 1.2  2002/01/19 19:56:09  pouaite
  petits crochets pour la mise en valeur de certains messages (cf changelog)

  Revision 1.1  2002/01/18 19:45:58  pouaite
  petit oubli d'un fichier..

*/

#include "coincoin.h"

#include <libintl.h>
#define _(String) gettext (String)

tribune_msg_info *
tribune_find_id(const DLFP_tribune *trib, int id)
{
  tribune_msg_info *it;
  /*
  it = trib->msg; 
  while (it) {
    if (it->id == id) return it;
    it = it->next;
  }
  return NULL;

  */

  it = trib->mi_tree_root;

  while (it && it->id != id) {
    if (id < it->id) it = it->left;
    else it = it->right;
  }
  return it;
}

tribune_msg_info*
tribune_find_previous_from_id(const DLFP_tribune *trib, int id)
{
  tribune_msg_info *mi;
  tribune_msg_info *prev = NULL;

  mi = trib->mi_tree_root;

  while (mi) {
    if (mi->id < id) {
      prev = mi;
      mi = mi->right;
    } else {
      mi = mi->left;
    }
  }
  return prev;
}


tribune_msg_info *
tribune_find_previous(const DLFP_tribune *trib, tribune_msg_info *mi)
{
  tribune_msg_info *it;

  it = tribune_find_previous_from_id(trib, mi->id);
  return it;
    /*
      trib->msg; 
      while (it) {
      if (it->next == mi) return it;
      it = it->next;
      }
      return NULL;
    */
}

/* si 'ww' contient une reference (du type '1630', '125421', '12:45:30') vers un message existant, on renvoie 
   son msg_info, et on rempli 'commentaire' 


   avertissement au lecteur téméraire:
     cette fonction est un sac de noeuds et ça ne va pas en s'arrangeant

     (en gros c'est toute une serie de tests pour filtres les refs invalides tout en
     etant le plus général possible ..)
*/
int
check_for_horloge_ref_basic(const unsigned char *ww, int *ref_h, int *ref_m, int *ref_s, int *ref_num)
{
  int l, h, m, s, num;  /* num est utilise pour les posts multiples (qui on un même timestamp) */
  const unsigned char *p;
  int use_deuxpt;
  unsigned char w[11];

  *ref_h = -1; *ref_m = -1; *ref_s = -1; *ref_num = -1;
  num = -1; h = -1; m = -1; s = -1;
  l = strlen(ww);

  if (l < 4 || l > 10) return 0; /* on enlimine les cas les plus explicites */
  strncpy(w, ww, 10); w[10] = 0;

  use_deuxpt = 0;
  p = w; 
  /* verifie que la chaine ne contient que des chiffres et des ':' ou des '.' (les ':' n'etant pas en premiere ou derniere position) */
  while (*p) { 
    if ((*p == ':' || *p == '.' || 
	 (use_deuxpt == 0 && *p == 'h') || (use_deuxpt == 1 && *p == 'm')) 
	&& p != w && *(p+1)) {
      use_deuxpt++;
    } else if (*p < '0' || *p > '9') {
      if (*(p+1) == 0 && strchr("¹²³",*p) == NULL)
	break;
    }

    p++;
  }
  if (*p) return 0;
  
  if (use_deuxpt == 0) {
    if (l == 4) {
      /* type '1630' */
      h = (w[0]-'0')*10 + (w[1]-'0');
      m = (w[2]-'0')*10 + (w[3]-'0');
      s = -1;
    } else if (l == 6) {
      h = (w[0]-'0')*10 + (w[1]-'0');
      m = (w[2]-'0')*10 + (w[3]-'0');
      s = (w[4]-'0')*10 + (w[5]-'0');
    } else if (l == 7) {
      h = (w[0]-'0')*10 + (w[1]-'0');
      m = (w[2]-'0')*10 + (w[3]-'0');
      s = (w[4]-'0')*10 + (w[5]-'0');
      if (w[6] == (unsigned char)'¹') num = 0; 
      else if (w[6] == (unsigned char)'²') num = 1; 
      else if (w[6] == (unsigned char)'³') num = 2; else return 0;
    } else return 0;

    /* ci-dessous minipatch pour Dae qui reference les posts multiples
       sous la forme hh:mm:ss:num -> wmc2 ne les reconnaissait pas comme des 
       refs, maintenant si */

  } else if (use_deuxpt <= 3) {
    /* il y a des separateurs entre les heure et les minutes [et les secondes] */
    int nb_char_h, nb_char_m, nb_char_s;
    p = w;
    h = 0;
    nb_char_h = nb_char_m = nb_char_s = 0;
    while (*p != ':' && *p != '.' && *p != 'h') {
      if (*p < '0' || *p > '9') return 0;
      h = 10*h + (*p - '0'); p++;
      nb_char_h++;
    }
    p++;
    m = 0;
    while (*p != ':' && *p != '.' && *p != 'm' && *p) {
      if (*p < '0' || *p > '9') return 0;
      m = 10*m + (*p - '0'); p++;
      nb_char_m++;
    }
    if (*p == ':' || *p == '.' || *p == 'm') {
      p++;
      s = 0;
      while (*p && *p != ':' && 
	     *p != (unsigned char)'¹' && *p != (unsigned char)'²' && *p != (unsigned char)'³') {
	if (*p < '0' || *p > '9') return 0;
	s = 10*s + (*p - '0'); p++;
	nb_char_s++;
      }
      if (*p == (unsigned char)'¹') num = 0;
      if (*p == (unsigned char)'²') num = 1;
      if (*p == (unsigned char)'³') num = 2;
      if (*p == ':') {
	p++; if (*p >= '0' && *p <= '9') num = *p - '1';
      }
    } else s = -1;

    /* le test qui tue pour arrêter de confondre la version du kernel avec une horloge .. */
    /* ça ira jusqu'au kernel 2.10.10 */
    if (nb_char_h > 2 || nb_char_m != 2 || nb_char_s == 1 || nb_char_s > 2) return 0;

  } else return 0;

  if (h > 23 || m > 59 || s > 59) return 0;

  *ref_h = h; *ref_m = m; *ref_s = s; *ref_num = num;

  return 1;
}

/* ceci est un commentaire à la con pour forcer le commit (oui je suis un tocard mais g la flemme de chercher à comprendre */


/* dans la famille des fonction pourries, je demande ... */
char *
tribune_get_tok(const unsigned char **p, const unsigned char **np, 
	unsigned char *tok, int max_toklen, int *has_initial_space)
{
  const unsigned char *start, *end;

  assert(p); 
  assert(*p); 
  assert(tok);

  /* has_initial_space doit etre initialise dans la procedure appelante (sinon y'a des pbs avec les tag html)
   *has_initial_space = 0; */ /* indique si le token commence par un (ou plusieurs) espace */

  start = *p; *np = NULL;
  /* saute les espaces */
  while (*start <= ' ' && *start != '\t' && *start) { start++; *has_initial_space = 1; }
  end = start;

  /* if (*start == '\t') printf("allez ! '%.20s'\n", start); */

  /* les bon vieux tags html (update les '<' et '>' des tags sont prefixés par une tabulation) */
  if (*start == '\t' && *(start+1) == '<') {
    static const char *balise = "abusiABUSI";
    int i;
    /* c'est un peu chiant, on risque de mal reconnaitre les balise ou
       d'en reconnaitre qui ont ete ajoutees a la main 
       on teste d'abord les balises courantes
    */
    for (i=0; i < 10; i++) { 
      if (start[2] == balise[i] && start[3] == '\t' && start[4] == '>') {
	end = start+5;
	break;
      } else if (start[2] == '/' && start[3] == balise[i] && start[4] == '\t' && start[5] == '>') {
	end = start+6;
	break;
      }
    }
    if (end == start) {
      int is_href;
      const unsigned char *s1 = "\t<a href=\"http://";
      const unsigned char *s2 = "\t<a href=\"ftp://";
      const unsigned char *s3 = "\t<a href=\"https://";
      const unsigned char *s4 = "\t<a href=\"..";
      /* puis les <a href> (c'est un peu particulier */

      /* c'est un peu facho, d'autant que c'est reverifié au niveau de open_url, mais
	 bon, apres la douche froide... */
      is_href = 0;
      if (strncasecmp(start, s1, strlen(s1)) == 0) is_href = 1; 
      if (strncasecmp(start, s2, strlen(s2)) == 0) is_href = 1; 
      if (strncasecmp(start, s3, strlen(s3)) == 0) is_href = 1; 
      if (strncasecmp(start, s4, strlen(s4)) == 0) is_href = 1; 
      if (is_href) {
	/* printf("get_tok: '"); */
	end = start+1;
	while (*end && *end != '\t') end++; /* { printf("%c", *end); end++; } */
	if (*end == '\t' && *(end+1)=='>') end+=2;
	/* printf("\n"); */
/* 	if (*end) end++; */
      } else {
	const char *p;
	/* sinon on ignore */
	start++;
	end=start+1;
	p = start;
	printf(_("get_tok not recognized: (len=%d)'"), (int)strlen(p));
	while (*p) { printf("%c", *p); p++;}
	printf("\n");
      }
    }
    /*
    {
      char c;
      c = *end; *end = 0;
      myprintf("tok='%<YEL %s>'\n", start);
      *end = c;
    }
    */
  } else if (*start == '\t') { /* ça pue .. le backend ou le coincoin est sans slip */
    printf(_("Hmmm, looks like there's an underpants problem here: %s\n"), start);
    start++;
    if (*start) end = start+1;
  } else {
    /* pour aider la reconnaissance des timestamp */
    if (*end >= '0' && *end <= '9') {
      unsigned char last = *end;
      while (*end && 
	     ((*end >= '0' && *end <= '9') || strchr(":.hm¹²³", *end))) {
	end++;
	if ((last < '0' || last > '9') && (*end < '0' || *end > '9'))
	  break; /* deux caractères non numériques consécutifs, c'est la fin de l'horloge.. */
	last = *end;
      }
      /* un petit coup de marche arriere si on n'a pas termine sur un chiffre */
      if (end-start > 4 && (*(end-1) == ':' || *(end-1) == '.' || *(end-1) == 'm')) end--;
    } else {
      /* un mot normal */
      while (*end && *end != '\t' && *end > ' ' && (*end < '0' || *end > '9')) end++;
    }
  }
  if (end == start) return NULL;
  else strncpy(tok, start, MIN(end-start, max_toklen-1));
  tok[MIN(end-start, max_toklen-1)] = 0;
  *p = start;
  *np = end;
  return tok;
}


/* oh le joli nom en anglais 

  cette fonction n'est pas utilisée ici mais dans coincoin_tribune

  c'est le bordel , ça évoluera surement

  TODO: pb à l'initialisation, il faut l'appeler dans l'ordre des ID, sinon y'a pb

  TODO: CETTE FONCTION EST NAZE MAIS JE SUIS TROP CREVE JE FAIS RIEN QUE DES CONNERIES CE SOIR
*/
int
tribune_msg_is_ref_to_me(DLFP_tribune *trib, const tribune_msg_info *ref_mi) {
  tribune_msg_info *mi;

  mi = trib->msg;
  
  /*printf("test de %02d:%02d:%2d(%d)..\n", ref_mi->hmsf[0], ref_mi->hmsf[1], ref_mi->hmsf[2], ref_mi->hmsf[3]);*/
  while (mi) {
    const unsigned char *p, *np;

    if (mi->is_my_message) {
      /* c'est du filtre qui va ramer si y'a 10000 messages en mémoire... */
      p = ref_mi->msg;
      while (p) {
	int has_initial_space = 0; /* inutilisé */
	unsigned char tok[512];
	
	if (tribune_get_tok(&p,&np,tok,512, &has_initial_space) == NULL) { break; }
	if (tok[0] >= '0' && tok[0] <= '9') {
	  int h,m,s,num;
	  if (check_for_horloge_ref_basic(tok, &h, &m, &s, &num)) {
	    /*	  printf(" id%05d -> contient ref '%s'\n", mi->id, tok);*/
	    if ((h == mi->hmsf[0] || (Prefs.pp_use_AM_PM && (h==mi->hmsf[0]%12) && mi->hmsf[0] > 12))
		&& m == mi->hmsf[1] && 
		(mi->hmsf[3] == 0 || (mi->hmsf[2] == s && 
				      (num == -1 || (num == 0 && mi->sub_timestamp == -1) || num == mi->sub_timestamp)))) {
	      /*	      printf("ref au message trouvée !\n");*/
	      return 1;
	    }
	  }
	}
	p=np;
      }
    }
    mi = mi->next;
  }
  return 0;
}



static tribune_msg_info *
tribune_find_horloge_ref(DLFP_tribune *trib, int caller_id, 
			 int h, int m, int s, int num, unsigned char *commentaire, int comment_sz)
{
  tribune_msg_info *mi, *best_mi;
  int best_mi_num;
  int previous_mi_was_a_match = 0, match;

  mi = trib->msg;
  best_mi = NULL;
  best_mi_num = 0;


  while (mi) {
    match = 0;
    if (mi->id > caller_id && best_mi ) break; /* on ne tente ipot que dans les cas desesperes ! */
    if (s == -1) {
      if ((mi->hmsf[0] == h || (Prefs.pp_use_AM_PM && (mi->hmsf[0] % 12 == h) && mi->hmsf[0] > 12))
	   && mi->hmsf[1] == m && best_mi == NULL && !previous_mi_was_a_match) {
	match = 1;
      }
    } else {
      if ((mi->hmsf[0] == h  || (Prefs.pp_use_AM_PM && (mi->hmsf[0] % 12 == h) && mi->hmsf[0] > 12))
	  && mi->hmsf[1] == m && mi->hmsf[2] == s) {
	if ((num == -1 || num == best_mi_num) && !previous_mi_was_a_match) {
	  match = 1; //break;
	}
	best_mi_num++;
      } else {
	best_mi_num = 0; /* la raison est tordue: si on conserve des messages
			    sur plusieurs jours, le comportement naturel est de
			    renvoyer la ref la + récente. C'est pour ça que le
			    break juste au-dessus est maintenant
			    commenté. Comme ça la boucle est éxecutée jusqu'à
			    ce qu'on atteigne caller_id, même quand on a trouvé
			    un message qui matche parfaitement, juste pour être
			    sûr qu'il n'y en a pas un plus récent qui fasse
			    l'affaire. Mais il faut aussi penser à réinitialiser
			    best_mi_num, cqfd ;-) */			    
      }
    }
    
    if (match) best_mi = mi;
    previous_mi_was_a_match = match;
    mi = mi->next;
  }
  
  if (commentaire) {
    char s_ts[12];
    tribune_msg_info *caller_mi;

    commentaire[0] = 0;
    caller_mi = tribune_find_id(trib, caller_id);
    if (caller_mi) {
      commentaire[0] = 0;
      if (s == -1) {    
	snprintf(s_ts, 10, "%02d:%02d", h,m);
      } else {
	char snum[3];
	snum[0] = snum[1] = snum[2] = 0;
	switch (num) {
	case -1: break;
	case 0: snum[0] = '¹'; break;
	case 1: snum[0] = '²'; break;
	case 2: snum[0] = '³'; break;
	default: snum[0] = ':'; snum[1] = '1' + num;
	}
	snprintf(s_ts, 12, "%02d:%02d:%02d%s", h,m,s,snum);
      }
      if (best_mi == NULL) {
	if (caller_mi->hmsf[0]*60+caller_mi->hmsf[1] < h*60+m) {
	  snprintf(commentaire, comment_sz, _("IPOT(tm) detected"));
	} else {
	  snprintf(commentaire, comment_sz, _("but where is '%s' ?"), s_ts);
	}
      } else if (best_mi->id > caller_mi->id) {
	snprintf(commentaire, comment_sz, _("[IPOT(tm)]"));
      } else if (best_mi->id == caller_mi->id) {
	snprintf(commentaire, comment_sz, _("Awww, we turn around and around and around and around..."));
      } else if (best_mi->in_boitakon) {
	KeyList *hk;
	char *nom;

	nom = (best_mi->login && best_mi->login[0]) ? best_mi->login : best_mi->useragent;
	hk = tribune_key_list_test_mi(trib, best_mi, Prefs.plopify_key_list);
	if (hk) {
	  snprintf(commentaire, comment_sz, _("Hello from %.30s in the boitakon ! "
		   "(because %s=%.20s)"), nom,
		   tribune_key_list_type_name(hk->type), hk->key);
	} else {
	  snprintf(commentaire, comment_sz, _("Hello from %.30s in the boitakon, "
		   "BUT YOU HAVE JUST FOUND A BUG IN THE BOITAKON :-("), nom);
	}
      }
    }
  }
  return best_mi;
}


tribune_msg_info *
check_for_horloge_ref(DLFP_tribune *trib, int caller_id, 
		      const unsigned char *ww, unsigned char *commentaire, int comment_sz, int *is_a_ref, int *ref_num)
{
  int h, m, s, num; /* num est utilise pour les posts multiples (qui on un même timestamp) */

  *is_a_ref = 0;
  if (check_for_horloge_ref_basic(ww, &h, &m, &s, &num) == 0) return NULL;
  *is_a_ref = 1;

  if (ref_num) *ref_num = num;
  
  return tribune_find_horloge_ref(trib, caller_id, 
				  h, m, s, num, commentaire, comment_sz);
}


void
tribune_msg_find_refs(DLFP_tribune *trib, tribune_msg_info *mi)
{
  const unsigned char *p, *np;
  int max_nb_refs;

  p = mi->msg;
  max_nb_refs = mi->nb_refs = 0;
  mi->refs = NULL;

  while (p) {
     int has_initial_space = 0; /* inutilisé */
     unsigned char tok[512];
     
     if (tribune_get_tok(&p,&np,tok,512, &has_initial_space) == NULL) { break; }
     if (tok[0] >= '0' && tok[0] <= '9') {
       int h,m,s,num;
       if (check_for_horloge_ref_basic(tok, &h, &m, &s, &num)) {
	 tribune_msg_info *ref_mi;

	 if (mi->nb_refs+1 > max_nb_refs) {
	   max_nb_refs += 10;
	   mi->refs = realloc(mi->refs, max_nb_refs*sizeof(tribune_msg_ref));
	 }

	 mi->refs[mi->nb_refs].h = h;
	 mi->refs[mi->nb_refs].m = m;
	 mi->refs[mi->nb_refs].s = s;
	 mi->refs[mi->nb_refs].num = num;
	 mi->refs[mi->nb_refs].nbmi = 0;
	 mi->refs[mi->nb_refs].mi = NULL;

	 ref_mi = tribune_find_horloge_ref(trib, mi->id, 
					   h, m, s, num, NULL, 0);
	 if (ref_mi && ref_mi->id <= mi->id) {
	   mi->refs[mi->nb_refs].mi = ref_mi;
	   mi->refs[mi->nb_refs].nbmi=1;
	   if (num == -1) {
	     /* gestion des post multiples */
	     tribune_msg_info *ref_mi2;
	     ref_mi2 = ref_mi->next;
	     while (ref_mi2 && ref_mi2->timestamp == ref_mi->timestamp) {
	       mi->refs[mi->nb_refs].nbmi++;
	       ref_mi2 = ref_mi2->next;
	     }
	   }
	 }
	 mi->nb_refs++;
       }
     }
     p=np;
  }
  if (mi->refs) mi->refs = realloc(mi->refs, mi->nb_refs*sizeof(tribune_msg_ref));

  if (Prefs.verbosity > 3) { 
    int i;
    myprintf("msg[%<YEL %04d>]: REFS=", mi->id);
    for (i=0; i < mi->nb_refs; i++) {
      myprintf("{%<CYA %02d>:%<CYA %02d>:%<CYA %02d>(%<cya %d>), %<YEL %04d>/%<GRN %d>} ", 
	mi->refs[i].h, mi->refs[i].m, mi->refs[i].s, mi->refs[i].num, mi->refs[i].nbmi ? mi->refs[i].mi->id : -1, mi->refs[i].nbmi);
    }
    myprintf("\n");
  }
}

KeyList *
tribune_key_list_clear_from_prefs(KeyList *first)
{
  KeyList *hk;
  hk = first;
  while (hk) {
    if (hk->from_prefs) {
      first = tribune_key_list_remove(first, hk->key, hk->type);
      hk = first;
    }
    if (hk) hk = hk->next;
  }
  return first;
}

/* supprime les keylist faisant ref à des messages detruits */
KeyList *
tribune_key_list_cleanup(DLFP_tribune *trib, KeyList *first)
{
  KeyList *hk;
  hk = first;
  while (hk) {
    if (hk->type == HK_ID || hk->type == HK_THREAD) {
      int id = atoi(hk->key);
      if (tribune_find_id(trib, id) == NULL) {
	first = tribune_key_list_remove(first, hk->key, hk->type);
	hk = first;
      }
    }
    if (hk) hk = hk->next;
  }
  return first;
}

void
tribune_key_list_destroy(KeyList *first)
{
  KeyList *hk, *n;
  hk = first;
  while (hk) {
    n = hk->next;
    free(hk->key); free(hk); hk = n;
  }
}
 
KeyList *
tribune_key_list_add(KeyList *first, const unsigned char *key, KeyListType type, int num, int from_prefs)
{
  KeyList *hk, *last;

  ALLOC_OBJ(hk, KeyList);
  hk->key = strdup(key);
  hk->type = type;
  hk->num = num;
  hk->from_prefs = from_prefs;
  hk->next = NULL;
  
  BLAHBLAH(1, myprintf(_("Adding keyword: '%<CYA %s>'\n"), key));
  last = first;
  if (last == NULL) {
    first = hk;
  } else {
    while (last->next != NULL && last->next->num > hk->num) last = last->next;
    hk->next = last->next;
    last->next = hk;
  }
  return first;
}

KeyList *
tribune_key_list_remove(KeyList *first, const unsigned char *key, KeyListType type)
{
  KeyList *hk, *prev;

  prev = NULL;
  hk = first;
  while (hk) {
    if (strcasecmp(key, hk->key)==0 && (hk->type == type || type == HK_ALL)) {
      BLAHBLAH(1, myprintf(_("Deleting key: '%<CYA %s>'\n"), key));
      if (prev) {
	/* supprime les refs des postvisuals vers cette clef.. */
	prev->next = hk->next;
      } else {
	first = hk->next;
      }
      free(hk->key);
      free(hk);
      break;
    }
    prev = hk;
    hk = hk->next;
  }
  return first;
}

static int
tribune_key_list_test_thread(DLFP_tribune *trib, tribune_msg_info *mi, int thread_id, int *antibug)
{
  int i;
  (*antibug)++;
  if (*antibug > 10000) { printf(_("sniff? sniff ? soit vous vous amusez à faire de threads de folie, soit ça sent le BEUGGUE!\n")); return 0; }

  if (mi == NULL) return 0;

  mi->bidouille_qui_pue = 1;

  /*  printf("test: mi->id=%d, %d\n", mi->id, thread_id); */

  if (mi->id == thread_id) return 1;

  for (i = 0; i < mi->nb_refs; i++) {
    int j;
    tribune_msg_info *tmi;

    tmi = mi->refs[i].mi; j = 0;
    while (j < mi->refs[i].nbmi && tmi) {
      if (tmi->bidouille_qui_pue == 0 && tribune_key_list_test_thread(trib, tmi, thread_id, antibug)) return 1;
      tmi = tmi->next; j++;
    }
  }
  return 0;
}

static int
tribune_key_list_test_mi_hk(DLFP_tribune *trib, tribune_msg_info *mi, KeyList *hk)
{
  if (hk->type == HK_UA) {
    if (strcmp(hk->key, mi->useragent) == 0) {
      return 1;
    }
  } else if (hk->type == HK_UA_NOLOGIN) {
    if (mi->login[0]==0 && strcmp(hk->key, mi->useragent) == 0) {
      return 1;
    }
  }  else if (hk->type == HK_LOGIN) {
    if (strcmp(hk->key, mi->login) == 0) {
      return 1;
    }
  } else if (hk->type == HK_WORD) {
    if (str_noaccent_casestr(mi->msg, hk->key)) {
      /* printf("mot clef %s trouvé dans le msg id=%d\n", hk->key, mi->id); */
      return 1;
    } 
  } else if (hk->type == HK_ID) {
    char sid[10];
    snprintf(sid,10, "%d", mi->id);
    if (strcmp(sid, hk->key) == 0) {
      return 1;
    }
  } else if (hk->type == HK_THREAD) {
    tribune_msg_info *tmi;
    int id;
    int antibug = 0;
    
    tmi = trib->msg; while (tmi) { tmi->bidouille_qui_pue = 0; tmi = tmi->next; }	
    id = atoi(hk->key);
    
    if (tribune_key_list_test_thread(trib, mi, id, &antibug)) { return 1; }
  }
  return 0;
}

KeyList *
tribune_key_list_test_mi(DLFP_tribune *trib, tribune_msg_info *mi, KeyList *klist)
{
  KeyList *hk;
  char sid[10];

  if (mi == NULL) return NULL;
  snprintf(sid,10, "%d", mi->id);
  
  hk = klist;
  while (hk) {
    if (tribune_key_list_test_mi_hk(trib,mi,hk)) return hk;
    hk = hk->next;
  }
  return NULL;
}


KeyList *
tribune_key_list_test_mi_num(DLFP_tribune *trib, tribune_msg_info *mi, KeyList *klist, int num)
{
  KeyList *hk;

  if (mi == NULL) return NULL;
  
  hk = klist;
  while (hk) {
    if (hk->num == num)
      if (tribune_key_list_test_mi_hk(trib,mi,hk)) return hk;
    hk = hk->next;
  }
  return NULL;
}



KeyList *
tribune_key_list_find(KeyList *hk, const char *s, KeyListType t)
{
  while (hk) {
    if ((hk->type != HK_WORD && strcmp(hk->key, s)==0) ||
	(hk->type == HK_WORD && strcasecmp(hk->key, s)==0)) {
      if (t == HK_ALL || t == hk->type) {
	return hk;
      }
    }
    hk = hk->next;
  }
  return NULL;
}

KeyList *
tribune_key_list_swap(KeyList *first, const char *s, KeyListType t, int num)
{
  /* verifie si le mot est déjà dans la liste */
  if (tribune_key_list_find(first, s, t) == NULL) {
    return tribune_key_list_add(first, s, t, num, 0);
  } else {
    return tribune_key_list_remove(first, s, t);
  }
}

const char*
tribune_key_list_type_name(KeyListType t)
{
  switch (t) {
  case HK_UA: return "useragent"; 
  case HK_UA_NOLOGIN: return "useragent w/o login";
  case HK_LOGIN: return "login";
  case HK_WORD: return "mot";
  case HK_ID: return "message_id";
  case HK_THREAD: return "thread depuis l'id";
  case HK_ALL: break;
  }
  return NULL;
}

/* renvoie un hash_code identifiant l'état de la boitakon (pour savoir si elle a été modifiée ou pas */
unsigned
tribune_key_list_get_state(KeyList *first, int num) {
  unsigned hash = 0x98651030;
  static unsigned bloup[4] = {0xf0e84bb1,0x8124e841,0xd1ccc871,0x31415976};
  KeyList *hk;
  int cnt = 0;

  hk = first;
  while (hk) {
    if (hk->num == num) {
      hash ^= (((int)hk->type) ^ bloup[cnt % 4]);
      hash ^= str_hache(hk->key, 100);
      cnt++;
    }
    hk = hk->next;
  }
  return hash;
}
