/*
  rcsid=$Id: pinnipede.c,v 1.16 2002/01/20 20:53:22 pouaite Exp $
  ChangeLog:
  $Log: pinnipede.c,v $
  Revision 1.16  2002/01/20 20:53:22  pouaite
  bugfix configure.in && http_win.c pour cygwin + 2-3 petis trucs

  Revision 1.15  2002/01/20 02:17:13  pouaite
  modifs d'ordre esthetique (!) sans grand interet

  Revision 1.14  2002/01/19 19:56:09  pouaite
  petits crochets pour la mise en valeur de certains messages (cf changelog)

  Revision 1.13  2002/01/18 00:28:42  pouaite
  le m�nage continue + grosses modifs (experimentales pour l'instant)

  Revision 1.12  2002/01/16 00:35:26  pouaite
  debut de detection des reponse � nos message avec des couleurs hideuses et certainement plein de bugs moisis

  Revision 1.11  2002/01/14 23:54:06  pouaite
  reconnaissance des posts effectu� par l'utilisateur du canard (� suivre...)

  Revision 1.10  2002/01/13 15:19:00  pouaite
  double patch: shift -> tribune.post_cmd et lordOric -> tribune.archive

  Revision 1.9  2002/01/12 19:03:54  pouaite
  bugfix de picohtml et raccourci altgr-e pour le symbole euro (gruik)

  Revision 1.7  2002/01/06 17:06:27  pouaite
  enlevage du patch de glandium (enfin j'essaye..)

  Revision 1.5  2001/12/16 20:28:45  pouaite
  bugfixes divers

  Revision 1.4  2001/12/16 16:46:12  pouaite
  Clippouille joins C0IN C0IN

  Revision 1.3  2001/12/16 01:43:33  pouaite
  filtrage des posts, meilleure gestion des posts multiples

  Revision 1.2  2001/12/02 18:20:58  pouaite
  correction (enfin!) du bug d'affichage lorsque plusieurs posts ont le m�me timestamp

*/

#include <X11/xpm.h>
#include "coincoin.h"
#include "time.h"
#include "../xpms/pp_minib.xpm"

/* chuis con, les bitfields c pas pour les chiens */
#define PWATTR_BD 1
#define PWATTR_IT 2
#define PWATTR_U  4
#define PWATTR_S  8
#define PWATTR_LNK 16
#define PWATTR_TSTAMP 32
//#define PWATTR_UA 64
#define PWATTR_NICK 128
#define PWATTR_REF 256 /* reference vers un autre post */
#define PWATTR_TMP_EMPH 512
#define PWATTR_HAS_INITIAL_SPACE 1024 /* indique si un espace doit etre insere entre ce mot et le
			     suivant (utilise par justif */
#define PWATTR_TROLLSCORE 2048
#define PWATTR_LOGIN 4096

#define NB_MINIB 11
#define MINIB_BW 20
#define MINIB_BH 12
#define MINIB_W (NB_MINIB*(MINIB_BW+1))
#define MINIB_H 13
#define MINIB_X0 (pp->win_width - MINIB_W)
#define MINIB_Y0 (pp->win_height - MINIB_H)
#define MINIB_X1 (MINIB_X0 + MINIB_W - 1)
#define MINIB_Y1 (MINIB_Y0 + MINIB_H - 1)

#define MINIB_BX(i) (pp->win_width+1 - (MINIB_BW+1)*(i+1))
#define MINIB_BY(i) (MINIB_Y0+1)

#define NB_MINIB 11

#define MINIB_FN_W 6

typedef struct _PostVisual PostVisual;
typedef struct _PostWord PostWord;

/* une liste de mots avec leurs attributs */
struct _PostWord {
  unsigned char *w; /* non malloc�, stocke dans la m�me zone que cette structure */
  unsigned char *attr_s;
  unsigned short attr;
  short xpos, xwidth, ligne;
  struct _PostWord *next;
  struct _PostVisual *parent;
};

/* liste chain�e de posts */
struct _PostVisual {
  int id; // message id 
  PostWord *first; /* la liste des mots */
  time_t tstamp;
  char sub_tstamp; /* sous numerotation quand plusieurs posts ont le m�me timestamp */
  int nblig:12; // nombre de lignes necessaire pour afficher ce message
  int ref_cnt:10; // compteur de references
  int is_my_message:1;
  int is_answer_to_me:1;
  int is_hilight_key:1;
  struct _PostVisual *next;
};

struct _PinnipedeFilter {
  int filter_mode;
  char *filter_name; /* contient le 'nom' du filtre) 
		      */
  char *ua;
  char *login;
  char *word;
  //  int hms[3]; /* filtre sur les ref au msg post� � l'heure indiqu�e dans hms */
  int *id; int nid; /* liste des id des messages affich�s dans le filtre de threads */
};


struct _Pinnipede {
  Window win;
  unsigned long win_bgpixel, timestamp_pixel, nick_pixel, login_pixel, 
    emph_pixel, trollscore_pixel, lnk_pixel, txt_pixel, strike_pixel, 
    popup_fgpixel, popup_bgpixel, minib_pixel, 
    hilight_my_msg_pixel,hilight_answer_my_msg_pixel,hilight_keyword_pixel;
  int mapped;
  int win_width, win_height, win_xpos, win_ypos;

  int zmsg_y, zmsg_h; /* zone d'affichage des messages */
  XFontStruct *fn_base, *fn_it, *fn_bd, *fn_itbd, *fn_minib;
  int fn_h;
  Pixmap minipix;

  PostVisual *pv;
  PostWord **lignes;
  int nb_lignes;
  int id_base, decal_base;
  int last_post_id; /* utilise pour savoir si la tribune a ete mise a jour.. */

  //  int html_mode;
  int nick_mode; /* 0 : n'affiche rien, 
		    1:  affiche les useragent raccourcis, 
		    2: affiche les logins, 
		    3: affiche les deux, 
		    4: affiche useragent ou login */
  int nosec_mode; /* supprime les secondes sur les posts ou c'est possible */
  int trollscore_mode;
  int survol_hash; /* pour determiner (a peu pres) si on affcihe une nouvelle info de survol... (apprixmatif...) */
  int use_minibar; /* les miniboutons sont-ils affich�s ? */
  int minib_pressed; /* numero du minibouton enfonce, -1 si aucun */
  int fortune_mode;
  int hilight_my_message_mode;
  int hilight_answer_to_me_mode;
  int hilight_key_mode;
  
  volatile int flag_tribune_updated;
  Pixmap lpix;

  PicoHtml *ph_fortune;
  int fortune_h, fortune_w;

  struct _PinnipedeFilter filter;

  //  int selection_mode; /* non nul quand on est en train de selectionner du texte � copier dans le clipboard (en dragant avec la souris) */
};



static int
pp_thread_filter_find_id(struct _PinnipedeFilter *f, int id) {
  int i;
  for (i=0; i < f->nid; i++) {
    if (f->id[i] == id) return 1;
  }
  return 0;
}

static int
filter_msg_info(tribune_msg_info *mi, struct _PinnipedeFilter *filter)
{
  if (filter->filter_mode == 0) return 1;
  if (filter->ua) {
    return (strcmp(filter->ua, mi->useragent) == 0);
  } else if (filter->login) {
    return (strcmp(filter->login, mi->login) == 0);
  } else if (filter->word && strlen(filter->word)) {
    return (strstr(mi->msg, filter->word) != NULL);
  } else if (filter->id != NULL) {
    return pp_thread_filter_find_id(filter, mi->id);
  } else {
    return 1;
  }
}



/* les deux fonctions suivantes permettent de se balader dans la liste des posts 
 (de maniere bourrine... c pas pour 25000 messages )
*/
int 
get_next_id(DLFP_tribune *trib, int id, tribune_msg_info **nmi, struct _PinnipedeFilter *filter) 
{
  tribune_msg_info *mi;
  int nid;

  mi = trib->msg;
  nid = -1;
  while (mi) {
    if (mi->id > id) {
      if (filter == NULL || filter_msg_info(mi,filter)) {
	nid = mi->id;
	break;
      }
    }
    mi = mi->next;
  }
  if (nmi) *nmi = mi;
  return nid;
}

int 
get_prev_id(DLFP_tribune *trib, int id, tribune_msg_info **prev, struct _PinnipedeFilter *filter) 
{
  tribune_msg_info *mi,*pmi;

  mi = trib->msg;
  pmi = NULL;
  while (mi) {
    if (mi && pmi) {
      if (pmi->id < id && mi->id >= id) {
	if (prev) *prev = pmi;
	return pmi->id;
      }
    }
    if (filter == NULL || filter_msg_info(mi,filter)) {
      pmi = mi;
    }
    mi = mi->next;
  }
  if (prev) *prev = NULL;
  return -1;
}

void
pp_unset_filter(struct _PinnipedeFilter *f)
{
  f->filter_mode = 0;
  if (f->filter_name) { free(f->filter_name); f->filter_name = NULL;}
  if (f->ua) { free(f->ua); f->ua = NULL;}
  if (f->login) { free(f->login); f->login = NULL; }
  if (f->word) { free(f->word); f->word = NULL; }
  if (f->id) { free(f->id); f->id = NULL; f->nid = 0; }
}

static void
pv_destroy(PostVisual *pv)
{
  PostWord *pw,*pw2;

  //  printf("pv_destroy(id=%d)\n", pv->id);
  pw = pv->first;
  while (pw) {
    pw2 = pw->next;
    free(pw);
    pw = pw2;
  }
  free(pv);
}

static PostWord*
pw_create(const unsigned char *w, unsigned short attr, const unsigned char *attr_s, PostVisual *parent)
{
  int alen; 
  int wlen;
  PostWord *pw;

  wlen = strlen(w);
  if (attr_s) alen=strlen(attr_s); else alen = -1;
  pw = malloc(sizeof(PostWord)+wlen+1+alen+1);
  pw->w = ((unsigned char*)pw)+sizeof(PostWord); strcpy(pw->w,w);
  if (attr_s) {
    pw->attr_s = ((unsigned char*)pw)+sizeof(PostWord)+wlen+1;
    strcpy(pw->attr_s, attr_s);
  } else pw->attr_s = NULL;
  pw->next = NULL;
  pw->xpos = -1; pw->ligne=-1; pw->xwidth = 0; 
  pw->attr = attr;
  pw->parent = parent;
  return pw;
}


/* construction d'un postvisual � partir du message 'mi' */
static PostVisual *
pv_tmsgi_parse(DLFP_tribune *trib, const tribune_msg_info *mi, int with_seconds, int html_mode, int nick_mode, int troll_mode) {
#define PVTP_SZ 512

  PostVisual *pv;
  PostWord *pw, *tmp;
  
  unsigned char s[PVTP_SZ];
  unsigned char attr_s[PVTP_SZ];

  const unsigned char *p, *np;
  unsigned short attr;
  int add_word;
  int has_initial_space; // indique si le prochain mot est coll� au precedent

  ALLOC_OBJ(pv, PostVisual);

  pv->first = NULL;
  pv->nblig = -1;
  pv->ref_cnt = 0;
  pv->next = NULL;
  pv->id = mi->id;
  pv->tstamp = mi->timestamp;
  pv->sub_tstamp = mi->sub_timestamp;
  pv->is_my_message = mi->is_my_message;
  pv->is_answer_to_me = mi->is_answer_to_me;
  pv->is_hilight_key = tribune_hilight_key_list_test_mi(mi, trib->hilight_key_list) == NULL ? 0 : 1;

  pw = NULL;

  if (troll_mode && Prefs.enable_troll_detector) {
    if (mi->troll_score > 0) {
      snprintf(s, PVTP_SZ, "%d", mi->troll_score);
    } else {
      snprintf(s, PVTP_SZ, " ");
    }

    tmp = pw_create(s, PWATTR_TROLLSCORE | (mi->troll_score > 2 ? PWATTR_BD : 0), NULL, pv);
    if (pw == NULL) { pv->first = tmp; } else { pw->next = tmp; }
    pw = tmp;
  }

  /* affichage timestamp */
  if (with_seconds) {
    snprintf(s, PVTP_SZ, "%02d:%02d:%02d",mi->hmsf[0], mi->hmsf[1], mi->hmsf[2]);
  } else {
    snprintf(s, PVTP_SZ, "%02d:%02d",mi->hmsf[0], mi->hmsf[1]);
  }
  
  tmp = pw_create(s, PWATTR_TSTAMP | (pw == NULL ? 0 : PWATTR_HAS_INITIAL_SPACE), NULL, pv);
  if (pw == NULL) { pv->first = tmp; } else { pw->next = tmp; }
  pw = tmp;


  if (nick_mode) {
    char *p;
#define SUA_SZ 15
    char short_ua[SUA_SZ];
    int is_login;

    if (mi->tatouage) {
      p = mi->tatouage->name;
      if (strcmp(mi->tatouage->name, "?") == 0) {
	make_short_name_from_ua(mi->useragent, short_ua, SUA_SZ);
	p = short_ua;
      }
    } else p = "[???]";

    is_login = 0;
    if (nick_mode == 4 && strlen(mi->login) != 0) {
      p = mi->login; is_login = 1;
    }
    
    if (nick_mode == 1 || nick_mode == 3 || nick_mode == 4) {
      tmp = pw_create(p, (is_login == 0 ? PWATTR_NICK : PWATTR_LOGIN) | PWATTR_HAS_INITIAL_SPACE, NULL, pv);
      if (pw == NULL) { pv->first = tmp; } else { pw->next = tmp; }
      pw = tmp;
    }

    if ((nick_mode == 2 || nick_mode == 3) && strlen(mi->login)) {
      tmp = pw_create(mi->login, PWATTR_LOGIN | PWATTR_HAS_INITIAL_SPACE, NULL, pv);
      if (pw == NULL) { pv->first = tmp; } else { pw->next = tmp; }
      pw = tmp;      
    }
  }

  p = mi->msg;
  attr = 0;

  has_initial_space = 1;
  while (p) {
    add_word = 1;
    if (tribune_get_tok(&p,&np,s,PVTP_SZ, &has_initial_space) == NULL) { break; }
    if (s[0] == '<' && html_mode) {
      add_word = 0;
      if (strcasecmp(s, "<i>")==0) {
	attr |= PWATTR_IT; 
      } else if (strcasecmp(s,"</i>")==0) {
	attr &= (~PWATTR_IT);
      } else if (strcasecmp(s,"<b>")==0) {
	attr |= PWATTR_BD; 
      } else if (strcasecmp(s,"</b>")==0) {
	attr &= (~PWATTR_BD);
      } else if (strcasecmp(s,"<u>")==0) {
	attr |= PWATTR_U;
      } else if (strcasecmp(s,"</u>")==0) {
	attr &= (~PWATTR_U);
      } else if (strcasecmp(s,"<s>")==0) {
	attr |= PWATTR_S; 
      } else if (strcasecmp(s,"</s>")==0) {
	attr &= (~PWATTR_S);
      } else if (strncasecmp(s,"<a href=\"", 9)==0) {
	int i;
	attr |= PWATTR_LNK;
	i = 9;
	while (s[i] != '\"' && s[i]) i++;
	s[i] = 0;
	strncpy(attr_s, s+9, PVTP_SZ); 
      } else if (strcasecmp(s,"</a>")==0) {
	attr &= (~PWATTR_LNK);
      } else {
	add_word = 1;
      }
    }
    if (add_word) {
      int is_ref;
      tribune_msg_info *ref_mi;

      ref_mi = check_for_horloge_ref(trib, mi->id, s,attr_s, PVTP_SZ, &is_ref, NULL);
      if (is_ref) {
	attr |= PWATTR_REF;
      }

      if (has_initial_space) {
	attr |= PWATTR_HAS_INITIAL_SPACE;
      } else {
	attr &= (~PWATTR_HAS_INITIAL_SPACE);
      }

      pw->next = pw_create(s, attr, (attr & PWATTR_LNK) ? attr_s : NULL, pv);
      has_initial_space = 0;
      attr &= ~PWATTR_REF;
      //      printf("ADD(id=%d): '%s'\n", mi->id, s);
      pw = pw->next;
    }
    p=np;
  }
  return pv;
}

static XFontStruct *
pv_get_font(Pinnipede *pp, unsigned attr)
{
  XFontStruct *fn;
  if ((attr & PWATTR_BD) && (attr & PWATTR_IT)) {
    fn = pp->fn_itbd;
  } else if (attr & PWATTR_IT) {
    fn = pp->fn_it;
  } else if (attr & (PWATTR_BD)) { //|PWATTR_TMP_EMPH)) {
    fn = pp->fn_bd;
  } else {
    fn = pp->fn_base;
  }
  return fn;
}

static void
pv_justif(Pinnipede *pp, PostVisual *pv, int x0, int width) {
  int x, start_of_line;
  PostWord *pw;
  XFontStruct *fn;
  int trollscore_width = 0;

  pw = pv->first;
  start_of_line = 1;
  x = x0;
  fn = pp->fn_base;
  pv->nblig = 0;

  if (pp->trollscore_mode) {
    fn = pv_get_font(pp, PWATTR_BD);
    trollscore_width = XTextWidth(fn, "0", 1);
  } else {
    trollscore_width = 0;
  }

  while (pw) {
    if (start_of_line) {
      pv->nblig++; 
    } else x += (pw->attr & PWATTR_HAS_INITIAL_SPACE) ? 4 : 0;
    fn = pv_get_font(pp, pw->attr);
    assert(pw->w); assert(strlen(pw->w));
    pw->xwidth = XTextWidth(fn, pw->w, strlen(pw->w));
    
    if (pw->attr & PWATTR_TROLLSCORE) {
      pw->xwidth = MAX(XTextWidth(fn, pw->w, strlen(pw->w)),trollscore_width);
    }

    pw->ligne = pv->nblig;
    pw->xpos = x;
    x += pw->xwidth;
    if (x > width-3 && start_of_line == 0) { 
      x = 13; start_of_line = 1;       
    } else {
      pw = pw->next;
      start_of_line = 0;
    }
  }
}

static PostVisual *
pp_find_pv(Pinnipede *pp, int id)
{
  PostVisual *pv;

  pv = pp->pv;
  while (pv) {
    if (pv->id == id) return pv;
    pv = pv->next;
  }
  return NULL;
}

/* ajout (si necessaire) du message 'id' dans la liste */
static PostVisual *
pp_pv_add(Pinnipede *pp, DLFP_tribune *trib, int id)
{
  PostVisual *pv;
  int with_seconds = 1;

  pv = pp_find_pv(pp,id);
  if (pv) {
    pv->ref_cnt++;
  } else {
    tribune_msg_info *mi;

    mi = tribune_find_id(trib,id);
    if (mi == NULL) {
      return NULL;
    }

    if (pp->nosec_mode) {
      with_seconds = mi->hmsf[3];
    }

    pv = pv_tmsgi_parse(trib, mi, with_seconds, 1, pp->nick_mode, pp->trollscore_mode); 
    pv_justif(pp, pv, 8, pp->win_width);
    assert(pv);
    pv->next = pp->pv;
    pp->pv = pv;
  }
  return pv;
}

static int
pp_pv_uncount(Pinnipede *pp) {
  PostVisual *pv;
  int cnt = 0;

  pv = pp->pv;
  while (pv) {
    //    assert(pv->ref_cnt > 0);
    pv->ref_cnt = 0;
    pv = pv->next;
  }
  return cnt;
}

static int
pp_pv_garbage_collect(Pinnipede *pp) {
  PostVisual *pv, *npv, *ppv;
  int cnt = 0;

  pv = pp->pv;
  ppv = NULL;
  while (pv) {
    //    printf("pv = %d, next = %p\n", pv->id, pv->next);
    assert(pv->ref_cnt >= 0);
    if (pv->ref_cnt == 0) {
      npv = pv->next;
      if (pv == pp->pv) pp->pv = npv;
      pv_destroy(pv);
      if (ppv) ppv->next = npv;
      pv = npv;
      cnt++;
    } else {
      ppv = pv;
      pv = pv->next;
    }
  }
  return cnt;
}

static void
pp_pv_destroy(Pinnipede *pp) {
  PostVisual *pv;

  pv = pp->pv;  
  while (pv) { pv->ref_cnt = 0; pv = pv->next; }
  pp_pv_garbage_collect(pp);
  if (pp->lignes) { free(pp->lignes); pp->lignes = NULL; }
}



/* a appeler quand la fortune est chang�e */
static void
pp_update_fortune(Dock *dock)
{
  Pinnipede *pp = dock->pinnipede;
  
  assert(flag_updating_comments == 0);
  if (!picohtml_isempty(pp->ph_fortune)) {
    picohtml_freetxt(pp->ph_fortune);
  }
  pp->fortune_h = 0; /* quand pp->fortune_h != 0 => il y a une fortune � afficher */
  pp->fortune_w = 0;
  if (pp->fortune_mode && dock->dlfp->fortune) {
    char *s;
    s = dock->dlfp->fortune;
    if (s == NULL) s = "pas de fortune pour l'instant...<br>";
    picohtml_parse(dock, pp->ph_fortune, s, pp->win_width - 6);
    picohtml_gettxtextent(pp->ph_fortune, &pp->fortune_w, &pp->fortune_h);
    if (!picohtml_isempty(pp->ph_fortune)) { /* on s'arr�te si la fortune est vide (s == "" par ex..)
						(�a peut arriver et �a declenche le assert(!isempty) de refresh_fortune) */
      pp->fortune_h += 3; 
      pp->fortune_h = MIN(pp->fortune_h, pp->win_height/2); /* faut pas exagerer */
    }
  }
}



/* macros pour le calcul des differentes positions d'affichage des lignes */
#define LINEY0(l) (pp->win_height-(pp->use_minibar ? MINIB_H:0)-(pp->nb_lignes-l)*pp->fn_h-(pp->zmsg_h - pp->nb_lignes*pp->fn_h)/2) //(pp->zmsg_y + (l)*pp->fn_h)
#define LINEY1(l) (LINEY0(l)+pp->fn_h-1) //(pp->zmsg_y + (l+1)*pp->fn_h-1)
#define LINEBASE(l) (LINEY0(l) + pp->fn_base->ascent) //(pp->zmsg_y + pp->fn_base->ascent + (l-1)*pp->fn_h)

/* adjust: param interne, appeler toujours avec adjust  = 0 */
void
pp_update_content(Dock *dock, DLFP_tribune *trib, int id_base, int decal, int adjust)
{
  Pinnipede *pp = dock->pinnipede;
  int cur_lig, id, plig;
  PostVisual *pv;

  if (flag_updating_tribune) return;

  pp_update_fortune(dock);

  pp->last_post_id = trib->last_post_id;

  pp->zmsg_y = pp->fortune_h == 0 ? 3 : pp->fortune_h;
  pp->zmsg_h = pp->win_height - pp->zmsg_y - (pp->use_minibar ? MINIB_H : 0);


  pp->nb_lignes = pp->zmsg_h / pp->fn_h;
  cur_lig = pp->nb_lignes;

  //  printf("y0 = %d, y1=%d dec=%d\n", LINEY0(0), LINEY1(pp->nb_lignes-1),(pp->zmsg_h - pp->nb_lignes*pp->fn_h)/2);
  
  id = id_base;

  /* 'scroll down' */
  //printf("entree update_content: id_base = %d (%d), decal = %d (%d), adjust=%d\n",
  //id_base,pp->id_base, decal, pp->decal_base, adjust);

  pp->id_base = -1;

  while (decal > 0) {
    int nid;
    nid = get_next_id(trib, id, NULL, &pp->filter);
    if (nid == -1) { decal = 0; break; }
    id = nid;
    pv = pp_pv_add(pp, trib, id);
    if (pv == NULL) { decal = 0; break; }
    decal -= pv->nblig;
  }

  /* assignation des lignes */
  pp_pv_uncount(pp);
  if (pp->lignes) free(pp->lignes);
  ALLOC_VEC(pp->lignes, pp->nb_lignes, PostWord*);

  while (cur_lig >= 0) {
    //printf("cur_lig = %d, id=%d", cur_lig, id);
    pv = pp_pv_add(pp, trib, id); 
    if (pv == NULL) { /* on n'arrive pas a remplir jusqu'en haut ? */
      if (adjust == 0) {
	if (pp->id_base >= 0) {
	//	if (pp->decal_base != 0 || get_next_id(trib, pp->id_base) != -1) { // on n'est pas lign� sur le dernier post ?
	  pp_update_content(dock,trib,pp->id_base,pp->decal_base+cur_lig, 1); /* pas joli-joli */
	  //	}
	} else {
	  id = get_next_id(trib, -1, NULL, &pp->filter); /* premier id */
	  if (id >= 0) { /* sinon �a veut dire que la tribune est comptelemt vide */
	    pp_update_content(dock,trib,id,0, 0); /* pas joli-joli */
	  }
	}
      }
      break;
    }
    pv->ref_cnt = 0;
    id = get_prev_id(trib, id, NULL, &pp->filter);


    //    if (id == -1 && pp->id_base == -1 && adjust == 0) { /* on vient de scroller beaucoup trop fort */
    //      printf("ca ppue.\n");
    //      id = get_next_id(trib, -1, NULL); /* premier id */
    //      if (id >= 0) { /* sinon �a veut dire que la tribune est comptelemt vide */
    //	pp_update_content(dock,trib,id,0, 0); /* pas joli-joli */
    //      }
    //    }

    //printf("pv->nblig=%d, , pid=%d\n",pv->nblig, id);
    if (decal + pv->nblig > 0) { // test si le 'scroll up a �t� suffisant
      PostWord *pw;

      if (pp->id_base == -1) { /* on sauve l'id_base et le decal soigneusement calcul� */
	pp->id_base = pv->id;
	pp->decal_base = decal;
      }

      cur_lig -= pv->nblig + decal;
      decal = 0;
      plig = 0;
      pw = pv->first;
      for (plig = 0; plig < pv->nblig && cur_lig+plig < pp->nb_lignes; plig++) {
	int l;
	if (cur_lig + plig >= 0) {
	  //	  printf("ajout: ligne %d, pw->w='%s'\n", cur_lig+plig, pw->w);
	  pp->lignes[cur_lig+plig] = pw;
	  pv->ref_cnt = 1;
	}
	l = pw->ligne;
	while (pw && l == pw->ligne) pw = pw->next;
      }
    } else {      
      decal += pv->nblig;
    }
  }
  pp_pv_garbage_collect(pp); // rhooooo
}



void
pp_minib_refresh(Dock *dock)
{
  Pinnipede *pp = dock->pinnipede;
  int i,x,y;


  if (pp->use_minibar == 0) return;

  /* dessin general */
  XSetForeground(dock->display, dock->NormalGC, pp->minib_pixel);

  XFillRectangle(dock->display, pp->lpix, dock->NormalGC, 0, 1, pp->win_width, MINIB_H-1);
  XSetForeground(dock->display, dock->NormalGC, WhitePixel(dock->display, dock->screennum));
  XDrawLine(dock->display, pp->lpix, dock->NormalGC, 
	    0, 0, MINIB_X1, 0);


  if (pp->filter.filter_mode) {
    char s_filtre[50];
    
    if (pp->filter.filter_name) {
      snprintf(s_filtre, 60, pp->filter.filter_name);
    } else {
      snprintf(s_filtre, 60, "FILTRE NON DEFINI");
    }
    XSetFont(dock->display, dock->NormalGC, pp->fn_minib->fid);
    XSetForeground(dock->display, dock->NormalGC, BlackPixel(dock->display, dock->screennum));
    XDrawString(dock->display, pp->lpix, dock->NormalGC, 5, pp->fn_minib->ascent+2, s_filtre, strlen(s_filtre));
  } else if (Prefs.user_cookie || Prefs.force_fortune_retrieval) {
    
    /* affichage de la charge du serveur de dlfp */

    char s_cpu[20];
    char s_xp[20], s_votes[20];
    int x, w;
    s_xp[0] = 0; s_cpu[0] = 0; s_votes[0] = 0;
    if (flag_updating_comments == 0) {
      if (dock->dlfp->CPU != -1.0) {
	snprintf(s_cpu, 20, "cpu:%1.2f", dock->dlfp->CPU);
      } else snprintf(s_cpu, 20, "cpu: ??");
      if (dock->dlfp->xp >= 0) {
	snprintf(s_xp, 20, "xp:%d", dock->dlfp->xp);
	snprintf(s_votes, 20, "[%d/%d]", dock->dlfp->votes_cur, dock->dlfp->votes_max);
      }
    } else {
      snprintf(s_cpu, 20, "maj...");
    }
    XSetFont(dock->display, dock->NormalGC, pp->fn_minib->fid);
    XSetForeground(dock->display, dock->NormalGC, BlackPixel(dock->display, dock->screennum));
    
    x = 5;
    w = MINIB_FN_W*strlen(s_cpu);
    if (pp->win_width - MINIB_W - (x+w) > 0) {
      XDrawString(dock->display, pp->lpix, dock->NormalGC, x, pp->fn_minib->ascent+2, s_cpu, strlen(s_cpu));
    }
    x += w + 6;
    w = MINIB_FN_W*strlen(s_xp);
    if (strlen(s_xp)) {
      if (pp->win_width - MINIB_W - (x+w) > 0) {
	XDrawString(dock->display, pp->lpix, dock->NormalGC, x, pp->fn_minib->ascent+2, s_xp, strlen(s_xp));
      }
    }
    x += w + 6;
    w = MINIB_FN_W*strlen(s_votes);
    if (strlen(s_votes)) {
      if (pp->win_width - MINIB_W - (x+w) > 0) {
	XDrawString(dock->display, pp->lpix, dock->NormalGC, x, pp->fn_minib->ascent+2, s_votes, strlen(s_votes));
      }
    }
  }

  /* dessin des boutons */
  for (i=0;i < NB_MINIB; i++) {
    x = MINIB_BX(i);
    y = 1; //MINIB_BY(i);
    if (i < 6) {
      XCopyArea(dock->display, pp->minipix, pp->lpix, dock->NormalGC,
		i*15, 1, 12, 12, 
		x + 4 + (i == pp->minib_pressed ? 1 : 0), 
		y + (i == pp->minib_pressed ? 1 : 0));
    } else {
      int rx, rw,ry,rh;

      rx = x + 4; ry = y + 2; rw = MINIB_BW - 8; rh = MINIB_BH - 4;
      if (i == 6) XSetForeground(dock->display, dock->NormalGC, pp->nick_pixel);
      else if (i == 7) XSetForeground(dock->display, dock->NormalGC, pp->timestamp_pixel);
      else if (i == 10) XSetForeground(dock->display, dock->NormalGC, pp->lnk_pixel);
      else if (i == 8) XSetForeground(dock->display, dock->NormalGC, pp->trollscore_pixel);
      else if (i == 9) XSetForeground(dock->display, dock->NormalGC, 
				       IRGB2PIXEL(Prefs.pp_fortune_bgcolor == Prefs.pp_bgcolor ?
						  Prefs.pp_fortune_fgcolor : Prefs.pp_fortune_bgcolor));
      else assert(0); /* prends �a gros bug a la con */
      XFillRectangle(dock->display, pp->lpix, dock->NormalGC, rx,ry,rw,rh);

      if ((i ==  6 && pp->nick_mode != 3) ||
	  (i ==  7 && pp->nosec_mode) ||
	  (i ==  10 && pp->filter.filter_mode == 0) ||
	  (i ==  8 && pp->trollscore_mode) ||
	  (i == 9 && pp->fortune_mode))
      {
	XSetForeground(dock->display, dock->NormalGC, pp->win_bgpixel);
	if (i != 6 || pp->nick_mode == 0) {
	  XFillRectangle(dock->display, pp->lpix, dock->NormalGC, rx+2,ry+2,rw-4,rh-4);
	} else { /* cas particulier pour nick_mode qui a 5 positions */
	  if (pp->nick_mode == 1) {
	    XFillRectangle(dock->display, pp->lpix, dock->NormalGC, rx+2+(rw-4)/2,ry+2,(rw-4)/2,rh-4);
	  } else if (pp->nick_mode == 2) {
	    XFillRectangle(dock->display, pp->lpix, dock->NormalGC, rx+2,ry+2,(rw-4)/2,rh-4);
	  } else if (pp->nick_mode == 4) {
	    XFillRectangle(dock->display, pp->lpix, dock->NormalGC, rx+2,ry+2,3,rh-4);	  
	    XFillRectangle(dock->display, pp->lpix, dock->NormalGC, rx+rw-2-3,ry+2,3,rh-4);	  
	  }
	}
      }
    }

    if (i == pp->minib_pressed) { 
      XSetForeground(dock->display, dock->NormalGC, BlackPixel(dock->display, dock->screennum));
    } else {
      XSetForeground(dock->display, dock->NormalGC, pp->minib_pixel);
    }
    XDrawLine(dock->display, pp->lpix, dock->NormalGC, 
	      x, y, x,y+MINIB_BH);
    XDrawLine(dock->display, pp->lpix, dock->NormalGC, 
	      x, y, x+MINIB_BW,y);

    if (i != pp->minib_pressed) { 
      XSetForeground(dock->display, dock->NormalGC, BlackPixel(dock->display, dock->screennum));
    } else {
      XSetForeground(dock->display, dock->NormalGC, pp->minib_pixel);
    }
    XDrawLine(dock->display, pp->lpix, dock->NormalGC, 
	      x+MINIB_BW, y, x+MINIB_BW,y+MINIB_BH);
    XDrawLine(dock->display, pp->lpix, dock->NormalGC, 
	      x, y+MINIB_BH, x+MINIB_BW,y+MINIB_BH);
  }

  /* et hop */
  XCopyArea(dock->display, pp->lpix, pp->win, dock->NormalGC,
	    0,0,pp->win_width, MINIB_H, 0, MINIB_Y0);
}

static int
pp_minib_get_button(Dock *dock, int x, int y)
{
  Pinnipede *pp = dock->pinnipede;
  int i;

  for (i=0; i < NB_MINIB; i++) {
    if (x >= MINIB_BX(i) && x < MINIB_BX(i)+MINIB_W && 
	y >= MINIB_BY(i) && y < MINIB_BY(i)+MINIB_W) {
      return i;
    }
  }
  return -1;
}

static void
pp_minib_show(Dock *dock) 
{
  Pinnipede *pp = dock->pinnipede;
  pp->use_minibar = 1;
  pp->minib_pressed = -1;
}

void 
pp_minib_hide(Dock *dock) 
{
  Pinnipede *pp = dock->pinnipede;
  pp->use_minibar = 0;
}


/*
void pp_draw_ua_symbol(Dock *dock, DLFP_tribune *trib, Drawable d, PostWord *pw, int y0, int y1)
{
  //  Pinnipede *pp = dock->pinnipede;
  DLFP_trib_load_rule *tatouage;
  unsigned long pixel;
  int symb, i,j;
  tribune_msg_info *mi;

  
  if (pw == NULL || pw->ligne != 1) {
    XSetForeground(dock->display, dock->NormalGC, BlackPixel(dock->display, dock->screennum));
    XFillRectangle(dock->display, d, dock->NormalGC, 0, y0, 10, y1-y0+1);
  } else {
    XSetForeground(dock->display, dock->NormalGC, BlackPixel(dock->display, dock->screennum));
    XFillRectangle(dock->display, d, dock->NormalGC, 0, y0, 3, y1-y0+1);
    XFillRectangle(dock->display, d, dock->NormalGC, 8, y0, 2, y1-y0+1);
    XFillRectangle(dock->display, d, dock->NormalGC, 3, y0, 5, y1-7-y0+1);
    XFillRectangle(dock->display, d, dock->NormalGC, 3, y1-2, 5, 3);

    mi = tribune_find_id(trib, pw->parent->id);
    symb = 1;
    if (mi) {
      tatouage = mi->tatouage;
      if (tatouage) {
	symb = tatouage->symb;
      }
    }
    pixel = pixel_color(dock, tatouage->R, tatouage->G, tatouage->B);
  
    for (i=0; i < 5; i++) {
      for (j=0; j < 5; j++) {
	if (symboles[symb].s[i][j] == ' ') {
	  XSetForeground(dock->display, dock->NormalGC, BlackPixel(dock->display, dock->screennum));
	} else {
	  XSetForeground(dock->display, dock->NormalGC, pixel);
	}
	XDrawPoint(dock->display, d, dock->NormalGC, 3+i, y1-7+j);
      }
    }
  }
  }*/

/* redessine la fortune */
void
pp_refresh_fortune(Dock *dock, Drawable d)
{
  Pinnipede *pp = dock->pinnipede;
  Pixmap fpix;

  if (pp->fortune_h > 0) {
    int x;
    assert(pp->fortune_h < 10000); /* tout de meme */

    fpix = XCreatePixmap(dock->display, pp->win, pp->win_width, pp->fortune_h, 
			 DefaultDepth(dock->display,dock->screennum));
    XSetForeground(dock->display, dock->NormalGC, IRGB2PIXEL(Prefs.pp_fortune_bgcolor));
    XFillRectangle(dock->display, fpix, dock->NormalGC, 0, 0, pp->win_width, pp->fortune_h);
    XSetForeground(dock->display, dock->NormalGC, RGB2PIXEL(128,128,128));
    XDrawLine(dock->display, fpix, dock->NormalGC, 0, pp->fortune_h-1, pp->win_width, pp->fortune_h-1);

    assert(!picohtml_isempty(pp->ph_fortune));

    x = (pp->win_width - pp->fortune_w)/2;
    picohtml_render(dock, pp->ph_fortune, fpix, dock->NormalGC, x, 0);
    XCopyArea(dock->display, fpix, d, dock->NormalGC, 0, 0, pp->win_width, pp->fortune_h, 0, 0);
    XFreePixmap(dock->display, fpix);
  } else {
  /* nettoyage ligne du haut */
    assert(LINEY0(0)>0);
    XClearArea(dock->display, pp->win, 0, 0, pp->win_width, LINEY0(0), False);
  }
}

/* dessine une ligne */
PostWord *
pp_draw_line(Dock *dock, Pixmap lpix, PostWord *pw, unsigned long bgpixel)
{
  Pinnipede *pp = dock->pinnipede;
  int pl;
  int old_pos;
  unsigned long pixel, old_pixel;
  int y;

  XSetForeground(dock->display, dock->NormalGC, bgpixel);
  XFillRectangle(dock->display, lpix, dock->NormalGC, 0, 0, pp->win_width, pp->fn_h);
  XSetBackground(dock->display, dock->NormalGC, bgpixel);

  old_pixel = pp->win_bgpixel;
  pixel = 0L;
  y = pp->fn_base->ascent;
  old_pos = 0;
  if (pw) {
    XFontStruct *fn;
    XFontStruct *prev_font;
    
    prev_font = NULL;
    pl = pw->ligne;
    
    /* affichage du petit crochet sur la gauche de certains messages */
    if (pw->parent->is_my_message || pw->parent->is_answer_to_me || pw->parent->is_hilight_key) {
      PostWord *pw2;
      int first_line, last_line;
      int do_hilight = 1;
      if (pw == pw->parent->first) { first_line = 1; } else first_line = 0;
      pw2 = pw;
      while (pw2 && pw2->ligne == pl) pw2 = pw2->next;
      if (pw2 == NULL) last_line = 1; else last_line = 0;

      
      if (pw->parent->is_my_message && pp->hilight_my_message_mode) { 
	pixel = pp->hilight_my_msg_pixel; 
      } else if (pw->parent->is_answer_to_me && pp->hilight_answer_to_me_mode) { 
	pixel = pp->hilight_answer_my_msg_pixel;
      } else if (pw->parent->is_hilight_key && pp->hilight_key_mode) { 
	pixel = pp->hilight_keyword_pixel;
      } else do_hilight = 0;
      
      if (do_hilight) {
	XSetForeground(dock->display, dock->NormalGC, pixel);
	XFillRectangle(dock->display, lpix, dock->NormalGC, 0, 0, 3, pp->fn_h);
	XDrawLine(dock->display, lpix, dock->NormalGC, 2,first_line,  2, pp->fn_h-1-last_line);
	if (first_line) XDrawLine(dock->display, lpix, dock->NormalGC, 3,0,5, 0);
	if (last_line) XDrawLine(dock->display, lpix, dock->NormalGC, 3,pp->fn_h-1,5, pp->fn_h-1);

	if (first_line) {
	  pw2 = pw;
	  while (pw2 && (pw2->attr & PWATTR_TSTAMP) == 0) pw2 = pw2->next;
	  if (pw2) {
	    int x0,width;
	    x0 = pw2->xpos-2; width = pw2->xwidth+3;
	    XDrawLine(dock->display, lpix, dock->NormalGC, 6, 0, x0, 0);
	    XDrawRectangle(dock->display, lpix, dock->NormalGC, x0,0,width,pp->fn_h-1);
	  }
	}

	old_pixel = pixel;
      }
    }

    /* la boucle sur tous les mots */
    while (pw && pw->ligne == pl) {
	
      if (pw->attr & PWATTR_TSTAMP) {
	pixel = pp->timestamp_pixel;
//	if (pw->parent->is_my_message) { pixel = IRGB2PIXEL(0x000080); }
      } else if (pw->attr & (PWATTR_LNK|PWATTR_REF)) {
	pixel = pp->lnk_pixel;
      } else if (pw->attr & PWATTR_NICK) {
	pixel = pp->nick_pixel;
      } else if (pw->attr & PWATTR_LOGIN) {
	pixel = pp->login_pixel;
      } else if (pw->attr & PWATTR_TROLLSCORE) {
	pixel = pp->trollscore_pixel;
      } else {
	pixel = pp->txt_pixel;
      }
      if (pixel != old_pixel) {
	XSetForeground(dock->display, dock->NormalGC, pixel);
	old_pixel = pixel;
      }
      fn = pv_get_font(pp, pw->attr);
      if (fn != prev_font) {
	XSetFont(dock->display, dock->NormalGC, fn->fid);
	prev_font = fn;
      }

      XDrawString(dock->display, lpix, dock->NormalGC, pw->xpos, y, pw->w, strlen(pw->w));

      if (pw->attr & (PWATTR_U | PWATTR_TMP_EMPH)) {
	XDrawLine(dock->display, lpix, dock->NormalGC, pw->xpos, y+1, pw->xpos+pw->xwidth-1, y+1);
      }
      if (pw->attr & (PWATTR_TMP_EMPH)) {
	if (pw->xwidth > 2) {
	  XDrawLine(dock->display, lpix, dock->NormalGC, pw->xpos+1, y+2, pw->xpos+pw->xwidth-2, y+2);
	}
      }

      if (pw->attr & PWATTR_S) {	  
	pixel = pp->strike_pixel; XSetForeground(dock->display, dock->NormalGC, pixel); old_pixel = pixel;
	XDrawLine(dock->display, lpix, dock->NormalGC, pw->xpos, y - fn->ascent + 2, pw->xpos+pw->xwidth-1, y+fn->descent - 2);
	XDrawLine(dock->display, lpix, dock->NormalGC, pw->xpos, y + fn->descent - 2, pw->xpos+pw->xwidth-1, y-fn->ascent + 2);
      }
      pw->attr &= ~PWATTR_TMP_EMPH;
      old_pos = pw->xpos + pw->xwidth;
      pw = pw->next;
    }
  }


  return pw;
}

/* 
   pw_ref: si non null, c'est que le pointeur survole une r�f�rence (c'est a d qu'il
   est utilise quand on survole une reference � un post precedent, 
   ou bien quand on survole un timestamp)
   
   cette fonction est malheureusement devenue un vrai sac de noeud
*/
void
pp_refresh(Dock *dock, DLFP_tribune *trib, Drawable d, PostWord *pw_ref)
{
  Pinnipede *pp = dock->pinnipede;
  int l;
  tribune_msg_info *ref_mi, *caller_mi;
  unsigned char ref_comment[200];
  int ref_in_window = 0; /* mis a 1 si le message soulign� par pw_ref est affich� parmi
			    les autres messages. sinon, on l'affiche en haut, dans une petite fenetre */

#define MAXANTIREF 60
  int anti_ref_id[MAXANTIREF];
  /* les antireferences : si on survolle un timestamp, on emphasize les commentaires qui
     lui font reference */
  int nb_anti_ref;
  int ref_num; /* utilise pour les ref pr�cises dans les post multiples (ie qui ont le meme timestamp) 
		  par defaut, vaut -1 (cad d�sactiv�)
		*/

  ref_num = -1;
  ref_comment[0] = 0;

  pp_refresh_fortune(dock, d);


  /* effacage des bandes blanches */
  {
    int y,h;
    y = LINEY0(pp->nb_lignes);
    h = (pp->win_height - y) - (pp->use_minibar ? (MINIB_H) : 0);
    if (h>0)
      XClearArea(dock->display, pp->win, 0, y, pp->win_width, h, False);
  }
  if (LINEY0(0) > pp->fortune_h) {
    XClearArea(dock->display, pp->win, 0, pp->fortune_h, pp->win_width, LINEY0(0)-pp->fortune_h, False);
  }

  caller_mi = NULL;
  ref_mi = NULL;
  nb_anti_ref = 0;
  
  /* premier cas: on survole une reference */
  if (pw_ref && (pw_ref->attr & PWATTR_REF)) {
    int bidon;

    ref_mi = check_for_horloge_ref(trib, pw_ref->parent->id, pw_ref->w, ref_comment, 200, &bidon, &ref_num); assert(bidon);
    if (ref_mi) { 
      tribune_msg_info *mi;

      /* on verifie que la ref apparait *entierement* dans la fenetre 
	 -> on boucle pour les situation ou il y a plusieurs messages qui ont le meme timestamp 
       */
      mi = ref_mi;
      while (mi && mi->timestamp == ref_mi->timestamp) {
	ref_in_window = 0;
	for (l=0; l < pp->nb_lignes; l++) {
	  if (pp->lignes[l]) {
	    if (pp->lignes[l]->parent->id == ref_mi->id) {
	      if (ref_in_window == 0) {
		/* sale ruse... si toutes les lignes sont l�, a la fin du FOR on obtient ref_in_window = 1 */
		ref_in_window = pp->lignes[l]->parent->nblig;
	      } else {
		ref_in_window--;
	      }
	    }
	  }
	}
	if (ref_in_window != 1) {
	  ref_in_window = 0;
	  break;
	}
	get_next_id(trib, mi->id, &mi, NULL);
	if (ref_num != -1) break; /* si c'est pas une ref � un multipost sans pr�cision, break (chuis pas clair) */
      }
	
      /* et maintenant on detecte toutes les autres references vers ce message pour les afficher
	 temporairement en gras (�a c vraiment pour faire le cakos)*/
      for (l=0; l < pp->nb_lignes; l++) {
	if (pp->lignes[l]) {
	  tribune_msg_info *ref2_mi;
	  int pl;
	  PostWord *pw;
	  pw = pp->lignes[l];
	  pl = pw->ligne;
	  while (pw && pl == pw->ligne) {
	    if (pw->attr & PWATTR_REF) {
	      int bidon;
	      ref2_mi = check_for_horloge_ref(trib, pw->parent->id, pw->w, NULL, 0, &bidon, NULL); assert(bidon);
	      if (ref2_mi && ref2_mi->timestamp == ref_mi->timestamp) { /* test sur timestamp pour les situation o� +sieurs msg ont le m�me */
		pw->attr |= PWATTR_TMP_EMPH;
	      }
	    }
	    pw = pw->next;
	  }
	}	
      }
    }
  } else if (pw_ref && (pw_ref->attr & PWATTR_TSTAMP)) {
    /* deuxieme cas, on survolle un TIMESTAMP -> active les antiref */
    for (l=0; l < pp->nb_lignes; l++) {
      if (pp->lignes[l]) {
	tribune_msg_info *ref2_mi;
	int pl;
	PostWord *pw;
	pw = pp->lignes[l];
	pl = pw->ligne;
	/* parcourt chaque ligne */
	while (pw && pl == pw->ligne) {
	  if (pw->attr & PWATTR_REF) {
	    int bidon;
	    ref2_mi = check_for_horloge_ref(trib, pw->parent->id, pw->w, NULL, 0, &bidon, NULL); assert(bidon);
	    if (ref2_mi && ref2_mi->timestamp == pw_ref->parent->tstamp) { /* test sur timestamp pour les situation o� +sieurs msg ont le m�me */
	      pw->attr |= PWATTR_TMP_EMPH;
	      /*	      if (nb_anti_ref < MAXANTIREF) {
		anti_ref_id[nb_anti_ref++] = pw->parent->id;
		}*/
	    }
	  }
	  pw = pw->next;
	}
      }
    }
  }



  /* 
     affichage du contenu de la tribune 
  */

  for (l=0; l < pp->nb_lignes; l++) {
    PostWord *pw;
    unsigned long bgpixel;

    pw = pp->lignes[l];

    bgpixel = pp->win_bgpixel;
    if (pw) {
      int i;

      /* if (pw->parent->is_answer_to_me) bgpixel = pp->answer_my_msg_bgpixel; */
      /*      if (pw->parent->is_my_message) bgpixel = pp->my_msg_bgpixel;*/

      if (ref_mi) {
	if (ref_num == -1) {
	  if (pw->parent->tstamp == ref_mi->timestamp && ref_in_window) {
	    bgpixel = pp->emph_pixel; 
	  }
	} else {
	  if (pw->parent->id == ref_mi->id && ref_in_window) {
	    bgpixel = pp->emph_pixel; 
	  }
	}
      }
      for (i = 0; i < nb_anti_ref; i++) {
	if (anti_ref_id[i] == pw->parent->id) {
	  bgpixel = pp->emph_pixel; 
	}
      }
    }

    pp_draw_line(dock, pp->lpix, pw, bgpixel);

    XCopyArea(dock->display, pp->lpix, d, dock->NormalGC, 0, 0, pp->win_width, pp->fn_h, 0, LINEY0(l));
  }

  if (pw_ref && ref_in_window == 0) {
    int y;
    tribune_msg_info *mi;
    
    y = 3;
    /* affichage de la reference tout en haut du pinnipede */

    /* 
       on boucle pour les situation ou il y a plusieurs messages qui ont le meme timestamp 
    */
    mi = ref_mi;
    while (mi && mi->timestamp == ref_mi->timestamp) {
      PostVisual *pv;
      pv = pp_pv_add(pp, trib, mi->id);
      if (pv) {
	PostWord *pw = pv->first;
	while (pw) {
	  pw = pp_draw_line(dock, pp->lpix, pw, pp->emph_pixel); //WhitePixel(dock->display, dock->screennum));
	  XCopyArea(dock->display, pp->lpix, d, dock->NormalGC, 0, 0, pp->win_width, pp->fn_h, 0, y);
	  y += pp->fn_h;
	}
      }
      get_next_id(trib, mi->id, &mi, NULL);
      if (ref_num != -1) break;
    }

    /* affichage du commentaire (optionnel) */
    if (ref_mi || strlen(ref_comment)) {
      if (strlen(ref_comment)) {
	XSetForeground(dock->display, dock->NormalGC, pp->emph_pixel); //WhitePixel(dock->display, dock->screennum));
	XFillRectangle(dock->display, pp->lpix, dock->NormalGC, 0, 0, pp->win_width, pp->fn_h);
	XSetBackground(dock->display, dock->NormalGC, pp->emph_pixel); //WhitePixel(dock->display, dock->screennum));
	XSetFont(dock->display, dock->NormalGC, pp->fn_it->fid);
	XSetForeground(dock->display, dock->NormalGC, pp->timestamp_pixel);	
	XDrawString(dock->display, pp->lpix, dock->NormalGC, 5, pp->fn_base->ascent, ref_comment, strlen(ref_comment));
	XCopyArea(dock->display, pp->lpix, d, dock->NormalGC, 0, 0, pp->win_width, pp->fn_h, 0, y);
	y += pp->fn_h;
      }
      XSetForeground(dock->display, dock->NormalGC, pp->emph_pixel); //WhitePixel(dock->display, dock->screennum));
      XFillRectangle(dock->display, pp->win, dock->NormalGC, 0, 0, pp->win_width, 3);
      XFillRectangle(dock->display, pp->win, dock->NormalGC, 0, y, pp->win_width, 3);
      y+=3;
      XSetForeground(dock->display, dock->NormalGC, BlackPixel(dock->display, dock->screennum));
      XDrawLine(dock->display, pp->win, dock->NormalGC, 0, y, pp->win_width, y);
    }
  }

  if (pp->use_minibar) {
    pp_minib_refresh(dock);
  }
}

void
pp_check_tribune_updated(Dock *dock, DLFP_tribune *trib)
{
  Pinnipede *pp = dock->pinnipede;
  if (pp && pp->mapped && flag_updating_tribune == 0) {
    /* test si on scrolle qutomatiquement pour afficher le nouveau message */
    //    if (trib->last_post_id != pp->last_post_id && pp->last_post_id == pp->id_base && pp->decal_base == 0) {
    if (pp->flag_tribune_updated) {
      if (trib->last_post_id != pp->last_post_id && pp->last_post_id == pp->id_base && pp->decal_base == 0) {
	pp_update_content(dock, trib, trib->last_post_id, 0, 0);
      } else {
	pp_update_content(dock, trib, pp->id_base, pp->decal_base, 0);
      }
      pp_refresh(dock, trib, pp->win, NULL);
      pp->flag_tribune_updated = 0;
    }
  }
}


static void
pp_load_fonts(Pinnipede *pp, Display *display, char *fn_family, int fn_size)
{
  char base_name[512];
  char ital_name[512];
  char bold_name[512];
  char itbd_name[512];

  /* police de base ... si on ne la trouve pas, c'est une erreur fatale */
  snprintf(base_name, 512, "-*-%s-medium-r-*-*-%d-*-*-*-*-*-%s", fn_family, fn_size, Prefs.font_encoding);
  pp->fn_base = XLoadQueryFont(display, base_name);
  if (!pp->fn_base) {
    fprintf(stderr, "XLoadQueryFont: failed loading font '%s'\n", base_name);
    fprintf(stderr, "Choisissez une autre police\n");
    exit(-1);
  }

  /* police italique -> on cherche d'abord la police oblique */
  snprintf(ital_name, 512, "-*-%s-medium-o-*-*-%d-*-*-*-*-*-%s", fn_family, fn_size, Prefs.font_encoding);
  pp->fn_it = XLoadQueryFont(display, ital_name);
  if (!pp->fn_it) {
    /* puis la police italique */
    BLAHBLAH(1, fprintf(stderr, "police oblique '%s' non trouvee -> on cherche la police italique\n", ital_name));
    snprintf(ital_name, 512, "-*-%s-medium-i-*-*-%d-*-*-*-*-*-%s", fn_family, fn_size, Prefs.font_encoding);
    pp->fn_it = XLoadQueryFont(display, ital_name);
    if (!pp->fn_it) {
      myfprintf(stderr, "%<RED WARNING>: erreur lors de la recherche de la fonte italique: '%s'\n", ital_name);
      myfprintf(stderr, "on va utiliser la fonte de base\n");

      /* pas de copie de pointer pour pas poser de pbs dans picohtml_destroy */
      pp->fn_it = XLoadQueryFont(display, base_name); assert(pp->fn_it);
    }
  }

  /* police bold */
  snprintf(bold_name, 512, "-*-%s-bold-r-*-*-%d-*-*-*-*-*-%s", fn_family, fn_size, Prefs.font_encoding);
  pp->fn_bd = XLoadQueryFont(display, bold_name);
  if (!pp->fn_bd) {
    myfprintf(stderr, "%<RED WARNING>: erreur lors de la recherche de la fonte bold: '%s'\n", bold_name);
    myfprintf(stderr, "on va utiliser la fonte de base\n");
    pp->fn_bd = XLoadQueryFont(display, base_name); assert(pp->fn_bd);
  }

  /* police bold oblique */
  snprintf(itbd_name, 512, "-*-%s-bold-o-*-*-%d-*-*-*-*-*-%s", fn_family, fn_size, Prefs.font_encoding);
  pp->fn_itbd = XLoadQueryFont(display, itbd_name);
  if (!pp->fn_itbd) {
    /* puis la police bold italique */
    BLAHBLAH(1, fprintf(stderr, "police bold oblique '%s' non trouvee -> on cherche la police bold italique\n", itbd_name));
    snprintf(itbd_name, 512, "-*-%s-bold-i-*-*-%d-*-*-*-*-*-%s", fn_family, fn_size, Prefs.font_encoding);
    pp->fn_itbd = XLoadQueryFont(display, itbd_name);
    if (!pp->fn_itbd) {
      myfprintf(stderr, "%<RED WARNING>: erreur lors de la recherche de la fonte italique: '%s'\n", itbd_name);
      myfprintf(stderr, "on va utiliser la fonte de base\n");

      /* pas de copie de pointer pour pas poser de pbs dans picohtml_destroy */
      pp->fn_itbd = XLoadQueryFont(display, base_name); assert(pp->fn_itbd);
    }
  }
  pp->fn_h = pp->fn_base->ascent + pp->fn_base->descent+1;
}

#define C2RGB(c) ((c) >> 16 & 0xff), ((c) >> 8 & 0xff), ((c) & 0xff)

/* initialisation */
void
pp_build(Dock *dock) 
{
  Pinnipede *pp;

  ALLOC_OBJ(pp, Pinnipede);
  dock->pinnipede = pp;


  pp->mapped = 0;
  pp->win_bgpixel = IRGB2PIXEL(Prefs.pp_bgcolor);
  pp->timestamp_pixel = IRGB2PIXEL(Prefs.pp_tstamp_color);
  pp->lnk_pixel = IRGB2PIXEL(Prefs.pp_url_color);
  pp->strike_pixel = RGB2PIXEL(128,0,0);
  pp->txt_pixel = IRGB2PIXEL(Prefs.pp_fgcolor);
  pp->popup_fgpixel = RGB2PIXEL(0, 0, 0);
  pp->popup_bgpixel = RGB2PIXEL(228, 228, 192);
  pp->nick_pixel = IRGB2PIXEL(Prefs.pp_useragent_color);
  pp->login_pixel = IRGB2PIXEL(0xff0000);
  pp->minib_pixel = IRGB2PIXEL(Prefs.pp_button_color);
  pp->emph_pixel = IRGB2PIXEL(Prefs.pp_emph_color);
  pp->trollscore_pixel = IRGB2PIXEL(Prefs.pp_trollscore_color);
  pp->hilight_my_msg_pixel = IRGB2PIXEL(Prefs.pp_my_msg_color);
  pp->hilight_answer_my_msg_pixel = IRGB2PIXEL(Prefs.pp_answer_my_msg_color);
  pp->hilight_keyword_pixel = IRGB2PIXEL(Prefs.pp_keyword_color);

  pp->id_base = -1; pp->decal_base = 0;

  pp->win_width = Prefs.pp_width;
  pp->win_height = Prefs.pp_height;
  pp->win_xpos = Prefs.pp_xpos;
  pp->win_ypos = Prefs.pp_ypos;
  pp->lignes = NULL;
  pp->nb_lignes = 0;
  //  pp->html_mode = Prefs.pp_html_mode;
  pp->nick_mode = Prefs.pp_nick_mode;
  pp->nosec_mode = Prefs.pp_nosec_mode;
  pp->trollscore_mode = Prefs.pp_trollscore_mode;
  pp->hilight_my_message_mode = 1;
  pp->hilight_answer_to_me_mode = 1;
  pp->hilight_key_mode = 1;

  pp->filter.filter_mode = 0;
  pp->filter.filter_name = NULL;
  pp->filter.ua = NULL;
  pp->filter.login = NULL;
  pp->filter.word = NULL;
  pp->filter.nid = 0; pp->filter.id = NULL;

  pp->fortune_mode = Prefs.pp_fortune_mode; 
  pp->fortune_h = 0;
  pp->ph_fortune = picohtml_create(dock, Prefs.pp_fortune_fn_family, Prefs.pp_fortune_fn_size, 0);
  picohtml_set_default_pixel_color(pp->ph_fortune, IRGB2PIXEL(Prefs.pp_fortune_fgcolor));
  picohtml_set_parag_indent(pp->ph_fortune, 5);
  picohtml_set_parag_skip(pp->ph_fortune, 1.0);
  picohtml_set_line_skip(pp->ph_fortune, 1.0);


  pp->pv = NULL;
  pp->survol_hash = 0;
  assert(Prefs.pp_fn_family);
  pp_load_fonts(pp, dock->display, Prefs.pp_fn_family, Prefs.pp_fn_size);
  pp->use_minibar = Prefs.pp_minibar_on; pp->minib_pressed = -1;

  pp->fn_minib = dock->fixed_font;

  pp->flag_tribune_updated = 0;
  {
    char s_xpm_bgcolor[30];
    /* on remplace la ligne de la couleur transparente par notre couleur de fond,
     c une ruse de sioux */
    snprintf(s_xpm_bgcolor, 30, " \tc #%06X", Prefs.pp_button_color);

    pp_minib_xpm[1] = s_xpm_bgcolor;

    assert(XpmCreatePixmapFromData(dock->display, dock->rootwin, 
				   pp_minib_xpm, &pp->minipix, NULL, NULL) == XpmSuccess);    
  }
}


/*
  un petit mot: j'ai enfin compris comment faire apparaitre cette fenetre
   ou je veux: il suffit de donner la position dans xcreatewindow
   et d'activer USPosition dans les WM_HINTS (MAIS, les valeurs indiqu�es dans
   les champs x et y n'ont aucune influence)
*/
void
pp_show(Dock *dock, DLFP_tribune *trib)
{
  XSetWindowAttributes wa;
  Pinnipede *pp = dock->pinnipede;
  int xpos, ypos;

  if (pp->win_xpos == -10000 && pp->win_ypos == -10000) {
    xpos = 0; ypos = 0; /* �a n'a d'effet que sur certain windowmanagers rustiques (genre pwm) */
  } else {
    xpos = pp->win_xpos;
    ypos = pp->win_ypos;
  }

  pp->win = XCreateSimpleWindow (dock->display, dock->rootwin, 
				 xpos, ypos, pp->win_width,pp->win_height, 0,
				 WhitePixel(dock->display, dock->screennum),
				 pp->win_bgpixel);
  wa.event_mask =
    ButtonPressMask | 
    ButtonReleaseMask | 
    ExposureMask | 
    PointerMotionMask | 
    StructureNotifyMask |
    EnterWindowMask | 
    //    ResizeRedirectMask |
    
    LeaveWindowMask;
  wa.override_redirect = False ;
  //wa.override_redirect = True ;
  XChangeWindowAttributes (dock->display, pp->win,
			   //CWBackPixmap | 
			   CWEventMask | CWOverrideRedirect, &wa);

  {
    XTextProperty window_title_property;
    char* window_title = "pinnipede teletype";
    XSizeHints* win_size_hints;
    int rc;
    //    XWMHints* win_hints;
    XClassHint *class_hint;
    char s[512];

    rc = XStringListToTextProperty(&window_title,1, &window_title_property); assert(rc);
    XSetWMName(dock->display, pp->win, &window_title_property);
    win_size_hints= XAllocSizeHints(); assert(win_size_hints);

    /* au premier lancement, la pos n'est pas connue (sauf si specifee
       dans les options ) */
    if (pp->win_xpos == -10000 && pp->win_ypos == -10000) {
      win_size_hints->flags = PSize | PMinSize;
    } else {
      win_size_hints->flags = USPosition | PSize | PMinSize;
    }
    win_size_hints->x = xpos; 
    win_size_hints->y = ypos;
    win_size_hints->min_width = 200; //MINIB_W+20;
    win_size_hints->min_height = 80;
    win_size_hints->base_width = 300;
    win_size_hints->base_height = 455;
    XSetWMNormalHints(dock->display, pp->win, win_size_hints);
    XFree(win_size_hints);

    /*   win_hints = XAllocWMHints(); assert(win_hints);
    win_hints->icon_window = dock->iconwin;
    win_hints->flags = IconWindowHint;
    XSetWMHints(dock->display, pp->win, win_hints);
    XFree(win_hints);*/

    class_hint = XAllocClassHint();
    class_hint->res_name = "pinnipede_teletype";
    sprintf(s, "wmcoincoin_%s", Prefs.site_root);
    class_hint->res_class = s;
    XSetClassHint(dock->display, pp->win, class_hint);
    XFree(class_hint);
  }

  /* pour etre inform� de la fermeture de la fenetre demandee par le windowmanager */
  XSetWMProtocols(dock->display, pp->win, &dock->atom_WM_DELETE_WINDOW, 1);

  pp->lpix = XCreatePixmap(dock->display, pp->win, pp->win_width, MAX(MINIB_H, pp->fn_h), DefaultDepth(dock->display,dock->screennum));

  /*
  {
    XWMHints *mwh = XAllocWMHints();
    mwh->initial_state = WithdrawnState;
    XSetWMHints(dock->display, pp->win, mwh);
    }*/

  XMapRaised(dock->display, pp->win);

  //  XMoveWindow(dock->display, pp->win, 100, 100);
  pp->mapped = 1;
  pp_update_content(dock, trib, trib->last_post_id, 0,0);
  pp->survol_hash = 0;
}

static PostWord *
pp_get_pw_at_xy(Pinnipede *pp,int x, int y)
{
  PostWord *pw;
  int l;
  pw = NULL;
  if (y >= pp->zmsg_y && y < pp->zmsg_y + pp->zmsg_h) {
    for (l=0; l < pp->nb_lignes; l++) {
      pw = pp->lignes[l];
      
      while (pw) {
	if (x >= pw->xpos && x < pw->xpos+pw->xwidth &&
	    y >= LINEY0(l) && y <= LINEY1(l)) 
	  {
	    break;
	  }
	pw = pw->next;
      }
      if (pw) break;
    }
  }
  return pw;
}

/* affiche le texte en haut du pinnipede (dans le style tr�s d�pouill� des useragents) */
static void
pp_popup_show_txt(Dock *dock, unsigned char *txt)
{
  Pinnipede *pp = dock->pinnipede;
  int l,cnt;
  XFontStruct *fn;
  char *s;
  int ry0, ry1;
 
  if (txt == NULL) return;
  if (strlen(txt) == 0) return;

    
  fn = pp->fn_bd;
  XSetFont(dock->display, dock->NormalGC, fn->fid);
  l = 0; s = txt;
  while (*s) {
    cnt = 0;
    while (s[cnt] && s[cnt] != '\n') {
      cnt++;
      if (XTextWidth(fn, s, cnt) > pp->win_width-16) {
	cnt--; break;
      }
    }
    XSetForeground(dock->display, dock->NormalGC, pp->popup_bgpixel);
    ry0 = (l == 0 ? 0 : (l+1)*pp->fn_h - fn->ascent);
    ry1 = (l+1)*pp->fn_h + fn->descent + (s[cnt]==0 ? 6 : 0);
    XFillRectangle(dock->display, pp->win, dock->NormalGC, 0, ry0,
		   pp->win_width, ry1 - ry0+1);
    if (s[cnt]==0) {
      XSetForeground(dock->display, dock->NormalGC, BlackPixel(dock->display, dock->screennum));
      XDrawLine(dock->display, pp->win, dock->NormalGC, 0, ry1, pp->win_width,ry1);
    }
    XSetForeground(dock->display, dock->NormalGC, pp->popup_fgpixel);
    XSetBackground(dock->display, dock->NormalGC, pp->popup_bgpixel);
    XDrawImageString(dock->display, pp->win, dock->NormalGC, 8, (l+1)*pp->fn_h,
		     s, cnt);	    
    s += cnt;
    if (*s == '\n') s++;
    if (*s) l++;
  }
}

/* renvoie le nombre de references vers le message base_mi (sauf ipot) */
static int
pp_count_backrefs(tribune_msg_info *base_mi)
{
  int nb_backrefs = 0;
  tribune_msg_info *mi;
  
  if (base_mi == NULL) return 0;

  /* on parcourt tous les message post�rieurs � base_mi */
  mi = base_mi->next;
  while (mi) {
    int i;
    /* on regarde toutes ses references */
    for (i = 0; i < mi->nb_refs; i++) {
      tribune_msg_info *ref_mi;
      int j;

      /* pour chaque ref, on regarde la liste (generalement de taille 1 ou 0) des messages point�s */
      for (j = 0, ref_mi = mi->refs[i].mi; j < mi->refs[i].nbmi; j++, ref_mi=ref_mi->next) {
	assert(ref_mi);

	/* si on pointe vers le bon */
	if (ref_mi == base_mi) {
	  nb_backrefs++;
	  break; /* si le message contient deux refs vers base_mi, on ne le compte qu'une fois */
	}
      }
    }
    mi = mi->next;
  }
  return nb_backrefs;
}


/* celle la est tordue ...
   il s'agit de verifier si on survole (avec la souris) une info interessante, 
   et d'agir le cas echeant (de maniere un peu d�sordonn�e)
*/
void
pp_check_survol(Dock *dock, DLFP_tribune *trib, int x, int y)
{
  Pinnipede *pp = dock->pinnipede;
  PostWord *pw;
  char survol[1024];
  char *p;
  int survol_hash;
  int is_a_ref = 0;

  pw = pp_get_pw_at_xy(pp,x,y);
  survol[0] = 0;
  if (pw) {
    if (pw->attr_s) { /* pour les [url] */
      strncpy(survol, pw->attr_s, 1024); survol[1023] = 0;
    } else if (pw->attr & PWATTR_TSTAMP) {
      tribune_msg_info *mi;
      char hilight_cause[512];
      int nrep;
      HilightKey *hk;
      mi = tribune_find_id(trib, pw->parent->id);

      hilight_cause[0] = 0;
      if (mi->is_my_message) {
	snprintf(hilight_cause, 512, " [vous avez post� ce message]");
      } else if (mi->is_answer_to_me) {
	snprintf(hilight_cause, 512, " [ce message r�pond � l'un des votres]");
      } else if ((hk = tribune_hilight_key_list_test_mi(mi, trib->hilight_key_list))) {
	snprintf(hilight_cause, 512, " [contient le %s-clef: '%s']", 
		 tribune_hilight_key_list_type_name(hk->type), hk->key);
      }
      nrep = pp_count_backrefs(mi);
      snprintf(survol, 1024, "id=%d ua=%s\n%d r�ponse%s%s", pw->parent->id, (mi ? mi->useragent : ""), 
	       nrep, (nrep > 1) ? "s" : "", hilight_cause);
      is_a_ref = 1;
    } else if (pw->attr & PWATTR_REF) {
      is_a_ref = 1;
    }
  }
  
  survol_hash = 0;
  if (is_a_ref == 0) {
    p = survol;
    while (*p) { survol_hash += (((unsigned)*p) * 253) + 23; p++; }
  } else {
    survol_hash = (int)pw; // pourquoi pas...
  }
  
  //  survol_hash = (int)pw; // ca c'est pas bon, on fait trop de refresh

  /* on evite de reafficher tant qu'on survolle le meme objet (pour eviter le clignotement) */
  if (pp->survol_hash != survol_hash) {
    if (is_a_ref || strlen(survol) == 0) {
      pp_refresh(dock, trib, pp->win, is_a_ref ? pw : NULL);
    }
    pp_popup_show_txt(dock, survol);
    pp->survol_hash = survol_hash;
  }
}

void
pp_unmap(Dock *dock)
{
  Pinnipede *pp = dock->pinnipede;

  /* on sauve la position de la fenetre (en prenant en compte les decorations du WM ) */
  get_window_pos_with_decor(dock->display, pp->win, &pp->win_xpos, &pp->win_ypos);
  //  pp_minib_hide(dock);
  XDestroyWindow(dock->display, pp->win);
  XFreePixmap(dock->display, pp->lpix);
  pp->win = None;
  pp->mapped = 0;
  pp_pv_destroy(pp);
}

int pp_ismapped(Dock *dock) {
  return dock->pinnipede->mapped;
}


/* --------- patch de lordOric, aka "plateau de fruits de mer" ------------*/
/* Scrinchote d'un message */
int 
pp_tribuneshot_save_msg( tribune_msg_info *mi, FILE *file )
{
   char time[10];
   
   assert(file); assert(mi);

   snprintf(time, 10, "%02d:%02d:%02d",mi->hmsf[0], mi->hmsf[1], mi->hmsf[2]);
   
   if ( mi->troll_score )
     fprintf( file, "<tr><td><FONT color=red><b>%d</b></FONT></td>\n",mi->troll_score);
   else
     fprintf( file, "<tr><td></td>\n");
   
   fprintf(file,"<td>%s</td>\n", time);
   if ( mi->login && strlen(mi->login))
     fprintf(file,"<td><FONT color=red><center>%s</center></FONT></td>\n",mi->login);
   else {
     char *p;
     char short_ua[15];
     if (mi->tatouage) {
       p = mi->tatouage->name;
       if (strcmp(mi->tatouage->name, "?") == 0) {
	 make_short_name_from_ua(mi->useragent, short_ua, 15);
	 p = short_ua;
       }
     } else p = "[???]";
     fprintf( file, "<td><FONT color=brown><center>%.12s</center></FONT></td>\n",p);
   }
   fprintf( file,"<td>%s</td></tr>\n", mi->msg);

   return 0;
}


/* Tribuneshot : un chouette plateau de fruits de mer ;) */
int
pp_tribuneshot_kikoooo(Dock *dock, DLFP_tribune *tribune ) 
{
  char *file_name;
  FILE *file;
  time_t time_shot;
  int file_exist;
  tribune_msg_info *msg = tribune->msg;
  
  file_name = str_substitute(Prefs.tribune_scrinechote, "~", getenv("HOME"));
  file = fopen( file_name, "r");
  file_exist = (file!=NULL);
  if ( file_exist ) fclose( file ); 
  
  file = fopen( file_name, "a");
  if ( ! file ) {
    char errmsg[512];
    snprintf(errmsg, 512, "Erreur d'ouverture de %s\n", file_name);
    msgbox_show(dock, errmsg);
    free(file_name);
    return -1;
  }
  
  if ( ! file_exist ) {
    fprintf( file, 
	     "<html><head><title>Archive tribune</title>"
	     "<meta http-equiv=\"Content-Type\" content=\"text/html; charset=iso-8859-15\">"
	     "</head>");
    fprintf( file, "<body><center>");
  }
  
  time( &time_shot );
  fprintf( file, "<br><br> *** Scrinechote - %s *** <br>", ctime( &time_shot ) );
  fprintf( file, "<table>");
  
  while ( msg ) {
    pp_tribuneshot_save_msg(msg, file);
    msg = msg->next;
  }
  
  fprintf( file, "</table><br>\n");
  fclose( file);
  
  {
    char infomsg[512];
    snprintf(infomsg,512,"Scrinechote de la tribune (%s) � %s", file_name, ctime(&time_shot));
    msgbox_show(dock, infomsg);
  }
  free(file_name);
  return 0;
}

/* ------------ fin tribuneshot ------------*/

void
pp_balloon_help(Dock *dock, int x, int y)
{
  Pinnipede *pp = dock->pinnipede;

  //  balloon_test(dock, x, y, pp->win_xpos, pp->win_ypos, 15000,
  //	       0, 0, pp->win_width, pp->win_height,
  balloon_show(dock, pp->win_xpos + x, pp->win_ypos + y, 40, 40, 
	       "<p align=center>bienvenue dans le <b><font color=#008000>Pinnip�de T�l�type</font></b></p>"
	       "Cette fen�tre � �t� sp�cialement con�ue par les plus grands experts pour offrir des conditions de moulage optimales.<br>"
	       "Voici donc un rappel des fonctionnalit�s:<br><br>"
	       "<b>Pour scroller</b>, utiliser la roulette de la souris, ou 'tirer' en cliquant avec le bouton de gauche. "
	       "Le scroll est automatique lors de l'apparition d'un nouveau message.<br><br>"
	       "Pour faire apparaitre/disparaitre <b>la barre de boutons</b>, utilisez le clic droite. Les deux premiers boutons permettent "
	       "de scroller, plus ou moins vite selon que vous cliquez avec le bouton de gauche ou de droite<br><br>"
	       "Les actions disponibles sur l'<b>horloge</b> identifiant un message sont:<br>"
	       "<font color=blue>Click Gauche</font><tab>: ouvre le palmip�de editor, et ins�re une r�f�rence au message<br>"
	       "<font color=blue>Click Milieu</font><tab>: copie le contenu du message dans le clipboard<br>"
	       "<font color=blue>Click Droite</font><tab>: copie le contenu du useragent dans le clipboard<br><br>"
	       "En cliquant sur une <b>[url]</b>, le r�sultat sera:<br>"
	       "<font color=blue>Click Gauche</font><tab>: ouverture de l'url dans le browser externe (si il a �t� "
	       "d�fini dans le fichier ~/.wmcoincoin/options). <b>Attention</b>, m�me si des pr�cautions ont �t� prises "
	       "depuis l'ignominieux wmcoincoin 2.0, ce genre de chose reste g�n�ralement consid�r� comme une faiblesse dans la s�curit�...<br>"
	       "<font color=blue>Click Milieu</font><tab>: ouverture de l'url avec le second browser (option http.browser2)<br>"
	       "<font color=blue>Click Droite</font><tab>: copie l'url dans le clipboard<br><br>"
	       "Quand le pointeur se trouve au-dessus d'une <b>r�f�rence � un post pr�c�dent</b>, celui-ci sera soulign�. Si vous cliquez:<br>"
	       "<font color=blue>Click Gauche</font><tab>: 'aller' au message r�f�renc�<br>"
	       "<font color=blue>Click Droite</font><tab>: copier cette r�f�rence dans le clipboard (d'accord, c'est pas tr�s utile...)<br><br>"

	       "Un filtrage rudimentaire est disponible � l'aide de <font color=blue>Ctrl+left clic</font> sur un mot, login, useragent.. (utiliser le bouton bleu pour l'annuler)<br>"
	       "Vous pouvez prendre un 'shot' de la tribune (aka plateau de fruits de mer), avec <font color=blue>Ctrl+Middle Clic</font><br><br>"
	       
	       "Pour comprendre l'affichage des <b>useragents</b> activ� par le bouton rouge sombre (� quinze pixels sur votre gauche), se reporter au "
	       "fichier <tt>~/.wmcoincoin/useragents</tt><br><br> (hint: il a 5 positions diff�rentes)<br><br>"
	       "Le pinnip�de t�l�type vous souhaite un agr�able moulage.", 500);
}

void
pp_minib_handle_button_release(Dock *dock, DLFP_tribune *trib, XButtonEvent *event)
{
  Pinnipede *pp = dock->pinnipede;
  int pressed, old_pressed;

  old_pressed = pp->minib_pressed;
  pressed = pp_minib_get_button(dock, event->x, event->y);
  pp->minib_pressed = -1;
  //printf("release?: %d\n", pressed);
  if (event->type == ButtonRelease  && pressed >= 0 && pressed == old_pressed) {
    switch (pressed) {
    case -1: break;
    case 0: 
    case 1: 
      {
	int q;
	if (event->button == Button1) {
	  q = 3;
	} else q = 10;
	    if (pressed == 1) q = -q;
	    pp_update_content(dock, trib, pp->id_base, pp->decal_base-q,0);
	    pp_refresh(dock, trib, pp->win, NULL);
      } break;
    case 2:
      {
	pp_update_content(dock, trib, get_next_id(trib, -1, NULL, &pp->filter), 0,0);
	pp_refresh(dock, trib, pp->win, NULL);
      } break;
    case 3:
      {
	pp_update_content(dock, trib, trib->last_post_id, 0,0);
	pp_refresh(dock, trib, pp->win, NULL);
      } break;
    case 4:
      {
	dock->tribune_update_request = 1; pp_minib_refresh(dock);
      } break;
    case 5:
      {
	pp_minib_refresh(dock);
	pp_balloon_help(dock, MINIB_BX(5)-20, MINIB_BY(5)-20);
      } break;
    case 6:
      {
	pp->nick_mode = (pp->nick_mode + 1) % 5;
	pp_pv_destroy(pp);
	pp_update_content(dock, trib, pp->id_base, pp->decal_base,0);
	pp_refresh(dock, trib, pp->win, NULL);	    
      } break;
    case 7:
      {
	pp->nosec_mode = (1-pp->nosec_mode);
	pp_pv_destroy(pp);
	pp_update_content(dock, trib, pp->id_base, pp->decal_base,0);
	pp_refresh(dock, trib, pp->win, NULL);	    
      } break;
    case 10:
      {
	pp->filter.filter_mode = (1-pp->filter.filter_mode);

	if (pp->filter.filter_mode) pp->id_base = -1; /* reset du scroll (necessaire, faut etre que le post 'id_base' soit bien affich� par le filtre) */

	pp_pv_destroy(pp);
	pp_update_content(dock, trib, pp->id_base, pp->decal_base,0);
	pp_refresh(dock, trib, pp->win, NULL);	    	    
      } break;
    case 8:
      {
	if (Prefs.enable_troll_detector) {
	  pp->trollscore_mode = (1-pp->trollscore_mode);
	  pp_pv_destroy(pp);
	  pp_update_content(dock, trib, pp->id_base, pp->decal_base,0);
	  pp_refresh(dock, trib, pp->win, NULL);	    	    
	} else {
	  msgbox_show(dock, "inutile de cliquer sur ce bouton, le troll_detector est d�sactiv� (voir l'option 'tribune.enable_troll_detector')");
	}
      } break;
    case 9:
      {
	if (Prefs.user_cookie || Prefs.force_fortune_retrieval) {
	  pp->fortune_mode = (1-pp->fortune_mode);
	  pp_pv_destroy(pp);
	  pp_update_content(dock, trib, pp->id_base, pp->decal_base,0);
	  pp_refresh(dock, trib, pp->win, NULL);	    	    	
	} else {
	  msgbox_show(dock, "inutile de cliquer sur ce bouton si vous n'avez ni donn� le cookie � wmcoincoin, ni utilise l'option 'http.force_fortune_retrieval'");
	}
      } break;
    default:
      assert(0); // pt1 j'aurais du faire un truc propre sur ces boutons, comme dans editw
    }
  } else pp_minib_refresh(dock);
}

#define THREAD_FILTER_SZ_REALLOC 10

static void
pp_thread_filter_add_refs(DLFP_tribune *trib, struct _PinnipedeFilter *f, tribune_msg_info *base_mi)
{
  int i,j;
  if (base_mi == NULL) return;

  /* on inspecte toutes les references */
  for (i = 0; i < base_mi->nb_refs; i++) {
    tribune_msg_info *mi;
    
    mi = base_mi->refs[i].mi;

    for (j = 0; j < base_mi->refs[i].nbmi; j++) {
      /* realloc la liste si il faut */
      if ((f->nid)%THREAD_FILTER_SZ_REALLOC == 0) {
	f->id = realloc(f->id, (f->nid+THREAD_FILTER_SZ_REALLOC) * sizeof(int)); assert(f->id);
      }
      /* si la ref n'etait pas d�j� dans la liste, on l'ajoute */
      if (pp_thread_filter_find_id(f, mi->id)==0) {
	myprintf("    ref(%d): ajout de %<YEL %d> <-- %<MAG %d>\n", f->nid, mi->id, base_mi->id);
	f->id[f->nid++] = mi->id;
	/* et on recurse ... */
	pp_thread_filter_add_refs(trib, f, mi);
      }
      mi = mi->next;
    }
  }
}

static void
pp_thread_filter_add_backrefs(DLFP_tribune *trib, struct _PinnipedeFilter *f, tribune_msg_info *base_mi)
{
  tribune_msg_info *mi;
  
  if (base_mi == NULL) return;

  /* on parcourt tous les message post�rieurs � base_mi */
  mi = base_mi->next;
  while (mi) {
    int i;
    /* on regarde toutes ses references */
    for (i = 0; i < mi->nb_refs; i++) {
      tribune_msg_info *ref_mi;
      int j;

      /* pour chaque ref, on regarde la liste (generalement de taille 1 ou 0) des messages point�s */
      for (j = 0, ref_mi = mi->refs[i].mi; j < mi->refs[i].nbmi; j++, ref_mi=ref_mi->next) {
	assert(ref_mi);

	/* si on pointe vers le bon */
	if (ref_mi == base_mi) {
	  /* realloc de la liste si necessaire */
	  if ((f->nid)%THREAD_FILTER_SZ_REALLOC == 0) {
	    f->id = realloc(f->id, (f->nid+THREAD_FILTER_SZ_REALLOC)*sizeof(int)); assert(f->id);
	  }
	  
	  /* si le message n'a pas encore ete traite on l'ajoute */
	  if (pp_thread_filter_find_id(f, mi->id)==0) {
	    myprintf("backref(%d): ajout de %<YEL %d> --> %<MAG %d>\n", f->nid, mi->id, base_mi->id);
	    
	    f->id[f->nid++] = mi->id;
	    /* et hop �a recurse un coup */
	    pp_thread_filter_add_backrefs(trib, f, mi);
	  }
	}
      }
    }
    mi = mi->next;
  }
}

void
pp_set_thread_filter(Dock *dock, DLFP_tribune *trib, int base_id)
{
  
  Pinnipede *pp = dock->pinnipede;
  char fname[200];
  tribune_msg_info *mi;

  
  mi = tribune_find_id(trib, base_id);
  if (mi == NULL) return;

  if (mi->hmsf[3] == 0) {
    snprintf(fname, 200, "thread: %02d:%02d", mi->hmsf[0], mi->hmsf[1]);
  } else {
    snprintf(fname, 200, "thread: %02d:%02d:%02d", mi->hmsf[0], mi->hmsf[1], mi->hmsf[2]);
  }


  pp_unset_filter(&pp->filter);
  pp->filter.filter_mode = 1;
  pp->filter.filter_name = strdup(fname);

  pp->filter.nid = 1;
  pp->filter.id = calloc(THREAD_FILTER_SZ_REALLOC, sizeof(int)); assert(pp->filter.id);
  pp->filter.id[0] = base_id;


  pp_thread_filter_add_refs(trib, &pp->filter, mi);
  pp_thread_filter_add_backrefs(trib, &pp->filter, mi);


  BLAHBLAH(2,printf("activation du filtre [%s]\n", pp->filter.filter_name));
  pp_update_content(dock, trib, -1, 0, 0);
  pp_refresh(dock, trib, pp->win, NULL);
}


void
pp_set_login_filter(Dock *dock, DLFP_tribune *trib, char *login)
{
  Pinnipede *pp = dock->pinnipede;
  char fname[200];

  snprintf(fname, 200, "login: <%s>", login);
  pp_unset_filter(&pp->filter);
  pp->filter.filter_mode = 1;
  pp->filter.filter_name = strdup(fname);
  pp->filter.login = strdup(login);

  BLAHBLAH(2,printf("activation du filtre [%s]\n", pp->filter.filter_name));
  pp_update_content(dock, trib, -1, 0, 0);
  pp_refresh(dock, trib, pp->win, NULL);	  
}

void
pp_set_ua_filter(Dock *dock, DLFP_tribune *trib, char *ua)
{
  Pinnipede *pp = dock->pinnipede;
  char fname[200];

  snprintf(fname, 200, "ua: [%.20s]", ua);
  pp_unset_filter(&pp->filter);
  pp->filter.filter_mode = 1;
  pp->filter.filter_name = strdup(fname);
  pp->filter.ua = strdup(ua);

  BLAHBLAH(2,printf("activation du filtre [%s]\n", pp->filter.filter_name));
  pp_update_content(dock, trib, -1, 0, 0);
  pp_refresh(dock, trib, pp->win, NULL);	  
}

void
pp_set_word_filter(Dock *dock, DLFP_tribune *trib, char *word)
{
  Pinnipede *pp = dock->pinnipede;
  char fname[200];

  snprintf(fname, 200, "mot: '%s'", word);
  pp_unset_filter(&pp->filter);
  pp->filter.filter_mode = 1;
  pp->filter.filter_name = strdup(fname);
  pp->filter.word = strdup(word);

  BLAHBLAH(2,printf("activation du filtre [%s]\n", pp->filter.filter_name));
  pp_update_content(dock, trib, -1, 0, 0);
  pp_refresh(dock, trib, pp->win, NULL);	  
}

/* gestion des ctrl+left click sur un mot du pinnipede
   --> permet l'activation du filtre sur le mot */
static void
pp_handle_control_left_clic(Dock *dock, DLFP_tribune *trib, int mx, int my)
{
  Pinnipede *pp = dock->pinnipede;
  PostWord *pw;
  pw = pp_get_pw_at_xy(pp, mx, my);
  if (pw) {
    if (pw->attr & PWATTR_TSTAMP) {
      /* control+clic sur l'horloge -> activation du filtre */
      pp_set_thread_filter(dock, trib, pw->parent->id); 
    } else if (pw->attr & PWATTR_LOGIN) {
      /* control+clic sur un <login> -> filtre ! */
      pp_set_login_filter(dock, trib, pw->w);
    } else if (pw->attr & PWATTR_NICK) {
      /* control+clic sur un useragent raccourci -> filtre ! */
      
      tribune_msg_info *mi;

      mi = tribune_find_id(trib, pw->parent->id);
      if (mi) {
	pp_set_ua_filter(dock, trib, mi->useragent);
      }
    } else if (strlen(pw->w) > 0) {
      /* control+clic sur un mot normal -> filtre ! */
      char *s, *p;
      s = strdup(pw->w);
      p = s + strlen(s) -1;
      while (p > s && !isalnum(*p)) { *p = 0; p--; }
      p = s;
      while (*p && !isalnum(*p)) p++;
      if (strlen(p)) {
	pp_set_word_filter(dock, trib, p);
      } else {
	pp_set_word_filter(dock, trib, pw->w);
      }
      free(s);
    }
  }
}


/* gestion des shift+left click sur un mot du pinnipede
   --> permet l'ajout/enlevement de mots clefs de mise en valeur de post */
static void
pp_handle_shift_left_clic(Dock *dock, DLFP_tribune *trib, int mx, int my)
{
  Pinnipede *pp = dock->pinnipede;
  PostWord *pw;
  pw = pp_get_pw_at_xy(pp, mx, my);
  if (pw) {
    tribune_msg_info *mi;
    mi = tribune_find_id(trib, pw->parent->id);


    if (mi && pw->attr & PWATTR_NICK) {
      tribune_hilight_key_list_swap(trib, mi->useragent, HK_UA);
    } else if (pw->attr & PWATTR_LOGIN) {
      tribune_hilight_key_list_swap(trib, mi->login, HK_LOGIN);
    } else if (pw->attr & PWATTR_TSTAMP) {
      char sid[10];
      snprintf(sid,10,"%d", pw->parent->id);
      tribune_hilight_key_list_swap(trib, sid, HK_ID);
    } else {
      char *s, *p;
      
      /* simplification du mot */
      s = strdup(pw->w);
      p = s + strlen(s) -1;
      while (p > s && !isalnum(*p)) { *p = 0; p--; }
      p = s;
      while (*p && !isalnum(*p)) p++;

      if (strlen(p) == 0) p = pw->w;

      tribune_hilight_key_list_swap(trib, p, HK_WORD);
      free(s);
    }
    pp_pv_destroy(pp); /* force le rafraichissement complet */
    pp_update_content(dock, trib, pp->id_base, pp->decal_base,0);
    pp_refresh(dock, trib, pp->win, NULL);
  }
}

static void
pp_handle_left_clic(Dock *dock, DLFP_tribune *trib, int mx, int my)
{
  Pinnipede *pp = dock->pinnipede;
  PostWord *pw;

  /* affichage/masquage du 'crochet' � gauche des messages mis en valeur */
  if (mx < 5) {
    pw = pp_get_pw_at_xy(pp, 20, my); /* le 20 est une ruse de chacal puant */
    if (pw) {
      int changed = 1;
      if (pw->parent->is_my_message) {
	pp->hilight_my_message_mode = 1-pp->hilight_my_message_mode;
      } else if (pw->parent->is_answer_to_me) {
	pp->hilight_answer_to_me_mode = 1-pp->hilight_answer_to_me_mode;
      } else if (pw->parent->is_hilight_key) {
	tribune_msg_info *mi;
	HilightKey *hk;
	mi = tribune_find_id(trib, pw->parent->id);
	if (mi && (hk = tribune_hilight_key_list_test_mi(mi, trib->hilight_key_list))) {
	  tribune_hilight_key_list_remove(trib, hk->key, hk->type);
	}
      } else changed = 0;
      if (changed) {
	pp_pv_destroy(pp); /* force le rafraichissement complet */
	pp_update_content(dock, trib, pp->id_base, pp->decal_base,0);
	pp_refresh(dock, trib, pp->win, NULL);
      }
    }
  }
  
  pw = pp_get_pw_at_xy(pp, mx, my);
  if (pw) {
    /* clic gauche sur une url , on affiche le truc dans le browser externe numero 1 */
    if (pw->attr & PWATTR_LNK) {
      if (strlen(pw->attr_s)) {
	open_url(pw->attr_s, pp->win_xpos + mx-5, pp->win_ypos+my-25, 1);
      }
    } else if (pw->attr & PWATTR_TSTAMP) {
      /* clic sur l'holorge -> ouverture du palmipede */
      
      char s_ts[11];
      char s_subts[3];
      
      s_subts[0] = s_subts[1] = s_subts[2] = 0;
      switch(pw->parent->sub_tstamp) {
      case -1: break;
      case 0: s_subts[0] = '�'; break;
      case 1: s_subts[0] = '�'; break;
      case 2: s_subts[0] = '�'; break;
      default: s_subts[0] = ':'; s_subts[1] = '1' + pw->parent->sub_tstamp;
      }
	
      snprintf(s_ts, 11, "%s%s", pw->w, s_subts);
	
      if (editw_ismapped(dock->editw) == 0) {
	if (Prefs.user_name) {
	  snprintf(dock->coin_coin_message, MESSAGE_MAX_LEN, "%s %s ",
		   Prefs.user_name, s_ts);
	} else {
	  snprintf(dock->coin_coin_message, MESSAGE_MAX_LEN, "%s ",
		   s_ts);
	}
	//	  strncpy(dock->coin_coin_message, pw->w, MESSAGE_MAX_LEN);
	// strncat(dock->coin_coin_message, " ", MESSAGE_MAX_LEN);
	dock->coin_coin_message[MESSAGE_MAX_LEN-1] = 0;
	editw_show(dock, dock->editw, 0);
	editw_move_end_of_line(dock->editw, 0);
	editw_refresh(dock, dock->editw);
      } else {
	char s[60];
	snprintf(s, 60, "%s ", s_ts);
	editw_insert_string(dock->editw, s);
	editw_refresh(dock, dock->editw);
      }
    } else if (pw->attr & PWATTR_REF) {
      /* clic sur une reference, on va essayer de se d�placer pour afficher la ref en bas du
	 pinnipede */
      tribune_msg_info *mi;
      int bidon;
      
      mi = check_for_horloge_ref(trib, pw->parent->id, pw->w, NULL, 0, &bidon, NULL); assert(bidon);
      if (mi) {
	pp_update_content(dock, trib, mi->id, 0, 0);
	pp_refresh(dock, trib, pp->win, NULL);
      }
    } 
  } /* if (pw) */  
}

/* gestion du relachement du bouton souris (si on n'est pas en train de 'tirer' la fenetre, 
   et si on n'a pas cliqu� sur la barre de petits boutons */
void 
pp_handle_button_release(Dock *dock, DLFP_tribune *trib, XButtonEvent *event)
{
  Pinnipede *pp = dock->pinnipede;
  int mx,my;
  static Time previous_clic = 0;
  static int q = 1;

  mx = event->x; my = event->y;
  /* acceleration de la roulette */
  if (event->time - previous_clic < 50) {
    q = 4;
    if (event->time - previous_clic < 20) {
      q = 10;
    }
  } else q = 2;
  //printf("t=%ld, pt=%ld, q=%d\n", event->time, previous_clic, q);
  previous_clic = event->time;
  
  if (event->button == Button4) {
    /* un coup de roulette */
    pp_update_content(dock, trib, pp->id_base, pp->decal_base-q,0);
    pp_refresh(dock, trib, pp->win, NULL);
    //    printf("scroll up  : id=%d %d\n",pp->id_base, pp->decal_base);
  } else if (event->button == Button5) {
    pp_update_content(dock, trib, pp->id_base, pp->decal_base+q,0);
    pp_refresh(dock, trib, pp->win, NULL);
    //printf("scroll down: id=%d %d\n",pp->id_base, pp->decal_base);
  } else if (event->button == Button1) {
    if (event->state & ShiftMask) {
      pp_handle_shift_left_clic(dock, trib, mx, my);
    } else if (event->state & ControlMask) {
      pp_handle_control_left_clic(dock, trib, mx, my);
    } else {
      pp_handle_left_clic(dock, trib, mx, my);
    }
  } else if (event->button == Button2) {
    if ((event->state & ControlMask)==0) {    
      PostWord *pw;

      pw = pp_get_pw_at_xy(pp, mx, my);
      /* middle clic sur une horloge, on copie le contenu du message dans le clipboard */
      if (pw && (pw->attr & PWATTR_TSTAMP)) {
	tribune_msg_info *mi;
	mi = tribune_find_id(trib, pw->parent->id);
	if (mi) {
	  if (mi->msg && strlen(mi->msg)) {
	    editw_cb_copy(dock, pp->win, mi->msg, strlen(mi->msg));
	  }
	}
      } else if (pw && pw->attr & PWATTR_LNK) {
	/* clic gauche sur une url , on affiche le truc dans le browser externe numero 2 */
	if (strlen(pw->attr_s)) {
	  open_url(pw->attr_s, pp->win_xpos + mx-5, pp->win_ypos+my-25, 2);
	}
      }
    } else {
      /* Ctrl+Middle clic: Et un scrinechote, un ! */
      pp_tribuneshot_kikoooo(dock, trib);
    }
  } else if (event->button == Button3) {
    PostWord *pw;

    pw = pp_get_pw_at_xy(pp, mx, my);

    /* right clic sur une url, on copy l'urk dans le clipboard */
    if (pw && pw->attr & PWATTR_LNK) {
      if (pw->attr_s && strlen(pw->attr_s)) {
	editw_cb_copy(dock, pp->win, pw->attr_s, strlen(pw->attr_s));
      }
    } else if (pw && (pw->attr & PWATTR_TSTAMP)) {
      /* right clic sur une horloge, on copy l'useragent dans le clipboard */
      tribune_msg_info *mi;
      mi = tribune_find_id(trib, pw->parent->id);
      if (mi) {
	if (mi->useragent && strlen(mi->useragent)) {
	  editw_cb_copy(dock, pp->win, mi->useragent, strlen(mi->useragent));	  
	}
      }
    } else if (pw && (pw->attr & PWATTR_REF)) {
      editw_cb_copy(dock, pp->win, pw->w, strlen(pw->w));  
    } else {
      /* right clic ailleurs, affiche la mini barre de boutons */
      if ((pp->use_minibar == 0) || my < pp->win_height - MINIB_H) {
	if (pp->use_minibar == 0) {
	  pp_minib_show(dock);
	} else {
	  pp_minib_hide(dock);
	}
	pp_update_content(dock, trib, pp->id_base, 0,0);
	pp_refresh(dock, trib, pp->win, NULL);	    	    
      }
    }
  }
}

int 
flush_expose(Dock *dock, Window w) {
  XEvent dummy;
  int i=0;

  while (XCheckTypedWindowEvent(dock->display, w, Expose, &dummy))
    i++;
  return i;
}

/*
int
has_pending_expose_events(Display *dpy, Window w) {
}
*/

void
pp_dispatch_event(Dock *dock, DLFP_tribune *trib, XEvent *event)
{
  Pinnipede *pp = dock->pinnipede;
  static int old_mouse_x = -1, old_mouse_y = -1;
  static int dragging = 0;
  static Time time_drag = 0;

  /* le pinnipede ne fait RIEN quand la tribune est en cours de mise � jour ... */
  if (flag_updating_tribune) return;

  switch (event->type) {
  case DestroyNotify: 
    {
      //      printf("destroy?\n");
    } break;
  case ButtonPress:
    {
      old_mouse_x = event->xbutton.x;
      old_mouse_y = event->xbutton.y;
      dragging = 0;
      time_drag = event->xbutton.time;
      if (pp->use_minibar) {
	pp->minib_pressed = pp_minib_get_button(dock, event->xbutton.x, event->xbutton.y);
	//printf("press: %d\n", pp->minib_pressed);
	if (pp->minib_pressed>=0) pp_minib_refresh(dock);
      }
    } break;
  case ButtonRelease:
    {
      old_mouse_x = event->xbutton.x;
      old_mouse_y = event->xbutton.y;
      if (dragging == 0) {
	pp_handle_button_release(dock, trib, &event->xbutton);
	if (pp->use_minibar) {
	  pp_minib_handle_button_release(dock, trib, &event->xbutton);
	}
      }
      dragging = 0;
      time_drag = event->xbutton.time;
    } break;
  case MotionNotify:
    {
      if ((event->xmotion.state & Button1Mask) == 0) {
	pp_check_survol(dock, trib, event->xmotion.x, event->xmotion.y);
	old_mouse_x = event->xmotion.x;
	old_mouse_y = event->xmotion.y;
      } else {
	int decal_y;
	decal_y = event->xmotion.y - old_mouse_y;
	decal_y /= 8;
	if (decal_y && (event->xmotion.time-time_drag)>25) {
	  dragging = 1;
	  //	  printf("move (%ld) %ld!\n", event->xmotion.time, time_drag);
	  pp_update_content(dock, trib, pp->id_base, pp->decal_base-decal_y,0);
	  pp_refresh(dock, trib, pp->win, NULL);
	  old_mouse_x = event->xmotion.x;
	  old_mouse_y = event->xmotion.y;
	  time_drag = event->xmotion.time;
	}
      }
    } break;
  case ConfigureNotify:
    {
      XWindowAttributes wa;
      Window child;


      
      //printf("ConfigureNotify: w<-%d, h<-%d\n", event->xconfigure.width, event->xconfigure.height);

      XGetWindowAttributes(dock->display, pp->win, &wa);
      
      //      printf("expose: width = %d (%d), height=%d (%d)\n", 
      //	     wa.width,pp->win_width,wa.height,pp->win_height);
      XTranslateCoordinates(dock->display, pp->win, dock->rootwin, 
      			    wa.x, wa.y, &pp->win_xpos, &pp->win_ypos, &child);
      
      //printf(" -> xpos=%d, ypos=%d\n", pp->win_xpos,pp->win_ypos);
      if (event->xconfigure.width != pp->win_width || event->xconfigure.height != pp->win_height) {
	pp->win_width = MAX(event->xconfigure.width,80);
	pp->win_height = MAX(event->xconfigure.height,80);
	
	/* eh oui, faut pas oublier �a.... */
	XFreePixmap(dock->display, pp->lpix);
	pp->lpix = XCreatePixmap(dock->display, pp->win, pp->win_width, MAX(MINIB_H, pp->fn_h), DefaultDepth(dock->display,dock->screennum));
	
	
	pp_pv_destroy(pp);
	pp_update_content(dock, trib, pp->id_base, pp->decal_base, 0);
      }
    } break;
  case EnterNotify:
    {
    } break;
  case LeaveNotify:
    {
    } break;
  case Expose:
    {
      static int zx0, zy0, zx1, zy1, zinit = 0;
      int x0, y0, x1, y1;

      x0 = event->xexpose.x;            y0 = event->xexpose.y;
      x1 = x0 + event->xexpose.width-1; y1 = y0 + event->xexpose.height -1;
      if (zinit == 0) {
	zx0 = x0; zy0 = y0; zx1 = x1; zy1 = y1;
      } else {
	zx0 = MIN(zx0, x0); zx1=MAX(zx1, x1);
	zy0 = MIN(zy0, y0); zy1=MAX(zy1, y1);
      }
      /*
	printf("expose_event: x=%d, y=%d, w=%d, h=%d cnt=%d --> z=[%d:%d]x[%d:%d]\n", 
	event->xexpose.x, event->xexpose.y, event->xexpose.width, event->xexpose.height, event->xexpose.count,
	zx0, zx1, zy0, zy1);
      */
      if (event->xexpose.count == 0) {
	pp_refresh(dock, trib, pp->win, NULL);
	flush_expose(dock, pp->win);
      }
    } break;
  case MapNotify:
    {
    } break;
  case SelectionRequest: 
    {
      editw_cb_handle_selectionrequest(dock, &(event->xselectionrequest));
    } break;
  case ClientMessage:
    {
      if (event->xclient.message_type == dock->atom_WM_PROTOCOLS
	  && event->xclient.format == 32 
	  && (Atom)event->xclient.data.l[0] == dock->atom_WM_DELETE_WINDOW) {
	pp_unmap(dock);
      }
    } break;
  }
}

Window pp_get_win(Dock *dock) {
  return dock->pinnipede->win;
}

void
pp_set_tribune_updated(Dock *dock)
{
  dock->pinnipede->flag_tribune_updated = 1;
}
