#ifndef WMCCC_H
#define WMCCC_H

#define IN_WMCCC
#include <gtk/gtk.h>
#include "prefs.h"
#include "options_list.h"
#include "coin_util.h"


#ifdef __GNUC__
# define UNUSED __attribute((unused))
#else
# define UNUSED
#endif


typedef struct _strucGlob {
  GtkWidget *main_win;
  GdkFont *fixed_font;
  int current_site;
  int updating_labels;
  
  pid_t wmcc_pid;
  char *options_file;
  char *tmp_options_file;
  int nb_selected_sites;
  SitePrefs *selected_sites[MAX_SITES];
  GdkColor modif_widget_color;
} strucGlob;

#ifndef GLOBALS_HERE
extern GeneralPrefs *Prefs;
extern strucGlob glob;
#else
GeneralPrefs *Prefs;
strucGlob glob;
#endif

#define ColorPtrKey  "ColorPtr"
#define BoolPtrKey  "BoolPtr"
#define IntPtrKey  "IntPtr"
#define StrPtrKey  "StrPtr"
#define ButtonKey    "ButtonPtr"
#define WidthKey  "Widthkey"
#define HeightKey  "Heightkey"
#define LabelKey     "LabelPtr"
#define FontNameKey  "FontNamePtr"
#define FontSizeKey  "FontSizePtr"
#define MainWindowKey "MainWindow"
#define ShiftKey "ShiftKey"
gint clist_selected_row_number(GtkWidget *clist);
void update_widget_bgcolor(GtkWidget *w, int icol);
int update_widget_bgpixmap(GtkWidget *w, char *pixname);
void update_transparency_fields_state();
void update_miniua_clist(MiniUARules *urs);
void site_panels_update(SitePrefs *sp);
void global_panels_update(GeneralPrefs *p);
void site_notebook_update_labels(GeneralPrefs *p);
void site_notebook_update(GeneralPrefs *p);
int save_prefs(gchar *filename, int do_backup);
void on_optionmenu_selected(GtkMenuShell *menu_shell, gpointer data);
void on_optionmenu_site_selected(GtkMenuShell *menu_shell, gpointer data);
void on_optionmenu_transp_selected(GtkMenuShell *menu_shell, gpointer data);
void clist_klist_set_row(GtkCList *clist, KeyList *hk, int row);
void clist_klist_row_to_entries(GtkCList *clist, int row);
void on_bt_color_draw(GtkWidget *widget, GdkRectangle *area, gpointer user_data);
gboolean on_bt_color_expose_event(GtkWidget *widget, GdkEventExpose  *event, gpointer user_data UNUSED);
void quick_message(gchar *message_fmt, ...);
void prefs_write_to_file(GeneralPrefs *p, FILE *f);
#endif
