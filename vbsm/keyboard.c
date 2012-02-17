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

void view_onBPressed () {
	GtkTreeSelection *selection;
	GtkTreeModel *model;
	GtkTreeIter iter;
	gint new_from;

	selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(view));
	if (gtk_tree_selection_get_selected(selection, &model, &iter)) {
		if ((config.common.running == TRUE) && (config.common.inside_sub == FALSE)) {
			if (mplayerAlive())
				new_from = getTimePos(2);
			else {
				time_t curr_time = time(NULL);
				new_from = config.common.init_timestamp - curr_time;
			}

			gtk_list_store_set(GTK_LIST_STORE(model), &iter, COL_FROM, new_from, -1);

			config.common.timestamp = time(NULL);
			config.common.inside_sub = TRUE;

			gchar *line;
			gtk_tree_model_get(model, &iter, COL_LINE, &line, -1);

			div_t q = div(strlen(line),20);
			config.vbsm.progress_seconds = q.quot + 2;

			char line2[255];
			sprintf(line2,"Suggested Duration: %u seconds", q.quot + 1);

			gtk_progress_bar_set_text(GTK_PROGRESS_BAR(config.vbsm.progress), line2);

			// If networking is enabled, send the line to server
			if (config.common.use_network == 1)
				put_subtitle(line);

			g_free(line);

			fprintf(config.vbsm.tmpFile, "Processed B key.\n");
		}
	}
}


void view_onMPressed () {
	GtkTreeSelection *selection;
	GtkTreeModel     *model;
	GtkTreeIter       iter;
	gint new_to;

	selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(view));
	if (gtk_tree_selection_get_selected(selection, &model, &iter)) {
		if ((config.common.running == TRUE) && (config.common.inside_sub == TRUE)){
			if (mplayerAlive())
				new_to = getTimePos(2);
			else {
				time_t curr_time = time(NULL);
				new_to = config.common.init_timestamp - curr_time;
			}

			gtk_list_store_set(GTK_LIST_STORE(model), &iter, COL_TO, new_to, -1);

			// Move to next line
			gtk_tree_selection_unselect_iter(selection, &iter);
			gtk_tree_model_iter_next(model,&iter);
			gtk_tree_selection_select_iter(selection, &iter);

			// Scroll down
			GtkTreePath *path = gtk_tree_model_get_path (model, &iter);
			gtk_tree_view_scroll_to_cell(GTK_TREE_VIEW(view), path, NULL, TRUE, 0.5, 0);

			config.common.inside_sub = FALSE;

			char line[16];
			sprintf(line," ");
			gtk_progress_bar_set_text(GTK_PROGRESS_BAR(config.vbsm.progress), line);
			gtk_progress_bar_set_fraction (GTK_PROGRESS_BAR(config.vbsm.progress), 0);

			// If networking is enabled, drop the line from server
			if (config.common.use_network == 1)
				put_subtitle("\n");

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

	if ((config.common.running == TRUE) && (config.common.inside_sub == FALSE)) {
		// Pause the player
		if (mplayerAlive()) 
			writeMPlayer("pause");

		config.common.running = FALSE;

		// Set status
		gtk_statusbar_push(GTK_STATUSBAR(config.vbsm.status), config.vbsm.status_context_id, "Status: PAUSED");
	}
	else if (config.common.running == FALSE) {
		if (haveLoadedText(window) && haveLoadedVideo(window)) {
			// Start the player
			if (mplayerAlive()) 
				writeMPlayer("pause");

			config.common.running = TRUE;

			// Set status
			gtk_statusbar_push(GTK_STATUSBAR(config.vbsm.status), config.vbsm.status_context_id, "Status: RUNNING");
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


