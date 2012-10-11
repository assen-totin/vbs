// This file is a part of Voody Blue Subtitler suit.
// 
// Author: Assen Totin <assen.totin@gmail.com>
//
// Home page: http://bilbo.online.bg/~assen/vbs
//
// This software is released under GNU General Public License.
// See the LICENSE file for details or visit http://www.gnu.org/copyleft/gpl.html 
// for details.

void file_dialog_open(GtkAction *action, gpointer param);
void file_dialog_save(GtkAction *action, gpointer param);

void quit_dialog(GtkAction *action, gpointer param);
static void quit_dialog_ok( GtkWidget *widget, gpointer data );

void set_font_size (GtkAction *action, gpointer param);
void set_font_size_ok(GtkWidget *widget, gpointer data);
void set_magic_key (GtkAction *action, gpointer param);
void set_magic_key_ok(GtkWidget *widget, gpointer data);
void set_full_screen (GtkAction *action, gpointer param);
void set_full_screen_ok(GtkWidget *widget, gpointer data);


