#ifdef __CYGWIN__
#include <getopt.h>
#endif
#include <sys/stat.h>
#include "coincoin.h"
#include "spell_coin.h"

#define TEST_CMDLINE_OPT(optid) { \
  char *errmsg = wmcc_prefs_validate_option(&Prefs,optid,optarg); \
  if (errmsg) { myprintf("Erreur pour l'option de la ligne de commande '%<GRN -%c>'\n: %s\n",optc,errmsg); \
  exit(1); } }

static void
wmcc_prefs_from_cmdline(int argc, char **argv, structPrefs *The_Prefs)
{
  int optc;

  /* tant que le 'kicker' de KDE ne sera pas capable de sauvegarder
     les options de ligne de commande d'une session a l'autre... */
  if (strstr(argv[0], "wmcoincoin-kde")) {
    myprintf("vous avec invoqué wmcoincoin-kde : %<YEL Mode KDE activé>\n");
    The_Prefs->use_iconwin = 0;
  }

  while ((optc = getopt(argc, argv, "hd:D:f:F:u:m:v:p:b:g::P:A:wBHx:r:s:C:X:c:o:W")) !=-1) {
    switch (optc) {
    case 'h':
      {
	myprintf("Home page: %<BLU http:/hules.free.fr/wmcoincoin>\n");

	myprintf("Dock applet pour WindowMaker -- affiche les news de %<GRN D>%<CYA a>%<GRN L>%<CYA inux>%<GRN F>%<CYA rench>%<GRN P>%<CYA age> (%<BLU http://linuxfr.org>)\n");
	myprintf("OPTIONS (la plupart de ces options peuvent être activées dans le\n"
		 " fichier '%<YEL ~/.wmcoincoin/options>'):\n");
	myprintf(" %<GRN -h>\t\t: mouaif...\n");
	myprintf(" %<GRN -d> %<CYA n>\t\t: fixe le delai entre deux checks de la tribune a %<CYA n> secondes (defaut %<grn %d>)\n", The_Prefs->dlfp_tribune_check_delay);
	myprintf(" %<GRN -D> %<CYA n>\t\t: fixe le delai entre deux checks des news a %<CYA n> secondes (default %<grn %d>)\n", The_Prefs->dlfp_news_check_delay);
	myprintf(" %<GRN -f> %<CYA fn>\t\t: utiliser la fonte de famille %<CYA fn> pour l'affiche des news\n"
		 "\t\t  (ex: fn = %<grn %s> (defaut), ou fn = clean)\n", The_Prefs->news_fn_family);
	myprintf(" %<GRN -F> %<CYA n>\t\t: utiliser une taille de fonte de %<CYA n> pixels pour l'affiche des news\n\t\t"
		 " (defaut = %<grn %d>)\n", The_Prefs->news_fn_size);
	myprintf(" %<GRN -u> %<CYA ua>\t\t: change le user-agent (defaut: %<grn %s>)\n", The_Prefs->user_agent);
	myprintf(" %<GRN -P> %<CYA proxy:port>\t: utilise un proxy\n");
	myprintf(" %<GRN -A> %<CYA username:password>: authentification pour le proxy, si necessaire\n");
	myprintf(" %<GRN -m> %<CYA msg>\t\t: change le message posté sur la tribune (defaut: '%<grn %s>')\n", The_Prefs->coin_coin_message);
	myprintf(" %<GRN -v> %<CYA n>\t\t: verbosité (defaut %<grn %d>) ,%<CYA n>=0,1,2,3\n", The_Prefs->verbosity);
	myprintf(" %<GRN -p> %<CYA n>\t\t: ne s'interesse qu'aux news postees il y a moins de %<CYA n> jours\n"
		 "\t\t (defaut: %<grn %d> jours)\n", The_Prefs->news_max_nb_days);
	myprintf(" %<GRN -b> %<CYA hexcoul>\t: couleur de fond, en RGB hexa (par defaut: %<grn %06x>, un magnifique(!) jaune)\n", The_Prefs->dock_bgcolor);
	myprintf(" %<GRN -c> %<CYA hexcoul>\t: couleur du texte de l'applet, en RGB hexa (par defaut: %<grn %06x>)\n", The_Prefs->dock_fgcolor);
	myprintf(" %<GRN -X> %<CYA file.xpm>\t: Fichier pixmap à mettre en fond du dock (fichier.xpm de 64x64)\n");
	myprintf(" %<GRN -w> \t\t: utilise le mode windowed plutot que le mode icone\n");
	myprintf("\t\tcette option est nécessaire pour utiliser wmcoincoin avec le kicker de KDE\n");
	myprintf("\t\tmais comme le kicker de KDE 2.1 a un bug qui l'empeche de sauver les options\n");
	myprintf("\t\tde la ligne de commande d'une session a l'autre, le mieux est d'invoquer\n");
	myprintf("\t\twmcoincoin sous le nom %<YEL wmcoincoin-kde> (c'est un bete lien symbolique)\n");
	myprintf("\t\tet l'option -w sera automatiquement activée...\n");
	myprintf(" %<GRN -B> \t\t: ajoute la bordure de wmcoincoin (pour windowmanager sans dock)\n");
	myprintf(" %<GRN -x> %<CYA x:y>\t\t: essaye de positionner wmcoincoin en (x,y), cette option\n"
		 "\t\tn'est utile que si le windowmanager n'a pas de dock \n"
		 "\t\t(utiliser avec l'option %<GRN -B>)\n");
#ifndef DISABLE_BALLOONS
	myprintf(" %<GRN -H> \t\t: desactive les ballons d'aide\n");	
#endif
	myprintf(" %<GRN -r> %<CYA url>\t\t: se connecte sur un autre site dacode que linuxfr (defaut %<grn http://linuxfr.org:80>)\n");
	myprintf(" %<GRN -s> %<CYA v>\t\t: vitesse de défilement du trolloscope (1 = rapide, 100 = 1 tres lent)\n(defaut %<grn %d>)\n", The_Prefs->default_trollo_speed);
	myprintf(" %<GRN -C> %<CYA cookie>\t: indique la valeur de votre cookie 'session_id' (il faut\n"
		 "\t\têtre authentifié). Cela permet à wmCoinCoin de détecter quand il y a une\n"
		 "\t\tnouvelle réponse à un de vos commentaires -- cette option est\n"
		 "\t\t tout à fait facultative\n");
	myprintf(" %<GRN -o> %<CYA fichier>\t: indique le nom du fichier d'options à utiliser dans le\n"
		 "\t\t rep ~/.wmcoincoin (defaut '%<grn %s>')\n", options_file_name);
	myprintf(" %<GRN -W>\t\t: ouvre le pinnipede des le lancement de wmcoincoin)\n");
	exit(0);
      } break;
    case 'd': TEST_CMDLINE_OPT(OPT_tribune_delay); break;
    case 'D': TEST_CMDLINE_OPT(OPT_news_delay); break;
    case 'f': TEST_CMDLINE_OPT(OPT_news_font_family); break;
    case 'F': TEST_CMDLINE_OPT(OPT_news_font_size); break; 
    case 'u': TEST_CMDLINE_OPT(OPT_palmipede_useragent); break;
    case 'm': TEST_CMDLINE_OPT(OPT_palmipede_default_message); break;
    case 'v':
      {
	The_Prefs->verbosity = atoi(optarg); assert(The_Prefs->verbosity >= 0 && The_Prefs->verbosity <= 5);
      } break;
    case 'p':
      {
	The_Prefs->news_max_nb_days = atoi(optarg);
	assert(The_Prefs->news_max_nb_days > 0 && The_Prefs->news_max_nb_days < 100);
      } break;
    case 'b': TEST_CMDLINE_OPT(OPT_dock_bg_color); break;
    case 'c': TEST_CMDLINE_OPT(OPT_dock_fg_color); break;
    case 'X': TEST_CMDLINE_OPT(OPT_dock_bg_pixmap); break;
    case 'g':
      {
	if (optarg == NULL) {
	  The_Prefs->debug = 1;
	} else The_Prefs->debug = atoi(optarg);
	
	myprintf("Activation du mode %<YEL debug> secret (dbg=%d)!\n", The_Prefs->debug);
      } break;
    case 'P': TEST_CMDLINE_OPT(OPT_http_proxy); break;
    case 'A': TEST_CMDLINE_OPT(OPT_http_proxy_auth); break;
    case 'w': TEST_CMDLINE_OPT(OPT_dock_iconwin); break;
    case 'B': TEST_CMDLINE_OPT(OPT_dock_draw_border); break;
    case 'H': TEST_CMDLINE_OPT(OPT_dock_use_balloons); break;
    case 'x': TEST_CMDLINE_OPT(OPT_dock_pos); break;
    case 'r': TEST_CMDLINE_OPT(OPT_http_site_url); break;
    case 's':
      {
	The_Prefs->default_trollo_speed = 1 << (atoi(optarg)-1);
	//	printf("%s -> %d\n",optarg, The_Prefs->default_trollo_speed);
	if (The_Prefs->default_trollo_speed < 1 || The_Prefs->default_trollo_speed > 128) {
	  myprintf("option '-s' : la vitesse doit se situer en 1 et 8\n");
	  exit(1);
	}
      } break;
    case 'C' : TEST_CMDLINE_OPT(OPT_http_cookie); break;
		case 'W' : The_Prefs->pinnipede_open_on_start = 1; break; /* auto ouvre le pinnipede au lancement */
    case 'o': break; /* cette option est traitée dans init_default_prefs (cad AVANT la lecture du fichier d'options) */
    case ':':
    case '?':
    default:
      exit(0); break;   
    }
  }
}

 
static void
check_wmcoincoin_prefs_dir()
{
  char rpath[2048];
  struct stat stbuf;
  int is_ok;

  snprintf(rpath,2048,"%s/.wmcoincoin", getenv("HOME"));
  is_ok = 1;
  if (stat(rpath, &stbuf) == 0) {
    if (!S_ISDIR(stbuf.st_mode)) {
      fprintf(stderr, "le fichier '%s' n'est pas un répertoire !!! supprimez le, svp\n", rpath);
      is_ok = 0;
    }
  } else {
    if (errno == ENOENT) {
      myfprintf(stderr, "création de '%<YEL %s>'..\n", rpath);
      if (mkdir(rpath,-1)) {
	myfprintf(stderr, "impossible de creer le repertoire '%s'\n[erreur: '%<red %s>']\n",
		 rpath, strerror(errno));
	is_ok = 0;
      }
    } else {
      fprintf(stderr, "Il y a quelque chose de bizarre avec le répertoire '%s' [err=%s]\n", rpath, strerror(errno));
      is_ok = 0;
    }
  }
  if (is_ok == 0) {
    exit(1);
  }
}

/* alors celle-là est vraiment à chier */
int
copy_file(char *dest, char *src) {
  FILE *in, *out;
  int c;
  
  in = fopen(src, "r");
  if (!in) {
    fprintf(stderr, "impossible de lire '%s' : %s\n", src, strerror(errno)); return -1;
  }
  out = fopen(dest, "w");
  if (!out) {
    fprintf(stderr, "impossible d'écrire dans '%s' : %s\n", dest, strerror(errno)); return -1;
  }

  while ((c=fgetc(in)) != EOF) {
    fputc(c, out);
  }
  fclose(in); fclose(out);
  return 0;
}


/* installe un fichier de WMCCDATADIR dans le ~/.wmcoincoin si nécessaire */
char *
check_install_data_file(char *data_file_name, char *dot_wmcc_file_name)
{
  char *dot_name;
  char *data_name;
  struct stat stbuf;
  int use_data = 0;


  dot_name = str_printf("%s/.wmcoincoin/%s", getenv("HOME"), dot_wmcc_file_name); 
  data_name = str_printf(WMCCDATADIR "/%s", data_file_name);

  if (stat(dot_name, &stbuf)) {
    fprintf(stderr, "ah ben y'a pas le fichier '%s', on va le créer\n", dot_name);
    copy_file(dot_name, data_name);
  }

  if (stat(dot_name, &stbuf)) {
    fprintf(stderr, "\ndésolé, demerdez-vous pour copier '%s' dans votre ~/.wmcoincoin ..\n", data_file_name);
    use_data = 1;
  } else if (!S_ISREG(stbuf.st_mode)) {
    fprintf(stderr, "\narrêtez de faire n'importe quoi dans votre ~/.wmcoincoin..\n\n");
    use_data = 1;
  }

  if (use_data) {
    if (stat(data_name, &stbuf)) {
      fprintf(stderr, "wmcoincoin a du être mal installé, je ne trouve pas le fichier '%s' par défaut dans le rép '" WMCCDATADIR "'..\n",
	      data_name);
      exit(1);
    } else {
      fprintf(stderr, "utilisation du fichier se trouvant dans '%s'\n", data_name);
    }
    free(dot_name); return data_name;
  } else {
    free(data_name); return dot_name;
  }
}

void
wmcc_prefs_initialize(int argc, char **argv, structPrefs *p)
{
  int i;
  char *errmsg;
  char *options_full_file_name;
  wmcc_prefs_set_default(p);
  ASSIGN_STRING_VAL(options_file_name, "options");
  /* pas très beau : préanalyse de la ligne de command pour détecter l'usage
     d'un fichier d'options autre que celui par défaut */
  for (i=1; i < argc-1; i++) {
    if (strcmp(argv[i], "-o") == 0) {
      assert(argv[i+1]);
      ASSIGN_STRING_VAL(options_file_name, argv[i+1]);
      myprintf("utilisation du fichier d'options '%<yel %s>'\n", argv[i+1]);
      break;
    }
  }

  /* creation de ~/.wmcoincoin si necessaire */
  check_wmcoincoin_prefs_dir();

  options_full_file_name = check_install_data_file("options", options_file_name);
  assert(options_full_file_name);
  errmsg = wmcc_prefs_read_options(p, options_full_file_name); /* lecture de '.wmcoincoin/options' */
  if (errmsg) {
    myprintf("Erreur pendant la lecture de '%s':\n%<YEL %s>\n" , options_full_file_name, errmsg);
    free(errmsg); exit(1);
  }
  free(options_full_file_name);
  wmcc_prefs_from_cmdline(argc, argv, p);
}

/* man fonction_a_la_con */
static int
str_copy_if_changed(char **a, char *b) {
  if (*a == NULL && b == NULL) {
    return 0;
  } if (*a == NULL && b) {
    *a = strdup(b);
    return 1;
  } else if (*a && b == NULL) {
    FREE_STRING(*a); return 1;
  } else if (strcmp(*a,b)) {
    FREE_STRING(*a); *a = strdup(b); return 1;
  }
  return 0;
}

static int
int_copy_if_changed(int *a, int b) {
  if (*a != b) {
    *a = b; return 1;
  } else return 0;
}

static int
bic_copy_if_changed(BiColor *a, BiColor b) {
  if (a->opaque != b.opaque || a->transp != b.transp) {
    *a = b; return 1;
  } else return 0;
}

static int
transp_copy_if_changed(TransparencyInfo *a, TransparencyInfo b) {
  int different = 0;
  
  if (a->type != b.type) different = 1;
  else {
    switch (b.type) {
    case FULL_TRANSPARENCY:
      break;
    case SHADING:
      if (a->shade.luminosite != b.shade.luminosite ||
	  a->shade.assombrissement != b.shade.assombrissement) different = 1;
      break;
    case TINTING:
      if (a->tint.white != b.tint.white ||
	  a->tint.black != b.tint.black) different = 1;
      break;
    default: assert(0);
    }
  }
  if (different) { *a = b; }
  return different;
}

static int
key_list_copy_if_changed(KeyList **a, KeyList *b)
{
  KeyList *hk;
  int changed = 0;

  hk = b;
  /* ajoute les nouvelle */
  while (hk) {
    KeyList *hka;
    if ((hka=tribune_key_list_find(*a, hk->key, hk->type)) == NULL) {
      changed = 1;
      *a = tribune_key_list_add(*a, hk->key, hk->type, hk->num, hk->from_prefs);
    } else if (hka->num != hk->num) {
      changed = 1;
      /* on l'enleve puis la rajoute pour assurer un tri dans le bon ordre */
      *a = tribune_key_list_remove(*a, hk->key, hk->type);
      *a = tribune_key_list_add(*a, hk->key, hk->type, hk->num, hk->from_prefs);
    }
    hk = hk->next;
  }
  hk = *a;
  /* vire les anciennes */
  while (hk) {
    KeyList *hkb;
    if (hk->from_prefs) {
      if ((hkb = tribune_key_list_find(b, hk->key, hk->type)) == NULL) {
	changed = 1;
	*a = tribune_key_list_remove(*a, hk->key, hk->type);
	hk = *a; continue;
      }
    }
    hk = hk->next;
  }
  printf("key_list changed: %d\n", changed);
  return changed;
}


static int
string_list_copy_if_changed(char ***a, int *na, char * const *b, const int nb)
{
  int changed = 0, i;
  
  if (*na != nb) changed = 1;
  else {
    for (i=0; i < *na; i++) {
      if (strcasecmp((*a)[i], b[i]) != 0) { changed = 1; break; }
    }
  }
  if (changed) {
    free(*a);
    *a = calloc(nb, sizeof(char*));
    for (i=0; i < nb; i++) {
      (*a)[i] = strdup(b[i]);
    }
  }
  return changed;
}

#define INT_OPT_CHANGED(_x) (Prefs._x != (int)newPrefs._x)
#define STR_OPT_CHANGED(_x) ((Prefs._x == NULL && newPrefs._x) || \
                             (Prefs._x && newPrefs._x == NULL) || \
                             ((Prefs._x != NULL && newPrefs._x != NULL) \
                              && strcmp(Prefs._x, newPrefs._x)))
#define INT_OPT_COPY(_x) {Prefs._x = (int)newPrefs._x; }
#define STR_OPT_COPY(_x) {FREE_STRING(Prefs._x); if (newPrefs._x) Prefs._x = strdup(newPrefs._x); }
#define STR_OPT_COPY_IF_CHANGED(_x) (str_copy_if_changed(&Prefs._x, newPrefs._x))
#define INT_OPT_COPY_IF_CHANGED(_x) (int_copy_if_changed(&Prefs._x, newPrefs._x))

#define BIC_OPT_CHANGED(_x) ((Prefs._x.opaque != (int)newPrefs._x.opaque) || \
                             (Prefs._x.transp != (int)newPrefs._x.transp))
#define BIC_OPT_COPY_IF_CHANGED(_x) (bic_copy_if_changed(&Prefs._x, newPrefs._x))

#define TRANSP_OPT_COPY_IF_CHANGED(_x) (transp_copy_if_changed(&Prefs._x, newPrefs._x))

#define KEY_LIST_COPY_IF_CHANGED(_x) (key_list_copy_if_changed(&Prefs._x, newPrefs._x))

#define STRING_LIST_COPY_IF_CHANGED(_x,_nb) (string_list_copy_if_changed(&Prefs._x, &Prefs._nb, \
   newPrefs._x, newPrefs._nb))


/* c'est un peu bourrin comme approche mais ça devrait marcher..*/
void
wmcc_prefs_relecture(Dock *dock)
{
  structPrefs newPrefs;
  char *errmsg;
  char *options_full_file_name;
  int scrollcoin_changed = 0;

  memset(&newPrefs, 0, sizeof(structPrefs));
  wmcc_prefs_set_default(&newPrefs);
  options_full_file_name = check_install_data_file("options", options_file_name);

  errmsg = wmcc_prefs_read_options(&newPrefs, options_full_file_name);
  if (errmsg) {
    char *s = str_printf("problème pendant la relecture des options [%s]<br>%s",
			 options_full_file_name, errmsg); free(errmsg);
    msgbox_show(dock, s); free(s);
  } else {
    int pp_need_refresh = 0;

    //    char *msg_cancelled_changes = strdup(""), *tmp;
    myprintf("relecture des options '%<YEL %s>' réussie\n", options_full_file_name);

    /* on dresse la liste des options non modifiable à chaud */
    /*    if (STR_OPT_CHANGED(font_encoding)) {
      tmp = str_printf("%sfont.encoding<br>", msg_cancelled_changes); 
      free(msg_cancelled_changes);msg_cancelled_changes = tmp; 
      }*/
    
    /* parametres de refresh */
    INT_OPT_COPY(dlfp_tribune_check_delay);
    INT_OPT_COPY(dlfp_news_check_delay);
    INT_OPT_COPY(dlfp_max_refresh_delay);
    INT_OPT_COPY(dlfp_switch_off_coincoin_delay);
    INT_OPT_COPY(tribune_max_msg);
    INT_OPT_COPY(tribune_backend_type);
    INT_OPT_COPY(news_max_nb_days);
    INT_OPT_COPY(use_balloons);

    /* l'ensemble des options http */
    INT_OPT_COPY(http_timeout);
    INT_OPT_COPY(proxy_nocache);
    INT_OPT_COPY(use_if_modified_since);
    STR_OPT_COPY(user_agent);
    STR_OPT_COPY(proxy_auth);
    if (STR_OPT_COPY_IF_CHANGED(proxy_name) ||
	STR_OPT_COPY_IF_CHANGED(site_root)) {
      flag_changed_http_params = 1; /* force le gethostbyname */
      myprintf("vous avez changé l'adresse du site/proxy, gethostbyname bientot en cours (le coincoin\n"
	       "va peut être se bloquer quelques secondes, voire plus\n");
    }
    INT_OPT_COPY(proxy_port);
    STR_OPT_COPY(user_name);
    STR_OPT_COPY(site_path);
    INT_OPT_COPY(site_port);
    STR_OPT_COPY(path_tribune_backend);
    STR_OPT_COPY(path_news_backend);
    STR_OPT_COPY(path_end_news_url);
    STR_OPT_COPY(path_tribune_add);
    STR_OPT_COPY(path_myposts);
    STR_OPT_COPY(path_messages);
    STR_OPT_COPY(user_cookie);
    STR_OPT_COPY(user_login);
    STR_OPT_COPY(browser_cmd);
    STR_OPT_COPY(browser2_cmd);
    INT_OPT_COPY(enable_troll_detector);
    INT_OPT_COPY(force_fortune_retrieval);

    /* spell */
    if (INT_OPT_COPY_IF_CHANGED(ew_do_spell) ||
	STR_OPT_COPY_IF_CHANGED(ew_spell_cmd) ||
	STR_OPT_COPY_IF_CHANGED(ew_spell_dict)) {
      check_if_should_kill_ispell(1);
    }

    STR_OPT_COPY(post_cmd);
    STR_OPT_COPY(tribune_scrinechote);

    /* à faire: plop_words */

    if (BIC_OPT_COPY_IF_CHANGED(sc_bg_color) ||
        BIC_OPT_COPY_IF_CHANGED(sc_bg_light_color) ||
	BIC_OPT_COPY_IF_CHANGED(sc_bg_dark_color) || 
	BIC_OPT_COPY_IF_CHANGED(sc_arrow_normal_color) ||
	BIC_OPT_COPY_IF_CHANGED(sc_arrow_emphasized_color) ||
	BIC_OPT_COPY_IF_CHANGED(sc_bar_color) ||
	BIC_OPT_COPY_IF_CHANGED(sc_bar_light_color) ||
	BIC_OPT_COPY_IF_CHANGED(sc_bar_dark_color)) {
      scrollcoin_changed = 1;
    }


    /* modifs sur l'apparence de la fenêtre des news */
    if (STR_OPT_COPY_IF_CHANGED(news_fn_family) || INT_OPT_COPY_IF_CHANGED(news_fn_size) ||
	INT_OPT_COPY_IF_CHANGED(news_bgcolor) || INT_OPT_COPY_IF_CHANGED(news_fgcolor) ||
	INT_OPT_COPY_IF_CHANGED(news_titles_bgcolor) ||
	INT_OPT_COPY_IF_CHANGED(news_titles_fgcolor) ||
	INT_OPT_COPY_IF_CHANGED(news_emph_color) ||
	scrollcoin_changed) {
      int showed = newswin_is_used(dock);
      newswin_destroy(dock); newswin_build(dock);
      if (showed) newswin_show(dock, dock->dlfp, -2);
    }

    /* déplacement du dock dans le cas ou la fenetre est en override redirect */
    if ((INT_OPT_COPY_IF_CHANGED(dock_xpos) || INT_OPT_COPY_IF_CHANGED(dock_ypos)) && 
	Prefs.draw_border && Prefs.use_iconwin==0) {
      XMoveWindow(dock->display, DOCK_WIN(dock), newPrefs.dock_xpos, newPrefs.dock_ypos);
    }
    if (STR_OPT_COPY_IF_CHANGED(dock_bgpixmap) || INT_OPT_COPY_IF_CHANGED(dock_bgcolor)) {
      int showed;
      if ((errmsg=dock_build_pixmap_porte(dock))) {
	fprintf(stderr, errmsg);
      }

      if ((showed = editw_ismapped(dock->editw))) editw_unmap(dock, dock->editw);
      editw_reload_colors(dock, dock->editw);
      if (showed) editw_show(dock, dock->editw,0);
    }
    INT_OPT_COPY(dock_fgcolor);
  

    /* test sur les grosses options du pinnipede */
    if (STR_OPT_COPY_IF_CHANGED(pp_fn_family) || 
	INT_OPT_COPY_IF_CHANGED(pp_fn_size) ||
	STR_OPT_COPY_IF_CHANGED(pp_fortune_fn_family) || 
        INT_OPT_COPY_IF_CHANGED(pp_fortune_fn_size) ||
        INT_OPT_COPY_IF_CHANGED(pp_fortune_bgcolor) ||
        INT_OPT_COPY_IF_CHANGED(pp_fortune_fgcolor)) {
      int showed = pp_ismapped(dock);
      pp_destroy(dock);
      pp_build(dock);
      if (showed) pp_show(dock, &dock->dlfp->tribune);
    }

    if (KEY_LIST_COPY_IF_CHANGED(plopify_key_list)) {
      tribune_update_boitakon(&dock->dlfp->tribune);
      pp_need_refresh = 1;
    }


    /* les options plus light se négocient avec un bon gros refresh */
    if (BIC_OPT_COPY_IF_CHANGED(pp_fgcolor) ||
        INT_OPT_COPY_IF_CHANGED(pp_bgcolor) ||
        BIC_OPT_COPY_IF_CHANGED(pp_tstamp_color) ||
        BIC_OPT_COPY_IF_CHANGED(pp_useragent_color) ||
        BIC_OPT_COPY_IF_CHANGED(pp_login_color) ||
        BIC_OPT_COPY_IF_CHANGED(pp_url_color) ||
	BIC_OPT_COPY_IF_CHANGED(pp_strike_color) ||
	BIC_OPT_COPY_IF_CHANGED(pp_buttonbar_bgcolor) ||
        BIC_OPT_COPY_IF_CHANGED(pp_buttonbar_fgcolor) ||
        BIC_OPT_COPY_IF_CHANGED(pp_buttonbar_msgcnt_color) ||
        BIC_OPT_COPY_IF_CHANGED(pp_buttonbar_updlcnt_color) ||
        BIC_OPT_COPY_IF_CHANGED(pp_buttonbar_progressbar_color) ||
        BIC_OPT_COPY_IF_CHANGED(pp_emph_color) ||
        BIC_OPT_COPY_IF_CHANGED(pp_sel_bgcolor) ||
        BIC_OPT_COPY_IF_CHANGED(pp_popup_fgcolor) ||
        BIC_OPT_COPY_IF_CHANGED(pp_popup_bgcolor) ||
        BIC_OPT_COPY_IF_CHANGED(pp_trollscore_color) ||
        BIC_OPT_COPY_IF_CHANGED(pp_my_msg_color) ||
        BIC_OPT_COPY_IF_CHANGED(pp_answer_my_msg_color) ||
        BIC_OPT_COPY_IF_CHANGED(pp_keyword_color[0]) ||
        BIC_OPT_COPY_IF_CHANGED(pp_keyword_color[1]) ||
        BIC_OPT_COPY_IF_CHANGED(pp_keyword_color[2]) ||
        BIC_OPT_COPY_IF_CHANGED(pp_keyword_color[3]) ||
        BIC_OPT_COPY_IF_CHANGED(pp_keyword_color[4]) ||
        BIC_OPT_COPY_IF_CHANGED(pp_plopify_color) ||
	TRANSP_OPT_COPY_IF_CHANGED(pp_transparency) ||
        INT_OPT_COPY_IF_CHANGED(use_fake_real_transparency) ||
	KEY_LIST_COPY_IF_CHANGED(hilight_key_list) ||
	STRING_LIST_COPY_IF_CHANGED(plop_words,nb_plop_words) ||
	pp_need_refresh)
      {
	pp_set_prefs_colors(dock);
	if (pp_ismapped(dock)) {
	  pp_unmap(dock);
	  pp_show(dock, &dock->dlfp->tribune);
	}
      }
  }
  wmcc_prefs_destroy(&newPrefs);
  free(options_full_file_name);
  myprintf("use_balloons: %d\n", Prefs.use_balloons);
}
