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
  rcsid=$Id: coincoin_tribune.c,v 1.32 2002/04/13 11:55:19 pouaite Exp $
  ChangeLog:
  $Log: coincoin_tribune.c,v $
  Revision 1.32  2002/04/13 11:55:19  pouaite
  fix kde3 + deux trois conneries

  Revision 1.31  2002/04/11 23:16:54  pouaite
  boitakon mega combo

  Revision 1.30  2002/04/09 23:38:29  pouaite
  boitakon et son cortège de bugfixes

  Revision 1.29  2002/04/09 00:28:19  pouaite
  quelques modifs faites dans un état d'hébétude avancé /!\ travaux en cours /!\

  Revision 1.28  2002/04/01 22:56:03  pouaite
  la pseudo-transparence du pinni, bugfixes divers, option tribune.backend_type

  Revision 1.27  2002/04/01 01:39:38  pouaite
  grosse grosse commition (cf changelog)

  Revision 1.26  2002/03/27 23:27:10  pouaite
  tjs des bugfixes (pour gerer des posts qui peuvent atteindre 10ko !), en parallele de la v2.3.6-5

  Revision 1.25  2002/03/27 19:02:04  pouaite
  bugfix pour le nouveau format du backend

  Revision 1.24  2002/03/18 22:46:49  pouaite
  1 ou 2 bugfix mineurs, et surtout suppression de la dependance avec la libXpm .. un premier pas vers wmc² en 8bits

  Revision 1.23  2002/03/03 10:10:04  pouaite
  bugfixes divers et variés

  Revision 1.22  2002/03/01 00:27:40  pouaite
  trois fois rien

  Revision 1.21  2002/02/27 00:32:19  pouaite
  modifs velues

  Revision 1.20  2002/02/24 22:13:56  pouaite
  modifs pour la v2.3.5 (selection, scrollcoin, plopification, bugfixes)

  Revision 1.19  2002/02/03 23:07:32  pouaite
  *** empty log message ***

  Revision 1.18  2002/02/02 23:49:17  pouaite
  plop

  Revision 1.17  2002/01/20 20:53:22  pouaite
  bugfix configure.in && http_win.c pour cygwin + 2-3 petis trucs

  Revision 1.16  2002/01/20 02:17:13  pouaite
  modifs d'ordre esthetique (!) sans grand interet

  Revision 1.15  2002/01/18 00:28:42  pouaite
  le ménage continue + grosses modifs (experimentales pour l'instant)

  Revision 1.14  2002/01/16 00:35:26  pouaite
  debut de detection des reponse à nos message avec des couleurs hideuses et certainement plein de bugs moisis

  Revision 1.13  2002/01/15 15:30:17  pouaite
  rien d'interessant

  Revision 1.12  2002/01/14 23:54:06  pouaite
  reconnaissance des posts effectué par l'utilisateur du canard (à suivre...)

  Revision 1.11  2002/01/13 20:06:14  pouaite
  decidement je fais rien que des conneries

  Revision 1.10  2002/01/13 19:44:48  pouaite
  minifix pour éviter que la commande externe soit lancée sur tous les messages à l'initialisation du coincoin

  Revision 1.9  2002/01/13 15:19:00  pouaite
  double patch: shift -> tribune.post_cmd et lordOric -> tribune.archive

  Revision 1.8  2002/01/12 17:29:08  pouaite
  support de l'iso8859-15 (euro..)

  Revision 1.7  2002/01/10 09:03:06  pouaite
  integration du patch de glandium (requetes http/1.1 avec header 'If-Modified-Since' --> coincoin plus gentil avec dacode)

  Revision 1.6  2002/01/06 16:52:37  pouaite
  preparation pour la prochaine v. de la tribune avec sa gestion integree du wiki et des logins, tout ça ..

  Revision 1.5  2001/12/16 21:51:22  pouaite
  *** empty log message ***

  Revision 1.4  2001/12/16 20:28:45  pouaite
  bugfixes divers

  Revision 1.3  2001/12/16 01:43:33  pouaite
  filtrage des posts, meilleure gestion des posts multiples

  Revision 1.2  2001/12/02 18:34:54  pouaite
  ajout de tags cvs Id et Log un peu partout...

*/

#include "coincoin.h"
#include "http.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

/* C'est sale, mais j'ai pas envie de la triballer dans toutes les fonctions
   - n'est accédé que dans ce fichier 

<pouaite> ce n'est pas sale, ton source change c'est normal ;)
*/
char tribune_last_modified[512] = "";

/* utilise tres localement, c'est la longueur DANS remote.rdf, la longueur réelle sera moindre
   (remplacement de &eacute par 'é' etc... ) */
#define TRIBUNE_UA_MAX_LEN 1000
#define TRIBUNE_MSG_MAX_LEN 15000 /* on peut y arriver avec un bon gros message plein de [][][][]... */
#define TRIBUNE_LOGIN_MAX_LEN 60


/*
  statistique à la noix sur le nombre de personnes sur la tribune
*/
#define TF_HASH_SZ 2048
void
tribune_frequentation(const DLFP_tribune *trib, int nb_minutes, int *ua_cnt, int *msg_cnt, int *my_msg_cnt) {
  unsigned short hash_cnt[TF_HASH_SZ];
  unsigned hash_val;
  const unsigned c2 = 31117, c1 = 11279; 

  time_t t_last;

  tribune_msg_info *it;
  int i;
  

  

  *ua_cnt = 0;
  *msg_cnt = 0;
  *my_msg_cnt = 0;

  it = tribune_find_id(trib, trib->last_post_id);
  if (it == NULL) return;
  t_last = it->timestamp;
  t_last += trib->nbsec_since_last_msg;

  memset(hash_cnt, 0, sizeof(hash_cnt));
  it = trib->msg; 
  while (it) {
    if (difftime(t_last, it->timestamp) < nb_minutes*60) {
      const char *s;
      if (it->login[0] == 0) {
	s = it->useragent;
      } else {
	s = it->login;
      }
      /* au pifometre... faudra ptet verifier qu'on hache correctement */
      hash_val = 1;
      for (i=0; s[i]; i++) {
	hash_val = ((hash_val*((unsigned char)s[i] + c1)) + 1) % c2;
      }
      hash_val = hash_val % TF_HASH_SZ;
      hash_cnt[hash_val]++;
      (*msg_cnt)++;
      if (it->is_my_message) (*my_msg_cnt)++;
    }

    it = it->next;
  }
  for (i=0; i < TF_HASH_SZ; i++) {
    if (hash_cnt[i]) (*ua_cnt)++;
  }
}

/* 
   essaye d'identifier le user agent selon
   les regle (regular expression) definies dans trib->rules 

   ce système est tout vieux (depuis la v1.0beta!) et tout moche
*/
void
tribune_tatouage(DLFP_tribune *trib, tribune_msg_info *it)
{
  DLFP_trib_load_rule *r;
  int rcnt;

  it->tatouage = NULL; /* ceux qui sont tatoues a NULL sont purement et simplement ignores */
  r = trib->rules;
  if (r == NULL) {
    BLAHBLAH(1, printf("impossible de tatouer->pas de regles\n"));
    return;
  }
  rcnt = 0;
  while (r) {
    int ca_colle;
    ALLOW_X_LOOP; /* car les regex, ça prend du temps ! */
    ca_colle = regexec(&r->rule, it->useragent, (size_t)0, (regmatch_t*)NULL, 0);
    if (ca_colle == 0) {
      it->tatouage = r; 
      BLAHBLAH(2, myprintf("'%<RED %s>' est %<CYA reconnu> dans regex %d\n",it->useragent, rcnt));
      break;
    }
    rcnt++;	       
    r = r->next;
  }
  if (it->tatouage == NULL) {
    BLAHBLAH(2, myprintf("'%<RED %s>' n'est %<CYA pas reconnu> dans regex\n", it->useragent));
  }

}

/* 
   renvoie l'age du message, en secondes 
*/
time_t
tribune_get_msg_age(const DLFP_tribune *trib, const tribune_msg_info *it)
{
  time_t linuxfr_time_now;
  linuxfr_time_now = trib->last_post_timestamp + trib->nbsec_since_last_msg;
  return (linuxfr_time_now - it->timestamp);
}

/* renvoie l'estimation de l'heure actuelle sur la tribune --
 en secondes */
time_t
tribune_get_time_now(const DLFP_tribune *trib)
{
  return (trib->last_post_timestamp + trib->nbsec_since_last_msg);
}

/*
  c'est triste, mais il faut bien que quelqu'un se charge d'éliminer les messages trop vieux
*/
static void
tribune_remove_old_msg(DLFP_tribune *trib)
{
  tribune_msg_info *it, *pit;
  int cnt;

  cnt = 0;
  it = trib->msg; pit = NULL;
  while (it) {
    cnt++;
    it = it->next;
  }
  while (cnt > Prefs.tribune_max_msg && trib->msg) {
    BLAHBLAH(4, printf("tribune_remove_old_msg: destruction de id=%d (date=%s)\n", trib->msg->id, ctime(&trib->msg->timestamp)));
    it = trib->msg->next;

    /* nettoyage des references à trib->msg */
    {
      tribune_msg_info *mi;
      int i;
      mi = trib->msg->next;
      while (mi) {
	for (i=0; i < mi->nb_refs; i++) {
	  /* si on trouve un ref à ce message ... */
	  if (mi->refs[i].mi == trib->msg) {
	    assert(mi->refs[i].nbmi>=1);
	    
	    /* alors on l'efface si la ref pointe uniquement sur lui,
	       et si c'est un ref sur plusieurs messages consécutifs, alors
	       un decremente le compteur et on la fait pointer sur le suivant. */
	    mi->refs[i].nbmi--;
	    if (mi->refs[i].nbmi == 0) {
	      mi->refs[i].mi = NULL;
	    } else {
	      mi->refs[i].mi = trib->msg->next;
	    }
	  }
	}
	mi = mi->next;
      }
    }

    free(trib->msg);
    cnt--;
    trib->msg = it;
  }
}

/*
  prout
*/
static time_t
timestamp_str_to_time_t(char *sts) 
{
  struct tm t;

  assert(strlen(sts) == 14);
  
  sscanf(sts, "%4d%2d%2d%2d%2d%2d", &t.tm_year,&t.tm_mon,&t.tm_mday,&t.tm_hour,&t.tm_min,&t.tm_sec);
  t.tm_mon = t.tm_mon - 1;
  t.tm_year = t.tm_year - 1900; 
  t.tm_isdst = -1;
  return mktime(&t);
}

/* verifie pour chaque message, si il est necessaire d'afficher les secondes, ou bien
   si le poste peut etre identifie sans ambiguite par hh:ss 

   nouveau (v2.3.2) -> gère aussi les sub_timestamp 
*/
static void 
update_secondes_flag(DLFP_tribune *trib)
{
  tribune_msg_info *it,*pit;

  pit = trib->msg;
  if (pit == NULL) return;
  it = pit->next;
  while (it) {
    if (it->next) it->hmsf[3] = 0; 
    if (it->hmsf[0] == pit->hmsf[0] && it->hmsf[1] == pit->hmsf[1]) {
      it->hmsf[3] = 1; pit->hmsf[3] = 1;
    }
    if (it->timestamp == pit->timestamp) {
      if (pit->sub_timestamp == -1) {
	pit->sub_timestamp = 0;
      }
      it->sub_timestamp = MIN(pit->sub_timestamp+1,9); /* MIN(10,.) sinon y'aura des pb dans le pinnipede */
    }

    pit = it;
    it = it->next;
  }
}

/*
  enleve les commentaires xml (qui sont inseres autour des lien du wiki etc)
*/
static char *
nettoie_message_tags(const char *inmsg) 
{
  char *outmsg;
  const char *s; char *w, *p;
  int in_comment;

  outmsg = malloc(strlen(inmsg)+1);
  in_comment = 0;
  p = outmsg;
  for (s = inmsg; *s; s++) {
    if (strncmp(s, "\t<!--",5)==0 && in_comment == 0) {
      w = strstr(s, "--\t>");
      if (w) {
	in_comment = w+4-s;
      }
    }
    if (in_comment == 0) {
      *p = *s; p++;
    } else in_comment--;
  }
  *p = 0;
  return outmsg;
}

char *
wiki_url_encode(const unsigned char *w)
{
  unsigned char *w2, *ret;
  w2 = str_preencode_for_http(w);
  ret = str_printf("\t<a href=\"%s%s\"\t>[%s]\t</a\t>", Prefs.tribune_wiki_emulation, w2, w);
  free(w2); 
  return ret;
}

int
do_wiki_emulation(const char *inmsg, char *dest) 
{
  int j;
  const char *s;

  int in_a_href = 0;
  s = inmsg; j = 0;
  while (*s) {
    if (*s == '\t') {
      if (strncasecmp(s, "\t<a href", 8) == 0 && in_a_href==0) {
	in_a_href = 1;
      } else if (strncasecmp(s, "\t</a", 4) == 0 && in_a_href) {
	in_a_href = 0;
      }
 
   }
    if (*s == '[') {
      char *pfin;
      
      pfin = strchr(s+1, ']');
      
      if (pfin && in_a_href == 0) {
	char *ptag, *pautre;
	ptag = strchr(s+1, '\t');
	pautre = strchr(s+1, '[');
	if ((pautre == NULL || pautre > pfin) &&
	    (ptag == NULL || ptag > pfin)) {
	  char *wiki_word, *wiki_url, *p;

	  wiki_word = malloc(pfin-s); assert(wiki_word);
	  strncpy(wiki_word, s+1, pfin-s-1); wiki_word[pfin-s-1] = 0;
	  wiki_url = wiki_url_encode(wiki_word); free(wiki_word);
	  p = wiki_url;
	  while (*p) { 
	    if (dest) dest[j] = *p; 
	    j++; p++;
	  }
	  s = pfin+1;
	  continue;
	}
      }
    }
    if(dest) dest[j] = *s; 
    j++; s++;
  }
  if (dest) dest[j] = 0;
  j++;
  return j;
}


void
tribune_update_boitakon(DLFP_tribune *trib)
{
  tribune_msg_info *mi = trib->msg;
  while (mi) {
    KeyList *hk = tribune_key_list_test_mi_num(trib, mi, Prefs.plopify_key_list, 2);
    if (hk) { /* bienvenu dans la boitakon */
      mi->in_boitakon = 1;
    } else mi->in_boitakon = 0;
    mi = mi->next;
  }
  flag_tribune_updated = 1;
}

/*
  enregistre un nouveau message
*/
static tribune_msg_info *
tribune_log_msg(DLFP_tribune *trib, char *ua, char *login, char *stimestamp, char *_message, int id, const unsigned char *my_useragent)
{
  tribune_msg_info *nit, *pit, *it;
  char *message = NULL;

  message = nettoie_message_tags(_message);

  /* emulation du wiki (en insérant les bons tags dans le message) */
  if (Prefs.tribune_wiki_emulation) {
    char *tmp = message;
    int sz;
    sz = do_wiki_emulation(tmp, NULL); message = malloc(sz); do_wiki_emulation(tmp, message);
    free(tmp);
  }

  BLAHBLAH(4, printf("message logué: '%s'\n", message));
  nit = trib->msg;
  pit = NULL;
  while (nit) {
    if (nit->id > id) {
      break;
    }
    pit = nit;
    nit = nit->next;
  }

  it = (tribune_msg_info*) malloc(sizeof(tribune_msg_info)+strlen(ua)+1+strlen(message)+1+strlen(login)+1);
  it->timestamp = timestamp_str_to_time_t(stimestamp);
  it->sub_timestamp = -1;
  it->useragent = ((char*)it) + sizeof(tribune_msg_info);
  it->msg = ((char*)it) + sizeof(tribune_msg_info) + strlen(ua) + 1;
  it->login = it->msg + strlen(message) + 1;
  it->in_boitakon = 0; /* voir plus bas */
  

  it->next = nit;
  if (pit) {
    pit->next = it;
  } else {
    trib->msg = it;
  }

  it->id = id;


  if (trib->last_post_id < it->id) {
    trib->last_post_timestamp = it->timestamp;
    trib->last_post_id = it->id;
    trib->last_post_time[0] = stimestamp[8];
    trib->last_post_time[1] = stimestamp[9];
    trib->last_post_time[2] = ':';
    trib->last_post_time[3] = stimestamp[10];
    trib->last_post_time[4] = stimestamp[11];
  }

  it->hmsf[0] = (stimestamp[8]-'0')*10 + (stimestamp[9]-'0');
  it->hmsf[1] = (stimestamp[10]-'0')*10 + (stimestamp[11]-'0');
  it->hmsf[2] = (stimestamp[12]-'0')*10 + (stimestamp[13]-'0');
  it->hmsf[3] = 1;

  strcpy(it->useragent, ua);
  strcpy(it->msg, message);
  strcpy(it->login, login);

  it->nb_refs = 0;
  it->refs = NULL; /* ça sera traité un peu plus tard */

  BLAHBLAH(3, myprintf("log msg id=%d, login=%s timestamp=%u msg='%<YEL %s>'\n", id, it->login, (unsigned)it->timestamp, it->msg));

  /* et on n'oublie pas..*/
  trib->nbsec_since_last_msg = 0;

  /* remise a jour du flag d'affichage des secondes */
  update_secondes_flag(trib);

  /* essaye de detecter si vous étez l'auteur du message */
  if (Prefs.user_login && Prefs.user_login[0] && trib->just_posted_anonymous == 0) {
    it->is_my_message = !strcmp(Prefs.user_login, it->login);
  } else {
    it->is_my_message = !strcmp(it->useragent, my_useragent);
/*    if (it->is_my_message) {
      myprintf("my_message: '%<yel %s>' == '%<grn %s>'\n", it->useragent, my_useragent);
    }*/
  }
  trib->just_posted_anonymous = 0;


  it->is_answer_to_me = 0;

  /* essaye d'identifier le user agent */
  tribune_tatouage(trib, it);

  /* evalue le potentiel trollesque */
  troll_detector(it);

  {
    KeyList *hk = tribune_key_list_test_mi_num(trib, it, Prefs.plopify_key_list, 2);
    if (hk) { /* bienvenu dans la boitakon */
      it->in_boitakon = 1;
      BLAHBLAH(2, myprintf("bienvenu au message de '%.20s' dans la boitakon\n", it->login ? it->login : it->useragent));
    }
  }

  free(message);
  return it;
}

/* 
   renvoie le nombre moyen de messages postes sur la tribune libre au
   cours des derniere 'trollo_log_extent' secondes
*/
void
dlfp_tribune_get_trollo_rate(const DLFP_tribune *trib, float *trollo_rate, float *trollo_score)
{
  tribune_msg_info *it;
  int cnt;
  
  it = trib->msg;

  *trollo_score = 0.;
  cnt = 0;
  while (it) {
    int age;
    float coef;
    
    age = tribune_get_msg_age(trib, it);
    //    printf("id=%d, %d , age=%d\n", it->id, cnt, tribune_get_msg_age(trib, it));
    if (tribune_get_msg_age(trib, it) <= trollo_log_extent*60) {
      cnt++;
      coef = 0.1 * (trollo_log_extent*60 - age)/((float)(trollo_log_extent*60));
      *trollo_score += (it->troll_score)*coef;
    }
    it = it->next;
  }
  /* renvoie le nb de messages par minute */
  *trollo_rate = (float)cnt/(float)trollo_log_extent;

}


/*
  merci shift pour ce patch !

  appelle le programme externe (dans l'ordre des id) pour chaque nouveau message reçu
*/
static void
dlfp_tribune_call_external(DLFP_tribune *trib, int last_id)
{
  tribune_msg_info *it;
  
  if (Prefs.post_cmd == NULL) {
    return;
  } else {
    BLAHBLAH(1, myprintf("dlfp_tribune_call_external, id=%d - %d\n", last_id, trib->last_post_id));
  }
  if (last_id != -1) { /* si ce n'est pas le premier appel.. */
    it = tribune_find_id(trib, last_id);
    if (it) it = it->next;
  } else {
    //    it = trib->msg;
    return; /* à l'initialisation, on évite de passer tous les messages dans le coincoin */
  }
  while (it) {
    char *qlogin;
    char *qmessage;
    char *qua;
    char sid[20], stimestamp[20], strollscore[20], *stypemessage;
    char *shift_cmd;

    const char *keys[] = {"$l", "$m", "$u", "$i", "$t", "$s", "$r"};
    const char *subs[] = {  "",   "",   "",   "",   "",   "",   ""};

    
    //----< Code pour passer les infos d'un post à une commande extérieure >

    qlogin = shell_quote(it->login);
    qmessage = shell_quote(it->msg);
    qua = shell_quote(it->useragent);
    snprintf(sid, 20, "%d", it->id);
    snprintf(stimestamp, 20, "%lu", (unsigned long)it->timestamp);
    snprintf(strollscore, 20, "%d", it->troll_score);
    if (it->is_my_message) stypemessage = "1";
    else if (it->is_answer_to_me) stypemessage = "2";
    else if (tribune_key_list_test_mi(trib, it, Prefs.hilight_key_list)) stypemessage = "3";
    else if (tribune_key_list_test_mi(trib, it, Prefs.plopify_key_list)) stypemessage = "4";
    else stypemessage = "0";

    subs[0] = qlogin;
    subs[1] = qmessage;
    subs[2] = qua;
    subs[3] = sid;
    subs[4] = stimestamp;
    subs[5] = strollscore;
    subs[6] = stypemessage;
    shift_cmd = str_multi_substitute(Prefs.post_cmd, keys, subs, 7);
    BLAHBLAH(2, myprintf("post_cmd: /bin/sh -c %<YEL %s>\n", shift_cmd));
    system(shift_cmd);

    free(shift_cmd);
    free(qlogin);
    free(qmessage);
    free(qua);

    //----</ Code >
    it = it->next;

    ALLOW_X_LOOP;
  }
}




/*
  detecte les refs par petites horloges
  (mais ne tente rien pour ipot)
*/
void
tribune_check_my_messages(DLFP_tribune *trib, int old_last_post_id) { 
  if (trib->last_post_id != old_last_post_id) { /* si de nouveaux messages ont été reçus */
    tribune_msg_info *it;

    /* essaye de detecter si il s'agit d'une réponse à un de vos messages 
     */
    if (old_last_post_id != -1) { /* si ce n'est pas le premier appel.. */
      it = tribune_find_id(trib, old_last_post_id);
      if (it) it = it->next;
    } else {
      it = trib->msg;
    }
    while (it) {
      if (tribune_msg_is_ref_to_me(trib, it)) {
	flag_updating_tribune++;
	it->is_answer_to_me = 1;
	flag_updating_tribune--;
	if (old_last_post_id != -1) flag_tribune_answer_to_me = 1;
      }
      flag_updating_tribune++;
      tribune_msg_find_refs(trib, it);
      flag_updating_tribune--;
      it = it->next;
    }
  }
}


/* decodage du message, quel que soit l'état du backend .. */
void
tribune_decode_message(char *dest, const char *src) {
  strncpy(dest, src, TRIBUNE_MSG_MAX_LEN); dest[TRIBUNE_MSG_MAX_LEN-1] = 0;
  if (Prefs.tribune_backend_type == 1) {
    mark_html_tags(dest, TRIBUNE_MSG_MAX_LEN);
  }
  convert_to_ascii(dest, dest, TRIBUNE_MSG_MAX_LEN);
  if (Prefs.tribune_backend_type == 2) {
    mark_html_tags(dest, TRIBUNE_MSG_MAX_LEN);    
    convert_to_ascii(dest, dest, TRIBUNE_MSG_MAX_LEN);
  }
  if (Prefs.tribune_backend_type == 3) {
    char *s, *s2;
    
    s = strdup(dest); assert(s);
    s2 = str_substitute(s, "</bgt;", "\t</b\t>"); free(s); s = s2; /* special super bug.. */
    s2 = str_substitute(s, "<b>", "\t<b\t>"); free(s); s = s2;
    s2 = str_substitute(s, "</b>", "\t</b\t>"); free(s); s = s2;
    s2 = str_substitute(s, "<i>", "\t<i\t>"); free(s); s = s2;
    s2 = str_substitute(s, "</i>", "\t</i\t>"); free(s); s = s2;
    s2 = str_substitute(s, "<u>", "\t<u\t>"); free(s); s = s2;
    s2 = str_substitute(s, "</u>", "\t</u\t>"); free(s); s = s2;
    s2 = str_substitute(s, "<s>", "\t<s\t>"); free(s); s = s2;
    s2 = str_substitute(s, "</s>", "\t</s\t>"); free(s); s = s2;
    s2 = str_substitute(s, "<a href", "\t<a href"); free(s); s = s2;
    s2 = str_substitute(s, "\">", "\"\t>"); free(s); s = s2;
    s2 = str_substitute(s, "</a>", "\t</a\t>"); free(s); s = s2;
    s2 = str_substitute(s, "<!--", "\t<!--"); free(s); s = s2;
    s2 = str_substitute(s, "-->", "--\t>"); free(s); s = s2;
    strncpy(dest, s, TRIBUNE_MSG_MAX_LEN); dest[TRIBUNE_MSG_MAX_LEN-1] = 0; free(s);
    convert_to_ascii(dest, dest, TRIBUNE_MSG_MAX_LEN); /* deuxième passe, à tout hasard */
  }
  BLAHBLAH(4,myprintf("message original: '%<CYA %s>'\n", src));
  BLAHBLAH(4,myprintf("message décodé: '%<MAG %s>'\n", dest));
}

/*
  lecture des nouveaux messages reçus

  my_useragent: useragent dernièrement utilisé, sert à reconnaitre si on est 
  l'auteur d'un post, ou non
*/
void
dlfp_tribune_update(DLFP *dlfp, const unsigned char *my_useragent)
{
  time_t now;
  char s[16384];
  SOCKET fd;
  char *errmsg;
  int old_last_post_id;

  const char *tribune_sign_posttime = "<post time=";
  const char *tribune_sign_info = "<info>";
  const char *tribune_sign_msg = "<message>";
  const char *tribune_sign_login = "<login>";

  errmsg = NULL;
  /* maj du nombre de secondes ecoulees depuis le dernier message recu
     (pour pouvoir calculer l'age des message -> on part du principe
     que l'horloge locale et l'horloge de linuxfr ne sont pas synchrones)
  */
  now = time(NULL);

  flag_updating_tribune++;
  old_last_post_id = dlfp->tribune.last_post_id;

  dlfp->tribune.nbsec_since_last_msg += difftime(now, dlfp->tribune.local_time_last_check);
  /* des fois qu'une des 2 horloges soit modifie a l'arrache */
  dlfp->tribune.nbsec_since_last_msg = MAX(dlfp->tribune.nbsec_since_last_msg,0);
  dlfp->tribune.local_time_last_check = time(NULL);

  flag_updating_tribune--;

  snprintf(s, 16384, "%s%s/%s", (strlen(Prefs.site_path) ? "/" : ""), Prefs.site_path, Prefs.path_tribune_backend);
  if ((Prefs.debug & 2) == 0) {
    fd = http_get(Prefs.site_root, Prefs.site_port, s, 
		  Prefs.proxy_name, Prefs.proxy_auth, Prefs.proxy_port, app_useragent, tribune_last_modified, 512);
  } else {
    snprintf(s, 16384, "%s/wmcoincoin/test/remote.xml", getenv("HOME"));
    myprintf("DEBUG: ouverture de '%<RED %s>'\n", s);
    fd = open(s, O_RDONLY);
  }

  if (fd != INVALID_SOCKET) {
    int roll_back_cnt = 0;
    while (http_get_line(s, 16384, fd) > 0) {
      if (strncasecmp(s,tribune_sign_posttime, strlen(tribune_sign_info)) == 0) {
	char stimestamp[15];
	char ua[TRIBUNE_UA_MAX_LEN];
	char msg[TRIBUNE_MSG_MAX_LEN];
	char login[TRIBUNE_LOGIN_MAX_LEN];
	int id;
	char *p;

	p = s + strlen(tribune_sign_posttime) + 1;
	strncpy(stimestamp, p, 14); stimestamp[14] = 0;
	p += 14;
	p = strstr(p, "id=");
	if (p == NULL) { errmsg = "id="; goto err; }
	id = atoi(p+4);
	if (id < 0) { errmsg="id sgn"; goto err; }

	//	printf("id=%d , last=%d\n",id,dlfp->tribune.last_post_id);
	if (tribune_find_id(&dlfp->tribune,id) && roll_back_cnt == 0) {
	  /*	  break;
            Rollback bugfix

	    il semblerait qu'il arrive parfois une sorte de race condition dans le backend (lors de posts multiples dans la
	    même seconde) :  à l'instant t, le backend contient les ids {n, n-1, n-2, ..}, et à l'instant t+1 
	    il reçoit le message n+1 en DEUXIEME POSITION: {n, n+1, n-1, n-2,...} du coup si le coincoin avait fait son
	    update entre t et t+1, il va rater le message n+1, puisque lors de la prochaine update, la lecture du backend 
	    s'arrêtera sur le message n, déjà connu.

	    solution: on fait un rollback lorsqu'il y un trou dans les id des 3 derniers messages
	  */
	  int need_roll_back = 0;

	  /* on regarde (comme un boeuf) si les message id-1, id-2 et id-3 ont bien été reçus */
	  if (id > 1 && dlfp->tribune.msg) {
	    if (tribune_find_id(&dlfp->tribune, id-1) == NULL) need_roll_back = 2;
	    if (id > 2 && dlfp->tribune.msg->next) {
	      if (tribune_find_id(&dlfp->tribune, id-2) == NULL) need_roll_back = 3;
	      if (id > 3 && dlfp->tribune.msg->next->next) {
		if (tribune_find_id(&dlfp->tribune, id-3) == NULL) need_roll_back = 4;
	      }
	    }
	  }
	  if (need_roll_back == 0) {
	    break; /* ça roule, cassos */
	  } else {
	    /* il manque un message, soit il provient de la tribune des modérateurs et est donc inaccessible,
	       soit il y a effectivement eu une race condition dans dacode */
	    
	    roll_back_cnt = 3;
	  }
	}

	if (http_get_line(s, 16384, fd) <= 0) { errmsg="httpgetline(info)"; goto err; }

	if (strncasecmp(s, tribune_sign_info,strlen(tribune_sign_info))) { errmsg="infosign"; goto err; }
	if (strncasecmp("</info>", s+strlen(s)-7,7)) { errmsg="</info>"; goto err; }
	s[strlen(s)-7] = 0; /* vire le /info */
	p = s + strlen(tribune_sign_info);

        convert_to_ascii(ua, p, TRIBUNE_UA_MAX_LEN);

	if (http_get_line(s, 16384, fd) <= 0) { errmsg="httpgetline(message)"; goto err; }
	if (strncasecmp(s, tribune_sign_msg,strlen(tribune_sign_msg))) { errmsg="messagesign"; goto err; }
	
	//	myprintf("message: '%<YEL %s>'\n\n", s); 

	// il arrive que le post tienne sur plusieurs lignes (je sais pas pourquoi) 
	{
	  int l;
	  l = strlen(s);
	  while (strncasecmp("</message>", s+l-10,10)) {
	    if (http_get_line(s+l, 16384 - l, fd) <= 0) {
	      errmsg="</message>"; goto err; 
	    }
	    l = strlen(s);
	  }
	}

	

	s[strlen(s)-10] = 0; /* vire le </message> */
	p = s + strlen(tribune_sign_msg);

	/* nettoyage des codes < 32 dans le message */
	{
	  int i = 0;

	  while (i < TRIBUNE_MSG_MAX_LEN && p[i]) {
	    if ((unsigned char)p[i] < ' ') p[i] = ' ';
	    i++;
	  }
	}

	/* attention, les '&lt;' deviennent '\t<' et les '&amp;lt;' devienne '<' */
	tribune_decode_message(msg, p);

	if (http_get_line(s, 16384, fd) <= 0) { errmsg="httpgetline(login)"; goto err; }
	if (strncasecmp(s, tribune_sign_login,strlen(tribune_sign_login))) { errmsg="messagesign_login"; goto err; }
	if (strncasecmp("</login>", s+strlen(s)-8,8)) { errmsg="</login>"; goto err; }

	s[strlen(s)-8] = 0; 
	p = s + strlen(tribune_sign_login);
	if (strcasecmp(p, "Anonyme") != 0) {
	  convert_to_ascii(login, p, TRIBUNE_LOGIN_MAX_LEN);
	} else {
	  login[0] = 0;
	}

	if (roll_back_cnt == 0 || tribune_find_id(&dlfp->tribune,id) == NULL) {
	  if (roll_back_cnt) {
	    myprintf("%<YEL \\o/ il vient peut être d'y avoir une race condition dans le backend de tribune !> (id=%d).\n"
		     "DON'T PANIC, le coincoin gère tout ça calmement, ça prouve au moins que je\n"
		     "ne me suis pas fait chier à faire ce bugfix pour rien\n", id);
	  }
	  flag_updating_tribune++;
	  tribune_log_msg(&dlfp->tribune, ua, login, stimestamp, msg, id, my_useragent);
	  flag_updating_tribune--;
	}

	BLAHBLAH(1, printf("dlfp_updatetribune: last_post_time=%5s - last_post_id=%d\n",
			   dlfp->tribune.last_post_time, id));
	if (roll_back_cnt > 1) roll_back_cnt--;
	else if (roll_back_cnt == 1) break;
      }
    }
  err:
    if (errmsg) {
      myfprintf(stderr, "il y a un problème dans le '%s',  j'arrive plus a le parser... erreur:%<YEL %s>\n", Prefs.path_tribune_backend, errmsg);
    }
    http_close(fd);
  } else {
    myfprintf(stderr, "erreur pendant la récupération de '%<YEL %s>' : %<RED %s>\n", s, http_error());
  }

  /* cleanup .. */
  flag_updating_tribune++;
  tribune_remove_old_msg(&dlfp->tribune);
  flag_updating_tribune--;

  tribune_check_my_messages(&dlfp->tribune, old_last_post_id);

  flag_tribune_updated = 1;  
  if (dlfp->tribune.last_post_id != old_last_post_id) { /* si de nouveaux messages ont été reçus */
    dlfp_tribune_call_external(&dlfp->tribune, old_last_post_id);    
  }
}
