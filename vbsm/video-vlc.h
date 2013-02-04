// This file is a part of Voody Blue Subtitler suit.
// 
// Author: Assen Totin <assen.totin@gmail.com>
//
// Home page: http://www.zavedil.com/software-desktop-vbs
//
// This software is released under GNU General Public License.
// See the LICENSE file for details or visit http://www.gnu.org/copyleft/gpl.html 
// for details.

void vlc_load_video(char videoFile[1024]);
int vlc_get_time_pos(int flag);
void vlc_pause();
void vlc_play();
void vlc_seek_absolute (int);
int vlc_query_position();
int vlc_query_duration(libvlc_media_t *m);
void vlc_sub_clear();
void vlc_set_clear(char sub[1024]);
static gboolean bus_cb (GstBus *bus, GstMessage *msg, gpointer data);
void vlc_init(char file_name[1024], int *);
void vlc_goto (long new_time);
