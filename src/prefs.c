#define __PREFS_C
#include "config.h"
#include "prefs.h"
#include "coin_util.h"
#include "myprintf.h"
#include <sys/utsname.h> /* pour la fonction uname */

#define BICOLOR_SET(x,a,b) { x.opaque = a; x.transp = b; }

static void
coincoin_default_useragent(char *s, int sz)
{
  struct utsname utsn;
  
  if (uname(&utsn) != -1) {
    snprintf(s, sz, "wmCoinCoin/" VERSION " (palmipede; %s %s %s)",
	     utsn.sysname, utsn.release, utsn.machine);
  } else {
    snprintf(s, sz, "wmCoinCoin/" VERSION " (palmipede; ?)");
  }
}


/*
  lecture d'une option (qui peut être multi-ligne, si celles ci se terminent par le caractère '\')
  remplit le nom de l'option, ainsi que son argument (concaténé si il est multiligne)

  en cas d'erreur, le message est renvoyé
 */
static char *
read_option_line(FILE *f, int *lcnt, char **opt_name, char **opt_arg)
{
  int i;
  char *s,*p;
  int multiline_opt;

  assert(*opt_name == NULL); 
  assert(*opt_arg == NULL); 

  /* lecture en sautant tous les commentaires */
  do {
    s = str_fget_line(f); str_trim(s); (*lcnt)++;
    if (s[0] == '#' ||
	(s[0] == 0 && !feof(f))) { FREE_STRING(s); }
  } while (s == NULL && !feof(f));
  
  if (s == NULL || s[0]==0) { FREE_STRING(s); return NULL; } /* fin du fichier */
  
  /* il faut commencer par lire le nom de l'option */
  p = strchr(s, ':');
  if (p == NULL || p == s) { 
    FREE_STRING(s); 
    return str_printf("[ligne %d] impossible de trouver un nom d'option", *lcnt); 
  }
  *p = 0; p++;
  *opt_name = strdup(s);
  if (strlen(p)) { 
    str_trim(p); 
  } else {
    FREE_STRING(s); 
    return str_printf("[ligne %d] la valeur de l'option '%s' manque", *lcnt, *opt_name); 
  }
  
  /* a ce p pointe vers la valeur de l'option */
  do {
    char *old_opt_arg;
    multiline_opt = 0;
    i = strlen(p);
    if (i && p[i-1] == '\\') { p[i-1] = 0; str_trim(p); multiline_opt = 1; }

    old_opt_arg = *opt_arg;
    if (old_opt_arg) {
      *opt_arg = str_printf("%s%s", *opt_arg, p); FREE_STRING(old_opt_arg);
    } else {
      *opt_arg = strdup(p);
    }
    FREE_STRING(s);

    if (multiline_opt) {
      s = str_fget_line(f); str_trim(s); (*lcnt)++;
    }
    p = s;
  } while (multiline_opt && s);
  return NULL;
}


static int
option_get_bool_val(const char *optarg) {
  if (optarg==NULL || optarg[0]==0) return 1;
  else if (strcmp(optarg, "1")==0) return 1;
  else if (strcasecmp(optarg, "on")==0) return 1;
  else if (strcasecmp(optarg, "yes")==0) return 1;
  else if (strcasecmp(optarg, "true")==0) return 1;
  else if (strcmp(optarg, "0")==0) return 0;
  else if (strcasecmp(optarg, "off")==0) return 0;
  else if (strcasecmp(optarg, "no")==0) return 0;
  else if (strcasecmp(optarg, "false")==0) return 0;
  else return -1;
}

static char *
option_get_xypos_val(const char  *_optarg, 
		     const char  *optname,
		     int *xpos, int *ypos)
{
  char *s;
  char *optarg;
  optarg = strdup(_optarg);
  if ((s = strchr(optarg, ':')) != NULL) {
    *s = 0;
    *xpos = atoi(optarg);
    *ypos = atoi(s+1);
    free(optarg);
    return NULL;
  } else {
    free(optarg);
    return str_printf("option '%s' invalide, on attendait deux nombre x et y au format x:y", optname);
  }
}

static char *
option_get_transp_val(const char *arg, const char * opt_name,TransparencyInfo *ti) {
  if (strcasecmp(arg, "full")==0) {
    ti->type = FULL_TRANSPARENCY;
  } else if (strncasecmp(arg, "shading",7)==0) {
    ti->type = SHADING;
    if (sscanf(arg+7, "%d %d", &ti->shade.luminosite, &ti->shade.assombrissement) != 2 || 
	ti->shade.luminosite < 0 || ti->shade.luminosite > ti->shade.assombrissement ||
	ti->shade.assombrissement > 100) {
      return str_printf("option '%s' invalide, le shading est défini par deux valeur x et y comprises "
			"entre 0 et 100, telles que x < y (plus x est grand, plus l'image finale sera "
			"lumineuse, plus y est petit plus les couleurs de l'image initiale sont "
			"attenuées", opt_name);
    }
  } else if (strncasecmp(arg, "tinting",7)==0) {
    ti->type = TINTING;
    if (sscanf(arg+7, "%lx %lx", &ti->tint.black, &ti->tint.white) != 2) {
      if (ti->tint.white > 0xffffff || ti->tint.black > 0xffffff) {
	return str_printf("option '%s' invalide, il faut spécifier deux couleurs RGB correspondant aux teintes du noir, et du blanc", opt_name);
      }
    }
  } else {
    return str_printf("option '%s' invalide, commencer par préciser le type de transparence: FULL, SHADING ou TINTING", opt_name);
  }
  return NULL;
}

static void
option_set_useragent(const char *optarg,
                     structPrefs *p)
{
  if (optarg == NULL) p->user_agent[0] = 0;
  else {
    struct utsname utsn;
    char *ua;
    const char *keys[] = {"$v", "$u", "$s", "$r", "$m"};
    const char *subs[] = {VERSION,"", ""  , ""  , ""};


    ua = str_substitute(optarg, "$v", VERSION);
    if (p->user_name) {
      subs[1] = p->user_name;
    } else {
      subs[1] = "[!palmipede.user_name!]";
    }
    if (uname(&utsn) != -1) {
      subs[2] = utsn.sysname;
      subs[3] = utsn.release;
      subs[4] = utsn.machine;
    }
    ua = str_multi_substitute(optarg, keys, subs, 5);
    strncpy(p->user_agent,ua,USERAGENT_MAX_LEN);
    p->user_agent[USERAGENT_MAX_LEN] = 0;

    free(ua);
  }
}

static void
option_site_root (const char  *optarg,
		  structPrefs *prefs)
{
  char *s;
  char *p,*p2;

  assert(optarg);
  if (prefs->site_root) free(prefs->site_root);
  prefs->site_root = NULL;
  if (prefs->site_path) free(prefs->site_path);
  prefs->site_path = NULL;

  s = strdup(optarg); // pour pouvoir ecrire dedans
  p = s;
  if (strstr(p,"http://")) p+=7;
  
  /* cherche numero de port et le nom du site */
  if ((p2 = strchr(p, ':'))) {
    *p2 = 0;
    prefs->site_root = strdup(p);
    p2++; p = p2;
    
    while (*p2 >= '0' && *p2 <= '9' && *p2) p2++;
    prefs->site_port = atoi(p);
    p = p2;
    while (*p == '/') p++;
  } else {
    p2 = strchr(p,'/');
    if (p2) {
      *p2 = 0;
      p2++;
    } else {
      p2 = p + strlen(p);
    }
    prefs->site_root = strdup(p);
    p = p2;
  }
  /* recupere le path */
  if (strlen(p)) {
    p2 = p + strlen(p) - 1;
    while (p2 > p && *p2 == '/') { *p2 = 0; p2--;}
  }
  prefs->site_path = strdup(p);
  myprintf("le site principal est : %<YEL http://%s>:%<GRN %d>/%<YEL %s>%s\n", 
	   prefs->site_root, prefs->site_port, prefs->site_path,strlen(prefs->site_path) ? "/" : "");
  free(s);
}

static void
option_set_proxy(const char  *optarg,
                 structPrefs *p)
{
  char *s, *S;
  
  if (p->proxy_name) free(p->proxy_name); 
  if (optarg == NULL || strlen(optarg) == 0) {
    fprintf(stderr, "désactivation du proxy");
    p->proxy_name = NULL;
  } else {
    S = strdup(optarg);
    if ((s = strchr(S, ':')) != NULL) {
      *s = 0;
      p->proxy_name = strdup(S);
      p->proxy_port = atoi(s+1); assert(p->proxy_port>0);
    } else {
      p->proxy_name = strdup(S);	  
    }
    if (p->verbosity >= 1) myprintf("PROXY: %<YEL %s>:%<GRN %d>\n", 
				    p->proxy_name, 
				    p->proxy_port);
    free(S);
  }
}

static char*
option_browser(const char *optarg,const char *optname,
	       structPrefs *p, int browser_num)
{
  int i, pc;
  char *s;
  char **bcmd = NULL;
  int ok = 0;

  if (browser_num == 1) bcmd = &p->browser_cmd; else bcmd = &p->browser2_cmd;
  if (*bcmd) free(*bcmd);
  *bcmd = strdup(optarg); assert(*bcmd);
  s = *bcmd; assert(s);
  i = 0; pc = 0;
  /* verifie que la seule occurence de '%' est bien suivie d'un 's' */
  while (s[i]) {
    if (pc == '%') {
      if (s[i] != 's') {
	ok = 0; break;
      } else ok++;
    }
    pc = s[i]; i++;
  }
  if (ok != 1) {
    return str_printf("[option %s] il doit y avoir une et une seule "
		      "occurence du charactère '%%', et il doit être suivi d'un 's' (c'est ce qui "
		      "sera remplacé par l'url)", optname);
  }
  if (strstr(s, "'%s'")) {
    myfprintf(stderr, "%s (ligne %d) : %<YEL warning>: '%%s' == Mal, %%s == Bien (toutes les urls sont généreusement escapées)\n");
  }
  return NULL;
}

/* stocke une liste de mots-plops séparés par une virgule */
/*
static void
option_plop_words(const char *optarg, structPrefs *p)
{
  char *s;
  char *sep = ",";
  char *prov;
  int i;

  s = strdup(optarg); assert(s);



  if (p->plop_words) {
    for (i=0; p->plop_words[i]; i++) {
      free(p->plop_words[i]);
    }
    free(p->plop_words); p->plop_words = NULL;
  }


  p->nb_plop_words = 0;
  for (i=0; s[i]; i++) { if (s[i]==*sep) p->nb_plop_words++; }

  if (p->nb_plop_words == 0) {
    myprintf("vous ètes prié de séparer les mots-plops par des %<YEL VIRGULES>, tous les autres caractères sont stockés dans les mots\n");
    exit(0);
  }

  p->plop_words=(char**)malloc((p->nb_plop_words+1)*sizeof(char *)); assert(p->plop_words);

  for(i=0;i<(int)p->nb_plop_words;i++) {
    if(i==0){
      prov = strtok(s,sep);
    } else {
      prov = strtok(NULL,sep);
    }
    assert(prov); 
    p->plop_words[i]=strdup(prov);
  }
  p->plop_words[i] = NULL;
  
  free(s);
}
*/

char*
option_get_string_list(unsigned char *optarg, char *optname, char ***list, int *nb_elt)
{ 
  int pass, cnt;
  char mot[1024];

  for (pass = 0; pass < 2; pass++) {
    unsigned char *s;

    s = optarg;
    cnt = 0;
    do {
      int i;
      if (s != optarg) {
	if (*s != ',') goto erreur;
	s++;
      }
      while (*s && *s <= ' ') s++;
      if (*s != '"') goto erreur;
      s++;
      i = 0;
      while (*s && i < 1023) {
	if (*s == '\\' && *(s+1) != '"') {
	  s++;
	} else if (*s == '"') {
	  break;
	}
	mot[i++] = *s; s++;
      }
      mot[i++] = 0;
      if (*s != '"') goto erreur;
      s++;
      while (*s && *s <= ' ') s++;

      if (pass == 1) {
	(*list)[cnt] = strdup(mot);
      }
      cnt++;
    } while (*s);
    if (pass == 0) {
      *nb_elt = cnt;
      if (cnt == 0) {
	*list = NULL;
      } else {
	*list = calloc(cnt, sizeof(char*));
      }
    }
  }

  return NULL;
 erreur:
  return str_printf("erreur pour l'option '%s', on attend une liste de mots entre guillements, séparés par des virgules", optname);
}


char*
option_get_key_list(unsigned char *optarg, char *optname, KeyList **pfirst, int nummin, int nummax)
{
  int cnt;
  char mot[1024];
  unsigned char *s;

  KeyList *first = *pfirst;

  first = tribune_key_list_clear_from_prefs(first);
  s = optarg;
  cnt = 0;
  do {
    KeyListType t;
    int i,num;
    if (s != optarg) {
      if (*s != ',') goto erreur;
      s++;
    }
    while (*s && *s <= ' ') s++;
    
    t = HK_WORD;
    if (strncasecmp(s, "ua:", 3) == 0) { t = HK_UA; s+= 3; }
    if (strncasecmp(s, "login:", 6) == 0) { t = HK_LOGIN; s+= 6; }
    if (strncasecmp(s, "word:", 5) == 0) { t = HK_WORD; s+=5; }
    if (strncasecmp(s, "id:", 3) == 0) { t = HK_ID; s+= 3; }
    if (strncasecmp(s, "thread:", 7) == 0) { t = HK_THREAD; s+= 7; }
    if (strncasecmp(s, "ua_nologin:", 11) == 0) { t = HK_UA_NOLOGIN; s+= 11; }

    num = nummin;
    if (*s >= '0' && *s <= '9') { 
      num = atoi(s); while (*s >= '0' && *s <= '9') s++; if (*s != ':') goto erreur; s++;
    }
    if (num < nummin || num > nummax) {
      return str_printf("argument invalide pour l'option '%s', mot %d, la valeur de l'argument numérique (%d) doit "
			"être comprise entre %d et %d", optname, cnt, num, nummin, nummax);
    }

    if (*s != '"') goto erreur;
    s++;
    i = 0;
    while (*s && i < 1023) {
      if (*s == '\\' && *(s+1) != '"') {
	s++;
      } else if (*s == '"') {
	break;
      }
      mot[i++] = *s; s++;
    }
    mot[i++] = 0;
    if (*s != '"') goto erreur;
    s++;
    while (*s && *s <= ' ') s++;

    
    first = tribune_key_list_add(first, mot, t, num, 1);
    cnt++;
  } while (*s);
  *pfirst = first;
  return NULL;
  
 erreur:
  return str_printf("argument invalide pour l'option '%s', mot %d, (rappel: il faut spécifier une liste de [UA|LOGIN|ID|etc]:[NUM:]\"unmot\" séparés par des virgules, cf le fichier d'options)\n", optname, cnt);
}


/* remplit la structure avec les valeurs par défaut des preferences */
void
wmcc_prefs_set_default(structPrefs *p) {
  assert(p);
  p->dlfp_tribune_check_delay = 30; /* 2 fois par minute */
  p->dlfp_news_check_delay = 300; /* 1 fois toutes les 5 min */

  p->dlfp_max_refresh_delay = 30;   /* 30 minutes entre deux refresh au max */
  p->dlfp_switch_off_coincoin_delay = 24*60; /* au bout d'un jour d'inactivité, le coincoin passe en horloge et arrête les refresh */
  p->tribune_max_msg = 300;
  p->debug = 0;
  p->verbosity = 0;
  p->tribune_backend_type = 3;
  p->tribune_wiki_emulation = NULL;
  ASSIGN_STRING_VAL(p->font_encoding, "iso8859-1");
  p->news_max_nb_days = 1;
  ASSIGN_STRING_VAL(p->news_fn_family, "helvetica");
  p->news_fn_size = 12;
  p->news_bgcolor = 0xdae6e6;
  p->news_fgcolor = 0x000000;
  p->news_xpos = -10000;
  p->news_ypos = -10000;
  p->news_width = 600;
  p->news_height = 300;

  if (p->user_agent) free(p->user_agent); 
  p->user_agent = malloc(USERAGENT_MAX_LEN+1);
  coincoin_default_useragent(p->user_agent, USERAGENT_MAX_LEN+1);
  ASSIGN_STRING_VAL(p->coin_coin_message, "coin ! coin !");
  p->user_name = NULL;
  p->dock_bgcolor = (255L<<16) + (193L<<8) + 44; /* un joli jaune (je trouve) (NDKad : y'a que toi)*/
  p->dock_fgcolor = 0x000000;
  p->dock_bgpixmap = NULL;
  p->http_timeout = 40;
  p->proxy_auth = NULL;
  p->proxy_name = NULL;
  p->proxy_port = 1080;/* meme valeur par defaut que curl ... */
  p->proxy_nocache = 0;
  p->use_if_modified_since = 1;
  p->use_balloons = 1;
  ASSIGN_STRING_VAL(p->balloon_fn_family, "helvetica");
  p->balloon_fn_size = 10;
  p->use_iconwin = 1; /* style windowmaker par defaut */
  p->draw_border = 0; /* idem */
  p->dock_xpos = p->dock_ypos = 0;
  p->start_in_boss_mode = 0;
  ASSIGN_STRING_VAL(p->site_root, "linuxfr.org");
  p->site_port = 80;
  ASSIGN_STRING_VAL(p->site_path, "");
  ASSIGN_STRING_VAL(p->path_tribune_backend, "board/remote.xml");
  ASSIGN_STRING_VAL(p->path_news_backend, "backend.rss");
  ASSIGN_STRING_VAL(p->path_end_news_url, ",0,-1,6.html");
  ASSIGN_STRING_VAL(p->path_tribune_add, "board/add.php3");
  ASSIGN_STRING_VAL(p->path_myposts, "users/posts.php3?order=id");
  ASSIGN_STRING_VAL(p->path_messages,"messages/");
  p->default_trollo_speed = 4;
  p->user_cookie = NULL; 
  p->user_login = NULL;
  p->force_fortune_retrieval = 0;
  p->browser_cmd = NULL;
  p->browser2_cmd = NULL;
  p->enable_troll_detector = 1;
  ASSIGN_STRING_VAL(p->pp_fn_family, "helvetica");
  p->pp_fn_size = 12;
  p->pp_bgcolor = 0xdae6e6;
  p->pp_start_in_transparency_mode = 0;
  p->use_fake_real_transparency = 0;
  p->pp_transparency.type = SHADING;
  p->pp_transparency.shade.luminosite = 20; 
  p->pp_transparency.shade.assombrissement = 70;
  BICOLOR_SET(p->pp_fgcolor,0x303030,0xd0d0d0);
  BICOLOR_SET(p->pp_tstamp_color,0x004000, 0xffff80);
  BICOLOR_SET(p->pp_useragent_color, 0x800000, 0xa0ffa0);
  BICOLOR_SET(p->pp_login_color, 0xff0000, 0xc0ffc0);
  BICOLOR_SET(p->pp_url_color, 0x0000ff, 0x80f0ff);
  BICOLOR_SET(p->pp_trollscore_color, 0xff0000, 0xffff00);
  BICOLOR_SET(p->pp_button_color, 0xdae6e6, 0x404040);
  BICOLOR_SET(p->pp_emph_color, 0xffffff, 0x00a080);
  BICOLOR_SET(p->pp_sel_bgcolor, 0xffd700, 0x008080);
  BICOLOR_SET(p->pp_popup_fgcolor, 0x000050, 0x000050);
  BICOLOR_SET(p->pp_popup_bgcolor, 0xc0d0d0, 0xc0d0d0);
  BICOLOR_SET(p->pp_my_msg_color, 0xf07000, 0xf07000);
  BICOLOR_SET(p->pp_answer_my_msg_color, 0xe0b080, 0xe0b080);
  BICOLOR_SET(p->pp_keyword_color[0], 0x008080, 0x00ffff);
  BICOLOR_SET(p->pp_keyword_color[1], 0xff0080, 0x40a0c0);
  BICOLOR_SET(p->pp_keyword_color[2], 0x800080, 0xffff00);
  BICOLOR_SET(p->pp_keyword_color[3], 0x00ff80, 0x808000);
  BICOLOR_SET(p->pp_keyword_color[4], 0x008000, 0x808080);
  BICOLOR_SET(p->pp_plopify_color,0xa0a0a0, 0x808080);

  tribune_key_list_destroy(p->hilight_key_list); p->hilight_key_list = NULL;
  tribune_key_list_destroy(p->plopify_key_list); p->plopify_key_list = NULL;
  option_get_string_list("\"plop\",\"grouik\",\"gruiiik\",\"glop\",\"buurp\","
			 "\"miaou\",\"sluurpe\",\"côôoot\",\"pika\",\"pikaaaa\",\"ka-pika\","
			 "\"chuuu\",\"prout\",\"uuuurg\",\"blob\",\"ploop\",\"pl0p\",\"c0in\","
			 "\"pouet\",\"coin!\",\"flebelebelblbll\",\"blop\",\"gloup\"",
			 "", &p->plop_words, &p->nb_plop_words);
  p->pp_xpos = -10000;
  p->pp_ypos = -10000;
  p->pp_width = 300;
  p->pp_height = 455;
  p->pp_minibar_on = 1;
  p->pp_nosec_mode = 1;
  p->pp_html_mode = 1;
  p->pp_nick_mode = 4;
  p->pp_trollscore_mode = 1;           
  p->pp_fortune_mode = 1;
  p->pp_fortune_bgcolor = 0xb0b0b0;
  p->pp_fortune_fgcolor = 0x000000;
  ASSIGN_STRING_VAL(p->pp_fortune_fn_family, "helvetica");
  p->pp_fortune_fn_size = 10;
  
  p->ew_do_spell = 0;                  /*Ca fonctionne (?)
					 donc je l'active par defaut
				       */
  ASSIGN_STRING_VAL(p->ew_spell_cmd, "ispell");  /*ispell par defaut car present 
						   partout 
						 */
  ASSIGN_STRING_VAL(p->ew_spell_dict, "french"); /* risque mais soyons chauvin */
  
  p->post_cmd = NULL;
  ASSIGN_STRING_VAL(p->tribune_scrinechote, "~/wmcc_tribune_shot.html");  
}

/* libere la mémoire allouée par les champs de la structure, *mais* pas la structure elle-même */
void
wmcc_prefs_destroy(structPrefs *p)
{
  int i;
  FREE_STRING(p->font_encoding);
  FREE_STRING(p->news_fn_family);
  FREE_STRING(p->user_agent);
  FREE_STRING(p->tribune_wiki_emulation);
  FREE_STRING(p->coin_coin_message);
  FREE_STRING(p->balloon_fn_family);
  FREE_STRING(p->dock_bgpixmap);
  FREE_STRING(p->site_root);
  FREE_STRING(p->site_path);
  FREE_STRING(p->proxy_auth); 
  FREE_STRING(p->proxy_name); 
  FREE_STRING(p->path_tribune_backend);
  FREE_STRING(p->path_news_backend);
  FREE_STRING(p->path_end_news_url);
  FREE_STRING(p->path_tribune_add);
  FREE_STRING(p->path_myposts);
  FREE_STRING(p->path_messages);
  FREE_STRING(p->user_cookie);
  FREE_STRING(p->user_login);
  FREE_STRING(p->user_name);
  FREE_STRING(p->browser_cmd);
  FREE_STRING(p->browser2_cmd);
  FREE_STRING(p->pp_fn_family);
  tribune_key_list_destroy(p->hilight_key_list); p->hilight_key_list = NULL;
  tribune_key_list_destroy(p->plopify_key_list); p->plopify_key_list = NULL;
  for (i=0; i < (int)p->nb_plop_words; i++) FREE_STRING(p->plop_words[i]);
  FREE_STRING(p->plop_words);
  FREE_STRING(p->pp_fortune_fn_family);
  FREE_STRING(p->ew_spell_cmd);
  FREE_STRING(p->ew_spell_dict);
  FREE_STRING(p->post_cmd);
  FREE_STRING(p->tribune_scrinechote);
}

void
option_get_filename(char *arg, char **fname) {
  assert(arg); assert(*fname == NULL);
  if (*arg == '~') {
    *fname = str_printf("%s%s", getenv("HOME"), arg+1);
  } else {
    *fname = strdup(arg);
  }
  assert(*fname);
}

/* les macros c'est sale mais c'est j'aime ça */
#define CHECK_INTEGER_ARG(bmin,bmax,i) { i=atoi(arg); if (i<(bmin) || ((bmax)>(bmin) && i > (bmax))) { return str_printf("valeur invalide pour l'option '%s' (doit être dans l'intervalle %d-%d", opt_name, (bmin), (bmax)==0 ? 10000000 : (bmax)); } }

#define CHECK_BOOL_ARG(x) { x = option_get_bool_val(arg); if (x < 0) return str_printf("valeur invalide pour l'option '%s' : doit être une valeur du type on/off, 1/0, true/false ou yes/no", opt_name); }
#define CHECK_BOOLNOT_ARG(x) { CHECK_BOOL_ARG(x); x = !x ; }

#define CHECK_COLOR_ARG(x) { if (sscanf(arg, "%x", &x)!=1) return str_printf("valeur invalide pour l'option '%s' : doit être une couleur RGB en héxadécimal, au format RRGGBB", opt_name); if (strchr(arg, ' ')) return str_printf("erreur, l'option '%s' n'accepte qu'une seule valeur de couleur", opt_name); }

#define CHECK_XYPOS_ARG(xpos,ypos) { char *err=option_get_xypos_val(arg,opt_name,&xpos,&ypos); if (err) return err; }

#define CHECK_BICOLOR_ARG(x) { if (sscanf(arg, "%x %x", &x.opaque, &x.transp)<1) return str_printf("valeur invalide pour l'option '%s' : doit être une couleur RGB en héxadécimal, au format RRGGBB (suivie optionnellement d'une deuxième couleur utilisée pour le mode transparent)", opt_name); }

#define CHECK_TRANSP_ARG(x) { char *err=option_get_transp_val(arg,opt_name,&x); if (err) return err; }

#define CHECK_KEY_LIST(x,_min,_max) { char *err=option_get_key_list(arg,opt_name,&x,_min,_max); if (err) return err; }

#define CHECK_FILENAME_ARG(x) { FREE_STRING(x); option_get_filename(arg, &x); }

/* assigne une option dans les preferences, renvoie un message d'erreur si y'a un pb */
char *
wmcc_prefs_validate_option(structPrefs *p, wmcc_options_id opt_num, unsigned char *arg)
{
  char *opt_name;
  assert(opt_num < NB_WMCC_OPTIONS);
  opt_name = wmcc_options_strings[opt_num];
  switch (opt_num) {
  case OPT_font_encoding: { 
    ASSIGN_STRING_VAL(p->font_encoding,arg); 
  } break; 
  case OPT_tribune_delay: {
    CHECK_INTEGER_ARG(10,10000, p->dlfp_tribune_check_delay); 
  } break;
  case OPT_tribunenews_max_refresh_delay: {
    CHECK_INTEGER_ARG(1,0, p->dlfp_max_refresh_delay);
  } break; 
  case OPT_tribunenews_switch_off_coincoin_delay: {
    CHECK_INTEGER_ARG(1,0, p->dlfp_switch_off_coincoin_delay);
  } break; 
  case OPT_tribune_max_messages: {
    CHECK_INTEGER_ARG(1,0, p->tribune_max_msg);
  } break; 
  case OPT_tribune_troll_detector: {
    CHECK_BOOL_ARG(p->enable_troll_detector);
  } break;
  case OPT_tribune_post_cmd: {
    ASSIGN_STRING_VAL(p->post_cmd, arg);
  } break; 
  case OPT_tribune_wiki_emulation: {
    ASSIGN_STRING_VAL(p->tribune_wiki_emulation, arg);
  } break; 
  case OPT_tribune_archive: {
    CHECK_FILENAME_ARG(p->tribune_scrinechote);
  } break; 
  case OPT_dock_bg_color: {
    CHECK_COLOR_ARG(p->dock_bgcolor);
  } break; 
  case OPT_dock_bg_pixmap: {
    CHECK_FILENAME_ARG(p->dock_bgpixmap);
  } break; 
  case OPT_dock_fg_color: {
    CHECK_COLOR_ARG(p->dock_fgcolor);
  } break; 
  case OPT_dock_draw_border: {
    CHECK_BOOL_ARG(p->draw_border);
  } break; 
  case OPT_dock_iconwin: {
    CHECK_BOOLNOT_ARG(p->use_iconwin);
  } break; 
  case OPT_dock_use_balloons: {
    CHECK_BOOL_ARG(p->use_balloons);
  } break; 
  case OPT_dock_balloons_font_family: {
    ASSIGN_STRING_VAL(p->balloon_fn_family, arg); 
  } break; 
  case OPT_dock_balloons_font_size: {
    CHECK_INTEGER_ARG(1,100, p->balloon_fn_size);
  } break; 
  case OPT_dock_pos: {
    CHECK_XYPOS_ARG(p->dock_xpos, p->dock_ypos);
  } break; 
  case OPT_dock_start_in_boss_mode: {
    CHECK_BOOL_ARG(p->start_in_boss_mode);
  } break; 
  case OPT_news_delay: {
    CHECK_INTEGER_ARG(60,10000, p->dlfp_news_check_delay);
  } break; 
  case OPT_news_font_family: {
    ASSIGN_STRING_VAL(p->news_fn_family, arg); 
  } break; 
  case OPT_news_font_size: {
    CHECK_INTEGER_ARG(1,0, p->news_fn_size);
  } break; 
  case OPT_news_bg_color: {
    CHECK_COLOR_ARG(p->news_bgcolor);
  } break; 
  case OPT_news_fg_color: {
    CHECK_COLOR_ARG(p->news_fgcolor);
  } break; 
  case OPT_news_location: {
    CHECK_XYPOS_ARG(p->news_xpos, p->news_ypos);
  } break; 
  case OPT_news_dimensions: {
    CHECK_XYPOS_ARG(p->news_width, p->news_height);
  } break; 
  case OPT_palmipede_username: {
    ASSIGN_STRING_VAL(p->user_name, arg);
  } break; 
  case OPT_palmipede_userlogin: {
    ASSIGN_STRING_VAL(p->user_login, arg);
  } break; 
  case OPT_palmipede_useragent: {
    option_set_useragent(arg, p);
  } break; 
  case OPT_palmipede_default_message: {
    ASSIGN_STRING_VAL(p->coin_coin_message, arg); 
  } break; 
  case OPT_tribune_backend_type: {
    CHECK_INTEGER_ARG(1,3, p->tribune_backend_type);
  } break; 
  case OPT_http_site_url: {
    option_site_root(arg,p);
  } break; 
  case OPT_http_path_tribune_backend: {
    ASSIGN_STRING_VAL(p->path_tribune_backend, arg); 
  } break; 
  case OPT_http_path_news_backend: {
    ASSIGN_STRING_VAL(p->path_news_backend, arg); 
  } break; 
  case OPT_http_path_end_news_url: {
    ASSIGN_STRING_VAL(p->path_end_news_url, arg); 
  } break; 
  case OPT_http_path_tribune_add: {
    ASSIGN_STRING_VAL(p->path_tribune_add, arg); 
  } break; 
  case OPT_http_path_myposts: {
    ASSIGN_STRING_VAL(p->path_myposts, arg); 
  } break; 
  case OPT_http_path_messages: {
    ASSIGN_STRING_VAL(p->path_messages, arg); 
  } break; 
  case OPT_http_cookie: {
    ASSIGN_STRING_VAL(p->user_cookie, arg); 
  } break; 
  case OPT_http_force_fortune_retrieval: {
    CHECK_BOOL_ARG(p->force_fortune_retrieval);
  } break; 
  case OPT_http_proxy: {
    option_set_proxy(arg, p);
  } break; 
  case OPT_http_proxy_auth: {
    ASSIGN_STRING_VAL(p->proxy_auth, arg); 
  } break; 
  case OPT_http_proxy_use_nocache: {
    CHECK_BOOL_ARG(p->proxy_nocache);
  } break; 
  case OPT_http_disable_if_modified_since: {
    CHECK_BOOLNOT_ARG(p->use_if_modified_since);
  } break; 
  case OPT_http_browser: {
    option_browser(arg, opt_name, p, 1);
  } break; 
  case OPT_http_browser2: {
    option_browser(arg, opt_name, p, 2);
  } break; 
  case OPT_pinnipede_font_family: {
    ASSIGN_STRING_VAL(p->pp_fn_family, arg); 
  } break; 
  case OPT_pinnipede_font_size: {
    CHECK_INTEGER_ARG(1,0, p->pp_fn_size);
  } break; 
  case OPT_pinnipede_bg_color: {
    CHECK_COLOR_ARG(p->pp_bgcolor);
  } break; 
  case OPT_pinnipede_start_in_transparency_mode: {
    CHECK_BOOL_ARG(p->pp_start_in_transparency_mode);
  } break;
  case OPT_pinnipede_use_fake_real_transparency: {
    CHECK_BOOL_ARG(p->use_fake_real_transparency);
  } break;
  case OPT_pinnipede_transparency: {
    CHECK_TRANSP_ARG(p->pp_transparency);
  } break;
  case OPT_pinnipede_fg_color: {
    CHECK_BICOLOR_ARG(p->pp_fgcolor);
  } break; 
  case OPT_pinnipede_clock_color: {
    CHECK_BICOLOR_ARG(p->pp_tstamp_color);
  } break; 
  case OPT_pinnipede_useragent_color: {
    CHECK_BICOLOR_ARG(p->pp_useragent_color);
  } break; 
  case OPT_pinnipede_login_color: {
    CHECK_BICOLOR_ARG(p->pp_login_color);
  } break; 
  case OPT_pinnipede_url_color: {
    CHECK_BICOLOR_ARG(p->pp_url_color);
  } break; 
  case OPT_pinnipede_trollscore_color: {
    CHECK_BICOLOR_ARG(p->pp_trollscore_color);
  } break; 
  case OPT_pinnipede_button_color: {
    CHECK_BICOLOR_ARG(p->pp_button_color);
  } break; 
  case OPT_pinnipede_emph_color: {
    CHECK_BICOLOR_ARG(p->pp_emph_color);
  } break; 
  case OPT_pinnipede_sel_bgcolor: {
    CHECK_BICOLOR_ARG(p->pp_sel_bgcolor);
  } break; 
  case OPT_pinnipede_popup_bgcolor: {
    CHECK_BICOLOR_ARG(p->pp_popup_bgcolor);
  } break; 
  case OPT_pinnipede_popup_fgcolor: {
    CHECK_BICOLOR_ARG(p->pp_popup_fgcolor);
  } break; 
  case OPT_pinnipede_hilight_my_msg_color: {
    CHECK_BICOLOR_ARG(p->pp_my_msg_color);
  } break; 
  case OPT_pinnipede_hilight_answer_my_msg_color: {
    CHECK_BICOLOR_ARG(p->pp_answer_my_msg_color);
  } break; 
  case OPT_pinnipede_hilight_keyword_color0: {
    CHECK_BICOLOR_ARG(p->pp_keyword_color[0]);
  } break; 
  case OPT_pinnipede_hilight_keyword_color1: {
    CHECK_BICOLOR_ARG(p->pp_keyword_color[1]);
  } break; 
  case OPT_pinnipede_hilight_keyword_color2: {
    CHECK_BICOLOR_ARG(p->pp_keyword_color[2]);
  } break; 
  case OPT_pinnipede_hilight_keyword_color3: {
    CHECK_BICOLOR_ARG(p->pp_keyword_color[3]);
  } break; 
  case OPT_pinnipede_hilight_keyword_color4: {
    CHECK_BICOLOR_ARG(p->pp_keyword_color[4]);
  } break; 
  case OPT_pinnipede_plopify_color: {
    CHECK_BICOLOR_ARG(p->pp_plopify_color);
  } break; 
  case OPT_pinnipede_fortune_bgcolor: {
    CHECK_COLOR_ARG(p->pp_fortune_bgcolor);
  } break; 
  case OPT_pinnipede_fortune_fgcolor: {
    CHECK_COLOR_ARG(p->pp_fortune_fgcolor);
  } break; 
  case OPT_pinnipede_fortune_font_family: {
    ASSIGN_STRING_VAL(p->pp_fortune_fn_family, arg); 
  } break; 
  case OPT_pinnipede_fortune_font_size: {
    CHECK_INTEGER_ARG(1,0, p->pp_fortune_fn_size);
  } break; 
  case OPT_pinnipede_location: {
    CHECK_XYPOS_ARG(p->pp_xpos, p->pp_ypos);
  } break; 
  case OPT_pinnipede_dimensions: {
    CHECK_XYPOS_ARG(p->pp_width, p->pp_height);
  } break; 
  case OPT_pinnipede_buttons: {
    CHECK_BOOL_ARG(p->pp_minibar_on);
  } break; 
  case OPT_pinnipede_show_tags: {
    CHECK_BOOL_ARG(p->pp_html_mode);
  } break; 
  case OPT_pinnipede_show_seconds: {
    CHECK_BOOLNOT_ARG(p->pp_nosec_mode);
  } break; 
  case OPT_pinnipede_nick_mode: {
    CHECK_INTEGER_ARG(0,4, p->pp_nick_mode);
  } break; 
  case OPT_pinnipede_show_troll_score: {
    CHECK_BOOL_ARG(p->pp_trollscore_mode);
  } break; 
  case OPT_pinnipede_show_fortune: {
    CHECK_BOOL_ARG(p->pp_fortune_mode);
  } break; 
  case OPT_pinnipede_plop_keywords: {
    CHECK_KEY_LIST(p->plopify_key_list,0,2);
  } break; 
  case OPT_pinnipede_hilight_keywords: {
    CHECK_KEY_LIST(p->hilight_key_list,0,4);
  } break; 
  case OPT_pinnipede_plop_words: {
    char *err = option_get_string_list(arg, opt_name,  &p->plop_words, &p->nb_plop_words);
    if (err) return err;
  } break; 
  case OPT_spell_enable: {
    CHECK_BOOL_ARG(p->ew_do_spell);
  } break; 
  case OPT_spell_cmd: {
    ASSIGN_STRING_VAL(p->ew_spell_cmd, arg); 
  } break; 
  case OPT_spell_dict: {
    ASSIGN_STRING_VAL(p->ew_spell_dict, arg); 
  } break;   
  default: printf("attention chérie, ça va couper\n"); assert(0);
  }
  return NULL;
}


/* lecture d'un fichier d'options, renvoie un message d'erreur si y'a un pb */
char *
wmcc_prefs_read_options(structPrefs *p, const char *filename)
{
  FILE *f;
  char *error;
  int lcnt;
  char *opt_name = NULL, *opt_arg = NULL;

  f = fopen(filename, "rt");
  if (f == NULL) {
    return str_printf("impossible d'ouvrir le fichier '%s' en lecture [attention wmcc ne le crée plus tout seul, si vous voulez utiliser les options par défaut, faites juste un 'touch %s', sinon copiez-y le fichier 'options' fourni avec wmc², et abondamment commenté]", filename, filename);
  }
  lcnt = 0;
  do {
    error = read_option_line(f, &lcnt, &opt_name, &opt_arg); if (error) goto ouille;
    if (opt_name) {
      wmcc_options_id i;
      for (i=0; i < NB_WMCC_OPTIONS; i++) {
	if (strcasecmp(opt_name, wmcc_options_strings[i])==0) {
	  error = wmcc_prefs_validate_option(p, i, opt_arg);
	  if (error) goto ouille;
	  break;
	}
      }
      if (i == NB_WMCC_OPTIONS) {
	error = str_printf("[ligne %d] l'option '%s' est inconnue", lcnt, opt_name);
	goto ouille;
      }
    }
    FREE_STRING(opt_name); FREE_STRING(opt_arg);
  } while (!feof(f));
  fclose(f);
  return NULL;

 ouille:
  FREE_STRING(opt_name); FREE_STRING(opt_arg);
  fclose(f);
  return error;
}
