/*
  rcsid=$Id: pinnipede.c,v 1.31 2002/03/08 23:53:40 pouaite Exp $
  ChangeLog:
  $Log: pinnipede.c,v $
  Revision 1.31  2002/03/08 23:53:40  pouaite
  derniers bugfixes pour la v2.3.6

  Revision 1.30  2002/03/07 18:54:34  pouaite
  raaa .. fix login_color (jjb) patch plop_words (estian) et bidouille pour le chunk encoding (a tester)

  Revision 1.29  2002/03/05 21:04:28  pouaite
  bugfixes suite à l'upgrade de dlfp [et retour au comportement à l'ancienne du clic sur les horloges pour les moules ronchonnes]

  Revision 1.28  2002/03/03 10:10:04  pouaite
  bugfixes divers et variés

  Revision 1.27  2002/03/01 00:27:40  pouaite
  trois fois rien

  Revision 1.26  2002/02/28 01:12:33  pouaite
  scrollcoin dans la fenetre des news

  Revision 1.25  2002/02/27 00:32:19  pouaite
  modifs velues

  Revision 1.24  2002/02/26 22:02:07  pouaite
  bugfix gruikissime pour les pbs de lag sous cygwin

  Revision 1.23  2002/02/26 09:18:23  pouaite
  bugfixes divers

  Revision 1.22  2002/02/25 18:02:35  pouaite
  bugfixes de la journee

  Revision 1.21  2002/02/25 01:36:58  pouaite
  bugfixes pour la compilation

  Revision 1.20  2002/02/24 22:13:57  pouaite
  modifs pour la v2.3.5 (selection, scrollcoin, plopification, bugfixes)

  Revision 1.19  2002/02/03 23:07:32  pouaite
  *** empty log message ***

  Revision 1.18  2002/02/02 23:49:17  pouaite
  plop

  Revision 1.17  2002/01/30 21:03:48  pouaite
  correction du bug du au signe des char, et d'un petit bug dans les reference d'horloges

  Revision 1.16  2002/01/20 20:53:22  pouaite
  bugfix configure.in && http_win.c pour cygwin + 2-3 petis trucs

  Revision 1.15  2002/01/20 02:17:13  pouaite
  modifs d'ordre esthetique (!) sans grand interet

  Revision 1.14  2002/01/19 19:56:09  pouaite
  petits crochets pour la mise en valeur de certains messages (cf changelog)

  Revision 1.13  2002/01/18 00:28:42  pouaite
  le ménage continue + grosses modifs (experimentales pour l'instant)

  Revision 1.12  2002/01/16 00:35:26  pouaite
  debut de detection des reponse à nos message avec des couleurs hideuses et certainement plein de bugs moisis

  Revision 1.11  2002/01/14 23:54:06  pouaite
  reconnaissance des posts effectué par l'utilisateur du canard (à suivre...)

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
  correction (enfin!) du bug d'affichage lorsque plusieurs posts ont le même timestamp

*/

#include <X11/xpm.h>
#include "coincoin.h"
#include "time.h"
#include "scrollcoin.h"

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

/*
#define MINIB_BW 20
#define MINIB_BH 12
#define MINIB_W (NB_MINIB*(MINIB_BW+1))
#define MINIB_X0 (pp->win_width - MINIB_W)
#define MINIB_X1 (MINIB_X0 + MINIB_W - 1)
#define MINIB_Y1 (MINIB_Y0 + MINIB_H - 1)

#define MINIB_BX(i) (pp->win_width+1 - (MINIB_BW+1)*(i+1))
#define MINIB_BY(i) (MINIB_Y0+1)

#define NB_MINIB 11*/


typedef struct _PostVisual PostVisual;
typedef struct _PostWord PostWord;

/* une liste de mots avec leurs attributs */
struct _PostWord {
  unsigned char *w; /* non mallocé, stocke dans la même zone que cette structure */
  unsigned char *attr_s;
  unsigned short attr;
  short xpos, xwidth, ligne;
  struct _PostWord *next;
  struct _PostVisual *parent;
};

/* liste chainée de posts */
struct _PostVisual {
  int id; // message id 
  PostWord *first; /* la liste des mots */
  time_t tstamp;
  signed char sub_tstamp; /* sous numerotation quand plusieurs posts ont le même timestamp */
  int nblig:12; // nombre de lignes necessaire pour afficher ce message
  int ref_cnt:9; // compteur de references

  int is_my_message:1;
  int is_answer_to_me:1;
  int is_hilight_key:1;
  int is_skipped_id:1; /* non nul si le message (id-1) n'existe pas */
  int is_plopified:3;  /* non nul si le message a été plopifié
			  =1, le message apparait en gris, tags html enleves
			  =2, le message est plopifié (mots remplacés par plop, grouik..)
			*/
  struct _PostVisual *next;
};

struct _PinnipedeFilter {
  int filter_mode;
  char *filter_name; /* contient le 'nom' du filtre) 
		      */
  char *ua;
  char *login;
  char *word;
  //  int hms[3]; /* filtre sur les ref au msg posté à l'heure indiquée dans hms */
  int *id; int nid; /* liste des id des messages affichés dans le filtre de threads */
};

typedef struct _PinnipedeLignesSel {
  int x0, x1;
  int trashed;
  PostWord *first_pw;
  PostWord *last_pw;
  int first_pw_pos, last_pw_pos;
} PinnipedeLignesSel;

typedef struct _PPMinib {
#define NB_MINIB 10
#define MINIB_H 10
#define MINIB_FN_W 6
#define MINIB_Y0 (pp->win_height - MINIB_H)
  enum { HELP, SCROLLBAR, REFRESH_TRIBUNE, REFRESH_NEWS, UA, SECOND, TSCORE, FORTUNE, FILTER, PLOPIFY } type;
  int x, y;
  int w, h;
  int clicked;
} PPMinib;

struct _Pinnipede {
  Window win;
  unsigned long win_bgpixel, timestamp_pixel, nick_pixel, login_pixel, 
    emph_pixel, trollscore_pixel, lnk_pixel, txt_pixel, strike_pixel, 
    popup_fgpixel, popup_bgpixel, minib_pixel, minib_dark_pixel, sel_bgpixel,
    hilight_my_msg_pixel,hilight_answer_my_msg_pixel,hilight_keyword_pixel,
    plopify_pixel;
  int mapped;
  int win_width, win_height, win_xpos, win_ypos;

  int zmsg_y, zmsg_h; /* zone d'affichage des messages */
  XFontStruct *fn_base, *fn_it, *fn_bd, *fn_itbd, *fn_minib;
  int fn_h;
  //  Pixmap minipix;

  PostVisual *pv;

  int nb_lignes;
  PostWord **lignes;

  ScrollCoin *sc;

  PinnipedeLignesSel *lignes_sel; /* utilisé uniquement pendant les selections */
  int sel_anchor_x, sel_anchor_y;
  int sel_head_x, sel_head_y;
  int sel_l0, sel_l1;

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
  int use_minibar; /* les miniboutons sont-ils affichés ? */

  PPMinib mb[NB_MINIB];

  int minib_pressed; /* numero du minibouton enfonce, -1 si aucun */
  int fortune_mode;
  int disable_plopify;

  int hilight_my_message_mode;
  int hilight_answer_to_me_mode;
  int hilight_key_mode;

  
  volatile int flag_tribune_updated;
  Pixmap lpix;

  PicoHtml *ph_fortune;
  int fortune_h, fortune_w;

  struct _PinnipedeFilter filter;

  //  int selection_mode; /* non nul quand on est en train de selectionner du texte à copier dans le clipboard (en dragant avec la souris) */
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
    return (str_noaccent_casestr(mi->msg, filter->word) != NULL);
  } else if (filter->id != NULL) {
    return pp_thread_filter_find_id(filter, mi->id);
  } else {
    return 1;
  }
}



/* les deux fonctions suivantes permettent de se balader dans la liste des posts 
 (de maniere bourrine... c pas pour 25000 messages )
*/
static int 
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

static int 
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

static int
count_all_id(DLFP_tribune *trib, struct _PinnipedeFilter *filter) {
  tribune_msg_info *mi;
  int nid;

  mi = trib->msg;
  nid = 0;
  while (mi) {
    if (filter == NULL || filter_msg_info(mi,filter)) {
      nid++;
    }
    mi = mi->next;
  }
  return nid;
}

static int
get_nth_id(DLFP_tribune *trib, struct _PinnipedeFilter *filter, int n) {
  tribune_msg_info *mi, *last_mi;

  mi = trib->msg; last_mi = NULL;
  while (mi && n > 0) {
    if (filter == NULL || filter_msg_info(mi,filter)) {
      n--;
      last_mi = mi;
    }
    mi = mi->next;
  }
  return (last_mi ? last_mi->id : -1);
}

static int
get_id_count(DLFP_tribune *trib, struct _PinnipedeFilter *filter, int id) {
  tribune_msg_info *mi;
  int cnt = 1;

  mi = trib->msg;
  while (mi) {
    if (filter == NULL || filter_msg_info(mi,filter)) {
      if (mi->id == id) {
	return cnt;
      }
      cnt++;
    }
    mi = mi->next;
  }
  return -1;
}

static void
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

/* remplace de maniere +/- aleatoire un mot par plop, grouik etc.. 
   en regardant aussi le mot precedent
*/
static void
plopify_word(unsigned char *s_src, unsigned sz, int bidon)
{
  unsigned char s_simple[sz], s_dest[sz];

  static char *not_plop[] = {"alors", 
			     "amha", 
			     "apres",
			     "aura", 
			     "auras",
			     "auraient",
			     "aurais",
			     "aurait",
			     "aurez",
			     "auront",
			     "aussi",
			     "autant",
			     "avant", 
			     "avec", 
			     "avoir", 
			     "beaucoup",
			     "ben",
			     "bof",
			     "celle",
			     "ces", 
			     "cette", 
			     "ceux",
			     "chez", 
			     "comme",
			     "comment", 
			     "dans",
			     "deja",
			     "depuis", 
			     "des", 
			     "devez",
			     "devons",
			     "devrez",
			     "devrons",
			     "dire",
			     "dois",
			     "doivent",
			     "donc", 
			     "dont",
			     "elle", 
			     "elles",
			     "encore",
			     "entre", 
			     "est", 
			     "etait", 
			     "ete",
			     "etes", 
			     "etre",
			     "fait", 
			     "fallait",
			     "falloir",
			     "fallu",
			     "faudra",
			     "faudrait",
			     "faut",
			     "fera", 
			     "ils", 
			     "la", 
			     "les", 
			     "leur", 
			     "leurs",
			     "lui",
			     "mais",
			     "meme", 
			     "mes", 
			     "mien", 
			     "moi"
			     "moins", 
			     "mon", 
			     "non", 
			     "nos", 
			     "notre", 
			     "nous", 
			     "ont", 
			     "ouais", 
			     "oui", 
			     "par", 
			     "parce", 
			     "pas", 
			     "pas", 
			     "peu", 
			     "peux",
			     "plus", 
			     "pour", 
			     "pourquoi", 
			     "pourtant",
			     "puis", 
			     "quand", 
			     "que", 
			     "quel",
			     "qui",
			     "quoi", 
			     "sais",
			     "sait",
			     "sans", 
			     "sera", 
			     "ses", 
			     "sien",
			     "son", 
			     "sont", 
			     "sur", 
			     "tant", 
			     "tien", 
			     "toujours",
			     "tous", 
			     "tout", 
			     "toutes", 
			     "une", 
			     "vas",
			     "vais", 
			     "vos", 
			     "votre", 
			     "vous", 
			     NULL};

  static unsigned nb_not_plop = 0;
  static unsigned *not_plop_hached = NULL;

  unsigned i;
  unsigned hache_s;
  unsigned src_pos, dest_pos, s_len;

  const char *s_bizarre = "'\",;:/!+=)]@^_\\-|([{}#~?.*$³²¹";

  /* comptage des mots à ne pas plopifier, et 'hachage' des ces mots
     (pour les detecter plus rapidement) 

     ici: initialisation des données (éxécuté au premier appel)
  */
  if (nb_not_plop == 0) {
    while (not_plop[nb_not_plop] != NULL) {
      nb_not_plop++;
    }
    ALLOC_VEC(not_plop_hached, nb_not_plop, int);
    for (i=0; i < nb_not_plop; i++) {
      not_plop_hached[i] = str_hache(not_plop[i], strlen(not_plop[i]));
    }
  }

  /* enleve les accents et vérifie que le mot ne contient que des lettres */
  
  src_pos = 0; dest_pos = 0;
  do {
    int do_plopify;

    do_plopify = 1;

    /* on copie les caractères bizarres qui peuvent preceder le mot */
    while (s_src[src_pos] && strchr(s_bizarre,s_src[src_pos]) && 
	   dest_pos < sz) { 
      s_dest[dest_pos++] = s_src[src_pos++]; 
    }
    if (s_src[src_pos]==0) break;

    /* on copie le mot dans s_simple */
    s_len = 0;
    while (s_src[src_pos+s_len] && !strchr(s_bizarre, s_src[src_pos+s_len])) {
      s_simple[s_len] = s_src[src_pos+s_len]; s_len++;
    }
    s_simple[s_len] = 0;    assert(s_len > 0);

    //    strcpy(s_simple, s);
    str_noaccent_tolower(s_simple);
  
    
    for (i = 0; i < s_len; i++) {
      if (!(s_simple[i] >= 'a' && s_simple[i] <= 'z')) {
	do_plopify = 0; break;
      }
    }

    if (s_len < 3) do_plopify = 0;

    if (do_plopify) {
      hache_s = str_hache(s_simple, s_len);
      for (i=0; i < nb_not_plop; i++) {
	if (hache_s == not_plop_hached[i]) { do_plopify = 0; break; }
      }
    }

    /* j'ai pas été très inspiré en écrivant tout ça, l'est po clair */

    if (do_plopify) {
      /* longeur > 10 => substitution assurée,
         longuer 0 => 1/2 chance sur deux */
      int mod = Prefs.nb_plop_words + (10-MIN(s_len,10))*Prefs.nb_plop_words/10;

      hache_s = (hache_s ^ bidon) % mod;
      if (hache_s >= Prefs.nb_plop_words) do_plopify = 0;
    }
    
    if (do_plopify) {
      i = 0; 
      while (Prefs.plop_words[hache_s][i] && dest_pos < sz) {
	s_dest[dest_pos++] = Prefs.plop_words[hache_s][i++];
      }
      src_pos += s_len;
    } else {
      i = 0;
      while (i < s_len && dest_pos < sz) {
	s_dest[dest_pos++] = s_src[src_pos++]; i++;
      }
    }
  } while (s_src[src_pos]);
  s_dest[dest_pos] = 0;
  strcpy(s_src, s_dest);
}


/* construction d'un postvisual à partir du message 'mi' */
static PostVisual *
pv_tmsgi_parse(DLFP_tribune *trib, tribune_msg_info *mi, int with_seconds, int html_mode, int nick_mode, int troll_mode, int disable_plopify) {
#define PVTP_SZ 512

  PostVisual *pv;
  PostWord *pw, *tmp;
  
  unsigned char s[PVTP_SZ];
  unsigned char attr_s[PVTP_SZ];

  const unsigned char *p, *np;
  unsigned short attr;
  int add_word;
  int has_initial_space; // indique si le prochain mot est collé au precedent

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
  pv->is_skipped_id = tribune_find_id(trib, mi->id-1) ? 0 : 1;
  pv->is_hilight_key = tribune_key_list_test_mi(trib, mi, trib->hilight_key_list) == NULL ? 0 : 1;
  pv->is_plopified = (tribune_key_list_test_mi(trib, mi, trib->plopify_key_list) == NULL) ? 0 : (disable_plopify ? 1 : 2);

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

    /* nouveau (v2.3.5) tous les '<' et '>' provenant d'authentiques tags
       html ont été préfixés d'une tabulation par 'convert_to_ascii', ce qui évite
       de se retrouver à interpreter à moitié un message du style "<a href='prout'>" 
       (par contre, y'a quelques feintes au trollo qui marcheront plus ;)
    */

    if (s[0] == '\t' && html_mode) {
      add_word = 0;
      if (strcasecmp(s, "\t<i\t>")==0) {
	attr |= PWATTR_IT; 
      } else if (strcasecmp(s,"\t</i\t>")==0) {
	attr &= (~PWATTR_IT);
      } else if (strcasecmp(s,"\t<b\t>")==0) {
	attr |= PWATTR_BD; 
      } else if (strcasecmp(s,"\t</b\t>")==0) {
	attr &= (~PWATTR_BD);
      } else if (strcasecmp(s,"\t<u\t>")==0) {
	attr |= PWATTR_U;
      } else if (strcasecmp(s,"\t</u\t>")==0) {
	attr &= (~PWATTR_U);
      } else if (strcasecmp(s,"\t<s\t>")==0) {
	attr |= PWATTR_S; 
      } else if (strcasecmp(s,"\t</s\t>")==0) {
	attr &= (~PWATTR_S);
      } else if (strncasecmp(s,"\t<a href=\"", 10)==0) {
	int i;
	attr |= PWATTR_LNK;
	i = strlen(s)-1; assert(i>0);
	while (s[i] != '\"' && i > 0) i--;
	s[i] = 0;
	strncpy(attr_s, s+10, PVTP_SZ); 
      } else if (strcasecmp(s,"\t</a\t>")==0) {
	attr &= (~PWATTR_LNK);
      } else {
	fprintf(stderr, "un tag qui pue ? '%s'\n", s);
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

      if (pv->is_plopified) {
	attr &= ~(PWATTR_U | PWATTR_BD | PWATTR_S | PWATTR_IT);
	
	if (pv->is_plopified >1) {
	  int i;
	  for (i=0; s[i]; i++) { if (s[i] >= 'A' && s[i] <= 'Z') s[i] = s[i]+'a'-'A'; }
	  if (strlen(s) >= 3) {
	    plopify_word(s, PVTP_SZ,  (char*)p - (char*)mi->msg);
	  }
	}
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

    pv = pv_tmsgi_parse(trib, mi, with_seconds, 1, 
			pp->nick_mode, pp->trollscore_mode, pp->disable_plopify); 
    pv_justif(pp, pv, 8, pp->win_width - (pp->sc ? SC_W-1 : 0));
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



/* a appeler quand la fortune est changée */
static void
pp_update_fortune(Dock *dock)
{
  Pinnipede *pp = dock->pinnipede;
  
  assert(flag_updating_comments == 0);
  if (!picohtml_isempty(pp->ph_fortune)) {
    picohtml_freetxt(pp->ph_fortune);
  }
  pp->fortune_h = 0; /* quand pp->fortune_h != 0 => il y a une fortune à afficher */
  pp->fortune_w = 0;
  if (pp->fortune_mode && dock->dlfp->fortune) {
    char *s;
    s = dock->dlfp->fortune;
    if (s == NULL) s = "pas de fortune pour l'instant...<br>";
    picohtml_parse(dock, pp->ph_fortune, s, pp->win_width - 6);
    picohtml_gettxtextent(pp->ph_fortune, &pp->fortune_w, &pp->fortune_h);
    if (!picohtml_isempty(pp->ph_fortune)) { /* on s'arrête si la fortune est vide (s == "" par ex..)
						(ça peut arriver et ça declenche le assert(!isempty) de refresh_fortune) */
      pp->fortune_h += 3; 
      pp->fortune_h = MIN(pp->fortune_h, pp->win_height/2); /* faut pas exagerer */
    }
  }
}


static void
pp_selection_unselect(Pinnipede *pp) {
  if (pp->lignes_sel) free(pp->lignes_sel);
  pp->lignes_sel = NULL;
  pp->sel_l0 = pp->sel_l1 = 0;
  pp->sel_anchor_x = pp->sel_head_x = 0;
  pp->sel_anchor_y = pp->sel_head_y = 0;
}



/* macros pour le calcul des differentes positions d'affichage des lignes */
#define LINEY0(l) (pp->win_height-(pp->use_minibar ? MINIB_H:0)-(pp->nb_lignes-l)*pp->fn_h-(pp->zmsg_h - pp->nb_lignes*pp->fn_h)/2) //(pp->zmsg_y + (l)*pp->fn_h)
#define LINEY1(l) (LINEY0(l)+pp->fn_h-1) //(pp->zmsg_y + (l+1)*pp->fn_h-1)
#define LINEBASE(l) (LINEY0(l) + pp->fn_base->ascent) //(pp->zmsg_y + pp->fn_base->ascent + (l-1)*pp->fn_h)

/* positionnenement de la scrollbar */
static void
pp_scrollcoin_move_resize(Dock *dock)
{
  Pinnipede *pp = dock->pinnipede;
  int y;
  y = LINEY0(0);
  scrollcoin_resize(pp->sc, pp->win_width - SC_W+1, y, pp->win_height - y - (pp->use_minibar ? MINIB_H-1 : 0));
}


/*
  ça tient du vilain bricolage.. mais bon: la scrollbar doit connaitre ses bornes
  le chiffre qu'elle renvoie correspond à l'id du message affiché sur la dernière ligne du pinnipede
  
  le problème est alors la borne vmin: c'est l'id du dernier message de la première page,
  ce qui demande un certain nombre de manip pour le determiner

  (si scroll_coin avait travaillé par ligne plutot que par id, ça aurait été encore pire)
*/

static void
pp_scrollcoin_update_bounds(Dock *dock, DLFP_tribune *trib)
{
  Pinnipede *pp = dock->pinnipede;
  tribune_msg_info *mi;
  int lcnt;

  int vmin, vmax;

  if (pp->sc == NULL) return;
  
  //  myprintf("%<YEL pp_scrollcoin_update_bounds>\n");
  vmax = count_all_id(trib, &pp->filter);
  vmin = 0;
  lcnt = 0;
  mi = trib->msg; /* premier message */
  while (lcnt < pp->nb_lignes && mi) {
    if (filter_msg_info(mi,&pp->filter)) {
      PostVisual *pv;
      pv = pp_pv_add(pp, trib, mi->id); 
      if (pv) { lcnt += pv->nblig; vmin++; }
    }
    mi = mi->next;
  }
  scrollcoin_setbounds(pp->sc, vmin, vmax);
  pp_scrollcoin_move_resize(dock);
}


/* mise à jour du contenu du pinnipede (reparse tous les messages affichés etc...
   c'est une des fonctions les plus importantes) */
/* adjust: param interne, appeler toujours avec adjust  = 0 */
void
pp_update_content(Dock *dock, DLFP_tribune *trib, int id_base, int decal, int adjust, int update_scrollbar_bounds)
{
  Pinnipede *pp = dock->pinnipede;
  int cur_lig, id, plig;
  PostVisual *pv;

  if (flag_updating_tribune) return;

  if (pp->lignes_sel) {
    pp_selection_unselect(pp);
  }

  pp_update_fortune(dock);

  pp->last_post_id = trib->last_post_id;

  pp->zmsg_y = pp->fortune_h == 0 ? 3 : pp->fortune_h;
  pp->zmsg_h = pp->win_height - pp->zmsg_y - (pp->use_minibar ? MINIB_H : 0);


  pp->nb_lignes = pp->zmsg_h / pp->fn_h;
  cur_lig = pp->nb_lignes;

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
	//	if (pp->decal_base != 0 || get_next_id(trib, pp->id_base) != -1) { // on n'est pas ligné sur le dernier post ?
	  pp_update_content(dock,trib,pp->id_base,pp->decal_base+cur_lig, 1, update_scrollbar_bounds); /* pas joli-joli */
	  //	}
	} else {
	  id = get_next_id(trib, -1, NULL, &pp->filter); /* premier id */
	  if (id >= 0) { /* sinon ça veut dire que la tribune est comptelemt vide */
	    pp_update_content(dock,trib,id,0, 0, update_scrollbar_bounds); /* pas joli-joli */
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
    //      if (id >= 0) { /* sinon ça veut dire que la tribune est comptelemt vide */
    //	pp_update_content(dock,trib,id,0, 0); /* pas joli-joli */
    //      }
    //    }

    //printf("pv->nblig=%d, , pid=%d\n",pv->nblig, id);
    if (decal + pv->nblig > 0) { // test si le 'scroll up a été suffisant
      PostWord *pw;

      if (pp->id_base == -1) { /* on sauve l'id_base et le decal soigneusement calculé */
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

  if (pp->sc) {
    if (update_scrollbar_bounds) { pp_scrollcoin_update_bounds(dock, trib); }
    else { pp_scrollcoin_move_resize(dock); }

    scrollcoin_setpos(pp->sc, get_id_count(trib, &pp->filter, pp->id_base));
  }
}

void
pp_minib_initialize(Pinnipede *pp)
{
  int i;

  int x;

  x = pp->win_width; //(pp->sc ? SC_W : 0);
  i = 0;
  pp->mb[i].type = HELP;            pp->mb[i].w = SC_W-1; x -= pp->mb[i].w; pp->mb[i].x = x; i++;
  pp->mb[i].type = SCROLLBAR;       pp->mb[i].w = 12; x -= pp->mb[i].w; pp->mb[i].x = x; i++;
  pp->mb[i].type = UA;              pp->mb[i].w = 12; x -= pp->mb[i].w; pp->mb[i].x = x; i++;
  pp->mb[i].type = SECOND;          pp->mb[i].w = 12; x -= pp->mb[i].w; pp->mb[i].x = x; i++;
  pp->mb[i].type = TSCORE;          pp->mb[i].w = 12; x -= pp->mb[i].w; pp->mb[i].x = x; i++;
  pp->mb[i].type = FORTUNE;         pp->mb[i].w = 12; x -= pp->mb[i].w; pp->mb[i].x = x; i++;
  pp->mb[i].type = FILTER;          pp->mb[i].w = 12; x -= pp->mb[i].w; pp->mb[i].x = x; i++;
  pp->mb[i].type = PLOPIFY;         pp->mb[i].w = 12; x -= pp->mb[i].w; pp->mb[i].x = x; i++;
  pp->mb[i].type = REFRESH_NEWS;    pp->mb[i].w = 60; x -= pp->mb[i].w; pp->mb[i].x = x; i++;
  pp->mb[i].type = REFRESH_TRIBUNE; pp->mb[i].w = 60; x -= pp->mb[i].w; pp->mb[i].x = x; i++;
  assert(i == NB_MINIB);

  for (i=0; i < NB_MINIB; i++) {
    pp->mb[i].clicked = 0;
    pp->mb[i].y = pp->win_height - MINIB_H;
    pp->mb[i].h = MINIB_H;
  }
}


/* a nice piece of shit, if i can say */
static void
pp_minib_refresh(Dock *dock)
{
  Pinnipede *pp = dock->pinnipede;
  int i,x,y;

  int x_minib;

  if (pp->use_minibar == 0) return;

  /* dessin general */
  XSetForeground(dock->display, dock->NormalGC, pp->minib_pixel);
  XFillRectangle(dock->display, pp->lpix, dock->NormalGC, 0, 1, pp->win_width, MINIB_H-1);
  XSetForeground(dock->display, dock->NormalGC, pp->minib_dark_pixel);
  XDrawLine(dock->display, pp->lpix, dock->NormalGC, 0, 0, pp->win_width, 0);

  x_minib = pp->mb[0].x;
  for (i=0; i < NB_MINIB; i++) x_minib = MIN(x_minib, pp->mb[i].x);

  XSetFont(dock->display, dock->NormalGC, pp->fn_minib->fid);
  if (pp->filter.filter_mode) {
    char s_filtre[70];
    
    if (pp->filter.filter_name) {
      snprintf(s_filtre, 60, "%.40s [match:%d msg]", 
	       pp->filter.filter_name, count_all_id(&dock->dlfp->tribune, &pp->filter));
    } else {
      snprintf(s_filtre, 60, "FILTRE NON DEFINI");
    }
    XSetForeground(dock->display, dock->NormalGC, BlackPixel(dock->display, dock->screennum));
    XDrawString(dock->display, pp->lpix, dock->NormalGC, 5, pp->fn_minib->ascent+1, s_filtre, strlen(s_filtre));
  } else {
    
    /* affichage de la charge du serveur de dlfp */

    char s_cpu[20];
    char s_xp[20], s_votes[20], s_nb_messages[20], s_http_stats[40];
    int x, w;
    s_xp[0] = 0; s_cpu[0] = 0; s_votes[0] = 0; s_nb_messages[0] = 0;
    if (Prefs.user_cookie || Prefs.force_fortune_retrieval) {
      if (flag_updating_comments == 0) {
	/*
	  
	le CPU a disparu (ouuiiiinnn!)
	if (dock->dlfp->CPU != -1.0) {
	snprintf(s_cpu, 20, "cpu:%1.2f", dock->dlfp->CPU);
	} else snprintf(s_cpu, 20, "cpu: ??");
	*/
	if (dock->dlfp->xp >= 0) {
	  snprintf(s_xp, 20, "xp:%d", dock->dlfp->xp);
	  snprintf(s_votes, 20, "[%d/%d]", dock->dlfp->votes_cur, dock->dlfp->votes_max);
	}
      }
    }
    snprintf(s_nb_messages, 20, "%d msg", count_all_id(&dock->dlfp->tribune, &pp->filter));
    snprintf(s_http_stats, 40, "UP:%d, DL:%d", global_http_upload_cnt, global_http_download_cnt);
    XSetForeground(dock->display, dock->NormalGC, BlackPixel(dock->display, dock->screennum));
    
    x = 5;
    w = MINIB_FN_W*strlen(s_cpu);
    if (x+w < x_minib) {
      XDrawString(dock->display, pp->lpix, dock->NormalGC, x, pp->fn_minib->ascent+1, s_cpu, strlen(s_cpu));
    }
    x += w + 6;
    w = MINIB_FN_W*strlen(s_xp);
    if (strlen(s_xp)) {
      if (x+w < x_minib) {
	XDrawString(dock->display, pp->lpix, dock->NormalGC, x, pp->fn_minib->ascent+1, s_xp, strlen(s_xp));
      }
    }
    x += w + 6;
    w = MINIB_FN_W*strlen(s_votes);
    if (strlen(s_votes)) {
      if (x+w < x_minib) {
	XDrawString(dock->display, pp->lpix, dock->NormalGC, x, pp->fn_minib->ascent+1, s_votes, strlen(s_votes));
      }
    }
    x += w + 6;
    w = MINIB_FN_W*strlen(s_nb_messages);
    if (strlen(s_nb_messages)) {
      if (x+w < x_minib) {
	XSetForeground(dock->display, dock->NormalGC, IRGB2PIXEL(0x7070af));	
	XDrawString(dock->display, pp->lpix, dock->NormalGC, x, pp->fn_minib->ascent+1, s_nb_messages, strlen(s_nb_messages));
      }
    }
    x += w + 6;
    w = MINIB_FN_W*strlen(s_http_stats);
    if (strlen(s_http_stats)) {
      if (x+w < x_minib) {
	XSetForeground(dock->display, dock->NormalGC, IRGB2PIXEL(0x7070af));	
	XDrawString(dock->display, pp->lpix, dock->NormalGC, x, pp->fn_minib->ascent+1, s_http_stats, strlen(s_http_stats));
      }
    }

  }

  /* dessin des boutons */
  for (i=0;i < NB_MINIB; i++) {
    int xc;

    x = pp->mb[i].x;
    xc = x + pp->mb[i].w / 2;
    y = 1;

    XSetForeground(dock->display, dock->NormalGC, pp->minib_dark_pixel);
    XDrawRectangle(dock->display, pp->lpix, dock->NormalGC, x, 0, pp->mb[i].w, pp->mb[i].h);


    if (pp->mb[i].clicked && pp->mb[i].type != REFRESH_TRIBUNE && pp->mb[i].type != REFRESH_NEWS) {
      XSetForeground(dock->display, dock->NormalGC, pp->minib_dark_pixel);
      XFillRectangle(dock->display, pp->lpix, dock->NormalGC, x, 1, pp->mb[i].w, pp->mb[i].h);
    }    
    switch (pp->mb[i].type) {
    case SCROLLBAR:
      {
	XSetForeground(dock->display, dock->NormalGC, pp->minib_dark_pixel);
	XDrawLine(dock->display, pp->lpix, dock->NormalGC, xc, 2, xc, 8);
	XDrawLine(dock->display, pp->lpix, dock->NormalGC, xc-1, 3, xc-1, 7);
	XDrawLine(dock->display, pp->lpix, dock->NormalGC, xc+1, 3, xc+1, 7);
      } break;
    case REFRESH_TRIBUNE:
      {
	char *s;

	s = "board";
	if (pp->mb[i].clicked == 0) {
	  int w, draw_bar = 0;
	  unsigned long bar_pixel = IRGB2PIXEL(0xffffff);

	  if (dock->wmcc_state_info == WMCC_SENDING_COINCOIN) { s = "post.."; draw_bar = 0; }
	  else if (dock->wmcc_state_info == WMCC_UPDATING_BOARD) { s = "updating"; draw_bar = 0; }
	  else if (dock->tribune_update_request) { s = "queued"; draw_bar = 0; }
	  else { draw_bar = 1; if (dock->wmcc_state_info != WMCC_IDLE) bar_pixel = IRGB2PIXEL(0xffc0c0); }
	  
	  if (draw_bar) {
	    XSetForeground(dock->display, dock->NormalGC, bar_pixel);
	    if (dock->tribune_refresh_delay > 0) {
	      w = ((pp->mb[i].w-2)*MIN(dock->tribune_refresh_cnt,dock->tribune_refresh_delay))/dock->tribune_refresh_delay;
	      w = (pp->mb[i].w-2 - w);
	      //	      printf("cnt: %d delai: %d w=%d\n", dock->tribune_refresh_cnt, dock->tribune_refresh_delay, w);
	      if (w > 0) {
		XFillRectangle(dock->display, pp->lpix, dock->NormalGC, x+1, 1, w, pp->mb[i].h-1);
	      }
	    }
	  }
	}
	XSetForeground(dock->display, dock->NormalGC, IRGB2PIXEL(0x303030));
	XDrawString(dock->display, pp->lpix, dock->NormalGC, xc-(MINIB_FN_W*strlen(s))/2, pp->fn_minib->ascent+1, s, strlen(s));
      } break;

    case REFRESH_NEWS:
      {
	char *s;

	s = "news/msg";
	if (pp->mb[i].clicked == 0) {
	  int w, draw_bar = 0;
	  unsigned long bar_pixel = IRGB2PIXEL(0xffffff);
	  
	  if (dock->wmcc_state_info == WMCC_UPDATING_NEWS) { s = "dl news"; draw_bar = 0; }
	  else if (dock->wmcc_state_info == WMCC_UPDATING_COMMENTS) { s = "dl com"; draw_bar = 0; }
	  else if (dock->wmcc_state_info == WMCC_UPDATING_MESSAGES) { s = "dl msg"; draw_bar = 0; }
	  else if (dock->news_update_request) { s = "queued"; draw_bar = 0; }
	  else { draw_bar = 1; if (dock->wmcc_state_info != WMCC_IDLE) bar_pixel = IRGB2PIXEL(0xffc0c0); }
	  
	  if (draw_bar) {
	    XSetForeground(dock->display, dock->NormalGC, bar_pixel);
	    if (dock->news_refresh_delay > 0) {
	      w = ((pp->mb[i].w-2)*MIN(dock->news_refresh_cnt,dock->news_refresh_delay))/dock->news_refresh_delay;
	      w = (pp->mb[i].w-2 - w);
	      //	      printf("news : cnt: %d delai: %d w=%d\n", dock->news_refresh_cnt, dock->news_refresh_delay, w);
	      if (w > 0) {
		XFillRectangle(dock->display, pp->lpix, dock->NormalGC, x+1, 1, w, pp->mb[i].h-1);
	      }
	    }
	  }
	}
	XSetForeground(dock->display, dock->NormalGC, IRGB2PIXEL(0x303030));
	XDrawString(dock->display, pp->lpix, dock->NormalGC, xc-(MINIB_FN_W*strlen(s))/2, pp->fn_minib->ascent+1, s, strlen(s));
      } break;
    case HELP:
      {
	XDrawString(dock->display, pp->lpix, dock->NormalGC, xc-MINIB_FN_W/2+1, pp->fn_minib->ascent+1, "?", 1);
      } break;
    case SECOND:
      {
	int rx, rw, ry, rh;

	rx = x + 3; ry  = y+2; rw = pp->mb[i].w-6; rh = pp->mb[i].h-6;
	XSetForeground(dock->display, dock->NormalGC, pp->timestamp_pixel);
	
	if (pp->nosec_mode) {
	  XFillRectangle(dock->display, pp->lpix, dock->NormalGC, rx, ry, rw+1, rh+1);
	} else {
	  XDrawRectangle(dock->display, pp->lpix, dock->NormalGC, rx, ry, rw, rh);
	}
      } break;
    case TSCORE:
      {
	int rx, rw, ry, rh;

	rx = x + 3; ry  = y+2; rw = pp->mb[i].w-6; rh = pp->mb[i].h-6;
	XSetForeground(dock->display, dock->NormalGC, pp->trollscore_pixel);
	
	if (pp->trollscore_mode) {
	  XFillRectangle(dock->display, pp->lpix, dock->NormalGC, rx, ry, rw+1, rh+1);
	} else {
	  XDrawRectangle(dock->display, pp->lpix, dock->NormalGC, rx, ry, rw, rh);
	}
      } break;
    case FORTUNE:
      {
	int rx, rw, ry, rh;

	rx = x + 3; ry  = y+2; rw = pp->mb[i].w-6; rh = pp->mb[i].h-6;
	XSetForeground(dock->display, dock->NormalGC, IRGB2PIXEL(0x8f8060));
	
	if (pp->fortune_mode) {
	  XFillRectangle(dock->display, pp->lpix, dock->NormalGC, rx, ry, rw+1, rh+1);
	} else {
	  XDrawRectangle(dock->display, pp->lpix, dock->NormalGC, rx, ry, rw, rh);
	}
      } break;
    case FILTER:
      {
	int rx, rw, ry, rh;

	rx = x + 3; ry  = y+2; rw = pp->mb[i].w-6; rh = pp->mb[i].h-6;
	XSetForeground(dock->display, dock->NormalGC, pp->lnk_pixel);
	
	if (pp->filter.filter_mode) {
	  XFillRectangle(dock->display, pp->lpix, dock->NormalGC, rx, ry, rw+1, rh+1);
	} else {
	  XDrawRectangle(dock->display, pp->lpix, dock->NormalGC, rx, ry, rw, rh);
	}
      } break;
    case PLOPIFY:
      {
	int rx, rw, ry, rh;

	rx = x + 3; ry  = y+2; rw = pp->mb[i].w-6; rh = pp->mb[i].h-6;
	XSetForeground(dock->display, dock->NormalGC, pp->plopify_pixel);
	
	if (pp->disable_plopify) {
	  XFillRectangle(dock->display, pp->lpix, dock->NormalGC, rx, ry, rw+1, rh+1);
	} else {
	  XDrawRectangle(dock->display, pp->lpix, dock->NormalGC, rx, ry, rw, rh);
	}
      } break;
    case UA:
      {
	int rx, rw, ry, rh;

	rx = x + 3; ry  = y+2; rw = pp->mb[i].w-6; rh = pp->mb[i].h-6;
	XSetForeground(dock->display, dock->NormalGC, pp->nick_pixel);
	
	XDrawRectangle(dock->display, pp->lpix, dock->NormalGC, rx, ry, rw, rh);
	if (pp->nick_mode == 3) {
	  XFillRectangle(dock->display, pp->lpix, dock->NormalGC, rx, ry, rw, rh);
	} else if (pp->nick_mode == 1) {
	  XFillRectangle(dock->display, pp->lpix, dock->NormalGC, rx,ry,xc-rx+1,rh);
	} else if (pp->nick_mode == 2) {
	  XFillRectangle(dock->display, pp->lpix, dock->NormalGC, xc,ry,xc-rx+1,rh);
	} else if (pp->nick_mode == 4) {
	  XDrawLine(dock->display, pp->lpix, dock->NormalGC, xc, ry, xc, ry+rh-1);
	}
      } break;
    default:
      abort(); break;
    }
  }

  /* et hop */
  XCopyArea(dock->display, pp->lpix, pp->win, dock->NormalGC,
	    0,0,pp->win_width, MINIB_H, 0, MINIB_Y0);
}


static PPMinib *
pp_minib_get_button(Dock *dock, int x, int y)
{
  Pinnipede *pp = dock->pinnipede;
  int i;

  for (i=0; i < NB_MINIB; i++) {
    if (x >= pp->mb[i].x && x < pp->mb[i].x+pp->mb[i].w && 
	y >= pp->mb[i].y && y < pp->mb[i].y+pp->mb[i].h) {
      return &pp->mb[i];
    }
  }
  return NULL;
}

static PPMinib *
pp_minib_pressed_button(Dock *dock)
{
  Pinnipede *pp = dock->pinnipede;
  int i;

  for (i=0; i < NB_MINIB; i++) {
    if (pp->mb[i].clicked)
      return &pp->mb[i];
  }
  return NULL;
}

static void
pp_minib_show(Dock *dock) 
{
  Pinnipede *pp = dock->pinnipede;
  pp->use_minibar = 1;
  pp_minib_initialize(pp);
}

void 
pp_minib_hide(Dock *dock) 
{
  Pinnipede *pp = dock->pinnipede;
  pp->use_minibar = 0;
}


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
pp_draw_line(Dock *dock, Pixmap lpix, PostWord *pw, 
	     unsigned long bgpixel, PinnipedeLignesSel *sel_info)
{
  Pinnipede *pp = dock->pinnipede;
  int pl;
  int old_pos;
  unsigned long pixel, old_pixel;
  int y;

  XSetForeground(dock->display, dock->NormalGC, bgpixel);
  XFillRectangle(dock->display, lpix, dock->NormalGC, 0, 0, pp->win_width, pp->fn_h);



  if (sel_info) {
    if (sel_info->x0 < sel_info->x1) {
      XSetForeground(dock->display, dock->NormalGC, pp->sel_bgpixel);
      XFillRectangle(dock->display, lpix, dock->NormalGC, 
		     sel_info->x0, 0, 
		     sel_info->x1-sel_info->x0, pp->fn_h);
    }
  }

  /* dessine une ligne horizontale pour signifier qu'il y a eu une coupure dans le
     "flux" des messages (puisque le msg id-1 n'existe pas / n'a pas été récupéré) */
  if (pw && pw->ligne == 1 && pw->parent->is_skipped_id) {
    XGCValues xgc;
    xgc.foreground = IRGB2PIXEL(0xff0000);
    xgc.line_style = LineOnOffDash;
    
    XChangeGC(dock->display, dock->NormalGC, GCForeground | GCLineStyle, &xgc);
    XDrawLine(dock->display, lpix, dock->NormalGC, 0, 0, pp->win_width-1, 0);
    xgc.line_style = LineSolid;
    XChangeGC(dock->display, dock->NormalGC, GCLineStyle, &xgc);
  }

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
    if (pw->parent->is_my_message || 
	pw->parent->is_answer_to_me || 
	pw->parent->is_hilight_key) {
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

      if (pw->parent->is_plopified) {
	pixel = pp->plopify_pixel;
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

      if (pw->attr & PWATTR_U) {
	int x1;
	if (pw->next && pw->next->ligne == pw->ligne && (pw->next->attr & PWATTR_U)) {
	  x1 = pw->next->xpos;
	} else {
	  x1 = pw->xpos+pw->xwidth-1;
	}
	XDrawLine(dock->display, lpix, dock->NormalGC, pw->xpos, y+1, x1, y+1);
      }
      if (pw->attr & (PWATTR_TMP_EMPH)) {
	XDrawLine(dock->display, lpix, dock->NormalGC, pw->xpos, y+1,  pw->xpos+pw->xwidth-1, y+1);
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

void pp_refresh_hilight_refs(Pinnipede *pp, DLFP_tribune *trib, time_t timestamp, int sub_timestamp) {
  int l;

  for (l=0; l < pp->nb_lignes; l++) {
    if (pp->lignes[l]) {
      tribune_msg_info *ref2_mi;
      int pl;
      PostWord *pw;

      pw = pp->lignes[l];
      pl = pw->ligne;
      while (pw && pl == pw->ligne) {
	if (pw->attr & PWATTR_REF) {
	  int bidon, ref2_num;

	  ref2_mi = check_for_horloge_ref(trib, pw->parent->id, pw->w, NULL, 0, &bidon, &ref2_num); assert(bidon);
	  if (ref2_mi && ref2_mi->timestamp == timestamp) { /* test sur timestamp pour les situation où +sieurs msg ont le même */
	    if (ref2_num == -1                                 /* ref à plusieurs posts */
		|| (ref2_num==0 && sub_timestamp <= 0) /* au cas on a mis un ¹ inutile par inadvertance */
		|| (ref2_num == sub_timestamp)) {
	      pw->attr |= PWATTR_TMP_EMPH;
	    }
	  }
	}
	pw = pw->next;
      }
    }	
  }
}

/* 
   pw_ref: si non null, c'est que le pointeur survole une référence (c'est a d qu'il
   est utilise quand on survole une reference à un post precedent, 
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
  int ref_in_window = 0; /* mis a 1 si le message souligné par pw_ref est affiché parmi
			    les autres messages. sinon, on l'affiche en haut, dans une petite fenetre */

#define MAXANTIREF 60
  int anti_ref_id[MAXANTIREF];
  /* les antireferences : si on survolle un timestamp, on emphasize les commentaires qui
     lui font reference */
  int nb_anti_ref;
  int ref_num; /* utilise pour les ref précises dans les post multiples (ie qui ont le meme timestamp) 
		  par defaut, vaut -1 (cad désactivé)
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
      XClearArea(dock->display, pp->win, 0, y, pp->win_width-(pp->sc ? SC_W-1:0), h, False);
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
		/* sale ruse... si toutes les lignes sont là, a la fin du FOR on obtient ref_in_window = 1 */
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
	if (ref_num != -1) break; /* si c'est pas une ref à un multipost sans précision, break (chuis pas clair) */
      }
	
      /* et maintenant on detecte toutes les autres references vers ce message pour les afficher
	 temporairement en gras (ça c vraiment pour faire le cakos)*/
      pp_refresh_hilight_refs(pp, trib, ref_mi->timestamp, ref_mi->sub_timestamp);
    }
  } else if (pw_ref && (pw_ref->attr & PWATTR_TSTAMP)) {
    pp_refresh_hilight_refs(pp, trib, pw_ref->parent->tstamp, pw_ref->parent->sub_tstamp);
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

    pp_draw_line(dock, pp->lpix, pw, bgpixel, NULL);

    XCopyArea(dock->display, pp->lpix, d, dock->NormalGC, 0, 0, pp->win_width - (pp->sc ? SC_W-1 : 0), pp->fn_h, 0, LINEY0(l));
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
	  pw = pp_draw_line(dock, pp->lpix, pw, pp->emph_pixel, NULL); //WhitePixel(dock->display, dock->screennum));
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
  if (pp->sc) { scrollcoin_refresh(pp->sc, pp->win, 1); }
}


/* appelée depuis wmcoincoin.c, pour gèrer l'autoscroll et rafraichir l'affichage */
void
pp_check_tribune_updated(Dock *dock, DLFP_tribune *trib)
{
  Pinnipede *pp = dock->pinnipede;
  if (pp && pp->mapped && flag_updating_tribune == 0 && pp->lignes_sel == NULL) {
    /* test si on scrolle qutomatiquement pour afficher le nouveau message */
    //    if (trib->last_post_id != pp->last_post_id && pp->last_post_id == pp->id_base && pp->decal_base == 0) {
    if (pp->flag_tribune_updated) {
      if (pp->sc) { 
	pp_scrollcoin_update_bounds(dock, trib); 
      }      
      if (trib->last_post_id != pp->last_post_id && pp->last_post_id == pp->id_base) { // && pp->decal_base == 0) {
	//	myprintf("pp_check_tribune_updated, on %<yel colle> de %d à %d\n", pp->last_post_id, trib->last_post_id);
	pp_update_content(dock, trib, trib->last_post_id, 0, 0, 0);
      } else {
	/*	if (trib->last_post_id != pp->last_post_id)
		printf("pp_check_tribune_updated, on laisse filer de %d à %d (pos=%d/%d)\n", pp->last_post_id, trib->last_post_id, pp->id_base, pp->decal_base);*/
	pp_update_content(dock, trib, pp->id_base, pp->decal_base, 0, 0);
      }
      pp_refresh(dock, trib, pp->win, NULL);
      pp->flag_tribune_updated = 0;
    }
  }
}

void
pp_animate(Dock *dock)
{
  Pinnipede *pp = dock->pinnipede;
  if (pp && pp->mapped /*&& flag_updating_tribune == 0*/) {
    /* pour affichage du temps restant avant refresh */
    pp_minib_refresh(dock);
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
  pp->login_pixel = IRGB2PIXEL(Prefs.pp_login_color);
  pp->minib_pixel = IRGB2PIXEL(0xcdcdcd);
  pp->minib_dark_pixel = IRGB2PIXEL(0x626262);

  pp->sel_bgpixel = RGB2PIXEL(255, 215, 0);
  pp->emph_pixel = IRGB2PIXEL(Prefs.pp_emph_color);
  pp->trollscore_pixel = IRGB2PIXEL(Prefs.pp_trollscore_color);
  pp->hilight_my_msg_pixel = IRGB2PIXEL(Prefs.pp_my_msg_color);
  pp->hilight_answer_my_msg_pixel = IRGB2PIXEL(Prefs.pp_answer_my_msg_color);
  pp->hilight_keyword_pixel = IRGB2PIXEL(Prefs.pp_keyword_color);
  pp->plopify_pixel = IRGB2PIXEL(Prefs.pp_plopify_color);

  pp->id_base = -1; pp->decal_base = 0;

  pp->win_width = Prefs.pp_width;
  pp->win_height = Prefs.pp_height;
  pp->win_xpos = Prefs.pp_xpos;
  pp->win_ypos = Prefs.pp_ypos;
  pp->lignes = NULL;
  pp->nb_lignes = 0;
  pp->lignes_sel = NULL;
  pp->sel_l0 = pp->sel_l1 = 0;
  pp->sel_anchor_x = pp->sel_head_x = 0;
  pp->sel_anchor_y = pp->sel_head_y = 0;

  pp->sc = NULL;

  //  pp->html_mode = Prefs.pp_html_mode;
  pp->nick_mode = Prefs.pp_nick_mode;
  pp->nosec_mode = Prefs.pp_nosec_mode;
  pp->trollscore_mode = Prefs.pp_trollscore_mode;
  pp->disable_plopify = 0;
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

  pp->use_minibar = Prefs.pp_minibar_on;

  if (pp->use_minibar)
    pp_minib_initialize(pp);

  pp->fn_minib = dock->fixed_font;

  pp->flag_tribune_updated = 0;
}

/* 
   fonction inutile, sert juste à reperer si y'a des memory leaks 
*/
#ifdef TEST_MEMLEAK
void
pp_destroy(Dock *dock)
{
  Pinnipede *pp = dock->pinnipede;
  if (pp->mapped) pp_unmap(dock);
  assert(pp->pv == NULL); assert(pp->sc == NULL); assert(pp->lignes_sel == NULL);
  picohtml_destroy(dock->display, pp->ph_fortune);
  free(pp); dock->pinnipede = NULL;
}
#endif 

/*
  un petit mot: j'ai enfin compris comment faire apparaitre cette fenetre
   ou je veux: il suffit de donner la position dans xcreatewindow
   et d'activer USPosition dans les WM_HINTS (MAIS, les valeurs indiquées dans
   les champs x et y n'ont aucune influence)
*/
void
pp_show(Dock *dock, DLFP_tribune *trib)
{
  XSetWindowAttributes wa;
  Pinnipede *pp = dock->pinnipede;
  int xpos, ypos;

  if (pp->win_xpos == -10000 && pp->win_ypos == -10000) {
    xpos = 0; ypos = 0; /* ça n'a d'effet que sur certain windowmanagers rustiques (genre pwm) */
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

  /* pour etre informé de la fermeture de la fenetre demandee par le windowmanager */
  XSetWMProtocols(dock->display, pp->win, &dock->atom_WM_DELETE_WINDOW, 1);

  pp->lpix = XCreatePixmap(dock->display, pp->win, pp->win_width, MAX(MINIB_H, pp->fn_h), DefaultDepth(dock->display,dock->screennum));

  /*
  {
    XWMHints *mwh = XAllocWMHints();
    mwh->initial_state = WithdrawnState;
    XSetWMHints(dock->display, pp->win, mwh);
    }*/

  XMapRaised(dock->display, pp->win);

  assert(pp->sc == NULL);

  pp->sc = scrollcoin_create(1,1,1,pp->win_width-SC_W+1, 0, pp->win_height-20);

  //  XMoveWindow(dock->display, pp->win, 100, 100);
  pp->mapped = 1;
  pp_update_content(dock, trib, trib->last_post_id, 0, 0, 1);
    
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

/* affiche le texte en haut du pinnipede (dans le style très dépouillé des useragents) */
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

  /* on parcourt tous les message postérieurs à base_mi */
  mi = base_mi->next;
  while (mi) {
    int i;
    /* on regarde toutes ses references */
    for (i = 0; i < mi->nb_refs; i++) {
      tribune_msg_info *ref_mi;
      int j;

      /* pour chaque ref, on regarde la liste (generalement de taille 1 ou 0) des messages pointés */
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
   et d'agir le cas echeant (de maniere un peu désordonnée)
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
      char blah[1024];
      char *s;
      int blah_sz = 1024;

      int nrep;
      KeyList *hk;
      mi = tribune_find_id(trib, pw->parent->id);

      s = blah; s[0] = 0;
      if (mi->is_my_message) {
	snprintf(s, blah_sz, "\n[vous avez posté ce message]"); blah_sz -= strlen(s); s += strlen(s);
      }
      if (mi->is_answer_to_me && blah_sz>30) {
	snprintf(s, blah_sz, "\n[ce message répond à l'un des votres]"); blah_sz -= strlen(s); s += strlen(s);
      }
      hk = tribune_key_list_test_mi(trib, mi, trib->hilight_key_list);
      if (hk && blah_sz > 60) {
	snprintf(s, blah_sz, "\nmessage 'encadré' car: "); blah_sz -= strlen(s); s += strlen(s);
	while (hk && blah_sz > 30) {
	  snprintf(s, blah_sz, " {%s='%.20s'}", 
		   tribune_key_list_type_name(hk->type), hk->key); blah_sz -= strlen(s); s += strlen(s);
	  hk = tribune_key_list_test_mi(trib, mi, hk->next);
	}
      }
      hk = tribune_key_list_test_mi(trib, mi, trib->plopify_key_list);
      if (hk && blah_sz > 60) {
	snprintf(s, blah_sz, "\nmessage plopifié  car: "); blah_sz -= strlen(s); s += strlen(s);
	while (hk && blah_sz > 30) {
	  snprintf(s, blah_sz, " {%s='%.20s'}", 
		   tribune_key_list_type_name(hk->type), hk->key); blah_sz -= strlen(s); s += strlen(s);
	  hk = tribune_key_list_test_mi(trib, mi, hk->next);
	}
      }
      nrep = pp_count_backrefs(mi);

      snprintf(survol, 1024, "id=%d ua=%s\n%d réponse%s%s", pw->parent->id, (mi ? mi->useragent : ""), 
	       nrep, (nrep > 1) ? "s" : "", blah);
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

  pp_selection_unselect(pp);

  /* le pp_refresh a juste pour but de 'delocker' le PostVisual sauvé dans le cache
     oui c'est de la bidouille qui sent les remugles nauséabonds */
  pp_refresh(dock, &dock->dlfp->tribune, pp->win, NULL);

  /* on sauve la position de la fenetre (en prenant en compte les decorations du WM ) */
  get_window_pos_with_decor(dock->display, pp->win, &pp->win_xpos, &pp->win_ypos);
  //  pp_minib_hide(dock);
  XDestroyWindow(dock->display, pp->win);
  XFreePixmap(dock->display, pp->lpix);
  pp->win = None;
  pp->mapped = 0;

  if (pp->sc) { scrollcoin_destroy(pp->sc); pp->sc = NULL; }

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
    snprintf(infomsg,512,"Scrinechote de la tribune (%s) à %s", file_name, ctime(&time_shot));
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
	       "<p align=center>bienvenue dans le <b><font color=#008000>Pinnipède Télétype</font></b></p>"
	       "Cette fenêtre à été spécialement conçue par les plus grands experts pour offrir "
	       "des conditions de moulage optimales.<br>"
	       "Voici donc un rappel des fonctionnalités:<br><br>"
	       "<b>Pour scroller</b>, utiliser la roulette de la souris, ou 'tirer' "
	       "en cliquant avec le bouton de droite. "
	       "Le scroll est automatique lors de l'apparition d'un nouveau message.<br><br>"
	       "Pour faire apparaitre/disparaitre <b>la barre de boutons</b>, utilisez le clic droite. "
	       "Les deux premiers boutons permettent "
	       "de scroller, plus ou moins vite selon que vous cliquez "
	       "avec le bouton de gauche ou de droite<br><br>"
	       "Les actions disponibles sur l'<b>horloge</b> identifiant un message sont:<br>"
	       "<font color=blue>Click Gauche</font><tab>: ouvre le palmipède editor, et insère une référence au message<br>"
	       "<font color=blue>Click Milieu</font><tab>: copie le contenu du message dans le clipboard<br>"
	       "<font color=blue>Click Droite</font><tab>: copie le contenu du useragent dans le clipboard<br><br>"
	       "En cliquant sur une <b>[url]</b>, le résultat sera:<br>"
	       "<font color=blue>Click Gauche</font><tab>: ouverture de l'url dans le browser externe (si il a été "
	       "défini dans le fichier ~/.wmcoincoin/options). <b>Attention</b>, même si des précautions ont été prises "
	       "depuis l'ignominieux wmcoincoin 2.0, ce genre de chose reste généralement considèré comme une faiblesse dans la sécurité...<br>"
	       "<font color=blue>Click Milieu</font><tab>: ouverture de l'url avec le second browser (option http.browser2)<br>"
	       "<font color=blue>Click Droite</font><tab>: copie l'url dans le clipboard<br><br>"
	       "Quand le pointeur se trouve au-dessus d'une <b>référence à un post précédent</b>, celui-ci sera souligné. Si vous cliquez:<br>"
	       "<font color=blue>Click Gauche</font><tab>: 'aller' au message référencé<br>"
	       "<font color=blue>Click Droite</font><tab>: copier cette référence dans le clipboard (d'accord, c'est pas très utile...)<br><br>"

	       "Un filtrage rudimentaire est disponible à l'aide de <font color=blue>Ctrl+left clic</font> "
	       "sur un mot, login, useragent.. (utiliser le bouton bleu pour l'annuler)<br> Si vous voulez au contraire mettre en relief les post d'une personne ou bien ceux contenant un certain mot, le <font color=blue>Shift+Click Droite</font> sera votre ami<br>"
	       
	       "<b>Nouveau:</b> quelqu'un vous saoule méchamment ? vous avez lancé un troll qui vous échappe ? Alors plopifiez les malfaisants avec un <font color=blue>Shift+Click Droite</font> sur son login/useragent<br><br>"

	       "Vous pouvez prendre un 'shot' de la tribune (aka plateau de fruits de mer), avec <font color=blue>Ctrl+Middle Clic</font><br><br>"

	       "Pour comprendre l'affichage des <b>useragents</b> activé par le bouton rouge sombre (à quinze pixels sur votre gauche), se reporter au "
	       "fichier <tt>~/.wmcoincoin/useragents</tt><br> (hint: il a 5 positions différentes)<br><br>"
	       "Le pinnipède télétype vous souhaite un agréable moulage.", 500);
}


static int
pp_minib_handle_button_press(Dock *dock, XButtonEvent *ev) {
  PPMinib *mb;
  mb = pp_minib_get_button(dock, ev->x, ev->y);
  if (mb) {
    mb->clicked = 1;
    pp_minib_refresh(dock);
    return 1;
  } else return 0;
}


int
pp_minib_handle_button_release(Dock *dock, DLFP_tribune *trib, XButtonEvent *event)
{
  Pinnipede *pp = dock->pinnipede;
  PPMinib *mb;
  int i, need_refresh;

  pp_selection_unselect(pp);

  mb = pp_minib_get_button(dock, event->x, event->y);
  if (event->type == ButtonRelease  && mb && mb->clicked == 1) {
    switch (mb->type) {
      /*
    case 0: 
    case 1: 
      {
	int q;
	if (event->button == Button1) {
	  q = 3;
	} else q = 10;
	    if (pressed == 1) q = -q;
	    pp_update_content(dock, trib, pp->id_base, pp->decal_base-q,0,0);
	    pp_refresh(dock, trib, pp->win, NULL);
      } break;
    case 2:
      {
	pp_update_content(dock, trib, get_next_id(trib, -1, NULL, &pp->filter), 0,0,0);
	pp_refresh(dock, trib, pp->win, NULL);
      } break;
    case 3:
      {
	pp_update_content(dock, trib, trib->last_post_id, 0,0,0);
	pp_refresh(dock, trib, pp->win, NULL);
	} break;
*/

    case REFRESH_TRIBUNE:
      {
	dock->tribune_update_request = 1; pp_minib_refresh(dock);
      } break;

    case REFRESH_NEWS:
      {
	dock->news_update_request = 1; pp_minib_refresh(dock);
      } break;

    case HELP:
      {
	pp_minib_refresh(dock);
	pp_balloon_help(dock, mb->x-20, mb->y-20);
      } break;
    case UA:
      {
	pp->nick_mode = (pp->nick_mode + 1) % 5;
	pp_pv_destroy(pp);
	pp_update_content(dock, trib, pp->id_base, pp->decal_base,0,1);
	pp_refresh(dock, trib, pp->win, NULL);	    
      } break;
    case SECOND:
      {
	pp->nosec_mode = (1-pp->nosec_mode);
	pp_pv_destroy(pp);
	pp_update_content(dock, trib, pp->id_base, pp->decal_base,0,1);
	pp_refresh(dock, trib, pp->win, NULL);	    
      } break;
    case FILTER:
      {
	pp->filter.filter_mode = (1-pp->filter.filter_mode);

	if (pp->filter.filter_mode) pp->id_base = -1; /* reset du scroll (necessaire, faut etre que le post 'id_base' soit bien affiché par le filtre) */

	pp_pv_destroy(pp);
	pp_update_content(dock, trib, pp->id_base, pp->decal_base,0,1);
	pp_refresh(dock, trib, pp->win, NULL);	    	    
      } break;
    case PLOPIFY:
      {
	pp->disable_plopify = (1-pp->disable_plopify);

	pp_pv_destroy(pp);
	pp_update_content(dock, trib, pp->id_base, pp->decal_base,0,1);
	pp_refresh(dock, trib, pp->win, NULL);	    	    
      } break;
    case TSCORE:
      {
	if (Prefs.enable_troll_detector) {
	  pp->trollscore_mode = (1-pp->trollscore_mode);
	  pp_pv_destroy(pp);
	  pp_update_content(dock, trib, pp->id_base, pp->decal_base,0,1);
	  pp_refresh(dock, trib, pp->win, NULL);	    	    
	} else {
	  msgbox_show(dock, "inutile de cliquer sur ce bouton, le troll_detector est désactivé (voir l'option 'tribune.enable_troll_detector')");
	}
      } break;
    case FORTUNE:
      {
	if (Prefs.user_cookie || Prefs.force_fortune_retrieval) {
	  pp->fortune_mode = (1-pp->fortune_mode);
	  pp_pv_destroy(pp);
	  pp_update_content(dock, trib, pp->id_base, pp->decal_base,0,1);
	  pp_refresh(dock, trib, pp->win, NULL);	    	    	
	} else {
	  msgbox_show(dock, "inutile de cliquer sur ce bouton si vous n'avez ni donné le cookie à wmcoincoin, ni utilise l'option 'http.force_fortune_retrieval'");
	}
      } break;
    case SCROLLBAR:
      {
	if (pp->sc) { scrollcoin_destroy(pp->sc); pp->sc = NULL; }
	else { pp->sc = scrollcoin_create(1,1,1,pp->win_width-SC_W+1, 0, pp->win_height-20); }
	pp_pv_destroy(pp);
	pp_update_content(dock, trib, pp->id_base, pp->decal_base,0,1);
	pp_refresh(dock, trib, pp->win, NULL);
      } break;
    default:
      assert(0); 
    }
  }
  for (i=0, need_refresh=0; i < NB_MINIB; i++) {
    if (pp->mb[i].clicked) {
      pp->mb[i].clicked = 0; need_refresh++;
    }
  }
  if (need_refresh) { pp_minib_refresh(dock); }
  return need_refresh;
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
      /* si la ref n'etait pas déjà dans la liste, on l'ajoute */
      if (pp_thread_filter_find_id(f, mi->id)==0) {
	//myprintf("    ref(%d): ajout de %<YEL %d> <-- %<MAG %d>\n", f->nid, mi->id, base_mi->id);
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

  /* on parcourt tous les message postérieurs à base_mi */
  mi = base_mi->next;
  while (mi) {
    int i;
    /* on regarde toutes ses references */
    for (i = 0; i < mi->nb_refs; i++) {
      tribune_msg_info *ref_mi;
      int j;

      /* pour chaque ref, on regarde la liste (generalement de taille 1 ou 0) des messages pointés */
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
	    //myprintf("backref(%d): ajout de %<YEL %d> --> %<MAG %d>\n", f->nid, mi->id, base_mi->id);
	    
	    f->id[f->nid++] = mi->id;
	    /* et hop ça recurse un coup */
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
  pp_update_content(dock, trib, -1, 0, 0, 1);
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
  pp_update_content(dock, trib, -1, 0, 0, 1);
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
  pp_update_content(dock, trib, -1, 0, 0, 1);
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
  pp_update_content(dock, trib, -1, 0, 0, 1);
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
      while (p > s && !isalnum((unsigned char)*p)) { *p = 0; p--; }
      p = s;
      while (*p && !isalnum((unsigned char)*p)) p++;
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
   --> permet l'ajout/enlevement de mots clefs de mise en valeur de post
   
ET aussi

  gestion des shift+right click sur un mot du pinnipede
   --> permet la plopification/deploppification des messages selon
   un useragent/login/mot clef
*/
static void
pp_handle_shift_clic(Dock *dock, DLFP_tribune *trib, KeyList **pkl, int mx, int my, int plopify_flag)
{
  Pinnipede *pp = dock->pinnipede;
  PostWord *pw;
  int thread_clic;

  thread_clic = 0;
  pw = pp_get_pw_at_xy(pp, mx, my);

  if (pw) {
    if (pw->attr & PWATTR_TROLLSCORE) thread_clic = 1;
  } else {
    if (mx < 20) { /* si on clique à gauche de l'horloge, on considère une action sur tout le
		      thread (le pb est qu'il n'y a pas forcement de mot à cet endroit) */
      pw = pp_get_pw_at_xy(pp, 20, my); /* le 20 est une ruse de chacal puant */
      thread_clic = 1;    
    }
  }

  if (pw) {
    tribune_msg_info *mi;
    mi = tribune_find_id(trib, pw->parent->id);

    if (mi && thread_clic) {
      char sid[15];
      snprintf(sid, 15, "%d", mi->id);
      *pkl = tribune_key_list_swap(*pkl, sid, HK_THREAD);
    } else if (mi && pw->attr & PWATTR_NICK) {
      if (plopify_flag == 0) {
	*pkl = tribune_key_list_swap(*pkl, mi->useragent, HK_UA);
      } else {
	if (mi->login[0]) *pkl = tribune_key_list_swap(*pkl, mi->login, HK_LOGIN);
	else tribune_key_list_swap(*pkl, mi->useragent, HK_UA_NOLOGIN);
      }
    } else if (pw->attr & PWATTR_LOGIN) {
      *pkl = tribune_key_list_swap(*pkl, mi->login, HK_LOGIN);
    } else if (pw->attr & PWATTR_TSTAMP) {
      char sid[10];
      snprintf(sid,10,"%d", pw->parent->id);
      *pkl = tribune_key_list_swap(*pkl, sid, HK_ID);
    } else {
      char *s, *p;
      KeyList *hk;

      /* pour deplopifier un post plopifié à cause d'un mot clef, on ne peut pas esperer que l'utilisateur
	 puisse recliquer sur le mot incriminé puisque celui a de fortes chances d'avoir été transformé
	 en 'plop' ou 'pikaa' ...
      */
      if (plopify_flag && mi && (hk = tribune_key_list_test_mi(trib, mi, *pkl))) {
	*pkl = tribune_key_list_remove(*pkl, hk->key, hk->type);
      } else {
	
	/* simplification du mot */
	s = strdup(pw->w);
	p = s + strlen(s) -1;
	while (p > s && !isalnum((unsigned char)*p)) { *p = 0; p--; }
	p = s;
	while (*p && !isalnum((unsigned char)*p)) p++;
	
	if (strlen(p) == 0) p = pw->w;
	
	*pkl = tribune_key_list_swap(*pkl, p, HK_WORD);
	free(s);
      }
    }

    *pkl = tribune_key_list_cleanup(trib, *pkl); /* supprime les key faisant ref à des messages detruits */

    pp_pv_destroy(pp); /* force le rafraichissement complet */
    pp_update_content(dock, trib, pp->id_base, pp->decal_base,0,1);
    pp_refresh(dock, trib, pp->win, NULL);
  }
}

static void
pp_handle_left_clic(Dock *dock, DLFP_tribune *trib, int mx, int my)
{
  Pinnipede *pp = dock->pinnipede;
  PostWord *pw;

  /* affichage/masquage du 'crochet' à gauche des messages mis en valeur */
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
	KeyList *hk;
	mi = tribune_find_id(trib, pw->parent->id);
	if (mi && (hk = tribune_key_list_test_mi(trib, mi, trib->hilight_key_list))) {
	  trib->hilight_key_list = tribune_key_list_remove(trib->hilight_key_list, hk->key, hk->type);
	}
      } else changed = 0;
      if (changed) {
	pp_pv_destroy(pp); /* force le rafraichissement complet */
	pp_update_content(dock, trib, pp->id_base, pp->decal_base,0,0);
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
      case 0: s_subts[0] = '¹'; break;
      case 1: s_subts[0] = '²'; break;
      case 2: s_subts[0] = '³'; break;
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
      /* clic sur une reference, on va essayer de se déplacer pour afficher la ref en bas du
	 pinnipede */
      tribune_msg_info *mi;
      int bidon;
      
      mi = check_for_horloge_ref(trib, pw->parent->id, pw->w, NULL, 0, &bidon, NULL); assert(bidon);
      if (mi) {
#ifdef BOULAI_MODE
	PostWord *trouve;
	int i;
	/* si le message referencé est déjà affiché, on ne fait rien */
	for (i=0, trouve=NULL; i < pp->nb_lignes; i++) {
 	  if (pp->lignes[i] && 
	      pp->lignes[i]->parent->id == mi->id && 
	      pp->lignes[i]->ligne == 0) trouve = pp->lignes[i];
	}
	if (trouve == NULL) {
	  PostVisual *pv;
	  /* c'est un peu lourd pour positionner le message juste en haut du pinnipede.. */
	  pv = pp_pv_add(pp, trib, mi->id);
	  if (pv) {
	    int ligne = pp->nb_lignes - pv->nblig; // - (my-LINEY0(0))/(LINEY0(1)-LINEY0(0));
	    pp_update_content(dock, trib, mi->id, ligne, 0, 0);
	  }
	  trouve = pp->lignes[0];
	}
	pp_refresh(dock, trib, pp->win, trouve);
#else
	pp_update_content(dock, trib, mi->id, 0, 0, 0);
	pp_refresh(dock, trib, pp->win, NULL);
#endif
      }
    } 
  } /* if (pw) */  
}

/* gestion du relachement du bouton souris (si on n'est pas en train de 'tirer' la fenetre, 
   et si on n'a pas cliqué sur la barre de petits boutons */
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
    pp_update_content(dock, trib, pp->id_base, pp->decal_base-q,0,0);
    pp_refresh(dock, trib, pp->win, NULL);
    //    printf("scroll up  : id=%d %d\n",pp->id_base, pp->decal_base);
  } else if (event->button == Button5) {
    pp_update_content(dock, trib, pp->id_base, pp->decal_base+q,0,0);
    pp_refresh(dock, trib, pp->win, NULL);
    //printf("scroll down: id=%d %d\n",pp->id_base, pp->decal_base);
  } else if (event->button == Button1) {
    if (event->state & ShiftMask) {
      pp_handle_shift_clic(dock, trib, &trib->hilight_key_list, mx, my, 0);
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
    if (event->state & ShiftMask) {
      pp_handle_shift_clic(dock, trib, &trib->plopify_key_list, mx, my, 1);
    } else {
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
	  pp_update_content(dock, trib, pp->id_base, 0,0,1);
	  pp_refresh(dock, trib, pp->win, NULL);	    	    
	}
      }
    }
  } /* if right clic */
}

int 
flush_expose(Dock *dock, Window w) {
  XEvent dummy;
  int i=0;

  while (XCheckTypedWindowEvent(dock->display, w, Expose, &dummy))
    i++;
  return i;
}


/* vilaine tentative pour 'flusher' les mouvements de souris consécutifs (cad 
   ne se situant pas de part et d'autre d'un evenement du style mouse-clic)

   hypothèse: XCheckIfEvent parcourt la file depuis l'ev le + vieux vers le + récent
   (ça semble naturel)

   ça a l'air de bien marcher :)
*/

int fcmmp_state; /* Mal !! */
Bool
flush_consecutive_mouse_motions_predicate(Display *display UNUSED, XEvent *ev, XPointer pw)
{
  //  printf("-- test: ev->type = %d\n", ev->type);
  if (fcmmp_state == 1) return False;
  else if (ev->type == MotionNotify && ev->xmotion.window == *((Window*)pw)) return True;
  else if (ev->type == ButtonPress || ev->type == ButtonRelease || ev->type == LeaveNotify || ev->type == EnterNotify) { fcmmp_state = 1; return False; }
  return False;
}

XEvent *
flush_consecutive_mouse_motions(Dock *dock, Window w, XEvent *ev_init) {
  static XEvent evtmp, ev; /* static == c mal (pas reentrant) */
  int cnt = 0;

  //  printf("debut\n");
  fcmmp_state = 0;
  while (XCheckIfEvent(dock->display, &evtmp, flush_consecutive_mouse_motions_predicate, (XPointer)&w)) {
    ev = evtmp; cnt++;
    assert(ev.type == MotionNotify);
    //    printf("** trouvé: x=%d, y=%d\n", ev.xmotion.x, ev.xmotion.y);
    fcmmp_state = 0;
  }
  //  printf("fin\n");

  if (cnt) return &ev; else return ev_init;
}

/*
int
has_pending_expose_events(Display *dpy, Window w) {
}
*/

/* 
   pour la selection (reperer quel mots sont à tel position, et quel caractère précisement.. )
*/
int
pp_selection_find_pos(Dock *dock, PostWord *first_pw, int x, PostWord **sel_pw, int *sel_pw_char_num)
{
  Pinnipede *pp = dock->pinnipede;
  PostWord *pw = first_pw;
  int x_sel;

  if (sel_pw) *sel_pw = NULL;
  if (sel_pw_char_num) *sel_pw_char_num = 0;
  x_sel = 0;

  if (first_pw == NULL) return 0;

  /* sans commentaires c'est mieux */
  while (pw && pw->ligne == first_pw->ligne) {
    x_sel = (pw == first_pw) ? 0 : pw->xpos;
    if (x < pw->xpos) 
      break;
    if (x < pw->xpos + pw->xwidth) {
      int i, len;
      XFontStruct *fn;

      fn = pv_get_font(pp, pw->attr);
      len = strlen(pw->w);
      for (i = 1; i <= len; i++) {
	int xx;
	xx = pw->xpos+XTextWidth(fn, pw->w, i);
	if (x < xx) {
	  break;
	}
	x_sel = xx;
      }
      if (sel_pw_char_num) *sel_pw_char_num = i-1;
      break;
    }
    if (pw->next==NULL || 
	pw->next->ligne != first_pw->ligne || 
	x < pw->next->xpos) {
      x_sel = pw->xpos+pw->xwidth;
      if (sel_pw_char_num) *sel_pw_char_num = strlen(pw->w);
      break;
    }
    pw = pw->next;
  }
  if (sel_pw) *sel_pw = pw;
  return x_sel;
}

/* affichage de la zone de selection (on ne réaffiche que ce qui est nécessaire) */
void
pp_selection_refresh(Dock *dock)
{
  Pinnipede *pp = dock->pinnipede;

  int l, l0, l1;
  int mouse_x0, mouse_x1;



  /* début de selection ? */
  if (pp->lignes_sel == NULL) {
    ALLOC_VEC(pp->lignes_sel, pp->nb_lignes, PinnipedeLignesSel);
  }

  l0 = pp->sel_anchor_y; l0 = MAX(l0, LINEY0(0)) - LINEY0(0);
  l0 = l0 / (LINEY0(1)-LINEY0(0));
  l0 = MIN(l0, pp->nb_lignes-1);
  

  l1 = pp->sel_head_y; l1 = MAX(l1, LINEY0(0)) - LINEY0(0);
  l1 = l1 / pp->fn_h;//(LINEY0(1)-LINEY0(0));
  l1 = MIN(l1, pp->nb_lignes-1);
  
  if (l1 < l0) {
    pp->sel_l0 = l1; mouse_x0 = pp->sel_head_x;
    pp->sel_l1 = l0; mouse_x1 = pp->sel_anchor_x;
  } else if (l0 < l1) {
    pp->sel_l0 = l0; mouse_x0 = pp->sel_anchor_x;
    pp->sel_l1 = l1; mouse_x1 = pp->sel_head_x;
  } else {
    pp->sel_l0 = pp->sel_l1 = l0;
    mouse_x0 = MIN(pp->sel_anchor_x, pp->sel_head_x);
    mouse_x1 = MAX(pp->sel_anchor_x, pp->sel_head_x);
  }

  BLAHBLAH(2,myprintf("%<YEL selection refresh: [%d:%d] -- [%d:%d] l0=%d, l1=%d>\n", 
		      pp->sel_anchor_x, pp->sel_anchor_y, pp->sel_head_x, pp->sel_head_y,pp->sel_l0,pp->sel_l1));
  
  for (l=0; l < pp->nb_lignes; l++) {
    int sel_x0, sel_x1;
    PostWord *first, *last;
    int first_pos, last_pos;

    sel_x0 = sel_x1 = 0;
    first_pos = last_pos = 0;
    first = last = NULL;

    if (l>=pp->sel_l0 && l<=pp->sel_l1 && pp->lignes[l]) {
      first = pp->lignes[l];
      sel_x0 = 0; //first->xpos;
      last = first; 
      while (last->next && last->next->ligne == last->ligne) last = last->next;
      sel_x1 = last->xpos+last->xwidth;
      first_pos = 0;
      last_pos = strlen(last->w);
    }
    if (l==pp->sel_l0) {
      sel_x0 = pp_selection_find_pos(dock, pp->lignes[l], mouse_x0, &first, &first_pos);
    }
    if (l==pp->sel_l1) {
      sel_x1 = pp_selection_find_pos(dock, pp->lignes[l], mouse_x1, &last, &last_pos);
    }

    /* marque les lignes qui étaient selectionnes pour qu'elles soient redessinnées */
    if (pp->lignes_sel[l].x0 != sel_x0 || pp->lignes_sel[l].x1 != sel_x1) {
      pp->lignes_sel[l].trashed = 1;
      pp->lignes_sel[l].x0 = sel_x0;              pp->lignes_sel[l].x1 = sel_x1;
      pp->lignes_sel[l].first_pw = first;         pp->lignes_sel[l].last_pw = last;
      pp->lignes_sel[l].first_pw_pos = first_pos; pp->lignes_sel[l].last_pw_pos = last_pos;
    } else {
      pp->lignes_sel[l].trashed = 0;
    }
    BLAHBLAH(3,printf("l=%2d, %d first=%s [%d], last=%s [%d] x0=%d, x1=%d\n", l, 
		      pp->lignes_sel[l].trashed, first ? first->w : NULL, first_pos,
		      last ? last->w : NULL, last_pos, 
		      pp->lignes_sel[l].x0, pp->lignes_sel[l].x1 = sel_x1));
  }
  for (l=0; l < pp->nb_lignes; l++) {

    if (pp->lignes_sel[l].trashed) {
    
      pp_draw_line(dock, pp->lpix, pp->lignes[l], pp->win_bgpixel, &pp->lignes_sel[l]);
      XCopyArea(dock->display, pp->lpix, pp->win, dock->NormalGC, 0, 0, pp->win_width - (pp->sc ? SC_W-1 : 0), pp->fn_h, 0, LINEY0(l));
    }
  }
}


/* copie la zone selectionnée dans un buffer et renvoie la
   taille de cette zone (on l'appelle une premiere fois pour connaitre la taille,
   puis une autre pour faire la copie effective)
*/
int
pp_selection_copy(Dock *dock, char *buff)
{
  Pinnipede *pp = dock->pinnipede;
  int l, nc;

  
  if (pp->lignes_sel == NULL) return 0;

  nc = 0;
  for (l=pp->sel_l0; l <= pp->sel_l1; l++) {
    PostWord *pw;
    pw = pp->lignes_sel[l].first_pw;
    while (pw) {
      int i, i0, i1;
      i0 = 0; i1 = strlen(pw->w);
      if (pw == pp->lignes_sel[l].first_pw) {
	i0 = pp->lignes_sel[l].first_pw_pos;
      } else if (pw == pp->lignes_sel[l].last_pw) {
	i1 = pp->lignes_sel[l].last_pw_pos;
      }
      if (pw != pp->lignes_sel[l].first_pw && (pw->attr & PWATTR_HAS_INITIAL_SPACE)) {
	if (buff) buff[nc] = ' ';
	nc++;
      }
      for (i=i0; i < i1; i++) {
	if (buff) buff[nc] = pw->w[i];
	nc++;
      }
      if (pw == pp->lignes_sel[l].last_pw) break;
      pw = pw->next;
    }
    if (l < pp->sel_l1 && nc) {
      if (buff) buff[nc] = (pw == NULL || pw->next == NULL) ? '\n' : ' ';
      nc++;
    }
  }
  if (buff) buff[nc] = 0;
  nc++;
  return nc;
}


/* lecture de la scrollbar, avec un refresh legerement differé pour éviter de trop charger... */
void
pp_check_scroll_pos(Dock *dock, DLFP_tribune *trib)
{
  Pinnipede *pp = dock->pinnipede;
  
  static int refresh_requested = 0;
  static int refresh_nb_delayed = 0;
  int new_pos;
 
  if (pp->sc == NULL) return;
  if (scrollcoin_read_requested_pos(pp->sc, &new_pos)) {
    int id;
    /* petit bricolage pas beau pour que ça affiche completement le premier post: 
       update_content rescrollera juste ce qu'il faut */
    if (new_pos == scrollcoin_get_vmin(pp->sc)) new_pos--;

    id = get_nth_id(trib, &pp->filter, new_pos);
    //    myprintf("scroll pos = %<MAG %d>, --> id_base = %d\n", new_pos, id);
    

    pp_update_content(dock, trib, id, 0,0,0);
    refresh_requested = 2;
  }
  if (refresh_requested == 1 || refresh_nb_delayed >= 0) {
    pp_refresh(dock, trib, pp->win, NULL);
    refresh_nb_delayed = 0; 
    refresh_requested = 0;
  } else if (refresh_requested > 0) {
    refresh_requested--; refresh_nb_delayed++;
  }
}

void
pp_dispatch_event(Dock *dock, DLFP_tribune *trib, XEvent *event)
{
  Pinnipede *pp = dock->pinnipede;
  static int old_mouse_x = -1, old_mouse_y = -1;
  static int mouse_button_press_x = -1, mouse_button_press_y = -1;
  
  static int dragging = 0;
  static Time time_drag = 0;

  /* le pinnipede ne fait RIEN quand la tribune est en cours de mise à jour ... */
  if (flag_updating_tribune) return;

  switch (event->type) {
  case DestroyNotify: 
    {
      //      printf("destroy?\n");
    } break;
  case ButtonPress:
    {
      pp_selection_unselect(pp);
      if (pp->sc && scrollcoin_handle_button_press(pp->sc, &event->xbutton, pp->win)) {
      } else if (pp->use_minibar && pp_minib_handle_button_press(dock, &event->xbutton)) {
      } else {
	mouse_button_press_x = old_mouse_x = event->xbutton.x;
	mouse_button_press_y = old_mouse_y = event->xbutton.y;
	dragging = 0;
	time_drag = event->xbutton.time;
      }
    } break;
  case ButtonRelease:
    {
      old_mouse_x = event->xbutton.x;
      old_mouse_y = event->xbutton.y;

      if (pp->lignes_sel) {
	int blen;
	
	blen = pp_selection_copy(dock, NULL);
	if (blen) {
	  char *buff;
	  
	  buff = malloc(blen); assert(buff);
	  pp_selection_copy(dock, buff);
	  editw_cb_copy(dock, pp->win, buff, blen-1);
	  free(buff);
	}
      } else {
	if (pp->sc && scrollcoin_handle_button_release(pp->sc, &event->xbutton, pp->win)) {
	  pp_check_scroll_pos(dock,trib);
	} else if (pp->use_minibar && pp_minib_handle_button_release(dock, trib, &event->xbutton)) {
	  //printf("plop\n");
	} else if (dragging == 0) {
	  pp_handle_button_release(dock, trib, &event->xbutton);
	}
      }
      dragging = 0;
      time_drag = event->xbutton.time;
    } break;
  case MotionNotify:
    {
      event = flush_consecutive_mouse_motions(dock, pp->win, event);

      if (pp->sc && scrollcoin_handle_motion(pp->sc, &event->xmotion, pp->win)) {
	pp_check_scroll_pos(dock,trib);
      } else if (pp_minib_pressed_button(dock)) {
	
      } else if (event->xmotion.state & Button1Mask && 
	  SQR(mouse_button_press_x - event->xbutton.x)+
	  SQR(mouse_button_press_y - event->xbutton.y) >= 6) {
	if (pp->lignes_sel == NULL) {
	  pp->sel_anchor_x = old_mouse_x; pp->sel_anchor_y = old_mouse_y;
	  pp_refresh(dock, trib, pp->win, NULL);
	}
	pp->sel_head_x = event->xmotion.x; pp->sel_head_y = event->xmotion.y;
	pp_selection_refresh(dock);
      } else if (event->xmotion.state & Button3Mask) {
	int decal_y;
	decal_y = event->xmotion.y - old_mouse_y;
	decal_y /= 8;
	if (decal_y && (event->xmotion.time-time_drag)>25) {
	  dragging = 1;
	  //	  printf("move (%ld) %ld!\n", event->xmotion.time, time_drag);
	  pp_update_content(dock, trib, pp->id_base, pp->decal_base-decal_y,0,0);
	  pp_refresh(dock, trib, pp->win, NULL);
	  old_mouse_x = event->xmotion.x;
	  old_mouse_y = event->xmotion.y;
	  time_drag = event->xmotion.time;
	}
      } else {
	pp_check_survol(dock, trib, event->xmotion.x, event->xmotion.y);
	old_mouse_x = event->xmotion.x;
	old_mouse_y = event->xmotion.y;
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
	
	/* eh oui, faut pas oublier ça.... */
	XFreePixmap(dock->display, pp->lpix);
	pp->lpix = XCreatePixmap(dock->display, pp->win, pp->win_width, MAX(MINIB_H, pp->fn_h), DefaultDepth(dock->display,dock->screennum));
	
	pp_minib_initialize(pp);
	pp_pv_destroy(pp);
	pp_update_content(dock, trib, pp->id_base, pp->decal_base, 0,1);
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
