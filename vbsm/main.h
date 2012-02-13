// This file is a part of Voody Blue Subtitler.
//
// Author: Assen Totin <assen@online.bg>
//
// Home page: http://bilbo.online.bg/~assen/vbs
//
// This software is released under GNU General Public License.
// See the LICENSE file for details or visit http://www.gnu.org/copyleft/gpl.html
// for details.

#include <glib.h>
#include <gtk/gtk.h>
#include <gdk/gdk.h>
#include <gdk/gdkkeysyms.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/timeb.h>
#include <pwd.h>

#include "export.h"
#include "keyboard.h"
#include "mouse.h"
#include "gui.h"
#include "edit.h"
#include "menu.h"
#include "video.h"


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


struct counters {
	time_t timestamp;
	bool running;
	bool inside_sub;
	bool have_loaded_text;
	guint status_context_id;
	pid_t mplayer_pid;
	short unsigned progress_seconds;
	GtkWidget *combo_export;
	GtkWidget *combo_import;
	GtkWidget *combo_cr;
	GtkWidget *status;
	GtkWidget *progress;
	FILE *pipeWrite;
	FILE *pipeRead;
	FILE *tmpFile;
	char tmpFileName[255];
	char configFileName[255];
	char globalExportFile[1024];
	bool config_export_cr;
	char config_export_encoding[255];
	char config_import_encoding[255];
};


void error_handler(char function[128], char error[128]);

#define VBS_ICON "/usr/local/share/vbs/vbs.png"

#define VBS_TMP_DIR "/tmp"

#define VBS_CONFIG_DIR ".vbs"
#define VBS_CONFIG_FILENAME "config"

#define VBS_CONFIG_HEADER "# VBS Config File\n#\n# Do not edit by hand!\n#\n\n"
#define VBS_CONFIG_DEFAULT_CR 0

// Global variables really sux; is there a way to pass a pointer to file selector clicked callback function?
GtkListStore *store;
GtkWidget *view;
struct counters counter;

