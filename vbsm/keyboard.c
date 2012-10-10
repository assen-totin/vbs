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

void on_pressed_b () {
	GtkTreeSelection *selection;
	GtkTreeModel *model;
	GtkTreeIter iter;
	gint new_from;

	selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(config.vbsm.mplayer_view));
	if (gtk_tree_selection_get_selected(selection, &model, &iter)) {
		if ((config.common.running == TRUE) && (config.common.inside_sub == FALSE)) {
			if (config.vbsm.video_backend == VBSM_VIDEO_BACKEND_MPLAYER) {
				if (mplayer_is_alive())
					new_from = mplayer_get_time_pos(2);
				else {
					time_t curr_time = time(NULL);
					new_from = 1000*(curr_time - config.common.init_timestamp);
				}
			}
			else if (config.vbsm.video_backend == VBSM_VIDEO_BACKEND_GSTREAMER) {
#ifdef HAVE_GSTREAMER
				new_from = gstreamer_query_position();
				if (new_from == -1) {
					time_t curr_time = time(NULL);
					new_from = 1000*(curr_time - config.common.init_timestamp);
				}
#endif
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
			if (config.common.network_mode == 1)
				put_subtitle(line);

			// If using GStreamer, show the sub
			if (config.vbsm.video_backend == VBSM_VIDEO_BACKEND_GSTREAMER) {
#ifdef HAVE_GSTREAMER
				char line3[1024];
				strcpy(&line3[0], line);
				gstreamer_sub_set(line3);
#endif
			}

			g_free(line);

			fprintf(config.vbsm.log_file_fp, "Processed B key.\n");
		}
	}
}


void on_pressed_m () {
	GtkTreeSelection *selection;
	GtkTreeModel     *model;
	GtkTreeIter       iter;
	gint new_to;

	selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(config.vbsm.mplayer_view));
	if (gtk_tree_selection_get_selected(selection, &model, &iter)) {
		if ((config.common.running == TRUE) && (config.common.inside_sub == TRUE)) {
			if (config.vbsm.video_backend == VBSM_VIDEO_BACKEND_MPLAYER) {
				if (mplayer_is_alive())
					new_to = mplayer_get_time_pos(2);
				else {
					time_t curr_time = time(NULL);
					new_to = 1000*(curr_time - config.common.init_timestamp);
				}
			}
                        else if (config.vbsm.video_backend == VBSM_VIDEO_BACKEND_GSTREAMER) {
#ifdef HAVE_GSTREAMER
                                new_to = gstreamer_query_position();
                                if (new_to == -1) {
                                        time_t curr_time = time(NULL);
                                        new_to = 1000*(curr_time - config.common.init_timestamp);
                                }
				// Clear the sub
				gstreamer_sub_clear();
#endif
                        }

			gtk_list_store_set(GTK_LIST_STORE(model), &iter, COL_TO, new_to, -1);

			// Move to next line
			gtk_tree_selection_unselect_iter(selection, &iter);
			if (gtk_tree_model_iter_next(model, &iter)) {
				gtk_tree_selection_select_iter(selection, &iter);

				// Scroll down
				GtkTreePath *path = gtk_tree_model_get_path (model, &iter);
				gtk_tree_view_scroll_to_cell(GTK_TREE_VIEW(config.vbsm.mplayer_view), path, NULL, TRUE, 0.5, 0);
			}

			config.common.inside_sub = FALSE;

			char line[16];
			sprintf(line," ");
			gtk_progress_bar_set_text(GTK_PROGRESS_BAR(config.vbsm.progress), line);
			gtk_progress_bar_set_fraction (GTK_PROGRESS_BAR(config.vbsm.progress), 0);

			// If networking is enabled, drop the line from server
			if (config.common.network_mode == 1)
				put_subtitle("\n");

			// Export subtitles
			export_subtitles();
		}
		else { 
			// Do nothing; if this is not present, the GTK widget will pop-up a small window and move the focus to it. WTF?
			int empty = 0;
		}
	}
}


void on_pressed_space (GtkWidget *window) {
	GtkTreeSelection *selection;
	GtkTreeModel     *model;
	GtkTreeIter       iter;

	if ((config.common.running == TRUE) && (config.common.inside_sub == FALSE)) {
		config.common.running = FALSE;
		gtk_statusbar_push(GTK_STATUSBAR(config.vbsm.status), config.vbsm.status_context_id, "Status: PAUSED");

		// Pause the player
		if ((config.vbsm.video_backend == VBSM_VIDEO_BACKEND_MPLAYER) && (mplayer_is_alive()))
			mplayer_pipe_write("pause");
		else if (config.vbsm.video_backend == VBSM_VIDEO_BACKEND_GSTREAMER) {
#ifdef HAVE_GSTREAMER
			gstreamer_pause();
#endif
		}

	}
	else if (config.common.running == FALSE) {
		if (haveLoadedText(window)) {
			config.common.running = TRUE;
			gtk_statusbar_push(GTK_STATUSBAR(config.vbsm.status), config.vbsm.status_context_id, "Status: RUNNING");

			// Start the player
			if ((config.vbsm.video_backend == VBSM_VIDEO_BACKEND_MPLAYER) && (mplayer_is_alive()))
				mplayer_pipe_write("pause");
			else if (config.vbsm.video_backend == VBSM_VIDEO_BACKEND_GSTREAMER) {
#ifdef HAVE_GSTREAMER
				gstreamer_play();
#endif
			}
		}
	}
}


void on_pressed_key (GtkTreeView *view, GdkEventKey *event, gpointer userdata) {
	GtkWidget *window = userdata;
	switch ( event->keyval ) {
		case GDK_b:
			on_pressed_b();
			break;
		case GDK_m:
			on_pressed_m();
			break;
		case GDK_n:
			on_pressed_m();
			on_pressed_b();
			break;
		case GDK_space:
			on_pressed_space(window);
			break;
		case GDK_s:
			export_subtitles();
			break;
	}
}


