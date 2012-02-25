// This file is a part of Voody Blue Subtitler suit.
// 
// Author: Assen Totin <assen.totin@gmail.com>
//
// Home page: http://bilbo.online.bg/~assen/vbs
//
// This software is released under GNU General Public License.
// See the LICENSE file for details or visit http://www.gnu.org/copyleft/gpl.html 
// for details.

int convert_time_from_srt(char *in_time);

void load_srt();

int proc_subtitle_net();

int proc_subtitle_local();

int show_subtitle(GtkWidget *subtitle);

#define VBSS_EXPECTING_CONNECTION "Expecting network connection..."
#define VBSS_NETWORK_OFF "Press SPACE to start playback..."

struct vbss_sub {
	time_t time_from;
	time_t time_to;
	char sub[1024];
};

// Globals
struct vbss_sub subs[1000];
char current_sub[1024];
