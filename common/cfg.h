// This file is a part of Voody Blue Subtitler suit.
// 
// Author: Assen Totin <assen.totin@gmail.com>
//
// Home page: http://www.zavedil.com/software-desktop-vbs
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

#ifdef HAVE_POSIX
	#define VBS_LOCAL_CONFIG_DIR ".vbs"
	#define VBSM_DEFAULT_VIDEO_SINK "ximagesink"
	#define VBS_DEFAULT_EXPORT_CR 0
#elif HAVE_WINDOWS
	#define VBS_LOCAL_CONFIG_DIR "_vbs"
	#define VBSM_DEFAULT_VIDEO_SINK "directdrawsink"
	#define VBS_DEFAULT_EXPORT_CR 1

#endif

#define VBS_DEFAULT_IMPORT_FILENAME "vbs-in.txt"
#define VBS_DEFAULT_EXPORT_FILENAME "vbs-out.srt"
#define VBS_GLOBAL_CONFIG_DIR "vbs"
#define VBS_CONFIG_FILENAME "config"
#define VBS_CONFIG_HEADER "# VBS Config File\n#\n# Do not edit by hand!\n#\n\n"
#define VBS_DEFAULT_LINE_SIZE 1024
#define VBS_DEFAULT_SERVER_PORT 42
#define VBS_DEFAULT_SERVER_NAME "localhost"
#define VBS_DEFAULT_MAGIC_KEY 1973
#define VBSS_DEFAULT_COLOUR_BG_R 17476
#define VBSS_DEFAULT_COLOUR_BG_G 17476
#define VBSS_DEFAULT_COLOUR_BG_B 17476
#define VBSS_DEFAULT_COLOUR_FG_R 65535
#define VBSS_DEFAULT_COLOUR_FG_G 65535
#define VBSS_DEFAULT_COLOUR_FG_B 65535
#define VBSS_DEFAULT_FONT_SIZE 32
#define VBSS_DEFAULT_FONT_NAME "Sans"
#define VBSS_DEFAULT_FONT_FACE "Regular"
#define VBSS_DEFAULT_FONT_JUSTIFY 0
#define VBSS_DEFAULT_FULL_SCREEN 0
#define VBSM_DEFAULT_PROGRESS_UPDATE 100
