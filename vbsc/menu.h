// This file is a part of Voody Blue Subtitler suit.
// 
// Author: Assen Totin <assen.totin@gmail.com>
//
// Home page: http://bilbo.online.bg/~assen/vbs
//
// This software is released under GNU General Public License.
// See the LICENSE file for details or visit http://www.gnu.org/copyleft/gpl.html 
// for details.

void file_dialog(GtkAction *action, gpointer param);
void file_dialog_ok_21( GtkWidget *fileDialogWidget, GtkFileSelection *fs );
void file_dialog_ok_41( GtkWidget *fileDialogWidget, GtkFileSelection *fs );

void quit_dialog(GtkAction *action, gpointer param);
static void quit_dialog_ok( GtkWidget *widget, gpointer data );

void set_font_size (GtkAction *action, gpointer param);
void set_font_size_ok(GtkWidget *widget, gpointer data);
void set_magic_key (GtkAction *action, gpointer param);
void set_magic_key_ok(GtkWidget *widget, gpointer data);
void set_full_screen (GtkAction *action, gpointer param);
void set_full_screen_ok(GtkWidget *widget, gpointer data);

#define VBSC_MENU_MAGIC_KEY_TITLE "Set Magic Key"
#define VBSC_MENU_MAGIC_KEY_TEXT "Enter magic key (a number between 1 and 2147483647):"
#define VBSC_MENU_FONT_SIZE_TITLE "Set Font Size"
#define VBSC_MENU_FONT_SIZE_TEXT "Enter font size (in points):"
#define VBSC_MENU_FULL_SCREEN_TITLE "Set Full-Screen"
#define VBSC_MENU_FULL_SCREEN_TEXT "Full-Screen mode:"

