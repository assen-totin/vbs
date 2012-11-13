// This file is a part of Voody Blue Subtitler suit.
// 
// Author: Assen Totin <assen.totin@gmail.com>
//
// Home page: http://www.zavedil.com/software-desktop-vbs
//
// This software is released under GNU General Public License.
// See the LICENSE file for details or visit http://www.gnu.org/copyleft/gpl.html 
// for details.

#define QUEUE_SIZE 64

int check_magic_key(unsigned char buffer[config.common.line_size]);

void signal_handler(int sig);

char *shm_at;
int sockfd;
int unix_time;

