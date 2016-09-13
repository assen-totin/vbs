// This file is a part of Voody Blue Subtitler suit.
// 
// Author: Assen Totin <assen.totin@gmail.com>
//
// Home page: http://www.zavedil.com/software-desktop-vbs
//
// This software is released under GNU General Public License.
// See the LICENSE file for details or visit http://www.gnu.org/copyleft/gpl.html 
// for details.

void on_clicked_row (GtkTreeView *view, GtkTreePath *path, GtkTreeViewColumn *col, gpointer userdata);
void on_clicked_button_video (GtkButton *button, gpointer user_data);
void on_clicked_button_sub (GtkButton *button, gpointer user_data);
void on_clicked_button_play (GtkButton *button, gpointer user_data);
int calc_new_time(long curr_time, int offset);
