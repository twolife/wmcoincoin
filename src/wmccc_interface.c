/*
 * NE PAS ÉDITER CE FICHIER - il est généré par Glade.
 */

#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>

#include <gdk/gdkkeysyms.h>
#include <gtk/gtk.h>

#include "wmccc_callbacks.h"
#include "wmccc_interface.h"
#include "wmccc_support.h"

GtkWidget*
create_main_win (void)
{
  GtkWidget *main_win;
  GtkWidget *hbox160;
  GtkWidget *vbox1;
  GtkWidget *notebook1;
  GtkWidget *scrolledwindow8;
  GtkWidget *text1;
  GtkWidget *label161;
  GtkWidget *vbox24;
  GtkWidget *frame16;
  GtkWidget *hbox17;
  GtkWidget *frame1;
  GtkWidget *table2;
  GtkWidget *label10;
  GtkWidget *label9;
  GtkWidget *bt_dock_skinpixmap;
  GtkWidget *bt_dock_fgcolor;
  GtkWidget *bt_dock_bgcolor;
  GtkWidget *bt_dock_bgpixmap;
  GtkWidget *label_bgpixmap;
  GtkWidget *frame2;
  GtkWidget *vbox2;
  GtkWidget *checkbutton_dock_use_iconwin;
  GtkWidget *checkbutton_dock_draw_border;
  GtkWidget *checkbutton_dock_use_balloons;
  GtkWidget *checkbutton_start_in_boss_mode;
  GtkWidget *frame17;
  GtkWidget *table16;
  GtkWidget *label79;
  GtkWidget *label87;
  GtkWidget *label88;
  GtkWidget *checkbutton_site_board;
  GtkWidget *checkbutton_site_news;
  GtkWidget *checkbutton_site_messages;
  GtkWidget *checkbutton_site_comments;
  GtkWidget *entry_site_name;
  GtkWidget *label90;
  GtkWidget *entry_site_root_url;
  GtkWidget *hbox16;
  GtkWidget *label91;
  GtkWidget *optionmenu_site_predefined;
  GtkWidget *optionmenu_site_predefined_menu;
  GtkWidget *glade_menuitem;
  GtkWidget *label103;
  GtkWidget *hbox18;
  GtkWidget *scrolledwindow5;
  GtkWidget *text_site_cookie;
  GtkWidget *table26;
  GtkWidget *label112;
  GtkWidget *label133;
  GtkWidget *optionmenu_site_locale;
  GtkWidget *optionmenu_site_locale_menu;
  GtkWidget *entry_user_login;
  GtkWidget *hbox163;
  GtkWidget *entry_site_alias1;
  GtkWidget *entry_site_alias2;
  GtkWidget *entry_site_alias3;
  GtkWidget *dock_tab;
  GtkWidget *vbox25;
  GtkWidget *frame18;
  GtkWidget *vbox3;
  GtkWidget *frame3;
  GtkWidget *hbox2;
  GtkWidget *table4;
  GtkWidget *label11;
  GtkWidget *label12;
  GtkWidget *bt_news_bgcolor;
  GtkWidget *label13;
  GtkWidget *label14;
  GtkWidget *bt_news_fgcolor;
  GtkWidget *bt_news_titles_bgcolor;
  GtkWidget *bt_news_titles_fgcolor;
  GtkWidget *bt_news_emph_color;
  GtkWidget *label19;
  GtkWidget *hbox3;
  GtkWidget *vseparator1;
  GtkWidget *vbox6;
  GtkWidget *label17;
  GtkWidget *hbox4;
  GtkWidget *combo5;
  GList *combo5_items = NULL;
  GtkWidget *comboentry_news_fnfamily;
  GtkObject *spinbutton_news_fnsize_adj;
  GtkWidget *spinbutton_news_fnsize;
  GtkWidget *frame4;
  GtkWidget *table5;
  GtkWidget *label20;
  GtkWidget *label21;
  GtkWidget *label22;
  GtkWidget *label23;
  GtkObject *spinbt_switch_off_coincoin_delay_adj;
  GtkWidget *spinbt_switch_off_coincoin_delay;
  GtkObject *spinbtmax_refresh_delay_adj;
  GtkWidget *spinbtmax_refresh_delay;
  GtkWidget *frame33;
  GtkWidget *table27;
  GtkWidget *label163;
  GtkWidget *label164;
  GtkWidget *label165;
  GtkWidget *label168;
  GtkWidget *label166;
  GtkWidget *label169;
  GtkWidget *bt_sc_bg_color;
  GtkWidget *bt_sc_bg_light_color;
  GtkWidget *bt_sc_bg_dark_color;
  GtkWidget *bt_sc_arrow_normal_color;
  GtkWidget *bt_sc_bar_color;
  GtkWidget *bt_sc_bar_light_color;
  GtkWidget *bt_sc_bar_dark_color;
  GtkWidget *bt_sc_arrow_emphasized_color;
  GtkWidget *bt_sc_bar_color_trans;
  GtkWidget *bt_sc_bar_light_color_trans;
  GtkWidget *bt_sc_bar_dark_color_trans;
  GtkWidget *bt_sc_arrow_emphasized_color_trans;
  GtkWidget *bt_sc_bg_color_trans;
  GtkWidget *bt_sc_bg_light_color_trans;
  GtkWidget *bt_sc_bg_dark_color_trans;
  GtkWidget *bt_sc_arrow_normal_color_trans;
  GtkWidget *label162;
  GtkWidget *label167;
  GtkWidget *frame19;
  GtkWidget *table18;
  GtkWidget *label104;
  GtkWidget *label105;
  GtkWidget *label106;
  GtkWidget *label107;
  GtkObject *spinbutton_news_delay_adj;
  GtkWidget *spinbutton_news_delay;
  GtkObject *spinbutton_news_max_age_adj;
  GtkWidget *spinbutton_news_max_age;
  GtkWidget *tab_News;
  GtkWidget *vbox26;
  GtkWidget *frame20;
  GtkWidget *vbox7;
  GtkWidget *checkbutton_trolldetector;
  GtkWidget *frame5;
  GtkWidget *hbox5;
  GtkWidget *label24;
  GtkWidget *scrolledwindow1;
  GtkWidget *text_post_cmd;
  GtkWidget *frame6;
  GtkWidget *entry_archive_filename;
  GtkWidget *frame7;
  GtkWidget *hbox6;
  GtkWidget *vbox8;
  GtkWidget *scrolledwindow3;
  GtkWidget *clist_miniua;
  GtkWidget *label39;
  GtkWidget *entry_miniua;
  GtkWidget *vbox9;
  GtkWidget *bt_mua_change;
  GtkWidget *bt_mua_add;
  GtkWidget *bt_mua_delete;
  GtkWidget *frame21;
  GtkWidget *table19;
  GtkWidget *label108;
  GtkWidget *label109;
  GtkObject *spinbutton_board_delay_adj;
  GtkWidget *spinbutton_board_delay;
  GtkWidget *label110;
  GtkObject *spinbutton_board_max_msg_adj;
  GtkWidget *spinbutton_board_max_msg;
  GtkWidget *combo1;
  GList *combo1_items = NULL;
  GtkWidget *combo_entry_board_wiki;
  GtkWidget *label111;
  GtkWidget *checkbutton_pp_use_AMPM;
  GtkWidget *checkbutton_board_auto_refresh;
  GtkWidget *label6;
  GtkWidget *vbox11;
  GtkWidget *frame8;
  GtkWidget *entry_coin_coin_message;
  GtkWidget *frame15;
  GtkWidget *vbox22;
  GtkWidget *label92;
  GtkWidget *hseparator3;
  GtkWidget *checkbutton_enable_spell;
  GtkWidget *table17;
  GtkWidget *label93;
  GtkWidget *label94;
  GtkWidget *combo6;
  GList *combo6_items = NULL;
  GtkWidget *entry_spell_dict;
  GtkWidget *combo7;
  GList *combo7_items = NULL;
  GtkWidget *entry_spell_cmd;
  GtkWidget *frame22;
  GtkWidget *table20;
  GtkObject *spinbutton_msg_max_length_adj;
  GtkWidget *spinbutton_msg_max_length;
  GtkObject *spinbutton_ua_max_length_adj;
  GtkWidget *spinbutton_ua_max_length;
  GtkWidget *label116;
  GtkWidget *hbox19;
  GtkWidget *entry_default_ua;
  GtkWidget *button_reset_ua;
  GtkWidget *label114;
  GtkWidget *label115;
  GtkWidget *label113;
  GtkWidget *label7;
  GtkWidget *vbox_pinni;
  GtkWidget *frame24;
  GtkWidget *vbox29;
  GtkWidget *hbox161;
  GtkWidget *checkbutton_pp_auto_open;
  GtkWidget *checkbutton_pp_use_classical_tabs;
  GtkWidget *checkbutton_pp_hungry_boitakon;
  GtkWidget *frame9;
  GtkWidget *vbox13;
  GtkWidget *hbox11;
  GtkWidget *checkbutton_start_in_transparency;
  GtkWidget *checkbutton_use_fake_real_transparency;
  GtkWidget *hseparator1;
  GtkWidget *hbox8;
  GtkWidget *vbox14;
  GtkWidget *label43;
  GtkWidget *optionmenu_transp;
  GtkWidget *optionmenu_transp_menu;
  GtkWidget *vseparator2;
  GtkWidget *table6;
  GtkWidget *label_black_tint;
  GtkWidget *label_shade_min;
  GtkWidget *label_shade_max;
  GtkWidget *label_white_tint;
  GtkWidget *bt_transp_black;
  GtkWidget *bt_transp_white;
  GtkObject *spinbutton_transp_shade_max_adj;
  GtkWidget *spinbutton_transp_shade_max;
  GtkObject *spinbutton_transp_shade_min_adj;
  GtkWidget *spinbutton_transp_shade_min;
  GtkWidget *frame26;
  GtkWidget *hbox20;
  GtkWidget *label118;
  GtkWidget *combo_pp_fn_family;
  GList *combo_pp_fn_family_items = NULL;
  GtkWidget *comboentry_pp_fnfamily;
  GtkWidget *label119;
  GtkObject *spinbutton_pp_fnsize_adj;
  GtkWidget *spinbutton_pp_fnsize;
  GtkWidget *frame10;
  GtkWidget *vbox15;
  GtkWidget *hbox9;
  GtkWidget *table7;
  GtkWidget *label46;
  GtkWidget *bt_pp_emph_color;
  GtkWidget *label47;
  GtkWidget *bt_pp_sel_color;
  GtkWidget *bt_pp_emph_color_trans;
  GtkWidget *bt_pp_sel_color_trans;
  GtkWidget *table8;
  GtkWidget *bt_pp_popup_fgcolor;
  GtkWidget *label49;
  GtkWidget *bt_pp_popup_bgcolor;
  GtkWidget *label50;
  GtkWidget *bt_pp_popup_bgcolor_trans;
  GtkWidget *bt_pp_popup_fgcolor_trans;
  GtkWidget *frame34;
  GtkWidget *table28;
  GtkWidget *checkbutton_show_seconds;
  GtkWidget *checkbutton_show_troll_score;
  GtkWidget *checkbutton_show_fortune;
  GtkWidget *checkbutton_show_tags;
  GtkWidget *optionmenu_nick_mode;
  GtkWidget *optionmenu_nick_mode_menu;
  GtkWidget *label172;
  GtkWidget *frame25;
  GtkWidget *vbox30;
  GtkWidget *checkbutton_mark_id_gaps;
  GtkWidget *frame27;
  GtkWidget *hbox21;
  GtkWidget *table21;
  GtkWidget *label120;
  GtkWidget *label121;
  GtkWidget *label122;
  GtkWidget *bt_pp_bgcolor;
  GtkWidget *bt_pp_fgcolor;
  GtkWidget *bt_pp_fgcolor_trans;
  GtkWidget *bt_pp_clock_color;
  GtkWidget *bt_pp_clock_color_trans;
  GtkWidget *table22;
  GtkWidget *label123;
  GtkWidget *label124;
  GtkWidget *label125;
  GtkWidget *bt_pp_login_color_trans;
  GtkWidget *bt_pp_ua_color;
  GtkWidget *bt_pp_ua_color_trans;
  GtkWidget *bt_pp_trollscore_color;
  GtkWidget *bt_pp_trollscore_color_trans;
  GtkWidget *bt_pp_login_color;
  GtkWidget *table23;
  GtkWidget *label126;
  GtkWidget *label127;
  GtkWidget *label128;
  GtkWidget *bt_pp_url_color;
  GtkWidget *bt_pp_url_color_trans;
  GtkWidget *bt_pp_visited_url_color;
  GtkWidget *bt_pp_visited_url_color_trans;
  GtkWidget *bt_pp_strike_color;
  GtkWidget *bt_pp_strike_color_trans;
  GtkWidget *label8;
  GtkWidget *vbox27;
  GtkWidget *frame23;
  GtkWidget *vbox28;
  GtkWidget *frame11;
  GtkWidget *hbox10;
  GtkWidget *table9;
  GtkWidget *label52;
  GtkWidget *bt_pp_bb_fgcolor;
  GtkWidget *bt_pp_bb_fgcolor_trans;
  GtkWidget *bt_pp_bb_bgcolor_trans;
  GtkWidget *bt_pp_bb_bgcolor;
  GtkWidget *label51;
  GtkWidget *checkbutton_pp_minibar_on;
  GtkWidget *table10;
  GtkWidget *label53;
  GtkWidget *label54;
  GtkWidget *bt_pp_bb_progressbar_color;
  GtkWidget *bt_pp_bb_updlcnt_color;
  GtkWidget *label55;
  GtkWidget *bt_pp_bb_msgcnt_color;
  GtkWidget *bt_pp_bb_progressbar_color_trans;
  GtkWidget *bt_pp_bb_updlcnt_color_trans;
  GtkWidget *bt_pp_bb_msgcnt_color_trans;
  GtkWidget *frame12;
  GtkWidget *vbox16;
  GtkWidget *hbox12;
  GtkWidget *pixmap1;
  GtkWidget *label58;
  GtkWidget *bt_pp_my_messages;
  GtkWidget *bt_pp_my_messages_trans;
  GtkWidget *label59;
  GtkWidget *bt_pp_answers_to_my_messages;
  GtkWidget *bt_pp_answers_to_my_messages_trans;
  GtkWidget *hseparator2;
  GtkWidget *table13;
  GtkWidget *bt_pp_hilight_keyword_color0;
  GtkWidget *bt_pp_hilight_keyword_color2;
  GtkWidget *bt_pp_hilight_keyword_color3;
  GtkWidget *bt_pp_hilight_keyword_color4;
  GtkWidget *bt_pp_hilight_keyword_color0_trans;
  GtkWidget *bt_pp_hilight_keyword_color1_trans;
  GtkWidget *bt_pp_hilight_keyword_color2_trans;
  GtkWidget *bt_pp_hilight_keyword_color3_trans;
  GtkWidget *bt_pp_hilight_keyword_color4_trans;
  GtkWidget *label61;
  GtkWidget *label62;
  GtkWidget *label60;
  GtkWidget *bt_pp_hilight_keyword_color1;
  GtkWidget *hbox14;
  GtkWidget *vbox38;
  GtkWidget *scrolledwindow6;
  GtkWidget *clist_kemph;
  GtkWidget *label155;
  GtkWidget *label156;
  GtkWidget *label157;
  GtkWidget *hbox164;
  GtkWidget *optionmenu_kemph_match;
  GtkWidget *optionmenu_kemph_match_menu;
  GtkWidget *entry_kemph_key;
  GtkWidget *optionmenu_kemph_categ;
  GtkWidget *optionmenu_kemph_categ_menu;
  GtkWidget *vbox35;
  GtkWidget *bt_kemph_up;
  GtkWidget *bt_kemph_down;
  GtkWidget *bt_kemph_del;
  GtkWidget *bt_kemph_change;
  GtkWidget *bt_kemph_add;
  GtkWidget *frame32;
  GtkWidget *vbox371;
  GtkWidget *hbox167;
  GtkWidget *label63;
  GtkWidget *bt_pp_plopified_color;
  GtkWidget *bt_pp_plopified_color_trans;
  GtkWidget *hbox165;
  GtkWidget *vbox39;
  GtkWidget *scrolledwindow7;
  GtkWidget *clist_kplop;
  GtkWidget *label158;
  GtkWidget *label159;
  GtkWidget *label160;
  GtkWidget *hbox166;
  GtkWidget *optionmenu_kplop_match;
  GtkWidget *optionmenu_kplop_match_menu;
  GtkWidget *entry_kplop_key;
  GtkWidget *optionmenu_kplop_categ;
  GtkWidget *optionmenu_kplop_categ_menu;
  GtkWidget *vbox37;
  GtkWidget *bt_kplop_up;
  GtkWidget *bt_kplop_down;
  GtkWidget *bt_kplop_del;
  GtkWidget *bt_kplop_change;
  GtkWidget *bt_kplop_add;
  GtkWidget *label117;
  GtkWidget *vbox31;
  GtkWidget *frame28;
  GtkWidget *vbox17;
  GtkWidget *frame13;
  GtkWidget *vbox18;
  GtkWidget *label64;
  GtkWidget *table14;
  GtkWidget *combo3;
  GList *combo3_items = NULL;
  GtkWidget *comboentry_browser2;
  GtkWidget *hseparator4;
  GtkWidget *combo2;
  GList *combo2_items = NULL;
  GtkWidget *comboentry_browser;
  GtkWidget *label66;
  GtkWidget *label67;
  GtkWidget *frame14;
  GtkWidget *entry_gogole_search;
  GtkWidget *frame29;
  GtkWidget *vbox32;
  GtkWidget *frame30;
  GtkWidget *vbox33;
  GtkWidget *hbox22;
  GtkWidget *label129;
  GtkWidget *entry_proxy;
  GtkWidget *label130;
  GtkObject *spinbutton_proxy_port_adj;
  GtkWidget *spinbutton_proxy_port;
  GtkWidget *label131;
  GtkWidget *entry_proxy_user;
  GtkWidget *label132;
  GtkWidget *entry_proxy_pass;
  GtkWidget *hbox23;
  GtkWidget *checkbutton_proxy_no_cache;
  GtkWidget *checkbutton_proxy_disable_if_modified_since;
  GtkWidget *frame31;
  GtkWidget *table24;
  GtkWidget *label134;
  GtkWidget *label135;
  GtkWidget *label136;
  GtkWidget *entry_path_news_backend;
  GtkWidget *entry_path_myposts;
  GtkWidget *label138;
  GtkWidget *entry_end_news_url;
  GtkWidget *label139;
  GtkWidget *entry_path_messages;
  GtkWidget *label170;
  GtkWidget *entry_path_board_add;
  GtkWidget *label137;
  GtkWidget *entry_board_post;
  GtkWidget *optionmenu_backend_type;
  GtkWidget *optionmenu_backend_type_menu;
  GtkWidget *label171;
  GtkWidget *entry_path_board_backend;
  GtkWidget *label42;
  GtkWidget *hbox162;
  GtkWidget *notebook_sites;
  GtkWidget *empty_notebook_page;
  GtkWidget *label142;
  GtkWidget *label143;
  GtkWidget *bt_mv_site_left;
  GtkWidget *bt_mv_site_right;
  GtkWidget *bt_new_site;
  GtkWidget *bt_remove_site;
  GtkWidget *vbox23;
  GtkWidget *pixmap2;
  GtkWidget *bt_save;
  GtkWidget *bt_save_as;
  GtkWidget *bt_apply;
  GtkWidget *bt_cancel;
  GtkTooltips *tooltips;

  tooltips = gtk_tooltips_new ();

  main_win = gtk_window_new (GTK_WINDOW_TOPLEVEL);
  gtk_object_set_data (GTK_OBJECT (main_win), "main_win", main_win);
  gtk_window_set_title (GTK_WINDOW (main_win), "WMCCC - Darth Vapor");
  gtk_window_set_default_size (GTK_WINDOW (main_win), 512, 512);
  gtk_window_set_policy (GTK_WINDOW (main_win), FALSE, TRUE, TRUE);

  hbox160 = gtk_hbox_new (FALSE, 0);
  gtk_widget_ref (hbox160);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "hbox160", hbox160,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (hbox160);
  gtk_container_add (GTK_CONTAINER (main_win), hbox160);

  vbox1 = gtk_vbox_new (FALSE, 0);
  gtk_widget_ref (vbox1);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "vbox1", vbox1,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (vbox1);
  gtk_box_pack_start (GTK_BOX (hbox160), vbox1, TRUE, TRUE, 0);

  notebook1 = gtk_notebook_new ();
  gtk_widget_ref (notebook1);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "notebook1", notebook1,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (notebook1);
  gtk_box_pack_start (GTK_BOX (vbox1), notebook1, TRUE, TRUE, 0);
  gtk_notebook_set_tab_hborder (GTK_NOTEBOOK (notebook1), 13);
  gtk_notebook_set_tab_vborder (GTK_NOTEBOOK (notebook1), 0);

  scrolledwindow8 = gtk_scrolled_window_new (NULL, NULL);
  gtk_widget_ref (scrolledwindow8);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "scrolledwindow8", scrolledwindow8,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (scrolledwindow8);
  gtk_container_add (GTK_CONTAINER (notebook1), scrolledwindow8);
  gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (scrolledwindow8), GTK_POLICY_NEVER, GTK_POLICY_ALWAYS);

  text1 = gtk_text_new (NULL, NULL);
  gtk_widget_ref (text1);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "text1", text1,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (text1);
  gtk_container_add (GTK_CONTAINER (scrolledwindow8), text1);
  gtk_text_insert (GTK_TEXT (text1), NULL, NULL, NULL,
                   "WMC² 2.4.2\n* bugfixes divers -- tout fonctionne à nouveau sur dlfp, à l'exception du flamophone.\n\nWMC² 2.4.1\n* ajout du menu contextuel\n* pour l'utilisation avec le templeet de linuxfr, voir http://hules.free.fr/wmcoincoin/rtff.html\n\nPASSAGE DE WMC² 2.3.8 à WMC² 2.4\n\nLe format du fichier d'options a beaucoup changé.. du coup il est conseillé d'utiliser le nouveau wmccc pour éditer les options, c'est plus simple. Sinon, lisez le fichier d'options par défaut, c'est expliqué.\n\n* Pour ceux qui ne veulent pas se faire chier, il suffit de rajouter site: \"lenomdusite\" à la _FIN_ de fichier d'options que vous utilisez actuellement, et vous devriez retrouver un coincoin quasi similaire à la v2.3.8 (voir qd même la remarque sur les cookies plus bas)\n\n* Les cookies:\nwmc² demande maintenant le NOM du cookie avec sa valeur, veillez donc à bien rajouter 'session_id=' devant la valeur du cookie.\n\n* Le bouton 'Apply' n'a pas forcément un effet immédiat, si wmc² est occupé à d/l des news ou autre, il faudra attendre quelques secondes (le bouton wmc3 du pinnipède clignote)\n\n* Certaines rares options ne peuvent pas être changées en cours de fonctionnement (comme le skin de wmc²)\n\n* IL EST TRES FORTEMENT CONSEILLE DE REPARTIR DU FICHIER D'OPTIONS PAR DEFAUT, ou alors d'ajouter à votre fichier d'options les diverses lignes 'url_replace' et 'board.miniua' du fichier d'options par défaut.\n\n* Les tabs du pinnipede ont trois états:\n  off : on ne voit pas les messages du site\n  on : les messages du site sont affichés\n  on principale: c'est le site par défaut sur lequel vous postez avec le palmipede.\n\nLe comportement par défaut des tabs pour le clic gauche est le suivant\n  (1) tab off --> tab on principale\n  (2) tab on principale -> tous les autres à on\n  (3) tab on principale + tous les autres à on -> tous les autres à off\n  les clics suivants alternent entre (2) et (3)\n\nSi ce comportement vous déplait, cochez l'option 'classical tabs' dans le volet 'pinnipède' de wmccc.\n\nLes tabs du pinnipede ont aussi une petite croix en bas à droite: celle-ci permet d'annuler l'auto-refresh du site (plus de maj automatique de la tribune du site).", -1);

  label161 = gtk_label_new ("Release Notes");
  gtk_widget_ref (label161);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "label161", label161,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (label161);
  gtk_notebook_set_tab_label (GTK_NOTEBOOK (notebook1), gtk_notebook_get_nth_page (GTK_NOTEBOOK (notebook1), 0), label161);

  vbox24 = gtk_vbox_new (FALSE, 0);
  gtk_widget_ref (vbox24);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "vbox24", vbox24,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (vbox24);
  gtk_container_add (GTK_CONTAINER (notebook1), vbox24);

  frame16 = gtk_frame_new ("Global settings");
  gtk_widget_ref (frame16);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "frame16", frame16,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (frame16);
  gtk_box_pack_start (GTK_BOX (vbox24), frame16, FALSE, TRUE, 0);
  gtk_container_set_border_width (GTK_CONTAINER (frame16), 5);
  gtk_frame_set_shadow_type (GTK_FRAME (frame16), GTK_SHADOW_OUT);

  hbox17 = gtk_hbox_new (FALSE, 0);
  gtk_widget_ref (hbox17);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "hbox17", hbox17,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (hbox17);
  gtk_container_add (GTK_CONTAINER (frame16), hbox17);

  frame1 = gtk_frame_new ("Dock appearance");
  gtk_widget_ref (frame1);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "frame1", frame1,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (frame1);
  gtk_box_pack_start (GTK_BOX (hbox17), frame1, FALSE, TRUE, 0);
  gtk_container_set_border_width (GTK_CONTAINER (frame1), 5);

  table2 = gtk_table_new (4, 2, FALSE);
  gtk_widget_ref (table2);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "table2", table2,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (table2);
  gtk_container_add (GTK_CONTAINER (frame1), table2);
  gtk_container_set_border_width (GTK_CONTAINER (table2), 3);
  gtk_table_set_row_spacings (GTK_TABLE (table2), 4);
  gtk_table_set_col_spacings (GTK_TABLE (table2), 7);

  label10 = gtk_label_new ("Background color");
  gtk_widget_ref (label10);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "label10", label10,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (label10);
  gtk_table_attach (GTK_TABLE (table2), label10, 0, 1, 0, 1,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_misc_set_alignment (GTK_MISC (label10), 0, 0.5);

  label9 = gtk_label_new ("Foreground color");
  gtk_widget_ref (label9);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "label9", label9,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (label9);
  gtk_table_attach (GTK_TABLE (table2), label9, 0, 1, 1, 2,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_misc_set_alignment (GTK_MISC (label9), 0, 0.5);

  bt_dock_skinpixmap = gtk_button_new ();
  gtk_widget_ref (bt_dock_skinpixmap);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "bt_dock_skinpixmap", bt_dock_skinpixmap,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (bt_dock_skinpixmap);
  gtk_table_attach (GTK_TABLE (table2), bt_dock_skinpixmap, 0, 2, 3, 4,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_widget_set_usize (bt_dock_skinpixmap, 324, 68);
  gtk_container_set_border_width (GTK_CONTAINER (bt_dock_skinpixmap), 2);

  bt_dock_fgcolor = gtk_button_new ();
  gtk_widget_ref (bt_dock_fgcolor);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "bt_dock_fgcolor", bt_dock_fgcolor,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (bt_dock_fgcolor);
  gtk_table_attach (GTK_TABLE (table2), bt_dock_fgcolor, 1, 2, 1, 2,
                    (GtkAttachOptions) (0),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_widget_set_usize (bt_dock_fgcolor, 50, 20);

  bt_dock_bgcolor = gtk_button_new ();
  gtk_widget_ref (bt_dock_bgcolor);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "bt_dock_bgcolor", bt_dock_bgcolor,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (bt_dock_bgcolor);
  gtk_table_attach (GTK_TABLE (table2), bt_dock_bgcolor, 1, 2, 0, 1,
                    (GtkAttachOptions) (0),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_widget_set_usize (bt_dock_bgcolor, 50, 20);

  bt_dock_bgpixmap = gtk_button_new ();
  gtk_widget_ref (bt_dock_bgpixmap);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "bt_dock_bgpixmap", bt_dock_bgpixmap,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (bt_dock_bgpixmap);
  gtk_table_attach (GTK_TABLE (table2), bt_dock_bgpixmap, 1, 2, 2, 3,
                    (GtkAttachOptions) (0),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_widget_set_usize (bt_dock_bgpixmap, 68, 68);
  gtk_container_set_border_width (GTK_CONTAINER (bt_dock_bgpixmap), 2);
  gtk_tooltips_set_tip (tooltips, bt_dock_bgpixmap, "coin ! coin !", NULL);

  label_bgpixmap = gtk_label_new ("Optionnal background pixmap");
  gtk_widget_ref (label_bgpixmap);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "label_bgpixmap", label_bgpixmap,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (label_bgpixmap);
  gtk_table_attach (GTK_TABLE (table2), label_bgpixmap, 0, 1, 2, 3,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_misc_set_alignment (GTK_MISC (label_bgpixmap), 0, 0.5);

  frame2 = gtk_frame_new ("Dock options");
  gtk_widget_ref (frame2);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "frame2", frame2,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (frame2);
  gtk_box_pack_start (GTK_BOX (hbox17), frame2, TRUE, TRUE, 0);
  gtk_container_set_border_width (GTK_CONTAINER (frame2), 5);

  vbox2 = gtk_vbox_new (FALSE, 0);
  gtk_widget_ref (vbox2);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "vbox2", vbox2,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (vbox2);
  gtk_container_add (GTK_CONTAINER (frame2), vbox2);

  checkbutton_dock_use_iconwin = gtk_check_button_new_with_label ("Use iconwin");
  gtk_widget_ref (checkbutton_dock_use_iconwin);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "checkbutton_dock_use_iconwin", checkbutton_dock_use_iconwin,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (checkbutton_dock_use_iconwin);
  gtk_box_pack_start (GTK_BOX (vbox2), checkbutton_dock_use_iconwin, FALSE, FALSE, 0);
  gtk_tooltips_set_tip (tooltips, checkbutton_dock_use_iconwin, "Toggle this if you're using a good windowmanager, such as windowmaker. If you're using gnome or kde, untoggle this.", NULL);

  checkbutton_dock_draw_border = gtk_check_button_new_with_label ("Draw border");
  gtk_widget_ref (checkbutton_dock_draw_border);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "checkbutton_dock_draw_border", checkbutton_dock_draw_border,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (checkbutton_dock_draw_border);
  gtk_box_pack_start (GTK_BOX (vbox2), checkbutton_dock_draw_border, FALSE, FALSE, 0);
  gtk_tooltips_set_tip (tooltips, checkbutton_dock_draw_border, "Toggle this if your windowmanager sucks.", NULL);

  checkbutton_dock_use_balloons = gtk_check_button_new_with_label ("Use balloons");
  gtk_widget_ref (checkbutton_dock_use_balloons);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "checkbutton_dock_use_balloons", checkbutton_dock_use_balloons,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (checkbutton_dock_use_balloons);
  gtk_box_pack_start (GTK_BOX (vbox2), checkbutton_dock_use_balloons, FALSE, FALSE, 0);
  gtk_tooltips_set_tip (tooltips, checkbutton_dock_use_balloons, "Toggle this if you don't want to see the help balloons of wmcc (you won't see clipouille anymore)", NULL);

  checkbutton_start_in_boss_mode = gtk_check_button_new_with_label ("Start in boss (clock) mode");
  gtk_widget_ref (checkbutton_start_in_boss_mode);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "checkbutton_start_in_boss_mode", checkbutton_start_in_boss_mode,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (checkbutton_start_in_boss_mode);
  gtk_box_pack_start (GTK_BOX (vbox2), checkbutton_start_in_boss_mode, FALSE, FALSE, 0);
  gtk_tooltips_set_tip (tooltips, checkbutton_start_in_boss_mode, "On startup, wmcc will appear as a simple 'asclock'. Use control+left clic on the applet to switch between clock mode and normal mode.", NULL);

  frame17 = gtk_frame_new ("Site settings");
  gtk_widget_ref (frame17);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "frame17", frame17,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (frame17);
  gtk_box_pack_end (GTK_BOX (vbox24), frame17, FALSE, TRUE, 0);
  gtk_container_set_border_width (GTK_CONTAINER (frame17), 5);
  gtk_frame_set_shadow_type (GTK_FRAME (frame17), GTK_SHADOW_OUT);

  table16 = gtk_table_new (5, 3, FALSE);
  gtk_widget_ref (table16);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "table16", table16,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (table16);
  gtk_container_add (GTK_CONTAINER (frame17), table16);
  gtk_container_set_border_width (GTK_CONTAINER (table16), 5);
  gtk_table_set_col_spacings (GTK_TABLE (table16), 11);

  label79 = gtk_label_new ("Site Name:");
  gtk_widget_ref (label79);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "label79", label79,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (label79);
  gtk_table_attach (GTK_TABLE (table16), label79, 0, 1, 1, 2,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_misc_set_alignment (GTK_MISC (label79), 0, 0.5);

  label87 = gtk_label_new ("Root URL:");
  gtk_widget_ref (label87);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "label87", label87,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (label87);
  gtk_table_attach (GTK_TABLE (table16), label87, 0, 1, 2, 3,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_misc_set_alignment (GTK_MISC (label87), 0, 0.5);

  label88 = gtk_label_new ("Site name aliases:");
  gtk_widget_ref (label88);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "label88", label88,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (label88);
  gtk_table_attach (GTK_TABLE (table16), label88, 0, 1, 3, 4,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_misc_set_alignment (GTK_MISC (label88), 0, 0.5);

  checkbutton_site_board = gtk_check_button_new_with_label ("board");
  gtk_widget_ref (checkbutton_site_board);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "checkbutton_site_board", checkbutton_site_board,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (checkbutton_site_board);
  gtk_table_attach (GTK_TABLE (table16), checkbutton_site_board, 2, 3, 1, 2,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_widget_set_usize (checkbutton_site_board, 116, -2);
  gtk_tooltips_set_tip (tooltips, checkbutton_site_board, "Do you want to see the board of the site in the pinnipede (if it has a valid backend)", NULL);

  checkbutton_site_news = gtk_check_button_new_with_label ("news");
  gtk_widget_ref (checkbutton_site_news);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "checkbutton_site_news", checkbutton_site_news,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (checkbutton_site_news);
  gtk_table_attach (GTK_TABLE (table16), checkbutton_site_news, 2, 3, 2, 3,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_tooltips_set_tip (tooltips, checkbutton_site_news, "Do you want wmcc to retreive the news of the site ?", NULL);

  checkbutton_site_messages = gtk_check_button_new_with_label ("messages");
  gtk_widget_ref (checkbutton_site_messages);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "checkbutton_site_messages", checkbutton_site_messages,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (checkbutton_site_messages);
  gtk_table_attach (GTK_TABLE (table16), checkbutton_site_messages, 2, 3, 3, 4,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_tooltips_set_tip (tooltips, checkbutton_site_messages, "Do you want wmcc to check the arrival of new messages (cookie needed)", NULL);

  checkbutton_site_comments = gtk_check_button_new_with_label ("comments");
  gtk_widget_ref (checkbutton_site_comments);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "checkbutton_site_comments", checkbutton_site_comments,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (checkbutton_site_comments);
  gtk_table_attach (GTK_TABLE (table16), checkbutton_site_comments, 2, 3, 4, 5,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_tooltips_set_tip (tooltips, checkbutton_site_comments, "Do you want wmcc to check for answers to your comments in the news ? (cookie needed)", NULL);

  entry_site_name = gtk_entry_new ();
  gtk_widget_ref (entry_site_name);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "entry_site_name", entry_site_name,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (entry_site_name);
  gtk_table_attach (GTK_TABLE (table16), entry_site_name, 1, 2, 1, 2,
                    (GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_widget_set_usize (entry_site_name, 60, -2);
  gtk_tooltips_set_tip (tooltips, entry_site_name, "Give a short name for the site (less than 8 characters is better)", NULL);

  label90 = gtk_label_new ("Site cookie(s)");
  gtk_widget_ref (label90);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "label90", label90,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (label90);
  gtk_table_attach (GTK_TABLE (table16), label90, 0, 1, 4, 5,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_misc_set_alignment (GTK_MISC (label90), 0, 0.5);

  entry_site_root_url = gtk_entry_new ();
  gtk_widget_ref (entry_site_root_url);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "entry_site_root_url", entry_site_root_url,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (entry_site_root_url);
  gtk_table_attach (GTK_TABLE (table16), entry_site_root_url, 1, 2, 2, 3,
                    (GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_tooltips_set_tip (tooltips, entry_site_root_url, "Give the url of the main dacode page (without the /index.php). See the 'http' tab for related entries", NULL);

  hbox16 = gtk_hbox_new (FALSE, 0);
  gtk_widget_ref (hbox16);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "hbox16", hbox16,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (hbox16);
  gtk_table_attach (GTK_TABLE (table16), hbox16, 1, 2, 0, 1,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (GTK_FILL), 0, 0);

  label91 = gtk_label_new ("Use predefined values");
  gtk_widget_ref (label91);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "label91", label91,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (label91);
  gtk_box_pack_start (GTK_BOX (hbox16), label91, FALSE, FALSE, 2);

  optionmenu_site_predefined = gtk_option_menu_new ();
  gtk_widget_ref (optionmenu_site_predefined);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "optionmenu_site_predefined", optionmenu_site_predefined,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (optionmenu_site_predefined);
  gtk_box_pack_start (GTK_BOX (hbox16), optionmenu_site_predefined, FALSE, FALSE, 0);
  gtk_tooltips_set_tip (tooltips, optionmenu_site_predefined, "Initialize the various entries below with predefined (and hopefully correct) values", NULL);
  optionmenu_site_predefined_menu = gtk_menu_new ();
  glade_menuitem = gtk_menu_item_new_with_label ("no");
  gtk_widget_show (glade_menuitem);
  gtk_menu_append (GTK_MENU (optionmenu_site_predefined_menu), glade_menuitem);
  glade_menuitem = gtk_menu_item_new_with_label ("linuxfr.org");
  gtk_widget_show (glade_menuitem);
  gtk_menu_append (GTK_MENU (optionmenu_site_predefined_menu), glade_menuitem);
  glade_menuitem = gtk_menu_item_new_with_label ("woof.lu");
  gtk_widget_show (glade_menuitem);
  gtk_menu_append (GTK_MENU (optionmenu_site_predefined_menu), glade_menuitem);
  glade_menuitem = gtk_menu_item_new_with_label ("mandrakefr.org");
  gtk_widget_show (glade_menuitem);
  gtk_menu_append (GTK_MENU (optionmenu_site_predefined_menu), glade_menuitem);
  gtk_option_menu_set_menu (GTK_OPTION_MENU (optionmenu_site_predefined), optionmenu_site_predefined_menu);

  label103 = gtk_label_new ("Check what:");
  gtk_widget_ref (label103);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "label103", label103,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (label103);
  gtk_table_attach (GTK_TABLE (table16), label103, 2, 3, 0, 1,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_misc_set_alignment (GTK_MISC (label103), 0, 0.5);

  hbox18 = gtk_hbox_new (FALSE, 5);
  gtk_widget_ref (hbox18);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "hbox18", hbox18,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (hbox18);
  gtk_table_attach (GTK_TABLE (table16), hbox18, 1, 2, 4, 5,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (GTK_FILL), 0, 0);

  scrolledwindow5 = gtk_scrolled_window_new (NULL, NULL);
  gtk_widget_ref (scrolledwindow5);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "scrolledwindow5", scrolledwindow5,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (scrolledwindow5);
  gtk_box_pack_start (GTK_BOX (hbox18), scrolledwindow5, TRUE, TRUE, 0);
  gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (scrolledwindow5), GTK_POLICY_NEVER, GTK_POLICY_AUTOMATIC);

  text_site_cookie = gtk_text_new (NULL, NULL);
  gtk_widget_ref (text_site_cookie);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "text_site_cookie", text_site_cookie,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (text_site_cookie);
  gtk_container_add (GTK_CONTAINER (scrolledwindow5), text_site_cookie);
  gtk_tooltips_set_tip (tooltips, text_site_cookie, "for 'classical' dacode sites, just put something like 'session_id=0934PLOPCOIN', for new.linuxfr.org you have to put 2 cookies, like 'unique_id=0plOP3Co1N; md5=GRUIK5453Pl0p'  (you may also have to fill more specific values in the http panel). Do NOT insert spaces around the '=' character, and put one space after the ';'. The value of the cookies can be found in the Cookie Manager of your browser.", NULL);
  gtk_text_set_editable (GTK_TEXT (text_site_cookie), TRUE);

  table26 = gtk_table_new (2, 2, FALSE);
  gtk_widget_ref (table26);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "table26", table26,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (table26);
  gtk_box_pack_start (GTK_BOX (hbox18), table26, FALSE, FALSE, 0);
  gtk_table_set_col_spacings (GTK_TABLE (table26), 3);

  label112 = gtk_label_new ("Your login:");
  gtk_widget_ref (label112);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "label112", label112,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (label112);
  gtk_table_attach (GTK_TABLE (table26), label112, 0, 1, 0, 1,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_misc_set_padding (GTK_MISC (label112), 5, 0);

  label133 = gtk_label_new ("Site locale:");
  gtk_widget_ref (label133);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "label133", label133,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (label133);
  gtk_table_attach (GTK_TABLE (table26), label133, 0, 1, 1, 2,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_misc_set_alignment (GTK_MISC (label133), 0, 0.5);

  optionmenu_site_locale = gtk_option_menu_new ();
  gtk_widget_ref (optionmenu_site_locale);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "optionmenu_site_locale", optionmenu_site_locale,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (optionmenu_site_locale);
  gtk_table_attach (GTK_TABLE (table26), optionmenu_site_locale, 1, 2, 1, 2,
                    (GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_tooltips_set_tip (tooltips, optionmenu_site_locale, "Setting a wrong value can prevent wmcc from downloading correctly the text of the news", NULL);
  optionmenu_site_locale_menu = gtk_menu_new ();
  glade_menuitem = gtk_menu_item_new_with_label ("EN");
  gtk_widget_show (glade_menuitem);
  gtk_menu_append (GTK_MENU (optionmenu_site_locale_menu), glade_menuitem);
  glade_menuitem = gtk_menu_item_new_with_label ("FR");
  gtk_widget_show (glade_menuitem);
  gtk_menu_append (GTK_MENU (optionmenu_site_locale_menu), glade_menuitem);
  gtk_option_menu_set_menu (GTK_OPTION_MENU (optionmenu_site_locale), optionmenu_site_locale_menu);

  entry_user_login = gtk_entry_new ();
  gtk_widget_ref (entry_user_login);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "entry_user_login", entry_user_login,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (entry_user_login);
  gtk_table_attach (GTK_TABLE (table26), entry_user_login, 1, 2, 0, 1,
                    (GtkAttachOptions) (GTK_EXPAND),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_widget_set_usize (entry_user_login, 110, -2);
  gtk_tooltips_set_tip (tooltips, entry_user_login, "Filling this is useless if you did not fill the cookie field. This information will help to recongnize your messages in the board, but be careful, the login is case-sensitive", NULL);

  hbox163 = gtk_hbox_new (FALSE, 0);
  gtk_widget_ref (hbox163);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "hbox163", hbox163,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (hbox163);
  gtk_table_attach (GTK_TABLE (table16), hbox163, 1, 2, 3, 4,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (GTK_FILL), 0, 0);

  entry_site_alias1 = gtk_entry_new ();
  gtk_widget_ref (entry_site_alias1);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "entry_site_alias1", entry_site_alias1,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (entry_site_alias1);
  gtk_box_pack_start (GTK_BOX (hbox163), entry_site_alias1, TRUE, TRUE, 0);
  gtk_widget_set_usize (entry_site_alias1, 80, -2);
  gtk_tooltips_set_tip (tooltips, entry_site_alias1, "Give alternative site names that other people may be using for 'cross-site' clock (such as 11:11:11@dlfp and 22:22:22@linuxfr)", NULL);

  entry_site_alias2 = gtk_entry_new ();
  gtk_widget_ref (entry_site_alias2);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "entry_site_alias2", entry_site_alias2,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (entry_site_alias2);
  gtk_box_pack_start (GTK_BOX (hbox163), entry_site_alias2, TRUE, TRUE, 0);
  gtk_widget_set_usize (entry_site_alias2, 80, -2);
  gtk_tooltips_set_tip (tooltips, entry_site_alias2, "Give alternative site names that other people may be using for 'cross-site' clock (such as 11:11:11@dlfp and 22:22:22@linuxfr)", NULL);

  entry_site_alias3 = gtk_entry_new ();
  gtk_widget_ref (entry_site_alias3);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "entry_site_alias3", entry_site_alias3,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (entry_site_alias3);
  gtk_box_pack_start (GTK_BOX (hbox163), entry_site_alias3, TRUE, TRUE, 0);
  gtk_widget_set_usize (entry_site_alias3, 80, -2);
  gtk_tooltips_set_tip (tooltips, entry_site_alias3, "Give alternative site names that other people may be using for 'cross-site' clock (such as 11:11:11@dlfp and 22:22:22@linuxfr)", NULL);

  dock_tab = gtk_label_new ("General");
  gtk_widget_ref (dock_tab);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "dock_tab", dock_tab,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (dock_tab);
  gtk_notebook_set_tab_label (GTK_NOTEBOOK (notebook1), gtk_notebook_get_nth_page (GTK_NOTEBOOK (notebook1), 1), dock_tab);

  vbox25 = gtk_vbox_new (FALSE, 0);
  gtk_widget_ref (vbox25);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "vbox25", vbox25,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (vbox25);
  gtk_container_add (GTK_CONTAINER (notebook1), vbox25);

  frame18 = gtk_frame_new ("Global settings");
  gtk_widget_ref (frame18);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "frame18", frame18,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (frame18);
  gtk_box_pack_start (GTK_BOX (vbox25), frame18, FALSE, TRUE, 0);
  gtk_container_set_border_width (GTK_CONTAINER (frame18), 5);
  gtk_frame_set_shadow_type (GTK_FRAME (frame18), GTK_SHADOW_OUT);

  vbox3 = gtk_vbox_new (FALSE, 0);
  gtk_widget_ref (vbox3);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "vbox3", vbox3,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (vbox3);
  gtk_container_add (GTK_CONTAINER (frame18), vbox3);

  frame3 = gtk_frame_new ("News appearance");
  gtk_widget_ref (frame3);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "frame3", frame3,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (frame3);
  gtk_box_pack_start (GTK_BOX (vbox3), frame3, FALSE, TRUE, 0);
  gtk_container_set_border_width (GTK_CONTAINER (frame3), 5);

  hbox2 = gtk_hbox_new (FALSE, 0);
  gtk_widget_ref (hbox2);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "hbox2", hbox2,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (hbox2);
  gtk_container_add (GTK_CONTAINER (frame3), hbox2);

  table4 = gtk_table_new (5, 2, FALSE);
  gtk_widget_ref (table4);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "table4", table4,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (table4);
  gtk_box_pack_start (GTK_BOX (hbox2), table4, FALSE, TRUE, 0);
  gtk_table_set_col_spacings (GTK_TABLE (table4), 4);

  label11 = gtk_label_new ("Background color");
  gtk_widget_ref (label11);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "label11", label11,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (label11);
  gtk_table_attach (GTK_TABLE (table4), label11, 0, 1, 0, 1,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_misc_set_alignment (GTK_MISC (label11), 0, 0.5);
  gtk_misc_set_padding (GTK_MISC (label11), 10, 0);

  label12 = gtk_label_new ("Foreground color");
  gtk_widget_ref (label12);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "label12", label12,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (label12);
  gtk_table_attach (GTK_TABLE (table4), label12, 0, 1, 1, 2,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_misc_set_alignment (GTK_MISC (label12), 0, 0.5);
  gtk_misc_set_padding (GTK_MISC (label12), 10, 0);

  bt_news_bgcolor = gtk_button_new ();
  gtk_widget_ref (bt_news_bgcolor);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "bt_news_bgcolor", bt_news_bgcolor,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (bt_news_bgcolor);
  gtk_table_attach (GTK_TABLE (table4), bt_news_bgcolor, 1, 2, 0, 1,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_widget_set_usize (bt_news_bgcolor, 50, 20);

  label13 = gtk_label_new ("Titles background color");
  gtk_widget_ref (label13);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "label13", label13,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (label13);
  gtk_table_attach (GTK_TABLE (table4), label13, 0, 1, 2, 3,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_misc_set_alignment (GTK_MISC (label13), 0, 0.5);
  gtk_misc_set_padding (GTK_MISC (label13), 10, 0);

  label14 = gtk_label_new ("Title foreground color");
  gtk_widget_ref (label14);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "label14", label14,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (label14);
  gtk_table_attach (GTK_TABLE (table4), label14, 0, 1, 3, 4,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_misc_set_alignment (GTK_MISC (label14), 0, 0.5);
  gtk_misc_set_padding (GTK_MISC (label14), 10, 0);

  bt_news_fgcolor = gtk_button_new ();
  gtk_widget_ref (bt_news_fgcolor);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "bt_news_fgcolor", bt_news_fgcolor,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (bt_news_fgcolor);
  gtk_table_attach (GTK_TABLE (table4), bt_news_fgcolor, 1, 2, 1, 2,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_widget_set_usize (bt_news_fgcolor, 50, 20);

  bt_news_titles_bgcolor = gtk_button_new ();
  gtk_widget_ref (bt_news_titles_bgcolor);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "bt_news_titles_bgcolor", bt_news_titles_bgcolor,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (bt_news_titles_bgcolor);
  gtk_table_attach (GTK_TABLE (table4), bt_news_titles_bgcolor, 1, 2, 2, 3,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_widget_set_usize (bt_news_titles_bgcolor, 50, 20);

  bt_news_titles_fgcolor = gtk_button_new ();
  gtk_widget_ref (bt_news_titles_fgcolor);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "bt_news_titles_fgcolor", bt_news_titles_fgcolor,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (bt_news_titles_fgcolor);
  gtk_table_attach (GTK_TABLE (table4), bt_news_titles_fgcolor, 1, 2, 3, 4,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_widget_set_usize (bt_news_titles_fgcolor, 50, 20);

  bt_news_emph_color = gtk_button_new ();
  gtk_widget_ref (bt_news_emph_color);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "bt_news_emph_color", bt_news_emph_color,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (bt_news_emph_color);
  gtk_table_attach (GTK_TABLE (table4), bt_news_emph_color, 1, 2, 4, 5,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_widget_set_usize (bt_news_emph_color, 50, 20);

  label19 = gtk_label_new ("Emphasized background color");
  gtk_widget_ref (label19);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "label19", label19,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (label19);
  gtk_table_attach (GTK_TABLE (table4), label19, 0, 1, 4, 5,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_misc_set_alignment (GTK_MISC (label19), 0, 0.5);
  gtk_misc_set_padding (GTK_MISC (label19), 10, 0);

  hbox3 = gtk_hbox_new (FALSE, 0);
  gtk_widget_ref (hbox3);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "hbox3", hbox3,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (hbox3);
  gtk_box_pack_start (GTK_BOX (hbox2), hbox3, FALSE, TRUE, 0);

  vseparator1 = gtk_vseparator_new ();
  gtk_widget_ref (vseparator1);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "vseparator1", vseparator1,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (vseparator1);
  gtk_box_pack_start (GTK_BOX (hbox3), vseparator1, TRUE, TRUE, 0);
  gtk_widget_set_usize (vseparator1, 26, -2);

  vbox6 = gtk_vbox_new (FALSE, 0);
  gtk_widget_ref (vbox6);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "vbox6", vbox6,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (vbox6);
  gtk_box_pack_start (GTK_BOX (hbox3), vbox6, FALSE, TRUE, 0);

  label17 = gtk_label_new ("News font");
  gtk_widget_ref (label17);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "label17", label17,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (label17);
  gtk_box_pack_start (GTK_BOX (vbox6), label17, FALSE, FALSE, 0);

  hbox4 = gtk_hbox_new (FALSE, 0);
  gtk_widget_ref (hbox4);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "hbox4", hbox4,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (hbox4);
  gtk_box_pack_start (GTK_BOX (vbox6), hbox4, FALSE, TRUE, 0);

  combo5 = gtk_combo_new ();
  gtk_widget_ref (combo5);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "combo5", combo5,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (combo5);
  gtk_box_pack_start (GTK_BOX (hbox4), combo5, FALSE, FALSE, 0);
  combo5_items = g_list_append (combo5_items, (gpointer) "helvetica");
  combo5_items = g_list_append (combo5_items, (gpointer) "verdana");
  combo5_items = g_list_append (combo5_items, (gpointer) "fixed");
  combo5_items = g_list_append (combo5_items, (gpointer) "courier");
  combo5_items = g_list_append (combo5_items, (gpointer) "arial");
  gtk_combo_set_popdown_strings (GTK_COMBO (combo5), combo5_items);
  g_list_free (combo5_items);

  comboentry_news_fnfamily = GTK_COMBO (combo5)->entry;
  gtk_widget_ref (comboentry_news_fnfamily);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "comboentry_news_fnfamily", comboentry_news_fnfamily,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (comboentry_news_fnfamily);
  gtk_entry_set_text (GTK_ENTRY (comboentry_news_fnfamily), "helvetica");

  spinbutton_news_fnsize_adj = gtk_adjustment_new (1, 4, 100, 1, 10, 10);
  spinbutton_news_fnsize = gtk_spin_button_new (GTK_ADJUSTMENT (spinbutton_news_fnsize_adj), 1, 0);
  gtk_widget_ref (spinbutton_news_fnsize);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "spinbutton_news_fnsize", spinbutton_news_fnsize,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (spinbutton_news_fnsize);
  gtk_box_pack_start (GTK_BOX (hbox4), spinbutton_news_fnsize, FALSE, FALSE, 0);
  gtk_widget_set_usize (spinbutton_news_fnsize, 60, -2);
  gtk_spin_button_set_numeric (GTK_SPIN_BUTTON (spinbutton_news_fnsize), TRUE);
  gtk_spin_button_set_update_policy (GTK_SPIN_BUTTON (spinbutton_news_fnsize), GTK_UPDATE_IF_VALID);

  frame4 = gtk_frame_new ("General news options");
  gtk_widget_ref (frame4);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "frame4", frame4,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (frame4);
  gtk_box_pack_start (GTK_BOX (vbox3), frame4, FALSE, TRUE, 0);
  gtk_container_set_border_width (GTK_CONTAINER (frame4), 5);

  table5 = gtk_table_new (2, 3, FALSE);
  gtk_widget_ref (table5);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "table5", table5,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (table5);
  gtk_container_add (GTK_CONTAINER (frame4), table5);
  gtk_table_set_row_spacings (GTK_TABLE (table5), 4);
  gtk_table_set_col_spacings (GTK_TABLE (table5), 5);

  label20 = gtk_label_new ("Max. delay of inactivity before switch off ");
  gtk_widget_ref (label20);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "label20", label20,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (label20);
  gtk_table_attach (GTK_TABLE (table5), label20, 0, 1, 0, 1,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_misc_set_alignment (GTK_MISC (label20), 0, 0.5);
  gtk_misc_set_padding (GTK_MISC (label20), 10, 0);

  label21 = gtk_label_new ("Max. delay between two refresh (setting this parameter to 0 will \ndisable the dynamic adjustment of the refresh rate)");
  gtk_widget_ref (label21);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "label21", label21,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (label21);
  gtk_table_attach (GTK_TABLE (table5), label21, 0, 1, 1, 2,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_label_set_justify (GTK_LABEL (label21), GTK_JUSTIFY_FILL);
  gtk_misc_set_alignment (GTK_MISC (label21), 0, 0.5);
  gtk_misc_set_padding (GTK_MISC (label21), 10, 0);

  label22 = gtk_label_new ("minutes");
  gtk_widget_ref (label22);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "label22", label22,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (label22);
  gtk_table_attach (GTK_TABLE (table5), label22, 2, 3, 0, 1,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_misc_set_alignment (GTK_MISC (label22), 0, 0.5);

  label23 = gtk_label_new ("minutes");
  gtk_widget_ref (label23);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "label23", label23,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (label23);
  gtk_table_attach (GTK_TABLE (table5), label23, 2, 3, 1, 2,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_misc_set_alignment (GTK_MISC (label23), 0, 0.5);

  spinbt_switch_off_coincoin_delay_adj = gtk_adjustment_new (1, 0, 1000, 1, 10, 10);
  spinbt_switch_off_coincoin_delay = gtk_spin_button_new (GTK_ADJUSTMENT (spinbt_switch_off_coincoin_delay_adj), 1, 0);
  gtk_widget_ref (spinbt_switch_off_coincoin_delay);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "spinbt_switch_off_coincoin_delay", spinbt_switch_off_coincoin_delay,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (spinbt_switch_off_coincoin_delay);
  gtk_table_attach (GTK_TABLE (table5), spinbt_switch_off_coincoin_delay, 1, 2, 0, 1,
                    (GtkAttachOptions) (0),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_widget_set_usize (spinbt_switch_off_coincoin_delay, 60, -2);

  spinbtmax_refresh_delay_adj = gtk_adjustment_new (1, 0, 100, 1, 10, 10);
  spinbtmax_refresh_delay = gtk_spin_button_new (GTK_ADJUSTMENT (spinbtmax_refresh_delay_adj), 1, 0);
  gtk_widget_ref (spinbtmax_refresh_delay);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "spinbtmax_refresh_delay", spinbtmax_refresh_delay,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (spinbtmax_refresh_delay);
  gtk_table_attach (GTK_TABLE (table5), spinbtmax_refresh_delay, 1, 2, 1, 2,
                    (GtkAttachOptions) (0),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_widget_set_usize (spinbtmax_refresh_delay, 60, -2);

  frame33 = gtk_frame_new ("Scrollcoin colors");
  gtk_widget_ref (frame33);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "frame33", frame33,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (frame33);
  gtk_box_pack_start (GTK_BOX (vbox3), frame33, TRUE, TRUE, 0);
  gtk_container_set_border_width (GTK_CONTAINER (frame33), 5);

  table27 = gtk_table_new (4, 6, FALSE);
  gtk_widget_ref (table27);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "table27", table27,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (table27);
  gtk_container_add (GTK_CONTAINER (frame33), table27);

  label163 = gtk_label_new ("Background light");
  gtk_widget_ref (label163);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "label163", label163,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (label163);
  gtk_table_attach (GTK_TABLE (table27), label163, 0, 1, 1, 2,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_misc_set_alignment (GTK_MISC (label163), 0, 0.5);
  gtk_misc_set_padding (GTK_MISC (label163), 10, 0);

  label164 = gtk_label_new ("Bakcground dark");
  gtk_widget_ref (label164);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "label164", label164,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (label164);
  gtk_table_attach (GTK_TABLE (table27), label164, 0, 1, 2, 3,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_misc_set_alignment (GTK_MISC (label164), 0, 0.5);
  gtk_misc_set_padding (GTK_MISC (label164), 10, 0);

  label165 = gtk_label_new ("Arrow normal");
  gtk_widget_ref (label165);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "label165", label165,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (label165);
  gtk_table_attach (GTK_TABLE (table27), label165, 0, 1, 3, 4,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_misc_set_alignment (GTK_MISC (label165), 0, 0.5);
  gtk_misc_set_padding (GTK_MISC (label165), 10, 0);

  label168 = gtk_label_new ("Bar dark pixel");
  gtk_widget_ref (label168);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "label168", label168,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (label168);
  gtk_table_attach (GTK_TABLE (table27), label168, 3, 4, 2, 3,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_misc_set_alignment (GTK_MISC (label168), 0, 0.5);
  gtk_misc_set_padding (GTK_MISC (label168), 10, 0);

  label166 = gtk_label_new ("Arrow emphasized");
  gtk_widget_ref (label166);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "label166", label166,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (label166);
  gtk_table_attach (GTK_TABLE (table27), label166, 3, 4, 3, 4,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_misc_set_alignment (GTK_MISC (label166), 0, 0.5);
  gtk_misc_set_padding (GTK_MISC (label166), 10, 0);

  label169 = gtk_label_new ("Bar light pixel");
  gtk_widget_ref (label169);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "label169", label169,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (label169);
  gtk_table_attach (GTK_TABLE (table27), label169, 3, 4, 1, 2,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_misc_set_alignment (GTK_MISC (label169), 0, 0.5);
  gtk_misc_set_padding (GTK_MISC (label169), 10, 0);

  bt_sc_bg_color = gtk_button_new ();
  gtk_widget_ref (bt_sc_bg_color);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "bt_sc_bg_color", bt_sc_bg_color,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (bt_sc_bg_color);
  gtk_table_attach (GTK_TABLE (table27), bt_sc_bg_color, 1, 2, 0, 1,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_widget_set_usize (bt_sc_bg_color, 50, 20);
  gtk_tooltips_set_tip (tooltips, bt_sc_bg_color, "Color in normal (non transparent) mode", NULL);

  bt_sc_bg_light_color = gtk_button_new ();
  gtk_widget_ref (bt_sc_bg_light_color);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "bt_sc_bg_light_color", bt_sc_bg_light_color,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (bt_sc_bg_light_color);
  gtk_table_attach (GTK_TABLE (table27), bt_sc_bg_light_color, 1, 2, 1, 2,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_widget_set_usize (bt_sc_bg_light_color, 50, 20);
  gtk_tooltips_set_tip (tooltips, bt_sc_bg_light_color, "Color in normal (non transparent) mode", NULL);

  bt_sc_bg_dark_color = gtk_button_new ();
  gtk_widget_ref (bt_sc_bg_dark_color);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "bt_sc_bg_dark_color", bt_sc_bg_dark_color,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (bt_sc_bg_dark_color);
  gtk_table_attach (GTK_TABLE (table27), bt_sc_bg_dark_color, 1, 2, 2, 3,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_widget_set_usize (bt_sc_bg_dark_color, 50, 20);
  gtk_tooltips_set_tip (tooltips, bt_sc_bg_dark_color, "Color in normal (non transparent) mode", NULL);

  bt_sc_arrow_normal_color = gtk_button_new ();
  gtk_widget_ref (bt_sc_arrow_normal_color);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "bt_sc_arrow_normal_color", bt_sc_arrow_normal_color,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (bt_sc_arrow_normal_color);
  gtk_table_attach (GTK_TABLE (table27), bt_sc_arrow_normal_color, 1, 2, 3, 4,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_widget_set_usize (bt_sc_arrow_normal_color, 50, 20);
  gtk_tooltips_set_tip (tooltips, bt_sc_arrow_normal_color, "Color in normal (non transparent) mode", NULL);

  bt_sc_bar_color = gtk_button_new ();
  gtk_widget_ref (bt_sc_bar_color);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "bt_sc_bar_color", bt_sc_bar_color,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (bt_sc_bar_color);
  gtk_table_attach (GTK_TABLE (table27), bt_sc_bar_color, 4, 5, 0, 1,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_widget_set_usize (bt_sc_bar_color, 50, 20);
  gtk_tooltips_set_tip (tooltips, bt_sc_bar_color, "Color in normal (non transparent) mode", NULL);

  bt_sc_bar_light_color = gtk_button_new ();
  gtk_widget_ref (bt_sc_bar_light_color);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "bt_sc_bar_light_color", bt_sc_bar_light_color,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (bt_sc_bar_light_color);
  gtk_table_attach (GTK_TABLE (table27), bt_sc_bar_light_color, 4, 5, 1, 2,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_widget_set_usize (bt_sc_bar_light_color, 50, 20);
  gtk_tooltips_set_tip (tooltips, bt_sc_bar_light_color, "Color in normal (non transparent) mode", NULL);

  bt_sc_bar_dark_color = gtk_button_new ();
  gtk_widget_ref (bt_sc_bar_dark_color);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "bt_sc_bar_dark_color", bt_sc_bar_dark_color,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (bt_sc_bar_dark_color);
  gtk_table_attach (GTK_TABLE (table27), bt_sc_bar_dark_color, 4, 5, 2, 3,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_widget_set_usize (bt_sc_bar_dark_color, 50, 20);
  gtk_tooltips_set_tip (tooltips, bt_sc_bar_dark_color, "Color in normal (non transparent) mode", NULL);

  bt_sc_arrow_emphasized_color = gtk_button_new ();
  gtk_widget_ref (bt_sc_arrow_emphasized_color);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "bt_sc_arrow_emphasized_color", bt_sc_arrow_emphasized_color,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (bt_sc_arrow_emphasized_color);
  gtk_table_attach (GTK_TABLE (table27), bt_sc_arrow_emphasized_color, 4, 5, 3, 4,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_widget_set_usize (bt_sc_arrow_emphasized_color, 50, 20);
  gtk_tooltips_set_tip (tooltips, bt_sc_arrow_emphasized_color, "Color in normal (non transparent) mode", NULL);

  bt_sc_bar_color_trans = gtk_button_new ();
  gtk_widget_ref (bt_sc_bar_color_trans);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "bt_sc_bar_color_trans", bt_sc_bar_color_trans,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (bt_sc_bar_color_trans);
  gtk_table_attach (GTK_TABLE (table27), bt_sc_bar_color_trans, 5, 6, 0, 1,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_widget_set_usize (bt_sc_bar_color_trans, 50, 20);
  gtk_tooltips_set_tip (tooltips, bt_sc_bar_color_trans, "Color in transparent mode", NULL);

  bt_sc_bar_light_color_trans = gtk_button_new ();
  gtk_widget_ref (bt_sc_bar_light_color_trans);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "bt_sc_bar_light_color_trans", bt_sc_bar_light_color_trans,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (bt_sc_bar_light_color_trans);
  gtk_table_attach (GTK_TABLE (table27), bt_sc_bar_light_color_trans, 5, 6, 1, 2,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_widget_set_usize (bt_sc_bar_light_color_trans, 50, 20);
  gtk_tooltips_set_tip (tooltips, bt_sc_bar_light_color_trans, "Color in transparent mode", NULL);

  bt_sc_bar_dark_color_trans = gtk_button_new ();
  gtk_widget_ref (bt_sc_bar_dark_color_trans);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "bt_sc_bar_dark_color_trans", bt_sc_bar_dark_color_trans,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (bt_sc_bar_dark_color_trans);
  gtk_table_attach (GTK_TABLE (table27), bt_sc_bar_dark_color_trans, 5, 6, 2, 3,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_widget_set_usize (bt_sc_bar_dark_color_trans, 50, 20);
  gtk_tooltips_set_tip (tooltips, bt_sc_bar_dark_color_trans, "Color in transparent mode", NULL);

  bt_sc_arrow_emphasized_color_trans = gtk_button_new ();
  gtk_widget_ref (bt_sc_arrow_emphasized_color_trans);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "bt_sc_arrow_emphasized_color_trans", bt_sc_arrow_emphasized_color_trans,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (bt_sc_arrow_emphasized_color_trans);
  gtk_table_attach (GTK_TABLE (table27), bt_sc_arrow_emphasized_color_trans, 5, 6, 3, 4,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_widget_set_usize (bt_sc_arrow_emphasized_color_trans, 50, 20);
  gtk_tooltips_set_tip (tooltips, bt_sc_arrow_emphasized_color_trans, "Color in transparent mode", NULL);

  bt_sc_bg_color_trans = gtk_button_new ();
  gtk_widget_ref (bt_sc_bg_color_trans);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "bt_sc_bg_color_trans", bt_sc_bg_color_trans,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (bt_sc_bg_color_trans);
  gtk_table_attach (GTK_TABLE (table27), bt_sc_bg_color_trans, 2, 3, 0, 1,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_widget_set_usize (bt_sc_bg_color_trans, 50, 20);
  gtk_tooltips_set_tip (tooltips, bt_sc_bg_color_trans, "Color in transparent mode", NULL);

  bt_sc_bg_light_color_trans = gtk_button_new ();
  gtk_widget_ref (bt_sc_bg_light_color_trans);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "bt_sc_bg_light_color_trans", bt_sc_bg_light_color_trans,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (bt_sc_bg_light_color_trans);
  gtk_table_attach (GTK_TABLE (table27), bt_sc_bg_light_color_trans, 2, 3, 1, 2,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_widget_set_usize (bt_sc_bg_light_color_trans, 50, 20);
  gtk_tooltips_set_tip (tooltips, bt_sc_bg_light_color_trans, "Color in transparent mode", NULL);

  bt_sc_bg_dark_color_trans = gtk_button_new ();
  gtk_widget_ref (bt_sc_bg_dark_color_trans);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "bt_sc_bg_dark_color_trans", bt_sc_bg_dark_color_trans,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (bt_sc_bg_dark_color_trans);
  gtk_table_attach (GTK_TABLE (table27), bt_sc_bg_dark_color_trans, 2, 3, 2, 3,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_widget_set_usize (bt_sc_bg_dark_color_trans, 50, 20);
  gtk_tooltips_set_tip (tooltips, bt_sc_bg_dark_color_trans, "Color in transparent mode", NULL);

  bt_sc_arrow_normal_color_trans = gtk_button_new ();
  gtk_widget_ref (bt_sc_arrow_normal_color_trans);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "bt_sc_arrow_normal_color_trans", bt_sc_arrow_normal_color_trans,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (bt_sc_arrow_normal_color_trans);
  gtk_table_attach (GTK_TABLE (table27), bt_sc_arrow_normal_color_trans, 2, 3, 3, 4,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_widget_set_usize (bt_sc_arrow_normal_color_trans, 50, 20);
  gtk_tooltips_set_tip (tooltips, bt_sc_arrow_normal_color_trans, "Color in transparent mode", NULL);

  label162 = gtk_label_new ("Background color");
  gtk_widget_ref (label162);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "label162", label162,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (label162);
  gtk_table_attach (GTK_TABLE (table27), label162, 0, 1, 0, 1,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_misc_set_alignment (GTK_MISC (label162), 0, 0.5);
  gtk_misc_set_padding (GTK_MISC (label162), 10, 0);

  label167 = gtk_label_new ("Bar pixel");
  gtk_widget_ref (label167);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "label167", label167,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (label167);
  gtk_table_attach (GTK_TABLE (table27), label167, 3, 4, 0, 1,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_label_set_justify (GTK_LABEL (label167), GTK_JUSTIFY_LEFT);
  gtk_misc_set_alignment (GTK_MISC (label167), 1.49012e-08, 0.5);
  gtk_misc_set_padding (GTK_MISC (label167), 10, 0);

  frame19 = gtk_frame_new ("Site settings");
  gtk_widget_ref (frame19);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "frame19", frame19,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (frame19);
  gtk_box_pack_end (GTK_BOX (vbox25), frame19, FALSE, TRUE, 0);
  gtk_container_set_border_width (GTK_CONTAINER (frame19), 5);
  gtk_frame_set_shadow_type (GTK_FRAME (frame19), GTK_SHADOW_OUT);

  table18 = gtk_table_new (2, 3, FALSE);
  gtk_widget_ref (table18);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "table18", table18,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (table18);
  gtk_container_add (GTK_CONTAINER (frame19), table18);

  label104 = gtk_label_new ("Delay between each check of the news");
  gtk_widget_ref (label104);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "label104", label104,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (label104);
  gtk_table_attach (GTK_TABLE (table18), label104, 0, 1, 0, 1,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_misc_set_alignment (GTK_MISC (label104), 0, 0.5);

  label105 = gtk_label_new ("Maximum age of the news retrieved");
  gtk_widget_ref (label105);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "label105", label105,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (label105);
  gtk_table_attach (GTK_TABLE (table18), label105, 0, 1, 1, 2,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_misc_set_alignment (GTK_MISC (label105), 0, 0.5);

  label106 = gtk_label_new ("seconds");
  gtk_widget_ref (label106);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "label106", label106,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (label106);
  gtk_table_attach (GTK_TABLE (table18), label106, 2, 3, 0, 1,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_label_set_justify (GTK_LABEL (label106), GTK_JUSTIFY_RIGHT);
  gtk_misc_set_alignment (GTK_MISC (label106), 0, 0.5);

  label107 = gtk_label_new ("days");
  gtk_widget_ref (label107);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "label107", label107,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (label107);
  gtk_table_attach (GTK_TABLE (table18), label107, 2, 3, 1, 2,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_label_set_justify (GTK_LABEL (label107), GTK_JUSTIFY_RIGHT);
  gtk_misc_set_alignment (GTK_MISC (label107), 0, 0.5);

  spinbutton_news_delay_adj = gtk_adjustment_new (1, 60, 10000, 1, 10, 10);
  spinbutton_news_delay = gtk_spin_button_new (GTK_ADJUSTMENT (spinbutton_news_delay_adj), 1, 0);
  gtk_widget_ref (spinbutton_news_delay);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "spinbutton_news_delay", spinbutton_news_delay,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (spinbutton_news_delay);
  gtk_table_attach (GTK_TABLE (table18), spinbutton_news_delay, 1, 2, 0, 1,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 5, 0);
  gtk_widget_set_usize (spinbutton_news_delay, 60, -2);
  gtk_spin_button_set_numeric (GTK_SPIN_BUTTON (spinbutton_news_delay), TRUE);
  gtk_spin_button_set_update_policy (GTK_SPIN_BUTTON (spinbutton_news_delay), GTK_UPDATE_IF_VALID);

  spinbutton_news_max_age_adj = gtk_adjustment_new (1, 1, 365, 1, 10, 10);
  spinbutton_news_max_age = gtk_spin_button_new (GTK_ADJUSTMENT (spinbutton_news_max_age_adj), 1, 0);
  gtk_widget_ref (spinbutton_news_max_age);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "spinbutton_news_max_age", spinbutton_news_max_age,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (spinbutton_news_max_age);
  gtk_table_attach (GTK_TABLE (table18), spinbutton_news_max_age, 1, 2, 1, 2,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 5, 0);
  gtk_spin_button_set_numeric (GTK_SPIN_BUTTON (spinbutton_news_max_age), TRUE);
  gtk_spin_button_set_update_policy (GTK_SPIN_BUTTON (spinbutton_news_max_age), GTK_UPDATE_IF_VALID);

  tab_News = gtk_label_new ("News");
  gtk_widget_ref (tab_News);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "tab_News", tab_News,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (tab_News);
  gtk_notebook_set_tab_label (GTK_NOTEBOOK (notebook1), gtk_notebook_get_nth_page (GTK_NOTEBOOK (notebook1), 2), tab_News);

  vbox26 = gtk_vbox_new (FALSE, 0);
  gtk_widget_ref (vbox26);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "vbox26", vbox26,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (vbox26);
  gtk_container_add (GTK_CONTAINER (notebook1), vbox26);

  frame20 = gtk_frame_new ("Global settings");
  gtk_widget_ref (frame20);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "frame20", frame20,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (frame20);
  gtk_box_pack_start (GTK_BOX (vbox26), frame20, TRUE, TRUE, 0);
  gtk_container_set_border_width (GTK_CONTAINER (frame20), 5);
  gtk_frame_set_shadow_type (GTK_FRAME (frame20), GTK_SHADOW_OUT);

  vbox7 = gtk_vbox_new (FALSE, 0);
  gtk_widget_ref (vbox7);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "vbox7", vbox7,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (vbox7);
  gtk_container_add (GTK_CONTAINER (frame20), vbox7);

  checkbutton_trolldetector = gtk_check_button_new_with_label ("Enable troll detector");
  gtk_widget_ref (checkbutton_trolldetector);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "checkbutton_trolldetector", checkbutton_trolldetector,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (checkbutton_trolldetector);
  gtk_box_pack_start (GTK_BOX (vbox7), checkbutton_trolldetector, FALSE, FALSE, 0);

  frame5 = gtk_frame_new ("Post-processing of messages via shell");
  gtk_widget_ref (frame5);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "frame5", frame5,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (frame5);
  gtk_box_pack_start (GTK_BOX (vbox7), frame5, FALSE, TRUE, 0);
  gtk_container_set_border_width (GTK_CONTAINER (frame5), 5);

  hbox5 = gtk_hbox_new (FALSE, 6);
  gtk_widget_ref (hbox5);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "hbox5", hbox5,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (hbox5);
  gtk_container_add (GTK_CONTAINER (frame5), hbox5);
  gtk_container_set_border_width (GTK_CONTAINER (hbox5), 7);

  label24 = gtk_label_new ("Shell command [CAUTION: be careful this script can receive any sequence of input arguments, be VERY CAREFUL for security risks]");
  gtk_widget_ref (label24);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "label24", label24,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (label24);
  gtk_box_pack_start (GTK_BOX (hbox5), label24, FALSE, FALSE, 0);
  gtk_widget_set_usize (label24, 160, -2);
  gtk_label_set_justify (GTK_LABEL (label24), GTK_JUSTIFY_FILL);
  gtk_label_set_line_wrap (GTK_LABEL (label24), TRUE);

  scrolledwindow1 = gtk_scrolled_window_new (NULL, NULL);
  gtk_widget_ref (scrolledwindow1);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "scrolledwindow1", scrolledwindow1,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (scrolledwindow1);
  gtk_box_pack_start (GTK_BOX (hbox5), scrolledwindow1, TRUE, TRUE, 0);
  gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (scrolledwindow1), GTK_POLICY_NEVER, GTK_POLICY_ALWAYS);

  text_post_cmd = gtk_text_new (NULL, NULL);
  gtk_widget_ref (text_post_cmd);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "text_post_cmd", text_post_cmd,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (text_post_cmd);
  gtk_container_add (GTK_CONTAINER (scrolledwindow1), text_post_cmd);
  gtk_tooltips_set_tip (tooltips, text_post_cmd, "$l -> login, $m -> message, $u -> useragent, $i -> #id du message, $t -> timestamp, $s -> troll_score,  $R -> message type ( bit 0: message posted by you, bit 1: answer to one of your messages, bit 2: message selected by a keyword, bit 3: plopified/boitakoned message), $v -> wmc² version, $h -> remote host", NULL);
  gtk_text_set_editable (GTK_TEXT (text_post_cmd), TRUE);

  frame6 = gtk_frame_new ("Archive file (use ctrl+middle clic in the pinnipede to store the board content)");
  gtk_widget_ref (frame6);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "frame6", frame6,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (frame6);
  gtk_box_pack_start (GTK_BOX (vbox7), frame6, FALSE, TRUE, 0);
  gtk_container_set_border_width (GTK_CONTAINER (frame6), 5);

  entry_archive_filename = gtk_entry_new ();
  gtk_widget_ref (entry_archive_filename);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "entry_archive_filename", entry_archive_filename,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (entry_archive_filename);
  gtk_container_add (GTK_CONTAINER (frame6), entry_archive_filename);
  gtk_tooltips_set_tip (tooltips, entry_archive_filename, "html filename", NULL);

  frame7 = gtk_frame_new ("Rules applied to useragents");
  gtk_widget_ref (frame7);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "frame7", frame7,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (frame7);
  gtk_box_pack_start (GTK_BOX (vbox7), frame7, TRUE, TRUE, 0);
  gtk_container_set_border_width (GTK_CONTAINER (frame7), 5);

  hbox6 = gtk_hbox_new (FALSE, 10);
  gtk_widget_ref (hbox6);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "hbox6", hbox6,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (hbox6);
  gtk_container_add (GTK_CONTAINER (frame7), hbox6);

  vbox8 = gtk_vbox_new (FALSE, 4);
  gtk_widget_ref (vbox8);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "vbox8", vbox8,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (vbox8);
  gtk_box_pack_start (GTK_BOX (hbox6), vbox8, TRUE, TRUE, 0);

  scrolledwindow3 = gtk_scrolled_window_new (NULL, NULL);
  gtk_widget_ref (scrolledwindow3);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "scrolledwindow3", scrolledwindow3,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (scrolledwindow3);
  gtk_box_pack_start (GTK_BOX (vbox8), scrolledwindow3, TRUE, TRUE, 0);
  gtk_container_set_border_width (GTK_CONTAINER (scrolledwindow3), 2);
  gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (scrolledwindow3), GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);

  clist_miniua = gtk_clist_new (1);
  gtk_widget_ref (clist_miniua);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "clist_miniua", clist_miniua,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (clist_miniua);
  gtk_container_add (GTK_CONTAINER (scrolledwindow3), clist_miniua);
  gtk_clist_set_column_width (GTK_CLIST (clist_miniua), 0, 80);
  gtk_clist_column_titles_show (GTK_CLIST (clist_miniua));

  label39 = gtk_label_new ("MiniUA Rules");
  gtk_widget_ref (label39);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "label39", label39,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (label39);
  gtk_clist_set_column_widget (GTK_CLIST (clist_miniua), 0, label39);

  entry_miniua = gtk_entry_new ();
  gtk_widget_ref (entry_miniua);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "entry_miniua", entry_miniua,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_box_pack_start (GTK_BOX (vbox8), entry_miniua, FALSE, FALSE, 0);

  vbox9 = gtk_vbox_new (FALSE, 1);
  gtk_widget_ref (vbox9);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "vbox9", vbox9,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (vbox9);
  gtk_box_pack_end (GTK_BOX (hbox6), vbox9, FALSE, FALSE, 0);
  gtk_container_set_border_width (GTK_CONTAINER (vbox9), 3);

  bt_mua_change = gtk_button_new_with_label ("Change");
  gtk_widget_ref (bt_mua_change);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "bt_mua_change", bt_mua_change,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_box_pack_end (GTK_BOX (vbox9), bt_mua_change, FALSE, FALSE, 0);

  bt_mua_add = gtk_button_new_with_label ("Add");
  gtk_widget_ref (bt_mua_add);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "bt_mua_add", bt_mua_add,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_box_pack_end (GTK_BOX (vbox9), bt_mua_add, FALSE, FALSE, 0);

  bt_mua_delete = gtk_button_new_with_label ("Delete");
  gtk_widget_ref (bt_mua_delete);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "bt_mua_delete", bt_mua_delete,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_box_pack_end (GTK_BOX (vbox9), bt_mua_delete, FALSE, FALSE, 0);

  frame21 = gtk_frame_new ("Site settings");
  gtk_widget_ref (frame21);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "frame21", frame21,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (frame21);
  gtk_box_pack_end (GTK_BOX (vbox26), frame21, FALSE, TRUE, 0);
  gtk_container_set_border_width (GTK_CONTAINER (frame21), 5);
  gtk_frame_set_shadow_type (GTK_FRAME (frame21), GTK_SHADOW_OUT);

  table19 = gtk_table_new (4, 3, FALSE);
  gtk_widget_ref (table19);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "table19", table19,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (table19);
  gtk_container_add (GTK_CONTAINER (frame21), table19);
  gtk_container_set_border_width (GTK_CONTAINER (table19), 3);
  gtk_table_set_col_spacings (GTK_TABLE (table19), 5);

  label108 = gtk_label_new ("Delay between two refreshs of the board");
  gtk_widget_ref (label108);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "label108", label108,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (label108);
  gtk_table_attach (GTK_TABLE (table19), label108, 0, 1, 0, 1,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_misc_set_alignment (GTK_MISC (label108), 0, 0.5);

  label109 = gtk_label_new ("Maximum number of messages stored for this board");
  gtk_widget_ref (label109);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "label109", label109,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (label109);
  gtk_table_attach (GTK_TABLE (table19), label109, 0, 1, 1, 2,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_label_set_line_wrap (GTK_LABEL (label109), TRUE);
  gtk_misc_set_alignment (GTK_MISC (label109), 0, 0.5);

  spinbutton_board_delay_adj = gtk_adjustment_new (1, 10, 10000, 1, 10, 10);
  spinbutton_board_delay = gtk_spin_button_new (GTK_ADJUSTMENT (spinbutton_board_delay_adj), 1, 0);
  gtk_widget_ref (spinbutton_board_delay);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "spinbutton_board_delay", spinbutton_board_delay,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (spinbutton_board_delay);
  gtk_table_attach (GTK_TABLE (table19), spinbutton_board_delay, 1, 2, 0, 1,
                    (GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_widget_set_usize (spinbutton_board_delay, 60, -2);
  gtk_spin_button_set_numeric (GTK_SPIN_BUTTON (spinbutton_board_delay), TRUE);
  gtk_spin_button_set_update_policy (GTK_SPIN_BUTTON (spinbutton_board_delay), GTK_UPDATE_IF_VALID);

  label110 = gtk_label_new ("seconds");
  gtk_widget_ref (label110);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "label110", label110,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (label110);
  gtk_table_attach (GTK_TABLE (table19), label110, 2, 3, 0, 1,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_misc_set_alignment (GTK_MISC (label110), 0, 0.5);

  spinbutton_board_max_msg_adj = gtk_adjustment_new (1, 10, 20000, 1, 10, 10);
  spinbutton_board_max_msg = gtk_spin_button_new (GTK_ADJUSTMENT (spinbutton_board_max_msg_adj), 1, 0);
  gtk_widget_ref (spinbutton_board_max_msg);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "spinbutton_board_max_msg", spinbutton_board_max_msg,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (spinbutton_board_max_msg);
  gtk_table_attach (GTK_TABLE (table19), spinbutton_board_max_msg, 1, 2, 1, 2,
                    (GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_spin_button_set_numeric (GTK_SPIN_BUTTON (spinbutton_board_max_msg), TRUE);
  gtk_spin_button_set_update_policy (GTK_SPIN_BUTTON (spinbutton_board_max_msg), GTK_UPDATE_IF_VALID);

  combo1 = gtk_combo_new ();
  gtk_widget_ref (combo1);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "combo1", combo1,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (combo1);
  gtk_table_attach (GTK_TABLE (table19), combo1, 1, 3, 2, 3,
                    (GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  combo1_items = g_list_append (combo1_items, (gpointer) "http://wiki.woof.lu/index.php?");
  gtk_combo_set_popdown_strings (GTK_COMBO (combo1), combo1_items);
  g_list_free (combo1_items);

  combo_entry_board_wiki = GTK_COMBO (combo1)->entry;
  gtk_widget_ref (combo_entry_board_wiki);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "combo_entry_board_wiki", combo_entry_board_wiki,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (combo_entry_board_wiki);
  gtk_tooltips_set_tip (tooltips, combo_entry_board_wiki, "if you want to emulate a proxy, put something like 'http://wiki.woof.lu/index.php?'", NULL);
  gtk_entry_set_text (GTK_ENTRY (combo_entry_board_wiki), "http://wiki.woof.lu/index.php?");

  label111 = gtk_label_new ("WikiUrl emulation");
  gtk_widget_ref (label111);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "label111", label111,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (label111);
  gtk_table_attach (GTK_TABLE (table19), label111, 0, 1, 2, 3,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_misc_set_alignment (GTK_MISC (label111), 0, 0.5);

  checkbutton_pp_use_AMPM = gtk_check_button_new_with_label ("Board uses AM/PM clocks");
  gtk_widget_ref (checkbutton_pp_use_AMPM);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "checkbutton_pp_use_AMPM", checkbutton_pp_use_AMPM,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (checkbutton_pp_use_AMPM);
  gtk_table_attach (GTK_TABLE (table19), checkbutton_pp_use_AMPM, 0, 1, 3, 4,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);

  checkbutton_board_auto_refresh = gtk_check_button_new_with_label ("Auto-refresh the board");
  gtk_widget_ref (checkbutton_board_auto_refresh);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "checkbutton_board_auto_refresh", checkbutton_board_auto_refresh,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (checkbutton_board_auto_refresh);
  gtk_table_attach (GTK_TABLE (table19), checkbutton_board_auto_refresh, 1, 2, 3, 4,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_tooltips_set_tip (tooltips, checkbutton_board_auto_refresh, "Untoggle this to disable the auto-refresh of the board (for example, in order to reduce the number of hits in the company's proxy of the 42 boards you're monitoring)", NULL);

  label6 = gtk_label_new ("Board");
  gtk_widget_ref (label6);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "label6", label6,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (label6);
  gtk_notebook_set_tab_label (GTK_NOTEBOOK (notebook1), gtk_notebook_get_nth_page (GTK_NOTEBOOK (notebook1), 3), label6);

  vbox11 = gtk_vbox_new (FALSE, 0);
  gtk_widget_ref (vbox11);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "vbox11", vbox11,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (vbox11);
  gtk_container_add (GTK_CONTAINER (notebook1), vbox11);

  frame8 = gtk_frame_new ("Default message on startup");
  gtk_widget_ref (frame8);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "frame8", frame8,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (frame8);
  gtk_box_pack_start (GTK_BOX (vbox11), frame8, FALSE, TRUE, 0);
  gtk_container_set_border_width (GTK_CONTAINER (frame8), 5);

  entry_coin_coin_message = gtk_entry_new ();
  gtk_widget_ref (entry_coin_coin_message);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "entry_coin_coin_message", entry_coin_coin_message,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (entry_coin_coin_message);
  gtk_container_add (GTK_CONTAINER (frame8), entry_coin_coin_message);
  gtk_tooltips_set_tip (tooltips, entry_coin_coin_message, "please, use a smart startup message, and listen carefully to what clipouille says.", NULL);

  frame15 = gtk_frame_new ("Spell checking");
  gtk_widget_ref (frame15);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "frame15", frame15,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (frame15);
  gtk_box_pack_start (GTK_BOX (vbox11), frame15, FALSE, TRUE, 0);
  gtk_container_set_border_width (GTK_CONTAINER (frame15), 5);

  vbox22 = gtk_vbox_new (FALSE, 0);
  gtk_widget_ref (vbox22);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "vbox22", vbox22,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (vbox22);
  gtk_container_add (GTK_CONTAINER (frame15), vbox22);

  label92 = gtk_label_new ("Do not enable spell checking if you do not have a working ispell with a working dicionnary, or this will slow down wmcoincoin (and it will complain a lot in the console)");
  gtk_widget_ref (label92);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "label92", label92,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (label92);
  gtk_box_pack_start (GTK_BOX (vbox22), label92, FALSE, FALSE, 0);
  gtk_widget_set_usize (label92, 600, -2);
  gtk_label_set_line_wrap (GTK_LABEL (label92), TRUE);

  hseparator3 = gtk_hseparator_new ();
  gtk_widget_ref (hseparator3);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "hseparator3", hseparator3,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (hseparator3);
  gtk_box_pack_start (GTK_BOX (vbox22), hseparator3, FALSE, TRUE, 0);

  checkbutton_enable_spell = gtk_check_button_new_with_label ("Enable the spell checking");
  gtk_widget_ref (checkbutton_enable_spell);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "checkbutton_enable_spell", checkbutton_enable_spell,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (checkbutton_enable_spell);
  gtk_box_pack_start (GTK_BOX (vbox22), checkbutton_enable_spell, FALSE, FALSE, 0);

  table17 = gtk_table_new (2, 2, FALSE);
  gtk_widget_ref (table17);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "table17", table17,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (table17);
  gtk_box_pack_start (GTK_BOX (vbox22), table17, FALSE, TRUE, 0);
  gtk_table_set_col_spacings (GTK_TABLE (table17), 9);

  label93 = gtk_label_new ("Spell checker (ispell is fine, aspell also but it is slower and uses more memory)");
  gtk_widget_ref (label93);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "label93", label93,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (label93);
  gtk_table_attach (GTK_TABLE (table17), label93, 0, 1, 0, 1,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_label_set_line_wrap (GTK_LABEL (label93), TRUE);
  gtk_misc_set_alignment (GTK_MISC (label93), 1, 0.5);

  label94 = gtk_label_new ("Dictionnary");
  gtk_widget_ref (label94);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "label94", label94,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (label94);
  gtk_table_attach (GTK_TABLE (table17), label94, 0, 1, 1, 2,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_misc_set_alignment (GTK_MISC (label94), 1, 0.5);

  combo6 = gtk_combo_new ();
  gtk_widget_ref (combo6);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "combo6", combo6,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (combo6);
  gtk_table_attach (GTK_TABLE (table17), combo6, 1, 2, 1, 2,
                    (GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  combo6_items = g_list_append (combo6_items, (gpointer) "french");
  combo6_items = g_list_append (combo6_items, (gpointer) "francais");
  combo6_items = g_list_append (combo6_items, (gpointer) "english");
  combo6_items = g_list_append (combo6_items, (gpointer) "british");
  combo6_items = g_list_append (combo6_items, (gpointer) "american");
  gtk_combo_set_popdown_strings (GTK_COMBO (combo6), combo6_items);
  g_list_free (combo6_items);

  entry_spell_dict = GTK_COMBO (combo6)->entry;
  gtk_widget_ref (entry_spell_dict);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "entry_spell_dict", entry_spell_dict,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (entry_spell_dict);
  gtk_entry_set_text (GTK_ENTRY (entry_spell_dict), "frenchentry_spell_cmd");

  combo7 = gtk_combo_new ();
  gtk_widget_ref (combo7);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "combo7", combo7,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (combo7);
  gtk_table_attach (GTK_TABLE (table17), combo7, 1, 2, 0, 1,
                    (GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  combo7_items = g_list_append (combo7_items, (gpointer) "ispell");
  combo7_items = g_list_append (combo7_items, (gpointer) "aspell");
  gtk_combo_set_popdown_strings (GTK_COMBO (combo7), combo7_items);
  g_list_free (combo7_items);

  entry_spell_cmd = GTK_COMBO (combo7)->entry;
  gtk_widget_ref (entry_spell_cmd);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "entry_spell_cmd", entry_spell_cmd,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (entry_spell_cmd);
  gtk_entry_set_text (GTK_ENTRY (entry_spell_cmd), "ispell");

  frame22 = gtk_frame_new ("Site settings");
  gtk_widget_ref (frame22);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "frame22", frame22,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (frame22);
  gtk_box_pack_end (GTK_BOX (vbox11), frame22, FALSE, TRUE, 0);
  gtk_container_set_border_width (GTK_CONTAINER (frame22), 5);
  gtk_frame_set_shadow_type (GTK_FRAME (frame22), GTK_SHADOW_OUT);

  table20 = gtk_table_new (3, 3, FALSE);
  gtk_widget_ref (table20);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "table20", table20,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (table20);
  gtk_container_add (GTK_CONTAINER (frame22), table20);
  gtk_table_set_col_spacings (GTK_TABLE (table20), 8);

  spinbutton_msg_max_length_adj = gtk_adjustment_new (1, 20, 1000, 1, 10, 10);
  spinbutton_msg_max_length = gtk_spin_button_new (GTK_ADJUSTMENT (spinbutton_msg_max_length_adj), 1, 0);
  gtk_widget_ref (spinbutton_msg_max_length);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "spinbutton_msg_max_length", spinbutton_msg_max_length,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (spinbutton_msg_max_length);
  gtk_table_attach (GTK_TABLE (table20), spinbutton_msg_max_length, 1, 2, 0, 1,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_widget_set_usize (spinbutton_msg_max_length, 60, -2);
  gtk_spin_button_set_numeric (GTK_SPIN_BUTTON (spinbutton_msg_max_length), TRUE);
  gtk_spin_button_set_update_policy (GTK_SPIN_BUTTON (spinbutton_msg_max_length), GTK_UPDATE_IF_VALID);

  spinbutton_ua_max_length_adj = gtk_adjustment_new (1, 20, 200, 1, 10, 10);
  spinbutton_ua_max_length = gtk_spin_button_new (GTK_ADJUSTMENT (spinbutton_ua_max_length_adj), 1, 0);
  gtk_widget_ref (spinbutton_ua_max_length);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "spinbutton_ua_max_length", spinbutton_ua_max_length,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (spinbutton_ua_max_length);
  gtk_table_attach (GTK_TABLE (table20), spinbutton_ua_max_length, 1, 2, 1, 2,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_widget_set_usize (spinbutton_ua_max_length, 60, -2);
  gtk_spin_button_set_numeric (GTK_SPIN_BUTTON (spinbutton_ua_max_length), TRUE);
  gtk_spin_button_set_update_policy (GTK_SPIN_BUTTON (spinbutton_ua_max_length), GTK_UPDATE_IF_VALID);

  label116 = gtk_label_new ("(if you set a higher value than the ua truncation length of the remote site, your messages posted anonymously might not be detected as 'yours')");
  gtk_widget_ref (label116);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "label116", label116,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (label116);
  gtk_table_attach (GTK_TABLE (table20), label116, 2, 3, 1, 2,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_widget_set_usize (label116, 407, -2);
  gtk_label_set_line_wrap (GTK_LABEL (label116), TRUE);
  gtk_misc_set_alignment (GTK_MISC (label116), 0, 0.5);

  hbox19 = gtk_hbox_new (FALSE, 0);
  gtk_widget_ref (hbox19);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "hbox19", hbox19,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (hbox19);
  gtk_table_attach (GTK_TABLE (table20), hbox19, 1, 3, 2, 3,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (GTK_FILL), 0, 0);

  entry_default_ua = gtk_entry_new ();
  gtk_widget_ref (entry_default_ua);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "entry_default_ua", entry_default_ua,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (entry_default_ua);
  gtk_box_pack_start (GTK_BOX (hbox19), entry_default_ua, TRUE, TRUE, 0);
  gtk_tooltips_set_tip (tooltips, entry_default_ua, "yes, you can enter whatever stupid string you want, just try, don't be shy", NULL);

  button_reset_ua = gtk_button_new_with_label ("Reset");
  gtk_widget_ref (button_reset_ua);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "button_reset_ua", button_reset_ua,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (button_reset_ua);
  gtk_box_pack_start (GTK_BOX (hbox19), button_reset_ua, FALSE, FALSE, 0);

  label114 = gtk_label_new ("Useragent maximum length (usually 60)");
  gtk_widget_ref (label114);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "label114", label114,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (label114);
  gtk_table_attach (GTK_TABLE (table20), label114, 0, 1, 1, 2,
                    (GtkAttachOptions) (0),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_widget_set_usize (label114, 130, -2);
  gtk_label_set_justify (GTK_LABEL (label114), GTK_JUSTIFY_RIGHT);
  gtk_label_set_line_wrap (GTK_LABEL (label114), TRUE);
  gtk_misc_set_alignment (GTK_MISC (label114), 1, 0.5);

  label115 = gtk_label_new ("Message maximum length (usually 255)");
  gtk_widget_ref (label115);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "label115", label115,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (label115);
  gtk_table_attach (GTK_TABLE (table20), label115, 0, 1, 0, 1,
                    (GtkAttachOptions) (0),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_widget_set_usize (label115, 130, -2);
  gtk_label_set_justify (GTK_LABEL (label115), GTK_JUSTIFY_RIGHT);
  gtk_label_set_line_wrap (GTK_LABEL (label115), TRUE);
  gtk_misc_set_alignment (GTK_MISC (label115), 1, 0.5);

  label113 = gtk_label_new ("Default useragent");
  gtk_widget_ref (label113);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "label113", label113,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (label113);
  gtk_table_attach (GTK_TABLE (table20), label113, 0, 1, 2, 3,
                    (GtkAttachOptions) (0),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_widget_set_usize (label113, 130, -2);
  gtk_label_set_justify (GTK_LABEL (label113), GTK_JUSTIFY_LEFT);
  gtk_label_set_line_wrap (GTK_LABEL (label113), TRUE);
  gtk_misc_set_alignment (GTK_MISC (label113), 1, 0.5);

  label7 = gtk_label_new ("Palmipede");
  gtk_widget_ref (label7);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "label7", label7,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (label7);
  gtk_notebook_set_tab_label (GTK_NOTEBOOK (notebook1), gtk_notebook_get_nth_page (GTK_NOTEBOOK (notebook1), 4), label7);

  vbox_pinni = gtk_vbox_new (FALSE, 0);
  gtk_widget_ref (vbox_pinni);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "vbox_pinni", vbox_pinni,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (vbox_pinni);
  gtk_container_add (GTK_CONTAINER (notebook1), vbox_pinni);

  frame24 = gtk_frame_new ("Global settings");
  gtk_widget_ref (frame24);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "frame24", frame24,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (frame24);
  gtk_box_pack_start (GTK_BOX (vbox_pinni), frame24, FALSE, TRUE, 0);
  gtk_container_set_border_width (GTK_CONTAINER (frame24), 5);
  gtk_frame_set_shadow_type (GTK_FRAME (frame24), GTK_SHADOW_OUT);

  vbox29 = gtk_vbox_new (FALSE, 0);
  gtk_widget_ref (vbox29);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "vbox29", vbox29,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (vbox29);
  gtk_container_add (GTK_CONTAINER (frame24), vbox29);

  hbox161 = gtk_hbox_new (FALSE, 0);
  gtk_widget_ref (hbox161);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "hbox161", hbox161,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (hbox161);
  gtk_box_pack_start (GTK_BOX (vbox29), hbox161, FALSE, FALSE, 0);

  checkbutton_pp_auto_open = gtk_check_button_new_with_label ("Auto-open the pinnipede on startup");
  gtk_widget_ref (checkbutton_pp_auto_open);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "checkbutton_pp_auto_open", checkbutton_pp_auto_open,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (checkbutton_pp_auto_open);
  gtk_box_pack_start (GTK_BOX (hbox161), checkbutton_pp_auto_open, FALSE, FALSE, 0);

  checkbutton_pp_use_classical_tabs = gtk_check_button_new_with_label ("Use 'classical' tabs behaviour");
  gtk_widget_ref (checkbutton_pp_use_classical_tabs);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "checkbutton_pp_use_classical_tabs", checkbutton_pp_use_classical_tabs,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (checkbutton_pp_use_classical_tabs);
  gtk_box_pack_start (GTK_BOX (hbox161), checkbutton_pp_use_classical_tabs, FALSE, FALSE, 0);

  checkbutton_pp_hungry_boitakon = gtk_check_button_new_with_label ("Very Hungry boitakon");
  gtk_widget_ref (checkbutton_pp_hungry_boitakon);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "checkbutton_pp_hungry_boitakon", checkbutton_pp_hungry_boitakon,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (checkbutton_pp_hungry_boitakon);
  gtk_box_pack_start (GTK_BOX (hbox161), checkbutton_pp_hungry_boitakon, FALSE, FALSE, 0);
  gtk_tooltips_set_tip (tooltips, checkbutton_pp_hungry_boitakon, "If a message replies to a message in the hungry boitakon and another one which is not in the boitakon, it will be put in the boitakon only if this option is toggled", NULL);

  frame9 = gtk_frame_new ("Transparency mode");
  gtk_widget_ref (frame9);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "frame9", frame9,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (frame9);
  gtk_box_pack_start (GTK_BOX (vbox29), frame9, TRUE, TRUE, 0);
  gtk_container_set_border_width (GTK_CONTAINER (frame9), 5);

  vbox13 = gtk_vbox_new (FALSE, 0);
  gtk_widget_ref (vbox13);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "vbox13", vbox13,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (vbox13);
  gtk_container_add (GTK_CONTAINER (frame9), vbox13);

  hbox11 = gtk_hbox_new (FALSE, 0);
  gtk_widget_ref (hbox11);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "hbox11", hbox11,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (hbox11);
  gtk_box_pack_start (GTK_BOX (vbox13), hbox11, FALSE, FALSE, 0);

  checkbutton_start_in_transparency = gtk_check_button_new_with_label ("Start in transparency mode");
  gtk_widget_ref (checkbutton_start_in_transparency);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "checkbutton_start_in_transparency", checkbutton_start_in_transparency,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (checkbutton_start_in_transparency);
  gtk_box_pack_start (GTK_BOX (hbox11), checkbutton_start_in_transparency, FALSE, FALSE, 0);

  checkbutton_use_fake_real_transparency = gtk_check_button_new_with_label ("Use fake real tranparency");
  gtk_widget_ref (checkbutton_use_fake_real_transparency);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "checkbutton_use_fake_real_transparency", checkbutton_use_fake_real_transparency,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (checkbutton_use_fake_real_transparency);
  gtk_box_pack_start (GTK_BOX (hbox11), checkbutton_use_fake_real_transparency, FALSE, FALSE, 0);
  gtk_tooltips_set_tip (tooltips, checkbutton_use_fake_real_transparency, "this is a strupid option, suitable for stupid screenshots", NULL);

  hseparator1 = gtk_hseparator_new ();
  gtk_widget_ref (hseparator1);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "hseparator1", hseparator1,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (hseparator1);
  gtk_box_pack_start (GTK_BOX (vbox13), hseparator1, TRUE, TRUE, 0);

  hbox8 = gtk_hbox_new (FALSE, 0);
  gtk_widget_ref (hbox8);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "hbox8", hbox8,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (hbox8);
  gtk_box_pack_start (GTK_BOX (vbox13), hbox8, TRUE, TRUE, 0);

  vbox14 = gtk_vbox_new (FALSE, 0);
  gtk_widget_ref (vbox14);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "vbox14", vbox14,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (vbox14);
  gtk_box_pack_start (GTK_BOX (hbox8), vbox14, FALSE, FALSE, 0);
  gtk_container_set_border_width (GTK_CONTAINER (vbox14), 10);

  label43 = gtk_label_new ("Tranparency type");
  gtk_widget_ref (label43);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "label43", label43,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (label43);
  gtk_box_pack_start (GTK_BOX (vbox14), label43, FALSE, FALSE, 0);

  optionmenu_transp = gtk_option_menu_new ();
  gtk_widget_ref (optionmenu_transp);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "optionmenu_transp", optionmenu_transp,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (optionmenu_transp);
  gtk_box_pack_start (GTK_BOX (vbox14), optionmenu_transp, FALSE, FALSE, 0);
  optionmenu_transp_menu = gtk_menu_new ();
  glade_menuitem = gtk_menu_item_new_with_label ("FULL");
  gtk_widget_show (glade_menuitem);
  gtk_menu_append (GTK_MENU (optionmenu_transp_menu), glade_menuitem);
  glade_menuitem = gtk_menu_item_new_with_label ("SHADE");
  gtk_widget_show (glade_menuitem);
  gtk_menu_append (GTK_MENU (optionmenu_transp_menu), glade_menuitem);
  glade_menuitem = gtk_menu_item_new_with_label ("TINT");
  gtk_widget_show (glade_menuitem);
  gtk_menu_append (GTK_MENU (optionmenu_transp_menu), glade_menuitem);
  gtk_option_menu_set_menu (GTK_OPTION_MENU (optionmenu_transp), optionmenu_transp_menu);

  vseparator2 = gtk_vseparator_new ();
  gtk_widget_ref (vseparator2);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "vseparator2", vseparator2,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (vseparator2);
  gtk_box_pack_start (GTK_BOX (hbox8), vseparator2, FALSE, TRUE, 0);

  table6 = gtk_table_new (2, 4, FALSE);
  gtk_widget_ref (table6);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "table6", table6,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (table6);
  gtk_box_pack_start (GTK_BOX (hbox8), table6, TRUE, TRUE, 8);
  gtk_container_set_border_width (GTK_CONTAINER (table6), 10);
  gtk_table_set_row_spacings (GTK_TABLE (table6), 5);
  gtk_table_set_col_spacings (GTK_TABLE (table6), 12);

  label_black_tint = gtk_label_new ("Black tint ");
  gtk_widget_ref (label_black_tint);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "label_black_tint", label_black_tint,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (label_black_tint);
  gtk_table_attach (GTK_TABLE (table6), label_black_tint, 0, 1, 1, 2,
                    (GtkAttachOptions) (0),
                    (GtkAttachOptions) (0), 16, 0);
  gtk_label_set_justify (GTK_LABEL (label_black_tint), GTK_JUSTIFY_RIGHT);
  gtk_misc_set_alignment (GTK_MISC (label_black_tint), 0, 0.5);

  label_shade_min = gtk_label_new ("Shade min (%)");
  gtk_widget_ref (label_shade_min);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "label_shade_min", label_shade_min,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (label_shade_min);
  gtk_table_attach (GTK_TABLE (table6), label_shade_min, 0, 1, 0, 1,
                    (GtkAttachOptions) (0),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_label_set_justify (GTK_LABEL (label_shade_min), GTK_JUSTIFY_RIGHT);
  gtk_misc_set_alignment (GTK_MISC (label_shade_min), 0, 0.5);

  label_shade_max = gtk_label_new ("Shade max (%)");
  gtk_widget_ref (label_shade_max);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "label_shade_max", label_shade_max,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (label_shade_max);
  gtk_table_attach (GTK_TABLE (table6), label_shade_max, 2, 3, 0, 1,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_misc_set_alignment (GTK_MISC (label_shade_max), 0, 0.5);

  label_white_tint = gtk_label_new ("White tint");
  gtk_widget_ref (label_white_tint);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "label_white_tint", label_white_tint,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (label_white_tint);
  gtk_table_attach (GTK_TABLE (table6), label_white_tint, 2, 3, 1, 2,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_misc_set_alignment (GTK_MISC (label_white_tint), 0, 0.5);

  bt_transp_black = gtk_button_new ();
  gtk_widget_ref (bt_transp_black);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "bt_transp_black", bt_transp_black,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (bt_transp_black);
  gtk_table_attach (GTK_TABLE (table6), bt_transp_black, 1, 2, 1, 2,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_widget_set_usize (bt_transp_black, 50, 20);
  gtk_tooltips_set_tip (tooltips, bt_transp_black, "Color in normal (non transparent) mode", NULL);

  bt_transp_white = gtk_button_new ();
  gtk_widget_ref (bt_transp_white);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "bt_transp_white", bt_transp_white,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (bt_transp_white);
  gtk_table_attach (GTK_TABLE (table6), bt_transp_white, 3, 4, 1, 2,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_widget_set_usize (bt_transp_white, 50, 20);
  gtk_tooltips_set_tip (tooltips, bt_transp_white, "Color in normal (non transparent) mode", NULL);

  spinbutton_transp_shade_max_adj = gtk_adjustment_new (1, 0, 100, 1, 10, 10);
  spinbutton_transp_shade_max = gtk_spin_button_new (GTK_ADJUSTMENT (spinbutton_transp_shade_max_adj), 1, 0);
  gtk_widget_ref (spinbutton_transp_shade_max);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "spinbutton_transp_shade_max", spinbutton_transp_shade_max,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (spinbutton_transp_shade_max);
  gtk_table_attach (GTK_TABLE (table6), spinbutton_transp_shade_max, 3, 4, 0, 1,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);

  spinbutton_transp_shade_min_adj = gtk_adjustment_new (1, 0, 100, 1, 10, 10);
  spinbutton_transp_shade_min = gtk_spin_button_new (GTK_ADJUSTMENT (spinbutton_transp_shade_min_adj), 1, 0);
  gtk_widget_ref (spinbutton_transp_shade_min);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "spinbutton_transp_shade_min", spinbutton_transp_shade_min,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (spinbutton_transp_shade_min);
  gtk_table_attach (GTK_TABLE (table6), spinbutton_transp_shade_min, 1, 2, 0, 1,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);

  frame26 = gtk_frame_new ("Pinnipede font");
  gtk_widget_ref (frame26);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "frame26", frame26,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (frame26);
  gtk_box_pack_start (GTK_BOX (vbox29), frame26, TRUE, TRUE, 0);
  gtk_container_set_border_width (GTK_CONTAINER (frame26), 5);

  hbox20 = gtk_hbox_new (FALSE, 5);
  gtk_widget_ref (hbox20);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "hbox20", hbox20,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (hbox20);
  gtk_container_add (GTK_CONTAINER (frame26), hbox20);
  gtk_container_set_border_width (GTK_CONTAINER (hbox20), 3);

  label118 = gtk_label_new ("Font family (helvetica, verdana,..)");
  gtk_widget_ref (label118);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "label118", label118,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (label118);
  gtk_box_pack_start (GTK_BOX (hbox20), label118, FALSE, FALSE, 0);

  combo_pp_fn_family = gtk_combo_new ();
  gtk_widget_ref (combo_pp_fn_family);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "combo_pp_fn_family", combo_pp_fn_family,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (combo_pp_fn_family);
  gtk_box_pack_start (GTK_BOX (hbox20), combo_pp_fn_family, TRUE, TRUE, 0);
  combo_pp_fn_family_items = g_list_append (combo_pp_fn_family_items, (gpointer) "helvetica");
  combo_pp_fn_family_items = g_list_append (combo_pp_fn_family_items, (gpointer) "verdana");
  combo_pp_fn_family_items = g_list_append (combo_pp_fn_family_items, (gpointer) "fixed");
  combo_pp_fn_family_items = g_list_append (combo_pp_fn_family_items, (gpointer) "courier");
  combo_pp_fn_family_items = g_list_append (combo_pp_fn_family_items, (gpointer) "arial");
  gtk_combo_set_popdown_strings (GTK_COMBO (combo_pp_fn_family), combo_pp_fn_family_items);
  g_list_free (combo_pp_fn_family_items);

  comboentry_pp_fnfamily = GTK_COMBO (combo_pp_fn_family)->entry;
  gtk_widget_ref (comboentry_pp_fnfamily);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "comboentry_pp_fnfamily", comboentry_pp_fnfamily,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (comboentry_pp_fnfamily);
  gtk_entry_set_text (GTK_ENTRY (comboentry_pp_fnfamily), "helvetica");

  label119 = gtk_label_new ("Font size");
  gtk_widget_ref (label119);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "label119", label119,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (label119);
  gtk_box_pack_start (GTK_BOX (hbox20), label119, FALSE, FALSE, 0);

  spinbutton_pp_fnsize_adj = gtk_adjustment_new (1, 4, 100, 1, 10, 10);
  spinbutton_pp_fnsize = gtk_spin_button_new (GTK_ADJUSTMENT (spinbutton_pp_fnsize_adj), 1, 0);
  gtk_widget_ref (spinbutton_pp_fnsize);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "spinbutton_pp_fnsize", spinbutton_pp_fnsize,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (spinbutton_pp_fnsize);
  gtk_box_pack_start (GTK_BOX (hbox20), spinbutton_pp_fnsize, FALSE, TRUE, 0);
  gtk_widget_set_usize (spinbutton_pp_fnsize, 60, -2);
  gtk_spin_button_set_numeric (GTK_SPIN_BUTTON (spinbutton_pp_fnsize), TRUE);
  gtk_spin_button_set_update_policy (GTK_SPIN_BUTTON (spinbutton_pp_fnsize), GTK_UPDATE_IF_VALID);

  frame10 = gtk_frame_new ("Pinnipede general colors");
  gtk_widget_ref (frame10);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "frame10", frame10,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (frame10);
  gtk_box_pack_start (GTK_BOX (vbox29), frame10, TRUE, TRUE, 0);
  gtk_container_set_border_width (GTK_CONTAINER (frame10), 5);

  vbox15 = gtk_vbox_new (FALSE, 0);
  gtk_widget_ref (vbox15);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "vbox15", vbox15,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (vbox15);
  gtk_container_add (GTK_CONTAINER (frame10), vbox15);

  hbox9 = gtk_hbox_new (FALSE, 0);
  gtk_widget_ref (hbox9);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "hbox9", hbox9,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (hbox9);
  gtk_box_pack_start (GTK_BOX (vbox15), hbox9, TRUE, TRUE, 0);

  table7 = gtk_table_new (2, 3, FALSE);
  gtk_widget_ref (table7);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "table7", table7,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (table7);
  gtk_box_pack_start (GTK_BOX (hbox9), table7, TRUE, TRUE, 0);
  gtk_container_set_border_width (GTK_CONTAINER (table7), 5);

  label46 = gtk_label_new ("Emphasized text background");
  gtk_widget_ref (label46);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "label46", label46,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (label46);
  gtk_table_attach (GTK_TABLE (table7), label46, 0, 1, 0, 1,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_misc_set_alignment (GTK_MISC (label46), 0, 0.5);

  bt_pp_emph_color = gtk_button_new ();
  gtk_widget_ref (bt_pp_emph_color);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "bt_pp_emph_color", bt_pp_emph_color,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (bt_pp_emph_color);
  gtk_table_attach (GTK_TABLE (table7), bt_pp_emph_color, 1, 2, 0, 1,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_widget_set_usize (bt_pp_emph_color, 50, 20);
  gtk_tooltips_set_tip (tooltips, bt_pp_emph_color, "Color in normal (non transparent) mode", NULL);

  label47 = gtk_label_new ("Selected text bakcground color");
  gtk_widget_ref (label47);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "label47", label47,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (label47);
  gtk_table_attach (GTK_TABLE (table7), label47, 0, 1, 1, 2,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_misc_set_alignment (GTK_MISC (label47), 0, 0.5);

  bt_pp_sel_color = gtk_button_new ();
  gtk_widget_ref (bt_pp_sel_color);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "bt_pp_sel_color", bt_pp_sel_color,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (bt_pp_sel_color);
  gtk_table_attach (GTK_TABLE (table7), bt_pp_sel_color, 1, 2, 1, 2,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_widget_set_usize (bt_pp_sel_color, 50, 20);
  gtk_tooltips_set_tip (tooltips, bt_pp_sel_color, "Color in normal (non transparent) mode", NULL);

  bt_pp_emph_color_trans = gtk_button_new ();
  gtk_widget_ref (bt_pp_emph_color_trans);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "bt_pp_emph_color_trans", bt_pp_emph_color_trans,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (bt_pp_emph_color_trans);
  gtk_table_attach (GTK_TABLE (table7), bt_pp_emph_color_trans, 2, 3, 0, 1,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_widget_set_usize (bt_pp_emph_color_trans, 50, 20);
  gtk_tooltips_set_tip (tooltips, bt_pp_emph_color_trans, "Color in transparent mode", NULL);

  bt_pp_sel_color_trans = gtk_button_new ();
  gtk_widget_ref (bt_pp_sel_color_trans);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "bt_pp_sel_color_trans", bt_pp_sel_color_trans,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (bt_pp_sel_color_trans);
  gtk_table_attach (GTK_TABLE (table7), bt_pp_sel_color_trans, 2, 3, 1, 2,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_widget_set_usize (bt_pp_sel_color_trans, 50, 20);
  gtk_tooltips_set_tip (tooltips, bt_pp_sel_color_trans, "Color in transparent mode", NULL);

  table8 = gtk_table_new (2, 3, FALSE);
  gtk_widget_ref (table8);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "table8", table8,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (table8);
  gtk_box_pack_start (GTK_BOX (hbox9), table8, TRUE, TRUE, 0);
  gtk_container_set_border_width (GTK_CONTAINER (table8), 5);

  bt_pp_popup_fgcolor = gtk_button_new ();
  gtk_widget_ref (bt_pp_popup_fgcolor);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "bt_pp_popup_fgcolor", bt_pp_popup_fgcolor,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (bt_pp_popup_fgcolor);
  gtk_table_attach (GTK_TABLE (table8), bt_pp_popup_fgcolor, 1, 2, 1, 2,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_widget_set_usize (bt_pp_popup_fgcolor, 50, 20);
  gtk_tooltips_set_tip (tooltips, bt_pp_popup_fgcolor, "Color in normal (non transparent) mode", NULL);

  label49 = gtk_label_new ("Popup text background color");
  gtk_widget_ref (label49);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "label49", label49,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (label49);
  gtk_table_attach (GTK_TABLE (table8), label49, 0, 1, 0, 1,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_label_set_justify (GTK_LABEL (label49), GTK_JUSTIFY_RIGHT);
  gtk_misc_set_alignment (GTK_MISC (label49), 0, 0.5);

  bt_pp_popup_bgcolor = gtk_button_new ();
  gtk_widget_ref (bt_pp_popup_bgcolor);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "bt_pp_popup_bgcolor", bt_pp_popup_bgcolor,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (bt_pp_popup_bgcolor);
  gtk_table_attach (GTK_TABLE (table8), bt_pp_popup_bgcolor, 1, 2, 0, 1,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_widget_set_usize (bt_pp_popup_bgcolor, 50, 20);
  gtk_tooltips_set_tip (tooltips, bt_pp_popup_bgcolor, "Color in normal (non transparent) mode", NULL);

  label50 = gtk_label_new ("Popup text foreground color");
  gtk_widget_ref (label50);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "label50", label50,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (label50);
  gtk_table_attach (GTK_TABLE (table8), label50, 0, 1, 1, 2,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_label_set_justify (GTK_LABEL (label50), GTK_JUSTIFY_RIGHT);
  gtk_misc_set_alignment (GTK_MISC (label50), 0, 0.5);

  bt_pp_popup_bgcolor_trans = gtk_button_new ();
  gtk_widget_ref (bt_pp_popup_bgcolor_trans);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "bt_pp_popup_bgcolor_trans", bt_pp_popup_bgcolor_trans,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (bt_pp_popup_bgcolor_trans);
  gtk_table_attach (GTK_TABLE (table8), bt_pp_popup_bgcolor_trans, 2, 3, 0, 1,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_widget_set_usize (bt_pp_popup_bgcolor_trans, 50, 20);
  gtk_tooltips_set_tip (tooltips, bt_pp_popup_bgcolor_trans, "Color in transparent mode", NULL);

  bt_pp_popup_fgcolor_trans = gtk_button_new ();
  gtk_widget_ref (bt_pp_popup_fgcolor_trans);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "bt_pp_popup_fgcolor_trans", bt_pp_popup_fgcolor_trans,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (bt_pp_popup_fgcolor_trans);
  gtk_table_attach (GTK_TABLE (table8), bt_pp_popup_fgcolor_trans, 2, 3, 1, 2,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_widget_set_usize (bt_pp_popup_fgcolor_trans, 50, 20);
  gtk_tooltips_set_tip (tooltips, bt_pp_popup_fgcolor_trans, "Color in transparent mode", NULL);

  frame34 = gtk_frame_new ("Various toggles");
  gtk_widget_ref (frame34);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "frame34", frame34,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (frame34);
  gtk_box_pack_start (GTK_BOX (vbox29), frame34, TRUE, TRUE, 0);
  gtk_container_set_border_width (GTK_CONTAINER (frame34), 5);

  table28 = gtk_table_new (2, 4, FALSE);
  gtk_widget_ref (table28);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "table28", table28,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (table28);
  gtk_container_add (GTK_CONTAINER (frame34), table28);
  gtk_table_set_col_spacings (GTK_TABLE (table28), 5);

  checkbutton_show_seconds = gtk_check_button_new_with_label ("Always show seconds");
  gtk_widget_ref (checkbutton_show_seconds);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "checkbutton_show_seconds", checkbutton_show_seconds,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (checkbutton_show_seconds);
  gtk_table_attach (GTK_TABLE (table28), checkbutton_show_seconds, 3, 4, 0, 1,
                    (GtkAttachOptions) (0),
                    (GtkAttachOptions) (0), 0, 0);

  checkbutton_show_troll_score = gtk_check_button_new_with_label ("Show troll score on startup");
  gtk_widget_ref (checkbutton_show_troll_score);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "checkbutton_show_troll_score", checkbutton_show_troll_score,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (checkbutton_show_troll_score);
  gtk_table_attach (GTK_TABLE (table28), checkbutton_show_troll_score, 1, 2, 0, 1,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);

  checkbutton_show_fortune = gtk_check_button_new_with_label ("Show fortune on startup");
  gtk_widget_ref (checkbutton_show_fortune);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "checkbutton_show_fortune", checkbutton_show_fortune,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (checkbutton_show_fortune);
  gtk_table_attach (GTK_TABLE (table28), checkbutton_show_fortune, 2, 3, 0, 1,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);

  checkbutton_show_tags = gtk_check_button_new_with_label ("Interpret html tags");
  gtk_widget_ref (checkbutton_show_tags);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "checkbutton_show_tags", checkbutton_show_tags,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (checkbutton_show_tags);
  gtk_table_attach (GTK_TABLE (table28), checkbutton_show_tags, 3, 4, 1, 2,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);

  optionmenu_nick_mode = gtk_option_menu_new ();
  gtk_widget_ref (optionmenu_nick_mode);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "optionmenu_nick_mode", optionmenu_nick_mode,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (optionmenu_nick_mode);
  gtk_table_attach (GTK_TABLE (table28), optionmenu_nick_mode, 2, 3, 1, 2,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  optionmenu_nick_mode_menu = gtk_menu_new ();
  glade_menuitem = gtk_menu_item_new_with_label ("NO LOGIN, NO UA");
  gtk_widget_show (glade_menuitem);
  gtk_menu_append (GTK_MENU (optionmenu_nick_mode_menu), glade_menuitem);
  glade_menuitem = gtk_menu_item_new_with_label ("USERAGENT");
  gtk_widget_show (glade_menuitem);
  gtk_menu_append (GTK_MENU (optionmenu_nick_mode_menu), glade_menuitem);
  glade_menuitem = gtk_menu_item_new_with_label ("LOGIN");
  gtk_widget_show (glade_menuitem);
  gtk_menu_append (GTK_MENU (optionmenu_nick_mode_menu), glade_menuitem);
  glade_menuitem = gtk_menu_item_new_with_label ("LOGIN AND USERAGENT");
  gtk_widget_show (glade_menuitem);
  gtk_menu_append (GTK_MENU (optionmenu_nick_mode_menu), glade_menuitem);
  glade_menuitem = gtk_menu_item_new_with_label ("LOGIN OR USERAGENT");
  gtk_widget_show (glade_menuitem);
  gtk_menu_append (GTK_MENU (optionmenu_nick_mode_menu), glade_menuitem);
  gtk_option_menu_set_menu (GTK_OPTION_MENU (optionmenu_nick_mode), optionmenu_nick_mode_menu);

  label172 = gtk_label_new ("startup nick mode:");
  gtk_widget_ref (label172);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "label172", label172,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (label172);
  gtk_table_attach (GTK_TABLE (table28), label172, 1, 2, 1, 2,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_label_set_justify (GTK_LABEL (label172), GTK_JUSTIFY_LEFT);
  gtk_misc_set_alignment (GTK_MISC (label172), 1, 0.5);

  frame25 = gtk_frame_new ("Site settings");
  gtk_widget_ref (frame25);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "frame25", frame25,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (frame25);
  gtk_box_pack_end (GTK_BOX (vbox_pinni), frame25, FALSE, FALSE, 0);
  gtk_container_set_border_width (GTK_CONTAINER (frame25), 5);
  gtk_frame_set_shadow_type (GTK_FRAME (frame25), GTK_SHADOW_OUT);

  vbox30 = gtk_vbox_new (FALSE, 0);
  gtk_widget_ref (vbox30);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "vbox30", vbox30,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (vbox30);
  gtk_container_add (GTK_CONTAINER (frame25), vbox30);

  checkbutton_mark_id_gaps = gtk_check_button_new_with_label ("View id gaps (red dotted line)");
  gtk_widget_ref (checkbutton_mark_id_gaps);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "checkbutton_mark_id_gaps", checkbutton_mark_id_gaps,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (checkbutton_mark_id_gaps);
  gtk_box_pack_start (GTK_BOX (vbox30), checkbutton_mark_id_gaps, FALSE, FALSE, 0);

  frame27 = gtk_frame_new ("Site specific colors");
  gtk_widget_ref (frame27);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "frame27", frame27,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (frame27);
  gtk_box_pack_start (GTK_BOX (vbox30), frame27, TRUE, TRUE, 0);
  gtk_container_set_border_width (GTK_CONTAINER (frame27), 5);

  hbox21 = gtk_hbox_new (FALSE, 0);
  gtk_widget_ref (hbox21);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "hbox21", hbox21,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (hbox21);
  gtk_container_add (GTK_CONTAINER (frame27), hbox21);

  table21 = gtk_table_new (3, 3, FALSE);
  gtk_widget_ref (table21);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "table21", table21,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (table21);
  gtk_box_pack_start (GTK_BOX (hbox21), table21, TRUE, TRUE, 0);

  label120 = gtk_label_new ("Text background");
  gtk_widget_ref (label120);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "label120", label120,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (label120);
  gtk_table_attach (GTK_TABLE (table21), label120, 0, 1, 0, 1,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_misc_set_alignment (GTK_MISC (label120), 0, 0.5);

  label121 = gtk_label_new ("Text foreground");
  gtk_widget_ref (label121);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "label121", label121,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (label121);
  gtk_table_attach (GTK_TABLE (table21), label121, 0, 1, 1, 2,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_misc_set_alignment (GTK_MISC (label121), 0, 0.5);

  label122 = gtk_label_new ("Clocks");
  gtk_widget_ref (label122);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "label122", label122,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (label122);
  gtk_table_attach (GTK_TABLE (table21), label122, 0, 1, 2, 3,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_misc_set_alignment (GTK_MISC (label122), 0, 0.5);

  bt_pp_bgcolor = gtk_button_new ();
  gtk_widget_ref (bt_pp_bgcolor);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "bt_pp_bgcolor", bt_pp_bgcolor,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (bt_pp_bgcolor);
  gtk_table_attach (GTK_TABLE (table21), bt_pp_bgcolor, 1, 2, 0, 1,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_widget_set_usize (bt_pp_bgcolor, 50, 20);
  gtk_tooltips_set_tip (tooltips, bt_pp_bgcolor, "Color in normal (non transparent) mode", NULL);

  bt_pp_fgcolor = gtk_button_new ();
  gtk_widget_ref (bt_pp_fgcolor);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "bt_pp_fgcolor", bt_pp_fgcolor,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (bt_pp_fgcolor);
  gtk_table_attach (GTK_TABLE (table21), bt_pp_fgcolor, 1, 2, 1, 2,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_widget_set_usize (bt_pp_fgcolor, 50, 20);
  gtk_tooltips_set_tip (tooltips, bt_pp_fgcolor, "Color in normal (non transparent) mode", NULL);

  bt_pp_fgcolor_trans = gtk_button_new ();
  gtk_widget_ref (bt_pp_fgcolor_trans);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "bt_pp_fgcolor_trans", bt_pp_fgcolor_trans,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (bt_pp_fgcolor_trans);
  gtk_table_attach (GTK_TABLE (table21), bt_pp_fgcolor_trans, 2, 3, 1, 2,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_widget_set_usize (bt_pp_fgcolor_trans, 50, 20);
  gtk_tooltips_set_tip (tooltips, bt_pp_fgcolor_trans, "Color in transparent mode", NULL);

  bt_pp_clock_color = gtk_button_new ();
  gtk_widget_ref (bt_pp_clock_color);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "bt_pp_clock_color", bt_pp_clock_color,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (bt_pp_clock_color);
  gtk_table_attach (GTK_TABLE (table21), bt_pp_clock_color, 1, 2, 2, 3,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_widget_set_usize (bt_pp_clock_color, 50, 20);
  gtk_tooltips_set_tip (tooltips, bt_pp_clock_color, "Color in normal (non transparent) mode", NULL);

  bt_pp_clock_color_trans = gtk_button_new ();
  gtk_widget_ref (bt_pp_clock_color_trans);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "bt_pp_clock_color_trans", bt_pp_clock_color_trans,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (bt_pp_clock_color_trans);
  gtk_table_attach (GTK_TABLE (table21), bt_pp_clock_color_trans, 2, 3, 2, 3,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_widget_set_usize (bt_pp_clock_color_trans, 50, 20);
  gtk_tooltips_set_tip (tooltips, bt_pp_clock_color_trans, "Color in transparent mode", NULL);

  table22 = gtk_table_new (3, 3, FALSE);
  gtk_widget_ref (table22);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "table22", table22,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (table22);
  gtk_box_pack_start (GTK_BOX (hbox21), table22, TRUE, TRUE, 0);

  label123 = gtk_label_new ("Login");
  gtk_widget_ref (label123);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "label123", label123,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (label123);
  gtk_table_attach (GTK_TABLE (table22), label123, 0, 1, 0, 1,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_misc_set_alignment (GTK_MISC (label123), 0, 0.5);

  label124 = gtk_label_new ("Useragent");
  gtk_widget_ref (label124);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "label124", label124,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (label124);
  gtk_table_attach (GTK_TABLE (table22), label124, 0, 1, 1, 2,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_misc_set_alignment (GTK_MISC (label124), 0, 0.5);

  label125 = gtk_label_new ("Troll score");
  gtk_widget_ref (label125);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "label125", label125,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (label125);
  gtk_table_attach (GTK_TABLE (table22), label125, 0, 1, 2, 3,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_misc_set_alignment (GTK_MISC (label125), 0, 0.5);

  bt_pp_login_color_trans = gtk_button_new ();
  gtk_widget_ref (bt_pp_login_color_trans);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "bt_pp_login_color_trans", bt_pp_login_color_trans,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (bt_pp_login_color_trans);
  gtk_table_attach (GTK_TABLE (table22), bt_pp_login_color_trans, 2, 3, 0, 1,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_widget_set_usize (bt_pp_login_color_trans, 50, 20);
  gtk_tooltips_set_tip (tooltips, bt_pp_login_color_trans, "Color in transparent mode", NULL);

  bt_pp_ua_color = gtk_button_new ();
  gtk_widget_ref (bt_pp_ua_color);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "bt_pp_ua_color", bt_pp_ua_color,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (bt_pp_ua_color);
  gtk_table_attach (GTK_TABLE (table22), bt_pp_ua_color, 1, 2, 1, 2,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_widget_set_usize (bt_pp_ua_color, 50, 20);
  gtk_tooltips_set_tip (tooltips, bt_pp_ua_color, "Color in normal (non transparent) mode", NULL);

  bt_pp_ua_color_trans = gtk_button_new ();
  gtk_widget_ref (bt_pp_ua_color_trans);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "bt_pp_ua_color_trans", bt_pp_ua_color_trans,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (bt_pp_ua_color_trans);
  gtk_table_attach (GTK_TABLE (table22), bt_pp_ua_color_trans, 2, 3, 1, 2,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_widget_set_usize (bt_pp_ua_color_trans, 50, 20);
  gtk_tooltips_set_tip (tooltips, bt_pp_ua_color_trans, "Color in transparent mode", NULL);

  bt_pp_trollscore_color = gtk_button_new ();
  gtk_widget_ref (bt_pp_trollscore_color);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "bt_pp_trollscore_color", bt_pp_trollscore_color,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (bt_pp_trollscore_color);
  gtk_table_attach (GTK_TABLE (table22), bt_pp_trollscore_color, 1, 2, 2, 3,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_widget_set_usize (bt_pp_trollscore_color, 50, 20);
  gtk_tooltips_set_tip (tooltips, bt_pp_trollscore_color, "Color in normal (non transparent) mode", NULL);

  bt_pp_trollscore_color_trans = gtk_button_new ();
  gtk_widget_ref (bt_pp_trollscore_color_trans);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "bt_pp_trollscore_color_trans", bt_pp_trollscore_color_trans,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (bt_pp_trollscore_color_trans);
  gtk_table_attach (GTK_TABLE (table22), bt_pp_trollscore_color_trans, 2, 3, 2, 3,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_widget_set_usize (bt_pp_trollscore_color_trans, 50, 20);
  gtk_tooltips_set_tip (tooltips, bt_pp_trollscore_color_trans, "Color in transparent mode", NULL);

  bt_pp_login_color = gtk_button_new ();
  gtk_widget_ref (bt_pp_login_color);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "bt_pp_login_color", bt_pp_login_color,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (bt_pp_login_color);
  gtk_table_attach (GTK_TABLE (table22), bt_pp_login_color, 1, 2, 0, 1,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_widget_set_usize (bt_pp_login_color, 50, 20);
  gtk_tooltips_set_tip (tooltips, bt_pp_login_color, "Color in normal (non transparent) mode", NULL);

  table23 = gtk_table_new (3, 3, FALSE);
  gtk_widget_ref (table23);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "table23", table23,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (table23);
  gtk_box_pack_start (GTK_BOX (hbox21), table23, TRUE, TRUE, 0);

  label126 = gtk_label_new ("URLs");
  gtk_widget_ref (label126);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "label126", label126,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (label126);
  gtk_table_attach (GTK_TABLE (table23), label126, 0, 1, 0, 1,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_misc_set_alignment (GTK_MISC (label126), 0, 0.5);

  label127 = gtk_label_new ("Visited URLs");
  gtk_widget_ref (label127);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "label127", label127,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (label127);
  gtk_table_attach (GTK_TABLE (table23), label127, 0, 1, 1, 2,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_misc_set_alignment (GTK_MISC (label127), 0, 0.5);

  label128 = gtk_label_new ("Strike line");
  gtk_widget_ref (label128);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "label128", label128,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (label128);
  gtk_table_attach (GTK_TABLE (table23), label128, 0, 1, 2, 3,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_misc_set_alignment (GTK_MISC (label128), 0, 0.5);

  bt_pp_url_color = gtk_button_new ();
  gtk_widget_ref (bt_pp_url_color);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "bt_pp_url_color", bt_pp_url_color,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (bt_pp_url_color);
  gtk_table_attach (GTK_TABLE (table23), bt_pp_url_color, 1, 2, 0, 1,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_widget_set_usize (bt_pp_url_color, 50, 20);
  gtk_tooltips_set_tip (tooltips, bt_pp_url_color, "Color in normal (non transparent) mode", NULL);

  bt_pp_url_color_trans = gtk_button_new ();
  gtk_widget_ref (bt_pp_url_color_trans);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "bt_pp_url_color_trans", bt_pp_url_color_trans,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (bt_pp_url_color_trans);
  gtk_table_attach (GTK_TABLE (table23), bt_pp_url_color_trans, 2, 3, 0, 1,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_widget_set_usize (bt_pp_url_color_trans, 50, 20);
  gtk_tooltips_set_tip (tooltips, bt_pp_url_color_trans, "Color in transparent mode", NULL);

  bt_pp_visited_url_color = gtk_button_new ();
  gtk_widget_ref (bt_pp_visited_url_color);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "bt_pp_visited_url_color", bt_pp_visited_url_color,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (bt_pp_visited_url_color);
  gtk_table_attach (GTK_TABLE (table23), bt_pp_visited_url_color, 1, 2, 1, 2,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_widget_set_usize (bt_pp_visited_url_color, 50, 20);
  gtk_tooltips_set_tip (tooltips, bt_pp_visited_url_color, "Color in normal (non transparent) mode", NULL);

  bt_pp_visited_url_color_trans = gtk_button_new ();
  gtk_widget_ref (bt_pp_visited_url_color_trans);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "bt_pp_visited_url_color_trans", bt_pp_visited_url_color_trans,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (bt_pp_visited_url_color_trans);
  gtk_table_attach (GTK_TABLE (table23), bt_pp_visited_url_color_trans, 2, 3, 1, 2,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_widget_set_usize (bt_pp_visited_url_color_trans, 50, 20);
  gtk_tooltips_set_tip (tooltips, bt_pp_visited_url_color_trans, "Color in transparent mode", NULL);

  bt_pp_strike_color = gtk_button_new ();
  gtk_widget_ref (bt_pp_strike_color);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "bt_pp_strike_color", bt_pp_strike_color,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (bt_pp_strike_color);
  gtk_table_attach (GTK_TABLE (table23), bt_pp_strike_color, 1, 2, 2, 3,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_widget_set_usize (bt_pp_strike_color, 50, 20);
  gtk_tooltips_set_tip (tooltips, bt_pp_strike_color, "Color in normal (non transparent) mode", NULL);

  bt_pp_strike_color_trans = gtk_button_new ();
  gtk_widget_ref (bt_pp_strike_color_trans);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "bt_pp_strike_color_trans", bt_pp_strike_color_trans,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (bt_pp_strike_color_trans);
  gtk_table_attach (GTK_TABLE (table23), bt_pp_strike_color_trans, 2, 3, 2, 3,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_widget_set_usize (bt_pp_strike_color_trans, 50, 20);
  gtk_tooltips_set_tip (tooltips, bt_pp_strike_color_trans, "Color in transparent mode", NULL);

  label8 = gtk_label_new ("Pinnipede");
  gtk_widget_ref (label8);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "label8", label8,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (label8);
  gtk_notebook_set_tab_label (GTK_NOTEBOOK (notebook1), gtk_notebook_get_nth_page (GTK_NOTEBOOK (notebook1), 5), label8);

  vbox27 = gtk_vbox_new (FALSE, 0);
  gtk_widget_ref (vbox27);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "vbox27", vbox27,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (vbox27);
  gtk_container_add (GTK_CONTAINER (notebook1), vbox27);

  frame23 = gtk_frame_new ("Global settings");
  gtk_widget_ref (frame23);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "frame23", frame23,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (frame23);
  gtk_box_pack_start (GTK_BOX (vbox27), frame23, TRUE, TRUE, 0);
  gtk_container_set_border_width (GTK_CONTAINER (frame23), 5);

  vbox28 = gtk_vbox_new (FALSE, 0);
  gtk_widget_ref (vbox28);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "vbox28", vbox28,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (vbox28);
  gtk_container_add (GTK_CONTAINER (frame23), vbox28);

  frame11 = gtk_frame_new ("Button Bar");
  gtk_widget_ref (frame11);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "frame11", frame11,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (frame11);
  gtk_box_pack_start (GTK_BOX (vbox28), frame11, FALSE, TRUE, 0);
  gtk_container_set_border_width (GTK_CONTAINER (frame11), 5);

  hbox10 = gtk_hbox_new (FALSE, 0);
  gtk_widget_ref (hbox10);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "hbox10", hbox10,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (hbox10);
  gtk_container_add (GTK_CONTAINER (frame11), hbox10);

  table9 = gtk_table_new (3, 3, FALSE);
  gtk_widget_ref (table9);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "table9", table9,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (table9);
  gtk_box_pack_start (GTK_BOX (hbox10), table9, TRUE, TRUE, 0);

  label52 = gtk_label_new ("Foreground color");
  gtk_widget_ref (label52);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "label52", label52,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (label52);
  gtk_table_attach (GTK_TABLE (table9), label52, 0, 1, 2, 3,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_misc_set_alignment (GTK_MISC (label52), 0, 0.5);

  bt_pp_bb_fgcolor = gtk_button_new ();
  gtk_widget_ref (bt_pp_bb_fgcolor);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "bt_pp_bb_fgcolor", bt_pp_bb_fgcolor,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (bt_pp_bb_fgcolor);
  gtk_table_attach (GTK_TABLE (table9), bt_pp_bb_fgcolor, 1, 2, 2, 3,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_widget_set_usize (bt_pp_bb_fgcolor, 50, 20);
  gtk_tooltips_set_tip (tooltips, bt_pp_bb_fgcolor, "Color in normal (non transparent) mode", NULL);

  bt_pp_bb_fgcolor_trans = gtk_button_new ();
  gtk_widget_ref (bt_pp_bb_fgcolor_trans);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "bt_pp_bb_fgcolor_trans", bt_pp_bb_fgcolor_trans,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (bt_pp_bb_fgcolor_trans);
  gtk_table_attach (GTK_TABLE (table9), bt_pp_bb_fgcolor_trans, 2, 3, 2, 3,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_widget_set_usize (bt_pp_bb_fgcolor_trans, 50, 20);
  gtk_tooltips_set_tip (tooltips, bt_pp_bb_fgcolor_trans, "Color in transparent mode", NULL);

  bt_pp_bb_bgcolor_trans = gtk_button_new ();
  gtk_widget_ref (bt_pp_bb_bgcolor_trans);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "bt_pp_bb_bgcolor_trans", bt_pp_bb_bgcolor_trans,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (bt_pp_bb_bgcolor_trans);
  gtk_table_attach (GTK_TABLE (table9), bt_pp_bb_bgcolor_trans, 2, 3, 1, 2,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_widget_set_usize (bt_pp_bb_bgcolor_trans, 50, 20);
  gtk_tooltips_set_tip (tooltips, bt_pp_bb_bgcolor_trans, "Color in transparent mode", NULL);

  bt_pp_bb_bgcolor = gtk_button_new ();
  gtk_widget_ref (bt_pp_bb_bgcolor);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "bt_pp_bb_bgcolor", bt_pp_bb_bgcolor,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (bt_pp_bb_bgcolor);
  gtk_table_attach (GTK_TABLE (table9), bt_pp_bb_bgcolor, 1, 2, 1, 2,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_widget_set_usize (bt_pp_bb_bgcolor, 50, 20);
  gtk_tooltips_set_tip (tooltips, bt_pp_bb_bgcolor, "Color in normal (non transparent) mode", NULL);

  label51 = gtk_label_new ("Background color");
  gtk_widget_ref (label51);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "label51", label51,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (label51);
  gtk_table_attach (GTK_TABLE (table9), label51, 0, 1, 1, 2,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_misc_set_alignment (GTK_MISC (label51), 0, 0.5);

  checkbutton_pp_minibar_on = gtk_check_button_new_with_label ("Visible on startup");
  gtk_widget_ref (checkbutton_pp_minibar_on);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "checkbutton_pp_minibar_on", checkbutton_pp_minibar_on,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (checkbutton_pp_minibar_on);
  gtk_table_attach (GTK_TABLE (table9), checkbutton_pp_minibar_on, 0, 3, 0, 1,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);

  table10 = gtk_table_new (3, 3, FALSE);
  gtk_widget_ref (table10);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "table10", table10,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (table10);
  gtk_box_pack_start (GTK_BOX (hbox10), table10, TRUE, TRUE, 0);

  label53 = gtk_label_new ("Progress bar color");
  gtk_widget_ref (label53);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "label53", label53,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (label53);
  gtk_table_attach (GTK_TABLE (table10), label53, 0, 1, 0, 1,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_misc_set_alignment (GTK_MISC (label53), 0, 0.5);

  label54 = gtk_label_new ("Upload/download counter color");
  gtk_widget_ref (label54);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "label54", label54,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (label54);
  gtk_table_attach (GTK_TABLE (table10), label54, 0, 1, 1, 2,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_misc_set_alignment (GTK_MISC (label54), 0, 0.5);

  bt_pp_bb_progressbar_color = gtk_button_new ();
  gtk_widget_ref (bt_pp_bb_progressbar_color);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "bt_pp_bb_progressbar_color", bt_pp_bb_progressbar_color,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (bt_pp_bb_progressbar_color);
  gtk_table_attach (GTK_TABLE (table10), bt_pp_bb_progressbar_color, 1, 2, 0, 1,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_widget_set_usize (bt_pp_bb_progressbar_color, 50, 20);
  gtk_tooltips_set_tip (tooltips, bt_pp_bb_progressbar_color, "Color in normal (non transparent) mode", NULL);

  bt_pp_bb_updlcnt_color = gtk_button_new ();
  gtk_widget_ref (bt_pp_bb_updlcnt_color);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "bt_pp_bb_updlcnt_color", bt_pp_bb_updlcnt_color,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (bt_pp_bb_updlcnt_color);
  gtk_table_attach (GTK_TABLE (table10), bt_pp_bb_updlcnt_color, 1, 2, 1, 2,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_widget_set_usize (bt_pp_bb_updlcnt_color, 50, 20);
  gtk_tooltips_set_tip (tooltips, bt_pp_bb_updlcnt_color, "Color in normal (non transparent) mode", NULL);

  label55 = gtk_label_new ("Message counter color");
  gtk_widget_ref (label55);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "label55", label55,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (label55);
  gtk_table_attach (GTK_TABLE (table10), label55, 0, 1, 2, 3,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_misc_set_alignment (GTK_MISC (label55), 0, 0.5);

  bt_pp_bb_msgcnt_color = gtk_button_new ();
  gtk_widget_ref (bt_pp_bb_msgcnt_color);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "bt_pp_bb_msgcnt_color", bt_pp_bb_msgcnt_color,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (bt_pp_bb_msgcnt_color);
  gtk_table_attach (GTK_TABLE (table10), bt_pp_bb_msgcnt_color, 1, 2, 2, 3,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_widget_set_usize (bt_pp_bb_msgcnt_color, 50, 20);
  gtk_tooltips_set_tip (tooltips, bt_pp_bb_msgcnt_color, "Color in normal (non transparent) mode", NULL);

  bt_pp_bb_progressbar_color_trans = gtk_button_new ();
  gtk_widget_ref (bt_pp_bb_progressbar_color_trans);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "bt_pp_bb_progressbar_color_trans", bt_pp_bb_progressbar_color_trans,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (bt_pp_bb_progressbar_color_trans);
  gtk_table_attach (GTK_TABLE (table10), bt_pp_bb_progressbar_color_trans, 2, 3, 0, 1,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_widget_set_usize (bt_pp_bb_progressbar_color_trans, 50, 20);
  gtk_tooltips_set_tip (tooltips, bt_pp_bb_progressbar_color_trans, "Color in transparent mode", NULL);

  bt_pp_bb_updlcnt_color_trans = gtk_button_new ();
  gtk_widget_ref (bt_pp_bb_updlcnt_color_trans);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "bt_pp_bb_updlcnt_color_trans", bt_pp_bb_updlcnt_color_trans,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (bt_pp_bb_updlcnt_color_trans);
  gtk_table_attach (GTK_TABLE (table10), bt_pp_bb_updlcnt_color_trans, 2, 3, 1, 2,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_widget_set_usize (bt_pp_bb_updlcnt_color_trans, 50, 20);
  gtk_tooltips_set_tip (tooltips, bt_pp_bb_updlcnt_color_trans, "Color in transparent mode", NULL);

  bt_pp_bb_msgcnt_color_trans = gtk_button_new ();
  gtk_widget_ref (bt_pp_bb_msgcnt_color_trans);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "bt_pp_bb_msgcnt_color_trans", bt_pp_bb_msgcnt_color_trans,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (bt_pp_bb_msgcnt_color_trans);
  gtk_table_attach (GTK_TABLE (table10), bt_pp_bb_msgcnt_color_trans, 2, 3, 2, 3,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_widget_set_usize (bt_pp_bb_msgcnt_color_trans, 50, 20);
  gtk_tooltips_set_tip (tooltips, bt_pp_bb_msgcnt_color_trans, "Color in transparent mode", NULL);

  frame12 = gtk_frame_new ("Bracket emphasizing");
  gtk_widget_ref (frame12);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "frame12", frame12,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (frame12);
  gtk_box_pack_start (GTK_BOX (vbox28), frame12, TRUE, TRUE, 0);
  gtk_container_set_border_width (GTK_CONTAINER (frame12), 5);

  vbox16 = gtk_vbox_new (FALSE, 0);
  gtk_widget_ref (vbox16);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "vbox16", vbox16,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (vbox16);
  gtk_container_add (GTK_CONTAINER (frame12), vbox16);

  hbox12 = gtk_hbox_new (FALSE, 0);
  gtk_widget_ref (hbox12);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "hbox12", hbox12,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (hbox12);
  gtk_box_pack_start (GTK_BOX (vbox16), hbox12, FALSE, TRUE, 0);

  pixmap1 = create_pixmap (main_win, "bracket.xpm");
  gtk_widget_ref (pixmap1);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "pixmap1", pixmap1,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (pixmap1);
  gtk_box_pack_start (GTK_BOX (hbox12), pixmap1, FALSE, FALSE, 3);
  gtk_misc_set_alignment (GTK_MISC (pixmap1), 0, 7.45058e-09);

  label58 = gtk_label_new ("Color for your messages");
  gtk_widget_ref (label58);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "label58", label58,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (label58);
  gtk_box_pack_start (GTK_BOX (hbox12), label58, FALSE, FALSE, 0);
  gtk_misc_set_alignment (GTK_MISC (label58), 0, 0.5);

  bt_pp_my_messages = gtk_button_new ();
  gtk_widget_ref (bt_pp_my_messages);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "bt_pp_my_messages", bt_pp_my_messages,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (bt_pp_my_messages);
  gtk_box_pack_start (GTK_BOX (hbox12), bt_pp_my_messages, FALSE, FALSE, 2);
  gtk_widget_set_usize (bt_pp_my_messages, 50, 20);
  gtk_tooltips_set_tip (tooltips, bt_pp_my_messages, "Color in normal (non transparent) mode", NULL);

  bt_pp_my_messages_trans = gtk_button_new ();
  gtk_widget_ref (bt_pp_my_messages_trans);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "bt_pp_my_messages_trans", bt_pp_my_messages_trans,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (bt_pp_my_messages_trans);
  gtk_box_pack_start (GTK_BOX (hbox12), bt_pp_my_messages_trans, FALSE, FALSE, 0);
  gtk_widget_set_usize (bt_pp_my_messages_trans, 50, 20);
  gtk_tooltips_set_tip (tooltips, bt_pp_my_messages_trans, "Color in transparent mode", NULL);

  label59 = gtk_label_new ("Color for answers to your messages");
  gtk_widget_ref (label59);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "label59", label59,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (label59);
  gtk_box_pack_start (GTK_BOX (hbox12), label59, FALSE, FALSE, 2);
  gtk_misc_set_alignment (GTK_MISC (label59), 0, 0.5);

  bt_pp_answers_to_my_messages = gtk_button_new ();
  gtk_widget_ref (bt_pp_answers_to_my_messages);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "bt_pp_answers_to_my_messages", bt_pp_answers_to_my_messages,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (bt_pp_answers_to_my_messages);
  gtk_box_pack_start (GTK_BOX (hbox12), bt_pp_answers_to_my_messages, FALSE, FALSE, 2);
  gtk_widget_set_usize (bt_pp_answers_to_my_messages, 50, 20);
  gtk_tooltips_set_tip (tooltips, bt_pp_answers_to_my_messages, "Color in normal (non transparent) mode", NULL);

  bt_pp_answers_to_my_messages_trans = gtk_button_new ();
  gtk_widget_ref (bt_pp_answers_to_my_messages_trans);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "bt_pp_answers_to_my_messages_trans", bt_pp_answers_to_my_messages_trans,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (bt_pp_answers_to_my_messages_trans);
  gtk_box_pack_start (GTK_BOX (hbox12), bt_pp_answers_to_my_messages_trans, FALSE, FALSE, 0);
  gtk_widget_set_usize (bt_pp_answers_to_my_messages_trans, 50, 20);
  gtk_tooltips_set_tip (tooltips, bt_pp_answers_to_my_messages_trans, "Color in transparent mode", NULL);

  hseparator2 = gtk_hseparator_new ();
  gtk_widget_ref (hseparator2);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "hseparator2", hseparator2,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (hseparator2);
  gtk_box_pack_start (GTK_BOX (vbox16), hseparator2, FALSE, TRUE, 8);

  table13 = gtk_table_new (2, 7, FALSE);
  gtk_widget_ref (table13);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "table13", table13,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (table13);
  gtk_box_pack_start (GTK_BOX (vbox16), table13, FALSE, TRUE, 0);

  bt_pp_hilight_keyword_color0 = gtk_button_new ();
  gtk_widget_ref (bt_pp_hilight_keyword_color0);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "bt_pp_hilight_keyword_color0", bt_pp_hilight_keyword_color0,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (bt_pp_hilight_keyword_color0);
  gtk_table_attach (GTK_TABLE (table13), bt_pp_hilight_keyword_color0, 2, 3, 0, 1,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_widget_set_usize (bt_pp_hilight_keyword_color0, 50, 20);
  gtk_tooltips_set_tip (tooltips, bt_pp_hilight_keyword_color0, "Color in normal (non transparent) mode", NULL);

  bt_pp_hilight_keyword_color2 = gtk_button_new ();
  gtk_widget_ref (bt_pp_hilight_keyword_color2);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "bt_pp_hilight_keyword_color2", bt_pp_hilight_keyword_color2,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (bt_pp_hilight_keyword_color2);
  gtk_table_attach (GTK_TABLE (table13), bt_pp_hilight_keyword_color2, 4, 5, 0, 1,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_widget_set_usize (bt_pp_hilight_keyword_color2, 50, 20);
  gtk_tooltips_set_tip (tooltips, bt_pp_hilight_keyword_color2, "Color in normal (non transparent) mode", NULL);

  bt_pp_hilight_keyword_color3 = gtk_button_new ();
  gtk_widget_ref (bt_pp_hilight_keyword_color3);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "bt_pp_hilight_keyword_color3", bt_pp_hilight_keyword_color3,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (bt_pp_hilight_keyword_color3);
  gtk_table_attach (GTK_TABLE (table13), bt_pp_hilight_keyword_color3, 5, 6, 0, 1,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_widget_set_usize (bt_pp_hilight_keyword_color3, 50, 20);
  gtk_tooltips_set_tip (tooltips, bt_pp_hilight_keyword_color3, "Color in normal (non transparent) mode", NULL);

  bt_pp_hilight_keyword_color4 = gtk_button_new ();
  gtk_widget_ref (bt_pp_hilight_keyword_color4);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "bt_pp_hilight_keyword_color4", bt_pp_hilight_keyword_color4,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (bt_pp_hilight_keyword_color4);
  gtk_table_attach (GTK_TABLE (table13), bt_pp_hilight_keyword_color4, 6, 7, 0, 1,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_widget_set_usize (bt_pp_hilight_keyword_color4, 50, 20);
  gtk_tooltips_set_tip (tooltips, bt_pp_hilight_keyword_color4, "Color in normal (non transparent) mode", NULL);

  bt_pp_hilight_keyword_color0_trans = gtk_button_new ();
  gtk_widget_ref (bt_pp_hilight_keyword_color0_trans);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "bt_pp_hilight_keyword_color0_trans", bt_pp_hilight_keyword_color0_trans,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (bt_pp_hilight_keyword_color0_trans);
  gtk_table_attach (GTK_TABLE (table13), bt_pp_hilight_keyword_color0_trans, 2, 3, 1, 2,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_widget_set_usize (bt_pp_hilight_keyword_color0_trans, 50, 20);
  gtk_tooltips_set_tip (tooltips, bt_pp_hilight_keyword_color0_trans, "Color in transparent mode", NULL);

  bt_pp_hilight_keyword_color1_trans = gtk_button_new ();
  gtk_widget_ref (bt_pp_hilight_keyword_color1_trans);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "bt_pp_hilight_keyword_color1_trans", bt_pp_hilight_keyword_color1_trans,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (bt_pp_hilight_keyword_color1_trans);
  gtk_table_attach (GTK_TABLE (table13), bt_pp_hilight_keyword_color1_trans, 3, 4, 1, 2,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_widget_set_usize (bt_pp_hilight_keyword_color1_trans, 50, 20);
  gtk_tooltips_set_tip (tooltips, bt_pp_hilight_keyword_color1_trans, "Color in transparent mode", NULL);

  bt_pp_hilight_keyword_color2_trans = gtk_button_new ();
  gtk_widget_ref (bt_pp_hilight_keyword_color2_trans);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "bt_pp_hilight_keyword_color2_trans", bt_pp_hilight_keyword_color2_trans,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (bt_pp_hilight_keyword_color2_trans);
  gtk_table_attach (GTK_TABLE (table13), bt_pp_hilight_keyword_color2_trans, 4, 5, 1, 2,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_widget_set_usize (bt_pp_hilight_keyword_color2_trans, 50, 20);
  gtk_tooltips_set_tip (tooltips, bt_pp_hilight_keyword_color2_trans, "Color in transparent mode", NULL);

  bt_pp_hilight_keyword_color3_trans = gtk_button_new ();
  gtk_widget_ref (bt_pp_hilight_keyword_color3_trans);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "bt_pp_hilight_keyword_color3_trans", bt_pp_hilight_keyword_color3_trans,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (bt_pp_hilight_keyword_color3_trans);
  gtk_table_attach (GTK_TABLE (table13), bt_pp_hilight_keyword_color3_trans, 5, 6, 1, 2,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_widget_set_usize (bt_pp_hilight_keyword_color3_trans, 50, 20);
  gtk_tooltips_set_tip (tooltips, bt_pp_hilight_keyword_color3_trans, "Color in transparent mode", NULL);

  bt_pp_hilight_keyword_color4_trans = gtk_button_new ();
  gtk_widget_ref (bt_pp_hilight_keyword_color4_trans);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "bt_pp_hilight_keyword_color4_trans", bt_pp_hilight_keyword_color4_trans,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (bt_pp_hilight_keyword_color4_trans);
  gtk_table_attach (GTK_TABLE (table13), bt_pp_hilight_keyword_color4_trans, 6, 7, 1, 2,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_widget_set_usize (bt_pp_hilight_keyword_color4_trans, 50, 20);
  gtk_tooltips_set_tip (tooltips, bt_pp_hilight_keyword_color4_trans, "Color in transparent mode", NULL);

  label61 = gtk_label_new ("(normal mode)");
  gtk_widget_ref (label61);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "label61", label61,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (label61);
  gtk_table_attach (GTK_TABLE (table13), label61, 1, 2, 0, 1,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_misc_set_alignment (GTK_MISC (label61), 1, 0.5);

  label62 = gtk_label_new ("(transparency mode)");
  gtk_widget_ref (label62);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "label62", label62,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (label62);
  gtk_table_attach (GTK_TABLE (table13), label62, 1, 2, 1, 2,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_misc_set_alignment (GTK_MISC (label62), 1, 0.5);

  label60 = gtk_label_new ("Keyword emphasizing, categ 0-4");
  gtk_widget_ref (label60);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "label60", label60,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (label60);
  gtk_table_attach (GTK_TABLE (table13), label60, 0, 1, 0, 2,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_misc_set_alignment (GTK_MISC (label60), 0, 0.5);
  gtk_misc_set_padding (GTK_MISC (label60), 18, 0);

  bt_pp_hilight_keyword_color1 = gtk_button_new ();
  gtk_widget_ref (bt_pp_hilight_keyword_color1);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "bt_pp_hilight_keyword_color1", bt_pp_hilight_keyword_color1,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (bt_pp_hilight_keyword_color1);
  gtk_table_attach (GTK_TABLE (table13), bt_pp_hilight_keyword_color1, 3, 4, 0, 1,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_widget_set_usize (bt_pp_hilight_keyword_color1, 50, 20);
  gtk_tooltips_set_tip (tooltips, bt_pp_hilight_keyword_color1, "Color in normal (non transparent) mode", NULL);

  hbox14 = gtk_hbox_new (FALSE, 0);
  gtk_widget_ref (hbox14);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "hbox14", hbox14,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (hbox14);
  gtk_box_pack_start (GTK_BOX (vbox16), hbox14, TRUE, TRUE, 0);

  vbox38 = gtk_vbox_new (FALSE, 0);
  gtk_widget_ref (vbox38);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "vbox38", vbox38,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (vbox38);
  gtk_box_pack_start (GTK_BOX (hbox14), vbox38, TRUE, TRUE, 0);

  scrolledwindow6 = gtk_scrolled_window_new (NULL, NULL);
  gtk_widget_ref (scrolledwindow6);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "scrolledwindow6", scrolledwindow6,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (scrolledwindow6);
  gtk_box_pack_start (GTK_BOX (vbox38), scrolledwindow6, TRUE, TRUE, 0);
  gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (scrolledwindow6), GTK_POLICY_NEVER, GTK_POLICY_AUTOMATIC);

  clist_kemph = gtk_clist_new (3);
  gtk_widget_ref (clist_kemph);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "clist_kemph", clist_kemph,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (clist_kemph);
  gtk_container_add (GTK_CONTAINER (scrolledwindow6), clist_kemph);
  gtk_clist_set_column_width (GTK_CLIST (clist_kemph), 0, 106);
  gtk_clist_set_column_width (GTK_CLIST (clist_kemph), 1, 323);
  gtk_clist_set_column_width (GTK_CLIST (clist_kemph), 2, 71);
  gtk_clist_column_titles_hide (GTK_CLIST (clist_kemph));

  label155 = gtk_label_new ("label155");
  gtk_widget_ref (label155);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "label155", label155,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (label155);
  gtk_clist_set_column_widget (GTK_CLIST (clist_kemph), 0, label155);

  label156 = gtk_label_new ("label156");
  gtk_widget_ref (label156);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "label156", label156,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (label156);
  gtk_clist_set_column_widget (GTK_CLIST (clist_kemph), 1, label156);

  label157 = gtk_label_new ("label157");
  gtk_widget_ref (label157);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "label157", label157,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (label157);
  gtk_clist_set_column_widget (GTK_CLIST (clist_kemph), 2, label157);

  hbox164 = gtk_hbox_new (FALSE, 0);
  gtk_widget_ref (hbox164);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "hbox164", hbox164,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (hbox164);
  gtk_box_pack_start (GTK_BOX (vbox38), hbox164, FALSE, FALSE, 0);

  optionmenu_kemph_match = gtk_option_menu_new ();
  gtk_widget_ref (optionmenu_kemph_match);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "optionmenu_kemph_match", optionmenu_kemph_match,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (optionmenu_kemph_match);
  gtk_box_pack_start (GTK_BOX (hbox164), optionmenu_kemph_match, FALSE, FALSE, 0);
  optionmenu_kemph_match_menu = gtk_menu_new ();
  glade_menuitem = gtk_menu_item_new_with_label ("match LOGIN");
  gtk_widget_show (glade_menuitem);
  gtk_menu_append (GTK_MENU (optionmenu_kemph_match_menu), glade_menuitem);
  glade_menuitem = gtk_menu_item_new_with_label ("match USERAGENT");
  gtk_widget_show (glade_menuitem);
  gtk_menu_append (GTK_MENU (optionmenu_kemph_match_menu), glade_menuitem);
  glade_menuitem = gtk_menu_item_new_with_label ("match WORD in message");
  gtk_widget_show (glade_menuitem);
  gtk_menu_append (GTK_MENU (optionmenu_kemph_match_menu), glade_menuitem);
  glade_menuitem = gtk_menu_item_new_with_label ("match USERAGENT if no login");
  gtk_widget_show (glade_menuitem);
  gtk_menu_append (GTK_MENU (optionmenu_kemph_match_menu), glade_menuitem);
  gtk_option_menu_set_menu (GTK_OPTION_MENU (optionmenu_kemph_match), optionmenu_kemph_match_menu);

  entry_kemph_key = gtk_entry_new ();
  gtk_widget_ref (entry_kemph_key);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "entry_kemph_key", entry_kemph_key,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (entry_kemph_key);
  gtk_box_pack_start (GTK_BOX (hbox164), entry_kemph_key, TRUE, TRUE, 0);

  optionmenu_kemph_categ = gtk_option_menu_new ();
  gtk_widget_ref (optionmenu_kemph_categ);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "optionmenu_kemph_categ", optionmenu_kemph_categ,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (optionmenu_kemph_categ);
  gtk_box_pack_start (GTK_BOX (hbox164), optionmenu_kemph_categ, FALSE, FALSE, 0);
  optionmenu_kemph_categ_menu = gtk_menu_new ();
  glade_menuitem = gtk_menu_item_new_with_label ("categ 0");
  gtk_widget_show (glade_menuitem);
  gtk_menu_append (GTK_MENU (optionmenu_kemph_categ_menu), glade_menuitem);
  glade_menuitem = gtk_menu_item_new_with_label ("categ 1");
  gtk_widget_show (glade_menuitem);
  gtk_menu_append (GTK_MENU (optionmenu_kemph_categ_menu), glade_menuitem);
  glade_menuitem = gtk_menu_item_new_with_label ("categ 2");
  gtk_widget_show (glade_menuitem);
  gtk_menu_append (GTK_MENU (optionmenu_kemph_categ_menu), glade_menuitem);
  glade_menuitem = gtk_menu_item_new_with_label ("categ 3");
  gtk_widget_show (glade_menuitem);
  gtk_menu_append (GTK_MENU (optionmenu_kemph_categ_menu), glade_menuitem);
  glade_menuitem = gtk_menu_item_new_with_label ("categ 4");
  gtk_widget_show (glade_menuitem);
  gtk_menu_append (GTK_MENU (optionmenu_kemph_categ_menu), glade_menuitem);
  gtk_option_menu_set_menu (GTK_OPTION_MENU (optionmenu_kemph_categ), optionmenu_kemph_categ_menu);

  vbox35 = gtk_vbox_new (FALSE, 0);
  gtk_widget_ref (vbox35);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "vbox35", vbox35,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (vbox35);
  gtk_box_pack_start (GTK_BOX (hbox14), vbox35, FALSE, FALSE, 0);

  bt_kemph_up = gtk_button_new_with_label ("Move up");
  gtk_widget_ref (bt_kemph_up);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "bt_kemph_up", bt_kemph_up,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (bt_kemph_up);
  gtk_box_pack_start (GTK_BOX (vbox35), bt_kemph_up, FALSE, FALSE, 0);

  bt_kemph_down = gtk_button_new_with_label ("Move down");
  gtk_widget_ref (bt_kemph_down);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "bt_kemph_down", bt_kemph_down,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (bt_kemph_down);
  gtk_box_pack_start (GTK_BOX (vbox35), bt_kemph_down, FALSE, FALSE, 0);

  bt_kemph_del = gtk_button_new_with_label ("Delete");
  gtk_widget_ref (bt_kemph_del);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "bt_kemph_del", bt_kemph_del,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (bt_kemph_del);
  gtk_box_pack_start (GTK_BOX (vbox35), bt_kemph_del, FALSE, FALSE, 0);

  bt_kemph_change = gtk_button_new_with_label ("Change");
  gtk_widget_ref (bt_kemph_change);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "bt_kemph_change", bt_kemph_change,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (bt_kemph_change);
  gtk_box_pack_end (GTK_BOX (vbox35), bt_kemph_change, FALSE, FALSE, 0);

  bt_kemph_add = gtk_button_new_with_label ("Add");
  gtk_widget_ref (bt_kemph_add);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "bt_kemph_add", bt_kemph_add,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (bt_kemph_add);
  gtk_box_pack_end (GTK_BOX (vbox35), bt_kemph_add, FALSE, FALSE, 0);

  frame32 = gtk_frame_new ("Plopification/Boitakon");
  gtk_widget_ref (frame32);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "frame32", frame32,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (frame32);
  gtk_box_pack_start (GTK_BOX (vbox28), frame32, TRUE, TRUE, 0);
  gtk_container_set_border_width (GTK_CONTAINER (frame32), 5);

  vbox371 = gtk_vbox_new (FALSE, 0);
  gtk_widget_ref (vbox371);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "vbox371", vbox371,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (vbox371);
  gtk_container_add (GTK_CONTAINER (frame32), vbox371);

  hbox167 = gtk_hbox_new (FALSE, 0);
  gtk_widget_ref (hbox167);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "hbox167", hbox167,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (hbox167);
  gtk_box_pack_start (GTK_BOX (vbox371), hbox167, FALSE, FALSE, 0);

  label63 = gtk_label_new ("Plopified text color");
  gtk_widget_ref (label63);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "label63", label63,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (label63);
  gtk_box_pack_start (GTK_BOX (hbox167), label63, FALSE, FALSE, 0);
  gtk_misc_set_alignment (GTK_MISC (label63), 3.42727e-07, 0.5);
  gtk_misc_set_padding (GTK_MISC (label63), 12, 0);

  bt_pp_plopified_color = gtk_button_new ();
  gtk_widget_ref (bt_pp_plopified_color);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "bt_pp_plopified_color", bt_pp_plopified_color,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (bt_pp_plopified_color);
  gtk_box_pack_start (GTK_BOX (hbox167), bt_pp_plopified_color, FALSE, FALSE, 0);
  gtk_widget_set_usize (bt_pp_plopified_color, 50, 20);
  gtk_tooltips_set_tip (tooltips, bt_pp_plopified_color, "Color in normal (non transparent) mode", NULL);

  bt_pp_plopified_color_trans = gtk_button_new ();
  gtk_widget_ref (bt_pp_plopified_color_trans);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "bt_pp_plopified_color_trans", bt_pp_plopified_color_trans,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (bt_pp_plopified_color_trans);
  gtk_box_pack_start (GTK_BOX (hbox167), bt_pp_plopified_color_trans, FALSE, FALSE, 0);
  gtk_widget_set_usize (bt_pp_plopified_color_trans, 50, 20);
  gtk_tooltips_set_tip (tooltips, bt_pp_plopified_color_trans, "Color in transparent mode", NULL);

  hbox165 = gtk_hbox_new (FALSE, 0);
  gtk_widget_ref (hbox165);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "hbox165", hbox165,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (hbox165);
  gtk_box_pack_start (GTK_BOX (vbox371), hbox165, TRUE, TRUE, 0);

  vbox39 = gtk_vbox_new (FALSE, 0);
  gtk_widget_ref (vbox39);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "vbox39", vbox39,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (vbox39);
  gtk_box_pack_start (GTK_BOX (hbox165), vbox39, TRUE, TRUE, 0);

  scrolledwindow7 = gtk_scrolled_window_new (NULL, NULL);
  gtk_widget_ref (scrolledwindow7);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "scrolledwindow7", scrolledwindow7,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (scrolledwindow7);
  gtk_box_pack_start (GTK_BOX (vbox39), scrolledwindow7, TRUE, TRUE, 0);
  gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (scrolledwindow7), GTK_POLICY_NEVER, GTK_POLICY_AUTOMATIC);

  clist_kplop = gtk_clist_new (3);
  gtk_widget_ref (clist_kplop);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "clist_kplop", clist_kplop,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (clist_kplop);
  gtk_container_add (GTK_CONTAINER (scrolledwindow7), clist_kplop);
  gtk_tooltips_set_tip (tooltips, clist_kplop, "Hint: to temporarily disable all plopification/boitakon, use the gray button on the pinnipede button bar.", NULL);
  gtk_clist_set_column_width (GTK_CLIST (clist_kplop), 0, 112);
  gtk_clist_set_column_width (GTK_CLIST (clist_kplop), 1, 295);
  gtk_clist_set_column_width (GTK_CLIST (clist_kplop), 2, 17);
  gtk_clist_column_titles_hide (GTK_CLIST (clist_kplop));

  label158 = gtk_label_new ("label155");
  gtk_widget_ref (label158);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "label158", label158,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (label158);
  gtk_clist_set_column_widget (GTK_CLIST (clist_kplop), 0, label158);

  label159 = gtk_label_new ("label156");
  gtk_widget_ref (label159);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "label159", label159,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (label159);
  gtk_clist_set_column_widget (GTK_CLIST (clist_kplop), 1, label159);

  label160 = gtk_label_new ("label157");
  gtk_widget_ref (label160);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "label160", label160,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (label160);
  gtk_clist_set_column_widget (GTK_CLIST (clist_kplop), 2, label160);

  hbox166 = gtk_hbox_new (FALSE, 0);
  gtk_widget_ref (hbox166);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "hbox166", hbox166,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (hbox166);
  gtk_box_pack_start (GTK_BOX (vbox39), hbox166, FALSE, FALSE, 0);

  optionmenu_kplop_match = gtk_option_menu_new ();
  gtk_widget_ref (optionmenu_kplop_match);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "optionmenu_kplop_match", optionmenu_kplop_match,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (optionmenu_kplop_match);
  gtk_box_pack_start (GTK_BOX (hbox166), optionmenu_kplop_match, FALSE, FALSE, 0);
  optionmenu_kplop_match_menu = gtk_menu_new ();
  glade_menuitem = gtk_menu_item_new_with_label ("match LOGIN");
  gtk_widget_show (glade_menuitem);
  gtk_menu_append (GTK_MENU (optionmenu_kplop_match_menu), glade_menuitem);
  glade_menuitem = gtk_menu_item_new_with_label ("match USERAGENT");
  gtk_widget_show (glade_menuitem);
  gtk_menu_append (GTK_MENU (optionmenu_kplop_match_menu), glade_menuitem);
  glade_menuitem = gtk_menu_item_new_with_label ("match WORD in message");
  gtk_widget_show (glade_menuitem);
  gtk_menu_append (GTK_MENU (optionmenu_kplop_match_menu), glade_menuitem);
  glade_menuitem = gtk_menu_item_new_with_label ("match USERAGENT if no login");
  gtk_widget_show (glade_menuitem);
  gtk_menu_append (GTK_MENU (optionmenu_kplop_match_menu), glade_menuitem);
  gtk_option_menu_set_menu (GTK_OPTION_MENU (optionmenu_kplop_match), optionmenu_kplop_match_menu);

  entry_kplop_key = gtk_entry_new ();
  gtk_widget_ref (entry_kplop_key);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "entry_kplop_key", entry_kplop_key,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (entry_kplop_key);
  gtk_box_pack_start (GTK_BOX (hbox166), entry_kplop_key, TRUE, TRUE, 0);

  optionmenu_kplop_categ = gtk_option_menu_new ();
  gtk_widget_ref (optionmenu_kplop_categ);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "optionmenu_kplop_categ", optionmenu_kplop_categ,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (optionmenu_kplop_categ);
  gtk_box_pack_start (GTK_BOX (hbox166), optionmenu_kplop_categ, FALSE, FALSE, 0);
  optionmenu_kplop_categ_menu = gtk_menu_new ();
  glade_menuitem = gtk_menu_item_new_with_label ("plopify");
  gtk_widget_show (glade_menuitem);
  gtk_menu_append (GTK_MENU (optionmenu_kplop_categ_menu), glade_menuitem);
  glade_menuitem = gtk_menu_item_new_with_label ("superplopify");
  gtk_widget_show (glade_menuitem);
  gtk_menu_append (GTK_MENU (optionmenu_kplop_categ_menu), glade_menuitem);
  glade_menuitem = gtk_menu_item_new_with_label ("boitakon");
  gtk_widget_show (glade_menuitem);
  gtk_menu_append (GTK_MENU (optionmenu_kplop_categ_menu), glade_menuitem);
  gtk_option_menu_set_menu (GTK_OPTION_MENU (optionmenu_kplop_categ), optionmenu_kplop_categ_menu);

  vbox37 = gtk_vbox_new (FALSE, 0);
  gtk_widget_ref (vbox37);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "vbox37", vbox37,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (vbox37);
  gtk_box_pack_start (GTK_BOX (hbox165), vbox37, FALSE, FALSE, 0);

  bt_kplop_up = gtk_button_new_with_label ("Move up");
  gtk_widget_ref (bt_kplop_up);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "bt_kplop_up", bt_kplop_up,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (bt_kplop_up);
  gtk_box_pack_start (GTK_BOX (vbox37), bt_kplop_up, FALSE, FALSE, 0);

  bt_kplop_down = gtk_button_new_with_label ("Move down");
  gtk_widget_ref (bt_kplop_down);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "bt_kplop_down", bt_kplop_down,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (bt_kplop_down);
  gtk_box_pack_start (GTK_BOX (vbox37), bt_kplop_down, FALSE, FALSE, 0);

  bt_kplop_del = gtk_button_new_with_label ("Delete");
  gtk_widget_ref (bt_kplop_del);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "bt_kplop_del", bt_kplop_del,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (bt_kplop_del);
  gtk_box_pack_start (GTK_BOX (vbox37), bt_kplop_del, FALSE, FALSE, 0);

  bt_kplop_change = gtk_button_new_with_label ("Change");
  gtk_widget_ref (bt_kplop_change);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "bt_kplop_change", bt_kplop_change,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (bt_kplop_change);
  gtk_box_pack_end (GTK_BOX (vbox37), bt_kplop_change, FALSE, FALSE, 0);

  bt_kplop_add = gtk_button_new_with_label ("Add");
  gtk_widget_ref (bt_kplop_add);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "bt_kplop_add", bt_kplop_add,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (bt_kplop_add);
  gtk_box_pack_end (GTK_BOX (vbox37), bt_kplop_add, FALSE, FALSE, 0);

  label117 = gtk_label_new ("Pinnipede2");
  gtk_widget_ref (label117);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "label117", label117,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (label117);
  gtk_notebook_set_tab_label (GTK_NOTEBOOK (notebook1), gtk_notebook_get_nth_page (GTK_NOTEBOOK (notebook1), 6), label117);

  vbox31 = gtk_vbox_new (FALSE, 0);
  gtk_widget_ref (vbox31);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "vbox31", vbox31,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (vbox31);
  gtk_container_add (GTK_CONTAINER (notebook1), vbox31);

  frame28 = gtk_frame_new ("Global settings");
  gtk_widget_ref (frame28);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "frame28", frame28,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (frame28);
  gtk_box_pack_start (GTK_BOX (vbox31), frame28, FALSE, TRUE, 0);
  gtk_container_set_border_width (GTK_CONTAINER (frame28), 5);
  gtk_frame_set_shadow_type (GTK_FRAME (frame28), GTK_SHADOW_OUT);

  vbox17 = gtk_vbox_new (FALSE, 0);
  gtk_widget_ref (vbox17);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "vbox17", vbox17,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (vbox17);
  gtk_container_add (GTK_CONTAINER (frame28), vbox17);

  frame13 = gtk_frame_new ("Browsers");
  gtk_widget_ref (frame13);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "frame13", frame13,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (frame13);
  gtk_box_pack_start (GTK_BOX (vbox17), frame13, FALSE, TRUE, 0);
  gtk_container_set_border_width (GTK_CONTAINER (frame13), 5);
  gtk_frame_set_label_align (GTK_FRAME (frame13), 4.84288e-08, 0.5);

  vbox18 = gtk_vbox_new (FALSE, 0);
  gtk_widget_ref (vbox18);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "vbox18", vbox18,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (vbox18);
  gtk_container_add (GTK_CONTAINER (frame13), vbox18);

  label64 = gtk_label_new ("These options allows you to launch an external browser from wmcoincoin, this is used for reading the news, the comments detected by the flamophone, the personnal messages, the [url]s of the pinnipede teletype.\nBE CAREFUL: this kind of 'external' command might be a great source of vulnerability since the urls can contain ANY caracter (think of 'http://plop.org; reboot' ..\nFor this reason, dangerous caracter are carefully escapped by wmcc before calling the external browser\nBy default, no browser is selected. \nIn the command, the %s is remplaced by the escaped URL. Do not quote it, or your browser will have problems to read urls containg character such as '&', '$' etc. Do not forget to put an '&' at the end of the command line or wmcc will freeze until the browser is closed.");
  gtk_widget_ref (label64);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "label64", label64,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (label64);
  gtk_box_pack_start (GTK_BOX (vbox18), label64, FALSE, FALSE, 0);
  gtk_widget_set_usize (label64, 590, -2);
  gtk_label_set_justify (GTK_LABEL (label64), GTK_JUSTIFY_FILL);
  gtk_label_set_line_wrap (GTK_LABEL (label64), TRUE);
  gtk_misc_set_alignment (GTK_MISC (label64), 0.4, 0.5);

  table14 = gtk_table_new (3, 2, FALSE);
  gtk_widget_ref (table14);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "table14", table14,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (table14);
  gtk_box_pack_start (GTK_BOX (vbox18), table14, FALSE, TRUE, 0);

  combo3 = gtk_combo_new ();
  gtk_widget_ref (combo3);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "combo3", combo3,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (combo3);
  gtk_table_attach (GTK_TABLE (table14), combo3, 1, 2, 2, 3,
                    (GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  combo3_items = g_list_append (combo3_items, (gpointer) "galeon -n %s &");
  combo3_items = g_list_append (combo3_items, (gpointer) "konqueror %s &");
  combo3_items = g_list_append (combo3_items, (gpointer) "netscape -remote openurl\\(%s\\) &");
  combo3_items = g_list_append (combo3_items, (gpointer) "mozilla %s &");
  combo3_items = g_list_append (combo3_items, (gpointer) "mozilla -remote openurl\\(%s,new-tab\\) &");
  combo3_items = g_list_append (combo3_items, (gpointer) "opera %s &");
  combo3_items = g_list_append (combo3_items, (gpointer) "xterm -fn 6x10 -bg white -fg black -geometry 120x70 -e w3m %s &");
  combo3_items = g_list_append (combo3_items, (gpointer) "links -g %s &");
  combo3_items = g_list_append (combo3_items, (gpointer) "dillo %s &");
  gtk_combo_set_popdown_strings (GTK_COMBO (combo3), combo3_items);
  g_list_free (combo3_items);

  comboentry_browser2 = GTK_COMBO (combo3)->entry;
  gtk_widget_ref (comboentry_browser2);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "comboentry_browser2", comboentry_browser2,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (comboentry_browser2);
  gtk_entry_set_text (GTK_ENTRY (comboentry_browser2), "galeon -n %s &");

  hseparator4 = gtk_hseparator_new ();
  gtk_widget_ref (hseparator4);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "hseparator4", hseparator4,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (hseparator4);
  gtk_table_attach (GTK_TABLE (table14), hseparator4, 0, 2, 0, 1,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (GTK_EXPAND | GTK_FILL), 0, 0);

  combo2 = gtk_combo_new ();
  gtk_widget_ref (combo2);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "combo2", combo2,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (combo2);
  gtk_table_attach (GTK_TABLE (table14), combo2, 1, 2, 1, 2,
                    (GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  combo2_items = g_list_append (combo2_items, (gpointer) "galeon -n %s &");
  combo2_items = g_list_append (combo2_items, (gpointer) "konqueror %s &");
  combo2_items = g_list_append (combo2_items, (gpointer) "netscape -remote openurl\\(%s\\) &");
  combo2_items = g_list_append (combo2_items, (gpointer) "mozilla %s &");
  combo2_items = g_list_append (combo2_items, (gpointer) "mozilla -remote openurl\\(%s,new-tab\\) &");
  combo2_items = g_list_append (combo2_items, (gpointer) "opera %s &");
  combo2_items = g_list_append (combo2_items, (gpointer) "xterm -fn 6x10 -bg white -fg black -geometry 120x70 -e w3m %s &");
  combo2_items = g_list_append (combo2_items, (gpointer) "links -g %s &");
  combo2_items = g_list_append (combo2_items, (gpointer) "dillo %s &");
  gtk_combo_set_popdown_strings (GTK_COMBO (combo2), combo2_items);
  g_list_free (combo2_items);

  comboentry_browser = GTK_COMBO (combo2)->entry;
  gtk_widget_ref (comboentry_browser);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "comboentry_browser", comboentry_browser,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (comboentry_browser);
  gtk_entry_set_text (GTK_ENTRY (comboentry_browser), "galeon -n %s &");

  label66 = gtk_label_new ("Main browser (left clic on an url)");
  gtk_widget_ref (label66);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "label66", label66,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (label66);
  gtk_table_attach (GTK_TABLE (table14), label66, 0, 1, 1, 2,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_widget_set_usize (label66, 202, -2);
  gtk_label_set_line_wrap (GTK_LABEL (label66), TRUE);
  gtk_misc_set_alignment (GTK_MISC (label66), 0, 0.5);

  label67 = gtk_label_new ("Secondary browser (middle clic)");
  gtk_widget_ref (label67);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "label67", label67,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (label67);
  gtk_table_attach (GTK_TABLE (table14), label67, 0, 1, 2, 3,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_widget_set_usize (label67, 150, -2);
  gtk_misc_set_alignment (GTK_MISC (label67), 0, 0.5);

  frame14 = gtk_frame_new ("Gogole search");
  gtk_widget_ref (frame14);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "frame14", frame14,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (frame14);
  gtk_box_pack_start (GTK_BOX (vbox17), frame14, FALSE, TRUE, 0);
  gtk_container_set_border_width (GTK_CONTAINER (frame14), 5);

  entry_gogole_search = gtk_entry_new ();
  gtk_widget_ref (entry_gogole_search);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "entry_gogole_search", entry_gogole_search,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (entry_gogole_search);
  gtk_container_add (GTK_CONTAINER (frame14), entry_gogole_search);
  gtk_tooltips_set_tip (tooltips, entry_gogole_search, "You can look for a term on a search engine, by ALT+{left|middle} clic on the selected word in the pinnipede. Just enter the URL of your search engine, where the %s will be replaced by the searched term.", NULL);

  frame29 = gtk_frame_new ("Site settings");
  gtk_widget_ref (frame29);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "frame29", frame29,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (frame29);
  gtk_box_pack_end (GTK_BOX (vbox31), frame29, FALSE, TRUE, 0);
  gtk_container_set_border_width (GTK_CONTAINER (frame29), 5);
  gtk_frame_set_shadow_type (GTK_FRAME (frame29), GTK_SHADOW_OUT);

  vbox32 = gtk_vbox_new (FALSE, 0);
  gtk_widget_ref (vbox32);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "vbox32", vbox32,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (vbox32);
  gtk_container_add (GTK_CONTAINER (frame29), vbox32);

  frame30 = gtk_frame_new ("Proxy settings");
  gtk_widget_ref (frame30);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "frame30", frame30,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (frame30);
  gtk_box_pack_start (GTK_BOX (vbox32), frame30, TRUE, TRUE, 0);
  gtk_container_set_border_width (GTK_CONTAINER (frame30), 5);

  vbox33 = gtk_vbox_new (FALSE, 0);
  gtk_widget_ref (vbox33);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "vbox33", vbox33,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (vbox33);
  gtk_container_add (GTK_CONTAINER (frame30), vbox33);

  hbox22 = gtk_hbox_new (FALSE, 3);
  gtk_widget_ref (hbox22);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "hbox22", hbox22,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (hbox22);
  gtk_box_pack_start (GTK_BOX (vbox33), hbox22, TRUE, TRUE, 0);

  label129 = gtk_label_new ("Proxy:");
  gtk_widget_ref (label129);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "label129", label129,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (label129);
  gtk_box_pack_start (GTK_BOX (hbox22), label129, FALSE, FALSE, 0);

  entry_proxy = gtk_entry_new ();
  gtk_widget_ref (entry_proxy);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "entry_proxy", entry_proxy,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (entry_proxy);
  gtk_box_pack_start (GTK_BOX (hbox22), entry_proxy, FALSE, TRUE, 0);
  gtk_tooltips_set_tip (tooltips, entry_proxy, "Just type the name of your proxy or its IP adress", NULL);

  label130 = gtk_label_new ("Port:");
  gtk_widget_ref (label130);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "label130", label130,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (label130);
  gtk_box_pack_start (GTK_BOX (hbox22), label130, FALSE, FALSE, 0);

  spinbutton_proxy_port_adj = gtk_adjustment_new (1, 0, 65536, 1, 10, 10);
  spinbutton_proxy_port = gtk_spin_button_new (GTK_ADJUSTMENT (spinbutton_proxy_port_adj), 1, 0);
  gtk_widget_ref (spinbutton_proxy_port);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "spinbutton_proxy_port", spinbutton_proxy_port,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (spinbutton_proxy_port);
  gtk_box_pack_start (GTK_BOX (hbox22), spinbutton_proxy_port, FALSE, TRUE, 0);
  gtk_widget_set_usize (spinbutton_proxy_port, 60, -2);
  gtk_spin_button_set_numeric (GTK_SPIN_BUTTON (spinbutton_proxy_port), TRUE);
  gtk_spin_button_set_update_policy (GTK_SPIN_BUTTON (spinbutton_proxy_port), GTK_UPDATE_IF_VALID);

  label131 = gtk_label_new ("User:");
  gtk_widget_ref (label131);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "label131", label131,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (label131);
  gtk_box_pack_start (GTK_BOX (hbox22), label131, FALSE, FALSE, 0);

  entry_proxy_user = gtk_entry_new ();
  gtk_widget_ref (entry_proxy_user);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "entry_proxy_user", entry_proxy_user,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (entry_proxy_user);
  gtk_box_pack_start (GTK_BOX (hbox22), entry_proxy_user, FALSE, TRUE, 0);
  gtk_tooltips_set_tip (tooltips, entry_proxy_user, "wmcc only performs the so-called 'basic proxy authentification'", NULL);

  label132 = gtk_label_new ("Passwd:");
  gtk_widget_ref (label132);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "label132", label132,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (label132);
  gtk_box_pack_start (GTK_BOX (hbox22), label132, FALSE, FALSE, 0);

  entry_proxy_pass = gtk_entry_new ();
  gtk_widget_ref (entry_proxy_pass);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "entry_proxy_pass", entry_proxy_pass,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (entry_proxy_pass);
  gtk_box_pack_start (GTK_BOX (hbox22), entry_proxy_pass, FALSE, TRUE, 0);
  gtk_widget_set_usize (entry_proxy_pass, 80, -2);

  hbox23 = gtk_hbox_new (FALSE, 0);
  gtk_widget_ref (hbox23);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "hbox23", hbox23,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (hbox23);
  gtk_box_pack_start (GTK_BOX (vbox33), hbox23, TRUE, TRUE, 0);

  checkbutton_proxy_no_cache = gtk_check_button_new_with_label ("Disable proxy cache");
  gtk_widget_ref (checkbutton_proxy_no_cache);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "checkbutton_proxy_no_cache", checkbutton_proxy_no_cache,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (checkbutton_proxy_no_cache);
  gtk_box_pack_start (GTK_BOX (hbox23), checkbutton_proxy_no_cache, TRUE, FALSE, 0);
  gtk_tooltips_set_tip (tooltips, checkbutton_proxy_no_cache, "Inserts 'Pragma: no-cache\" and \"Cache-Control: no cache in http requests. This might be useful even if you don't use a proxy, but your provider uses a transparent proxy (try this option if you find that there is a great 'lag' on the boards)", NULL);

  checkbutton_proxy_disable_if_modified_since = gtk_check_button_new_with_label ("Use 'if-modified-since' header");
  gtk_widget_ref (checkbutton_proxy_disable_if_modified_since);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "checkbutton_proxy_disable_if_modified_since", checkbutton_proxy_disable_if_modified_since,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (checkbutton_proxy_disable_if_modified_since);
  gtk_box_pack_start (GTK_BOX (hbox23), checkbutton_proxy_disable_if_modified_since, TRUE, FALSE, 0);
  gtk_tooltips_set_tip (tooltips, checkbutton_proxy_disable_if_modified_since, "Using the 'if-modified-since' header, wmcc downloads data only where the remote server reports it has been modified. If you experience lag problems, try disabling this feature)", NULL);

  frame31 = gtk_frame_new ("Backends");
  gtk_widget_ref (frame31);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "frame31", frame31,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (frame31);
  gtk_box_pack_start (GTK_BOX (vbox32), frame31, TRUE, TRUE, 0);
  gtk_container_set_border_width (GTK_CONTAINER (frame31), 5);

  table24 = gtk_table_new (4, 4, FALSE);
  gtk_widget_ref (table24);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "table24", table24,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (table24);
  gtk_container_add (GTK_CONTAINER (frame31), table24);
  gtk_table_set_col_spacings (GTK_TABLE (table24), 4);

  label134 = gtk_label_new ("Board:");
  gtk_widget_ref (label134);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "label134", label134,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (label134);
  gtk_table_attach (GTK_TABLE (table24), label134, 0, 1, 0, 1,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_misc_set_alignment (GTK_MISC (label134), 0, 0.5);

  label135 = gtk_label_new ("News:");
  gtk_widget_ref (label135);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "label135", label135,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (label135);
  gtk_table_attach (GTK_TABLE (table24), label135, 0, 1, 2, 3,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_misc_set_alignment (GTK_MISC (label135), 0, 0.5);

  label136 = gtk_label_new ("Comments:");
  gtk_widget_ref (label136);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "label136", label136,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (label136);
  gtk_table_attach (GTK_TABLE (table24), label136, 0, 1, 3, 4,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_misc_set_alignment (GTK_MISC (label136), 0, 0.5);

  entry_path_news_backend = gtk_entry_new ();
  gtk_widget_ref (entry_path_news_backend);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "entry_path_news_backend", entry_path_news_backend,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (entry_path_news_backend);
  gtk_table_attach (GTK_TABLE (table24), entry_path_news_backend, 1, 2, 2, 3,
                    (GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_tooltips_set_tip (tooltips, entry_path_news_backend, "path to the news backend (usually 'backend.rdf')", NULL);

  entry_path_myposts = gtk_entry_new ();
  gtk_widget_ref (entry_path_myposts);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "entry_path_myposts", entry_path_myposts,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (entry_path_myposts);
  gtk_table_attach (GTK_TABLE (table24), entry_path_myposts, 1, 2, 3, 4,
                    (GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_tooltips_set_tip (tooltips, entry_path_myposts, "path to the 'your comments' page , usually 'users/posts.php3?order=id'", NULL);

  label138 = gtk_label_new ("News ext.:");
  gtk_widget_ref (label138);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "label138", label138,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (label138);
  gtk_table_attach (GTK_TABLE (table24), label138, 2, 3, 2, 3,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_misc_set_alignment (GTK_MISC (label138), 0, 0.5);

  entry_end_news_url = gtk_entry_new ();
  gtk_widget_ref (entry_end_news_url);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "entry_end_news_url", entry_end_news_url,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (entry_end_news_url);
  gtk_table_attach (GTK_TABLE (table24), entry_end_news_url, 3, 4, 2, 3,
                    (GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_tooltips_set_tip (tooltips, entry_end_news_url, "this is the end of the news urls, such that its text is downloaded on a suitable (and parsable) dacode theme (linuxfr 'printable' theme is the best)", NULL);

  label139 = gtk_label_new ("Messages:");
  gtk_widget_ref (label139);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "label139", label139,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (label139);
  gtk_table_attach (GTK_TABLE (table24), label139, 2, 3, 3, 4,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_misc_set_alignment (GTK_MISC (label139), 0, 0.5);

  entry_path_messages = gtk_entry_new ();
  gtk_widget_ref (entry_path_messages);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "entry_path_messages", entry_path_messages,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (entry_path_messages);
  gtk_table_attach (GTK_TABLE (table24), entry_path_messages, 3, 4, 3, 4,
                    (GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_tooltips_set_tip (tooltips, entry_path_messages, "path to the messages pages (usually 'messages/')", NULL);

  label170 = gtk_label_new ("Board post url:");
  gtk_widget_ref (label170);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "label170", label170,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (label170);
  gtk_table_attach (GTK_TABLE (table24), label170, 0, 1, 1, 2,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_misc_set_alignment (GTK_MISC (label170), 0, 0.5);

  entry_path_board_add = gtk_entry_new ();
  gtk_widget_ref (entry_path_board_add);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "entry_path_board_add", entry_path_board_add,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (entry_path_board_add);
  gtk_table_attach (GTK_TABLE (table24), entry_path_board_add, 1, 2, 1, 2,
                    (GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_tooltips_set_tip (tooltips, entry_path_board_add, "usually board/add.php3 ( just board.html on new.linuxfr.org)", NULL);

  label137 = gtk_label_new ("Board post data:");
  gtk_widget_ref (label137);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "label137", label137,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (label137);
  gtk_table_attach (GTK_TABLE (table24), label137, 2, 3, 1, 2,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_misc_set_alignment (GTK_MISC (label137), 0, 0.5);

  entry_board_post = gtk_entry_new ();
  gtk_widget_ref (entry_board_post);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "entry_board_post", entry_board_post,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (entry_board_post);
  gtk_table_attach (GTK_TABLE (table24), entry_board_post, 3, 4, 1, 2,
                    (GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_tooltips_set_tip (tooltips, entry_board_post, "what to send to post the message (the %s is replace by the message content) (usually 'message=%s', or message=%s&section=1", NULL);

  optionmenu_backend_type = gtk_option_menu_new ();
  gtk_widget_ref (optionmenu_backend_type);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "optionmenu_backend_type", optionmenu_backend_type,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (optionmenu_backend_type);
  gtk_table_attach (GTK_TABLE (table24), optionmenu_backend_type, 3, 4, 0, 1,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  optionmenu_backend_type_menu = gtk_menu_new ();
  glade_menuitem = gtk_menu_item_new_with_label ("tags not encoded");
  gtk_widget_show (glade_menuitem);
  gtk_menu_append (GTK_MENU (optionmenu_backend_type_menu), glade_menuitem);
  glade_menuitem = gtk_menu_item_new_with_label ("tags encodeds");
  gtk_widget_show (glade_menuitem);
  gtk_menu_append (GTK_MENU (optionmenu_backend_type_menu), glade_menuitem);
  glade_menuitem = gtk_menu_item_new_with_label ("without pants");
  gtk_widget_show (glade_menuitem);
  gtk_menu_append (GTK_MENU (optionmenu_backend_type_menu), glade_menuitem);
  gtk_option_menu_set_menu (GTK_OPTION_MENU (optionmenu_backend_type), optionmenu_backend_type_menu);

  label171 = gtk_label_new ("Backend type:");
  gtk_widget_ref (label171);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "label171", label171,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (label171);
  gtk_table_attach (GTK_TABLE (table24), label171, 2, 3, 0, 1,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_misc_set_alignment (GTK_MISC (label171), 0, 0.5);

  entry_path_board_backend = gtk_entry_new ();
  gtk_widget_ref (entry_path_board_backend);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "entry_path_board_backend", entry_path_board_backend,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (entry_path_board_backend);
  gtk_table_attach (GTK_TABLE (table24), entry_path_board_backend, 1, 2, 0, 1,
                    (GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_tooltips_set_tip (tooltips, entry_path_board_backend, "path to the board backend (usually 'board/remote.xml')", NULL);

  label42 = gtk_label_new ("Http");
  gtk_widget_ref (label42);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "label42", label42,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (label42);
  gtk_notebook_set_tab_label (GTK_NOTEBOOK (notebook1), gtk_notebook_get_nth_page (GTK_NOTEBOOK (notebook1), 7), label42);

  hbox162 = gtk_hbox_new (FALSE, 0);
  gtk_widget_ref (hbox162);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "hbox162", hbox162,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (hbox162);
  gtk_box_pack_start (GTK_BOX (vbox1), hbox162, FALSE, FALSE, 0);

  notebook_sites = gtk_notebook_new ();
  gtk_widget_ref (notebook_sites);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "notebook_sites", notebook_sites,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (notebook_sites);
  gtk_box_pack_start (GTK_BOX (hbox162), notebook_sites, TRUE, TRUE, 0);
  gtk_widget_set_usize (notebook_sites, -2, 30);
  gtk_notebook_set_show_border (GTK_NOTEBOOK (notebook_sites), FALSE);
  gtk_notebook_set_tab_pos (GTK_NOTEBOOK (notebook_sites), GTK_POS_BOTTOM);
  gtk_notebook_set_scrollable (GTK_NOTEBOOK (notebook_sites), TRUE);

  empty_notebook_page = gtk_vbox_new (FALSE, 0);
  gtk_widget_show (empty_notebook_page);
  gtk_container_add (GTK_CONTAINER (notebook_sites), empty_notebook_page);

  label142 = gtk_label_new ("label142");
  gtk_widget_ref (label142);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "label142", label142,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (label142);
  gtk_notebook_set_tab_label (GTK_NOTEBOOK (notebook_sites), gtk_notebook_get_nth_page (GTK_NOTEBOOK (notebook_sites), 0), label142);

  empty_notebook_page = gtk_vbox_new (FALSE, 0);
  gtk_widget_show (empty_notebook_page);
  gtk_container_add (GTK_CONTAINER (notebook_sites), empty_notebook_page);

  label143 = gtk_label_new ("label143");
  gtk_widget_ref (label143);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "label143", label143,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (label143);
  gtk_notebook_set_tab_label (GTK_NOTEBOOK (notebook_sites), gtk_notebook_get_nth_page (GTK_NOTEBOOK (notebook_sites), 1), label143);

  bt_mv_site_left = gtk_button_new_with_label ("<-");
  gtk_widget_ref (bt_mv_site_left);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "bt_mv_site_left", bt_mv_site_left,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (bt_mv_site_left);
  gtk_box_pack_start (GTK_BOX (hbox162), bt_mv_site_left, FALSE, FALSE, 0);

  bt_mv_site_right = gtk_button_new_with_label ("->");
  gtk_widget_ref (bt_mv_site_right);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "bt_mv_site_right", bt_mv_site_right,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (bt_mv_site_right);
  gtk_box_pack_start (GTK_BOX (hbox162), bt_mv_site_right, FALSE, FALSE, 0);

  bt_new_site = gtk_button_new_with_label ("New site");
  gtk_widget_ref (bt_new_site);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "bt_new_site", bt_new_site,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (bt_new_site);
  gtk_box_pack_start (GTK_BOX (hbox162), bt_new_site, FALSE, FALSE, 0);

  bt_remove_site = gtk_button_new_with_label ("Delete site");
  gtk_widget_ref (bt_remove_site);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "bt_remove_site", bt_remove_site,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (bt_remove_site);
  gtk_box_pack_start (GTK_BOX (hbox162), bt_remove_site, FALSE, FALSE, 0);

  vbox23 = gtk_vbox_new (FALSE, 0);
  gtk_widget_ref (vbox23);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "vbox23", vbox23,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (vbox23);
  gtk_box_pack_start (GTK_BOX (hbox160), vbox23, FALSE, TRUE, 0);

  pixmap2 = create_pixmap (main_win, "logo.xpm");
  gtk_widget_ref (pixmap2);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "pixmap2", pixmap2,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (pixmap2);
  gtk_box_pack_start (GTK_BOX (vbox23), pixmap2, FALSE, FALSE, 0);

  bt_save = gtk_button_new_with_label ("Save");
  gtk_widget_ref (bt_save);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "bt_save", bt_save,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (bt_save);
  gtk_box_pack_start (GTK_BOX (vbox23), bt_save, FALSE, FALSE, 0);
  gtk_tooltips_set_tip (tooltips, bt_save, "Press OK to commit the changes and quit", NULL);

  bt_save_as = gtk_button_new_with_label ("Save As");
  gtk_widget_ref (bt_save_as);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "bt_save_as", bt_save_as,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (bt_save_as);
  gtk_box_pack_start (GTK_BOX (vbox23), bt_save_as, FALSE, FALSE, 0);

  bt_apply = gtk_button_new_with_label ("Apply");
  gtk_widget_ref (bt_apply);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "bt_apply", bt_apply,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (bt_apply);
  gtk_box_pack_start (GTK_BOX (vbox23), bt_apply, FALSE, FALSE, 0);
  gtk_tooltips_set_tip (tooltips, bt_apply, "Restore default values", NULL);

  bt_cancel = gtk_button_new_with_label ("Cancel");
  gtk_widget_ref (bt_cancel);
  gtk_object_set_data_full (GTK_OBJECT (main_win), "bt_cancel", bt_cancel,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (bt_cancel);
  gtk_box_pack_start (GTK_BOX (vbox23), bt_cancel, FALSE, FALSE, 0);
  gtk_tooltips_set_tip (tooltips, bt_cancel, "Cancel the changes and exit immediatly", NULL);

  gtk_signal_connect (GTK_OBJECT (main_win), "delete_event",
                      GTK_SIGNAL_FUNC (on_main_win_delete_event),
                      NULL);
  gtk_signal_connect (GTK_OBJECT (bt_dock_skinpixmap), "clicked",
                      GTK_SIGNAL_FUNC (on_bt_pixmap_clicked),
                      NULL);
  gtk_signal_connect (GTK_OBJECT (bt_dock_fgcolor), "clicked",
                      GTK_SIGNAL_FUNC (on_bt_color_clicked),
                      NULL);
  gtk_signal_connect (GTK_OBJECT (bt_dock_bgcolor), "clicked",
                      GTK_SIGNAL_FUNC (on_bt_color_clicked),
                      NULL);
  gtk_signal_connect (GTK_OBJECT (bt_dock_bgpixmap), "clicked",
                      GTK_SIGNAL_FUNC (on_bt_pixmap_clicked),
                      NULL);
  gtk_signal_connect (GTK_OBJECT (checkbutton_dock_use_iconwin), "toggled",
                      GTK_SIGNAL_FUNC (on_checkbutton_toggled),
                      NULL);
  gtk_signal_connect (GTK_OBJECT (checkbutton_dock_draw_border), "toggled",
                      GTK_SIGNAL_FUNC (on_checkbutton_toggled),
                      NULL);
  gtk_signal_connect (GTK_OBJECT (checkbutton_dock_use_balloons), "toggled",
                      GTK_SIGNAL_FUNC (on_checkbutton_toggled),
                      NULL);
  gtk_signal_connect (GTK_OBJECT (checkbutton_start_in_boss_mode), "toggled",
                      GTK_SIGNAL_FUNC (on_checkbutton_toggled),
                      NULL);
  gtk_signal_connect (GTK_OBJECT (checkbutton_site_board), "toggled",
                      GTK_SIGNAL_FUNC (on_checkbutton_toggled),
                      NULL);
  gtk_signal_connect (GTK_OBJECT (checkbutton_site_news), "toggled",
                      GTK_SIGNAL_FUNC (on_checkbutton_toggled),
                      NULL);
  gtk_signal_connect (GTK_OBJECT (checkbutton_site_messages), "toggled",
                      GTK_SIGNAL_FUNC (on_checkbutton_toggled),
                      NULL);
  gtk_signal_connect (GTK_OBJECT (checkbutton_site_comments), "toggled",
                      GTK_SIGNAL_FUNC (on_checkbutton_toggled),
                      NULL);
  gtk_signal_connect (GTK_OBJECT (entry_site_name), "changed",
                      GTK_SIGNAL_FUNC (on_entry_sitename_changed),
                      NULL);
  gtk_signal_connect (GTK_OBJECT (entry_site_root_url), "changed",
                      GTK_SIGNAL_FUNC (on_editable_changed),
                      NULL);
  gtk_signal_connect (GTK_OBJECT (text_site_cookie), "changed",
                      GTK_SIGNAL_FUNC (on_editable_changed),
                      NULL);
  gtk_signal_connect (GTK_OBJECT (entry_user_login), "changed",
                      GTK_SIGNAL_FUNC (on_editable_changed),
                      NULL);
  gtk_signal_connect (GTK_OBJECT (entry_site_alias1), "changed",
                      GTK_SIGNAL_FUNC (on_editable_changed),
                      NULL);
  gtk_signal_connect (GTK_OBJECT (entry_site_alias2), "changed",
                      GTK_SIGNAL_FUNC (on_editable_changed),
                      NULL);
  gtk_signal_connect (GTK_OBJECT (entry_site_alias3), "changed",
                      GTK_SIGNAL_FUNC (on_editable_changed),
                      NULL);
  gtk_signal_connect (GTK_OBJECT (bt_news_bgcolor), "clicked",
                      GTK_SIGNAL_FUNC (on_bt_color_clicked),
                      NULL);
  gtk_signal_connect (GTK_OBJECT (bt_news_fgcolor), "clicked",
                      GTK_SIGNAL_FUNC (on_bt_color_clicked),
                      NULL);
  gtk_signal_connect (GTK_OBJECT (bt_news_titles_bgcolor), "clicked",
                      GTK_SIGNAL_FUNC (on_bt_color_clicked),
                      NULL);
  gtk_signal_connect (GTK_OBJECT (bt_news_titles_fgcolor), "clicked",
                      GTK_SIGNAL_FUNC (on_bt_color_clicked),
                      NULL);
  gtk_signal_connect (GTK_OBJECT (bt_news_emph_color), "clicked",
                      GTK_SIGNAL_FUNC (on_bt_color_clicked),
                      NULL);
  gtk_signal_connect (GTK_OBJECT (comboentry_news_fnfamily), "changed",
                      GTK_SIGNAL_FUNC (on_editable_changed),
                      NULL);
  gtk_signal_connect (GTK_OBJECT (spinbutton_news_fnsize), "changed",
                      GTK_SIGNAL_FUNC (on_spinbutton_changed),
                      NULL);
  gtk_signal_connect (GTK_OBJECT (spinbt_switch_off_coincoin_delay), "changed",
                      GTK_SIGNAL_FUNC (on_spinbutton_changed),
                      NULL);
  gtk_signal_connect (GTK_OBJECT (spinbtmax_refresh_delay), "changed",
                      GTK_SIGNAL_FUNC (on_spinbutton_changed),
                      NULL);
  gtk_signal_connect (GTK_OBJECT (bt_sc_bg_color), "clicked",
                      GTK_SIGNAL_FUNC (on_bt_color_clicked),
                      NULL);
  gtk_signal_connect (GTK_OBJECT (bt_sc_bg_light_color), "clicked",
                      GTK_SIGNAL_FUNC (on_bt_color_clicked),
                      NULL);
  gtk_signal_connect (GTK_OBJECT (bt_sc_bg_dark_color), "clicked",
                      GTK_SIGNAL_FUNC (on_bt_color_clicked),
                      NULL);
  gtk_signal_connect (GTK_OBJECT (bt_sc_arrow_normal_color), "clicked",
                      GTK_SIGNAL_FUNC (on_bt_color_clicked),
                      NULL);
  gtk_signal_connect (GTK_OBJECT (bt_sc_bar_color), "clicked",
                      GTK_SIGNAL_FUNC (on_bt_color_clicked),
                      NULL);
  gtk_signal_connect (GTK_OBJECT (bt_sc_bar_light_color), "clicked",
                      GTK_SIGNAL_FUNC (on_bt_color_clicked),
                      NULL);
  gtk_signal_connect (GTK_OBJECT (bt_sc_bar_dark_color), "clicked",
                      GTK_SIGNAL_FUNC (on_bt_color_clicked),
                      NULL);
  gtk_signal_connect (GTK_OBJECT (bt_sc_arrow_emphasized_color), "clicked",
                      GTK_SIGNAL_FUNC (on_bt_color_clicked),
                      NULL);
  gtk_signal_connect (GTK_OBJECT (bt_sc_bar_color_trans), "clicked",
                      GTK_SIGNAL_FUNC (on_bt_color_clicked),
                      NULL);
  gtk_signal_connect (GTK_OBJECT (bt_sc_bar_light_color_trans), "clicked",
                      GTK_SIGNAL_FUNC (on_bt_color_clicked),
                      NULL);
  gtk_signal_connect (GTK_OBJECT (bt_sc_bar_dark_color_trans), "clicked",
                      GTK_SIGNAL_FUNC (on_bt_color_clicked),
                      NULL);
  gtk_signal_connect (GTK_OBJECT (bt_sc_arrow_emphasized_color_trans), "clicked",
                      GTK_SIGNAL_FUNC (on_bt_color_clicked),
                      NULL);
  gtk_signal_connect (GTK_OBJECT (bt_sc_bg_color_trans), "clicked",
                      GTK_SIGNAL_FUNC (on_bt_color_clicked),
                      NULL);
  gtk_signal_connect (GTK_OBJECT (bt_sc_bg_light_color_trans), "clicked",
                      GTK_SIGNAL_FUNC (on_bt_color_clicked),
                      NULL);
  gtk_signal_connect (GTK_OBJECT (bt_sc_bg_dark_color_trans), "clicked",
                      GTK_SIGNAL_FUNC (on_bt_color_clicked),
                      NULL);
  gtk_signal_connect (GTK_OBJECT (bt_sc_arrow_normal_color_trans), "clicked",
                      GTK_SIGNAL_FUNC (on_bt_color_clicked),
                      NULL);
  gtk_signal_connect (GTK_OBJECT (spinbutton_news_delay), "changed",
                      GTK_SIGNAL_FUNC (on_spinbutton_changed),
                      NULL);
  gtk_signal_connect (GTK_OBJECT (spinbutton_news_max_age), "changed",
                      GTK_SIGNAL_FUNC (on_spinbutton_changed),
                      NULL);
  gtk_signal_connect (GTK_OBJECT (checkbutton_trolldetector), "toggled",
                      GTK_SIGNAL_FUNC (on_checkbutton_toggled),
                      NULL);
  gtk_signal_connect (GTK_OBJECT (text_post_cmd), "changed",
                      GTK_SIGNAL_FUNC (on_editable_changed),
                      NULL);
  gtk_signal_connect (GTK_OBJECT (entry_archive_filename), "changed",
                      GTK_SIGNAL_FUNC (on_editable_changed),
                      NULL);
  gtk_signal_connect (GTK_OBJECT (clist_miniua), "select_row",
                      GTK_SIGNAL_FUNC (on_clist_miniua_select_row),
                      NULL);
  gtk_signal_connect (GTK_OBJECT (bt_mua_change), "clicked",
                      GTK_SIGNAL_FUNC (on_bt_mua_change_clicked),
                      NULL);
  gtk_signal_connect (GTK_OBJECT (bt_mua_add), "clicked",
                      GTK_SIGNAL_FUNC (on_bt_mua_add_clicked),
                      NULL);
  gtk_signal_connect (GTK_OBJECT (bt_mua_delete), "clicked",
                      GTK_SIGNAL_FUNC (on_bt_mua_delete_clicked),
                      NULL);
  gtk_signal_connect (GTK_OBJECT (spinbutton_board_delay), "changed",
                      GTK_SIGNAL_FUNC (on_spinbutton_changed),
                      NULL);
  gtk_signal_connect (GTK_OBJECT (spinbutton_board_max_msg), "changed",
                      GTK_SIGNAL_FUNC (on_spinbutton_changed),
                      NULL);
  gtk_signal_connect (GTK_OBJECT (combo_entry_board_wiki), "changed",
                      GTK_SIGNAL_FUNC (on_editable_changed),
                      NULL);
  gtk_signal_connect (GTK_OBJECT (checkbutton_pp_use_AMPM), "toggled",
                      GTK_SIGNAL_FUNC (on_checkbutton_toggled),
                      NULL);
  gtk_signal_connect (GTK_OBJECT (checkbutton_board_auto_refresh), "toggled",
                      GTK_SIGNAL_FUNC (on_checkbutton_toggled),
                      NULL);
  gtk_signal_connect (GTK_OBJECT (entry_coin_coin_message), "changed",
                      GTK_SIGNAL_FUNC (on_editable_changed),
                      NULL);
  gtk_signal_connect (GTK_OBJECT (checkbutton_enable_spell), "toggled",
                      GTK_SIGNAL_FUNC (on_checkbutton_toggled),
                      NULL);
  gtk_signal_connect (GTK_OBJECT (entry_spell_dict), "changed",
                      GTK_SIGNAL_FUNC (on_editable_changed),
                      NULL);
  gtk_signal_connect (GTK_OBJECT (entry_spell_cmd), "changed",
                      GTK_SIGNAL_FUNC (on_editable_changed),
                      NULL);
  gtk_signal_connect (GTK_OBJECT (spinbutton_msg_max_length), "changed",
                      GTK_SIGNAL_FUNC (on_spinbutton_changed),
                      NULL);
  gtk_signal_connect (GTK_OBJECT (spinbutton_ua_max_length), "changed",
                      GTK_SIGNAL_FUNC (on_spinbutton_changed),
                      NULL);
  gtk_signal_connect (GTK_OBJECT (entry_default_ua), "changed",
                      GTK_SIGNAL_FUNC (on_editable_changed),
                      NULL);
  gtk_signal_connect (GTK_OBJECT (button_reset_ua), "clicked",
                      GTK_SIGNAL_FUNC (on_button_reset_ua_clicked),
                      NULL);
  gtk_signal_connect (GTK_OBJECT (checkbutton_pp_auto_open), "toggled",
                      GTK_SIGNAL_FUNC (on_checkbutton_toggled),
                      NULL);
  gtk_signal_connect (GTK_OBJECT (checkbutton_pp_use_classical_tabs), "toggled",
                      GTK_SIGNAL_FUNC (on_checkbutton_toggled),
                      NULL);
  gtk_signal_connect (GTK_OBJECT (checkbutton_pp_hungry_boitakon), "toggled",
                      GTK_SIGNAL_FUNC (on_checkbutton_toggled),
                      NULL);
  gtk_signal_connect (GTK_OBJECT (checkbutton_start_in_transparency), "toggled",
                      GTK_SIGNAL_FUNC (on_checkbutton_toggled),
                      NULL);
  gtk_signal_connect (GTK_OBJECT (checkbutton_use_fake_real_transparency), "toggled",
                      GTK_SIGNAL_FUNC (on_checkbutton_toggled),
                      NULL);
  gtk_signal_connect (GTK_OBJECT (bt_transp_black), "clicked",
                      GTK_SIGNAL_FUNC (on_bt_color_clicked),
                      NULL);
  gtk_signal_connect (GTK_OBJECT (bt_transp_white), "clicked",
                      GTK_SIGNAL_FUNC (on_bt_color_clicked),
                      NULL);
  gtk_signal_connect (GTK_OBJECT (spinbutton_transp_shade_max), "changed",
                      GTK_SIGNAL_FUNC (on_spinbutton_changed),
                      NULL);
  gtk_signal_connect (GTK_OBJECT (spinbutton_transp_shade_min), "changed",
                      GTK_SIGNAL_FUNC (on_spinbutton_changed),
                      NULL);
  gtk_signal_connect (GTK_OBJECT (comboentry_pp_fnfamily), "changed",
                      GTK_SIGNAL_FUNC (on_editable_changed),
                      NULL);
  gtk_signal_connect (GTK_OBJECT (spinbutton_pp_fnsize), "changed",
                      GTK_SIGNAL_FUNC (on_spinbutton_changed),
                      NULL);
  gtk_signal_connect (GTK_OBJECT (bt_pp_emph_color), "clicked",
                      GTK_SIGNAL_FUNC (on_bt_color_clicked),
                      NULL);
  gtk_signal_connect (GTK_OBJECT (bt_pp_sel_color), "clicked",
                      GTK_SIGNAL_FUNC (on_bt_color_clicked),
                      NULL);
  gtk_signal_connect (GTK_OBJECT (bt_pp_emph_color_trans), "clicked",
                      GTK_SIGNAL_FUNC (on_bt_color_clicked),
                      NULL);
  gtk_signal_connect (GTK_OBJECT (bt_pp_sel_color_trans), "clicked",
                      GTK_SIGNAL_FUNC (on_bt_color_clicked),
                      NULL);
  gtk_signal_connect (GTK_OBJECT (bt_pp_popup_fgcolor), "clicked",
                      GTK_SIGNAL_FUNC (on_bt_color_clicked),
                      NULL);
  gtk_signal_connect (GTK_OBJECT (bt_pp_popup_bgcolor), "clicked",
                      GTK_SIGNAL_FUNC (on_bt_color_clicked),
                      NULL);
  gtk_signal_connect (GTK_OBJECT (bt_pp_popup_bgcolor_trans), "clicked",
                      GTK_SIGNAL_FUNC (on_bt_color_clicked),
                      NULL);
  gtk_signal_connect (GTK_OBJECT (bt_pp_popup_fgcolor_trans), "clicked",
                      GTK_SIGNAL_FUNC (on_bt_color_clicked),
                      NULL);
  gtk_signal_connect (GTK_OBJECT (checkbutton_show_seconds), "toggled",
                      GTK_SIGNAL_FUNC (on_checkbutton_toggled),
                      NULL);
  gtk_signal_connect (GTK_OBJECT (checkbutton_show_troll_score), "toggled",
                      GTK_SIGNAL_FUNC (on_checkbutton_toggled),
                      NULL);
  gtk_signal_connect (GTK_OBJECT (checkbutton_show_fortune), "toggled",
                      GTK_SIGNAL_FUNC (on_checkbutton_toggled),
                      NULL);
  gtk_signal_connect (GTK_OBJECT (checkbutton_show_tags), "toggled",
                      GTK_SIGNAL_FUNC (on_checkbutton_toggled),
                      NULL);
  gtk_signal_connect (GTK_OBJECT (checkbutton_mark_id_gaps), "toggled",
                      GTK_SIGNAL_FUNC (on_checkbutton_toggled),
                      NULL);
  gtk_signal_connect (GTK_OBJECT (bt_pp_bgcolor), "clicked",
                      GTK_SIGNAL_FUNC (on_bt_color_clicked),
                      NULL);
  gtk_signal_connect (GTK_OBJECT (bt_pp_fgcolor), "clicked",
                      GTK_SIGNAL_FUNC (on_bt_color_clicked),
                      NULL);
  gtk_signal_connect (GTK_OBJECT (bt_pp_fgcolor_trans), "clicked",
                      GTK_SIGNAL_FUNC (on_bt_color_clicked),
                      NULL);
  gtk_signal_connect (GTK_OBJECT (bt_pp_clock_color), "clicked",
                      GTK_SIGNAL_FUNC (on_bt_color_clicked),
                      NULL);
  gtk_signal_connect (GTK_OBJECT (bt_pp_clock_color_trans), "clicked",
                      GTK_SIGNAL_FUNC (on_bt_color_clicked),
                      NULL);
  gtk_signal_connect (GTK_OBJECT (bt_pp_login_color_trans), "clicked",
                      GTK_SIGNAL_FUNC (on_bt_color_clicked),
                      NULL);
  gtk_signal_connect (GTK_OBJECT (bt_pp_ua_color), "clicked",
                      GTK_SIGNAL_FUNC (on_bt_color_clicked),
                      NULL);
  gtk_signal_connect (GTK_OBJECT (bt_pp_ua_color_trans), "clicked",
                      GTK_SIGNAL_FUNC (on_bt_color_clicked),
                      NULL);
  gtk_signal_connect (GTK_OBJECT (bt_pp_trollscore_color), "clicked",
                      GTK_SIGNAL_FUNC (on_bt_color_clicked),
                      NULL);
  gtk_signal_connect (GTK_OBJECT (bt_pp_trollscore_color_trans), "clicked",
                      GTK_SIGNAL_FUNC (on_bt_color_clicked),
                      NULL);
  gtk_signal_connect (GTK_OBJECT (bt_pp_login_color), "clicked",
                      GTK_SIGNAL_FUNC (on_bt_color_clicked),
                      NULL);
  gtk_signal_connect (GTK_OBJECT (bt_pp_url_color), "clicked",
                      GTK_SIGNAL_FUNC (on_bt_color_clicked),
                      NULL);
  gtk_signal_connect (GTK_OBJECT (bt_pp_url_color_trans), "clicked",
                      GTK_SIGNAL_FUNC (on_bt_color_clicked),
                      NULL);
  gtk_signal_connect (GTK_OBJECT (bt_pp_visited_url_color), "clicked",
                      GTK_SIGNAL_FUNC (on_bt_color_clicked),
                      NULL);
  gtk_signal_connect (GTK_OBJECT (bt_pp_visited_url_color_trans), "clicked",
                      GTK_SIGNAL_FUNC (on_bt_color_clicked),
                      NULL);
  gtk_signal_connect (GTK_OBJECT (bt_pp_strike_color), "clicked",
                      GTK_SIGNAL_FUNC (on_bt_color_clicked),
                      NULL);
  gtk_signal_connect (GTK_OBJECT (bt_pp_strike_color_trans), "clicked",
                      GTK_SIGNAL_FUNC (on_bt_color_clicked),
                      NULL);
  gtk_signal_connect (GTK_OBJECT (bt_pp_bb_fgcolor), "clicked",
                      GTK_SIGNAL_FUNC (on_bt_color_clicked),
                      NULL);
  gtk_signal_connect (GTK_OBJECT (bt_pp_bb_fgcolor_trans), "clicked",
                      GTK_SIGNAL_FUNC (on_bt_color_clicked),
                      NULL);
  gtk_signal_connect (GTK_OBJECT (bt_pp_bb_bgcolor_trans), "clicked",
                      GTK_SIGNAL_FUNC (on_bt_color_clicked),
                      NULL);
  gtk_signal_connect (GTK_OBJECT (bt_pp_bb_bgcolor), "clicked",
                      GTK_SIGNAL_FUNC (on_bt_color_clicked),
                      NULL);
  gtk_signal_connect (GTK_OBJECT (checkbutton_pp_minibar_on), "toggled",
                      GTK_SIGNAL_FUNC (on_checkbutton_toggled),
                      NULL);
  gtk_signal_connect (GTK_OBJECT (bt_pp_bb_progressbar_color), "clicked",
                      GTK_SIGNAL_FUNC (on_bt_color_clicked),
                      NULL);
  gtk_signal_connect (GTK_OBJECT (bt_pp_bb_updlcnt_color), "clicked",
                      GTK_SIGNAL_FUNC (on_bt_color_clicked),
                      NULL);
  gtk_signal_connect (GTK_OBJECT (bt_pp_bb_msgcnt_color), "clicked",
                      GTK_SIGNAL_FUNC (on_bt_color_clicked),
                      NULL);
  gtk_signal_connect (GTK_OBJECT (bt_pp_bb_progressbar_color_trans), "clicked",
                      GTK_SIGNAL_FUNC (on_bt_color_clicked),
                      NULL);
  gtk_signal_connect (GTK_OBJECT (bt_pp_bb_updlcnt_color_trans), "clicked",
                      GTK_SIGNAL_FUNC (on_bt_color_clicked),
                      NULL);
  gtk_signal_connect (GTK_OBJECT (bt_pp_bb_msgcnt_color_trans), "clicked",
                      GTK_SIGNAL_FUNC (on_bt_color_clicked),
                      NULL);
  gtk_signal_connect (GTK_OBJECT (bt_pp_my_messages), "clicked",
                      GTK_SIGNAL_FUNC (on_bt_color_clicked),
                      NULL);
  gtk_signal_connect (GTK_OBJECT (bt_pp_my_messages_trans), "clicked",
                      GTK_SIGNAL_FUNC (on_bt_color_clicked),
                      NULL);
  gtk_signal_connect (GTK_OBJECT (bt_pp_answers_to_my_messages), "clicked",
                      GTK_SIGNAL_FUNC (on_bt_color_clicked),
                      NULL);
  gtk_signal_connect (GTK_OBJECT (bt_pp_answers_to_my_messages_trans), "clicked",
                      GTK_SIGNAL_FUNC (on_bt_color_clicked),
                      NULL);
  gtk_signal_connect (GTK_OBJECT (bt_pp_hilight_keyword_color0), "clicked",
                      GTK_SIGNAL_FUNC (on_bt_color_clicked),
                      NULL);
  gtk_signal_connect (GTK_OBJECT (bt_pp_hilight_keyword_color2), "clicked",
                      GTK_SIGNAL_FUNC (on_bt_color_clicked),
                      NULL);
  gtk_signal_connect (GTK_OBJECT (bt_pp_hilight_keyword_color3), "clicked",
                      GTK_SIGNAL_FUNC (on_bt_color_clicked),
                      NULL);
  gtk_signal_connect (GTK_OBJECT (bt_pp_hilight_keyword_color4), "clicked",
                      GTK_SIGNAL_FUNC (on_bt_color_clicked),
                      NULL);
  gtk_signal_connect (GTK_OBJECT (bt_pp_hilight_keyword_color0_trans), "clicked",
                      GTK_SIGNAL_FUNC (on_bt_color_clicked),
                      NULL);
  gtk_signal_connect (GTK_OBJECT (bt_pp_hilight_keyword_color1_trans), "clicked",
                      GTK_SIGNAL_FUNC (on_bt_color_clicked),
                      NULL);
  gtk_signal_connect (GTK_OBJECT (bt_pp_hilight_keyword_color2_trans), "clicked",
                      GTK_SIGNAL_FUNC (on_bt_color_clicked),
                      NULL);
  gtk_signal_connect (GTK_OBJECT (bt_pp_hilight_keyword_color3_trans), "clicked",
                      GTK_SIGNAL_FUNC (on_bt_color_clicked),
                      NULL);
  gtk_signal_connect (GTK_OBJECT (bt_pp_hilight_keyword_color4_trans), "clicked",
                      GTK_SIGNAL_FUNC (on_bt_color_clicked),
                      NULL);
  gtk_signal_connect (GTK_OBJECT (bt_pp_hilight_keyword_color1), "clicked",
                      GTK_SIGNAL_FUNC (on_bt_color_clicked),
                      NULL);
  gtk_signal_connect (GTK_OBJECT (clist_kemph), "row_move",
                      GTK_SIGNAL_FUNC (on_clist_klist_row_move),
                      "clist_kemph");
  gtk_signal_connect (GTK_OBJECT (clist_kemph), "select_row",
                      GTK_SIGNAL_FUNC (on_clist_klist_select_row),
                      "clist_kemph");
  gtk_signal_connect (GTK_OBJECT (bt_kemph_up), "clicked",
                      GTK_SIGNAL_FUNC (on_bt_klist_up_clicked),
                      "clist_kemph");
  gtk_signal_connect (GTK_OBJECT (bt_kemph_down), "clicked",
                      GTK_SIGNAL_FUNC (on_bt_klist_down_clicked),
                      "clist_kemph");
  gtk_signal_connect (GTK_OBJECT (bt_kemph_del), "clicked",
                      GTK_SIGNAL_FUNC (on_bt_klist_del_clicked),
                      "clist_kemph");
  gtk_signal_connect (GTK_OBJECT (bt_kemph_change), "clicked",
                      GTK_SIGNAL_FUNC (on_bt_klist_change_clicked),
                      "clist_kemph");
  gtk_signal_connect (GTK_OBJECT (bt_kemph_add), "clicked",
                      GTK_SIGNAL_FUNC (on_bt_klist_add_clicked),
                      "clist_kemph");
  gtk_signal_connect (GTK_OBJECT (bt_pp_plopified_color), "clicked",
                      GTK_SIGNAL_FUNC (on_bt_color_clicked),
                      NULL);
  gtk_signal_connect (GTK_OBJECT (bt_pp_plopified_color_trans), "clicked",
                      GTK_SIGNAL_FUNC (on_bt_color_clicked),
                      NULL);
  gtk_signal_connect (GTK_OBJECT (clist_kplop), "row_move",
                      GTK_SIGNAL_FUNC (on_clist_klist_row_move),
                      "clist_kplop");
  gtk_signal_connect (GTK_OBJECT (clist_kplop), "select_row",
                      GTK_SIGNAL_FUNC (on_clist_klist_select_row),
                      "clist_kplop");
  gtk_signal_connect (GTK_OBJECT (bt_kplop_up), "clicked",
                      GTK_SIGNAL_FUNC (on_bt_klist_up_clicked),
                      "clist_kplop");
  gtk_signal_connect (GTK_OBJECT (bt_kplop_down), "clicked",
                      GTK_SIGNAL_FUNC (on_bt_klist_down_clicked),
                      "clist_kplop");
  gtk_signal_connect (GTK_OBJECT (bt_kplop_del), "clicked",
                      GTK_SIGNAL_FUNC (on_bt_klist_del_clicked),
                      "clist_kplop");
  gtk_signal_connect (GTK_OBJECT (bt_kplop_change), "clicked",
                      GTK_SIGNAL_FUNC (on_bt_klist_change_clicked),
                      "clist_kplop");
  gtk_signal_connect (GTK_OBJECT (bt_kplop_add), "clicked",
                      GTK_SIGNAL_FUNC (on_bt_klist_add_clicked),
                      "clist_kplop");
  gtk_signal_connect (GTK_OBJECT (comboentry_browser2), "changed",
                      GTK_SIGNAL_FUNC (on_editable_changed),
                      NULL);
  gtk_signal_connect (GTK_OBJECT (comboentry_browser), "changed",
                      GTK_SIGNAL_FUNC (on_editable_changed),
                      NULL);
  gtk_signal_connect (GTK_OBJECT (entry_gogole_search), "changed",
                      GTK_SIGNAL_FUNC (on_editable_changed),
                      NULL);
  gtk_signal_connect (GTK_OBJECT (entry_proxy), "changed",
                      GTK_SIGNAL_FUNC (on_editable_changed),
                      NULL);
  gtk_signal_connect (GTK_OBJECT (spinbutton_proxy_port), "changed",
                      GTK_SIGNAL_FUNC (on_spinbutton_changed),
                      NULL);
  gtk_signal_connect (GTK_OBJECT (checkbutton_proxy_no_cache), "toggled",
                      GTK_SIGNAL_FUNC (on_checkbutton_toggled),
                      NULL);
  gtk_signal_connect (GTK_OBJECT (checkbutton_proxy_disable_if_modified_since), "toggled",
                      GTK_SIGNAL_FUNC (on_checkbutton_toggled),
                      NULL);
  gtk_signal_connect (GTK_OBJECT (entry_path_news_backend), "changed",
                      GTK_SIGNAL_FUNC (on_editable_changed),
                      NULL);
  gtk_signal_connect (GTK_OBJECT (entry_path_myposts), "changed",
                      GTK_SIGNAL_FUNC (on_editable_changed),
                      NULL);
  gtk_signal_connect (GTK_OBJECT (entry_end_news_url), "changed",
                      GTK_SIGNAL_FUNC (on_editable_changed),
                      NULL);
  gtk_signal_connect (GTK_OBJECT (entry_path_messages), "changed",
                      GTK_SIGNAL_FUNC (on_editable_changed),
                      NULL);
  gtk_signal_connect (GTK_OBJECT (entry_path_board_add), "changed",
                      GTK_SIGNAL_FUNC (on_editable_changed),
                      NULL);
  gtk_signal_connect (GTK_OBJECT (entry_board_post), "changed",
                      GTK_SIGNAL_FUNC (on_editable_changed),
                      NULL);
  gtk_signal_connect (GTK_OBJECT (entry_path_board_backend), "changed",
                      GTK_SIGNAL_FUNC (on_editable_changed),
                      NULL);
  gtk_signal_connect (GTK_OBJECT (notebook_sites), "switch_page",
                      GTK_SIGNAL_FUNC (on_notebook_sites_switch_page),
                      NULL);
  gtk_signal_connect (GTK_OBJECT (bt_mv_site_left), "clicked",
                      GTK_SIGNAL_FUNC (on_bt_mv_site_clicked),
                      (gpointer)-1);
  gtk_signal_connect (GTK_OBJECT (bt_mv_site_right), "clicked",
                      GTK_SIGNAL_FUNC (on_bt_mv_site_clicked),
                      (gpointer)+1);
  gtk_signal_connect (GTK_OBJECT (bt_new_site), "clicked",
                      GTK_SIGNAL_FUNC (on_bt_new_site_clicked),
                      NULL);
  gtk_signal_connect (GTK_OBJECT (bt_remove_site), "clicked",
                      GTK_SIGNAL_FUNC (on_bt_remove_site_clicked),
                      NULL);
  gtk_signal_connect (GTK_OBJECT (bt_save), "clicked",
                      GTK_SIGNAL_FUNC (on_bt_save_clicked),
                      NULL);
  gtk_signal_connect (GTK_OBJECT (bt_save_as), "clicked",
                      GTK_SIGNAL_FUNC (on_bt_save_as_clicked),
                      NULL);
  gtk_signal_connect (GTK_OBJECT (bt_apply), "clicked",
                      GTK_SIGNAL_FUNC (on_bt_apply_clicked),
                      NULL);
  gtk_signal_connect (GTK_OBJECT (bt_cancel), "clicked",
                      GTK_SIGNAL_FUNC (on_bt_cancel_clicked),
                      NULL);

  gtk_object_set_data (GTK_OBJECT (main_win), "tooltips", tooltips);

  return main_win;
}

GtkWidget*
create_colorselectiondialog (void)
{
  GtkWidget *colorselectiondialog;
  GtkWidget *bt_colorsel_ok;
  GtkWidget *bt_colorsel_cancel;
  GtkWidget *help_button1;

  colorselectiondialog = gtk_color_selection_dialog_new ("Choix de la couleur");
  gtk_object_set_data (GTK_OBJECT (colorselectiondialog), "colorselectiondialog", colorselectiondialog);
  gtk_container_set_border_width (GTK_CONTAINER (colorselectiondialog), 10);
  GTK_WINDOW (colorselectiondialog)->type = GTK_WINDOW_DIALOG;
  gtk_window_set_modal (GTK_WINDOW (colorselectiondialog), TRUE);

  bt_colorsel_ok = GTK_COLOR_SELECTION_DIALOG (colorselectiondialog)->ok_button;
  gtk_object_set_data (GTK_OBJECT (colorselectiondialog), "bt_colorsel_ok", bt_colorsel_ok);
  gtk_widget_show (bt_colorsel_ok);
  GTK_WIDGET_SET_FLAGS (bt_colorsel_ok, GTK_CAN_DEFAULT);

  bt_colorsel_cancel = GTK_COLOR_SELECTION_DIALOG (colorselectiondialog)->cancel_button;
  gtk_object_set_data (GTK_OBJECT (colorselectiondialog), "bt_colorsel_cancel", bt_colorsel_cancel);
  gtk_widget_show (bt_colorsel_cancel);
  GTK_WIDGET_SET_FLAGS (bt_colorsel_cancel, GTK_CAN_DEFAULT);

  help_button1 = GTK_COLOR_SELECTION_DIALOG (colorselectiondialog)->help_button;
  gtk_object_set_data (GTK_OBJECT (colorselectiondialog), "help_button1", help_button1);
  gtk_widget_show (help_button1);
  GTK_WIDGET_SET_FLAGS (help_button1, GTK_CAN_DEFAULT);

  gtk_signal_connect (GTK_OBJECT (bt_colorsel_ok), "clicked",
                      GTK_SIGNAL_FUNC (on_bt_colorsel_ok_clicked),
                      NULL);
  gtk_signal_connect (GTK_OBJECT (bt_colorsel_cancel), "clicked",
                      GTK_SIGNAL_FUNC (on_bt_colorsel_cancel_clicked),
                      NULL);

  return colorselectiondialog;
}

GtkWidget*
create_fontselectiondialog (void)
{
  GtkWidget *fontselectiondialog;
  GtkWidget *bt_fontsel_ok;
  GtkWidget *bt_font_sel_cancel;
  GtkWidget *bt_fontsel_apply;

  fontselectiondialog = gtk_font_selection_dialog_new ("Choisir la police");
  gtk_object_set_data (GTK_OBJECT (fontselectiondialog), "fontselectiondialog", fontselectiondialog);
  gtk_container_set_border_width (GTK_CONTAINER (fontselectiondialog), 4);
  gtk_window_set_policy (GTK_WINDOW (fontselectiondialog), FALSE, TRUE, TRUE);

  bt_fontsel_ok = GTK_FONT_SELECTION_DIALOG (fontselectiondialog)->ok_button;
  gtk_object_set_data (GTK_OBJECT (fontselectiondialog), "bt_fontsel_ok", bt_fontsel_ok);
  gtk_widget_show (bt_fontsel_ok);
  GTK_WIDGET_SET_FLAGS (bt_fontsel_ok, GTK_CAN_DEFAULT);

  bt_font_sel_cancel = GTK_FONT_SELECTION_DIALOG (fontselectiondialog)->cancel_button;
  gtk_object_set_data (GTK_OBJECT (fontselectiondialog), "bt_font_sel_cancel", bt_font_sel_cancel);
  gtk_widget_show (bt_font_sel_cancel);
  GTK_WIDGET_SET_FLAGS (bt_font_sel_cancel, GTK_CAN_DEFAULT);

  bt_fontsel_apply = GTK_FONT_SELECTION_DIALOG (fontselectiondialog)->apply_button;
  gtk_object_set_data (GTK_OBJECT (fontselectiondialog), "bt_fontsel_apply", bt_fontsel_apply);
  gtk_widget_show (bt_fontsel_apply);
  GTK_WIDGET_SET_FLAGS (bt_fontsel_apply, GTK_CAN_DEFAULT);

  gtk_signal_connect (GTK_OBJECT (bt_fontsel_ok), "clicked",
                      GTK_SIGNAL_FUNC (on_bt_fontsel_ok_clicked),
                      NULL);
  gtk_signal_connect (GTK_OBJECT (bt_font_sel_cancel), "clicked",
                      GTK_SIGNAL_FUNC (on_bt_fontsel_cancel_clicked),
                      NULL);

  return fontselectiondialog;
}

GtkWidget*
create_fileselection_save (void)
{
  GtkWidget *fileselection_save;
  GtkWidget *fileselection_ok_button;
  GtkWidget *fileselection_cancel_button;

  fileselection_save = gtk_file_selection_new ("Select File");
  gtk_object_set_data (GTK_OBJECT (fileselection_save), "fileselection_save", fileselection_save);
  gtk_container_set_border_width (GTK_CONTAINER (fileselection_save), 10);

  fileselection_ok_button = GTK_FILE_SELECTION (fileselection_save)->ok_button;
  gtk_object_set_data (GTK_OBJECT (fileselection_save), "fileselection_ok_button", fileselection_ok_button);
  gtk_widget_show (fileselection_ok_button);
  GTK_WIDGET_SET_FLAGS (fileselection_ok_button, GTK_CAN_DEFAULT);

  fileselection_cancel_button = GTK_FILE_SELECTION (fileselection_save)->cancel_button;
  gtk_object_set_data (GTK_OBJECT (fileselection_save), "fileselection_cancel_button", fileselection_cancel_button);
  gtk_widget_show (fileselection_cancel_button);
  GTK_WIDGET_SET_FLAGS (fileselection_cancel_button, GTK_CAN_DEFAULT);

  gtk_signal_connect (GTK_OBJECT (fileselection_save), "delete_event",
                      GTK_SIGNAL_FUNC (on_fileselection_delete_event),
                      NULL);
  gtk_signal_connect (GTK_OBJECT (fileselection_ok_button), "clicked",
                      GTK_SIGNAL_FUNC (on_fileselection_ok_button_clicked),
                      NULL);
  gtk_signal_connect (GTK_OBJECT (fileselection_cancel_button), "clicked",
                      GTK_SIGNAL_FUNC (on_fileselection_cancel_button_clicked),
                      NULL);

  return fileselection_save;
}

GtkWidget*
create_fileselection_xpm (void)
{
  GtkWidget *fileselection_xpm;
  GtkWidget *fileselection_xpm_ok_bt;
  GtkWidget *fileselection_xpm_cancel_bt;

  fileselection_xpm = gtk_file_selection_new ("Select a .xpm file");
  gtk_object_set_data (GTK_OBJECT (fileselection_xpm), "fileselection_xpm", fileselection_xpm);
  gtk_container_set_border_width (GTK_CONTAINER (fileselection_xpm), 10);

  fileselection_xpm_ok_bt = GTK_FILE_SELECTION (fileselection_xpm)->ok_button;
  gtk_object_set_data (GTK_OBJECT (fileselection_xpm), "fileselection_xpm_ok_bt", fileselection_xpm_ok_bt);
  gtk_widget_show (fileselection_xpm_ok_bt);
  GTK_WIDGET_SET_FLAGS (fileselection_xpm_ok_bt, GTK_CAN_DEFAULT);

  fileselection_xpm_cancel_bt = GTK_FILE_SELECTION (fileselection_xpm)->cancel_button;
  gtk_object_set_data (GTK_OBJECT (fileselection_xpm), "fileselection_xpm_cancel_bt", fileselection_xpm_cancel_bt);
  gtk_widget_show (fileselection_xpm_cancel_bt);
  GTK_WIDGET_SET_FLAGS (fileselection_xpm_cancel_bt, GTK_CAN_DEFAULT);

  gtk_signal_connect (GTK_OBJECT (fileselection_xpm), "delete_event",
                      GTK_SIGNAL_FUNC (on_fileselection_delete_event),
                      NULL);
  gtk_signal_connect (GTK_OBJECT (fileselection_xpm_ok_bt), "clicked",
                      GTK_SIGNAL_FUNC (on_fileselection_xpm_ok_bt_clicked),
                      NULL);
  gtk_signal_connect (GTK_OBJECT (fileselection_xpm_cancel_bt), "clicked",
                      GTK_SIGNAL_FUNC (on_fileselection_xpm_cancel_bt_clicked),
                      NULL);

  return fileselection_xpm;
}

GtkWidget*
create_dialog_errmsg (void)
{
  GtkWidget *dialog_errmsg;
  GtkWidget *dialog_vbox1;
  GtkWidget *fixed1;
  GtkWidget *pixmap3;
  GtkWidget *label_errmsg;
  GtkWidget *dialog_action_area1;
  GtkWidget *ok_button;

  dialog_errmsg = gtk_dialog_new ();
  gtk_object_set_data (GTK_OBJECT (dialog_errmsg), "dialog_errmsg", dialog_errmsg);
  gtk_window_set_title (GTK_WINDOW (dialog_errmsg), "wmccc is talking to you");
  gtk_window_set_policy (GTK_WINDOW (dialog_errmsg), TRUE, TRUE, FALSE);

  dialog_vbox1 = GTK_DIALOG (dialog_errmsg)->vbox;
  gtk_object_set_data (GTK_OBJECT (dialog_errmsg), "dialog_vbox1", dialog_vbox1);
  gtk_widget_show (dialog_vbox1);
  gtk_widget_set_usize (dialog_vbox1, 400, 250);

  fixed1 = gtk_fixed_new ();
  gtk_widget_ref (fixed1);
  gtk_object_set_data_full (GTK_OBJECT (dialog_errmsg), "fixed1", fixed1,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (fixed1);
  gtk_box_pack_start (GTK_BOX (dialog_vbox1), fixed1, TRUE, TRUE, 0);

  pixmap3 = create_pixmap (dialog_errmsg, "badnews.xpm");
  gtk_widget_ref (pixmap3);
  gtk_object_set_data_full (GTK_OBJECT (dialog_errmsg), "pixmap3", pixmap3,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (pixmap3);
  gtk_fixed_put (GTK_FIXED (fixed1), pixmap3, 8, 8);
  gtk_widget_set_uposition (pixmap3, 8, 8);
  gtk_widget_set_usize (pixmap3, 0, 0);

  label_errmsg = gtk_label_new ("");
  gtk_label_parse_uline (GTK_LABEL (label_errmsg),
                         "label_errmsg");
  gtk_widget_ref (label_errmsg);
  gtk_object_set_data_full (GTK_OBJECT (dialog_errmsg), "label_errmsg", label_errmsg,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (label_errmsg);
  gtk_fixed_put (GTK_FIXED (fixed1), label_errmsg, 120, 8);
  gtk_widget_set_uposition (label_errmsg, 120, 8);
  gtk_widget_set_usize (label_errmsg, 312, 104);
  gtk_label_set_line_wrap (GTK_LABEL (label_errmsg), TRUE);

  dialog_action_area1 = GTK_DIALOG (dialog_errmsg)->action_area;
  gtk_object_set_data (GTK_OBJECT (dialog_errmsg), "dialog_action_area1", dialog_action_area1);
  gtk_widget_show (dialog_action_area1);
  gtk_container_set_border_width (GTK_CONTAINER (dialog_action_area1), 10);

  ok_button = gtk_button_new_with_label ("Honk ! Honk !");
  gtk_widget_ref (ok_button);
  gtk_object_set_data_full (GTK_OBJECT (dialog_errmsg), "ok_button", ok_button,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (ok_button);
  gtk_box_pack_start (GTK_BOX (dialog_action_area1), ok_button, FALSE, FALSE, 0);
  gtk_button_set_relief (GTK_BUTTON (ok_button), GTK_RELIEF_NONE);

  gtk_signal_connect (GTK_OBJECT (dialog_errmsg), "delete_event",
                      GTK_SIGNAL_FUNC (gtk_widget_destroy),
                      NULL);

  return dialog_errmsg;
}

