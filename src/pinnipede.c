/*
  rcsid=$Id: pinnipede.c,v 1.67 2002/08/18 00:29:30 pouaite Exp $
  ChangeLog:
  $Log: pinnipede.c,v $
  Revision 1.67  2002/08/18 00:29:30  pouaite
  en travaux .. prière de porter le casque

  Revision 1.66  2002/08/17 18:33:39  pouaite
  grosse commition

  Revision 1.65  2002/06/26 22:19:49  pouaite
  ptit fix pour la tribune de f-cpu + patch de lordoric

  Revision 1.64  2002/06/23 10:44:05  pouaite
  i18n-isation of the coincoin(kwakkwak), thanks to the incredible jjb !

  Revision 1.63  2002/06/01 17:54:04  pouaite
  nettoyage

  Revision 1.62  2002/05/28 20:11:55  pouaite
  modif pr un pinnipede + fluide qd il y a bcp de messages stockés + tribune sur plusieurs jours

  Revision 1.61  2002/05/27 18:39:14  pouaite
  trucs du week-end + patch de binny

  Revision 1.60  2002/04/26 04:45:51  pouaite
  reconnaissance des horloges suivies de 3 pts de suspension

  Revision 1.59  2002/04/24 19:44:00  pouaite
  option pinnipede.use_AM_PM pour les horloges à l'anglaise comme sur http://woof.lu

  Revision 1.58  2002/04/23 23:31:02  pouaite
  je suis une buse

  Revision 1.57  2002/04/23 23:16:29  pouaite
  \o/ j'ai enfin réussi à chopper le bug mysterieux de l'autoscroll du pinni \o/

  Revision 1.56  2002/04/22 14:38:17  pouaite
  bugfix du bugfix du slip

  Revision 1.55  2002/04/22 13:03:03  pouaite
  un ptit bug tout naze dans le survol des horloges (fantomes)

  Revision 1.54  2002/04/15 19:56:38  pouaite
  v2.3.7a

  Revision 1.53  2002/04/14 23:24:22  pouaite
  re-fix pour kde ..

  Revision 1.52  2002/04/13 11:55:19  pouaite
  fix kde3 + deux trois conneries

  Revision 1.51  2002/04/12 12:49:56  pouaite
  *** empty log message ***

  Revision 1.50  2002/04/11 23:16:54  pouaite
  boitakon mega combo

  Revision 1.49  2002/04/11 10:15:28  pouaite
  fix compil

  Revision 1.48  2002/04/10 22:53:44  pouaite
  un commit et au lit

  Revision 1.47  2002/04/09 23:38:29  pouaite
  boitakon et son cortège de bugfixes

  Revision 1.46  2002/04/09 00:28:19  pouaite
  quelques modifs faites dans un état d'hébétude avancé /!\ travaux en cours /!\

  Revision 1.45  2002/04/03 20:15:11  pouaite
  plop

  Revision 1.44  2002/04/02 22:29:29  pouaite
  bugfixes transparence

  Revision 1.43  2002/04/01 22:56:03  pouaite
  la pseudo-transparence du pinni, bugfixes divers, option tribune.backend_type

  Revision 1.42  2002/04/01 01:39:38  pouaite
  grosse grosse commition (cf changelog)

  Revision 1.41  2002/03/28 00:06:15  pouaite
  le clic sur un login ouvre le palmipede en remplissant '/msg lelogin '

  Revision 1.40  2002/03/27 20:45:06  pouaite
  deuxième vague de bugfix

  Revision 1.39  2002/03/24 23:26:38  pouaite
  patch de lordoric + bricoles à deux francs

  Revision 1.38  2002/03/21 22:53:07  pouaite
  ajout d'une icone pour la fenetre du pinnipede et des news

  Revision 1.37  2002/03/19 09:55:58  pouaite
  bugfixes compilation

  Revision 1.36  2002/03/18 22:46:49  pouaite
  1 ou 2 bugfix mineurs, et surtout suppression de la dependance avec la libXpm .. un premier pas vers wmc² en 8bits

  Revision 1.35  2002/03/10 22:45:36  pouaite
  <mavie>dernier commit avant de passer la nuit dans le train</mavie> , spéciale dédicace à shift et à son patch ;)

  Revision 1.34  2002/03/10 16:07:10  pouaite
  pseudo transp basique dans le pinnipede (en cours..)

  Revision 1.33  2002/03/09 19:45:52  pouaite
  microbugfix du plopifieur et ajout d'une macro PATCH_LEVEL

  Revision 1.32  2002/03/09 00:25:12  pouaite
  coin coin

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

#include "pinnipede.h"

inline static int
pp_thread_filter_find_id(const struct _PinnipedeFilter *f, id_type id) {
  int i;
  for (i=0; i < f->nid; i++) {
    if (id_type_eq(f->id[i],id)) return 1;
  }
  return 0;
}

inline int
filter_msg_info(const board_msg_info *mi, const struct _PinnipedeFilter *filter)
{
  /* cas particulier: la boitakon */
  if (mi->in_boitakon && filter->filter_boitakon) return 0;

  if (filter->visible_sites[id_type_sid(mi->id)] == 0) return 0;
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
 (de maniere bourrine... c pas pour 250000 messages )
*/
static id_type
get_next_id_filtered(Boards *boards, id_type id, board_msg_info **nmi, struct _PinnipedeFilter *filter) 
{
  board_msg_info *mi;
  id_type nid;

  nid = id_type_invalid_id();
  mi = boards_find_id(boards, id);
  if (mi) {
    mi = mi->g_next;
    while (mi) {
      if (filter == NULL || filter_msg_info(mi,filter)) {
	nid = mi->id;
	break;
      }
      mi = mi->g_next;
    }
  }
  if (nmi) *nmi = mi;
  return nid;
}

static id_type
get_prev_id_filtered(Boards *boards, id_type id, board_msg_info **prev, const struct _PinnipedeFilter *filter) 
{
  board_msg_info *mi;

  mi = boards_find_id(boards, id);
  if (mi) {
    mi = mi->g_prev;
    while (mi) {
      if (filter == NULL || filter_msg_info(mi,filter)) {
	break;
      }
      mi = mi->g_prev;
    }
  }

  if (prev) *prev = mi;
  if (mi) {
    return mi->id;
  }
  else return id_type_invalid_id();
}

id_type
get_last_id_filtered(Boards *boards, struct _PinnipedeFilter *filter) {
  board_msg_info *mi;

  mi = boards->last;
  while (mi) {
    if (filter == NULL || filter_msg_info(mi,filter)) {
      return mi->id;
    }
    mi = mi->g_prev;
  }
  return id_type_invalid_id();
}

static id_type
get_first_id_filtered(Boards *boards, struct _PinnipedeFilter *filter) {
  board_msg_info *mi;

  mi = boards->first;
  while (mi) {
    if (filter == NULL || filter_msg_info(mi,filter)) {
      return mi->id;
    }
    mi = mi->g_next;
  }
  return id_type_invalid_id();
}

int
count_all_id_filtered(Boards *boards, struct _PinnipedeFilter *filter) {
  board_msg_info *mi;
  int nid;

  mi = boards->first;
  nid = 0;
  while (mi) {
    if (filter == NULL || filter_msg_info(mi,filter)) {
      nid++;
    }
    mi = mi->g_next;
  }
  return nid;
}

id_type
get_nth_id_filtered(Boards *boards, struct _PinnipedeFilter *filter, int n) {
  board_msg_info *mi, *last_mi;

  mi = boards->first; last_mi = NULL;
  while (mi && n > 0) {
    if (filter == NULL || filter_msg_info(mi,filter)) {
      n--;
      last_mi = mi;
    }
    mi = mi->g_next;
  }
  return (last_mi ? last_mi->id : id_type_invalid_id());
}

static int
get_id_count_filtered(Boards *boards, struct _PinnipedeFilter *filter, id_type id) {
  board_msg_info *mi;
  int cnt = 1;

  mi = boards->first;
  while (mi) {
    if (filter == NULL || filter_msg_info(mi,filter)) {
      if (id_type_eq(mi->id, id)) {
	return cnt;
      }
      cnt++;
    }
    mi = mi->g_next;
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
  unsigned hache_s = 0;
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
	   dest_pos < sz-1) { 
      s_dest[dest_pos++] = s_src[src_pos++]; 
    }
    if (s_src[src_pos]==0) break;
    if (dest_pos == sz-1) break;
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
      while (Prefs.plop_words[hache_s][i] && dest_pos < sz-1) {
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
pv_tmsgi_parse(Board *board, board_msg_info *mi, int with_seconds, int html_mode, int nick_mode, int troll_mode, int disable_plopify, KeyList *hk_plop, KeyList *hk_hili) {
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

  pv->is_skipped_id = board_find_id(board, mi->id.lid-1) ? 0 : 1;

  pv->is_hilight_key = 0;
  if (hk_hili) {
    pv->is_hilight_key = hk_hili->num+1; assert(hk_hili->num < NB_PP_KEYWORD_CATEG);
  }
  pv->is_plopified = 0;
  if (hk_plop) {
    pv->is_plopified = (disable_plopify ? 1 : hk_plop->num+2);
  }
    
  /*
  printf("pv = %p\n", pv);
  printf("  pv = %d\n", pv->id);
  printf("  pv->is_my_message = %d\n", pv->is_my_message);
  printf("  pv->is_answer = %d\n", pv->is_answer_to_me);
  printf("  pv->is_high = %d\n", pv->is_hilight_key);
  */

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

    /*

    popopo !! a reparer

    if (mi->tatouage) {
      p = mi->tatouage->name;
      if (strcmp(mi->tatouage->name, "?") == 0) {
    */
	make_short_name_from_ua(mi->useragent, short_ua, SUA_SZ);
	p = short_ua;
	/*      }
    } else p = "[???]";
	*/

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

  if (pv->is_plopified == 3) p = "plop"; /* bienvenue dans le monde de la hard plopification */


  has_initial_space = 1;
  while (p) {
    add_word = 1;
    if (board_get_tok(&p,&np,s,PVTP_SZ, &has_initial_space) == NULL) { break; }

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
      } else if (strcasecmp(s,"\t<tt\t>")==0) {
	attr |= PWATTR_TT;
      } else if (strcasecmp(s,"\t</tt\t>")==0) {
	attr &= (~PWATTR_TT);
      } else if (strncasecmp(s,"\t<a href=\"", 10)==0) {
	int i;
	char *url;
	attr |= PWATTR_LNK;
	i = strlen(s)-1; assert(i>0);
	while (s[i] != '\"' && i > 0) i--;
	s[i] = 0;
	url = s+10;
	if (url[0] == '.') { /* chemin relatif :-/ */
	  if (url[1] == '.') url+=2;
	  /* quick & ugly fix, ne marche pas quand le site n'est pas lesite/board mais
	     lesite/blah/blah/board .. pff */
	  snprintf(attr_s, PVTP_SZ, "http://%s%s", board->site->prefs->site_root, url);
	} else {
	  strncpy(attr_s, url, PVTP_SZ); 
	}
      } else if (strcasecmp(s,"\t</a\t>")==0) {
	attr &= (~PWATTR_LNK);
      } else {
	fprintf(stderr, "un tag qui pue ? '%s'\n", s);
	add_word = 1;
      }
    }
    if (add_word) {
      int is_ref;
      board_msg_info *ref_mi;

      ref_mi = check_for_horloge_ref(board->boards, mi->id, s,attr_s, PVTP_SZ, &is_ref, NULL);
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
	  if (strlen(s) >= 3 && Prefs.nb_plop_words > 0) {
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
  } else if (attr & (PWATTR_BD)) {
    fn = pp->fn_bd;
  } else if (attr & (PWATTR_TT)) {
    fn = pp->fn_minib;
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
pp_find_pv(Pinnipede *pp, id_type id)
{
  PostVisual *pv;

  pv = pp->pv;
  while (pv) {
    if (id_type_eq(pv->id,id)) return pv;
    pv = pv->next;
  }
  return NULL;
}

/* ajout (si necessaire) du message 'id' dans la liste */
PostVisual *
pp_pv_add(Pinnipede *pp, Boards *boards, id_type id)
{
  PostVisual *pv;
  int with_seconds = 1;

  pv = pp_find_pv(pp, id);
  if (pv) {
    pv->ref_cnt++;
  } else {
    board_msg_info *mi;

    mi = boards_find_id(boards,id);
    if (mi == NULL) {
      return NULL;
    }

    if (pp->nosec_mode) {
      with_seconds = mi->hmsf[3];
    }

    pv = pv_tmsgi_parse(boards->btab[mi->id.sid], mi, with_seconds, 1, 
			pp->nick_mode, pp->trollscore_mode, pp->disable_plopify,
			board_key_list_test_mi(boards, mi, Prefs.plopify_key_list),
			board_key_list_test_mi(boards, mi, Prefs.hilight_key_list)); 
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

void
pp_pv_destroy(Pinnipede *pp) {
  PostVisual *pv;

  pv = pp->pv;  
  while (pv) { pv->ref_cnt = 0; pv = pv->next; }
  pp_pv_garbage_collect(pp);
  if (pp->lignes) { free(pp->lignes); pp->lignes = NULL; }
}


void
pp_selection_unselect(Pinnipede *pp) {
  if (pp->lignes_sel) free(pp->lignes_sel);
  pp->lignes_sel = NULL;
  pp->sel_l0 = pp->sel_l1 = 0;
  pp->sel_anchor_x = pp->sel_head_x = 0;
  pp->sel_anchor_y = pp->sel_head_y = 0;
}

unsigned long
pp_get_win_bgcolor(Dock *dock) {
  Pinnipede *pp = dock->pinnipede;

  if (pp->active_tab) {
    int sid = pp->active_tab->site->site_id; assert(sid>=0);
    return pp->win_bgpixel[sid];
  } else return WhitePixel(dock->display, dock->screennum);
}



/* mise à jour du contenu du pinnipede (reparse tous les messages affichés etc...
   c'est une des fonctions les plus importantes) */
/* adjust: param interne, appeler toujours avec adjust  = 0 */
void
pp_update_content(Dock *dock, id_type id_base, int decal, int adjust, int update_scrollbar_bounds)
{
  Pinnipede *pp = dock->pinnipede;
  Boards *boards = dock->sites->boards;

  int cur_lig, plig;
  id_type id;
  PostVisual *pv;

  if (flag_updating_board) return;


  if (pp->lignes_sel) {
    pp_selection_unselect(pp);
  }
  if (pp->last_selected_text) {
    free(pp->last_selected_text); pp->last_selected_text = NULL;
  }

  pp_update_fortune(dock);

  

  //  printf("[colle_en_bas=%d] ..", pp->colle_en_bas);
  if (id_type_is_invalid(id_base) ||
      id_type_eq(id_base, get_last_id_filtered(boards, &pp->filter))) {
    pp->colle_en_bas = 1;
  } else pp->colle_en_bas = 0;


  pp->last_post_id = boards_last_id(boards);

  /* pour determiner la position de la zone d'affichage */
  pp_widgets_set_pos(dock);

/*   printf("id_base = %d/%d, pp->id_base=%d, last_id=%d, colle_en_bas=%d decal=%d\n", */
/*   	 id_base.sid,id_base.lid, pp->id_base.lid,  */
/* 	 get_last_id_filtered(boards, &pp->filter).lid, pp->colle_en_bas, decal); */

  pp->nb_lignes = pp->zmsg_h / pp->fn_h;
  cur_lig = pp->nb_lignes;

  /* on s'aligne sur le premier message qui n'est pas dans la boitakon */
  if (pp->disable_plopify == 0) {
    board_msg_info *mi;
    mi = boards_find_id(boards, id_base);
    while (mi && mi->in_boitakon) {
      decal = 0;
      id_base = get_prev_id_filtered(boards, id_base, &mi, &pp->filter);
    }
  }

  id = id_base;


  /* 'scroll down' */
  //printf("entree update_content: id_base = %d (%d), decal = %d (%d), adjust=%d\n",
  //id_base,pp->id_base, decal, pp->decal_base, adjust);

  pp->id_base = id_type_invalid_id();

  /* 'reduit' le decalage */
  while (decal > 0) {
    id_type nid;
    nid = get_next_id_filtered(boards, id, NULL, &pp->filter);
    if (id_type_is_invalid(nid)) { decal = 0; break; }
    id = nid;
    pv = pp_pv_add(pp, boards, id);
    if (pv == NULL) { decal = 0; break; }
    decal -= pv->nblig;
  }

  /* assignation des lignes */
  pp_pv_uncount(pp);
  if (pp->lignes) free(pp->lignes);
  ALLOC_VEC(pp->lignes, pp->nb_lignes, PostWord*);

  while (cur_lig >= 0) {
    pv = pp_pv_add(pp, boards, id); 
    //    printf("cur_lig = %d, id=%d pv=%p\n", cur_lig, id.lid, pv);
    if (pv == NULL) { /* on n'arrive pas a remplir jusqu'en haut ? */
      if (adjust == 0) {
	if (!id_type_is_invalid(pp->id_base)) {
	  pp_update_content(dock,pp->id_base,pp->decal_base+cur_lig, 1, update_scrollbar_bounds); /* pas joli-joli */
	} else {
	  id = get_first_id_filtered(boards, &pp->filter); /* premier id affichable */

	  if (!id_type_is_invalid(id)) { /* sinon ça veut dire que la tribune est comptelemt vide */
	    pp_update_content(dock,id,0, 0, update_scrollbar_bounds); /* pas joli-joli */
	  }
	}
      }
      break;
    }
    pv->ref_cnt = 0;
    id = get_prev_id_filtered(boards, id, NULL, &pp->filter);

    //printf("pv->nblig=%d, , pid=%d\n",pv->nblig, id);
    if (decal + pv->nblig > 0) { // test si le 'scroll up a été suffisant
      PostWord *pw;

      if (id_type_is_invalid(pp->id_base)) { /* on sauve l'id_base et le decal soigneusement calculé */
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
    if (update_scrollbar_bounds) { pp_scrollcoin_update_bounds(dock); }
/*     else { pp_scrollcoin_move_resize(dock); } */
     scrollcoin_setpos(pp->sc, get_id_count_filtered(boards, &pp->filter, pp->id_base)); 
  } 
}


void
pp_clear_win_area(Dock *dock, int x, int y, int w, int h)
{
  Pinnipede *pp = dock->pinnipede;
  if (pp->transparency_mode == 0 && w > 0 && h > 0) {
    //    XClearArea(dock->display, pp->win, x, y, w, h, False);
    XSetForeground(dock->display, dock->NormalGC, pp_get_win_bgcolor(dock));
    XFillRectangle(dock->display, pp->win, dock->NormalGC, x, y, w, h);
  } else {
    XCopyArea(dock->display, pp->bg_pixmap, pp->win, dock->NormalGC, x, y, 
	      w, h, x, y);
  }
}

/* dessine une ligne */
PostWord *
pp_draw_line(Dock *dock, Pixmap lpix, PostWord *pw, 
	     unsigned long bgpixel, PinnipedeLignesSel *sel_info, int use_bg_pixmap, int dest_y)
{
  Pinnipede *pp = dock->pinnipede;
  int pl;
  int old_pos;
  unsigned long pixel, old_pixel;
  int y;
  int site_num = -1;

  if (pw) {
    assert(pw->parent);
    site_num = id_type_sid(pw->parent->id);
  } else {
    assert(pp->active_tab);
    site_num = pp->active_tab->site->site_id;
  }
  assert(site_num >=0 && site_num < MAX_SITES);

  XSetForeground(dock->display, dock->NormalGC, bgpixel);
  if (use_bg_pixmap == 0) {
    XFillRectangle(dock->display, lpix, dock->NormalGC, 0, 0, pp->win_width, pp->fn_h);
  } else {
    assert(pp->bg_pixmap != None);
    XCopyArea(dock->display, pp->bg_pixmap, pp->lpix, dock->NormalGC, 0, dest_y, pp->win_width - (pp->sc ? SC_W-1 : 0), pp->fn_h, 0, 0);
  }

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
  old_pixel = bgpixel;

  pixel = 0L;
  y = pp->fn_base->ascent;
  old_pos = 0;
  if (pw) {
    XFontStruct *fn;
    XFontStruct *prev_font;
    
    prev_font = NULL;
    pl = pw->ligne;
    /*
      printf("pw->parent = %d\n", pw->parent->id);
      printf("pw->parent->is_my_message = %d\n", pw->parent->is_my_message);
      printf("pw->parent->is_answer = %d\n", pw->parent->is_answer_to_me);
      printf("pw->parent->is_high = %d\n", pw->parent->is_hilight_key);
    */
    
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
	pixel = pp->hilight_keyword_pixel[pw->parent->is_hilight_key-1];
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
	pixel = pp->timestamp_pixel[site_num];
//	if (pw->parent->is_my_message) { pixel = IRGB2PIXEL(0x000080); }
      } else if (pw->attr & (PWATTR_LNK|PWATTR_REF)) {
	pixel = pp->lnk_pixel[site_num];
      } else if (pw->attr & PWATTR_NICK) {
	pixel = pp->nick_pixel[site_num];
      } else if (pw->attr & PWATTR_LOGIN) {
	pixel = pp->login_pixel[site_num];
      } else if (pw->attr & PWATTR_TROLLSCORE) {
	pixel = pp->trollscore_pixel[site_num];
      } else {
	pixel = pp->txt_pixel[site_num];
      }

      if (pw->parent->is_plopified) {
	pixel = pp->plopify_pixel;
      }

      if (pw->attr & (PWATTR_TMP_EMPH)) {
	XSetForeground(dock->display, dock->NormalGC, pp->popup_bgpixel);
	XFillRectangle(dock->display, lpix, dock->NormalGC,pw->xpos, 1, pw->xwidth, pp->fn_h-1);
	pixel = pp->popup_fgpixel;
	XSetForeground(dock->display, dock->NormalGC, pixel);
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
	int x1;
	pixel = pp->strike_pixel[site_num]; 
	XSetForeground(dock->display, dock->NormalGC, pixel); old_pixel = pixel;
	if (pw->next && pw->next->ligne == pw->ligne && (pw->next->attr & PWATTR_S)) {
	  x1 = pw->next->xpos;
	} else {
	  x1 = pw->xpos+pw->xwidth-1;
	}
	XDrawLine(dock->display, lpix, dock->NormalGC, pw->xpos, y-fn->ascent/2+fn->descent/2, x1, y-fn->ascent/2+fn->descent/2);
      }
      pw->attr &= ~PWATTR_TMP_EMPH;
      old_pos = pw->xpos + pw->xwidth;
      pw = pw->next;
    }
  }


  return pw;
}

void pp_refresh_hilight_refs(Pinnipede *pp, Boards *boards, int sid, time_t timestamp, int sub_timestamp) {
  int l;

  for (l=0; l < pp->nb_lignes; l++) {
    if (pp->lignes[l]) {
      board_msg_info *ref2_mi;
      int pl;
      PostWord *pw;

      pw = pp->lignes[l];
      pl = pw->ligne;
      while (pw && pl == pw->ligne) {
	if (pw->attr & PWATTR_REF) {
	  int bidon, ref2_num;

	  ref2_mi = check_for_horloge_ref(boards, pw->parent->id, pw->w, NULL, 0, &bidon, &ref2_num); assert(bidon);
	  if (ref2_mi && ref2_mi->timestamp == timestamp) { /* test sur timestamp pour les situation où +sieurs msg ont le même */
	    if (id_type_sid(ref2_mi->id) == sid) {
	      if (ref2_num == -1                                 /* ref à plusieurs posts */
		  || (ref2_num==0 && sub_timestamp <= 0) /* au cas on a mis un ¹ inutile par inadvertance */
		  || (ref2_num == sub_timestamp)) {
		pw->attr |= PWATTR_TMP_EMPH;
	      }
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
pp_refresh(Dock *dock, Drawable d, PostWord *pw_ref)
{
  Pinnipede *pp = dock->pinnipede;
  Boards *boards = dock->sites->boards;

  int l;
  board_msg_info *ref_mi, *caller_mi;
  unsigned char ref_comment[200];
  int ref_in_window = 0; /* mis a 1 si le message souligné par pw_ref est affiché parmi
			    les autres messages. sinon, on l'affiche en haut, dans une petite fenetre */

#define MAXANTIREF 60
  id_type anti_ref_id[MAXANTIREF];
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
    /* en haut */
    y = LINEY0(0);
    h = LINEY0(0)-pp->zmsg_y1;
    if (h>0) {
      pp_clear_win_area(dock, 0, y, pp->win_width-(pp->sc ? SC_W-1:0), h);
    }

    /* en bas */
    y = LINEY0(pp->nb_lignes);
    h = (pp->zmsg_y2 - y) + 1;
    if (h>0) {
      pp_clear_win_area(dock, 0, y, pp->win_width-(pp->sc ? SC_W-1:0), h);
    }
  }

  caller_mi = NULL;
  ref_mi = NULL;
  nb_anti_ref = 0;
  
  /* premier cas: on survole une reference */
  if (pw_ref && (pw_ref->attr & PWATTR_REF)) {
    int bidon;

    ref_mi = check_for_horloge_ref(boards, pw_ref->parent->id, pw_ref->w, ref_comment, 200, &bidon, &ref_num); assert(bidon);
    if (ref_mi) { 
      board_msg_info *mi;

      /* on verifie que la ref apparait *entierement* dans la fenetre 
	 -> on boucle pour les situation ou il y a plusieurs messages qui ont le meme timestamp 
       */
      mi = ref_mi;
      while (mi && mi->timestamp == ref_mi->timestamp && 
	     id_type_sid(mi->id) == id_type_sid(ref_mi->id)) {
	ref_in_window = 0;
	for (l=0; l < pp->nb_lignes; l++) {
	  if (pp->lignes[l]) {
	    if (id_type_eq(pp->lignes[l]->parent->id, ref_mi->id)) {
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
	get_next_id_filtered(boards, mi->id, &mi, NULL);
	if (ref_num != -1) break; /* si c'est pas une ref à un multipost sans précision, break (chuis pas clair) */
      }
	
      /* et maintenant on detecte toutes les autres references vers ce message pour les afficher
	 temporairement en gras (ça c vraiment pour faire le cakos)*/
      pp_refresh_hilight_refs(pp, boards, id_type_sid(ref_mi->id), 
			      ref_mi->timestamp, ref_mi->sub_timestamp);
    }
  } else if (pw_ref && (pw_ref->attr & PWATTR_TSTAMP)) {
    pp_refresh_hilight_refs(pp, boards, id_type_sid(pw_ref->parent->id), 
			    pw_ref->parent->tstamp, pw_ref->parent->sub_tstamp);
  }



  /* 
     affichage du contenu de la tribune 
  */

  for (l=0; l < pp->nb_lignes; l++) {
    PostWord *pw;
    unsigned long bgpixel;
    int opaque_bg;
    
    pw = pp->lignes[l];
    opaque_bg = 0;
    bgpixel = pp->win_bgpixel[pp->active_tab->site->site_id];
    
   if (pw) {
     int site_num;
     int i;
     
     site_num = id_type_sid(pw->parent->id);
     bgpixel = pp->win_bgpixel[site_num];
      
      /* if (pw->parent->is_answer_to_me) bgpixel = pp->answer_my_msg_bgpixel; */
      /*      if (pw->parent->is_my_message) bgpixel = pp->my_msg_bgpixel;*/
     
     if (ref_mi) {
       if (ref_num == -1) {
	 if (pw->parent->tstamp == ref_mi->timestamp && ref_in_window
	     && id_type_sid(pw->parent->id) == id_type_sid(ref_mi->id)) {
	   bgpixel = pp->emph_pixel; opaque_bg = 1;
	 }
       } else {
	 if (id_type_eq(pw->parent->id, ref_mi->id) && ref_in_window) {
	   bgpixel = pp->emph_pixel; opaque_bg = 1;
	 }
       }
     }
     for (i = 0; i < nb_anti_ref; i++) {
       if (id_type_eq(anti_ref_id[i], pw->parent->id)) {
	 bgpixel = pp->emph_pixel; opaque_bg = 1;
       }
     }
   }
   
    pp_draw_line(dock, pp->lpix, pw, bgpixel, NULL, 
		 pp->transparency_mode && !opaque_bg, LINEY0(l));

    XCopyArea(dock->display, pp->lpix, d, dock->NormalGC, 0, 0, pp->win_width - (pp->sc ? SC_W-1 : 0), pp->fn_h, 0, LINEY0(l));
  }

  if (pw_ref && ref_in_window == 0) {
    int y;
    board_msg_info *mi;
    
    y = 3;
    /* affichage de la reference tout en haut du pinnipede */

    /* 
       on boucle pour les situation ou il y a plusieurs messages qui ont le meme timestamp 
    */
    mi = ref_mi;
    while (mi && mi->timestamp == ref_mi->timestamp
	   && id_type_sid(mi->id) == id_type_sid(ref_mi->id)) {
      PostVisual *pv;
      if (mi->in_boitakon == 0 || pp->disable_plopify) {
	pv = pp_pv_add(pp, boards, mi->id);
	if (pv) {
	  PostWord *pw = pv->first;
	  while (pw) {
	    pw = pp_draw_line(dock, pp->lpix, pw, pp->emph_pixel, NULL, 0, y); 
	    XCopyArea(dock->display, pp->lpix, d, dock->NormalGC, 0, 0, pp->win_width, pp->fn_h, 0, y);
	    y += pp->fn_h;
	  }
	}
      }
      get_next_id_filtered(boards, mi->id, &mi, NULL);
      if (ref_num != -1) break;
    }

    /* affichage du commentaire (optionnel) */
    if (ref_mi || strlen(ref_comment)) {
      if (strlen(ref_comment) && (ref_mi==NULL || (ref_mi->in_boitakon && pp->disable_plopify == 0))) {
	XSetForeground(dock->display, dock->NormalGC, pp->emph_pixel); //WhitePixel(dock->display, dock->screennum));
	XFillRectangle(dock->display, pp->lpix, dock->NormalGC, 0, 0, pp->win_width, pp->fn_h);
	XSetBackground(dock->display, dock->NormalGC, pp->emph_pixel); //WhitePixel(dock->display, dock->screennum));
	XSetFont(dock->display, dock->NormalGC, pp->fn_it->fid);
	XSetForeground(dock->display, dock->NormalGC, pp->timestamp_pixel[pp->active_tab->site->site_id]);
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

  pp_widgets_refresh(dock);
}


/* appelée depuis wmcoincoin.c, pour gèrer l'autoscroll et rafraichir l'affichage */
void
pp_check_board_updated(Dock *dock)
{
  Pinnipede *pp = dock->pinnipede;
  Boards *boards = dock->sites->boards;

  if (pp && pp->mapped && flag_updating_board == 0) {
    /* test si on scrolle qutomatiquement pour afficher le nouveau message */
    //    if (trib->last_post_id != pp->last_post_id && pp->last_post_id == pp->id_base && pp->decal_base == 0) {
    if (pp->flag_board_updated) {
      id_type last_id;

      /* eh oui, il faut pas autoscroller ou rafraichir alors qu'une selection est active 
	 (le update_content deselectionne automatiquement, mais faut être sûr que
	 l'utilisateur n'est pas justement *en train* de selectionner)
       */
      if (pp->lignes_sel) {
	if (difftime(time(NULL),pp->time_sel) > 20.) { /* on accorde 20 sec. de delai */
	  pp_selection_unselect(pp);
	} else {
	  return;
	}
      }

      if (pp->sc) { 
	pp_scrollcoin_update_bounds(dock); 
      }      
      last_id = get_last_id_filtered(boards, &pp->filter);
      if ((!id_type_eq(last_id, pp->last_post_id)) && 
	  pp->colle_en_bas) { // && pp->decal_base == 0) {
	//	myprintf("pp_check_board_updated, on %<yel colle> de %d à %d\n", pp->last_post_id, trib->last_post_id);
	pp_update_content(dock, last_id, 0, 0, 0);
      } else {
	/*	if (trib->last_post_id != pp->last_post_id)
		printf("pp_check_board_updated, on laisse filer de %d à %d (pos=%d/%d)\n", pp->last_post_id, trib->last_post_id, pp->id_base, pp->decal_base);*/
	pp_update_content(dock, pp->id_base, pp->decal_base, 0, 0);
      }
      pp_refresh(dock, pp->win, NULL);
      pp->flag_board_updated = 0;
    }
  }
}

void
pp_animate(Dock *dock)
{
  Pinnipede *pp = dock->pinnipede;
  if (pp && pp->mapped /*&& flag_updating_board == 0*/) {
    /* pour affichage du temps restant avant refresh */
    pp_minib_refresh(dock);
    pp_tabs_refresh(dock);
  }
}

static int
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
    fprintf(stderr, _("XLoadQueryFont: failed loading font '%s'\n"), base_name);
    fprintf(stderr, _("Please choose another font.\n"));
    return -1;
  }

  /* police italique -> on cherche d'abord la police oblique */
  snprintf(ital_name, 512, "-*-%s-medium-o-*-*-%d-*-*-*-*-*-%s", fn_family, fn_size, Prefs.font_encoding);
  pp->fn_it = XLoadQueryFont(display, ital_name);
  if (!pp->fn_it) {
    /* puis la police italique */
    BLAHBLAH(1, fprintf(stderr, _("Slanted font '%s' not found -> we're looking for the italic font\n"), ital_name));
    snprintf(ital_name, 512, "-*-%s-medium-i-*-*-%d-*-*-*-*-*-%s", fn_family, fn_size, Prefs.font_encoding);
    pp->fn_it = XLoadQueryFont(display, ital_name);
    if (!pp->fn_it) {
      myfprintf(stderr, _("%<RED WARNING>: error while looking for the italic font: '%s'\n"), ital_name);
      myfprintf(stderr, _("We'll use the base font.\n"));

      /* pas de copie de pointer pour pas poser de pbs dans picohtml_destroy */
      pp->fn_it = XLoadQueryFont(display, base_name); assert(pp->fn_it);
    }
  }

  /* police bold */
  snprintf(bold_name, 512, "-*-%s-bold-r-*-*-%d-*-*-*-*-*-%s", fn_family, fn_size, Prefs.font_encoding);
  pp->fn_bd = XLoadQueryFont(display, bold_name);
  if (!pp->fn_bd) {
    myfprintf(stderr, _("%<RED WARNING>: error while looking for the bold font: '%s'\n"), bold_name);
    myfprintf(stderr, _("We'll use the base font.\n"));
    pp->fn_bd = XLoadQueryFont(display, base_name); assert(pp->fn_bd);
  }

  /* police bold oblique */
  snprintf(itbd_name, 512, "-*-%s-bold-o-*-*-%d-*-*-*-*-*-%s", fn_family, fn_size, Prefs.font_encoding);
  pp->fn_itbd = XLoadQueryFont(display, itbd_name);
  if (!pp->fn_itbd) {
    /* puis la police bold italique */
    BLAHBLAH(1, fprintf(stderr, _("Bold slanted font '%s' not found -> we're looking for the italic font\n"), itbd_name));
    snprintf(itbd_name, 512, "-*-%s-bold-i-*-*-%d-*-*-*-*-*-%s", fn_family, fn_size, Prefs.font_encoding);
    pp->fn_itbd = XLoadQueryFont(display, itbd_name);
    if (!pp->fn_itbd) {
      myfprintf(stderr, _("%<RED WARNING>: error while looking for the italic font: '%s'\n"), itbd_name);
      myfprintf(stderr, _("We'll use the base font.\n"));

      /* pas de copie de pointer pour pas poser de pbs dans picohtml_destroy */
      pp->fn_itbd = XLoadQueryFont(display, base_name); assert(pp->fn_itbd);
    }
  }
  pp->fn_h = pp->fn_base->ascent + pp->fn_base->descent+1;
  return 0;
}

#define GET_BICOLOR(x) (pp->transparency_mode ? IRGB2PIXEL(x.transp) : IRGB2PIXEL(x.opaque))

void
pp_set_prefs_colors(Dock *dock) 
{
  Pinnipede *pp = dock->pinnipede;
  int i;
  
  for (i=0; i <MAX_SITES; i++) {
    if (Prefs.site[i] == NULL) continue;
    pp->win_bgpixel[i] = IRGB2PIXEL(Prefs.site[i]->pp_bgcolor);
    pp->timestamp_pixel[i] = GET_BICOLOR(Prefs.site[i]->pp_tstamp_color);
    pp->lnk_pixel[i] = GET_BICOLOR(Prefs.site[i]->pp_url_color);
    pp->strike_pixel[i] = GET_BICOLOR(Prefs.site[i]->pp_strike_color);
    pp->txt_pixel[i] = GET_BICOLOR(Prefs.site[i]->pp_fgcolor);
    pp->nick_pixel[i] = GET_BICOLOR(Prefs.site[i]->pp_useragent_color);
    pp->login_pixel[i] = GET_BICOLOR(Prefs.site[i]->pp_login_color);
    pp->trollscore_pixel[i] = GET_BICOLOR(Prefs.site[i]->pp_trollscore_color);
  }
  pp->popup_fgpixel = GET_BICOLOR(Prefs.pp_popup_fgcolor);
  pp->popup_bgpixel = GET_BICOLOR(Prefs.pp_popup_bgcolor);
  pp->minib_pixel = GET_BICOLOR(Prefs.pp_buttonbar_bgcolor);
  pp->minib_dark_pixel = GET_BICOLOR(Prefs.pp_buttonbar_fgcolor);
  pp->minib_msgcnt_pixel = GET_BICOLOR(Prefs.pp_buttonbar_msgcnt_color);
  pp->minib_updlcnt_pixel = GET_BICOLOR(Prefs.pp_buttonbar_updlcnt_color);
  pp->minib_progress_bar_pixel = GET_BICOLOR(Prefs.pp_buttonbar_progressbar_color);
  pp->sel_bgpixel = GET_BICOLOR(Prefs.pp_sel_bgcolor);
  pp->emph_pixel = GET_BICOLOR(Prefs.pp_emph_color);
  pp->hilight_my_msg_pixel = GET_BICOLOR(Prefs.pp_my_msg_color);
  pp->hilight_answer_my_msg_pixel = GET_BICOLOR(Prefs.pp_answer_my_msg_color);
  for (i=0; i < NB_PP_KEYWORD_CATEG; i++) {
    pp->hilight_keyword_pixel[i] = GET_BICOLOR(Prefs.pp_keyword_color[i]);
  }
  pp->plopify_pixel = GET_BICOLOR(Prefs.pp_plopify_color);

  if (pp->sc) {
    scrollcoin_change_colors(pp->sc, pp->transparency_mode);
  }
}

void
pp_change_transparency_mode(Dock *dock, int on_off)
{
  Pinnipede *pp = dock->pinnipede;
  pp->transparency_mode = on_off;
  pp_set_prefs_colors(dock);
}

/* initialisation */
void
pp_build(Dock *dock) 
{
  Pinnipede *pp;

  ALLOC_OBJ(pp, Pinnipede);
  dock->pinnipede = pp;


  pp->mapped = 0;

  pp->bg_pixmap = None;
  pp_change_transparency_mode(dock, Prefs.pp_start_in_transparency_mode);

  pp->id_base = id_type_invalid_id(); pp->decal_base = 0;
  pp->colle_en_bas = 1;

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
  pp->last_selected_text = NULL;

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
  pp->filter.filter_boitakon = 1;

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
  if (pp_load_fonts(pp, dock->display, Prefs.pp_fn_family, Prefs.pp_fn_size)) {
    myprintf(_("Failed to load the '%s' fonts with size '%d'\nLet's try with helvetica/12.\n"),Prefs.pp_fn_family, Prefs.pp_fn_size);
    if (pp_load_fonts(pp, dock->display, "helvetica", 12)==-1) {
      myprintf(_("Uuuurg !! No helvetica, I shoot my nose.\n")); exit(-1);
    }
  }

  pp->use_minibar = Prefs.pp_minibar_on;

  //  if (pp->use_minibar)
  //    pp_minib_initialize(pp);
  pp_widgets_set_pos(dock);

  pp->fn_minib = dock->fixed_font;

  pp_tabs_build(dock);

  pp->flag_board_updated = 0;
}

void
pp_destroy(Dock *dock)
{
  Pinnipede *pp = dock->pinnipede;
  if (pp->mapped) pp_unmap(dock);
  assert(pp->pv == NULL); assert(pp->sc == NULL); 
  assert(pp->lignes_sel == NULL); assert(pp->lignes == NULL);
  picohtml_destroy(dock->display, pp->ph_fortune);
  XFreeFont(dock->display, pp->fn_base); XFreeFont(dock->display, pp->fn_it); 
  XFreeFont(dock->display, pp->fn_bd); XFreeFont(dock->display, pp->fn_itbd);
  pp_tabs_destroy(pp);
  free(pp); dock->pinnipede = NULL;
}



void
pp_update_bg_pixmap(Dock *dock)
{
  Pinnipede *pp = dock->pinnipede;
  assert(pp->win != None);

  if (pp->bg_pixmap != None) {
    XFreePixmap(dock->display, pp->bg_pixmap); pp->bg_pixmap = None;
  }
  if (pp->transparency_mode) {
    int xpos, ypos;
    get_window_pos_without_decor(dock->display, dock->rootwin, pp->win, &xpos, &ypos);
    //    printf("window pos: %d, %d (pp_xpos=%d, pp_ypos=%d)\n", xpos, ypos, pp->win_xpos, pp->win_ypos);
    pp->bg_pixmap = extract_root_pixmap_and_shade(dock->rgba_context,
						  xpos, ypos, 
						  pp->win_width, pp->win_height,
						  &Prefs.pp_transparency, 
						  Prefs.use_fake_real_transparency);
    if (pp->bg_pixmap == None) {
      myprintf(_("%<yel impossible to use the pseudo-transparency> (probable solution: relaunch wmsetbg or its equivalent)\n"));
      pp_change_transparency_mode(dock, 0);
    }
  }
}




/*
  un petit mot: j'ai enfin compris comment faire apparaitre cette fenetre
   ou je veux: il suffit de donner la position dans xcreatewindow
   et d'activer USPosition dans les WM_HINTS (MAIS, les valeurs indiquées dans
   les champs x et y n'ont aucune influence)
*/
void
pp_show(Dock *dock)
{
  Boards *boards = dock->sites->boards;
  XSetWindowAttributes wa;
  Pinnipede *pp = dock->pinnipede;
  int xpos, ypos;

  /*
  {
    board_msg_info *mi = boards->first;
    int cnt = 1;
    while (mi) {
      myprintf("%3d sid=%2d lid=%<YEL %5d> login=%.10s msg=%.60s\n", cnt++,
	       id_type_sid(mi->id), id_type_lid(mi->id), mi->login, mi->msg);
      mi = mi->g_next;
    }
  }
  */

  if (pp->active_tab == NULL) {
    msgbox_show(dock, "looks like you fucked your options file, no board ");
    return;
  }
  if (pp->win_xpos == -10000 && pp->win_ypos == -10000) {
    xpos = 700; ypos = 500; /* ça n'a d'effet que sur certain windowmanagers rustiques (genre pwm) */
  } else {
    xpos = pp->win_xpos;
    ypos = pp->win_ypos;
  }

  pp->win = XCreateSimpleWindow (dock->display, dock->rootwin, 
				 xpos, ypos, pp->win_width,pp->win_height, 0,
				 WhitePixel(dock->display, dock->screennum),
				 pp_get_win_bgcolor(dock));
  wa.event_mask =
    ButtonPressMask | 
    ButtonReleaseMask | 
    ExposureMask | 
    PointerMotionMask | 
    StructureNotifyMask |
    EnterWindowMask | 
    //    ResizeRedirectMask |
    
    LeaveWindowMask;

  /* ça sera a changer .. pour l'instant ça ira */
  //  if (pp->transparency_mode) {
  //    wa.override_redirect = True ;
  //  } else
    wa.override_redirect = False ;

  
  //wa.background_pixmap = ParentRelative;
  //wa.override_redirect = False ;
  XChangeWindowAttributes (dock->display, pp->win,
			   //CWBackPixmap | 
			   CWEventMask | CWOverrideRedirect, &wa);

  {
    XTextProperty window_title_property;
    char* window_title = NULL; // = "pinnipede teletype";
    char* icon_title = "pinnipede";
    XSizeHints* win_size_hints;
    int rc;
    //    XWMHints* win_hints;
    XClassHint *class_hint;
    XWMHints *wm_hint;
    char s[512];

    /* nom de la fenetre */
    window_title = str_printf("pinnipede teletype");
    rc = XStringListToTextProperty(&window_title,1, &window_title_property); assert(rc);
    XSetWMName(dock->display, pp->win, &window_title_property);
    XFree(window_title_property.value);

    /* nom de la fenetre iconifiée */
    rc = XStringListToTextProperty(&icon_title,1, &window_title_property); assert(rc);
    XSetWMIconName(dock->display, pp->win, &window_title_property);
    win_size_hints= XAllocSizeHints(); assert(win_size_hints);
    XFree(window_title_property.value);

    free(window_title); window_title = NULL;

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
    sprintf(s, "wmcoincoin");
    class_hint->res_class = s;
    XSetClassHint(dock->display, pp->win, class_hint);
    XFree(class_hint);

    wm_hint = XAllocWMHints(); assert(wm_hint);
    wm_hint->icon_pixmap = dock->wm_icon_pix;
    wm_hint->icon_mask = dock->wm_icon_mask;
    wm_hint->flags = IconPixmapHint | IconMaskHint;
    XSetWMHints(dock->display, pp->win, wm_hint);
    XFree(wm_hint);
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

  pp_update_bg_pixmap(dock);

  XMapRaised(dock->display, pp->win);


  assert(pp->sc == NULL);

  pp->sc = scrollcoin_create(1,1,1,pp->win_width-SC_W+1, 0, pp->win_height-20, pp->transparency_mode);

  //  XMoveWindow(dock->display, pp->win, 100, 100);
  pp->mapped = 1;
  pp_update_content(dock, pp->colle_en_bas ? 
		    get_last_id_filtered(boards, &pp->filter) : 
		    pp->id_base, 0, 0, 1);
    
  pp->survol_hash = 0;
}

static PostWord *
pp_get_pw_at_xy(Pinnipede *pp,int x, int y)
{
  PostWord *pw;
  int l;
  pw = NULL;
  if (y >= pp->zmsg_y1 && y <= pp->zmsg_y2) {
    int trouve = 0;
    for (l=0; l < pp->nb_lignes; l++) {
      pw = pp->lignes[l];
      
      while (pw && pw->ligne == pp->lignes[l]->ligne) {
	if (x >= pw->xpos && x < pw->xpos+pw->xwidth &&
	    y >= LINEY0(l) && y <= LINEY1(l)) 
	  {
	    trouve = 1;
	    break;
	  }
	pw = pw->next;
      }
      if (trouve) break;
    }
    if (trouve == 0) pw = NULL; /* raa le bug tout naze */
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
/* to do: comptage inter-sites */
static int
pp_count_backrefs(board_msg_info *base_mi)
{
  int nb_backrefs = 0;
  board_msg_info *mi;
  
  if (base_mi == NULL) return 0;

  /* on parcourt tous les message postérieurs à base_mi */
  mi = base_mi->next;
  while (mi) {
    int i;
    /* on regarde toutes ses references */
    for (i = 0; i < mi->nb_refs; i++) {
      board_msg_info *ref_mi;
      int j;

      /* pour chaque ref, on regarde la liste (generalement de taille 1 ou 0) des messages pointés */
      for (j = 0, ref_mi = mi->refs[i].mi; j < mi->refs[i].nbmi; j++, ref_mi=ref_mi->next) {
	/* on notera qu'on fait bien ref_mi->next et pas ref_mi->g_next */
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
pp_check_survol(Dock *dock, int x, int y)
{
  Pinnipede *pp = dock->pinnipede;
  Boards *boards = dock->sites->boards;
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
      board_msg_info *mi;
      char blah[1024];
      char *s;
      int blah_sz = 1024;

      int nrep;
      KeyList *hk;
      mi = boards_find_id(boards, pw->parent->id);

      s = blah; s[0] = 0;
      if (mi->is_my_message) {
	snprintf(s, blah_sz, _("\n[you posted this message]")); blah_sz -= strlen(s); s += strlen(s);
      }
      if (mi->is_answer_to_me && blah_sz>30) {
	snprintf(s, blah_sz, _("\n[this message answers to one of yours]")); blah_sz -= strlen(s); s += strlen(s);
      }
      hk = board_key_list_test_mi(boards, mi, Prefs.hilight_key_list);
      if (hk && blah_sz > 60) {
	snprintf(s, blah_sz, _("\nmessage 'boxed' because: ")); blah_sz -= strlen(s); s += strlen(s);
	while (hk && blah_sz > 30) {
	  snprintf(s, blah_sz, " {%s='%.20s'}", 
		   key_list_type_name(hk->type), hk->key); blah_sz -= strlen(s); s += strlen(s);
	  hk = board_key_list_test_mi(boards, mi, hk->next);
	}
      }
      hk = board_key_list_test_mi(boards, mi, Prefs.plopify_key_list);
      if (hk && blah_sz > 60) {
	snprintf(s, blah_sz, _("\nmessage plopified (level %d) because: "), hk->num); blah_sz -= strlen(s); s += strlen(s);
	while (hk && blah_sz > 30) {
	  snprintf(s, blah_sz, " {%s='%.20s'}", 
		   key_list_type_name(hk->type), hk->key); blah_sz -= strlen(s); s += strlen(s);
	  hk = board_key_list_test_mi(boards, mi, hk->next);
	}
      }
      nrep = pp_count_backrefs(mi);

      snprintf(survol, 1024, "[%s] id=%d ua=%s\n%d %s%s", 
	       Prefs.site[pw->parent->id.sid]->site_name,
	       pw->parent->id.lid, 
	       (mi ? mi->useragent : ""), 
	       nrep, (nrep > 1) ? _("answers") : _("answer"), blah);
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
      pp_refresh(dock, pp->win, is_a_ref ? pw : NULL);
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
  pp_refresh(dock, pp->win, NULL);

  /* on sauve la position de la fenetre (en prenant en compte les decorations du WM ) */
  get_window_pos_with_decor(dock->display, pp->win, &pp->win_xpos, &pp->win_ypos);
  //  pp_minib_hide(dock);
  XDestroyWindow(dock->display, pp->win);
  XFreePixmap(dock->display, pp->lpix); pp->lpix = None;
  if (pp->bg_pixmap != None) {
    XFreePixmap(dock->display, pp->bg_pixmap); pp->bg_pixmap = None;
  }
  pp->win = None;
  pp->mapped = 0;

  if (pp->sc) { scrollcoin_destroy(pp->sc); pp->sc = NULL; }
  if (pp->last_selected_text) { free(pp->last_selected_text); pp->last_selected_text = NULL; }

  pp_pv_destroy(pp);
}

int pp_ismapped(Dock *dock) {
  return dock->pinnipede->mapped;
}


/* --------------------------- section plateau de fruit de mer, veuillez insulter lordOric 
   pour tous les bugs liès à cette partie ;) ---------------------------------------------
*/

/* 23/03/2002 pitit patch pour protéger les > et < pour le validator */
char *
pp_boardshot_encode( const char *chaine )
{
  char *tmp;
  char *retour;

  const char *key[] = { "<", ">", "&" };
  const char *subst[] = { "&lt;", "&gt;", "&amp;" };
  const char *key2[] = { "\t&lt;", "\t&gt;" };
  const char *subst2[] = { "<", ">" };
  
  if (chaine == NULL) return NULL;

  /* Passage des <> en &lt; et &gt; */
  tmp = str_multi_substitute( chaine, key, subst, 3 );
  
  /* Restauration des balises '<' et '>' précédés d'une tabulation (cad ceux qui sont
     d'authentiques tags */
  retour = str_multi_substitute( tmp, key2, subst2, 2 );
  free(tmp);
  return retour;
}

/* --------- patch de lordOric, aka "plateau de fruits de mer" ------------*/
/* Scrinchote d'un message */
int 
pp_boardshot_save_msg( board_msg_info *mi, FILE *file )
{
  char time[10];
  
  char *tmp;
  
  assert(file); assert(mi);
	
  snprintf(time, 10, "%02d:%02d:%02d",mi->hmsf[0], mi->hmsf[1], mi->hmsf[2]);
	
  if ( mi->troll_score )
    fprintf( file, "<tr><td><FONT color=red><b>%d</b></FONT></td>\n",mi->troll_score);
  else
    fprintf( file, "<tr><td></td>\n");
	
  tmp = pp_boardshot_encode( mi->useragent );
  fprintf(file,"<td> <span title='%s'> %s </span> </td>\n",tmp, time);	 
  free (tmp);
	
  if ( mi->login && strlen(mi->login)) {
    tmp = pp_boardshot_encode( mi->login );
    fprintf(file,"<td align=CENTER><FONT color=red>%s</FONT></td>\n", tmp);
    free(tmp);
  } else {
    char *p;
    p = mi->tatoo.name;
    /*    char short_ua[15];
    if (mi->tatouage) {
      if (strcmp(mi->tatouage->name, "?") == 0) {
	make_short_name_from_ua(mi->useragent, short_ua, 15);
	p = short_ua;
      }
      } else p = "[???]";*/
    tmp = pp_boardshot_encode(p);
    fprintf( file, "<td align=CENTER><FONT color=brown>%.12s</FONT></td>\n", tmp);
    free(tmp);
  }
	
  tmp = pp_boardshot_encode(mi->msg);
  fprintf( file,"<td>%s</td></tr>\n", tmp);
  free(tmp);
  
  return 0;
}


/* Tribuneshot : un chouette plateau de fruits de mer ;) */
int
pp_boardshot_kikoooo(Dock *dock ) 
{
  Boards *boards = dock->sites->boards;
  char *file_name;
  FILE *file;
  time_t time_shot;
  int file_exist;
  board_msg_info *msg = boards->first;
  
  file_name = str_substitute(Prefs.board_scrinechote, "~", getenv("HOME"));
  file = fopen( file_name, "r");
  file_exist = (file!=NULL);
  if ( file_exist ) fclose( file ); 
  
  file = fopen( file_name, "a");
  if ( ! file ) {
    char errmsg[512];
    snprintf(errmsg, 512, _("Error while opening %s\n"), file_name);
    msgbox_show(dock, errmsg);
    free(file_name);
    return -1;
  }
  
  if ( ! file_exist ) {
    fprintf( file, 
	     "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.01 Transitional//EN\">"
	     "<html><head><title>Scrinechote tribune</title>"
	     "<meta http-equiv=\"Content-Type\" content=\"text/html; charset=iso-8859-15\">"
	     "</head>");
    fprintf( file, "<body>");
  }
  
  time( &time_shot );
  fprintf( file, "<br><br><center><h2> *** Scrinechote - %s *** </h2></center><br>", ctime( &time_shot ) );
  fprintf( file, "<table align=CENTER>");
  
  while ( msg ) {
    pp_boardshot_save_msg(msg, file);
    msg = msg->g_next;
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
	       _("<p align=center> Welcome to the <b><font color=#008000>Pinnipede Teletype</font></b></p>"
	       "This window was specially designed by the greatest experts to offer you "
	       "optimal mouling conditions.<br>"
	       "Here is a summary of its functionalities:<br><br>"
	       "<b>To scroll</b>, use the mouse wheel, or 'drag' "
	       "while clicking with the right button. "
	       "Scrolling is automatic when a new message appears.<br><br>"
	       "To bring the <b>button bar</b> or make it disappear, use the right clic. "
	       "The two first buttons allow "
	       "to scroll, faster or slower.<br><br>"
	       "The available actions on the <b>clock</b> near each message are:<br>"
	       "<font color=blue>Left Click</font><tab>: opens the palmipede editor, and inserts a reference to the message<br>"
	       "<font color=blue>Middle Click</font><tab>: copies the contents of the message to the clipboard<br>"
	       "<font color=blue>Right Click</font><tab>: copies the useragent to the clipboard<br><br>"
	       "If you click on an <b>[url]</b>, the result will be:<br>"
	       "<font color=blue>Left Click</font><tab>: opens the url in the external browser (if it has been "
	       "defined in the ~/.wmcoincoin/options file). <b>Warning</b>, even if precautions have been taken"
	       "since the inominious wmcoincoin 2.0, this kind of things is generally considered as a security weakness...<br>"
	       "<font color=blue>Middle Click</font><tab>: opens the url with the second browser (the http.browser2 option)<br>"
	       "<font color=blue>Right Click</font><tab>: copies the url to the clipboard<br><br>"
	       "When the pointer is over a <b>reference to a previous post</b>, it will be underlined. If you click:<br>"
	       "<font color=blue>Left Click</font><tab>: brings the referenced message<br>"
	       "<font color=blue>Right Click</font><tab>: copies the reference to the clipboard (okay, it's not very useful...)<br><br>"
	       "Some basic filtering is available with the help of <font color=blue>Ctrl+Left Click</font> "
	       "on a word, login, useragent... Use the blue button to cancel.<br> If you want to emphasize the messages of a personne, or those containing a word, <font color=blue>Shift+Left Click</font> will be your friend<br>"
	       "<b>New:</b> someone is annoying you ? You have launched a troll that you don't control ? Then plopify the evil with a <font color=blue>Shift+Right Click</font> on his login/useragent<br><br>"
	       "You can take a 'shot' of the board (the so-called seafood tray), with <font color=blue>Ctrl+Middle Click</font><br><br>"
	       "To understand the display of the <b>useragents</b> activated by the dark red button (about fifteen pixels on your left), you can see "
	       "the <tt>~/.wmcoincoin/useragents</tt><br> file (hint: the button has 5 different positions)<br><br>"
	       "The pinnipede teletype wishes you a nice mouling."), 500);
}


#define THREAD_FILTER_SZ_REALLOC 10

/* pas glop */
static void
pp_thread_filter_add_refs(Boards *boards, struct _PinnipedeFilter *f, board_msg_info *base_mi)
{
  int i,j;
  if (base_mi == NULL) return;

  /* on inspecte toutes les references */
  for (i = 0; i < base_mi->nb_refs; i++) {
    board_msg_info *mi;
    
    mi = base_mi->refs[i].mi;

    for (j = 0; j < base_mi->refs[i].nbmi; j++) {
      /* realloc la liste si il faut */
      if ((f->nid)%THREAD_FILTER_SZ_REALLOC == 0) {
	f->id = realloc(f->id, (f->nid+THREAD_FILTER_SZ_REALLOC) * sizeof(id_type)); assert(f->id);
      }
      /* si la ref n'etait pas déjà dans la liste, on l'ajoute */
      if (pp_thread_filter_find_id(f, mi->id)==0) {
	//myprintf("    ref(%d): ajout de %<YEL %d> <-- %<MAG %d>\n", f->nid, mi->id, base_mi->id);
	f->id[f->nid++] = mi->id;
	/* et on recurse ... */
	pp_thread_filter_add_refs(boards, f, mi);
      }
      mi = mi->next; /* et pas g_next, car multi-ref sur un même site  */
    }
  }
}

/* rolala y'a des features qui font mal à la tête */
static void
pp_thread_filter_add_backrefs(Boards *boards, struct _PinnipedeFilter *f, board_msg_info *base_mi)
{
  board_msg_info *mi;
  
  if (base_mi == NULL) return;

  /* on parcourt tous les message postérieurs à base_mi */
  mi = base_mi->g_next;
  while (mi) {
    int i;
    /* on regarde toutes ses references */
    for (i = 0; i < mi->nb_refs; i++) {
      board_msg_info *ref_mi;
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
	    pp_thread_filter_add_backrefs(boards, f, mi);
	  }
	}
      }
    }
    mi = mi->g_next;
  }
}

void
pp_set_thread_filter(Dock *dock, id_type base_id)
{
  Pinnipede *pp = dock->pinnipede;
  Boards *boards = dock->sites->boards;

  char fname[200];
  board_msg_info *mi;

  mi = boards_find_id(boards, base_id);
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


  pp_thread_filter_add_refs(boards, &pp->filter, mi);
  pp_thread_filter_add_backrefs(boards, &pp->filter, mi);


  BLAHBLAH(2,printf(_("Activating the filter [%s]\n"), pp->filter.filter_name));
  pp_update_content(dock, id_type_invalid_id(), 0, 0, 1);
  pp_refresh(dock, pp->win, NULL);
}


void
pp_set_login_filter(Dock *dock, char *login)
{
  Pinnipede *pp = dock->pinnipede;

  char fname[200];

  snprintf(fname, 200, "login: <%s>", login);
  pp_unset_filter(&pp->filter);
  pp->filter.filter_mode = 1;
  pp->filter.filter_name = strdup(fname);
  pp->filter.login = strdup(login);

  BLAHBLAH(2,printf(_("Activating the filter [%s]\n"), pp->filter.filter_name));
  pp_update_content(dock, id_type_invalid_id(), 0, 0, 1);
  pp_refresh(dock, pp->win, NULL);	  
}

void
pp_set_ua_filter(Dock *dock, char *ua)
{
  Pinnipede *pp = dock->pinnipede;
  char fname[200];

  snprintf(fname, 200, "ua: [%.20s]", ua);
  pp_unset_filter(&pp->filter);
  pp->filter.filter_mode = 1;
  pp->filter.filter_name = strdup(fname);
  pp->filter.ua = strdup(ua);

  BLAHBLAH(2,printf(_("Activating the filter [%s]\n"), pp->filter.filter_name));
  pp_update_content(dock, id_type_invalid_id(), 0, 0, 1);
  pp_refresh(dock, pp->win, NULL);
}

void
pp_set_word_filter(Dock *dock, char *word)
{
  Pinnipede *pp = dock->pinnipede;
  char fname[200];

  snprintf(fname, 200, "mot: '%s'", word);
  pp_unset_filter(&pp->filter);
  pp->filter.filter_mode = 1;
  pp->filter.filter_name = strdup(fname);
  pp->filter.word = strdup(word);

  BLAHBLAH(2,printf(_("Activating the filter [%s]\n"), pp->filter.filter_name));
  pp_update_content(dock, id_type_invalid_id(), 0, 0, 1);
  pp_refresh(dock, pp->win, NULL);	  
}

/* gestion des ctrl+left click sur un mot du pinnipede
   --> permet l'activation du filtre sur le mot */
static void
pp_handle_control_left_clic(Dock *dock, int mx, int my)
{
  Pinnipede *pp = dock->pinnipede;
  Boards *boards = dock->sites->boards;

  if (pp->last_selected_text && strlen(pp->last_selected_text) < 512) {
    pp_set_word_filter(dock, pp->last_selected_text);
  } else {
    PostWord *pw;
    pw = pp_get_pw_at_xy(pp, mx, my);
    if (pw) {
      if (pw->attr & PWATTR_TSTAMP) {
	/* control+clic sur l'horloge -> activation du filtre */
	pp_set_thread_filter(dock, pw->parent->id); 
      } else if (pw->attr & PWATTR_LOGIN) {
	/* control+clic sur un <login> -> filtre ! */
	pp_set_login_filter(dock, pw->w);
      } else if (pw->attr & PWATTR_NICK) {
	/* control+clic sur un useragent raccourci -> filtre ! */
	
	board_msg_info *mi;
	
	mi = boards_find_id(boards, pw->parent->id);
	if (mi) {
	  pp_set_ua_filter(dock, mi->useragent);
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
	  pp_set_word_filter(dock, p);
	} else {
	  pp_set_word_filter(dock, pw->w);
	}
	free(s);
      }
    }
  }
}

static void
pp_handle_alt_clic(Dock *dock, XButtonEvent *ev)
{
  Pinnipede *pp = dock->pinnipede;
  int mx,my;
  char *w = NULL;

  if (Prefs.gogole_search_url == NULL) return;
  mx = ev->x; my = ev->y;
  if (pp->last_selected_text) {
    w = pp->last_selected_text;
  } else {
    PostWord *pw;
    pw = pp_get_pw_at_xy(pp, mx, my);
    if (pw) w = pw->w;
  }
  if (w) {
    char *s;
    char *ww;
    ww = str_preencode_for_http(w);
    if (strlen(ww)>512) ww[512] = 0; /* faut pas pousser grand mère */
    s = str_substitute(Prefs.gogole_search_url, "%s", ww);
    open_url(s, pp->win_xpos + mx-5, pp->win_ypos+my-25, ev->button == Button1 ? 1 : 2);
    free(s); free(ww); 
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
pp_handle_shift_clic(Dock *dock, KeyList **pkl, int mx, int my, int plopify_level)
{
  Pinnipede *pp = dock->pinnipede;
  Boards *boards = dock->sites->boards;
  int num;
  unsigned boitakon_state=0;

  num = 0;
  if (plopify_level == 2) num = 1;
  if (plopify_level == 3) num = 2; /* on a fait la mega combo pour rentrer un nuisible dans la boitakon */

  if (plopify_level) {
    boitakon_state = key_list_get_state(*pkl, 2);
  }

  /* shift clic alors que du texte est selectionné: on applique le truc sur le texte selectionné */  

  if (pp->last_selected_text && strlen(pp->last_selected_text) < 512) {
    *pkl = key_list_swap(*pkl, pp->last_selected_text, HK_WORD, 0);
  } else {
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
      board_msg_info *mi;

      mi = boards_find_id(boards, pw->parent->id);

      if (mi && thread_clic) {
	char sid[15];
	snprintf(sid, 15, "%d", id_type_to_int(mi->id));
	*pkl = key_list_swap(*pkl, sid, HK_THREAD, num);
      } else if (mi && pw->attr & PWATTR_NICK) {
	if (plopify_level == 0) {
	  *pkl = key_list_swap(*pkl, mi->useragent, HK_UA, num);
	} else {
	  if (mi->login[0]) *pkl = key_list_swap(*pkl, mi->login, HK_LOGIN, num);
	  else *pkl = key_list_swap(*pkl, mi->useragent, HK_UA_NOLOGIN, num);
	}
      } else if (pw->attr & PWATTR_LOGIN) {
	*pkl = key_list_swap(*pkl, mi->login, HK_LOGIN, num);
      } else if (pw->attr & PWATTR_TSTAMP) {
	char sid[10];
	snprintf(sid,10,"%d", id_type_to_int(pw->parent->id));
	*pkl = key_list_swap(*pkl, sid, HK_ID, num);
      } else {
	char *s, *p;
	KeyList *hk;

	/* pour deplopifier un post plopifié à cause d'un mot clef, on ne peut pas esperer que l'utilisateur
	   puisse recliquer sur le mot incriminé puisque celui a de fortes chances d'avoir été transformé
	   en 'plop' ou 'pikaa' ...
	*/
	if (plopify_level && mi && (hk = board_key_list_test_mi(boards, mi, *pkl))) {
	  *pkl = key_list_remove(*pkl, hk->key, hk->type);
	} else {
	
	  /* simplification du mot */
	  s = strdup(pw->w);
	  p = s + strlen(s) -1;
	  while (p > s && !isalnum((unsigned char)*p)) { *p = 0; p--; }
	  p = s;
	  while (*p && !isalnum((unsigned char)*p)) p++;
	
	  if (strlen(p) == 0) p = pw->w;
	
	  *pkl = key_list_swap(*pkl, p, HK_WORD, num);
	  free(s);
	}
      }
    } else {
      return; /* pour eviter un refresh inutile */
    }
  }
  *pkl = board_key_list_cleanup(boards, *pkl); /* supprime les key faisant ref à des messages detruits */
  
  /* vérifie si la boitakon a été modifiée */
  if (plopify_level) {
    if (boitakon_state != key_list_get_state(*pkl, 2)) {
      boards_update_boitakon(boards);
    }
  }

  pp_pv_destroy(pp); /* force le rafraichissement complet */
  pp_update_content(dock, pp->id_base, pp->decal_base,0,1);
  pp_refresh(dock, pp->win, NULL);
}

static void
pp_open_login_home_in_browser(Dock *dock, int sid, int mx, int my, char *w, int bnum) {
  Pinnipede *pp = dock->pinnipede;
  char *s;
  assert(w);
  assert(Prefs.site[sid]);
  s = str_printf("http://%s:%d/%s~%s", 
		 Prefs.site[sid]->site_root, 
		 Prefs.site[sid]->site_port, 
		 Prefs.site[sid]->site_path, w);
  open_url(s, pp->win_xpos + mx-5, pp->win_ypos+my-25, bnum);
  free(s);
}

static void
pp_handle_left_clic(Dock *dock, int mx, int my)
{
  Pinnipede *pp = dock->pinnipede;
  Boards *boards = dock->sites->boards;
  PostWord *pw;

  /* affichage/masquage du 'crochet' à gauche des messages mis en valeur */
  if (mx < 5) {
    pw = pp_get_pw_at_xy(pp, 20, my); /* le 20 est une ruse de chacal puant */
    if (pw) {
      int changed = 1;
      if (pw->parent->is_my_message) {
	/* désactivé car inutile */
	/* pp->hilight_my_message_mode = 1-pp->hilight_my_message_mode; */
      } else if (pw->parent->is_answer_to_me) {
	/* pareil */
	/* pp->hilight_answer_to_me_mode = 1-pp->hilight_answer_to_me_mode; */
      } else if (pw->parent->is_hilight_key) {
	board_msg_info *mi;
	KeyList *hk;
	mi = boards_find_id(boards, pw->parent->id);
	if (mi && (hk = board_key_list_test_mi(boards, mi, Prefs.hilight_key_list))) {
	  Prefs.hilight_key_list = key_list_remove(Prefs.hilight_key_list, hk->key, hk->type);
	}
      } else changed = 0;
      if (changed) {
	pp_pv_destroy(pp); /* force le rafraichissement complet */
	pp_update_content(dock, pp->id_base, pp->decal_base,0,0);
	pp_refresh(dock, pp->win, NULL);
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
	char *username = Prefs.site[id_type_sid(pw->parent->id)]->user_name;
	if (username) {
	  snprintf(dock->coin_coin_message, MESSAGE_MAXMAX_LEN, "%s %s ",
		   username, s_ts);
	} else {
	  snprintf(dock->coin_coin_message, MESSAGE_MAXMAX_LEN, "%s ",
		   s_ts);
	}
	//	  strncpy(dock->coin_coin_message, pw->w, MESSAGE_MAX_LEN);
	// strncat(dock->coin_coin_message, " ", MESSAGE_MAX_LEN);
	dock->coin_coin_message[MESSAGE_MAXMAX_LEN] = 0;
	editw_show(dock, Prefs.site[id_type_sid(pw->parent->id)], 0);
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
      board_msg_info *mi;
      int bidon;
      int ref_num;

      mi = check_for_horloge_ref(boards, pw->parent->id, pw->w, NULL, 0, &bidon, &ref_num); assert(bidon);



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
	  pv = pp_pv_add(pp, mi->id);
	  if (pv) {
	    int ligne = pp->nb_lignes - pv->nblig; // - (my-LINEY0(0))/(LINEY0(1)-LINEY0(0));
	    pp_update_content(dock, mi->id, ligne, 0, 0);
	  }
	  trouve = pp->lignes[0];
	}
	pp_refresh(dock, pp->win, trouve);
#else
	/* si la reference désigne plusieurs post de la même heure ,
	   on se déplace vers le dernier du bloc
	*/
	if (ref_num == -1) {
	  while (mi->next && mi->next->timestamp == mi->timestamp) {
	    mi = mi->next;
	  }
	}

	pp_update_content(dock, mi->id, 0, 0, 0);
	pp_refresh(dock, pp->win, NULL);
#endif
      }
    } else if (pw->attr & PWATTR_LOGIN) {
      pp_open_login_home_in_browser(dock, id_type_sid(pw->parent->id),mx, my, pw->w,1);
    }
  } /* if (pw) */  
}

/* gestion du relachement du bouton souris (si on n'est pas en train de 'tirer' la fenetre, 
   et si on n'a pas cliqué sur la barre de petits boutons */
void 
pp_handle_button_release(Dock *dock, XButtonEvent *event)
{
  Pinnipede *pp = dock->pinnipede;
  Boards *boards = dock->sites->boards;
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
    pp_update_content(dock, pp->id_base, pp->decal_base-q,0,0);
    pp_refresh(dock, pp->win, NULL);
    //    printf("scroll up  : id=%d %d\n",pp->id_base, pp->decal_base);
  } else if (event->button == Button5) {
    pp_update_content(dock, pp->id_base, pp->decal_base+q,0,0);
    pp_refresh(dock, pp->win, NULL);
    //printf("scroll down: id=%d %d\n",pp->id_base, pp->decal_base);
  } else if (event->button == Button1) {
    if (event->state & ShiftMask) {
      pp_handle_shift_clic(dock, &Prefs.hilight_key_list, mx, my, 0);
    } else if (event->state & ControlMask) {
      pp_handle_control_left_clic(dock, mx, my);
    } else if (event->state & (Mod1Mask | Mod4Mask) ) { /* on est gentil, les deux marchent */
      pp_handle_alt_clic(dock, event);
    } else {
      pp_handle_left_clic(dock, mx, my);
    }
  } else if (event->button == Button2) {
    if ((event->state & ControlMask)==0) {    
      PostWord *pw;

      pw = pp_get_pw_at_xy(pp, mx, my);
      /* middle clic sur une horloge, on copie le contenu du message dans le clipboard */
      if (pw && (pw->attr & PWATTR_TSTAMP)) {
	board_msg_info *mi;
	mi = boards_find_id(boards, pw->parent->id);
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
      } else if (pw && pw->attr & PWATTR_LOGIN) {
	pp_open_login_home_in_browser(dock, id_type_sid(pw->parent->id), mx, my, pw->w,2);
      }
    } else if (event->state & (Mod1Mask|Mod4Mask)) { /* les 2 touches marchent */
      pp_handle_alt_clic(dock, event);
    } else {
      /* Ctrl+Middle clic: Et un scrinechote, un ! */
      pp_boardshot_kikoooo(dock);
    }
  } else if (event->button == Button3) {
    if (event->state & ShiftMask) {
      int plop_level = 1;
      if (event->state & Mod1Mask) {
	plop_level = 2;
	if ((event->state & ControlMask) && 
	    (event->state & Mod4Mask)) {
	  //	  printf("yo! %04x\n", event->state);
	  plop_level = 3;
	}
      }
      pp_handle_shift_clic(dock, &Prefs.plopify_key_list, mx, my, plop_level);
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
	board_msg_info *mi;
	mi = boards_find_id(boards, pw->parent->id);
	if (mi) {
	  if (mi->useragent && strlen(mi->useragent)) {
	    editw_cb_copy(dock, pp->win, mi->useragent, strlen(mi->useragent));	  
	  }
	}
      } else if (pw && (pw->attr & PWATTR_REF)) {
	editw_cb_copy(dock, pp->win, pw->w, strlen(pw->w));  
      } else if (pw && (pw->attr & PWATTR_LOGIN)) {
	char *s;
	s = str_printf("/msg %s ", pw->w);
	if (editw_ismapped(dock->editw) == 0) {
	  strncpy(dock->coin_coin_message, s, MESSAGE_MAXMAX_LEN);
	  dock->coin_coin_message[MESSAGE_MAXMAX_LEN] = 0;
	  editw_show(dock, Prefs.site[id_type_sid(pw->parent->id)], 0);
	  editw_move_end_of_line(dock->editw, 0);
	} else {
	  editw_erase(dock->editw); editw_erase(dock->editw); // deux fois pour être sur de tout effacer (un peu gruik mais j'ai la flemme)
	  editw_insert_string(dock->editw, s);
	}
	free(s);
	editw_refresh(dock, dock->editw);
      } else {
	/* right clic ailleurs, affiche la mini barre de boutons */
	if ((pp->use_minibar == 0) || my < pp->win_height - MINIB_H) {
	  if (pp->use_minibar == 0) {
	    pp_minib_show(dock);
	  } else {
	    pp_minib_hide(dock);
	  }
	  pp_update_content(dock, pp->id_base, 0,0,1);
	  pp_refresh(dock, pp->win, NULL);	    	    
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
      unsigned long bgpixel = pp->win_bgpixel[pp->active_tab->site->site_id];
      if (pp->lignes[l]) { bgpixel = pp->win_bgpixel[id_type_sid(pp->lignes[l]->parent->id)]; }
    
      pp_draw_line(dock, pp->lpix, pp->lignes[l], bgpixel, 
		   &pp->lignes_sel[l], pp->transparency_mode, LINEY0(l));
      XCopyArea(dock->display, pp->lpix, pp->win, dock->NormalGC, 0, 0, 
		pp->win_width - (pp->sc ? SC_W-1 : 0), pp->fn_h, 0, LINEY0(l));
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
    /*    printf("first=%d, last=%d\n", pp->lignes_sel[l].first_pw_pos, pp->lignes_sel[l].last_pw_pos); */
    pw = pp->lignes_sel[l].first_pw;
    while (pw) {
      int i, i0, i1;
      i0 = 0; i1 = strlen(pw->w);
      if (pw == pp->lignes_sel[l].first_pw) {
	i0 = pp->lignes_sel[l].first_pw_pos;
      } 
      if (pw == pp->lignes_sel[l].last_pw) {
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


void
pp_dispatch_event(Dock *dock, XEvent *event)
{
  Pinnipede *pp = dock->pinnipede;

  static int old_mouse_x = -1, old_mouse_y = -1;
  static int mouse_button_press_x = -1, mouse_button_press_y = -1;
  
  static int dragging = 0;
  static Time time_drag = 0;

  /* le pinnipede ne fait RIEN quand la tribune est en cours de mise à jour ... */
  if (flag_updating_board) return;

  switch (event->type) {
  case DestroyNotify: 
    {
      //      printf("destroy?\n");
    } break;
  case ButtonPress:
    {     
      pp_selection_unselect(pp);
      if (pp_widgets_handle_button_press(dock, &event->xbutton)) {
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
	  if (pp->last_selected_text) { free(pp->last_selected_text); pp->last_selected_text=NULL;}	  	  
	  pp->last_selected_text = malloc(blen); assert(pp->last_selected_text);
	  pp_selection_copy(dock, pp->last_selected_text);
	  editw_cb_copy(dock, pp->win, pp->last_selected_text, blen-1);
	  pp->time_sel = time(NULL);
	}
      } else {
	if (pp_widgets_handle_button_release(dock, &event->xbutton)) {
	  //printf("plop\n");
	} else if (dragging == 0) {
	  pp_handle_button_release(dock, &event->xbutton);
	}
      }
      dragging = 0;
      time_drag = event->xbutton.time;
    } break;
  case MotionNotify:
    {
      event = flush_consecutive_mouse_motions(dock, pp->win, event);

      if (pp_widgets_handle_motion(dock, &event->xmotion)) {
	/* plop */
      } else if (event->xmotion.state & Button1Mask && 
	  SQR(mouse_button_press_x - event->xbutton.x)+
	  SQR(mouse_button_press_y - event->xbutton.y) >= 6) {

	pp->time_sel = time(NULL);
	if (pp->lignes_sel == NULL) {
	  pp->sel_anchor_x = old_mouse_x; pp->sel_anchor_y = old_mouse_y;
	  pp_refresh(dock, pp->win, NULL);
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
	  pp_update_content(dock, pp->id_base, pp->decal_base-decal_y,0,0);
	  pp_refresh(dock, pp->win, NULL);
	  old_mouse_x = event->xmotion.x;
	  old_mouse_y = event->xmotion.y;
	  time_drag = event->xmotion.time;
	}
      } else {
	pp_check_survol(dock, event->xmotion.x, event->xmotion.y);
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
      if (event->xconfigure.width != pp->win_width || event->xconfigure.height != pp->win_height ||
	  pp->transparency_mode) {
	pp->win_width = MAX(event->xconfigure.width,80);
	pp->win_height = MAX(event->xconfigure.height,80);
	
	/* eh oui, faut pas oublier ça.... */
	XFreePixmap(dock->display, pp->lpix);
	pp->lpix = XCreatePixmap(dock->display, pp->win, pp->win_width, MAX(MINIB_H, pp->fn_h), DefaultDepth(dock->display,dock->screennum));

	/* euh oui, a fixer d'une maniere ou d'une autre,
	   mais il ne faut pas que la fenetre soit mappée quand
	   on met à jour l'image de fond ...(en mode use_fake_real_transparency) */
	if (Prefs.use_fake_real_transparency == 0)
	  pp_update_bg_pixmap(dock);
	
	//	pp_minib_initialize(pp);
	pp_pv_destroy(pp);
	pp_update_content(dock, pp->id_base, pp->decal_base, 0,1);
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
	pp_refresh(dock, pp->win, NULL);
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
pp_set_board_updated(Dock *dock)
{
  dock->pinnipede->flag_board_updated = 1;
}

/* sauvegarde de la position et des dimensions du pinni 
   (appelé par wmcc_save_state de wmcoincoin.c) 
 */
void
pp_save_state(Dock *dock, FILE *f) {
  Pinnipede *pp = dock->pinnipede;

  fprintf(f, "%d %d %d %d %d\n", 
	  pp->mapped,
	  pp->win_xpos, pp->win_ypos, 
	  pp->win_width, pp->win_height);
}


void
pp_restore_state(Dock *dock, FILE *f) {
  Pinnipede *pp = dock->pinnipede;
  int mapped, win_xpos, win_ypos, win_width, win_height;

  if (fscanf(f, "%d %d %d %d %d\n", 
	     &mapped,
	     &win_xpos, &win_ypos, 
	     &win_width, &win_height) == 5) {

    /* on vérifie qu'on n'a pas spécifié de préferences dans le fichier d'options */
    if (Prefs.pp_xpos == -10000 && Prefs.pp_ypos == -10000) {
      if (win_xpos != -10000) {
	pp->win_xpos = MAX(MIN(win_xpos,3000),-20);
	pp->win_ypos = MAX(MIN(win_ypos,3000),-20);
	pp->win_width = MAX(MIN(win_width,3000),50);
	pp->win_height = MAX(MIN(win_height,3000),50);
      }
      pp_widgets_set_pos(dock);
      //      if (pp->use_minibar)
      //	pp_minib_initialize(pp);
    }
  }  
}
