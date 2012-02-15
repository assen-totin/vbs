// This file is a part of Voody Blue Subtitler suit.
// 
// Author: Assen Totin <assen.totin@gmail.com>
//
// Home page: http://bilbo.online.bg/~assen/vbs
//
// This software is released under GNU General Public License.
// See the LICENSE file for details or visit http://www.gnu.org/copyleft/gpl.html 
// for details.

#include <stdbool.h>
#include <netdb.h>

//#define VBS_GLOBAL_CONFIG_DIR "vbs"
#define VBS_LOCAL_CONFIG_DIR ".vbs"
#define VBS_CONFIG_FILENAME "config"
#define VBS_CONFIG_HEADER "# VBS Config File\n#\n# Do not edit by hand!\n#\n\n"
#define VBS_DEFAULT_LINE_SIZE 1024
#define VBS_DEFAULT_TCP_PORT 42
#define VBS_DEFAULT_SERVER "vbs42.online.bg"
#define VBS_DEFAULT_MAGIC_KEY 1973
#define VBS_DEFAULT_COLOUR_BG_RED 17476
#define VBS_DEFAULT_COLOUR_BG_GREEN 17476
#define VBS_DEFAULT_COLOUR_BG_BLUE 17476
#define VBS_DEFAULT_COLOUR_FG_RED 65535
#define VBS_DEFAULT_COLOUR_FG_GREEN 65535
#define VBS_DEFAULT_COLOUR_FG_BLUE 65535
#define VBS_DEFAULT_FONT_SIZE 32
#define VBS_DEFAULT_CR 0 
#define VBS_TEST_MODE 1

struct vbsm {
	time_t init_timestamp;
        time_t timestamp;
        bool running;
        bool inside_sub;
        bool have_loaded_text;
        guint status_context_id;
        pid_t mplayer_pid;
        short unsigned progress_seconds;
        GtkWidget *menu_widget;
        GtkWidget *status;
        GtkWidget *progress;
        FILE *pipeWrite;
        FILE *pipeRead;
        FILE *tmpFile;
        char tmpFileName[255];
        char globalExportFile[1024];
};


struct vbss {
	int colour_bg_r;
	int colour_bg_g;
	int colour_bg_b;
	int colour_fg_r;
	int colour_fg_g;
	int colour_fg_b;
	int font_size;
};

struct common {
        int test_mode;
        int magic_key;
	char config_file_name[255];
        int export_cr;
        char export_encoding[255];
        char import_encoding[255];
        int line_size;
	int use_network;
	char server_name[255];
        int tcp_port;
        struct hostent host_entry;
};

struct configuration {
	struct common;
	struct vbsm;
	struct vbss;
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

void error_handler(char function[256], char error[256], bool exit_flag);
void check_config();
int get_subtitle(char *buffer);

