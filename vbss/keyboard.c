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
                case GDK_q:
                        on_q_pressed(window);
                        break;
                case GDK_w: 
			on_w_pressed(window);
			break;
                case GDK_e: 
                        on_e_pressed(window);
                        break;
                case GDK_r: 
                        on_r_pressed(window);
                        break;
                case GDK_t: 
                        on_t_pressed(window);
                        break;
                case GDK_y: 
                        on_y_pressed(window);
                        break;
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

void on_q_pressed (GtkWidget *window) {
        if ((!config.common.inside_sub) && (!config.vbss.paused)) {
        	config.common.init_timestamp -= 60;
        }
}

void on_w_pressed (GtkWidget *window) {
        if ((!config.common.inside_sub) && (!config.vbss.paused)) {
                config.common.init_timestamp -= 5;
        }
}

void on_e_pressed (GtkWidget *window) {
        if ((!config.common.inside_sub) && (!config.vbss.paused)) {
                config.common.init_timestamp -= 1;
        }
}

void on_r_pressed (GtkWidget *window) {
        if ((!config.common.inside_sub) && (!config.vbss.paused)) {
                config.common.init_timestamp += 1;
        }
}

void on_t_pressed (GtkWidget *window) {
        if ((!config.common.inside_sub) && (!config.vbss.paused)) {
                config.common.init_timestamp += 5;
        }
}

void on_y_pressed (GtkWidget *window) {
        if ((!config.common.inside_sub) && (!config.vbss.paused)) {
                config.common.init_timestamp += 60;
        }
}
