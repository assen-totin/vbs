// This file is a part of Voody Blue Subtitler suit.
// 
// Author: Assen Totin <assen.totin@gmail.com>
//
// Home page: http://bilbo.online.bg/~assen/vbs
//
// This software is released under GNU General Public License.
// See the LICENSE file for details or visit http://www.gnu.org/copyleft/gpl.html 
// for details.

void default_config();
void config_char(char *line, char *param);
int config_int(char *line);
void write_config();
void read_config();
void check_config();

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

