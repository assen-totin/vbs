// This file is a part of Voody Blue Subtitler suit.
// 
// Author: Assen Totin <assen.totin@gmail.com>
//
// Home page: http://bilbo.online.bg/~assen/vbs
//
// This software is released under GNU General Public License.
// See the LICENSE file for details or visit http://www.gnu.org/copyleft/gpl.html 
// for details.

void fileDialogOpen(GtkAction *action, gpointer param);
void fileDialogSave(GtkAction *action, gpointer param);

void quitDialog(GtkWidget *widget, gpointer window);

void zero_timing(GtkAction *action, gpointer param);

void set_timer(GtkWidget *widget, gpointer window);

void help_contents(GtkWidget *widget, gpointer window);

void insert_subtitle(GtkAction *action, gpointer param);

void set_video_output (GtkWidget *widget, gpointer window);

void set_video_backend (GtkWidget *widget, gpointer window);

struct video_backend {
        char name[255];
	int num;
	bool show_menu_output;
        bool dflt;
};

static struct video_backend video_backends[] = {
	{"MPlayer", VBSM_VIDEO_BACKEND_MPLAYER, false, true},
	{"GStreamer", VBSM_VIDEO_BACKEND_GSTREAMER, true, false}
};

struct video_output {
        char name[255];
	char code[255];
        bool dflt;
};

static struct video_output video_outputs[] = {
	{"X11", "ximagesink", true},
	{"Xv", "xvimagesink", false},
	{"SDL", "sdlvideosink", false}
};

