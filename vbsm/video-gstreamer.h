// This file is a part of Voody Blue Subtitler suit.
// 
// Author: Assen Totin <assen.totin@gmail.com>
//
// Home page: http://bilbo.online.bg/~assen/vbs
//
// This software is released under GNU General Public License.
// See the LICENSE file for details or visit http://www.gnu.org/copyleft/gpl.html 
// for details.

#include <gst/gst.h>
#include <gst/interfaces/xoverlay.h>

static GstSeekFlags seek_flags = GST_SEEK_FLAG_FLUSH | GST_SEEK_FLAG_KEY_UNIT;

void gstreamer_load_video(char videoFile[1024]);
int gstreamer_get_time_pos(int flag);
void gstreamer_pause();
void gstreamer_play();
void gstreamer_seek (gint);
void gstreamer_seek_absolute (guint64);
guint64 gstreamer_query_position();
guint64 gstreamer_query_duration();


