#include "board_priv.h"

/* pinaise j'en ai marre de faire ce genre de fonctions */
char* rss_nettoie_la_soupe_de_tags(char *src) {
  strbuf sb; strbuf_init(&sb, "");
  char *p, *p2;
  while (src[0]) {
    int skip;
    skip = 0;
    if (*src == '\t') {
      p = src+1;
      if (*p == '<') {
        p++;
        while (*p && (isspace(*p) || *p == '/')) ++p;
        p2 = p;
        while (*p2 && !isspace(*p2) && *p2 != '/' && *p2 != '>') ++p2;
        if (*p2) {
          if ((strncasecmp(p, "br", 2)==0 && p2-p == 2) ||
              (strncasecmp(p, "p", 1)==0 && p2-p == 1) ||
              (strncasecmp(p, "div", 3)==0 && p2-p == 3) ||
              (strncasecmp(p, "table", 5)==0 && p2-p == 5) ||
              (strncasecmp(p, "tr", 2)==0 && p2-p == 2) ||
              (strncasecmp(p, "td", 2)==0 && p2-p == 2)) {
            skip = 1;
          } else if (strncasecmp(p, "img", 3)==0 && p2 - p == 3) {            
            char *ps, *pe;
            printf("img tag here : %.50s\n", p);
            ps = str_case_str(p, "src=");
            pe = strstr(p, "\t>");
            if (ps && pe && ps < pe) {
              ps += 4;
              while (*ps && (isspace(*ps) || *ps == '"' || *ps == '\'')) ++ps;
              if (*ps) {                
                strbuf_cat(&sb, "\t<a href=\"./");
                while (*ps && *ps != '"' && *ps != '\'' && *ps != '\t' && !isspace(*ps)) 
                  strbuf_putc(&sb,*ps++);
                strbuf_cat(&sb, "\"\t><img>\t</a\t>");
                printf("sbuff = %s\n", sb.str);
              }
            }
            ++p; skip = 1;
          }
        }
      }
      if (skip) {
        p = strchr(src+1,'\t'); 
        if (p) { src = p+1; if (*src) src++; strbuf_putc(&sb, ' '); }
        else { return sb.str; }
      }
    }
    if (!skip) strbuf_putc(&sb, *src++);
  }
  return sb.str;
  //*dest = 0;
}


RSSBonusInfo *rss_register(Boards *boards, md5_byte_t mimd5[16], char *link, board_msg_info *mi) {
  RSSBonusInfo *ri;
  int i;
  ALLOC_OBJ(ri, RSSBonusInfo); 
  ri->link = NULL;
  ri->id = mi->id;

  if (link && strlen(link)) {
    ri->link = strdup(link);
  } else { 
    ri->link = strdup("(none)");
  }
  md5_digest(ri->link,ri->linkmd5);

  //printf("reghister = '%s', md5 = %02x%02x..%02x\n", link, ri->linkmd5[0], ri->linkmd5[1], ri->linkmd5[15]);

  memcpy(ri->md5,mimd5,16);
  if (boards->nb_rss_e == boards->max_rss_e) {
    boards->max_rss_e *= 2; 
    boards->rss_e = realloc(boards->rss_e, sizeof(RSSBonusInfo*)*boards->max_rss_e); 
    assert(boards->rss_e);
  }
  boards->rss_e[boards->nb_rss_e++] = ri;
  for (i = boards->nb_rss_e - 1; i; --i) {
    if (memcmp(boards->rss_e[i-1]->linkmd5, boards->rss_e[i]->linkmd5, 16) > 0) {
      RSSBonusInfo *tmp = boards->rss_e[i];
      boards->rss_e[i] = boards->rss_e[i-1]; 
      boards->rss_e[i-1] = tmp;
    } else break;
  }
  /*for (i=0; i < boards->nb_rss_e; ++i) {
    printf(" rss %d/%d : %02x%02x..%02x\n", i, boards->nb_rss_e, boards->rss_e[i]->linkmd5[0], boards->rss_e[i]->linkmd5[1], boards->rss_e[i]->linkmd5[15]);
    }*/
  return ri;
}

RSSBonusInfo *
rss_find_from_link(Boards *boards, char *link) {
  int i0, i1;
  md5_byte_t md5[16];
  md5_digest(link,md5);
  //printf("link = '%s', md5 = %02x%02x..%02x\n", link, md5[0], md5[1], md5[15]);
  i0 = 0; i1 = boards->nb_rss_e-1;
  while (i1>=i0) {
    int i = (i0 + i1)/2;
    //printf("i0=%d, i1=%d, compare with %02x%02x..%02x\n", i0, i1, boards->rss_e[i]->linkmd5[0], boards->rss_e[i]->linkmd5[1], boards->rss_e[i]->linkmd5[15]);
    int cmp = memcmp(boards->rss_e[i]->linkmd5, md5, 16);
    if (i1 <= i0 && cmp) return NULL;        
    if (cmp < 0) {
      i0 = i+1;
    } else if (cmp > 0) {
      i1 = i-1;
    } else return boards->rss_e[i];
  }
  return NULL;
}

RSSBonusInfo *
rss_find_from_id(Boards *boards, id_type id) {
  int i;
  for (i=0; i < boards->nb_rss_e; ++i) {
    if (id_type_eq(boards->rss_e[i]->id, id)) return boards->rss_e[i];
  }
  return NULL;
}


int
rss_board_update(Board *board, char *path) {
  HttpRequest r;
  //int http_err_flag = 0;
  //char *errmsg = NULL;
  char *rsstxt = NULL, *p;
  char *rss_title = NULL;
  XMLBlock xmlb;
  int pos, count, count_dir, refresh_request = -1, cest_bon_je_connais_la_suite = 0;
  time_t temps_debut = time(NULL), temps_last_modified;
  clear_XMLBlock(&xmlb);
  wmcc_init_http_request(&r, board->site->prefs, path);
  if (board->site->prefs->use_if_modified_since) { r.p_last_modified = &board->last_modified; }
  http_request_send(&r);
  if (!http_is_ok(&r)) return 1;
  wmcc_log_http_request(board->site, &r);
  rsstxt = http_read_all(&r, path);
  if (!http_is_ok(&r)) goto ratai;
  http_request_close(&r);
  if (!rsstxt || !http_is_ok(&r)) return 1; /* "not modified" */
  
  if (strlen(rsstxt)==0) goto RAS;

  /* tentative de conversion vers iso8859-15 */
  if ((pos = get_XMLBlock(rsstxt, strlen(rsstxt), "?xml", &xmlb))>=0) {
    XMLAttr *a;
    for (a = xmlb.attr; a; a = a->next) {
      if (str_case_startswith(a->name, "encoding")) {
        if (board->encoding) { free(board->encoding); board->encoding = NULL; }
        board->encoding = str_ndup(a->value,a->value_len);
        printf("%s: found encoding: value = '%s'\n", board->site->prefs->site_name, board->encoding);
        convert_to_iso8859(board->encoding, &rsstxt);
        break;
      }
    }
  }

  pos = get_XMLBlock(rsstxt, strlen(rsstxt), "title", &xmlb);
  if (pos < 0 || xmlb.content_len == 0) goto ratai;
  /*if (board->rss_title) free(board->rss_title);
    board->rss_title = str_ndup(xmlb.content, xmlb.content_len);*/
  rss_title = str_ndup(xmlb.content, xmlb.content_len);
  myprintf("got TITLE: '%<YEL %s>'\n", rss_title);

  if (board->rss_title) {
    free(board->rss_title);
  }
  board->rss_title = str_ndup(rss_title, 100);

  if (get_XMLBlock(rsstxt, strlen(rsstxt), "ttl", &xmlb) >= 0) {
    refresh_request = atoi(xmlb.content) * 60; /* en minutes */
    printf("ttl detected, %d\n", refresh_request);
  } if (get_XMLBlock(rsstxt, strlen(rsstxt), "*:updatePeriod", &xmlb) >= 0) {
    int period = 1;
    if (str_case_startswith(xmlb.content, "hour")) period = 3600;
    else if (str_case_startswith(xmlb.content, "min")) period = 60;
    if (get_XMLBlock(rsstxt, strlen(rsstxt), "*:updateFrequency", &xmlb) >= 0) {
      refresh_request = period * atoi(xmlb.content);
    }
  }
  if (refresh_request != -1 && board->site->prefs->board_check_delay < refresh_request) {
    BLAHBLAH(0, myprintf("Changing update frequency for %<grn %s> to %<MAG %d> sec.\n", rss_title, refresh_request));
    board->site->prefs->board_check_delay = refresh_request;
  }

  p = rsstxt;
  
  if (board->last_post_id == -1) {
    count = 1000; count_dir = -1;
  } else {
    count = board->last_post_id+1;
    count_dir = +1;
  }

  temps_last_modified = temps_debut;
  if (board->last_modified) {
    str_to_time_t(board->last_modified, &temps_last_modified);
    printf("last_modified='%s' -> time_t = %ld\n", board->last_modified, temps_last_modified);
  }
  do {
    int pos_next_item;
    pos_next_item = get_XMLBlock(p, strlen(p), "item", &xmlb);
    if (pos_next_item >= 0) {
      XMLBlock b2;
      char *title, *link, *description, *msg, *author, *comments_url, *pubdate, *fake_ua;
      char msgd[BOARD_MSG_MAX_LEN];
      char stimestamp[15];
      time_t timestamp = time(NULL);
      title = link = description = msg = author = comments_url = pubdate = fake_ua = NULL;

      //time_t_to_tstamp(temps_debut, stimestamp); 

      //temps_debut--; /* pour eviter d'avoir un paquet de news avec le meme tstamp */
      clear_XMLBlock(&b2);
      if (get_XMLBlock(xmlb.content, xmlb.content_len, "title", &b2) &&  b2.content_len) {
        title = str_ndup(b2.content, b2.content_len);
        //printf("found title: '%s'\n", title);        
      }
      if (get_XMLBlock(xmlb.content, xmlb.content_len, "link", &b2) &&  b2.content_len) {
        link = str_ndup(b2.content, b2.content_len);
        //printf("found link: '%s'\n", link);
      }
      if (get_XMLBlock(xmlb.content, xmlb.content_len, "description", &b2) &&  b2.content_len) {
        description = str_ndup(b2.content, b2.content_len);
      }
      if (get_XMLBlock(xmlb.content, xmlb.content_len, "author", &b2) &&  b2.content_len) {
        author = str_ndup(b2.content, b2.content_len);
        //printf("found author: '%s'\n", author);
      }
      if (get_XMLBlock(xmlb.content, xmlb.content_len, "comments", &b2) &&  b2.content_len) {
        comments_url = str_ndup(b2.content, b2.content_len);
      }
      /* format date: http://www.w3.org/TR/NOTE-datetime */
      if (get_XMLBlock(xmlb.content, xmlb.content_len, "pubDate", &b2) &&  b2.content_len) {
        pubdate = str_ndup(b2.content, b2.content_len);
      }
      if (pubdate == NULL && get_XMLBlock(xmlb.content, xmlb.content_len, "*:date", &b2) &&  b2.content_len) {
        pubdate = str_ndup(b2.content, b2.content_len);        
      }

      /* une petite remarque pour poser la problematique calmement:
         Comment determiner raisonnablement la date de publication d'une news
          - <pubDate>date_format_rfc_822</pubDate>

          - <dc:date>date_iso_8601</dc:date>

          - sinon :
            . si la news était connue par wmcc lors de sa precedente execution,
            on reprend la meme date sans paniquer.

            . sinon, on prend l'heure courante.
               * si  le serveur web a renvoye un last-modified, on prend cette valeur.

         Pour un fun toujours plus extreme, il faut bien gérer tous les
         problemes de timezone:
           PUTAIN DE BORDERL DE MARDE
      */

      /* c'est trop la merde avec les decalages horaires.. */
      if (pubdate) {
          if (str_to_time_t(pubdate, &timestamp)) {
          time_t_to_tstamp(timestamp, stimestamp);
          myprintf("converted %<YEL %s> to %<YEL %s> !\n", pubdate, stimestamp);
        } else BLAHBLAH(0, printf("could not convert '%s' to a valid date..\n", pubdate));
      }

      timestamp = MIN(timestamp, temps_debut);
      timestamp = MIN(timestamp, temps_last_modified);
      time_t_to_tstamp(timestamp, stimestamp);

      destroy_XMLBlock(&b2);
      
      str_trunc_nice(description, 512);
      if (link) {
        char *p = strstr(link, "*http://"); // enleve une couche de merde dans les liens yahoo
        if (p) { p++; memmove(link, p, strlen(p)+1); }
      }

      msg = NULL;
      if (title && link) msg = str_cat_printf(msg, "{&lt;a href=&quot;%s&quot;&gt;&lt;u&gt;&lt;b&gt;%s&lt;/b&gt;&lt;/u&gt;&lt;/a&gt;}", link, title);
      else if (title) msg = str_cat_printf(msg, "{&lt;b&gt;%s&lt;/b;&gt}", title);
      else if (link) msg = str_cat_printf(msg, "{&lt;a href=&quot;%s&quot;&gt;[News]&lt;/a&gt;}", link);
      if (description) msg = str_cat_printf(msg, " %s", description);
      if (comments_url) msg = str_cat_printf(msg, " &lt;a href=&quot;%s&quot;&gt;[comments]&lt;/a&gt;", comments_url);
      if (msg) {
        md5_byte_t md5[16];
        md5_state_t ms; md5_init(&ms);
        int was_already_viewed = 0;
        if (title) md5_append(&ms, title, strlen(title));
        if (link) md5_append(&ms, link, strlen(link));
        if (description) md5_append(&ms, description, strlen(description));
        md5_finish(&ms,md5);

        /* cherche le news dans le cache (au premier dl uniquement) */
        if (board->oldmd5) {
          md5_and_time *m = find_md5_in_md5_array(md5,board->oldmd5);
          if (m && strlen(m->tstamp) == 14) {
            was_already_viewed = m->viewed;
            strcpy(stimestamp, m->tstamp);
            BLAHBLAH(0, myprintf("the news '%<GRN %s>' was found in the cache!\n", title));
          }
        }

        /* cherche dans la liste des news dejà lues (après le premier dl) */
        if (board_find_md5(board, md5)) {
          myprintf("the news %<MAG %s>/%<CYA %s> is already known\n", rss_title, md5txt(md5));
          //cest_bon_je_connais_la_suite = 1; // si on suppose que les rss se remplissent toujours par le haut..
        } else {
          board_msg_info *mi;
          id_type id;
          id_type_set_lid(&id, count);
          id_type_set_sid(&id, board->site->site_id);
          //board_decode_message(board, msgd, msg);


          /* nettoyage des codes < 32 dans le message */
          {
            int i; 
            for (i=0; i < BOARD_MSG_MAX_LEN && msg[i]; ++i)
              if ((unsigned char)msg[i] < ' ') msg[i] = ' ';
          }
          
          fake_ua = str_printf("%s", rss_title ? rss_title : "?");
          if (pubdate) { fake_ua = str_cat_printf(fake_ua, " pubDate: %s", pubdate); }

          /* attention, les '&lt;' deviennent '\t<' et les '&amp;lt;' devienne '<' */
          board_decode_message(board, msgd, msg);

          {
            char *soupe = rss_nettoie_la_soupe_de_tags(msgd);
            strncpy(msgd, soupe, sizeof msgd); free(soupe); msgd[(sizeof msgd) - 1] = 0;
          }
          assert(board_find_id(board,id_type_lid(id)) == NULL);
          flag_updating_board++;
          if (author && strlen(author)) {
            author = str_cat_printf(author, "@%s", rss_title);
            mi = board_log_msg(board, fake_ua, author, stimestamp, msgd, id_type_lid(id), NULL);
          } else 
            mi = board_log_msg(board, fake_ua, rss_title, stimestamp, msgd, id_type_lid(id), NULL);
          board->nb_msg_at_last_check++;
          if (!was_already_viewed) board->nb_msg_since_last_viewed++;
          mi->ri = rss_register(board->boards, md5, link, mi);
          flag_updating_board--;
          count += count_dir;
        }
      }
      FREE_STRING(title); FREE_STRING(link); FREE_STRING(description); FREE_STRING(author); 
      FREE_STRING(comments_url); FREE_STRING(msg); FREE_STRING(pubdate); FREE_STRING(fake_ua);
    } else { 
      printf("fin de '%s'\n", rss_title);
      break;
    }
    
    p += pos_next_item;
  } while (!cest_bon_je_connais_la_suite);

 RAS:
  if (board->oldmd5 && board->last_post_id > 0) release_md5_array(board);
  destroy_XMLBlock(&xmlb);
  FREE_STRING(rss_title);
  FREE_STRING(rsstxt); 
  return 0;
 ratai:
  if (board->oldmd5 && board->last_post_id > 0) release_md5_array(board);
  destroy_XMLBlock(&xmlb);
  FREE_STRING(rss_title);
  FREE_STRING(rsstxt);
  return 1;
}
