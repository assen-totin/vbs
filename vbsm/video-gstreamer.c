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

void gstreamer_seek (gint value) {
        gst_element_seek (config.vbsm.gstreamer_playbin2, 1.0, GST_FORMAT_TIME, seek_flags, GST_SEEK_TYPE_CUR, value * GST_SECOND, GST_SEEK_TYPE_NONE, GST_CLOCK_TIME_NONE);
}


void gstreamer_seek_absolute (guint64 value) {
        gst_element_seek (playbin2, 1.0, GST_FORMAT_TIME, seek_flags, GST_SEEK_TYPE_SET, value, GST_SEEK_TYPE_NONE, GST_CLOCK_TIME_NONE);
}


guint64 gstreamer_query_position () {
        GstFormat format = GST_FORMAT_TIME;
        gint64 cur;

        gst_element_query_position (playbin2, &format, &cur);
        if (format != GST_FORMAT_TIME)
                return GST_CLOCK_TIME_NONE;

        return cur;
}

guint64 gstreamer_query_duration () {
        GstFormat format = GST_FORMAT_TIME;
        gint64 cur;

        gst_element_query_duration (playbin2, &format, &cur);
        if (format != GST_FORMAT_TIME)
                return GST_CLOCK_TIME_NONE;

        return cur;
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

