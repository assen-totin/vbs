// This file is a part of Voody Blue Subtitler suit.
// 
// Author: Assen Totin <assen.totin@gmail.com>
//
// Home page: http://www.zavedil.com/software-desktop-vbs
//
// This software is released under GNU General Public License.
// See the LICENSE file for details or visit http://www.gnu.org/copyleft/gpl.html 
// for details.

#include "../common/common.h"

void format_cell_from(GtkTreeViewColumn *col, GtkCellRenderer *renderer, GtkTreeModel *model, GtkTreeIter *iter, gpointer user_data){
	gint from;
	gchar res[32];
	gtk_tree_model_get(model, iter, COL_FROM, &from, -1);
	convert_time_to_srt(from, &res[0], TIME_SEC);
	g_object_set(renderer, "text", res, NULL);
}


void format_cell_to(GtkTreeViewColumn *col, GtkCellRenderer *renderer, GtkTreeModel *model, GtkTreeIter *iter, gpointer user_data){
	gint from;
	gchar res[32];
	gtk_tree_model_get(model, iter, COL_TO, &from, -1);
	convert_time_to_srt(from, &res[0], TIME_SEC);
	g_object_set(renderer, "text", res, NULL);
}


void *create_view_and_model (void){
	GtkCellRenderer *renderer;
	GtkTreeModel *model;
	GtkTreeIter iter;
	GtkTreeViewColumn *column;

	config.vbsm.subtitles_view = gtk_tree_view_new();

	config.vbsm.mplayer_store = gtk_list_store_new(NUM_COLS, G_TYPE_STRING, G_TYPE_UINT, G_TYPE_UINT);

	// Column 1
	renderer = gtk_cell_renderer_text_new();
	gtk_tree_view_insert_column_with_attributes (GTK_TREE_VIEW (config.vbsm.subtitles_view), -1, "From", renderer, "text", COL_FROM, NULL);
	column = gtk_tree_view_get_column (GTK_TREE_VIEW (config.vbsm.subtitles_view), 0);
	gtk_tree_view_column_set_cell_data_func(column, renderer, format_cell_from, NULL, NULL);

	// Column 2
	renderer = gtk_cell_renderer_text_new();
	gtk_tree_view_insert_column_with_attributes (GTK_TREE_VIEW (config.vbsm.subtitles_view), -1, "To", renderer, "text", COL_TO, NULL);
	column = gtk_tree_view_get_column (GTK_TREE_VIEW (config.vbsm.subtitles_view), 1);
	gtk_tree_view_column_set_cell_data_func(column, renderer, format_cell_to, NULL, NULL);

	// Column 3 
	renderer = gtk_cell_renderer_text_new();
	gtk_tree_view_insert_column_with_attributes (GTK_TREE_VIEW (config.vbsm.subtitles_view), -1, "Text", renderer, "text", COL_LINE, NULL);

	// Editable cells
	g_object_set(renderer, "editable", TRUE, NULL);
	g_signal_connect(renderer, "edited", (GCallback) cell_edit, config.vbsm.subtitles_view);
	
	gtk_list_store_append (config.vbsm.mplayer_store, &iter);
	gtk_list_store_set (config.vbsm.mplayer_store, &iter, COL_LINE, " ", COL_FROM, 0, COL_TO, 0, -1);
	
	model = GTK_TREE_MODEL(config.vbsm.mplayer_store);
	gtk_tree_view_set_model (GTK_TREE_VIEW (config.vbsm.subtitles_view), model);

	// The tree view has acquired its own reference to the  model, so we can drop ours. 
	// That way the model will be freed automatically when the tree view is destroyed 
	g_object_unref (model);
}

int progress_bar_update() {
	long curr_time_msec = get_time_msec();

	// Update progress bar
	if (config.common.inside_sub == TRUE) {
		gdouble frac = (gdouble) (curr_time_msec - config.common.timestamp_msec) / (gdouble) (1000 * (config.vbsm.progress_seconds));
		if (frac <= 1) {
			gtk_progress_bar_set_fraction (GTK_PROGRESS_BAR(config.vbsm.progress), frac);
		}
		else {
			div_t time_diff = div(curr_time_msec - config.common.timestamp_msec, 1000);
			if (time_diff.rem < config.vbsm.progress_update_msec) {
				char line2[255];
				sprintf(line2, _("Suggested Duration: %u seconds (So far: %u seconds)"), config.vbsm.progress_seconds, time_diff.quot);
				gtk_progress_bar_set_text(GTK_PROGRESS_BAR(config.vbsm.progress), line2);
				gtk_progress_bar_set_fraction (GTK_PROGRESS_BAR(config.vbsm.progress), 1);
			}
		}
	}

	// If only playing the movie, move to next subtitle as it gets displayed - until a 'b' is pressed
	// Because this section queries the player (which is an overhead, especially on MPlayer), limit it to once a second.
	div_t msec = div(curr_time_msec, 1000);
	if ((config.common.running == TRUE) && (config.common.inside_sub == FALSE) && (msec.rem < config.vbsm.progress_update_msec)) {
        	GtkTreeModel *model;
	        GtkTreeIter iter;
        	gint from, to, local = -1;
	        gchar *line;

		GtkTreeSelection *selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(config.vbsm.subtitles_view));
		if (gtk_tree_selection_get_selected(selection, &model, &iter)) {
			gtk_tree_model_get(model, &iter, COL_FROM, &from, COL_TO, &to, COL_LINE, &line, -1);
#ifdef HAVE_MPLAYER
			if (config.vbsm.video_backend == VBSM_VIDEO_BACKEND_MPLAYER) {
				if (mplayer_is_alive())
					local = mplayer_get_time_pos(2);
			}
#endif
#ifdef HAVE_GSTREAMER
			if (config.vbsm.video_backend == VBSM_VIDEO_BACKEND_GSTREAMER) {
				if (config.vbsm.have_loaded_video)
					local = gstreamer_query_position();
			}
#endif
#ifdef HAVE_VLC
			if (config.vbsm.video_backend == VBSM_VIDEO_BACKEND_VLC) {
                                if (config.vbsm.have_loaded_video)
                                        local = vlc_query_position();
			}
#endif
                        if ((!config.vbsm.have_loaded_video) || (local == -1)) {
                        	long curr_time_msec = get_time_msec();
                                local = curr_time_msec - config.common.init_timestamp_msec;
                        }

			// If using GStreamer or VLC, show the sub while inside
			if ((local > from) && (local < to) && (config.vbsm.have_loaded_video)) {
				char line3[1024];
				strcpy(&line3[0], line);
#ifdef HAVE_GSTREAMER
				if (config.vbsm.video_backend == VBSM_VIDEO_BACKEND_GSTREAMER) 
					gstreamer_sub_set(line3);
#endif
#ifdef HAVE_VLC
				if (config.vbsm.video_backend == VBSM_VIDEO_BACKEND_VLC) 
                                        vlc_sub_set(line3);
#endif
			}

			// If out of the sub, move the list to next
			if ((to > from) && (local > to)) {
				if (gtk_tree_model_iter_next(model, &iter)) {
					// Move to next line
					gtk_tree_selection_select_iter(selection, &iter);

	                                // Move the cursor to the first cell of the new line. We need this because pressing space to pause
        	                        // will emit 'row-activated', which will receive the path of the last cursor (and not of the last iter).
                	                GtkTreePath *path = gtk_tree_model_get_path (model, &iter);
                        	        gtk_tree_view_set_cursor (GTK_TREE_VIEW(config.vbsm.subtitles_view), path, NULL, FALSE);

                                	// Scroll down
					gtk_tree_view_scroll_to_cell (GTK_TREE_VIEW(config.vbsm.subtitles_view), path, NULL, TRUE, 0.5, 0);

				}
				if (config.vbsm.have_loaded_video) {
#ifdef HAVE_GSTREAMER
					if (config.vbsm.video_backend == VBSM_VIDEO_BACKEND_GSTREAMER)
						gstreamer_sub_clear();
#endif
#ifdef HAVE_VLC
					if (config.vbsm.video_backend == VBSM_VIDEO_BACKEND_VLC) 
						vlc_sub_clear();
#endif
				}
			}
		}
	}

	return 1;
}

void show_warning_subtitles() {
	GtkWidget *dialog = gtk_message_dialog_new(GTK_WINDOW(config.vbsm.window),
		GTK_DIALOG_DESTROY_WITH_PARENT | GTK_DIALOG_MODAL,
		GTK_MESSAGE_ERROR,
		GTK_BUTTONS_OK,
		_("Subtitles not loaded."));
	gtk_window_set_title(GTK_WINDOW(dialog), _("Error"));
	gtk_dialog_run(GTK_DIALOG(dialog));
	gtk_widget_destroy(dialog);
}


