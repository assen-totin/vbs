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

void format_cell_from(GtkTreeViewColumn *col, GtkCellRenderer *renderer, GtkTreeModel *model, GtkTreeIter *iter, gpointer user_data){
        gint from;
        gchar res[32];
        gtk_tree_model_get(model, iter, COL_FROM, &from, -1);
        convert_time_srt(from, &res[0], 2);
        g_object_set(renderer, "text", res, NULL);
}


void format_cell_to(GtkTreeViewColumn *col, GtkCellRenderer *renderer, GtkTreeModel *model, GtkTreeIter *iter, gpointer user_data){
        gint from;
        gchar res[32];
        gtk_tree_model_get(model, iter, COL_TO, &from, -1);
        convert_time_srt(from, &res[0], 2);
        g_object_set(renderer, "text", res, NULL);
}


void *create_view_and_model (void){
        GtkCellRenderer     *renderer;
        GtkTreeModel        *model;
        GtkTreeIter    iter;
        GtkTreeViewColumn *column;

        config.vbsm.subtitles_view = gtk_tree_view_new();

        config.vbsm.mplayer_store = gtk_list_store_new(NUM_COLS, G_TYPE_STRING, G_TYPE_UINT, G_TYPE_UINT);

        /* --- Column #1 --- */
        renderer = gtk_cell_renderer_text_new();
        gtk_tree_view_insert_column_with_attributes (GTK_TREE_VIEW (config.vbsm.subtitles_view), -1, "From", renderer, "text", COL_FROM, NULL);
        column = gtk_tree_view_get_column (GTK_TREE_VIEW (config.vbsm.subtitles_view), 0);
        gtk_tree_view_column_set_cell_data_func(column, renderer, format_cell_from, NULL, NULL);

        /* --- Column #2 --- */
        renderer = gtk_cell_renderer_text_new();
        gtk_tree_view_insert_column_with_attributes (GTK_TREE_VIEW (config.vbsm.subtitles_view), -1, "To", renderer, "text", COL_TO, NULL);
        column = gtk_tree_view_get_column (GTK_TREE_VIEW (config.vbsm.subtitles_view), 1);
        gtk_tree_view_column_set_cell_data_func(column, renderer, format_cell_to, NULL, NULL);

        /* --- Column #3 --- */
        renderer = gtk_cell_renderer_text_new();
        gtk_tree_view_insert_column_with_attributes (GTK_TREE_VIEW (config.vbsm.subtitles_view), -1, "Text", renderer, "text", COL_LINE, NULL);

        // Editable cells
        g_object_set(renderer, "editable", TRUE, NULL);
        g_signal_connect(renderer, "edited", (GCallback) cell_edit, config.vbsm.subtitles_view);
        
        gtk_list_store_append (config.vbsm.mplayer_store, &iter);
        gtk_list_store_set (config.vbsm.mplayer_store, &iter, COL_LINE, " ", COL_FROM, 0, COL_TO, 0, -1);
        
        model = GTK_TREE_MODEL(config.vbsm.mplayer_store);
        gtk_tree_view_set_model (GTK_TREE_VIEW (config.vbsm.subtitles_view), model);

        /* The tree view has acquired its own reference to the
         *  model, so we can drop ours. That way the model will
         *  be freed automatically when the tree view is destroyed 
         */
        g_object_unref (model);
}

int progress_bar_update() {
	long curr_time_msec = get_time_msec();

	// Update progress bar
	if (config.common.inside_sub == TRUE) {
		gdouble frac = (gdouble) (curr_time_msec - config.common.timestamp_msec) / (gdouble) (1000 * (config.vbsm.progress_seconds - 1));
		if (frac <= 1) {
			gtk_progress_bar_set_fraction (GTK_PROGRESS_BAR(config.vbsm.progress), frac);
		}
		else {
			div_t time_diff = div(curr_time_msec - config.common.timestamp_msec, 1000);
			if (time_diff.rem < config.vbsm.progress_update_msec) {
				char line2[255];
				sprintf(line2, _("Suggested Duration: %u seconds (So far: %u seconds)"), config.vbsm.progress_seconds - 1, time_diff.quot);
				gtk_progress_bar_set_text(GTK_PROGRESS_BAR(config.vbsm.progress), line2);
			}
		}
	}

        GtkTreeSelection *selection;
        GtkTreeModel *model;
        GtkTreeIter iter;
        gint from, to, local;
	gchar *line;

	// If only playing the movie, move to next subtitle as it gets displayed - until a 'b' is pressed
	// Because this section queries the player (which is an overhead, especially on MPlayer), limit it to once a second.
	div_t msec = div(curr_time_msec, 1000);
	if ((config.common.running == TRUE) && (config.common.inside_sub == FALSE) && (msec.rem < config.vbsm.progress_update_msec)) {
		selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(config.vbsm.subtitles_view));
		if (gtk_tree_selection_get_selected(selection, &model, &iter)) {
			gtk_tree_model_get(model, &iter, COL_FROM, &from, COL_TO, &to, COL_LINE, &line, -1);

			if (config.vbsm.video_backend == VBSM_VIDEO_BACKEND_MPLAYER) {
#ifdef HAVE_MPLAYER
				if (mplayer_is_alive())
					local = mplayer_get_time_pos(2);
				else {
					long curr_time_msec = get_time_msec();
					local = curr_time_msec - config.common.init_timestamp_msec;
				}
#endif
			}

			else if (config.vbsm.video_backend == VBSM_VIDEO_BACKEND_GSTREAMER) {
#ifdef HAVE_GSTREAMER
				if (config.vbsm.have_loaded_video)
					local = gstreamer_query_position();

				if ((!config.vbsm.have_loaded_video) || (local == -1)) {
					long curr_time_msec = get_time_msec();
					local = curr_time_msec - config.common.init_timestamp_msec;
				}
#endif
			}

			// If using GStreamer, show the sub while inside
                        if ((config.vbsm.video_backend == VBSM_VIDEO_BACKEND_GSTREAMER) && (local > from) && (local < to)) {
#ifdef HAVE_GSTREAMER
				if (config.vbsm.have_loaded_video) {
	                                char line3[1024];
        	                        strcpy(&line3[0], line);
                	                gstreamer_sub_set(line3);
				}
#endif
                         }

			// If out of the sub, move the list to next
			if ((to > from) && (local > to)) {
				if (gtk_tree_model_iter_next(model, &iter)) {
					// Move to next line
					gtk_tree_selection_select_iter(selection, &iter);

					// Scroll down
					GtkTreePath *path = gtk_tree_model_get_path (model, &iter);
					gtk_tree_view_scroll_to_cell (GTK_TREE_VIEW(config.vbsm.subtitles_view), path, NULL, TRUE, 0.5, 0);

				}
				if (config.vbsm.video_backend == VBSM_VIDEO_BACKEND_GSTREAMER) {
#ifdef HAVE_GSTREAMER
					if (config.vbsm.have_loaded_video)
						gstreamer_sub_clear();
#endif
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

	gtk_container_add(GTK_CONTAINER(gtk_dialog_get_content_area(GTK_DIALOG(warnDialog))), warnLabel);

	g_signal_connect (G_OBJECT(buttonOK), "clicked", G_CALLBACK (quitDialogCancel), (gpointer) warnDialog);

	gtk_widget_show_all (warnDialog);
}


