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

void on_clicked_row (GtkTreeView *view, GtkTreePath *path, GtkTreeViewColumn *col, gpointer userdata) {
	GtkTreeModel *model;
	GtkTreeIter   iter;
	GtkWidget *window = userdata;
	int subNum;

	if ((config.common.running == FALSE) && (haveLoadedText(window))) {
		model = gtk_tree_view_get_model(view);
		if (gtk_tree_model_get_iter(model, &iter, path)) {
			gint from;
			gtk_tree_model_get(model, &iter, COL_FROM, &from, -1);
			if (from > 0) {
				// Move the player -  seeking actually needs some time to complete,
				// that's why we seek 5 seconds earlier and leave 1 second timeout, then pause
				// If seeking to first row, we might net a negative time (will crash mplayer) - be carefull
				// new_time is in seconds!
				int new_time = (int) from/1000 - 5;
				if (new_time <= 0)
					new_time = 1;

				if ((config.vbsm.video_backend == VBSM_VIDEO_BACKEND_MPLAYER) && (mplayer_is_alive())) {
					char command[255];
					sprintf(command, "pausing_keep seek %u 2", new_time);
					mplayer_pipe_write(command);

					// Tell mplayer to load subtitles as they exist now
					mplayer_pipe_write("pausing_keep sub_remove");
					sprintf(command, "pausing_keep sub_load %s", &config.vbsm.sub_file_name[0]);
					mplayer_pipe_write(command);

// This *should* normally work; 
// However, my mplayer crashes with "signal 11 in sub_find" when executing "sub_select"
// after getSubNum(). 
// Since we clear all subs before reloading, the new level should always be 1. 
//    subNum = getSubNum();
					subNum = 1;
					sprintf(command, "pausing_keep sub_select %u", subNum);
					mplayer_pipe_write(command);
				}

				else if (config.vbsm.video_backend == VBSM_VIDEO_BACKEND_GSTREAMER) {
#ifdef HAVE_GSTREAMER
					// Put new_time in milliseconds
					new_time = new_time * 1000;
					// Clear current sub - if any
					gstreamer_sub_clear();					

					// Seek to new position, stop
					gstreamer_seek_absolute(new_time);
					gstreamer_pause();
#endif
				}

			}
		}
	}
}

