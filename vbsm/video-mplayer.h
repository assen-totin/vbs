// This file is a part of Voody Blue Subtitler suit.
// 
// Author: Assen Totin <assen.totin@gmail.com>
//
// Home page: http://www.zavedil.com/software-desktop-vbs
//
// This software is released under GNU General Public License.
// See the LICENSE file for details or visit http://www.gnu.org/copyleft/gpl.html 
// for details.

bool mplayer_is_alive();

void mplayer_load_video(char videoFile[1024], int *);

void mplayer_pipe_write(char *command);

int mplayer_get_time_pos(int flag);

int mplayer_get_time_length();

void mplayer_goto(int new_time, bool pause);

#define VBSM_MPLAYER_BINARY "mplayer"

