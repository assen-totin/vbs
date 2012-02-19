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

void on_key_pressed (GtkTreeView *view, GdkEventKey *event, gpointer userdata) {
        GtkWidget *window = userdata;
        switch ( event->keyval ) {
                case GDK_space:
                        on_space_pressed(window);
                        break;
                case GDK_j:
                        on_j_pressed(window);
                        break;
                case GDK_k:on_k_pressed(window);

        }
}

void on_space_pressed (GtkWidget *window) {
        if (!config.common.inside_sub) {
                if (!config.vbss.paused) {
                        config.vbss.paused = true;
                        config.common.timestamp = time(NULL);
                }
                else {
                        config.vbss.paused = false;
                        time_t curr_timestamp = time(NULL);
                        config.common.init_timestamp += (curr_timestamp - config.common.timestamp);
                        strcpy(&current_sub[0], "\n");
                }
        }
}

void on_j_pressed (GtkWidget *window) {
        if ((!config.common.inside_sub) && (!config.vbss.paused)) {
        	config.common.init_timestamp += 60;
        }
}

void on_k_pressed (GtkWidget *window) {
        if ((!config.common.inside_sub) && (!config.vbss.paused)) {
                config.common.init_timestamp -= 60;
        }
}

