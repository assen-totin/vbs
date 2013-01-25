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
#include "video-vlc.h"
#ifdef HAVE_POSIX
	#include <gdk/gdkx.h>
#elif HAVE_WINDOWS
	#include <gdk/gdkwin32.h>
#endif

void vlc_seek_absolute (int value) {
	libvlc_media_player_set_pause(config.vbsm.vlc_player, 1);
	libvlc_media_player_set_time (config.vbsm.vlc_player, value * 1000);
	// Need this sleep to allow video to change time
	//sleep(1);
}


int vlc_query_position() {
	return libvlc_media_player_get_time(config.vbsm.vlc_player);
}

int vlc_query_duration(libvlc_media_t *m) {
	// Unlike GStreamer, in VLC this method runs on the media object, not on the player
	// Also, the playback must have been started (+ sleep(1) to make sure everythign is loaded)
	return libvlc_media_get_duration(m);
}


void vlc_pause() {
	libvlc_media_player_set_pause(config.vbsm.vlc_player, 1);
}


void vlc_play() {
	libvlc_media_player_set_pause(config.vbsm.vlc_player, 0);
}


void vlc_sub_clear() {
	libvlc_video_set_marquee_string (config.vbsm.vlc_player, libvlc_marquee_Text, " ");
}

void vlc_sub_set(char sub[1024]) {
	// Replace | with \n
	fix_new_line(&sub[0]);
	libvlc_video_set_marquee_string (config.vbsm.vlc_player, libvlc_marquee_Text, &sub[0]);
}

void vlc_init(char file_name[1024]) {
	// Boot args -r equried for subs
	const char * const vlc_args[] = {"--sub-filter=marq", "--no-xlib"};

	// New player instance
	libvlc_instance_t *inst = libvlc_new (2, vlc_args);

	// Define media
	char uri[2048];
#ifdef HAVE_POSIX
        sprintf(&uri[0], "file://%s", &file_name[0]);
#elif HAVE_WINDOWS
        char _tmp[2048];
        win_filename_to_uri(&file_name[0], &_tmp[0]);
        sprintf(&uri[0], "file:///%s", &_tmp[0]);
#endif
	libvlc_media_t *media = libvlc_media_new_path (inst, &uri[0]);

	// Create player
	libvlc_media_player_t *mp = libvlc_media_player_new_from_media (media);

	// Create marquee for subtitles
	libvlc_video_set_marquee_int(mp, libvlc_marquee_Size, 20);
	libvlc_video_set_marquee_int(mp, libvlc_marquee_Opacity, 255);
	libvlc_video_set_marquee_int (mp, libvlc_marquee_Position, 8);
	libvlc_video_set_marquee_int(mp, libvlc_marquee_Color, 0x00ffffff);
	libvlc_video_set_marquee_int(mp, libvlc_marquee_Enable, 1);
	libvlc_video_set_marquee_string (mp, libvlc_marquee_Text, " ");

	// Merge with existing widget
#ifdef HAVE_POSIX
	#ifdef HAVE_GTK2
	libvlc_media_player_set_xwindow (mp, GDK_WINDOW_XWINDOW (config.vbsm.widget_player->window));
	#elif HAVE_GTK3
	libvlc_media_player_set_xwindow (mp, GDK_WINDOW_XID (gtk_widget_get_window(config.vbsm.widget_player)));
	#endif
#elif HAVE_WINDOWS
	#ifdef HAVE_GTK2
	libvlc_media_player_set_hwnd (mp, (guintptr) GDK_WINDOW_HWND (config.vbsm.widget_player->window));
	#elif HAVE_GTK3
	libvlc_media_player_set_hwnd (mp, (guintptr) GDK_WINDOW_HWND (gtk_widget_get_window(config.vbsm.widget_player)));
	#endif
#endif

	// To pause, activate the playback, then sleep for 1 second and rewind to the beginning
	libvlc_media_player_play (mp);
	sleep(1);
	libvlc_media_player_set_pause(mp, 1);
        libvlc_media_player_set_time (mp, 100);

	// Query duration - result in seconds
	char err_msg[1024];
	libvlc_time_t duration = libvlc_media_get_duration(media);
	if (duration > -1) {
		config.vbsm.film_duration = (int) (duration/1000);
		sprintf(&err_msg[0], "Film duration in seconds: %u for filename %s", config.vbsm.film_duration, &file_name[0]);
	}
	else {
		sprintf(&err_msg[0], "Failed to get duration for filename %s", &file_name[0]);
	}
	error_handler("vlc_init()", &err_msg[0], 0);

	// Free
	libvlc_media_release(media);
}


void vlc_goto (long new_time) {
	// Clear current sub - if any
	vlc_sub_clear();

	// Seek to new position, stop
	vlc_seek_absolute(new_time);
	vlc_pause();
}

