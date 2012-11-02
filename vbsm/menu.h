// This file is a part of Voody Blue Subtitler suit.
// 
// Author: Assen Totin <assen.totin@gmail.com>
//
// Home page: http://www.zavedil.com/software-desktop-vbs
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

char *get_help_text();

struct video_backend {
	char name[255];
	int num;
	bool show_menu_output;
	bool dflt;
};

static struct video_backend video_backends[] = {
#ifdef HAVE_POSIX
	#ifdef HAVE_GSTREAMER
	#ifdef HAVE_MPLAYER
	{"GStreamer", VBSM_VIDEO_BACKEND_GSTREAMER, true, true},
	{"MPlayer", VBSM_VIDEO_BACKEND_MPLAYER, false, false}
	#endif
	#endif

	#ifdef HAVE_GSTREAMER
	#ifndef HAVE_MPLAYER
	{"GStreamer", VBSM_VIDEO_BACKEND_GSTREAMER, true, true}
	#endif
	#endif

	#ifdef HAVE_MPLAYER
	#ifndef HAVE_GSTREAMER
	{"MPlayer", VBSM_VIDEO_BACKEND_MPLAYER, false, false}
	#endif
	#endif
#elif HAVE_WINDOWS
	{"GStreamer", VBSM_VIDEO_BACKEND_GSTREAMER, true, true}
#endif
};

struct video_output {
	char name[255];
	char code[255];
	bool dflt;
};

static struct video_output video_outputs[] = {
#ifdef HAVE_POSIX
	{"X11", "ximagesink", true},
	{"Xv", "xvimagesink", false},
	{"SDL", "sdlvideosink", false},
	{"Auto", "autovideosink", false}
#elif HAVE_WINDOWS
        {"Direct Draw", "directdrawsink", true},
        {"Direct Show", "dshowvideosink", false},
        {"Direct 3D", "d3dvideosink", false},
        {"Auto", "autovideosink", false}
#endif
};

