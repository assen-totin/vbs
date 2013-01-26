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

void gstreamer_seek_absolute (int value) {
	gst_element_seek (config.vbsm.gstreamer_playbin2, 1.0, GST_FORMAT_TIME, seek_flags, GST_SEEK_TYPE_SET, value * GST_MSECOND, GST_SEEK_TYPE_NONE, GST_CLOCK_TIME_NONE);
}


int gstreamer_query_position() {
	GstFormat format = GST_FORMAT_TIME;
	gint64 cur;

#ifdef HAVE_GST_0_10
	gst_element_query_position (config.vbsm.gstreamer_playbin2, &format, &cur);
#elif HAVE_GST_1_0
	gst_element_query_position (config.vbsm.gstreamer_playbin2, GST_FORMAT_TIME, &cur);
#endif
	if (format != GST_FORMAT_TIME)
		return -1;

	return (int)(cur/1000000);
}

int gstreamer_query_duration() {
	GstFormat format = GST_FORMAT_TIME;
	gint64 cur;

#ifdef HAVE_GST_0_10
	gst_element_query_duration (config.vbsm.gstreamer_playbin2, &format, &cur);
#elif HAVE_GST_1_0
	gst_element_query_duration (config.vbsm.gstreamer_playbin2, (GstFormat) &format, &cur);
#endif
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

#ifdef HAVE_GST_0_10
	config.vbsm.gstreamer_playbin2 = gst_element_factory_make ("playbin2", "playbin2");
#elif HAVE_GST_1_0
	config.vbsm.gstreamer_playbin2 = gst_element_factory_make ("playbin", "playbin");
#endif

	g_object_set (G_OBJECT (config.vbsm.gstreamer_playbin2), "uri", &uri[0], NULL);

	GstElement *videosink = gst_element_factory_make (&config.vbsm.gstreamer_video_sink[0], "videosink");
	// Preserve original size of video
	g_object_set(G_OBJECT(videosink), "force-aspect-ratio", TRUE);
	config.vbsm.gstreamer_textoverlay = gst_element_factory_make("textoverlay", "textoverlay");

	GstElement *timeoverlay = gst_element_factory_make("timeoverlay", "timeoverlay");

	gst_bin_add_many (GST_BIN (pipeline), config.vbsm.gstreamer_textoverlay, timeoverlay, videosink, NULL);

#ifdef HAVE_GST_0_10
	GstPad *pad = gst_element_get_pad(config.vbsm.gstreamer_textoverlay, "video_sink");
#elif HAVE_GST_1_0
	GstPad *pad = gst_element_get_static_pad(config.vbsm.gstreamer_textoverlay, "video_sink");
#endif
	gst_element_add_pad(pipeline, gst_ghost_pad_new("sink", pad));

	gst_element_link_many(config.vbsm.gstreamer_textoverlay, timeoverlay, videosink);

#ifdef HAVE_GST_0_10
	g_object_set(G_OBJECT(config.vbsm.gstreamer_textoverlay), "halign", "center", "valign", "bottom", "font-desc", "Sans Bold 14", "text", " ", NULL);
#elif HAVE_GST_1_0
	g_object_set(G_OBJECT(config.vbsm.gstreamer_textoverlay), "font-desc", "Sans Bold 14", "text", " ", NULL);
#endif

	g_object_set(G_OBJECT(timeoverlay), "font-desc", "Sans 12", NULL);

	// Merge with existing widget
#ifdef HAVE_GST_0_10
	if (GST_IS_X_OVERLAY (videosink)) {
	#ifdef HAVE_POSIX
		#ifdef HAVE_GTK2
		gst_x_overlay_set_xwindow_id (GST_X_OVERLAY (videosink), GDK_WINDOW_XWINDOW (config.vbsm.widget_player->window));
		#elif HAVE_GTK3
		gst_x_overlay_set_xwindow_id (GST_X_OVERLAY (videosink), GDK_WINDOW_XID (gtk_widget_get_window(config.vbsm.widget_player)));
		#endif
	#elif HAVE_WINDOWS
		#ifdef HAVE_GTK2
		gst_x_overlay_set_xwindow_id (GST_X_OVERLAY (videosink), (guintptr) GDK_WINDOW_HWND (config.vbsm.widget_player->window));
		#elif HAVE_GTK3
		gst_x_overlay_set_xwindow_id (GST_X_OVERLAY (videosink), (guintptr) GDK_WINDOW_HWND (gtk_widget_get_window(config.vbsm.widget_player)));
		#endif
	#endif
#elif HAVE_GST_1_0
	if (GST_IS_VIDEO_OVERLAY (videosink)) {
	#ifdef HAVE_POSIX
		#ifdef HAVE_GTK2
		//gst_video_overlay_set_window_handle(GST_VIDEO_OVERLAY(videosink), GDK_WINDOW_XWINDOW(config.vbsm.widget_player));
		gst_video_overlay_set_window_handle(GST_VIDEO_OVERLAY(videosink), GDK_WINDOW_XID(gtk_widget_get_window(config.vbsm.widget_player)) );
		#elif HAVE_GTK3
		gst_video_overlay_set_window_handle(GST_VIDEO_OVERLAY(videosink), GDK_WINDOW_XID(gtk_widget_get_window(config.vbsm.widget_player)) );
		#endif
	#endif
#endif
	}

	g_object_set (G_OBJECT (config.vbsm.gstreamer_playbin2), "video-sink", pipeline, NULL);

	gst_element_set_state (config.vbsm.gstreamer_playbin2, GST_STATE_PAUSED);

	sleep(1);

	// Query duration - result in seconds
	GstQuery *query;
	gboolean res;
	char err_msg[1024];
	query = gst_query_new_duration (GST_FORMAT_TIME);
	res = gst_element_query (pipeline, query);
	if (res) {
		gint64 duration;
		gst_query_parse_duration (query, NULL, &duration);
		config.vbsm.film_duration = (int) (duration/1000000000);
		sprintf(&err_msg[0], "Film duration in seconds: %u for filename %s", config.vbsm.film_duration, &file_name[0]);
	}
	else {
		sprintf(&err_msg[0], "Failed to get duration for filename %s", &file_name[0]);
	}
	error_handler("gstreamer_init()", &err_msg[0], 0);
	gst_query_unref (query);
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

