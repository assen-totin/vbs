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

	if ((config.common.running == FALSE) && (have_loaded_text(window))) {
		model = gtk_tree_view_get_model(view);
		if (gtk_tree_model_get_iter(model, &iter, path)) {
			gint from;
			gtk_tree_model_get(model, &iter, COL_FROM, &from, -1);

			// Move the player -  seeking actually needs some time to complete,
			// that's why we seek 5 seconds earlier and leave 1 second timeout, then pause
			// If seeking to first row, we might get a negative time (will crash mplayer) - be careful
			// new_time is in seconds!
			int new_time = (int) from/1000 - 5;
			if (new_time <= 0)
				new_time = 1;

#ifdef HAVE_MPLAYER
			if (config.vbsm.video_backend == VBSM_VIDEO_BACKEND_MPLAYER)
				mplayer_goto(new_time, false);
#endif
#ifdef HAVE_GSTREAMER
			if (config.vbsm.video_backend == VBSM_VIDEO_BACKEND_GSTREAMER)
				gstreamer_goto(new_time);
#endif
#ifdef HAVE_GSTREAMER
                        if (config.vbsm.video_backend == VBSM_VIDEO_BACKEND_VLC)
                                vlc_goto(new_time);
#endif
		}
	}
}


void on_clicked_button (GtkButton *button, gpointer user_data) {
	long offset = (int) user_data;
	long curr_time;
	int new_time;

	if ((config.common.running == FALSE) && (config.vbsm.have_loaded_video)) {
#ifdef HAVE_MPLAYER
		if (config.vbsm.video_backend == VBSM_VIDEO_BACKEND_MPLAYER) {
			if (mplayer_is_alive()) {
				curr_time = mplayer_get_time_pos(2);
				new_time = calc_new_time(curr_time, offset);
				mplayer_goto(new_time, true);
			}
		}
#endif
#ifdef HAVE_GSTREAMER
		if (config.vbsm.video_backend == VBSM_VIDEO_BACKEND_GSTREAMER) {
			curr_time = gstreamer_query_position();
			if (curr_time > 0) {
				new_time = calc_new_time(curr_time, offset);
				gstreamer_goto(new_time);
			}
		}
#endif
	}
#ifdef HAVE_GSTREAMER
                if (config.vbsm.video_backend == VBSM_VIDEO_BACKEND_VLC) {
                        curr_time = vlc_query_position();
                        if (curr_time > 0) {
                                new_time = calc_new_time(curr_time, offset);
                                vlc_goto(new_time);
                        }
                }
#endif
        }

	// Restore focus to subtitles widget
	gtk_widget_grab_focus(config.vbsm.subtitles_view);
}


int calc_new_time(long curr_time, int offset) {
	int new_time;

	curr_time = (int) (curr_time/1000);
	if (offset == 0) 
		new_time = 1;
	else {
		new_time = curr_time + offset;
		if (new_time < 1)
			new_time = 1;
		else if (new_time > config.vbsm.film_duration)
			new_time = config.vbsm.film_duration - 5;
	}

	return new_time;
}
