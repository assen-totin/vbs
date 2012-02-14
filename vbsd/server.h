// This file is a part of Voody Blue Subtitler suit.
// 
// Author: Assen Totin <assen.totin@gmail.com>
//
// Home page: http://bilbo.online.bg/~assen/vbs
//
// This software is released under GNU General Public License.
// See the LICENSE file for details or visit http://www.gnu.org/copyleft/gpl.html 
// for details.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <syslog.h>
#include <signal.h>
#include <sys/ipc.h>
#include <time.h>
#include <math.h>

#define VBS_GLOBAL_CONFIG_DIR "vbs"
#define QUEUE_SIZE 64

//#define LINESIZE 1024
//#define TCP_PORT 88
//#define MAGIC_KEY 1973

