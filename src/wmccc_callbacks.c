#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

#include <stdlib.h>
#include <gtk/gtk.h>
#include <signal.h>
#include "wmccc_callbacks.h"
#include "wmccc_interface.h"
#include "wmccc_support.h"
#include "wmccc.h"

/*
void
messagebox(char *msg) {
  GtkWidget *wg, *wgtxt;

  g_print(msg);
  wg = create_dialog_msg_ok();
  wgtxt = lookup_widget(wg, "label_dialog_msg_ok"); g_assert(wgtxt);
  gtk_label_set_text(GTK_LABEL(wgtxt), msg);
  gtk_widget_show(wg);
  }*/

/* ----------------------- COLOR SELECTION TRASH ----------------*/
static void
option_color_selection(GtkWidget *from)
{
  GtkWidget *main_window;
  static GtkWidget *colorselectiondialog = NULL;
  GtkColorSelection *colorsel;
  gdouble colord[4];

  int *col_ptr;

  main_window = gtk_object_get_data(GTK_OBJECT(from), MainWindowKey); g_assert(main_window);
  col_ptr = gtk_object_get_data(GTK_OBJECT(from), ColorPtrKey); g_assert(col_ptr);

  g_print("open_color_selection, color=%06x\n", *col_ptr);
  if (!colorselectiondialog) 
    colorselectiondialog = create_colorselectiondialog();

  gtk_object_set_data(GTK_OBJECT(colorselectiondialog), MainWindowKey, main_window);
  gtk_object_set_data(GTK_OBJECT(colorselectiondialog), ColorPtrKey, col_ptr);
  gtk_object_set_data(GTK_OBJECT(colorselectiondialog), ButtonKey, from);
  colorsel = GTK_COLOR_SELECTION(GTK_COLOR_SELECTION_DIALOG(colorselectiondialog)->colorsel);
  colord[0] = (((*col_ptr) & 0xff0000) >> 16)/255.0;
  colord[1] = (((*col_ptr) & 0x00ff00) >> 8)/255.0;
  colord[2] = (((*col_ptr) & 0x0000ff))/255.0;
  colord[3] = 1.0;
  gtk_color_selection_set_color(colorsel, colord);
  gtk_widget_show(colorselectiondialog);
}

void
on_bt_color_clicked(GtkButton *button, gpointer user_data UNUSED) {
  option_color_selection(GTK_WIDGET(button));
}

void
on_bt_colorsel_ok_clicked              (GtkButton       *button,
                                        gpointer         user_data UNUSED)
{
  GtkColorSelectionDialog *colorseldial;
  gpointer *col_ptr;
  int *color = NULL; 
  int r,g,b;
  gdouble colord[4];

  colorseldial = GTK_COLOR_SELECTION_DIALOG(gtk_widget_get_toplevel (GTK_WIDGET (button)));
  g_assert(colorseldial);
  col_ptr = gtk_object_get_data(GTK_OBJECT (colorseldial), ColorPtrKey); g_assert(col_ptr);
  color = (int*)col_ptr;
  gtk_color_selection_get_color (GTK_COLOR_SELECTION(colorseldial->colorsel),
				 colord);
  
  r = MIN((colord[0]*255 + 0.49),255);
  g = MIN((colord[1]*255 + 0.49),255);
  b = MIN((colord[2]*255 + 0.49),255);
  *color = (r<<16) + (g<<8) + b;
  g_print("color set to : %06x %f %f %f\n", *color, colord[0], colord[1], colord[2]);
  update_widget_bgcolor(GTK_WIDGET(gtk_object_get_data(GTK_OBJECT (colorseldial), ButtonKey)),
			*color);
  gtk_widget_hide(GTK_WIDGET(colorseldial));
}

void
on_bt_colorsel_cancel_clicked(GtkButton *button, gpointer user_data UNUSED) {
  gtk_widget_hide(gtk_widget_get_toplevel (GTK_WIDGET (button)));
}

/* ------------------------------ TOGGLES -------------------------------*/

void
on_checkbutton_toggled(GtkToggleButton *togglebutton, gpointer user_data UNUSED) {
  int *bool_ptr = NULL;
  bool_ptr = gtk_object_get_data(GTK_OBJECT(togglebutton), BoolPtrKey); g_assert(bool_ptr);
  //  g_print("on_checkbutton_toggled: %d ---> %d\n", *bool_ptr,  gtk_toggle_button_get_active(togglebutton));
  *bool_ptr = gtk_toggle_button_get_active(togglebutton);
}

/* ------------------------------ SPIN BUTTONS -------------------------------*/

void
on_spinbutton_changed(GtkEditable *editable, gpointer user_data UNUSED) {
  int *i_ptr = NULL;
  i_ptr = gtk_object_get_data(GTK_OBJECT(editable), IntPtrKey); g_assert(i_ptr);
  *i_ptr = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(editable));
}

static void
option_font_selection(GtkWidget *labl, char **pfn_name, int *pfnsz) {
  GtkWidget *main_window;
  GtkWidget *fontsel;
  main_window = lookup_widget (GTK_WIDGET (labl), "main_win");
  fontsel = create_fontselectiondialog (); g_assert(fontsel);
  gtk_object_set_data (GTK_OBJECT (fontsel),  MainWindowKey, main_window);
  gtk_object_set_data (GTK_OBJECT (fontsel),  LabelKey, labl);
  gtk_object_set_data (GTK_OBJECT (fontsel),  FontNameKey, pfn_name);
  gtk_object_set_data (GTK_OBJECT (fontsel),  FontSizeKey, pfnsz);
  gtk_font_selection_dialog_set_preview_text(GTK_FONT_SELECTION_DIALOG(fontsel),
					     "coin ! coin ! plop ! pikaaaaa !");
  gtk_widget_show(fontsel);
}

/* --------------------  TEXT/ENTRY  -----------------------------*/
void
on_editable_changed(GtkEditable *editable, gpointer user_data UNUSED) {
  unsigned char **ppstr = NULL;
  //  g_print("on_editable_changed\n");
  ppstr = gtk_object_get_data(GTK_OBJECT(editable), StrPtrKey); 

  if (ppstr == NULL) return; /* ça arrive pendant l'initialisation */

  if (*ppstr) g_free(*ppstr); *ppstr = NULL;
  *ppstr = gtk_editable_get_chars(GTK_EDITABLE(editable), 0, -1);
  if (*ppstr) {
    if (strlen(*ppstr)==0) { g_free(*ppstr); *ppstr = NULL; }
    else {
      int i;
      for (i=0; i < (int)strlen(*ppstr); i++) {
	if ((*ppstr)[i] < ' ' && (*ppstr)[i] != '\n') (*ppstr)[i] = ' ';
      }
    }
  }
}

/*-------------------- OPTION MENU (pas gèré par glade :/) -------------*/

void
on_optionmenu_selected(GtkMenuShell *menu_shell,
		       gpointer data UNUSED) {
  GtkWidget *active_item;
  
  int item_index;
  int *i_ptr, shift;
  active_item = gtk_menu_get_active(GTK_MENU(menu_shell));
  item_index = g_list_index(menu_shell->children, active_item);
  i_ptr = gtk_object_get_data(GTK_OBJECT(menu_shell), IntPtrKey); g_assert(i_ptr);
  shift = GPOINTER_TO_INT(gtk_object_get_data(GTK_OBJECT(menu_shell), ShiftKey));
  *i_ptr = item_index+shift;
}

/* --------------------------DOCK PANEL EVENTS --------------------*/
void
on_bt_dock_bgpixmap_clicked(GtkButton *button UNUSED, gpointer user_data UNUSED) {
  g_print("hello\n");
}

void
on_entry_sitename_changed(GtkEditable *editable, gpointer user_data) {
  on_editable_changed(editable, user_data);
  site_notebook_update_labels(Prefs);
}

gboolean
on_main_win_delete_event(GtkWidget *widget UNUSED, GdkEvent *event UNUSED, 
			 gpointer user_data UNUSED) {
  gtk_main_quit();
  return FALSE;
}


void
on_bt_news_font_clicked(GtkButton *button, gpointer user_data UNUSED)
{
  option_font_selection(lookup_widget(GTK_WIDGET(button),"label_news_font"), 
			&Prefs->news_fn_family, &Prefs->news_fn_size);
}


void
on_bt_fontsel_ok_clicked               (GtkButton       *button,
                                        gpointer         user_data UNUSED)
{
  GtkWidget *fontsel, *labl;
  char **pfn_family;
  int *pfn_size;
  gchar *font_name;
  fontsel = gtk_widget_get_toplevel (GTK_WIDGET (button));
   /* If no font is selected, or the font couldn't be loaded, just return. */
  font_name = gtk_font_selection_dialog_get_font_name (GTK_FONT_SELECTION_DIALOG (fontsel));
  if (font_name) {
    int i;
    char *s, *p;
    pfn_family = gtk_object_get_data(GTK_OBJECT(fontsel), FontNameKey); g_assert(pfn_family);
    pfn_size = gtk_object_get_data(GTK_OBJECT(fontsel), FontSizeKey); g_assert(pfn_size);
    labl = gtk_object_get_data(GTK_OBJECT(fontsel), LabelKey); g_assert(labl);
    s = font_name;
    i = 0; while (*s && i != 2) { if (*s == '-') i++; s++; }
    p = s;
    while (*s && i != 3) { if (*s == '-') i++; s++; }
    if (s-p > 1) {
      char name[100];
      if (*pfn_family) g_free(*pfn_family);
      *pfn_family = g_new0(gchar, s-p);
      strncpy(*pfn_family, p, s-p-1);
      while (*s && i != 7) { if (*s == '-') i++; s++; }
      p = s;
      while (*s >= '0' && *s <= '9') s++;
      if (s-p) {
	*pfn_size = MAX(atoi(p), 2);
      }
      g_snprintf(name, 100, "%s / %d", *pfn_family, *pfn_size);
      gtk_label_set_text(GTK_LABEL(labl), name);
    }
  }
  gtk_widget_destroy (gtk_widget_get_toplevel (GTK_WIDGET (button)));
}


void
on_bt_fontsel_cancel_clicked(GtkButton *button, gpointer user_data UNUSED) {
  gtk_widget_destroy (gtk_widget_get_toplevel (GTK_WIDGET (button)));
}


void
on_bt_mua_change_clicked(GtkButton *button, gpointer user_data UNUSED) {
  GtkWidget *clist;
  GtkWidget *wg;
  int row;
  char *s;

  clist = lookup_widget(GTK_WIDGET(button), "clist_miniua"); g_assert(clist);
  wg = lookup_widget(GTK_WIDGET(button), "entry_miniua"); g_assert(wg);
  row = GPOINTER_TO_INT(gtk_object_get_data(GTK_OBJECT(wg), "row number"));
  s = gtk_editable_get_chars(GTK_EDITABLE(wg), 0, -1);
  if (s) {
    MiniUARule r;
    char *err;
    err = string_to_miniuarule(s, &r);
    if (err) {
      g_print("'%s' : \n%s\n", s, err);
      quick_message(err);
      g_free(err);
    } else {
      MiniUARule *old_r;
      old_r = gtk_clist_get_row_data(GTK_CLIST(clist), row);
      if (old_r) {
	r.next = old_r->next;
	miniuarule_clear(old_r);
	memcpy(old_r, &r, sizeof(MiniUARules));
	update_miniua_clist(&Prefs->miniuarules);
      }
    }
    g_free(s);
  }
}


void
on_bt_mua_add_clicked(GtkButton *button, gpointer user_data UNUSED) {
  GtkWidget *clist;
  GtkWidget *wg;
  char *s;

  clist = lookup_widget(GTK_WIDGET(button), "clist_miniua"); g_assert(clist);
  wg = lookup_widget(GTK_WIDGET(button), "entry_miniua"); g_assert(wg);
  s = gtk_editable_get_chars(GTK_EDITABLE(wg), 0, -1);
  if (s) {
    MiniUARule r;
    char *err;
    err = string_to_miniuarule(s, &r);
    if (err) {
      quick_message(err);
      g_free(err);
    } else {
      update_miniua_clist(&Prefs->miniuarules);
    }
    g_free(s);
  }  
}


void
on_bt_mua_delete_clicked(GtkButton *button, gpointer user_data UNUSED) {
  GtkWidget *clist;
  GtkWidget *wg;
  MiniUARule *r;
  int row;

  clist = lookup_widget(GTK_WIDGET(button), "clist_miniua"); g_assert(clist);
  wg = lookup_widget(GTK_WIDGET(button), "entry_miniua"); g_assert(wg);
  row = GPOINTER_TO_INT(gtk_object_get_data(GTK_OBJECT(wg), "row number"));
  r = gtk_clist_get_row_data(GTK_CLIST(clist), row);
  if (r) {
    miniuarules_destroy(&Prefs->miniuarules, r);
    update_miniua_clist(&Prefs->miniuarules);
  }
}


void
on_clist_miniua_select_row(GtkCList *clist, gint row, gint column,
			   GdkEvent *event UNUSED, gpointer user_data UNUSED)
{
  GtkWidget *wg;
  gchar *text;

  gtk_clist_get_text(GTK_CLIST(clist), row, column, &text);
  wg = lookup_widget(GTK_WIDGET(clist), "entry_miniua"); g_assert(wg);
  gtk_entry_set_text(GTK_ENTRY(wg), text); /* ne PAS liberer text */
  gtk_object_set_data(GTK_OBJECT(wg), "row number", GINT_TO_POINTER(row));
}


void
on_notebook_sites_switch_page(GtkNotebook *notebook, GtkNotebookPage *page UNUSED,
			      gint page_num, gpointer user_data UNUSED)
{
  if (glob.updating_labels) return;
  g_print("switch page %d->%d\n",gtk_notebook_get_current_page(notebook),page_num);
  glob.current_site = page_num; g_assert(page_num < Prefs->nb_sites);
  glob.updating_labels++;
  site_panels_update(Prefs->site[glob.current_site]);
  glob.updating_labels--;
}

void
on_bt_mv_site_clicked(GtkButton *button, gpointer user_data UNUSED) {
  int dir = (int)user_data;
  GtkWidget *tabs;
  //  g_print("data: %p\n", user_data);
  tabs = lookup_widget(GTK_WIDGET(button), "notebook_sites"); g_assert(tabs);

  g_print("%d %d\n", gtk_notebook_get_current_page(GTK_NOTEBOOK(tabs)), glob.current_site);
  
  g_assert(gtk_notebook_get_current_page(GTK_NOTEBOOK(tabs)) == glob.current_site);
  
  if (glob.current_site + dir < Prefs->nb_sites && glob.current_site + dir >= 0) {
    SitePrefs *tmp = Prefs->site[glob.current_site];
    Prefs->site[glob.current_site] = Prefs->site[glob.current_site+dir];
    Prefs->site[glob.current_site+dir] = tmp;
    site_notebook_update_labels(Prefs);
    glob.current_site += dir;
    glob.updating_labels++;
    gtk_notebook_set_page(GTK_NOTEBOOK(tabs), glob.current_site);
    glob.updating_labels--;
  }
}

void
on_bt_new_site_clicked(GtkButton *button UNUSED, gpointer user_data UNUSED) {
  SitePrefs *sp;
  if (Prefs->nb_sites == MAX_SITES) return;

  sp = g_new(SitePrefs,1);
  wmcc_site_prefs_set_default(sp);
  Prefs->site[Prefs->nb_sites] = sp;
  Prefs->nb_sites++;
  g_assert(sp->all_names == NULL);
  sp->all_names = g_new0(char *,4);
  sp->all_names[0] = strdup("plop!"); /* mouaif */
  site_notebook_update(Prefs);
}

void
on_bt_remove_site_clicked(GtkButton *button, gpointer user_data UNUSED) {
  GtkWidget *tabs;
  SitePrefs *sp;
  int i;

  if (Prefs->nb_sites == 1) return;
  tabs = lookup_widget(GTK_WIDGET(button), "notebook_sites"); g_assert(tabs);
  g_print("RM page current=%d , site=%d\n",gtk_notebook_get_current_page(GTK_NOTEBOOK(tabs)),glob.current_site);
  g_assert(gtk_notebook_get_current_page(GTK_NOTEBOOK(tabs)) == glob.current_site);
  
  sp = Prefs->site[glob.current_site];
  for (i=glob.current_site; i < Prefs->nb_sites; i++) {
    Prefs->site[i] = Prefs->site[i+1];
  }
  Prefs->nb_sites--;
  glob.updating_labels++;
  gtk_notebook_remove_page(GTK_NOTEBOOK(tabs), glob.current_site);
  glob.updating_labels--;
  site_notebook_update(Prefs);
  wmcc_site_prefs_destroy(sp);
  g_free(sp);
}



void
on_bt_save_clicked(GtkButton *button UNUSED, gpointer user_data UNUSED) {
  save_prefs(glob.options_file, 1);
}

void
on_bt_save_as_clicked(GtkButton *button UNUSED, gpointer user_data UNUSED) {
  static GtkWidget *save_filesel = NULL;
  if (save_filesel == NULL)
    save_filesel = create_fileselection_save();

  gtk_object_set_data (GTK_OBJECT (save_filesel), MainWindowKey, glob.main_win);

  /* If the current document has a filename we use that as the default. */
  if (glob.options_file)
    gtk_file_selection_set_filename(GTK_FILE_SELECTION (save_filesel),
				    glob.options_file);

  gtk_widget_show(save_filesel);
  gdk_window_raise(save_filesel->window);
}

void
on_bt_apply_clicked(GtkButton *button UNUSED, gpointer user_data UNUSED) {
  g_assert(glob.tmp_options_file);
  if (save_prefs(glob.tmp_options_file, 0) == 0) {
    g_assert(glob.wmcc_pid > 0);
    kill(glob.wmcc_pid, SIGUSR2);
  }
}

void
on_bt_cancel_clicked(GtkButton *button UNUSED, gpointer user_data UNUSED) {
  gtk_main_quit();  
}

gboolean
on_fileselection_delete_event(GtkWidget *widget, GdkEvent *event UNUSED,
			      gpointer user_data UNUSED) {
  gtk_widget_hide (gtk_widget_get_toplevel (widget));
  return TRUE;
}


void
on_fileselection_cancel_button_clicked(GtkButton *button, gpointer user_data UNUSED) {
  gtk_widget_hide (gtk_widget_get_toplevel (GTK_WIDGET (button)));
}

void
on_fileselection_ok_button_clicked(GtkButton *button, gpointer user_data UNUSED) {
  GtkWidget *filesel, *main_window;
  gchar *filename;

  filesel = gtk_widget_get_toplevel (GTK_WIDGET (button));
  main_window = gtk_object_get_data (GTK_OBJECT (filesel), MainWindowKey);
  gtk_widget_hide (filesel);
  filename = gtk_file_selection_get_filename (GTK_FILE_SELECTION (filesel));
  if (filename && strlen(filename)) {
    save_prefs(filename,1);
    g_free(glob.options_file); glob.options_file = strdup(filename);
  }
}

void
on_bt_pixmap_clicked(GtkButton *button, gpointer user_data UNUSED) {
  static GtkWidget *xpm_filesel = NULL;
  gchar **pxpm;
  if (xpm_filesel == NULL)
    xpm_filesel = create_fileselection_xpm();

  gtk_object_set_data(GTK_OBJECT (xpm_filesel), MainWindowKey, glob.main_win);
  pxpm = gtk_object_get_data(GTK_OBJECT(button), StrPtrKey); g_assert(pxpm);
  gtk_object_set_data(GTK_OBJECT(xpm_filesel), StrPtrKey, pxpm);
  gtk_object_set_data(GTK_OBJECT(xpm_filesel), ButtonKey, button);
  /* If the current document has a filename we use that as the default. */
  if (*pxpm)
    gtk_file_selection_set_filename(GTK_FILE_SELECTION (xpm_filesel),
				    *pxpm);
  gtk_file_selection_complete(GTK_FILE_SELECTION (xpm_filesel), "*.xpm");
  gtk_widget_show(xpm_filesel);
  gdk_window_raise(xpm_filesel->window);
}


void
on_fileselection_xpm_ok_bt_clicked(GtkButton *button, gpointer user_data UNUSED) {
  GtkWidget *filesel, *main_window, *pixbutton;
  int w,h;
  gchar **pxpm;
  gchar *filename;
  filesel = gtk_widget_get_toplevel (GTK_WIDGET (button));
  main_window = gtk_object_get_data (GTK_OBJECT (filesel), MainWindowKey);
  pxpm = gtk_object_get_data(GTK_OBJECT(filesel), StrPtrKey); g_assert(pxpm);
  pixbutton = gtk_object_get_data(GTK_OBJECT(filesel), ButtonKey); g_assert(pixbutton);
  w = GPOINTER_TO_INT(gtk_object_get_data(GTK_OBJECT(pixbutton), WidthKey)); g_assert(w>0);
  h = GPOINTER_TO_INT(gtk_object_get_data(GTK_OBJECT(pixbutton), HeightKey)); g_assert(h>0);
  gtk_widget_hide (filesel);
  filename = gtk_file_selection_get_filename (GTK_FILE_SELECTION (filesel));
  if (filename && strlen(filename)) {
    if (*pxpm) g_free(*pxpm);
    *pxpm = strdup(filename);
    if (!update_widget_bgpixmap(pixbutton, *pxpm)) {
      g_free(*pxpm); *pxpm = NULL;
    }
  }
}


void
on_fileselection_xpm_cancel_bt_clicked(GtkButton *button, gpointer user_data UNUSED) {
  gtk_widget_hide(gtk_widget_get_toplevel(GTK_WIDGET (button)));  
}


void
on_optionmenu_site_selected(GtkMenuShell *menu_shell,
			    gpointer data UNUSED) {
  GtkWidget *active_item;
  SitePrefs *sp;
  
  enum { NONE, DLFP, WOOF, DLFP2, GLANDIUM, FCPU} item_index;

  active_item = gtk_menu_get_active(GTK_MENU(menu_shell));
  item_index = g_list_index(menu_shell->children, active_item);

  if (item_index != NONE && glob.current_site >= 0 && glob.current_site < Prefs->nb_sites) {
    int i;
    sp = Prefs->site[glob.current_site];

    if (sp->site_root) g_free(sp->site_root); sp->site_root = NULL;
    for (i=0; i < 4; i++) {
      if (sp->all_names[i]) { g_free(sp->all_names[i]); sp->all_names[i] = NULL; }
    }
    if (sp->user_cookie) {
      g_free(sp->user_cookie); sp->user_cookie = NULL;
    }
    switch (item_index) {
    case NONE: break;
    case DLFP: 
      sp->site_root = strdup("http://www.linuxfr.org");
      sp->all_names[0] = strdup("dlfp");
      sp->all_names[1] = strdup("linuxfr");
      sp->user_cookie = strdup("session_id=COIN025coin8778PL0p");
      sp->locale = locFR;
      sp->use_AM_PM = 0;
      sp->palmi_msg_max_len = 255;
      sp->palmi_ua_max_len = 60;
      sp->check_news = 1;
      sp->check_board = 1;
      sp->check_comments = 1;
      sp->check_messages = 1;
      break;
    case WOOF:
      sp->site_root = strdup("http://www.woof.lu");
      sp->all_names[0] = strdup("woof");
      sp->all_names[1] = strdup("wooflu");
      sp->user_cookie = strdup("session_id=COIN025coin8778PL0p");
      sp->locale = locEN;
      sp->use_AM_PM = 0;
      sp->palmi_msg_max_len = 400;
      sp->palmi_ua_max_len = 60;
      sp->check_news = 1;
      sp->check_board = 1;
      sp->check_comments = 1;
      sp->check_messages = 1;
      break;
    case DLFP2:
      sp->site_root = strdup("http://new.linuxfr.org");
      sp->path_board_add = strdup("board.html");
      sp->board_post = strdup("message=%s&section=1");
      sp->all_names[0] = strdup("dlfp2");
      sp->all_names[1] = strdup("linuxfr2");
      sp->user_cookie = strdup("unique_id=COIN;md5=PLOP");
      sp->locale = locFR;
      sp->use_AM_PM = 0;
      sp->palmi_msg_max_len = 255;
      sp->palmi_ua_max_len = 60;
      sp->check_news = 0;
      sp->check_board = 1;
      sp->check_comments = 0;
      sp->check_messages = 0;      
    case GLANDIUM:
      break;
    case FCPU:
      break;
    default:
      break;
    }
    site_panels_update(sp);
  }
}

void
on_optionmenu_transp_selected(GtkMenuShell *menu_shell,
			    gpointer data UNUSED) {
  GtkWidget *active_item;
  int item_index;
  active_item = gtk_menu_get_active(GTK_MENU(menu_shell));
  item_index = g_list_index(menu_shell->children, active_item);
  if (item_index == 0) {
    Prefs->pp_transparency.type = FULL_TRANSPARENCY;
  } else if (item_index == 1) {
    Prefs->pp_transparency.type = SHADING;
  } else if (item_index == 2) {
    Prefs->pp_transparency.type = TINTING;
  }
  update_transparency_fields_state();
}

void
reorder_hk(GtkCList *clist, KeyList **pfirst)
{
  KeyList *first = NULL, *hk, *prev = NULL;
  int row;
  for (row = 0; row < clist->rows; row++) {
    hk = (KeyList*) gtk_clist_get_row_data(clist, row);
    if (first == NULL) {
      first = hk;
    } else {
      prev->next = hk;
    }
    prev = hk;
    hk->next = NULL;
  }
  *pfirst = first;
}


static int
get_optionmenu_choice(char *name) {
  GtkMenu *menu;
  GtkWidget *active_item;
  menu = GTK_MENU(gtk_option_menu_get_menu(GTK_OPTION_MENU(lookup_widget(glob.main_win, name)))); 
  g_assert(menu);
  active_item = gtk_menu_get_active(menu);
  g_assert(active_item);
  return g_list_index (GTK_MENU_SHELL(menu)->children, active_item);
}

static KeyList *
get_hk(int isplop) {
  KeyListType ktype;
  int iktype, categ;
  char *key;
  KeyList *hk;
  iktype = get_optionmenu_choice(isplop ? "optionmenu_kplop_match" : "optionmenu_kemph_match");
  
  switch (iktype) {
  case 0: ktype = HK_LOGIN; break;
  case 1: ktype = HK_UA; break;
  case 2: ktype = HK_WORD; break;
  case 3: ktype = HK_UA_NOLOGIN; break;
  default: ktype = HK_WORD;
  }
  categ = get_optionmenu_choice(isplop ? "optionmenu_kplop_categ" : "optionmenu_kemph_categ");
  key = gtk_editable_get_chars(GTK_EDITABLE(lookup_widget(glob.main_win, isplop ? "entry_kplop_key" : "entry_kemph_key")), 0, -1);
  g_assert(key);
  hk = key_list_add(NULL, key, ktype, categ, 1);
  g_free(key);
  return hk;
}


void
on_bt_klist_up_clicked(GtkButton *button, gpointer user_data) {
  char *clname = (char*)user_data;
  GtkWidget *wg = lookup_widget(GTK_WIDGET(button), clname);
  int rownum = clist_selected_row_number(wg);
  int isplop = strcmp(clname, "clist_kemph");
  if (rownum >= 1) gtk_clist_row_move(GTK_CLIST(wg), rownum, rownum-1);
  gtk_clist_moveto(GTK_CLIST(wg), clist_selected_row_number(wg), 0, .0, .0);
  reorder_hk(GTK_CLIST(wg), isplop ? &Prefs->plopify_key_list : &Prefs->hilight_key_list);
}

void
on_bt_klist_down_clicked(GtkButton *button, gpointer user_data) {
  char *clname = (char*)user_data;
  GtkWidget *wg = lookup_widget(GTK_WIDGET(button), clname);
  int rownum = clist_selected_row_number(wg);
  int isplop = strcmp(clname, "clist_kemph");
  if (rownum >= 0) gtk_clist_row_move(GTK_CLIST(wg), rownum, rownum+1);
  gtk_clist_moveto(GTK_CLIST(wg), clist_selected_row_number(wg), 0, .0, .0);
  reorder_hk(GTK_CLIST(wg), isplop ? &Prefs->plopify_key_list : &Prefs->hilight_key_list);
}

void
on_bt_klist_del_clicked(GtkButton *button, gpointer user_data) {
  char *clname = (char*)user_data;
  GtkWidget *wg = lookup_widget(GTK_WIDGET(button), clname);
  int row = clist_selected_row_number(wg);
  int isplop = strcmp(clname, "clist_kemph");
  KeyList *hk = gtk_clist_get_row_data(GTK_CLIST(wg), row); g_assert(hk);
  hk->next = NULL;
  key_list_destroy(hk);
  gtk_clist_remove(GTK_CLIST(wg), row);
  reorder_hk(GTK_CLIST(wg), isplop ? &Prefs->plopify_key_list : &Prefs->hilight_key_list);
}

void
on_bt_klist_change_clicked(GtkButton *button, gpointer user_data) {
  char *clname = (char*)user_data;
  GtkWidget *wg = lookup_widget(GTK_WIDGET(button), clname);
  int row = clist_selected_row_number(wg);
  int isplop = strcmp(clname, "clist_kemph");
  KeyList *hk, *hk_old = gtk_clist_get_row_data(GTK_CLIST(wg), row); g_assert(hk_old);
  hk = get_hk(isplop);
  if (hk_old && hk) {
    hk_old->next = NULL;
    key_list_destroy(hk_old);
    clist_klist_set_row(GTK_CLIST(wg), hk, row);
    reorder_hk(GTK_CLIST(wg), isplop ? &Prefs->plopify_key_list : &Prefs->hilight_key_list);
  }
}

void
on_bt_klist_add_clicked(GtkButton *button, gpointer user_data) {
  char *clname = (char*)user_data;
  GtkWidget *wg = lookup_widget(GTK_WIDGET(button), clname);
  int isplop = strcmp(clname, "clist_kemph");
  KeyList *hk;
  hk = get_hk(isplop);
  if (hk) {
    char *s[3] = {"","",""};
    int row = gtk_clist_append(GTK_CLIST(wg), s);
    clist_klist_set_row(GTK_CLIST(wg), hk, row);
    reorder_hk(GTK_CLIST(wg), isplop ? &Prefs->plopify_key_list : &Prefs->hilight_key_list);
  }
}

void
on_clist_klist_row_move(GtkCList *clist, gint arg1, gint arg2, gpointer user_data) {
  char *clname = (char*)user_data;
  int rownum = clist_selected_row_number(GTK_WIDGET(clist));
  
  g_print("row_move %d %d %s (%d)\n", arg1, arg2,  clname, rownum);
  reorder_hk(clist, strcmp(clname, "clist_kemph") == 0 ? &Prefs->hilight_key_list : &Prefs->plopify_key_list);
}

void
on_clist_klist_select_row(GtkCList *clist, gint row, gint column,
  GdkEvent *event UNUSED, gpointer user_data) {
  char *clname = (char*)user_data;
  GtkWidget *wg = lookup_widget(GTK_WIDGET(clist), clname);

  clist_klist_row_to_entries(GTK_CLIST(wg), row);
  g_print("select_row %d %d %s (%d)\n", row, column, clname, row);
}

