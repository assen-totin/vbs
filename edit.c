// This file is a part of Voody Blue Subtitler.
// 
// Author: Assen Totin <assen@online.bg>
//
// Home page: http://bilbo.online.bg/~assen/vbs
//
// This software is released under GNU General Public License.
// See the LICENSE file for details or visit http://www.gnu.org/copyleft/gpl.html 
// for details.

#include "main.h"

void view_cellEdit(GtkCellRendererText *cell, gchar *path_string, gchar *new_text, gpointer notUsed) {
 GtkTreeIter iter;
 GtkTreeModel *model;
 GtkTreeSelection *selection;

 selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(view));
 gtk_tree_selection_get_selected(selection, &model, &iter);

 if (gtk_tree_model_get_iter_from_string (model, &iter, path_string)) {
  gtk_list_store_set (GTK_LIST_STORE(model), &iter, COL_LINE, new_text, -1);
 }
}

