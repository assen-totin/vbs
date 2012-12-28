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
#include "video-gstreamer.h"
#ifdef HAVE_POSIX
	#include <gdk/gdkx.h>
#elif HAVE_WINDOWS
	#include <gdk/gdkwin32.h>
#endif

static GstSeekFlags seek_flags = GST_SEEK_FLAG_FLUSH | GST_SEEK_FLAG_KEY_UNIT;

void gstreamer_seek (int value) {
	gst_element_seek (config.vbsm.gstreamer_playbin2, 1.0, GST_FORMAT_TIME, seek_flags, GST_SEEK_TYPE_CUR, value * GST_SECOND, GST_SEEK_TYPE_NONE, GST_CLOCK_TIME_NONE);
}


void gstreamer_seek_absolute (int value) {
	gst_element_seek (config.vbsm.gstreamer_playbin2, 1.0, GST_FORMAT_TIME, seek_flags, GST_SEEK_TYPE_SET, value * GST_MSECOND, GST_SEEK_TYPE_NONE, GST_CLOCK_TIME_NONE);
}


int gstreamer_query_position() {
	GstFormat format = GST_FORMAT_TIME;
	gint64 cur;

	gst_element_query_position (config.vbsm.gstreamer_playbin2, &format, &cur);
	if (format != GST_FORMAT_TIME)
		return -1;

	return (int)(cur/1000000);
}

int gstreamer_query_duration() {
	GstFormat format = GST_FORMAT_TIME;
	gint64 cur;

	gst_element_query_duration (config.vbsm.gstreamer_playbin2, &format, &cur);
	if (format != GST_FORMAT_TIME)
		return -1;

	return (int)(cur/1000000);
}


void gstreamer_pause() {
	gst_element_set_state (config.vbsm.gstreamer_playbin2, GST_STATE_PAUSED);
}


void gstreamer_play() {
	gst_element_set_state (config.vbsm.gstreamer_playbin2, GST_STATE_PLAYING);
}


void gstreamer_sub_clear() {
	g_object_set(G_OBJECT(config.vbsm.gstreamer_textoverlay),
			"text", " ",
			NULL);

}

void gstreamer_sub_set(char sub[1024]) {
	// Replace | with \n
	fix_new_line(&sub[0]);
	
	g_object_set(G_OBJECT(config.vbsm.gstreamer_textoverlay),
			"text", &sub[0],
			NULL);
}

void gstreamer_init(char file_name[1024]) {
	gst_init(0, NULL);

	char uri[2048];
#ifdef HAVE_POSIX
	sprintf(&uri[0], "file://%s", &file_name[0]);
#elif HAVE_WINDOWS
	char _tmp[2048];
	win_filename_to_uri(&file_name[0], &_tmp[0]);
	sprintf(&uri[0], "file:///%s", &_tmp[0]);
#endif
	GstElement *pipeline = gst_pipeline_new ("my-pipeline");

	config.vbsm.gstreamer_playbin2 = gst_element_factory_make ("playbin2", "playbin2");
	g_object_set (G_OBJECT (config.vbsm.gstreamer_playbin2), "uri", &uri[0], NULL);

	GstElement *videosink = gst_element_factory_make (&config.vbsm.gstreamer_video_sink[0], "videosink");
	// Preserve original size of video
	g_object_set(G_OBJECT(videosink), "force-aspect-ratio", "true");
	config.vbsm.gstreamer_textoverlay = gst_element_factory_make("textoverlay", "textoverlay");
	GstElement *timeoverlay = gst_element_factory_make("timeoverlay", "timeoverlay");

	gst_bin_add_many (GST_BIN (pipeline), config.vbsm.gstreamer_textoverlay, timeoverlay, videosink, NULL);

	GstPad *pad = gst_element_get_pad(config.vbsm.gstreamer_textoverlay, "video_sink");
	gst_element_add_pad(pipeline, gst_ghost_pad_new("sink", pad));

	gst_element_link_many(config.vbsm.gstreamer_textoverlay, timeoverlay, videosink);

	g_object_set(G_OBJECT(config.vbsm.gstreamer_textoverlay),
			"halign", "center",
			"valign", "bottom",
			"font-desc", "Sans Bold 14",
			"text", " ",
			NULL);

	g_object_set(G_OBJECT(timeoverlay),
			"font-desc", "Sans 12",
			NULL);

	// Merge with existing widget
	if (GST_IS_X_OVERLAY (videosink)) {
#ifdef HAVE_GTK2
	#ifdef HAVE_POSIX
		gst_x_overlay_set_xwindow_id (GST_X_OVERLAY (videosink), GDK_WINDOW_XWINDOW (config.vbsm.gstreamer_widget_player->window));
	#elif HAVE_WINDOWS
		gst_x_overlay_set_xwindow_id (GST_X_OVERLAY (videosink), (guintptr) GDK_WINDOW_HWND (config.vbsm.gstreamer_widget_player->window));
	#endif
#elif HAVE_GTK3
		gst_x_overlay_set_xwindow_id (GST_X_OVERLAY (videosink), GDK_WINDOW_XID ((gtk_widget_get_window(config.vbsm.gstreamer_widget_player))));
#endif
	}

	g_object_set (G_OBJECT (config.vbsm.gstreamer_playbin2), "video-sink", pipeline, NULL);

	gst_element_set_state (config.vbsm.gstreamer_playbin2, GST_STATE_PAUSED);
}

static gboolean bus_cb (GstBus *bus, GstMessage *msg, gpointer data) {
	switch (GST_MESSAGE_TYPE (msg)) {
		case GST_MESSAGE_EOS: {
			g_debug ("end-of-stream");
			break;
		}
		case GST_MESSAGE_ERROR: {
			gchar *debug;
			GError *err;

			gst_message_parse_error (msg, &err, &debug);
			g_free (debug);

			g_warning ("Error: %s", err->message);
			g_error_free (err);
			break;
		}
		default:
			break;
	}

	return TRUE;
}

void gstreamer_goto (long new_time) {
	// Put new_time in milliseconds
	new_time = new_time * 1000;
	// Clear current sub - if any
	gstreamer_sub_clear();

	// Seek to new position, stop
	gstreamer_seek_absolute(new_time);
	gstreamer_pause();
}

