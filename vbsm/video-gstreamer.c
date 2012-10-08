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

void gstreamer_seek (int value) {
        gst_element_seek (config.vbsm.gstreamer_playbin2, 1.0, GST_FORMAT_TIME, seek_flags, GST_SEEK_TYPE_CUR, value * GST_SECOND, GST_SEEK_TYPE_NONE, GST_CLOCK_TIME_NONE);
}


void gstreamer_seek_absolute (int value) {
        gst_element_seek (config.vbsm.gstreamer_playbin2, 1.0, GST_FORMAT_TIME, seek_flags, GST_SEEK_TYPE_SET, value * GST_MSECOND, GST_SEEK_TYPE_NONE, GST_CLOCK_TIME_NONE);
}


int gstreamer_query_position() {
        GstFormat format = GST_FORMAT_TIME;
        gint64 cur;

        gst_element_query_position (playbin2, &format, &cur);
        if (format != GST_FORMAT_TIME)
                return GST_CLOCK_TIME_NONE;

        return int(cur/1000000);
}

int gstreamer_query_duration() {
        GstFormat format = GST_FORMAT_TIME;
        gint64 cur;

        gst_element_query_duration (playbin2, &format, &cur);
        if (format != GST_FORMAT_TIME)
                return GST_CLOCK_TIME_NONE;

        return int(cur/1000000);
}


void gstreamer_pause() {
        gst_element_set_state (config.vbsm.gstreamer_playbin2, GST_STATE_PAUSED);
}


void gstreamer_play() {
        gst_element_set_state (config.vbsm.gstreamer_playbin2, GST_STATE_PLAYING);
}


void gstreamer_load_video(char file_name[1024]) {
	char uri[2048];

	sprintf(&uri[0], "file://%s", &file_name[1024]);

	g_object_set (G_OBJECT (config.vbsm.gstreamer_playbin2), "uri", uri, NULL);

	gstreamer_pause();
}


void gstreamer_sub_clear() {
        g_object_set(G_OBJECT(config.vbsm.gstreamer_textoverlay),
                        "text", " ",
                        NULL);

}

void gstreamer_sub_set(char[1024] sub) {
        g_object_set(G_OBJECT(config.vbsm.gstreamer_textoverlay),
                        "text", &sub[0],
                        NULL);

}

void gstreamer_init() {
        config.vbsm.gstreamer_playbin2 = gst_element_factory_make ("playbin2", "playbin2");

        GstElement *videosink = gst_element_factory_make ("ximagesink", "videosink");

        config.vbsm.gstreamer_textoverlay = gst_element_factory_make("textoverlay", "textoverlay");

        GstElement *mybin = gst_bin_new("mybin");
        gst_bin_add (GST_BIN (mybin), config.vbsm.gstreamer_textoverlay);
        GstPad *pad = gst_element_get_pad(config.vbsm.gstreamer_textoverlay, "video_sink");
        gst_element_add_pad(mybin, gst_ghost_pad_new("sink", pad));
        gst_bin_add (GST_BIN (mybin), videosink);

        g_object_set(G_OBJECT(config.vbsm.gstreamer_textoverlay),
                        "halign", "center",
                        "valign", "bottom",
                        "font-desc", "Sans Bold 16",
                        "text", " ",
                        NULL);

        GstBus *bus;
        bus = gst_pipeline_get_bus (GST_PIPELINE (config.vbsm.gstreamer_playbin2));
        gst_bus_add_watch (bus, bus_cb, NULL);
        gst_object_unref (bus);
        
        if (GST_IS_X_OVERLAY (videosink)) {
                gst_x_overlay_set_xwindow_id (GST_X_OVERLAY (videosink), GPOINTER_TO_INT (config.vbsm.gstreamer_widget_player));
        }

        gst_element_link_pads(config.vbsm.gstreamer_textoverlay, "src", videosink, "sink");

        g_object_set (G_OBJECT (config.vbsm.gstreamer_playbin2), "video-sink", mybin, NULL);

        gst_element_set_state (config.vbsm.gstreamer_playbin2, GST_STATE_PAUSED);
}
