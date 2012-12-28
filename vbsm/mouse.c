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

void on_clicked_row (GtkTreeView *view, GtkTreePath *path, GtkTreeViewColumn *col, gpointer userdata) {
	GtkTreeModel *model;
	GtkTreeIter iter;
	GtkWidget *window = userdata;
	int subNum;

	if ((config.common.running == FALSE) && (have_loaded_text(window))) {
		model = gtk_tree_view_get_model(view);
		if (gtk_tree_model_get_iter(model, &iter, path)) {
			gint from;
			gtk_tree_model_get(model, &iter, COL_FROM, &from, -1);
			if (from > 0) {
				// Move the player -  seeking actually needs some time to complete,
				// that's why we seek 5 seconds earlier and leave 1 second timeout, then pause
				// If seeking to first row, we might get a negative time (will crash mplayer) - be careful
				// new_time is in seconds!
				int new_time = (int) from/1000 - 5;
				if (new_time <= 0)
					new_time = 1;

				if (config.vbsm.video_backend == VBSM_VIDEO_BACKEND_MPLAYER) {
#ifdef HAVE_MPLAYER
					mplayer_goto(new_time);
#endif
				}

				else if (config.vbsm.video_backend == VBSM_VIDEO_BACKEND_GSTREAMER) {
#ifdef HAVE_GSTREAMER
					gstreamer_goto(new_time);
#endif
				}

			}
		}
	}
}


void on_clicked_button (GtkButton *button, gpointer user_data) {
	long new_time = (int) user_data;
	long curr_time;

	if ((config.common.running == FALSE) && (config.vbsm.have_loaded_video)) {
		if (config.vbsm.video_backend == VBSM_VIDEO_BACKEND_MPLAYER) {
#ifdef HAVE_MPLAYER
			if (mplayer_is_alive()) {
				curr_time = mplayer_get_time_pos(2);
				if (new_time != 0) {
					new_time += curr_time;
					if (new_time < 1)
						new_time = 1;
					else if (new_time > config.vbsm.film_duration)
						new_time = config.vbsm.film_duration - 5;
				}
				else 
					new_time = 1;
				mplayer_goto(new_time);
			}
#endif
		}
		else if (config.vbsm.video_backend == VBSM_VIDEO_BACKEND_GSTREAMER) {
#ifdef HAVE_GSTREAMER
			curr_time = gstreamer_query_position();
			if (curr_time > 0) {
				curr_time = (int) curr_time/1000;
				if (new_time != 0) {
					new_time += curr_time;
					if (new_time < 1)
						new_time = 1;
					else if (new_time > config.vbsm.film_duration)
						new_time = config.vbsm.film_duration - 5;
				}
				else
					new_time = 1;
				gstreamer_goto(new_time);
			}
#endif
		}
	}

	// Restore focus to subtitles widget
	gtk_widget_grab_focus(config.vbsm.subtitles_view);
}
