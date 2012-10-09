// This file is a part of Voody Blue Subtitler suit.
// 
// Author: Assen Totin <assen.totin@gmail.com>
//
// Home page: http://bilbo.online.bg/~assen/vbs
//
// This software is released under GNU General Public License.
// See the LICENSE file for details or visit http://www.gnu.org/copyleft/gpl.html 
// for details.

bool mplayer_is_alive();

void *mplayer_load_video(char videoFile[1024]);

void mplayer_pipe_write(char *command);

int mplayer_get_time_pos(int flag);

#define VBS_MPLAYER_BINARY "mplayer"

