// This file is a part of Voody Blue Subtitler suit.
// 
// Author: Assen Totin <assen.totin@gmail.com>
//
// Home page: http://bilbo.online.bg/~assen/vbs
//
// This software is released under GNU General Public License.
// See the LICENSE file for details or visit http://www.gnu.org/copyleft/gpl.html 
// for details.

#include <errno.h>
#include <fcntl.h>
#include <gdk/gdk.h>
#include <gdk/gdkkeysyms.h>
#include <glib.h>
#include <gtk/gtk.h>
#include <math.h>
#include <netdb.h>
#include <netinet/in.h>
#include <openssl/md5.h>
#include <pwd.h>
#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <sys/ipc.h>
#include <syslog.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/timeb.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

#define VBS_TMP_DIR "/tmp"

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

struct struct_vbsm {
        bool have_loaded_text;
        guint status_context_id;
        pid_t mplayer_pid;
        short unsigned progress_seconds;
        GtkWidget *menu_widget;
	GtkWidget *menu_widget2;
        GtkWidget *status;
        GtkWidget *progress;
        FILE *pipeWrite;
        FILE *pipeRead;
        char tmpFileName[255];
	FILE *tmpFile;
};


struct struct_vbss {
	bool paused;
	int local_subs_count;
	int colour_bg_r;
	int colour_bg_g;
	int colour_bg_b;
	int colour_fg_r;
	int colour_fg_g;
	int colour_fg_b;
	int font_size;
};

struct struct_common {
	bool running;
	bool inside_sub;
	time_t init_timestamp;
	time_t timestamp;
        int test_mode;
        int magic_key;
	char config_file_name[1024];
        int export_cr;
        char export_encoding[255];
        char import_encoding[255];
        char import_filename[1024];
        FILE *import_fp;
        char export_filename[1024];
        FILE *export_fp;
        int line_size;
	int use_network;
	char server_name[255];
        int tcp_port;
        struct hostent *host_entry;
};

struct configuration {
	struct struct_common common;
	struct struct_vbsm vbsm;
	struct struct_vbss vbss;
};

struct configuration config;

struct encEntry {
	char name[255];
	bool dflt;
};


static struct encEntry encEntries[] = {
  {"UTF-8", true},
  {"ISO8859-1", false},
  {"WINDOWS-1250", false},
  {"WINDOWS-1251", false},
  {"KOI8R", false},
  {"KOI8U", false}
};

// Global variables really sux; is there a way to pass a pointer to file selector clicked callback function?
GtkListStore *store;
GtkWidget *view;
struct configuration config;

#include "../vbsd/server.h"
#include "../vbss/gui.h"

#include "../vbsm/vbs.h"
#include "../vbsm/export.h"
#include "../vbsm/keyboard.h"
#include "../vbsm/mouse.h"
#include "../vbsm/gui.h"
#include "../vbsm/edit.h"
#include "../vbsm/video.h"

#include "config.h"
#include "error.h"
#include "network.h"
#include "util.h"
#include "menu.h"

