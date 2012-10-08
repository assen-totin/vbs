// This file is a part of Voody Blue Subtitler suit.
// 
// Author: Assen Totin <assen.totin@gmail.com>
//
// Home page: http://bilbo.online.bg/~assen/vbs
//
// This software is released under GNU General Public License.
// See the LICENSE file for details or visit http://www.gnu.org/copyleft/gpl.html 
// for details.

#include "../common/common.h"

int progressBarUpdate() {
	// Update progress bar
	if (config.common.inside_sub == TRUE) {
		time_t curr_time = time(NULL);
		gdouble frac = (gdouble) (curr_time - config.common.timestamp) / (gdouble) (config.vbsm.progress_seconds - 1);
		if (frac <= 1) {
			gtk_progress_bar_set_fraction (GTK_PROGRESS_BAR(config.vbsm.progress), frac);
		}
		else {
			char line2[255];
			sprintf(line2,"Suggested Duration: %u seconds (So far: %u seconds)", config.vbsm.progress_seconds - 1, curr_time - config.common.timestamp);
			gtk_progress_bar_set_text(GTK_PROGRESS_BAR(config.vbsm.progress), line2);
		}
	}

	// If only playing the movie, move to next subtitle as it gets displayed - until a 'b' is pressed
	if ((config.common.running == TRUE) && (config.common.inside_sub == FALSE)){
		GtkTreeSelection *selection;
		GtkTreeModel     *model;
		GtkTreeIter       iter;
		gint from, to, local;

		selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(view));
		if (gtk_tree_selection_get_selected(selection, &model, &iter)) {
			gtk_tree_model_get(model, &iter, COL_FROM, &from, COL_TO, &to, -1);
			if (mplayer_is_alive())
				local = mplayer_get_time_pos(2);
			else {
				time_t curr_time = time(NULL);
				local = 1000*(curr_time - config.common.init_timestamp);
			}
			if ((to > from) && (to < local)) {
				if (gtk_tree_model_iter_next(model, &iter)) {
					// Move to next line
					gtk_tree_selection_select_iter(selection, &iter);

					// Scroll down
					GtkTreePath *path = gtk_tree_model_get_path (model, &iter);
					gtk_tree_view_scroll_to_cell (GTK_TREE_VIEW(view), path, NULL, TRUE, 0.5, 0);
				}
			}
		}
	}

	return 1;
}


void warnDialog(GtkWidget *window, char warning[1024]) {
	GtkWidget *warnDialog, *warnLabel;

	warnDialog = gtk_dialog_new_with_buttons (VBS_WARNING, GTK_WINDOW(window), GTK_DIALOG_MODAL, NULL);

	GtkWidget *buttonOK = gtk_dialog_add_button (GTK_DIALOG(warnDialog), GTK_STOCK_OK, GTK_RESPONSE_OK);
	gtk_dialog_set_default_response (GTK_DIALOG (warnDialog), GTK_RESPONSE_OK) ;

	warnLabel = gtk_label_new(warning);

	gtk_container_add (GTK_CONTAINER (GTK_DIALOG(warnDialog)->vbox), warnLabel);

	g_signal_connect (G_OBJECT(buttonOK), "clicked", G_CALLBACK (quitDialogCancel), (gpointer) warnDialog);

	gtk_widget_show_all (warnDialog);
}


