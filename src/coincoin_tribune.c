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
  rcsid=$Id: coincoin_tribune.c,v 1.10 2002/01/13 19:44:48 pouaite Exp $
  ChangeLog:
  $Log: coincoin_tribune.c,v $
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
   - n'est accédé que dans ce fichier */
char tribune_last_modified[512] = "";

/* utilise tres localement, c'est la longueur DANS remote.rdf, la longueur réelle sera moindre
   (remplacement de &eacute par 'é' etc... ) */
#define TRIBUNE_UA_MAX_LEN 1000
#define TRIBUNE_MSG_MAX_LEN 6000
#define TRIBUNE_LOGIN_MAX_LEN 60

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


#define TF_HASH_SZ 2048
void
tribune_frequentation(const DLFP_tribune *trib, int nb_minutes, int *ua_cnt, int *msg_cnt) {
  unsigned short hash_cnt[TF_HASH_SZ];
  unsigned hash_val;
  const unsigned c2 = 31117, c1 = 11279; 

  time_t t_last;

  tribune_msg_info *it;
  int i;
  

  

  *ua_cnt = 0;
  *msg_cnt = 0;

  it = tribune_find_id(trib, trib->last_post_id);
  if (it == NULL) return;
  t_last = it->timestamp;
  t_last += trib->nbsec_since_last_msg;

  bzero(hash_cnt, sizeof(hash_cnt));
  it = trib->msg; 
  while (it) {
    if (difftime(t_last, it->timestamp) < nb_minutes*60) {
      /* au pifometre... faudra ptet verifier qu'on hache correctement */
      hash_val = 1;
      for (i=0; it->useragent[i]; i++) {
	hash_val = ((hash_val*((unsigned char)it->useragent[i] + c1)) + 1) % c2;
      }
      hash_val = hash_val % TF_HASH_SZ;
      hash_cnt[hash_val]++;
      (*msg_cnt)++;
    }

    it = it->next;
  }
  for (i=0; i < TF_HASH_SZ; i++) {
    if (hash_cnt[i]) (*ua_cnt)++;
  }
}

/* essaye d'identifier le user agent selon
   les regle (regular expression) definies dans trib->rules */
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

/* renvoie l'age du message, en secondes */
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
    free(trib->msg);
    cnt--;
    trib->msg = it;
  }

    //    age = tribune_get_msg_age(trib, it);
    //    BLAHBLAH(4, printf("tribune_remove_old_msg: test de id=%d: son age est %d secondes (max=%d) (nbsec_since_last_msg=%d, last_timestamp=%02d:%02d, time_stamp=%02d:%02d)\n", it->id, age,TRIBUNE_LOAD_NB_MINUTES*60, trib->nbsec_since_last_msg, ts2hm(trib->last_post_timestamp), ts2hm(it->timestamp)));
    //    if (age > (TRIBUNE_LOAD_WIDTH*8+7)*60) {
    //if (age > 7200) {
  /*
    if (cnt > TRIBUNE_MAX_MSG) {
      if (pit) {
	pit->next = it->next;
	free(it);
	it = pit->next;
      } else {
	trib->msg = it->next;
	free(it);
	it = trib->msg;
      }
    } else {
      pit = it;
      it = it->next;
    }
  }
  */

}

time_t
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
    if (strncmp(s, "<!--",4)==0 && in_comment == 0) {
      w = strstr(s, "-->");
      if (w) {
	in_comment = w+3-s;
      }
    }
    if (in_comment == 0) {
      *p = *s; p++;
    } else in_comment--;
  }
  *p = 0;
  return outmsg;
}

static tribune_msg_info *
tribune_log_msg(DLFP_tribune *trib, char *ua, char *login, char *stimestamp, char *_message, int id)
{
  tribune_msg_info *nit, *pit, *it;
  char *message = NULL;

  message = nettoie_message_tags(_message);
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

  BLAHBLAH(3, printf("log msg id=%d, login=%s timestamp=%u msg='%s'\n", id, it->login, (unsigned)it->timestamp, it->msg));

  /* et on n'oublie pas..*/
  trib->nbsec_since_last_msg = 0;

  /* remise a jour du flag d'affichage des secondes */
  update_secondes_flag(trib);

  /* essaye d'identifier le user agent */
  tribune_tatouage(trib, it);

  /* evalue le potentiel trollesque */
  troll_detector(it);

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
*/
void
dlfp_tribune_call_external(const DLFP_tribune *trib, int last_id)
{
  tribune_msg_info *it;
  

  if (Prefs.post_cmd == NULL) {
    return;
  } else {
    BLAHBLAH(1, myprintf("dlfp_tribune_call_external, id=%d - %d\n", last_id, trib->last_post_id));
  }
  if (last_id != -1) { /* si ce n'est pas le premier appel.. */
    it = tribune_find_id(trib, last_id);
  } else {
    //    it = trib->msg;
    return; /* à l'initialisation, on évite de passer tous les messages dans le coincoin */
  }
  while (it) {
    char *qlogin;
    char *qmessage;
    char *qua;
    char sid[20], stimestamp[20], strollscore[20];
    char *shift_cmd, *s2;
    
    //----< Code pour passer les infos d'un post à une commande extérieure >

    qlogin = shell_quote(it->login);
    qmessage = shell_quote(it->msg);
    qua = shell_quote(it->useragent);
    snprintf(sid, 20, "%d", it->id);
    snprintf(stimestamp, 20, "%ld", it->timestamp);
    snprintf(strollscore, 20, "%d", it->troll_score);

    shift_cmd = strdup(Prefs.post_cmd);
    s2 = str_substitute(shift_cmd, "$l", qlogin); free(shift_cmd); shift_cmd = s2; 
    s2 = str_substitute(shift_cmd, "$m", qmessage); free(shift_cmd); shift_cmd = s2; 
    s2 = str_substitute(shift_cmd, "$u", qua); free(shift_cmd); shift_cmd = s2; 
    s2 = str_substitute(shift_cmd, "$i", sid); free(shift_cmd); shift_cmd = s2; 
    s2 = str_substitute(shift_cmd, "$t", stimestamp); free(shift_cmd); shift_cmd = s2; 
    s2 = str_substitute(shift_cmd, "$s", strollscore); free(shift_cmd); shift_cmd = s2; 
    BLAHBLAH(2, myprintf("post_cmd: /bin/sh -c %<YEL %s>\n", shift_cmd));
    system(shift_cmd);

    free(shift_cmd);
    free(qlogin);
    free(qmessage);
    free(qua);

    //----</ Code >
    it = it->next;
  }
}


void
dlfp_updatetribune(DLFP *dlfp)
{
  time_t now;
  char s[8192];
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

  snprintf(s, 8192, "%s%s/%s", (strlen(Prefs.site_path) ? "/" : ""), Prefs.site_path, Prefs.path_tribune_backend);
  if ((Prefs.debug & 2) == 0) {
    fd = http_get(Prefs.site_root, Prefs.site_port, s, 
		  Prefs.proxy_name, Prefs.proxy_auth, Prefs.proxy_port, APP_USERAGENT, tribune_last_modified);
  } else {
    snprintf(s, 8192, "%s/wmcoincoin/test/remote.rdf", getenv("HOME"));
    myprintf("DEBUG: ouverture de '%<RED %s>'\n", s);
    fd = open(s, O_RDONLY);
  }
  if (fd != INVALID_SOCKET) {
    while (http_get_line(s, 8192, fd) > 0) {
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
	if (tribune_find_id(&dlfp->tribune,id)) {
	  break;
	}

	if (http_get_line(s, 8192, fd) <= 0) { errmsg="httpgetline(info)"; goto err; }

	if (strncasecmp(s, tribune_sign_info,strlen(tribune_sign_info))) { errmsg="infosign"; goto err; }
	if (strncasecmp("</info>", s+strlen(s)-7,7)) { errmsg="</info>"; goto err; }
	s[strlen(s)-7] = 0; /* vire le /info */
	p = s + strlen(tribune_sign_info);

        convert_to_ascii(ua, p, TRIBUNE_UA_MAX_LEN, 1);

	if (http_get_line(s, 8192, fd) <= 0) { errmsg="httpgetline(message)"; goto err; }
	if (strncasecmp(s, tribune_sign_msg,strlen(tribune_sign_msg))) { errmsg="messagesign"; goto err; }
	

	// il arrive que le post tienne sur plusieurs lignes (je sais pas pourquoi) 
	{
	  int l;
	  l = strlen(s);
	  while (strncasecmp("</message>", s+l-10,10)) {
	    if (http_get_line(s+l, 8192 - l, fd) <= 0) {
	      errmsg="</message>"; goto err; 
	    }
	    l = strlen(s);
	  }
	}

	

	s[strlen(s)-10] = 0; /* vire le </message> */
	p = s + strlen(tribune_sign_msg);
	convert_to_ascii(msg, p, TRIBUNE_MSG_MAX_LEN, 1);

	if (http_get_line(s, 8192, fd) <= 0) { errmsg="httpgetline(login)"; goto err; }
	if (strncasecmp(s, tribune_sign_login,strlen(tribune_sign_login))) { errmsg="messagesign_login"; goto err; }
	if (strncasecmp("</login>", s+strlen(s)-8,8)) { errmsg="</login>"; goto err; }

	s[strlen(s)-8] = 0; 
	p = s + strlen(tribune_sign_login);
	if (strcasecmp(p, "Anonyme") != 0) {
	  convert_to_ascii(login, p, TRIBUNE_LOGIN_MAX_LEN, 1);
	} else {
	  login[0] = 0;
	}
	//	printf("login = '%s'\n", login);

	flag_updating_tribune++;
	tribune_log_msg(&dlfp->tribune, ua, login, stimestamp, msg, id);
	flag_updating_tribune--;
	    
	BLAHBLAH(1, printf("dlfp_updatetribune: last_post_time=%5s - last_post_id=%d\n",
			   dlfp->tribune.last_post_time, id));
	//	myprintf("%<CYA %s> %<YEL %.10s> '%<GRN %s>'\n", stimestamp+8, ua, msg);
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

  flag_tribune_updated = 1;  

  if (dlfp->tribune.last_post_id != old_last_post_id) { /* si de nouveaux messages ont été reçus */
    dlfp_tribune_call_external(&dlfp->tribune, old_last_post_id);
  }
}
