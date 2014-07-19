/**
 * Header file for configuration-related functions. 
 * @author Assen Totin assen.totin@gmail.com
 * 
 * Created for the Voody Blue Subtitler suit, copyright (C) 2014 Assen Totin, assen.totin@gmail.com 
 * 
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

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
#define VBS_DEFAULT_MAGIC_KEY "1973"
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
#define VBSS_DEFAULT_FULL_SCREEN 1
#define VBSS_DEFAULT_UPDATE_MSEC 100
#define VBSM_DEFAULT_PROGRESS_UPDATE 100
