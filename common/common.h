// This file is a part of Voody Blue Subtitler suit.
// 
// Author: Assen Totin <assen.totin@gmail.com>
//
// Home page: http://www.zavedil.com/software-desktop-vbs
//
// This software is released under GNU General Public License.
// See the LICENSE file for details or visit http://www.gnu.org/copyleft/gpl.html 
// for details.

#include "../config.h"

#include <errno.h>
#include <fcntl.h>
#include <gdk/gdk.h>
#include <gdk/gdkkeysyms.h>
#include <glib.h>
#include <glib/gstdio.h>
#include <gtk/gtk.h>
#include <math.h>
#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <sys/stat.h>
#include <sys/timeb.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>
#include <libintl.h>

#ifdef HAVE_POSIX
	#include <netdb.h>
	#include <netinet/in.h>
	#include <sys/socket.h>
	#include <sys/ipc.h>
	#include <syslog.h>
	#include <sys/wait.h>
	#define SLASH "/"
#endif

#ifdef HAVE_WINDOWS
	#include <winsock2.h>
	#include <ws2tcpip.h>
	#define SLASH "\\"
#endif

#ifdef HAVE_GSTREAMER
	#include <gst/gst.h>
	#include <gst/interfaces/xoverlay.h>
#endif

#define _(String) gettext (String)

#ifndef MAX_PATH
#define MAX_PATH 260
#endif

enum {
	COL_LINE = 0,
	COL_FROM,
	COL_TO,
	NUM_COLS
};

enum {
	VBS_IMPORT_FILTER_TEXT = 0,
	VBS_IMPORT_FILTER_SRT
};

enum {
	TIME_SEC = 0,
	TIME_MSEC
};

enum {
	VBSM_VIDEO_BACKEND_MPLAYER = 0,
	VBSM_VIDEO_BACKEND_GSTREAMER
};

enum {
	VBSS_JUSTIFY_LEFT = 0,
	VBSS_JUSTIFY_RIGHT,
	VBSS_JUSTIFY_CENTER,
	VBSS_JUSTIFY_FILL
};

struct struct_vbsm {
	bool have_loaded_text;
	bool have_loaded_video;
	guint status_context_id;
	short unsigned progress_seconds;
	GtkWidget *window;
	GtkWidget *status;
	GtkWidget *subtitles_view;
	GtkWidget *progress;
	int progress_update_msec;
	char log_file_name[255];
	FILE *log_file_fp;
	char sub_file_name[255];
	int unsigned video_backend;
	pid_t mplayer_pid;
	FILE *mplayer_pipe_write;
	FILE *mplayer_pipe_read;
	GtkListStore *mplayer_store;
#ifdef HAVE_GSTREAMER
	GstElement *gstreamer_playbin2;
	GstElement *gstreamer_textoverlay;
#endif
	GtkWidget *gstreamer_widget_player;
	char gstreamer_video_sink[1024];
};


struct struct_vbss {
	char import_filename[1024];
	char current_sub[1024];
	int total_subtitles;
	bool paused;
	int full_screen;
	int colour_bg_r;
	int colour_bg_g;
	int colour_bg_b;
	int colour_fg_r;
	int colour_fg_g;
	int colour_fg_b;
	char font_name[128];
	char font_face[128];
	int font_size;
	int justify;
};

struct struct_common {
	bool running;
	bool inside_sub;
	long init_timestamp_msec;
	long timestamp_msec;
	char magic_key[255];
	char config_file_name[1024];
	int export_cr;
	char export_encoding[255];
	char import_encoding[255];
	char export_filename[1024];
	int line_size;
	int network_mode;
	char server_name[255];
	int tcp_port;
	struct hostent *host_entry;
};

struct configuration {
	struct struct_common common;
	struct struct_vbsm vbsm;
	struct struct_vbss vbss;
};

struct cmdl {
	gchar *short_f;
};

struct subtitle_srt {
	long time_from;
	long time_to;
	char sub[1024];
};

struct enc_entry {
	char name[255];
	bool dflt;
};

static struct enc_entry enc_entries[] = {
  {"UTF-8", true},
  {"ISO8859-1", false},
  {"WINDOWS-1250", false},
  {"WINDOWS-1251", false},
  {"KOI8R", false},
  {"KOI8U", false}
};

// Global variables really sux; is there a way to pass a pointer to file selector clicked callback function?
struct configuration config;
struct cmdl cmdl_config;
int can_recv_from_net;

#include "cfg.h"
#include "error.h"
#include "network.h"
#include "util.h"
#include "menu.h"
#ifdef HAVE_WINDOWS
	#include "util-win.h"
#endif

#include "../vbsd/vbsd.h"

#include "../vbss/vbss.h"
#include "../vbss/keyboard.h"

#include "../vbsm/vbsm.h"
#include "../vbsm/export.h"
#include "../vbsm/import.h"
#include "../vbsm/keyboard.h"
#include "../vbsm/mouse.h"
#include "../vbsm/gui.h"
#include "../vbsm/edit.h"
#include "../vbsm/video-mplayer.h"
#include "../vbsm/menu.h"
#ifdef HAVE_GSTREAMER
	#include "../vbsm/video-gstreamer.h"
#endif

#include "../vbsc/vbsc.h"
#include "../vbsc/menu.h"

