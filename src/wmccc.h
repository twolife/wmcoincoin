#ifndef WMCCC
#define WMCCC
#include "prefs.h"


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
void quick_message(char *msg);
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
#endif
