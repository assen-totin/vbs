// This file is a part of Voody Blue Subtitler suit.
// 
// Author: Assen Totin <assen.totin@gmail.com>
//
// Home page: http://bilbo.online.bg/~assen/vbs
//
// This software is released under GNU General Public License.
// See the LICENSE file for details or visit http://www.gnu.org/copyleft/gpl.html 
// for details.

void *loadVideo(char videoFile[1024]);

void writeMPlayer(char *command);

int getTimePos(int flag);

#define VBS_MPLAYER_BINARY "mplayer"

