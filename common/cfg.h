// This file is a part of Voody Blue Subtitler suit.
// 
// Author: Assen Totin <assen.totin@gmail.com>
//
// Home page: http://bilbo.online.bg/~assen/vbs
//
// This software is released under GNU General Public License.
// See the LICENSE file for details or visit http://www.gnu.org/copyleft/gpl.html 
// for details.

void get_cmdl_config(int argc, char *argv[]);
void default_config();
void config_char(char *line, char *param);
int config_int(char *line);
void write_config();
void read_config();
void check_config(int mode);

static GOptionEntry cmdl_entries[] =
{
	{ "config", 'f', 0, G_OPTION_ARG_STRING, &cmdl_config.short_f, "Alternative config file", "" },
	{ NULL }
};

#define GETTEXT_PACKAGE "gtk20"

#define VBS_GLOBAL_CONFIG_DIR "vbs"
#define VBS_LOCAL_CONFIG_DIR ".vbs"
#define VBS_CONFIG_FILENAME "config"
#define VBS_CONFIG_HEADER "# VBS Config File\n#\n# Do not edit by hand!\n#\n\n"
#define VBS_EXPORT_FILENAME "/tmp/vbs-out.txt"
#define VBS_IMPORT_FILENAME "/tmp/vbs-in.txt"
#define VBS_DEFAULT_LINE_SIZE 1024
#define VBS_DEFAULT_TCP_PORT 42
#define VBS_DEFAULT_SERVER "localhost"
#define VBS_DEFAULT_MAGIC_KEY 1973
#define VBS_DEFAULT_COLOUR_BG_RED 17476
#define VBS_DEFAULT_COLOUR_BG_GREEN 17476
#define VBS_DEFAULT_COLOUR_BG_BLUE 17476
#define VBS_DEFAULT_COLOUR_FG_RED 65535
#define VBS_DEFAULT_COLOUR_FG_GREEN 65535
#define VBS_DEFAULT_COLOUR_FG_BLUE 65535
#define VBS_DEFAULT_FONT_SIZE 32
#define VBS_DEFAULT_CR 0 
#define VBS_FULL_SCREEN 0
#define VBSM_VIDEO_BACKEND "# Set video backend to 0 for MPLayer or to 1 for GStreamer\n"
#define VBSM_GS_VIDEO_SINK "ximagesink"
