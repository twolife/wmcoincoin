/* this is the
   WMCCC (wmcoincoin configurator)
   main file.

--begin patch of jr-lamoule

                       Based on a wonderful idea of my dear friend JR-Lamoule (aka Jihaire)

--end patch of jr-lamoule
*/


/*
 * main.c initial généré par Glade. Éditer ce fichier à votre
 * convenance. Glade n'écrira plus dans ce fichier.
 */

#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif


#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/stat.h>  /* umask */
#include "wmccc_interface.h"
#include "wmccc_support.h"
#define GLOBALS_HERE
#define __WMCCC_C
#include "wmccc.h"
#include "options_list.h"
#include "coin_util.h"

/* Function to open a dialog box displaying the message provided. */
void quick_message(gchar *message) {

  GtkWidget *dialog;
  GtkWidget *label, *okay_button;
  
  g_assert(message);
  /* Create the widgets */
  dialog = create_dialog_errmsg();
  okay_button = lookup_widget(dialog, "ok_button"); g_assert(okay_button);
  gtk_signal_connect_object (GTK_OBJECT (okay_button), "clicked",
			     GTK_SIGNAL_FUNC (gtk_widget_destroy), GTK_OBJECT(dialog));
  gtk_window_set_modal(GTK_WINDOW(dialog), TRUE);
  label = lookup_widget(dialog, "label_errmsg"); g_assert(label);
  gtk_label_set_text(GTK_LABEL(label), message);
  gtk_widget_show_all (dialog);
}


gint clist_selected_row_number(GtkWidget *clist) {
  GList  *l;
  gint   row=-1;
  for (l = GTK_CLIST(clist)->selection; l; l = l->next) {
    row=GPOINTER_TO_INT(l->data);
  }
  return(row);
}

char *
miniuarule_to_string(MiniUARule *r) {
  int n = 0;
  char s[1024];
  int sz = 0;
  s[0] = 0; 
  if (r->site_name) { 
    sz += g_snprintf(s+sz, 1024-sz, "site:\"%s\"", r->site_name); n++; 
  }
  if (r->user_login) {
    sz += g_snprintf(s+sz, 1024-sz, "%slogin:\"%s\"", n?", ":"", r->user_login); n++; 
  }
  if (r->rgx) { 
    sz += g_snprintf(s+sz, 1024-sz, "%sua:\"%s\"", n?", ":"", r->s_rgx); n++; 
  }
  sz +=   g_snprintf(s+sz, 1024-sz, " => ");
  n = 0;
  if (r->color != -1) { 
    sz += g_snprintf(s+sz, 1024-sz, "color%s=\"#%06x\"", r->color_terminal?":":"", r->color); n++;
  }
  if (r->symb != -1) {
    sz += g_snprintf(s+sz, 1024-sz, "%ssymb%s=\"%s\"", n?", ":"", r->symb_terminal?":":"", symboles[r->symb].name); n++; 
  }
  if (r->rua) {
    sz += g_snprintf(s+sz, 1024-sz, "%sua%s=\"%s\"", n?", ":"", r->ua_terminal?":":"", r->rua); 
  }
  return g_strndup(s,1024);
}

void
icolor_to_gdk(int icol, GdkColor *gcol)
{
  gcol->pixel = 0;
  gcol->red   = ((icol & 0xff0000) >> 16)*256;
  gcol->green = ((icol & 0x00ff00) >> 8)*256;
  gcol->blue  = ((icol & 0x0000ff))*256;
}

/* Create a new hbox with an image and a label packed into it
 * and return the box. */
GtkWidget *xpm_label_box( GtkWidget *parent,
                          gchar     *xpm_filename,
                          gchar     *label_text, int *pix_ok) {
    GtkWidget *box1;
    GtkWidget *label = NULL;
    GtkWidget *pixmapwid = NULL;
    GdkPixmap *pixmap;
    GdkBitmap *mask;
    GtkStyle *style;

    /* Create box for xpm and label */
    box1 = gtk_hbox_new (FALSE, 0);
    gtk_container_set_border_width (GTK_CONTAINER (box1), 2);

    /* Get the style of the button to get the
     * background color. */
    style = gtk_widget_get_style(parent);

    /* Now on to the xpm stuff */
    //    if (parent->window)
    pixmap = gdk_pixmap_create_from_xpm (glob.main_win->window, &mask,
                                         &style->bg[GTK_STATE_NORMAL],
                                         xpm_filename);
    
    *pix_ok = 1;
    if (pixmap == NULL) {
      label_text = "NONE";
      *pix_ok = 0;
    }

    if (pixmap) {
      pixmapwid = gtk_pixmap_new (pixmap, mask);
    }
    //    g_print("pixmap=%x '%s'", pixmapwid, xpm_filename);

    if (label_text) {
      /* Create a label for the button */
      label = gtk_label_new (label_text);
    }
    /* Pack the pixmap and label into the box */
    if (pixmap) {
      gtk_box_pack_start (GTK_BOX (box1), pixmapwid, TRUE, FALSE, 0);
    }
    if (label_text) {
      gtk_box_pack_start (GTK_BOX (box1), label, TRUE, TRUE, 0);
    }
    if (pixmap)
      gtk_widget_show(pixmapwid);
    if (label_text)
      gtk_widget_show(label);

    return(box1);
}


/* set the button ne background pixmap */
int  update_widget_bgpixmap(GtkWidget *w, char *pixname)
{
  GtkWidget *box;
  int ok = 1;
  if (GTK_BIN(w)->child)
    gtk_container_remove(GTK_CONTAINER(w), GTK_BIN(w)->child);
  box = xpm_label_box(w, pixname, NULL, &ok);
  gtk_widget_show(box);
  gtk_container_add (GTK_CONTAINER(w), box);
  return ok;
}


void update_miniua_clist(MiniUARules *urs) {
  GtkWidget *wg;
  MiniUARule *ur;
  wg = lookup_widget(glob.main_win, "clist_miniua"); g_assert(wg);
  gtk_clist_clear(GTK_CLIST(wg));
  for (ur = urs->first; ur; ur = ur->next) {
    char *s = miniuarule_to_string(ur);
    gtk_clist_set_row_data(GTK_CLIST(wg),gtk_clist_append(GTK_CLIST(wg), &s),ur);
    g_free(s);
  }
}
/*
  c chiiiiiiaaaaant à faire ..
*/
void clist_klist_row_to_entries(GtkCList *clist, int row) {
  int isplop = (GTK_WIDGET(clist) == lookup_widget(glob.main_win, "clist_kplop"));
  KeyList *hk= gtk_clist_get_row_data(clist, row);
  GtkWidget *om_categ, *om_match, *om_key;

  if (isplop) {
    om_categ = lookup_widget(glob.main_win, "optionmenu_kplop_categ"); 
    om_match = lookup_widget(glob.main_win, "optionmenu_kplop_match"); 
    om_key = lookup_widget(glob.main_win, "entry_kplop_key");
  } else {
    om_categ = lookup_widget(glob.main_win, "optionmenu_kemph_categ"); 
    om_match = lookup_widget(glob.main_win, "optionmenu_kemph_match"); 
    om_key = lookup_widget(glob.main_win, "entry_kemph_key");
  }
  if (hk) {
    int i = 0;
    gtk_option_menu_set_history(GTK_OPTION_MENU(om_categ), hk->num);
    switch (hk->type) {
    case HK_LOGIN: i = 0; break;
    case HK_UA: i=1; break;
    case HK_WORD: i=2; break;
    case HK_UA_NOLOGIN: i=3; break;
    default: i = 2; break;
    }
    gtk_option_menu_set_history(GTK_OPTION_MENU(om_match), i);
    gtk_entry_set_text(GTK_ENTRY(om_key), hk->key);
  } else {
    gtk_entry_set_text(GTK_ENTRY(om_key), "");
  }
}


void
clist_klist_set_row(GtkCList *clist, KeyList *hk, int row) {
  const char *stype;
  char scateg[20];
  int isplop = (GTK_WIDGET(clist) == lookup_widget(glob.main_win, "clist_kplop"));

  if (!isplop) {
    snprintf(scateg, 20, "%d", hk->num);
  } else {
    snprintf(scateg, 20, "%s", hk->num == 0 ? "plopify" : (hk->num == 1 ? "superplopify" : "boitakon"));
  }
  stype = key_list_type_name(hk->type);
  gtk_clist_set_text(clist, row, 0, stype);
  gtk_clist_set_text(clist, row, 1, hk->key);
  gtk_clist_set_text(clist, row, 2, scateg);
  gtk_clist_set_row_data(clist, row, hk);
}

void update_keylist_clist(char *wname, KeyList *first) {
  GtkWidget *wg;
  KeyList *hk;
  wg = lookup_widget(glob.main_win, wname); g_assert(wg);
  gtk_clist_clear(GTK_CLIST(wg));
  gtk_clist_set_reorderable(GTK_CLIST(wg), TRUE);
  for (hk = first; hk; hk=hk->next) {
    char *slst[3];
    int row;
    slst[0] = slst[1] = slst[2] = "plop";
    row = gtk_clist_append(GTK_CLIST(wg), (char**)slst);
    clist_klist_set_row(GTK_CLIST(wg), hk, row);
  }
}

void update_transparency_fields_state() {
  gtk_widget_set_sensitive(lookup_widget(glob.main_win, "label_shade_min"), 
			   (Prefs->pp_transparency.type == SHADING) ? 
			   TRUE:FALSE);
  gtk_widget_set_sensitive(lookup_widget(glob.main_win, "spinbutton_transp_shade_min"), 
			   (Prefs->pp_transparency.type == SHADING) ? 
			   TRUE:FALSE);
   gtk_widget_set_sensitive(lookup_widget(glob.main_win, "label_shade_max"), 
			   (Prefs->pp_transparency.type == SHADING) ? 
			   TRUE:FALSE);
   gtk_widget_set_sensitive(lookup_widget(glob.main_win, "spinbutton_transp_shade_max"), 
			   (Prefs->pp_transparency.type == SHADING) ? 
			   TRUE:FALSE);
  gtk_widget_set_sensitive(lookup_widget(glob.main_win, "label_black_tint"), 
			   (Prefs->pp_transparency.type == TINTING) ? 
			   TRUE:FALSE);
   gtk_widget_set_sensitive(lookup_widget(glob.main_win, "bt_transp_black"), 
			   (Prefs->pp_transparency.type == TINTING) ? 
			   TRUE:FALSE);
  gtk_widget_set_sensitive(lookup_widget(glob.main_win, "label_white_tint"), 
			   (Prefs->pp_transparency.type == TINTING) ? 
			   TRUE:FALSE);
  gtk_widget_set_sensitive(lookup_widget(glob.main_win, "bt_transp_white"), 
			   (Prefs->pp_transparency.type == TINTING) ? 
			   TRUE:FALSE);
}

/* set the button background color 
   and 'links' it to the specified property
*/
void  link_color_button(GtkWidget *main, gchar *bt_name, int *icol)
{
  GtkWidget *bt;
  bt = lookup_widget(main, bt_name); g_assert(bt);
  gtk_object_set_data(GTK_OBJECT(bt), MainWindowKey, main);
  gtk_object_set_data(GTK_OBJECT(bt), ColorPtrKey, icol);

  gtk_signal_connect_after (GTK_OBJECT (bt), "draw",
                            GTK_SIGNAL_FUNC (on_bt_color_draw),
                            NULL);
  gtk_signal_connect_after (GTK_OBJECT (bt), "expose_event", 
                            GTK_SIGNAL_FUNC (on_bt_color_expose_event), 
                            NULL);
  on_bt_color_expose_event(bt, NULL, NULL);
}


void  link_pixmap_button(GtkWidget *main, gchar *bt_name, char **pfname, int width, int height)
{
  GtkWidget *bt;
  bt = lookup_widget(main, bt_name); g_assert(bt);
  //  if (!bt->window) return;
  if (!update_widget_bgpixmap(bt, *pfname) && *pfname) {
    g_free(*pfname); *pfname = NULL;
  }
  gtk_object_set_data(GTK_OBJECT(bt), MainWindowKey, main);
  gtk_object_set_data(GTK_OBJECT(bt), StrPtrKey, pfname);
  gtk_object_set_data(GTK_OBJECT(bt), WidthKey, GINT_TO_POINTER(width));
  gtk_object_set_data(GTK_OBJECT(bt), HeightKey, GINT_TO_POINTER(height));
}


/* set the button ne background color */
void  link_toggle_button(GtkWidget *main, gchar *bt_name, int *t)
{
  GtkWidget *bt;
  bt = lookup_widget(main, bt_name); g_assert(bt);
  gtk_object_set_data(GTK_OBJECT(bt), MainWindowKey, main);
  gtk_object_set_data(GTK_OBJECT(bt), BoolPtrKey, t);
  gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(bt), *t ? TRUE : FALSE);
}

void  link_spin_button(GtkWidget *main, gchar *bt_name, int *v)
{
  GtkWidget *bt;
  bt = lookup_widget(main, bt_name); g_assert(bt);
  gtk_object_set_data(GTK_OBJECT(bt), MainWindowKey, main);
  gtk_object_set_data(GTK_OBJECT(bt), IntPtrKey, v);
  gtk_spin_button_set_numeric(GTK_SPIN_BUTTON(bt), TRUE);
  gtk_spin_button_set_value(GTK_SPIN_BUTTON(bt), *v);
}

void link_optionmenu_widget(GtkWidget *main, gchar *name, int *v, int shift)
{
  GtkWidget *menu = lookup_widget(main, name); g_assert(menu); 
  gtk_option_menu_set_history(GTK_OPTION_MENU(menu), (*v) - shift);
  gtk_object_set_data(GTK_OBJECT(GTK_OPTION_MENU(menu)->menu), IntPtrKey, v);
  gtk_object_set_data(GTK_OBJECT(GTK_OPTION_MENU(menu)->menu), ShiftKey, GINT_TO_POINTER(shift));
  gtk_signal_connect(GTK_OBJECT (GTK_OPTION_MENU(menu)->menu),
		     "deactivate", GTK_SIGNAL_FUNC(on_optionmenu_selected),
		     NULL);
}

void
link_text_widget(GtkWidget *main, gchar *widget_name, char **ppstr)
{
  GtkWidget *wg;
  int foo;
  wg = lookup_widget(main, widget_name); g_assert(wg);
  gtk_object_set_data(GTK_OBJECT(wg), MainWindowKey, main);
  gtk_object_set_data(GTK_OBJECT(wg), StrPtrKey, NULL);
  gtk_text_set_word_wrap(GTK_TEXT(wg), TRUE);
  gtk_text_set_line_wrap(GTK_TEXT(wg), TRUE);
  gtk_editable_delete_text(GTK_EDITABLE(wg), 0, -1);
  if (*ppstr) {
    foo = 0;
    //    g_print("tEXT: %s\n", *ppstr);
    gtk_editable_insert_text(GTK_EDITABLE(wg), *ppstr, strlen(*ppstr), &foo);
  }
  gtk_object_set_data(GTK_OBJECT(wg), StrPtrKey, ppstr);
}

void
link_entry_widget(GtkWidget *main, gchar *widget_name, char **ppstr)
{
  GtkWidget *wg;
  wg = lookup_widget(main, widget_name); g_assert(wg);
  gtk_object_set_data(GTK_OBJECT(wg), MainWindowKey, main);
  gtk_object_set_data(GTK_OBJECT(wg), StrPtrKey, ppstr);
  if (*ppstr)
    gtk_entry_set_text(GTK_ENTRY(wg), *ppstr);
  else gtk_entry_set_text(GTK_ENTRY(wg), "");
}

void
site_notebook_update_labels(GeneralPrefs *p) {
  GtkNotebook *tabs;
  GtkWidget *tab;
  int pnum;

  glob.updating_labels++;
  tabs = GTK_NOTEBOOK(lookup_widget(glob.main_win, "notebook_sites")); g_assert(tabs);
  for (pnum = 0; pnum < p->nb_sites; pnum++) {
    GtkWidget *label;
    char *s;
    tab = gtk_notebook_get_nth_page(tabs, pnum);
    if (p->site[pnum]->all_names[0] && strlen(p->site[pnum]->all_names[0])) {
      s = g_strndup(p->site[pnum]->all_names[0], 20);
    } else {
      s = g_strdup("unnamed");
    }
    label = gtk_label_new(s);
    g_free(s); s= NULL;
    if (tab == NULL) {
      GtkWidget *empty_notebook_page;

      empty_notebook_page = gtk_vbox_new (FALSE, 0);
      gtk_widget_show (empty_notebook_page);
      gtk_container_add (GTK_CONTAINER (tabs), empty_notebook_page);

      gtk_widget_show (label);
      gtk_notebook_set_tab_label(tabs, gtk_notebook_get_nth_page(tabs, pnum), label);

      //g_print("+ tab %d <- '%s'\n", pnum, p->site[pnum]->all_names[0]);
      tab = gtk_notebook_get_nth_page(tabs, pnum);
      g_assert(tab);
    } else {
      //g_print(". tab %d <- '%s'\n", pnum, p->site[pnum]->all_names[0]);
      gtk_notebook_set_tab_label (tabs, tab, label);
    }
  }
  /* remove remaining pages */
  while ((tab = gtk_notebook_get_nth_page(tabs, pnum))) {
    gtk_notebook_remove_page(tabs, pnum);
  }
  glob.updating_labels--;  



  if (glob.updating_labels == 0) 
    glob.current_site = gtk_notebook_get_current_page(tabs);
}

void 
site_notebook_update(GeneralPrefs *p) {
  site_notebook_update_labels(p);
  site_panels_update(p->site[glob.current_site]);
}

void
dock_global_panel_update(GeneralPrefs *p) {
  link_color_button(glob.main_win, "bt_dock_bgcolor", &p->dock_bgcolor);
  link_color_button(glob.main_win, "bt_dock_fgcolor", &p->dock_fgcolor);
  link_pixmap_button(glob.main_win, "bt_dock_bgpixmap", &p->dock_bgpixmap, 64, 64);
  link_pixmap_button(glob.main_win, "bt_dock_skinpixmap", &p->dock_skin_pixmap, 320, 64);
  link_toggle_button(glob.main_win, "checkbutton_dock_use_iconwin", &p->use_iconwin);
  link_toggle_button(glob.main_win, "checkbutton_dock_draw_border", &p->draw_border);
  link_toggle_button(glob.main_win, "checkbutton_dock_use_balloons", &p->use_balloons);
  link_toggle_button(glob.main_win, "checkbutton_start_in_boss_mode", &p->start_in_boss_mode);
}

void
dock_site_panel_update(SitePrefs *sp) {
  g_assert(glob.current_site < Prefs->nb_sites);
  link_entry_widget(glob.main_win, "entry_site_name", &sp->all_names[0]);
  link_entry_widget(glob.main_win, "entry_site_alias1", &sp->all_names[1]);
  link_entry_widget(glob.main_win, "entry_site_alias2", &sp->all_names[2]);
  link_entry_widget(glob.main_win, "entry_site_alias3", &sp->all_names[3]);
  link_entry_widget(glob.main_win, "entry_site_root_url", &sp->site_root);
  link_text_widget(glob.main_win, "text_site_cookie", &sp->user_cookie);
  link_entry_widget(glob.main_win, "entry_user_login", &sp->user_login);
  link_toggle_button(glob.main_win, "checkbutton_site_board", &sp->check_board);
  link_toggle_button(glob.main_win, "checkbutton_site_news", &sp->check_news);
  link_toggle_button(glob.main_win, "checkbutton_site_messages", &sp->check_messages);
  link_toggle_button(glob.main_win, "checkbutton_site_comments", &sp->check_comments);
  g_assert(sizeof(sp->locale) == sizeof(int));
  link_optionmenu_widget(glob.main_win, "optionmenu_site_locale", (int*)&sp->locale,0);
}

void
news_global_panel_update(GeneralPrefs *p) {
  link_color_button(glob.main_win, "bt_news_bgcolor", &p->news_bgcolor);
  link_color_button(glob.main_win, "bt_news_fgcolor", &p->news_fgcolor);
  link_color_button(glob.main_win, "bt_news_titles_bgcolor", &p->news_titles_bgcolor);
  link_color_button(glob.main_win, "bt_news_titles_fgcolor", &p->news_titles_fgcolor);
  link_color_button(glob.main_win, "bt_news_emph_color", &p->news_emph_color);
  link_spin_button(glob.main_win, "spinbt_switch_off_coincoin_delay", &p->switch_off_coincoin_delay);
  link_spin_button(glob.main_win, "spinbtmax_refresh_delay", &p->max_refresh_delay);
  link_entry_widget(glob.main_win, "comboentry_news_fnfamily", &p->news_fn_family);
  link_spin_button(glob.main_win, "spinbutton_news_fnsize", &p->news_fn_size);

  link_color_button(glob.main_win, "bt_sc_bg_color", &p->sc_bg_color.opaque);
  link_color_button(glob.main_win, "bt_sc_bg_color_trans", &p->sc_bg_color.transp);
  link_color_button(glob.main_win, "bt_sc_bg_dark_color", &p->sc_bg_dark_color.opaque);
  link_color_button(glob.main_win, "bt_sc_bg_dark_color_trans", &p->sc_bg_dark_color.transp);
  link_color_button(glob.main_win, "bt_sc_bg_light_color", &p->sc_bg_light_color.opaque);
  link_color_button(glob.main_win, "bt_sc_bg_light_color_trans", &p->sc_bg_light_color.transp);
  link_color_button(glob.main_win, "bt_sc_bar_color", &p->sc_bar_color.opaque);
  link_color_button(glob.main_win, "bt_sc_bar_color_trans", &p->sc_bar_color.transp);
  link_color_button(glob.main_win, "bt_sc_bar_dark_color", &p->sc_bar_dark_color.opaque);
  link_color_button(glob.main_win, "bt_sc_bar_dark_color_trans", &p->sc_bar_dark_color.transp);
  link_color_button(glob.main_win, "bt_sc_bar_light_color", &p->sc_bar_light_color.opaque);
  link_color_button(glob.main_win, "bt_sc_bar_light_color_trans", &p->sc_bar_light_color.transp);
  link_color_button(glob.main_win, "bt_sc_arrow_normal_color", &p->sc_arrow_normal_color.opaque);
  link_color_button(glob.main_win, "bt_sc_arrow_normal_color_trans", &p->sc_arrow_normal_color.transp);
  link_color_button(glob.main_win, "bt_sc_arrow_emphasized_color", &p->sc_arrow_emphasized_color.opaque);
  link_color_button(glob.main_win, "bt_sc_arrow_emphasized_color_trans", &p->sc_arrow_emphasized_color.transp);


  link_color_button(glob.main_win, "bt_pp_hilight_keyword_color1", &p->pp_keyword_color[1].opaque);
  

  //  g_snprintf(name, 100, "%s / %d", Prefs->news_fn_family, Prefs->news_fn_size);
  //  gtk_label_set_text(GTK_LABEL(lookup_widget(glob.main_win,"label_news_font")), name);
}

void
news_site_panel_update(SitePrefs *sp) {
  g_assert(glob.current_site < Prefs->nb_sites);
  link_spin_button(glob.main_win, "spinbutton_news_delay", &sp->news_check_delay);
  link_spin_button(glob.main_win, "spinbutton_news_max_age", &sp->news_max_nb_days);
}

void
board_global_panel_update(GeneralPrefs *p) {
  link_toggle_button(glob.main_win, "checkbutton_trolldetector", &p->enable_troll_detector);
  link_toggle_button(glob.main_win, "checkbutton_board_auto_dl_pictures", &p->board_auto_dl_pictures);
  link_entry_widget(glob.main_win, "entry_archive_filename", &p->board_scrinechote);
  update_miniua_clist(&p->miniuarules);
}

void
bigorno_global_panel_update(GeneralPrefs *p) {
  link_toggle_button(glob.main_win, "checkbutton_post_cmd", &p->post_cmd_enabled[0]);
  link_text_widget(glob.main_win, "text_post_cmd", &p->post_cmd[0]);
  link_toggle_button(glob.main_win, "checkbutton_post_cmd2", &p->post_cmd_enabled[1]);
  link_text_widget(glob.main_win, "text_post_cmd2", &p->post_cmd[1]);
}

void
board_site_panel_update(SitePrefs *sp) {
  link_spin_button(glob.main_win, "spinbutton_board_delay", &sp->board_check_delay);
  link_spin_button(glob.main_win, "spinbutton_board_max_msg", &sp->board_max_msg);
  link_entry_widget(glob.main_win, "combo_entry_board_wiki", &sp->board_wiki_emulation);  
  link_toggle_button(glob.main_win, "checkbutton_pp_use_AMPM", &sp->use_AM_PM);
  link_toggle_button(glob.main_win, "checkbutton_board_auto_refresh", &sp->board_auto_refresh);
}

void
palmipede_global_panel_update(GeneralPrefs *p) {
  link_entry_widget(glob.main_win, "entry_coin_coin_message", &p->coin_coin_message);
  link_toggle_button(glob.main_win, "checkbutton_enable_spell", &p->ew_do_spell);
  link_entry_widget(glob.main_win, "entry_spell_cmd", &p->ew_spell_cmd);
  link_entry_widget(glob.main_win, "entry_spell_dict", &p->ew_spell_dict);
}

void
palmipede_site_panel_update(SitePrefs *sp) {
  link_spin_button(glob.main_win, "spinbutton_msg_max_length", &sp->palmi_msg_max_len);
  link_spin_button(glob.main_win, "spinbutton_ua_max_length", &sp->palmi_ua_max_len);
  link_entry_widget(glob.main_win, "entry_default_ua", &sp->user_agent);
  
}

void
pinnipede_global_panel_update(GeneralPrefs *p) {
  /* first panel */

  link_toggle_button(glob.main_win, "checkbutton_pp_auto_open", &p->pinnipede_open_on_start);
  link_toggle_button(glob.main_win, "checkbutton_pp_use_classical_tabs", &p->pp_use_classical_tabs);
  link_toggle_button(glob.main_win, "checkbutton_pp_use_colored_tabs", &p->pp_use_colored_tabs);
  link_toggle_button(glob.main_win, "checkbutton_pp_hungry_boitakon", &p->hungry_boitakon);  
  
  link_toggle_button(glob.main_win, "checkbutton_start_in_transparency", &p->pp_start_in_transparency_mode);
  link_toggle_button(glob.main_win, "checkbutton_use_fake_real_transparency", &p->use_fake_real_transparency);
  link_spin_button(glob.main_win, "spinbutton_transp_shade_min", &p->pp_transparency.shade.luminosite);
  link_spin_button(glob.main_win, "spinbutton_transp_shade_max", &p->pp_transparency.shade.assombrissement);
  link_color_button(glob.main_win, "bt_transp_black",  &p->pp_transparency.tint.black);
  link_color_button(glob.main_win, "bt_transp_white",  &p->pp_transparency.tint.white);
  gtk_option_menu_set_history(GTK_OPTION_MENU(lookup_widget(glob.main_win, "optionmenu_transp")),
			      (p->pp_transparency.type == FULL_TRANSPARENCY ? 0 :
			       (p->pp_transparency.type == SHADING ? 1 : 2)));
  update_transparency_fields_state();

  link_entry_widget(glob.main_win, "comboentry_pp_fnfamily", &p->pp_fn_family);
  link_spin_button(glob.main_win, "spinbutton_pp_fnsize", &p->pp_fn_size);

  link_toggle_button(glob.main_win, "checkbutton_show_troll_score", &p->pp_trollscore_mode);
  link_toggle_button(glob.main_win, "checkbutton_show_fortune", &p->pp_fortune_mode);
  link_toggle_button(glob.main_win, "checkbutton_show_seconds", &p->pp_show_sec_mode);
  link_toggle_button(glob.main_win, "checkbutton_show_tags", &p->pp_html_mode);

  link_optionmenu_widget(glob.main_win, "optionmenu_nick_mode", &p->pp_nick_mode, 0);

  link_color_button(glob.main_win, "bt_pp_emph_color", &p->pp_emph_color.opaque);
  link_color_button(glob.main_win, "bt_pp_emph_color_trans", &p->pp_emph_color.transp);
  link_color_button(glob.main_win, "bt_pp_sel_color", &p->pp_sel_bgcolor.opaque);
  link_color_button(glob.main_win, "bt_pp_sel_color_trans", &p->pp_sel_bgcolor.transp);
  link_color_button(glob.main_win, "bt_pp_popup_bgcolor", &p->pp_popup_bgcolor.opaque);
  link_color_button(glob.main_win, "bt_pp_popup_bgcolor_trans", &p->pp_popup_bgcolor.transp);
  link_color_button(glob.main_win, "bt_pp_popup_fgcolor", &p->pp_popup_fgcolor.opaque);
  link_color_button(glob.main_win, "bt_pp_popup_fgcolor_trans", &p->pp_popup_fgcolor.transp);

  /* second panel */
  link_toggle_button(glob.main_win, "checkbutton_pp_minibar_on", &p->pp_minibar_on);
  link_color_button(glob.main_win, "bt_pp_bb_bgcolor", &p->pp_buttonbar_bgcolor.opaque);
  link_color_button(glob.main_win, "bt_pp_bb_bgcolor_trans", &p->pp_buttonbar_bgcolor.transp);
  link_color_button(glob.main_win, "bt_pp_bb_fgcolor", &p->pp_buttonbar_fgcolor.opaque);
  link_color_button(glob.main_win, "bt_pp_bb_fgcolor_trans", &p->pp_buttonbar_fgcolor.transp);
  link_color_button(glob.main_win, "bt_pp_bb_progressbar_color", &p->pp_buttonbar_progressbar_color.opaque);
  link_color_button(glob.main_win, "bt_pp_bb_progressbar_color_trans", &p->pp_buttonbar_progressbar_color.transp);
  link_color_button(glob.main_win, "bt_pp_bb_updlcnt_color", &p->pp_buttonbar_updlcnt_color.opaque);
  link_color_button(glob.main_win, "bt_pp_bb_updlcnt_color_trans", &p->pp_buttonbar_updlcnt_color.transp);
  link_color_button(glob.main_win, "bt_pp_bb_msgcnt_color", &p-> pp_buttonbar_msgcnt_color.opaque);
  link_color_button(glob.main_win, "bt_pp_bb_msgcnt_color_trans", &p-> pp_buttonbar_msgcnt_color.transp);
  link_color_button(glob.main_win, "bt_pp_my_messages", &p->pp_my_msg_color.opaque);
  link_color_button(glob.main_win, "bt_pp_my_messages_trans", &p->pp_my_msg_color.transp);
  link_color_button(glob.main_win, "bt_pp_answers_to_my_messages", &p->pp_answer_my_msg_color.opaque);
  link_color_button(glob.main_win, "bt_pp_answers_to_my_messages_trans", &p->pp_answer_my_msg_color.transp);
  link_color_button(glob.main_win, "bt_pp_plopified_color", &p->pp_plopify_color.opaque);
  link_color_button(glob.main_win, "bt_pp_plopified_color_trans", &p->pp_plopify_color.transp);
  link_color_button(glob.main_win, "bt_pp_hilight_keyword_color0", &p->pp_keyword_color[0].opaque);
  link_color_button(glob.main_win, "bt_pp_hilight_keyword_color0_trans", &p->pp_keyword_color[0].transp);
  link_color_button(glob.main_win, "bt_pp_hilight_keyword_color1", &p->pp_keyword_color[1].opaque);
  link_color_button(glob.main_win, "bt_pp_hilight_keyword_color1_trans", &p->pp_keyword_color[1].transp);
  link_color_button(glob.main_win, "bt_pp_hilight_keyword_color2", &p->pp_keyword_color[2].opaque);
  link_color_button(glob.main_win, "bt_pp_hilight_keyword_color2_trans", &p->pp_keyword_color[2].transp);
  link_color_button(glob.main_win, "bt_pp_hilight_keyword_color3", &p->pp_keyword_color[3].opaque);
  link_color_button(glob.main_win, "bt_pp_hilight_keyword_color3_trans", &p->pp_keyword_color[3].transp);
  link_color_button(glob.main_win, "bt_pp_hilight_keyword_color4", &p->pp_keyword_color[4].opaque);
  link_color_button(glob.main_win, "bt_pp_hilight_keyword_color4_trans", &p->pp_keyword_color[4].transp);

  update_keylist_clist("clist_kemph", Prefs->hilight_key_list);
  update_keylist_clist("clist_kplop", Prefs->plopify_key_list);
}

void
pinnipede_site_panel_update(SitePrefs *sp) 
{
  /* first panel */
  link_toggle_button(glob.main_win, "checkbutton_mark_id_gaps", &sp->mark_id_gaps);

  link_color_button(glob.main_win, "bt_pp_fgcolor", &sp->pp_fgcolor.opaque);
  link_color_button(glob.main_win, "bt_pp_fgcolor_trans", &sp->pp_fgcolor.transp);
  link_color_button(glob.main_win, "bt_pp_bgcolor", &sp->pp_bgcolor);
  link_color_button(glob.main_win, "bt_pp_clock_color", &sp->pp_tstamp_color.opaque);
  link_color_button(glob.main_win, "bt_pp_clock_color_trans", &sp->pp_tstamp_color.transp);
  link_color_button(glob.main_win, "bt_pp_login_color", &sp->pp_login_color.opaque);
  link_color_button(glob.main_win, "bt_pp_login_color_trans", &sp->pp_login_color.transp);
  link_color_button(glob.main_win, "bt_pp_ua_color", &sp->pp_useragent_color.opaque);
  link_color_button(glob.main_win, "bt_pp_ua_color_trans", &sp->pp_useragent_color.transp);
  link_color_button(glob.main_win, "bt_pp_trollscore_color", &sp->pp_trollscore_color.opaque);
  link_color_button(glob.main_win, "bt_pp_trollscore_color_trans", &sp->pp_trollscore_color.transp);
  link_color_button(glob.main_win, "bt_pp_url_color", &sp->pp_url_color.opaque);
  link_color_button(glob.main_win, "bt_pp_url_color_trans", &sp->pp_url_color.transp);
  link_color_button(glob.main_win, "bt_pp_visited_url_color", &sp->pp_visited_url_color.opaque);
  link_color_button(glob.main_win, "bt_pp_visited_url_color_trans", &sp->pp_visited_url_color.transp);
  link_color_button(glob.main_win, "bt_pp_strike_color", &sp->pp_strike_color.opaque);
  link_color_button(glob.main_win, "bt_pp_strike_color_trans", &sp->pp_strike_color.transp);

  link_toggle_button(glob.main_win, "togglebutton_pp_login_style_bold", &sp->pp_login_style.bold);
  link_toggle_button(glob.main_win, "togglebutton_pp_login_style_underlined", &sp->pp_login_style.underlined);
  link_toggle_button(glob.main_win, "togglebutton_pp_login_style_slanted", &sp->pp_login_style.slanted);
  link_toggle_button(glob.main_win, "togglebutton_pp_login_style_teletype", &sp->pp_login_style.teletype);
  link_toggle_button(glob.main_win, "togglebutton_pp_ua_style_bold", &sp->pp_ua_style.bold);
  link_toggle_button(glob.main_win, "togglebutton_pp_ua_style_underlined", &sp->pp_ua_style.underlined);
  link_toggle_button(glob.main_win, "togglebutton_pp_ua_style_slanted", &sp->pp_ua_style.slanted);
  link_toggle_button(glob.main_win, "togglebutton_pp_ua_style_teletype", &sp->pp_ua_style.teletype);
  link_toggle_button(glob.main_win, "togglebutton_pp_clock_style_bold", &sp->pp_clock_style.bold);
  link_toggle_button(glob.main_win, "togglebutton_pp_clock_style_underlined", &sp->pp_clock_style.underlined);
  link_toggle_button(glob.main_win, "togglebutton_pp_clock_style_slanted", &sp->pp_clock_style.slanted);
  link_toggle_button(glob.main_win, "togglebutton_pp_clock_style_teletype", &sp->pp_clock_style.teletype);


  /* second panel */
}

void
http_global_panel_update(GeneralPrefs *p)
{
  link_entry_widget(glob.main_win, "comboentry_browser", &p->browser_cmd);
  link_entry_widget(glob.main_win, "comboentry_browser2", &p->browser2_cmd);
  link_entry_widget(glob.main_win, "entry_gogole_search", &p->gogole_search_url);
}

void
http_site_panel_update(SitePrefs *sp)
{
  static int once = 0;
  
  link_entry_widget(glob.main_win, "entry_proxy", &sp->proxy_name);
  link_spin_button(glob.main_win, "spinbutton_proxy_port", &sp->proxy_port);
  link_entry_widget(glob.main_win, "entry_proxy_user", &sp->proxy_auth_user);
  if (once == 0) gtk_entry_set_visibility(GTK_ENTRY(lookup_widget(glob.main_win, "entry_proxy_pass")), FALSE);
  link_entry_widget(glob.main_win, "entry_proxy_pass", &sp->proxy_auth_pass);
  link_toggle_button(glob.main_win, "checkbutton_proxy_no_cache", &sp->proxy_nocache);
  link_toggle_button(glob.main_win, "checkbutton_proxy_disable_if_modified_since", 
		     &sp->use_if_modified_since);

  link_entry_widget(glob.main_win, "entry_path_board_backend", &sp->path_board_backend);
  link_entry_widget(glob.main_win, "entry_path_news_backend", &sp->path_news_backend);
  link_entry_widget(glob.main_win, "entry_path_myposts", &sp->path_myposts);
  link_entry_widget(glob.main_win, "entry_path_board_add", &sp->path_board_add);
  link_entry_widget(glob.main_win, "entry_board_post", &sp->board_post);
  link_entry_widget(glob.main_win, "entry_end_news_url", &sp->path_end_news_url);
  link_entry_widget(glob.main_win, "entry_path_messages", &sp->path_messages);
  link_optionmenu_widget(glob.main_win, "optionmenu_backend_type", &sp->board_backend_type, 1);
  once++;
}

void
global_panels_update(GeneralPrefs *p)
{
  dock_global_panel_update(p);
  news_global_panel_update(p);
  board_global_panel_update(p);
  bigorno_global_panel_update(p);
  palmipede_global_panel_update(p);
  pinnipede_global_panel_update(p);
  http_global_panel_update(p);
}

void
site_panels_update(SitePrefs *sp)
{
  dock_site_panel_update(sp);
  news_site_panel_update(sp);
  board_site_panel_update(sp);
  palmipede_site_panel_update(sp);
  pinnipede_site_panel_update(sp);
  http_site_panel_update(sp);
}


void 
prefs_write_to_file_keylist(FILE *f, int opt, KeyList *hk) {
  if (hk == NULL) return;
  fprintf(f, "%s: ", wmcc_options_strings[opt]);
  for (; hk; hk = hk->next) {
    switch (hk->type) {
    case HK_UA: fprintf(f, "UA:"); break;
    case HK_LOGIN: fprintf(f, "LOGIN:"); break;
    case HK_WORD: fprintf(f, "WORD:"); break;
    case HK_ID: fprintf(f, "ID:"); break;
    case HK_THREAD: fprintf(f, "THREAD:"); break;
    case HK_UA_NOLOGIN: fprintf(f, "UA_NOLOGIN:"); break;
    default: g_assert(0); break;
    }
    fprintf(f, "%d:\"%s\"%s\n", hk->num, hk->key, hk->next ? ", \\" : "");
  }
}

void
prefs_savestr(FILE *f, const char *o, int with_dot, const char *s, const char *default_s) {
  if (s && s[0] && (default_s == NULL || strcmp(s,default_s))) {
    if (with_dot) fprintf(f, ".");
    fprintf(f, "%s: %s\n", o, s);
  }
}

void
prefs_savefontstyle(FILE *f, const char *o, int with_dot, FontStyle *fs, FontStyle *default_fs) {
  if (memcmp(fs,default_fs,sizeof(FontStyle))) {
    if (with_dot) fprintf(f, ".");
    fprintf(f, "%s: %s%s%s%s\n", o, 
            (fs->underlined ? "U" : "."), 
            (fs->slanted ? "I" : "."), 
            (fs->bold ? "B" : "."),
            (fs->teletype ? "T" : "."));
  }
}

#define SAVESTR(o,_s, p) if (p->_s && p->_s[0]) { fprintf(f, "%s: %s\n", o, p->_s); }
#define SAVEBOOL(o,_b, p) fprintf(f, "%s: %s\n", o, (p->_b)?"on":"off");
#define SAVEINT(o,_i, p) fprintf(f, "%s: %d\n", o, p->_i);
#define SAVEPOS(o,_x,_y, p) fprintf(f, "%s: %d:%d\n", o, p->_x, p->_y);
#define SAVECOL(o,_c, p) fprintf(f, "%s: %06x\n", o, p->_c);
#define SAVEBICOL(o, _bic, p) fprintf(f, "%s: %06x %06x\n", o, p->_bic.opaque, p->_bic.transp);


#define G_SAVESTR(o,_s) prefs_savestr(f, wmcc_options_strings[o], 0, p->_s, default_p->_s)
#define G_SAVEBOOL(o,_b) if (p->_b != default_p->_b) { SAVEBOOL(wmcc_options_strings[o], _b, p);}
#define G_SAVEINT(o,_i) if (p->_i != default_p->_i) { SAVEINT(wmcc_options_strings[o], _i, p); }
#define G_SAVEPOS(o,_x,_y) if (p->_x != default_p->_x || p->_y != default_p->_y) \
                           { SAVEPOS(wmcc_options_strings[o],_x,_y, p); }
#define G_SAVECOL(o,_c) if (p->_c != default_p->_c) { SAVECOL(wmcc_options_strings[o],_c, p); }
#define G_SAVEBICOL(o, _bic) if (p->_bic.opaque != default_p->_bic.opaque || p->_bic.transp != default_p->_bic.transp) { SAVEBICOL(wmcc_options_strings[o], _bic, p); }
#define G_SAVEFONTSTYLE(o, _fs) prefs_savefontstyle(f, wmcc_options_strings[o], 0, &(p->_fs), &(default_p->_fs))
#define G_SAVESTRLST(o,lst,nb) if (p->nb) { int i; fprintf(f, "%s: ", wmcc_options_strings[o]); \
      for (i=0; i < (int)p->nb; i++) { fprintf(f, "\"%s\"%s", p->lst[i], (i==(int)p->nb-1)?"\n":", \\\n"); } }
#define G_SAVEKL(o, hk) { prefs_write_to_file_keylist(f, o, p->hk); }

#define DOTIFY(o) (wmcc_options_strings[o]+1)

#define SP_SAVESTR(o,_s) prefs_savestr(f, DOTIFY(o), 1, sp->_s, default_sp->_s)
#define SP_SAVEBOOL(o,_b) if (sp->_b != default_sp->_b) { fprintf(f, ".");  SAVEBOOL(DOTIFY(o),_b, sp); }
#define SP_SAVEINT(o,_i) if (sp->_i != default_sp->_i) { fprintf(f, "."); SAVEINT(DOTIFY(o),_i, sp); }
#define SP_SAVEPOS(o,_x,_y) if (sp->_x != default_sp->_x || sp->_y != default_sp->_y) { fprintf(f, "."); SAVEPOS(DOTIFY(o),_x,_y, sp); }
#define SP_SAVECOL(o,_c) if (sp->_c != default_sp->_c) { fprintf(f, "."); SAVECOL(DOTIFY(o),_c, sp); }
#define SP_SAVEBICOL(o, _bic) if (sp->_bic.opaque != default_sp->_bic.opaque || sp->_bic.transp != default_sp->_bic.transp) { fprintf(f, "."); SAVEBICOL(DOTIFY(o), _bic, sp); }
#define SP_SAVEFONTSTYLE(o, _fs) prefs_savefontstyle(f, DOTIFY(o), 1, &(sp->_fs), &(default_sp->_fs))

void
prefs_write_to_file(GeneralPrefs *p, FILE *f) {
  char default_ua[1024];
  GeneralPrefs _default_p, *default_p;
  SitePrefs _default_sp, *default_sp;
  int site_num;

  /* prefs par defaut, pour reperer les valeurs modifiées */
  memset(&_default_p, 0, sizeof(_default_p));
  memset(&_default_sp, 0, sizeof(_default_sp));
  wmcc_prefs_set_default(&_default_p);
  wmcc_site_prefs_set_default(&_default_sp);
  default_p = &_default_p; default_sp = &_default_sp;

  coincoin_default_useragent(default_ua, 1024);
  G_SAVEINT(OPT_verbosity_underpants, verbosity_underpants);
  G_SAVEINT(OPT_verbosity_http, verbosity_http);
  G_SAVESTR(OPT_font_encoding, font_encoding);
  G_SAVEINT(OPT_tribunenews_max_refresh_delay, max_refresh_delay);
  G_SAVEINT(OPT_tribunenews_switch_off_coincoin_delay, switch_off_coincoin_delay);
  G_SAVEBOOL(OPT_tribune_troll_detector, enable_troll_detector);
  G_SAVEBOOL(OPT_board_auto_dl_pictures, board_auto_dl_pictures);
  G_SAVESTR(OPT_tribune_post_cmd, post_cmd[0]);
  G_SAVEBOOL(OPT_tribune_post_cmd_enabled, post_cmd_enabled[0]);
  G_SAVESTR(OPT_tribune_post_cmd2, post_cmd[1]);
  G_SAVEBOOL(OPT_tribune_post_cmd2_enabled, post_cmd_enabled[1]);
  G_SAVESTR(OPT_tribune_archive, board_scrinechote);
  G_SAVECOL(OPT_dock_bg_color, dock_bgcolor);
  G_SAVESTR(OPT_dock_bg_pixmap, dock_bgpixmap);
  G_SAVESTR(OPT_dock_skin_pixmap, dock_skin_pixmap);
  G_SAVECOL(OPT_dock_fg_color, dock_fgcolor);
  G_SAVEBOOL(OPT_dock_draw_border,draw_border);
  G_SAVEBOOL(OPT_dock_iconwin,use_iconwin);
  G_SAVEBOOL(OPT_palmipede_override_wmanager, palmipede_override_redirect);
  G_SAVEBOOL(OPT_dock_use_balloons,use_balloons);
  G_SAVESTR(OPT_dock_balloons_font_family,balloon_fn_family);
  G_SAVEINT(OPT_dock_balloons_font_size,balloon_fn_size);


  G_SAVEPOS(OPT_dock_pos, dock_xpos, dock_ypos);

  G_SAVEBOOL(OPT_dock_start_in_boss_mode, start_in_boss_mode);
  G_SAVESTR(OPT_news_font_family, news_fn_family);
  G_SAVEINT(OPT_news_font_size, news_fn_size);
  G_SAVECOL(OPT_news_bg_color, news_bgcolor);
  G_SAVECOL(OPT_news_fg_color, news_fgcolor);
  G_SAVECOL(OPT_news_titles_fg_color, news_titles_fgcolor);
  G_SAVECOL(OPT_news_titles_bg_color, news_titles_bgcolor);
  G_SAVECOL(OPT_news_emph_color, news_emph_color);
  G_SAVEPOS(OPT_news_location, news_xpos, news_ypos);
  G_SAVEPOS(OPT_news_dimensions, news_width, news_height);
  G_SAVESTR(OPT_palmipede_default_message, coin_coin_message);
  G_SAVESTR(OPT_http_browser,browser_cmd);
  G_SAVESTR(OPT_http_browser2,browser2_cmd);
  G_SAVESTR(OPT_http_gogole_search_url, gogole_search_url);
  G_SAVEINT(OPT_http_timeout, http_timeout);
  G_SAVEINT(OPT_http_inet_ip_version, http_inet_ip_version);
  G_SAVESTR(OPT_pinnipede_font_family,pp_fn_family);
  G_SAVEINT(OPT_pinnipede_font_size,pp_fn_size);
  G_SAVEBOOL(OPT_pinnipede_start_in_transparency_mode,pp_start_in_transparency_mode);
  G_SAVEBOOL(OPT_pinnipede_use_fake_real_transparency,use_fake_real_transparency);
  G_SAVEBOOL(OPT_pinnipede_use_classical_tabs,pp_use_classical_tabs);
  G_SAVEBOOL(OPT_pinnipede_use_colored_tabs,pp_use_colored_tabs);
  G_SAVEBOOL(OPT_pinnipede_hungry_boitakon,hungry_boitakon);

  if (memcmp(&p->pp_transparency, &default_p->pp_transparency, sizeof(p->pp_transparency))) {
    fprintf(f, "%s: ", wmcc_options_strings[OPT_pinnipede_transparency]);
    switch (p->pp_transparency.type) {
    case FULL_TRANSPARENCY: fprintf(f, "full\n"); break;
    case SHADING: fprintf(f, "shading %d %d\n", 
			  p->pp_transparency.shade.luminosite,
			  p->pp_transparency.shade.assombrissement); break;
    case TINTING: fprintf(f, "tinting %06x %06x\n", 
			  p->pp_transparency.tint.black,
			  p->pp_transparency.tint.white); break;
    default: g_assert(0); break;
    }
  }
  G_SAVEBICOL(OPT_pinnipede_emph_color, pp_emph_color);
  G_SAVEBICOL(OPT_pinnipede_sel_bgcolor, pp_sel_bgcolor);
  G_SAVEBICOL(OPT_pinnipede_popup_fgcolor, pp_popup_fgcolor);
  G_SAVEBICOL(OPT_pinnipede_popup_bgcolor, pp_popup_bgcolor);
  G_SAVEBICOL(OPT_pinnipede_buttonbar_bgcolor, pp_buttonbar_bgcolor);
  G_SAVEBICOL(OPT_pinnipede_buttonbar_fgcolor, pp_buttonbar_fgcolor);
  G_SAVEBICOL(OPT_pinnipede_buttonbar_msgcnt_color, pp_buttonbar_msgcnt_color);
  G_SAVEBICOL(OPT_pinnipede_buttonbar_updlcnt_color, pp_buttonbar_updlcnt_color);
  G_SAVEBICOL(OPT_pinnipede_buttonbar_progressbar_color, pp_buttonbar_progressbar_color);
  G_SAVEBICOL(OPT_pinnipede_hilight_my_msg_color, pp_my_msg_color);
  G_SAVEBICOL(OPT_pinnipede_hilight_answer_my_msg_color, pp_answer_my_msg_color);
  G_SAVEBICOL(OPT_pinnipede_hilight_keyword_color0, pp_keyword_color[0]);
  G_SAVEBICOL(OPT_pinnipede_hilight_keyword_color1, pp_keyword_color[1]);
  G_SAVEBICOL(OPT_pinnipede_hilight_keyword_color2, pp_keyword_color[2]);
  G_SAVEBICOL(OPT_pinnipede_hilight_keyword_color3, pp_keyword_color[3]);
  G_SAVEBICOL(OPT_pinnipede_hilight_keyword_color4, pp_keyword_color[4]);
  G_SAVEBICOL(OPT_pinnipede_plopify_color, pp_plopify_color);
  G_SAVECOL(OPT_pinnipede_fortune_bgcolor, pp_fortune_bgcolor);
  G_SAVECOL(OPT_pinnipede_fortune_fgcolor, pp_fortune_fgcolor);
  G_SAVESTR(OPT_pinnipede_fortune_font_family,pp_fortune_fn_family);
  G_SAVEINT(OPT_pinnipede_fortune_font_size,pp_fortune_fn_size);
  G_SAVEBICOL(OPT_scrollcoin_bg_color,sc_bg_color);
  G_SAVEBICOL(OPT_scrollcoin_bg_light_color,sc_bg_light_color);
  G_SAVEBICOL(OPT_scrollcoin_bg_dark_color,sc_bg_dark_color);
  G_SAVEBICOL(OPT_scrollcoin_arrow_normal_color,sc_arrow_normal_color);
  G_SAVEBICOL(OPT_scrollcoin_arrow_emphasized_color,sc_arrow_emphasized_color);
  G_SAVEBICOL(OPT_scrollcoin_bar_color,sc_bar_color);
  G_SAVEBICOL(OPT_scrollcoin_bar_light_color,sc_bar_light_color);
  G_SAVEBICOL(OPT_scrollcoin_bar_dark_color, sc_bar_dark_color);
  G_SAVEPOS(OPT_pinnipede_location,pp_xpos, pp_ypos);
  G_SAVEPOS(OPT_pinnipede_dimensions,pp_width, pp_height);
  G_SAVEBOOL(OPT_pinnipede_buttons,pp_minibar_on);
  G_SAVEBOOL(OPT_pinnipede_show_tags,pp_html_mode);
  G_SAVEBOOL(OPT_pinnipede_show_seconds,pp_show_sec_mode);
  G_SAVEINT(OPT_pinnipede_nick_mode,pp_nick_mode);
  G_SAVEBOOL(OPT_pinnipede_show_troll_score,pp_trollscore_mode);
  G_SAVEBOOL(OPT_pinnipede_show_fortune,pp_fortune_mode);
  G_SAVEKL(OPT_pinnipede_plop_keywords,plopify_key_list);
  G_SAVEKL(OPT_pinnipede_hilight_keywords,hilight_key_list);
  G_SAVESTRLST(OPT_pinnipede_plop_words,plop_words,nb_plop_words);

  {
    MiniUARule *r;
    for (r = p->miniuarules.first; r; r=r->next) {
      char *s = miniuarule_to_string(r);
      fprintf(f, "%s: %s\n", wmcc_options_strings[OPT_board_miniua_rule], s);
      g_free(s);
    }
  }

  {
    URLReplacement *ur;
    for (ur = p->url_repl.first; ur; ur=ur->next) {
      fprintf(f, "%s: \"%s\" => \"%s\"\n", wmcc_options_strings[OPT_pinnipede_url_replace], 
	      ur->key, ur->repl);
    }
  }

  G_SAVEBOOL(OPT_pinnipede_auto_open,pinnipede_open_on_start);

  G_SAVEBOOL(OPT_spell_enable,ew_do_spell);
  G_SAVESTR(OPT_spell_cmd,ew_spell_cmd);
  G_SAVESTR(OPT_spell_dict,ew_spell_dict);

  g_print("nb_sites = %d\n", Prefs->nb_sites);
  for (site_num = 0; site_num < Prefs->nb_sites; site_num++) {
    SitePrefs *sp = Prefs->site[site_num];
    int i;
    g_print("site = %s\n", sp->all_names[0]);
    fprintf(f, "%s: ", wmcc_options_strings[OPT_site]);
    for (i=0; i < MAX(4,sp->nb_names); i++) if (sp->all_names[i] && strlen(sp->all_names[i])) { fprintf(f, "%s \"%s\" ", i==0?"":",",sp->all_names[i]); }
    fprintf(f,"\n");
    
    {
      char *s = sp->user_cookie;
      while (s && *s) {
	char *ck, *s2;
	s2 = strchr(s, '\n');
	if (s2 == NULL) {
	  ck = strdup(s);
	} else ck = g_strndup(s, s2-s);	
	g_assert(ck);
	if (*ck) {
	  fprintf(f, "%s: %s\n", wmcc_options_strings[OPTS_http_cookie], ck);
	}
	g_free(ck);
	s = s2 ? s2+1 : NULL;
      }
    }
    SP_SAVEBOOL(OPTS_check_news, check_news);
    SP_SAVEBOOL(OPTS_check_board, check_board);
    SP_SAVEBOOL(OPTS_check_comments, check_comments);
    SP_SAVEBOOL(OPTS_check_messages, check_messages);
    fprintf(f, ".%s: %s\n", DOTIFY(OPTSG_locale), sp->locale == locFR ? "fr" : "en");
    SP_SAVEINT(OPTSG_tribune_delay, board_check_delay);
    SP_SAVEINT(OPTSG_tribune_max_messages, board_max_msg);
    SP_SAVEINT(OPTSG_tribune_backend_type, board_backend_type);
    SP_SAVESTR(OPTSG_tribune_wiki_emulation, board_wiki_emulation);
    SP_SAVEINT(OPTSG_news_delay, news_check_delay);
    SP_SAVEINT(OPTSG_news_max_age, news_max_nb_days);
    SP_SAVESTR(OPTSG_palmipede_username, user_name);
    SP_SAVESTR(OPTSG_palmipede_userlogin, user_login);
    if (sp->user_agent == NULL || strcmp(sp->user_agent, default_ua)) {
      SP_SAVESTR(OPTSG_palmipede_useragent, user_agent);
    }
    SP_SAVEINT(OPTSG_palmipede_msg_max_length, palmi_msg_max_len);
    SP_SAVEINT(OPTSG_palmipede_useragent_max_length, palmi_ua_max_len);
    SP_SAVESTR(OPTSG_http_site_url, site_root);
    /*    {
      fprintf(f, ".%s: %s%s%s\n", DOTIFY(OPTSG_http_site_url),
	      sp->site_root, strlen(sp->site_path) ? "/" : "", sp->site_path);
	      }*/
    //SP_SAVESTR(OPTSG_http_site_url, site_root);
    SP_SAVESTR(OPTSG_http_path_tribune_backend, path_board_backend);
    SP_SAVESTR(OPTSG_http_path_news_backend, path_news_backend);
    SP_SAVESTR(OPTSG_http_path_end_news_url, path_end_news_url);
    SP_SAVESTR(OPTSG_http_path_tribune_add, path_board_add);
    SP_SAVESTR(OPTSG_http_board_post, board_post);
    SP_SAVESTR(OPTSG_http_path_myposts, path_myposts);
    SP_SAVESTR(OPTSG_http_path_messages, path_messages);
    SP_SAVEBOOL(OPTSG_http_force_fortune_retrieval, force_fortune_retrieval);
    if (sp->proxy_name && strlen(sp->proxy_name)) {
      fprintf(f, ".%s: %s:%d\n", DOTIFY(OPTSG_http_proxy), sp->proxy_name, sp->proxy_port);
      if (sp->proxy_auth_user && strlen(sp->proxy_auth_user) &&
	  sp->proxy_auth_pass && strlen(sp->proxy_auth_pass)) {
	fprintf(f, ".%s: %s:%s\n", DOTIFY(OPTSG_http_proxy_auth), sp->proxy_auth_user, sp->proxy_auth_pass);
      } 
    }
    SP_SAVEBOOL(OPTSG_http_proxy_use_nocache, proxy_nocache);
    SP_SAVEBOOL(OPTSG_http_use_if_modified_since, use_if_modified_since);
    SP_SAVECOL(OPTSG_pinnipede_bg_color, pp_bgcolor);
    SP_SAVEBICOL(OPTSG_pinnipede_fg_color, pp_fgcolor);
    SP_SAVEBICOL(OPTSG_pinnipede_clock_color, pp_tstamp_color);
    SP_SAVEBICOL(OPTSG_pinnipede_useragent_color, pp_useragent_color);
    SP_SAVEBICOL(OPTSG_pinnipede_login_color, pp_login_color);
    SP_SAVEBICOL(OPTSG_pinnipede_url_color, pp_url_color);
    SP_SAVEBICOL(OPTSG_pinnipede_visited_url_color, pp_visited_url_color);
    SP_SAVEBICOL(OPTSG_pinnipede_trollscore_color, pp_trollscore_color);
    SP_SAVEBICOL(OPTSG_pinnipede_strike_color, pp_strike_color);
    SP_SAVEFONTSTYLE(OPTSG_pinnipede_clock_style, pp_clock_style);
    SP_SAVEFONTSTYLE(OPTSG_pinnipede_login_style, pp_login_style);
    SP_SAVEFONTSTYLE(OPTSG_pinnipede_useragent_style, pp_ua_style);
    SP_SAVEBOOL(OPTSG_pinnipede_use_AM_PM, use_AM_PM);
    SP_SAVEBOOL(OPTS_board_auto_refresh, board_auto_refresh);
    SP_SAVEBOOL(OPTSG_pinnipede_mark_id_gaps, mark_id_gaps);
  }
}

int
save_prefs(gchar *filename, int do_backup) {
  FILE *f;
  char *tmpfname;
  if (do_backup) {
    tmpfname = g_strdup_printf("%s_wmccc_tmp", filename);
  } else {
    tmpfname = strdup(filename);
  }


  f = open_wfile(tmpfname);
  if (f == NULL) {
    char *errmsg = g_strdup_printf("Can't save '%s' : %s", tmpfname, strerror(errno));
    quick_message(errmsg);
    g_free(errmsg);
  } else {
    char *backup[4];
    int i;
    fprintf(f, "### -*- mode: wmccoptions -*-\n### edited by wmccc -- look for *.wmccc.*.bak for backups\n");
    prefs_write_to_file(Prefs, f);
    fclose(f);
    if (do_backup) {
      backup[0] = filename;
      backup[1] = g_strdup_printf("%s.wmccc.bak", filename);
      backup[2] = g_strdup_printf("%s.wmccc.2.bak", filename);
      backup[3] = g_strdup_printf("%s.wmccc.3.bak", filename);
      for (i=3; i >= 1; i--) {
	rename(backup[i-1], backup[i]);
      }
    
      if (rename(tmpfname, filename) == -1) {
	char *errmsg = g_strdup_printf("Couldn't rename '%s' to '%s' : %s", tmpfname, filename, strerror(errno));
	quick_message(errmsg);
	g_free(errmsg);
	rename(backup[1], backup[0]);
      }
      for (i=1; i < 4; i++) {
	g_free(backup[i]);
      }
    }
  }
  g_free(tmpfname);
  return 0;
}

#define WMCCC_RC_STYLE \
"style \"fixed\" = \"default\" { fontset = \"fixed\" " \
"  fg[NORMAL]=\"#000080\" } " \
"widget \"*.GtkCList\" style \"fixed\" " \
"widget \"*.GtkEntry\" style \"fixed\" " 
//"widget \"*.GtkText\" style \"fixed\" " 
int
main (int argc, char *argv[])
{
  char *err;

  glob.current_site = 0;
  glob.main_win = NULL;
  //  gtk_set_locale ();

  umask(077);
  gtk_init (&argc, &argv);

  add_pixmap_directory (WMCCDATADIR);

  //  gtk_rc_parse("wmccc_gtkrc");
  //  gtk_rc_parse_string(WMCCC_RC_STYLE);

  if (argc < 2) {
    glob.options_file = g_strdup_printf("%s/.wmcoincoin/options", getenv("HOME"));
  } else {
    if (strcmp(argv[1], "-wmccpid")==0) {
      g_print("wmc³ launched from wmc²\n"); g_assert(argc == 5);
      glob.wmcc_pid = atoi(argv[2]);
      glob.options_file = strdup(argv[3]);
      glob.tmp_options_file = strdup(argv[4]);
    } else {
      glob.wmcc_pid = 0;
      if (argc != 2) {
	g_print("error, too much arguments");
	return -1;
      }
      glob.options_file = strdup(argv[1]);
      glob.tmp_options_file = NULL;
    }
  }

  glob.updating_labels = 0;
  //  glob.fixed_font = gdk_font_load("-misc-fixed-medium-r-normal-*-*-140-*-*-c-*-iso8859-1");
  //  g_assert(fixed_font);
  Prefs = g_new0(GeneralPrefs, 1);
  wmcc_prefs_set_default(Prefs);

  err = wmcc_prefs_read_options(Prefs, glob.options_file,1);
  if (err) {
    g_print("erreur: %s\n", err);
    return 2;
  }

  if (Prefs->nb_sites == 0) {
    SitePrefs *sp;
    g_print("wow ! no sites ! i make one for you\n");
    sp = g_new0(SitePrefs, 1);
    wmcc_site_prefs_set_default(sp);
    Prefs->nb_sites = 1;
    Prefs->site[0] = sp;
  }

  /* petite bidouille pour les alias de sites et le nom de site */
  {
    int snum;
    for (snum = 0; snum < Prefs->nb_sites; snum++) {
      SitePrefs *sp = Prefs->site[snum];
      int i;
      sp->all_names = g_renew(char *, sp->all_names, MAX(4, sp->nb_names));
      for (i = sp->nb_names; i < 4; i++) sp->all_names[i] = NULL;
      sp->site_name = NULL;
    }
  }

  /*
   * The following code was added by Glade to create one of each component
   * (except popup menus), just so that you see something after building
   * the project. Delete any components that you don't want shown initially.
   */
  glob.main_win = create_main_win ();

  if (glob.tmp_options_file == NULL) {
    gtk_widget_set_state(lookup_widget(glob.main_win, "bt_apply"), GTK_STATE_INSENSITIVE);
  }

  /* glade ne supporte pas les callbacks pour les optionmenus :-/ */
  {
    GtkWidget *option_menu;
    option_menu = lookup_widget(glob.main_win, "optionmenu_site_predefined"); 
    g_assert(option_menu);
    gtk_signal_connect(GTK_OBJECT (GTK_OPTION_MENU(option_menu)->menu),
		       "deactivate", GTK_SIGNAL_FUNC(on_optionmenu_site_selected),
		       NULL);
    option_menu = lookup_widget(glob.main_win, "optionmenu_transp"); 
    g_assert(option_menu);
    gtk_signal_connect(GTK_OBJECT (GTK_OPTION_MENU(option_menu)->menu),
		       "deactivate", GTK_SIGNAL_FUNC(on_optionmenu_transp_selected),
		       NULL);
  }

  gtk_widget_show (glob.main_win);

  gtk_main (); 

  wmcc_prefs_destroy(Prefs);
  return 0;
}

