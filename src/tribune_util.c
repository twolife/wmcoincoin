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

  rcsid=$Id: tribune_util.c,v 1.1 2002/01/18 19:45:58 pouaite Exp $
  ChangeLog:
  $Log: tribune_util.c,v $
  Revision 1.1  2002/01/18 19:45:58  pouaite
  petit oubli d'un fichier..

*/

#include "coincoin.h"

tribune_msg_info *
tribune_find_id(const DLFP_tribune *trib, int id)
{
  tribune_msg_info *it;

  it = trib->msg; 
  while (it) {
    if (it->id == id) return it;
    it = it->next;
  }
  return NULL;
}

tribune_msg_info *
tribune_find_previous(const DLFP_tribune *trib, tribune_msg_info *mi)
{
  tribune_msg_info *it;

  it = trib->msg; 
  while (it) {
    if (it->next == mi) return it;
    it = it->next;
  }
  return NULL;
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
  int l, h, m, s, num; // num est utilise pour les posts multiples (qui on un même timestamp)
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

    //  } else if (use_deuxpt <= 2) {
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

  //has_initial_space doit etre initialise dans la procedure appelante (sinon y'a des pbs avec les tag html)
  //*has_initial_space = 0; /* indique si le token commence par un (ou plusieurs) espace */

  start = *p; *np = NULL;
  // saute les espaces
  while (*start <= ' ' && *start) { start++; *has_initial_space = 1; }
  end = start;

  /* les bon vieux tags html */
  if (*start == '<') {
    static const char *balise = "abusiABUSI";
    int i;
    /* c'est un peu chiant, on risque de mal reconnaitre les balise ou
       d'en reconnaitre qui ont ete ajoutees a la main 
       on teste d'abord les balises courantes
    */
    for (i=0; i < 10; i++) { 
      if (start[1] == balise[i] && start[2] == '>') {
	end = start+3;
	break;
      } else if (start[1] == '/' && start[2] == balise[i] && start[3] == '>') {
	end = start+4;
	break;
      }
    }
    if (end == start) {
      int is_href;
      const unsigned char *s1 = "<a href=\"http://";
      const unsigned char *s2 = "<a href=\"ftp://";
      const unsigned char *s3 = "<a href=\"https://";
      /* puis les <a href> (c'est un peu particulier */

      /* c'est un peu facho, d'autant que c'est reverifié au niveau de open_url, mais
	 bon, apres la douche froide... */
      is_href = 0;
      if (strncasecmp(start, s1, strlen(s1)) == 0) is_href = 1; 
      if (strncasecmp(start, s2, strlen(s2)) == 0) is_href = 1; 
      if (strncasecmp(start, s3, strlen(s3)) == 0) is_href = 1; 
      if (is_href) {
	//	printf("get_tok: '");
	while (*end && *end != '>') end++; //{ printf("%c", *end); end++; }
	//printf("\n");
	if (*end) end++;
      } else {
	/* sinon on ignore */
	end++;
	//	printf("get_tok pas reconnu: '");
	while (*end && *end != '<' && *end > ' ') end++; //{ printf("%c", *end); end++;}
	//printf("\n");
      }
    }
  } else {
    /* pour aider la reconnaissance des timestamp */
    if (*end >= '0' && *end <= '9') {
      while (*end && 
	     ((*end >= '0' && *end <= '9') || strchr(":.hm¹²³", *end))) {
	end++;
      }
      /* un petit coup de marche arriere si on n'a pas termine sur un chiffre */
      if (end-start > 4 && (*(end-1) == ':' || *(end-1) == '.' || *(end-1) == 'm')) end--;
    } else {
      /* un mot normal */
      while (*end && *end != '<' && *end > ' ' && (*end < '0' || *end > '9')) end++;
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
  
  printf("test de %02d:%02d:%2d(%d)..\n", ref_mi->hmsf[0], ref_mi->hmsf[1], ref_mi->hmsf[2], ref_mi->hmsf[3]);
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
	    //	  printf(" id%05d -> contient ref '%s'\n", mi->id, tok);
	    if (h == mi->hmsf[0] && m == mi->hmsf[1] && 
		(mi->hmsf[3] == 0 || mi->hmsf[2] == s)) {
	      printf("ref au message trouvée !\n");
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

  mi = trib->msg;
  best_mi = NULL;
  best_mi_num = 0;

  while (mi) {
    if (mi->id > caller_id && best_mi ) break; /* on ne tente ipot que dans les cas desesperes ! */
    if (s == -1) {
      if (mi->hmsf[0] == h && mi->hmsf[1] == m && best_mi == NULL) {
	best_mi = mi;
      }
    } else {
      if (mi->hmsf[0] == h && mi->hmsf[1] == m && mi->hmsf[2] == s) {
	if (num == -1 || num == best_mi_num) {
	  best_mi = mi; break;
	}
	best_mi_num++;
      }
    }
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
	  snprintf(commentaire, comment_sz, "IPOT(tm) detected");
	} else {
	  snprintf(commentaire, comment_sz, "où qu'il est '%s' ?", s_ts);
	}
      } else if (best_mi->id > caller_mi->id) {
	snprintf(commentaire, comment_sz, "[IPOT(tm)]");
      } else if (best_mi->id == caller_mi->id) {
	snprintf(commentaire, comment_sz, "merde on tourne en rond merde on tourne en rond merde...");
      }
    }
  }
  return best_mi;
}


tribune_msg_info *
check_for_horloge_ref(DLFP_tribune *trib, int caller_id, 
		      const unsigned char *ww, unsigned char *commentaire, int comment_sz, int *is_a_ref, int *ref_num)
{
  int h, m, s, num; // num est utilise pour les posts multiples (qui on un même timestamp)

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
	     while (ref_mi2->timestamp == ref_mi->timestamp) {
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

  { 
    int i;
    myprintf("msg[%<YEL %04d>]: REFS=", mi->id);
    for (i=0; i < mi->nb_refs; i++) {
      myprintf("{%<CYA %02d>:%<CYA %02d>:%<CYA %02d>(%<cya %d>), %<YEL %04d>/%<GRN %d>} ", 
	mi->refs[i].h, mi->refs[i].m, mi->refs[i].s, mi->refs[i].num, mi->refs[i].nbmi ? mi->refs[i].mi->id : -1, mi->refs[i].nbmi);
    }
    myprintf("\n");
  }
}
