#include "board_priv.h"

int pop3_ok(const char *s) {
  if (s == NULL || strlen(s) < 3 || strncmp(s, "+OK", 3)) return 0;
  else return 1;
}

int pop3_end_multiline(char *s) {
  str_rtrim_lf(s);
  if (strlen(s) == 1 && s[0] == '.') return 1;
  else return 0;
}

int
pop3_wait_ok_line(TelnetSession *ts) {
  char line[512];
  if (ts->error) return 0;
  telnet_get_line(ts, line, 512);
  if (!ts->error) {
    return pop3_ok(line);
  }
  return 0;
}

static void unhtml_body(char *txt) {
  char *src, *dest, in_tag = 0;
  printf("dehtmlization de %.40s\n", txt);
  for (src = txt, dest = txt; *src; ++src) {
    if (*src == '<' && !in_tag) in_tag++;
    else if (*src == '>' && in_tag != 0) in_tag--;
    else if (!in_tag) { *dest++ = *src; }
  }
  *dest=0;
}

int
pop3_check_message(Board *board, TelnetSession *ts, int msgnum, const char *uidl, int *pcount) {
  int was_already_viewed = 0;
  md5_byte_t md5[16];
  md5_digest(uidl,md5);
  /* cherche le news dans le cache (au premier dl uniquement) */
  if (board->oldmd5) {
    md5_and_time *m = find_md5_in_md5_array(md5,board->oldmd5);
    if (m && strlen(m->tstamp) == 14) {
      was_already_viewed = m->viewed;
      BLAHBLAH(0, myprintf("the mail '%<GRN %s>' was found in the cache!\n", uidl));
    }
  }
  /* cherche dans la liste des news dejà lues (après le premier dl) */
  if (board_find_md5(board, md5)) {
    myprintf("the mail %<MAG %s>/%<CYA %s> is already known\n", ts->host, uidl);
  } else {
    char ua[100];
    char subject[100];
    char body[4000];
    char sender[100];
    char mime_boundary[100];
    char s[40]; 
    int in_body = 0, html_junk=0;
    time_t timestamp = time(NULL);
    ua[0] = 0; subject[0] = 0; body[0] = 0; sender[0] = 0; mime_boundary[0] = 0;
    snprintf(s, 40, "TOP %d 20\r\n", msgnum);
    BLAHBLAH(1,myprintf("sending %s", s));
    telnet_send(ts, s);
    if (!pop3_wait_ok_line(ts)) return -1;
    do {
      char line[512];
      telnet_get_line(ts, line, 512); str_rtrim_lf(line);
      if (ts->error) return -2;
      if (line[0] == '.') {
        if (strlen(line) == 1) { 
          break;
        } else memmove(line, line+1, strlen(line)+1);
      }
      if (str_startswith(line, "User-Agent:")) {
        strncpy(ua, line+11, 100); ua[99] = 0; str_trim(ua); 
      }
      if (str_startswith(line, "X-Mailer:")) {
        strncpy(ua, line+9, 100); ua[99] = 0; str_trim(ua); 
      }
      if (str_startswith(line, "Return-Path:")) {
        strncpy(sender, line+12, 100); sender[99] = 0; str_trim(sender); 
      }
      if (str_startswith(line, "Subject:")) {
        strncpy(subject, line+8, 100); subject[99] = 0; str_trim(subject);         
      }
      if (str_startswith(line, "Date:")) {
        if (str_to_time_t(line+5, &timestamp)) {
          printf("got time! : %ld\n", (long)timestamp);
          if (timestamp > time(NULL)) timestamp = time(NULL);
        }
        else printf("could not convert '%s' to a date\n", line+5);
      }
      if (!in_body && strstr(line, "boundary=")) {
        char *sbound = strstr(line, "boundary=");
        if (sbound) { 
          str_rtrim_lf(line);
          char *tmp = str_dup_unquoted(sbound+9);
          strncpy(mime_boundary, tmp, 100); mime_boundary[99] = 0; 
          printf("looking mime-boundary: '%s'\n", mime_boundary); 
          free(tmp);
        }
      } else if (strlen(mime_boundary) && strstr(line, mime_boundary)) {
        printf("found mime-boundary %s\n", mime_boundary);
        mime_boundary[0] = 0;
      }
      if (!in_body && strstr(line, "text/html")) {
        html_junk = 1;
      }
      if (strlen(line) == 0 && mime_boundary[0] == 0) in_body = 1;
      else if (in_body) {
        strncat(body, " ", 3999 - strlen(body)); body[3999] = 0;
        strncat(body, line, 3999 - strlen(body)); body[3999] = 0;
      }
    } while (1);
    if (strlen(sender) == 0) strcpy(sender, "(unknown)");    
    if (html_junk) unhtml_body(body);
    str_trunc_nice(body, 512);
    { int i; for (i = 0; body[i]; ++i) if (body[i] > 0 && body[i] < ' ') body[i] = ' '; }
    printf("fin, tout est ok: subject=%s\nbody=%s\nua=%s\nsender=%s\n\n", subject, body, ua, sender);
    {
      board_msg_info *mi = NULL;
      id_type id;
      char msgd[BOARD_MSG_MAX_LEN];
      char *subject2, *body2, /**ua2,*/ /**sender2,*/ *msg = NULL;
      char stimestamp[15];
      static const char *htmlkeys[] = { "<", ">", "&", "\"" };
      static const char *htmlsubs[] = { "&lt;", "&gt;", "&amp;", "&quot;" };

      subject2 = str_multi_substitute(subject, htmlkeys, htmlsubs, 4);
      body2 = str_multi_substitute(body, htmlkeys, htmlsubs, 4);
      //ua2 = str_multi_substitute(ua, htmlkeys, htmlsubs, 4);
      //sender2 = str_multi_substitute(sender, htmlkeys, htmlsubs, 4);
      msg = str_printf("<u><b>%s</b></u> -- %s", subject2, body2);
      id_type_set_lid(&id, *pcount); (*pcount)++;
      id_type_set_sid(&id, board->site->site_id);
      board_decode_message(board, msgd, msg);
      time_t_to_tstamp(timestamp, stimestamp);

      assert(board_find_id(board,id_type_lid(id)) == NULL);

      flag_updating_board++;
      mi = board_log_msg(board, ua, sender, stimestamp, msgd, id_type_lid(id), NULL);
      if (!mi->in_boitakon) {
        board->nb_msg_at_last_check++;
      }
      if (!was_already_viewed) board->nb_msg_since_last_viewed++;
      mi->ri = rss_register(board->boards, md5, NULL, mi);
      flag_updating_board--;

      free(msg); /*free(ua2);*/ free(body2); free(subject2); /*free(sender2);*/
    }
  }
  return 0;
}

int pop3_board_update(Board *board, char *path) {
  TelnetSession ts; 
  SplittedURL su;
  char greeting[512],line[512];
  char *user, *pass, *a, *b;
  int nb_msg = 0, mailbox_size = 0, msgcnt;
  char **uidls = NULL;
  int count, use_normal_auth = 1;
  printf("update de %s\n",path);
  user = board->site->prefs->pop3_user;
  pass = board->site->prefs->pop3_pass;
  if (str_is_empty(user) || str_is_empty(pass)) {
    myprintf("%<RED I can't log you into %s, stupid !> no login/pass known for this site (fill .wmcoincoin/options.auth)\n", path);
    return 1;
  };

  if (board->last_post_id == -1) {
    count = 1000; 
  } else {
    count = board->last_post_id+1;
  }

  telnet_session_init(&ts);
  if (split_url(path,&su)) assert(0);
  assert(su.type == POP3_URL || su.type == APOP_URL);
  ts.host = strdup(su.host);
  ts.port = su.port;
  telnet_session_open(&ts);
  telnet_get_line(&ts, greeting, 512);
  BLAHBLAH(1,myprintf("got greeting from %s : '%<YEL %s>'\n", path, greeting));
  if (ts.error || !pop3_ok(greeting)) goto err;
  if (0 && /* APOP ne marche pas sur free :-/ pas pigé pourquoi, le digest a pourtant l'air bon */
      strlen(greeting) > 8 && (a=strrchr(greeting, '<')) && (b=strrchr(greeting, '>')) && b > a) { /* tentative APOP */    
    char *msgid, *uid, *apop_cmd = NULL;
    md5_byte_t md5[16]; 
    BLAHBLAH(1,printf("trying APOP\n"));
    uid = str_ndup(a, b-a + 1);
    msgid = str_printf("%s%s", uid, pass);
    printf("msgid = '%s'\n", msgid);
    memset(md5,0,16);
    md5_digest(msgid, md5);
    apop_cmd = str_printf("APOP %s %s\r\n", user, md5txt(md5));
    BLAHBLAH(1,printf("sending %s", apop_cmd));
    telnet_send(&ts, apop_cmd);
    if (pop3_wait_ok_line(&ts)) use_normal_auth = 0;
    free(uid);
    free(apop_cmd);
    free(msgid);
  }
  if (use_normal_auth) {
    BLAHBLAH(1,myprintf("sending USER %s\n", user));
    telnet_send(&ts, "USER "); telnet_send(&ts, user); telnet_send(&ts, "\r\n");
    if (!pop3_wait_ok_line(&ts)) goto err;
    BLAHBLAH(1,myprintf("sending PASS ..\n", pass));
    telnet_send(&ts, "PASS "); telnet_send(&ts, pass); telnet_send(&ts, "\r\n");
    if (!pop3_wait_ok_line(&ts)) goto err;
  }
  telnet_send(&ts, "STAT\r\n");
  telnet_get_line(&ts, line, 512);
  if (ts.error || !pop3_ok(line)) goto err;
  if (sscanf(line, "+OK %d %d", &nb_msg, &mailbox_size) != 2) goto err;
  telnet_send(&ts, "UIDL\r\n");
  if (!pop3_wait_ok_line(&ts)) goto err;
  ALLOC_VEC(uidls, nb_msg, char *);
  for (msgcnt = 0; msgcnt < nb_msg; ++msgcnt) {
    char uidl[500];
    int n;
    telnet_get_line(&ts, line, 512);
    if (ts.error) goto err;
    if (sscanf(line, "%d %500s", &n, uidl) != 2 || n != msgcnt+1) goto err;
    uidls[msgcnt] = strdup(uidl);
    printf("got uidl=%s\n", uidl);
  }
  telnet_get_line(&ts, line, 512);
  if (!pop3_end_multiline(line)) goto err;
  for (msgcnt = 0; msgcnt < nb_msg; ++msgcnt) {
    if (pop3_check_message(board, &ts, msgcnt+1, uidls[msgcnt], &count) != 0) {
      printf("erreur pendant le d/l de %s ..\n", uidls[msgcnt]); goto err;
    }
  }

  BLAHBLAH(1,myprintf("sending QUIT\n"));
  telnet_send(&ts, "QUIT\r\n");
  if (uidls) { int i; for (i = 0; i < nb_msg; ++i) FREE_STRING(uidls[i]); free(uidls); }
  telnet_session_close(&ts);
  return 0;
 err:
  if (uidls) { int i; for (i = 0; i < nb_msg; ++i) FREE_STRING(uidls[i]); free(uidls); }
  telnet_session_close(&ts);
  return 1;
}
