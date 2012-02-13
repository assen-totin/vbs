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

void view_onBPressed () {
	GtkTreeSelection *selection;
	GtkTreeModel     *model;
	GtkTreeIter       iter;

	selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(view));
	if (gtk_tree_selection_get_selected(selection, &model, &iter)) {
		if ((counter.running == TRUE) && (counter.inside_sub == FALSE)) {
			gint new_from = getTimePos(2);
			gtk_list_store_set (GTK_LIST_STORE(model), &iter, COL_FROM, new_from, -1);

			counter.timestamp = time(NULL);
			counter.inside_sub = TRUE;

			gchar *line;

			gtk_tree_model_get(model, &iter, COL_LINE, &line, -1);

			div_t q = div(strlen(line),20);
			counter.progress_seconds = q.quot + 2;

			char line2[255];
			sprintf(line2,"Suggested Duration: %u seconds", q.quot + 1);

			gtk_progress_bar_set_text(GTK_PROGRESS_BAR(counter.progress), line2);

			g_free(line);
			fprintf(counter.tmpFile, "Processed B key.\n");
		}
	}
}


void view_onMPressed () {
	GtkTreeSelection *selection;
	GtkTreeModel     *model;
	GtkTreeIter       iter;

	selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(view));
	if (gtk_tree_selection_get_selected(selection, &model, &iter)) {
		if ((counter.running == TRUE) && (counter.inside_sub == TRUE)){

			gint new_to = getTimePos(2);
			gtk_list_store_set (GTK_LIST_STORE(model), &iter, COL_TO, new_to, -1);

			// Move to next line
			gtk_tree_selection_unselect_iter(selection, &iter);
			gtk_tree_model_iter_next(model,&iter);
			gtk_tree_selection_select_iter(selection, &iter);

			// Scroll down
			GtkTreePath *path = gtk_tree_model_get_path (model, &iter);
			gtk_tree_view_scroll_to_cell (GTK_TREE_VIEW(view), path, NULL, TRUE, 0.5, 0);

			counter.inside_sub = FALSE;

			char line[16];
			sprintf(line," ");
			gtk_progress_bar_set_text(GTK_PROGRESS_BAR(counter.progress), line);
			gtk_progress_bar_set_fraction (GTK_PROGRESS_BAR(counter.progress), 0);

			// Export subtitles
			exportSubtitles();
		}
		else { 
			// Do nothing; if this is not present, the GTK widget will pop-up a small window and move the focus to it. WTF?
			int empty = 0;
		}
	}
}


void view_onSpacePressed (GtkWidget *window) {
	GtkTreeSelection *selection;
	GtkTreeModel     *model;
	GtkTreeIter       iter;

	if ((counter.running == TRUE) && (counter.inside_sub == FALSE)) {
		// Pause the player
		if (mplayerAlive()) {writeMPlayer("pause");}

		counter.running = FALSE;

		// Set status
		gtk_statusbar_push(GTK_STATUSBAR(counter.status), counter.status_context_id, "Status: PAUSED");
	}
	else if (counter.running == FALSE) {
		if (haveLoadedText(window) && haveLoadedVideo(window)) {
			// Start the player
			if (mplayerAlive()) {writeMPlayer("pause");}

			counter.running = TRUE;

			// Set status
			gtk_statusbar_push(GTK_STATUSBAR(counter.status), counter.status_context_id, "Status: RUNNING");
		}
	}
}


void view_onKeyPressed (GtkTreeView *view, GdkEventKey *event, gpointer userdata) {
	GtkWidget *window = userdata;
	switch ( event->keyval ) {
		case GDK_b:
			view_onBPressed();
			break;
		case GDK_m:
			view_onMPressed();
			break;
		case GDK_n:
			view_onMPressed();
			view_onBPressed();
			break;
		case GDK_space:
			view_onSpacePressed(window);
			break;
		case GDK_s:
			exportSubtitles();
			break;
	}
}


