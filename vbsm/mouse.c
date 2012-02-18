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

void view_onRowActivated (GtkTreeView *view, GtkTreePath *path, GtkTreeViewColumn *col, gpointer userdata) {
	GtkTreeModel *model;
	GtkTreeIter   iter;
	GtkWidget *window = userdata;
	int subNum;

	if ((config.common.running == FALSE) && (haveLoadedText(window))) {
		model = gtk_tree_view_get_model(view);
		if (gtk_tree_model_get_iter(model, &iter, path)) {
			gint from;
			gtk_tree_model_get(model, &iter, COL_FROM, &from, -1);
			if ((from > 0) && (mplayerAlive())){
				// Move the player -  seeking actually needs some time to complete, 
				// that's why we seek a second earlier and leave 1 second timeout, then pause
				char command[255];
				sprintf(command, "pausing_keep seek %u 2", from/1000-2);
				writeMPlayer(command);

				// Tell mplayer to load subtitles as they exist now
				writeMPlayer("pausing_keep sub_remove");
				char fileNameEscaped[1024];
				escapeFileName(&config.common.export_filename[0],&fileNameEscaped[0]);
				sprintf(command, "pausing_keep sub_load %s", &fileNameEscaped[0]);
				writeMPlayer(command);

// This *should* normally work; 
// However, my mplayer crashes with "signal 11 in sub_find" when executing "sub_select"
// after getSubNum(). 
// Since we clear all subs before reloading, the new level should always be 1. 
//    subNum = getSubNum();
				subNum = 1;
				sprintf(command, "pausing_keep sub_select %u", subNum);
				writeMPlayer(command);
			}
		}
	}
}

