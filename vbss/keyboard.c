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

#ifdef HAVE_GTK3
	#define GDK_y GDK_KEY_y
	#define GDK_t GDK_KEY_t
	#define GDK_r GDK_KEY_r
	#define GDK_q GDK_KEY_q
	#define GDK_w GDK_KEY_w
	#define GDK_e GDK_KEY_e
	#define GDK_space GDK_KEY_space
#endif

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
			config.common.timestamp_msec = get_time_msec();
			strcpy(&config.vbss.current_sub[0], _("Playback paused.\nPress SPACE to continue..."));
		}
		else {
			config.vbss.paused = false;
			long curr_time_msec = get_time_msec();
			config.common.init_timestamp_msec += (curr_time_msec - config.common.timestamp_msec);
			strcpy(&config.vbss.current_sub[0], "\n");
		}
	}
}

void on_q_pressed (GtkWidget *window) {
	if ((!config.common.inside_sub) && (!config.vbss.paused)) {
		config.common.init_timestamp_msec -= 60000;
	}
}

void on_w_pressed (GtkWidget *window) {
	if ((!config.common.inside_sub) && (!config.vbss.paused)) {
		config.common.init_timestamp_msec -= 5000;
	}
}

void on_e_pressed (GtkWidget *window) {
	if ((!config.common.inside_sub) && (!config.vbss.paused)) {
		config.common.init_timestamp_msec -= 1000;
	}
}

void on_r_pressed (GtkWidget *window) {
	if ((!config.common.inside_sub) && (!config.vbss.paused)) {
		config.common.init_timestamp_msec += 1000;
	}
}

void on_t_pressed (GtkWidget *window) {
	if ((!config.common.inside_sub) && (!config.vbss.paused)) {
		config.common.init_timestamp_msec += 5000;
	}
}

void on_y_pressed (GtkWidget *window) {
	if ((!config.common.inside_sub) && (!config.vbss.paused)) {
		config.common.init_timestamp_msec += 60000;
	}
}

