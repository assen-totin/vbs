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
#include "video-gstreamer.h"
#include <gdk/gdkx.h>

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


/*
void gstreamer_load_video(char file_name[1024]) {
	char uri[2048];

	sprintf(&uri[0], "file://%s", &file_name[0]);

	g_object_set (G_OBJECT (config.vbsm.gstreamer_playbin2), "uri", uri, NULL);

	gstreamer_pause();
}
*/

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
	sprintf(&uri[0], "file://%s", &file_name[0]);

        config.vbsm.gstreamer_playbin2 = gst_element_factory_make ("playbin2", "playbin2");

	g_object_set (G_OBJECT (config.vbsm.gstreamer_playbin2), "uri", &uri[0], NULL);

        GstElement *videosink = gst_element_factory_make (&config.vbsm.gstreamer_video_sink[0], "videosink");

        config.vbsm.gstreamer_textoverlay = gst_element_factory_make("textoverlay", "textoverlay");

        GstElement *mybin = gst_bin_new("mybin");
        gst_bin_add (GST_BIN (mybin), config.vbsm.gstreamer_textoverlay);

        GstPad *pad = gst_element_get_pad(config.vbsm.gstreamer_textoverlay, "video_sink");
        gst_element_add_pad(mybin, gst_ghost_pad_new("sink", pad));

        gst_bin_add (GST_BIN (mybin), videosink);

        g_object_set(G_OBJECT(config.vbsm.gstreamer_textoverlay),
                        "halign", "center",
                        "valign", "bottom",
                        "font-desc", "Sans Bold 14",
                        "text", " ",
                        NULL);
	/*
        GstBus *bus;
        bus = gst_pipeline_get_bus (GST_PIPELINE (config.vbsm.gstreamer_playbin2));
        gst_bus_add_watch (bus, bus_cb, NULL);
        gst_object_unref (bus);
        */

	// Merge with existing widget
        if (GST_IS_X_OVERLAY (videosink)) {
#ifdef HAVE_GTK2
		gst_x_overlay_set_xwindow_id (GST_X_OVERLAY (videosink), GDK_WINDOW_XWINDOW (config.vbsm.gstreamer_widget_player->window));
#elif HAVE_GTK3
		gst_x_overlay_set_xwindow_id (GST_X_OVERLAY (videosink), GDK_WINDOW_XID ((gtk_widget_get_window(config.vbsm.gstreamer_widget_player))));
#endif
        }

        gst_element_link_pads(config.vbsm.gstreamer_textoverlay, "src", videosink, "sink");

        g_object_set (G_OBJECT (config.vbsm.gstreamer_playbin2), "video-sink", mybin, NULL);

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

