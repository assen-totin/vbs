#include <stdbool.h>
#include <netdb.h>

//#define VBS_GLOBAL_CONFIG_DIR "vbs"
#define VBS_LOCAL_CONFIG_DIR ".vbs"
#define VBS_CONFIG_FILENAME "config"
#define VBS_CONFIG_HEADER "# VBS Config File\n#\n# Do not edit by hand!\n#\n\n"
#define VBS_DEFAULT_LINE_SIZE 1024
#define VBS_DEFAULT_TCP_PORT 42
#define VBS_DEFAULT_SERVER "vbs42.online.bg"
#define VBS_DEFAULT_MAGIC 1973
#define VBS_DEFAULT_COLOUR_BG_RED 17476
#define VBS_DEFAULT_COLOUR_BG_GREEN 17476
#define VBS_DEFAULT_COLOUR_BG_BLUE 17476
#define VBS_DEFAULT_COLOUR_FG_RED 65535
#define VBS_DEFAULT_COLOUR_FG_GREEN 65535
#define VBS_DEFAULT_COLOUR_FG_BLUE 65535
#define VBS_DEFAULT_CR 1
#define VBS_TEST_MODE 1

struct configuration {
	int test_mode;
	int magic_key;
	int export_cr;
	char file_name[255];
	char export_encoding[255];
	char import_encoding[255];
	int line_size;
	int tcp_port;
	char server_name[255];
	struct hostent host_entry;
	int use_network;
	int colour_bg_r;
	int colour_bg_g;
	int colour_bg_b;
	int colour_fg_r;
	int colour_fg_g;
	int colour_fg_b;
	int font_size;
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

