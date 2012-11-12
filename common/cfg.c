// This file is a part of Voody Blue Subtitler suit.
// 
// Author: Assen Totin <assen.totin@gmail.com>
//
// Home page: http://www.zavedil.com/software-desktop-vbs
//
// This software is released under GNU General Public License.
// See the LICENSE file for details or visit http://www.gnu.org/copyleft/gpl.html 
// for details.

#include "common.h"

void get_cmdl_config(int argc, char *argv[]) {
	GError *error = NULL;
	GOptionContext *cmdl_context;

	cmdl_context = g_option_context_new ("");
	g_option_context_add_main_entries (cmdl_context, cmdl_entries, GETTEXT_PACKAGE);
	g_option_context_add_group (cmdl_context, gtk_get_option_group (TRUE));

	if (!g_option_context_parse (cmdl_context, &argc, &argv, &error))
		error_handler("option parsing", error->message, 1);
}


void default_config() {
	char _tmp[MAX_PATH];
	get_config_dir(&_tmp[0]);

	config.common.init_timestamp_msec = get_time_msec();
	config.common.magic_key = VBS_DEFAULT_MAGIC_KEY;
	config.common.export_cr = VBS_DEFAULT_EXPORT_CR;
	strcpy(&config.common.export_encoding[0], "UTF-8");
	strcpy(&config.common.import_encoding[0], "UTF-8");
	sprintf(&config.common.export_filename[0], "%s%s", &_tmp[0], VBS_DEFAULT_EXPORT_FILENAME);
	config.common.line_size = VBS_DEFAULT_LINE_SIZE;
	config.common.network_mode = 0;
	strcpy(&config.common.server_name[0], VBS_DEFAULT_SERVER_NAME);
	config.common.tcp_port = VBS_DEFAULT_SERVER_PORT;

	sprintf(&config.vbss.import_filename[0], "%s%s", &_tmp[0], VBS_DEFAULT_IMPORT_FILENAME);
	config.vbss.full_screen = VBSS_DEFAULT_FULL_SCREEN;
	config.vbss.colour_bg_r = VBSS_DEFAULT_COLOUR_BG_R;
	config.vbss.colour_bg_g = VBSS_DEFAULT_COLOUR_BG_G;
	config.vbss.colour_bg_b = VBSS_DEFAULT_COLOUR_BG_B;
	config.vbss.colour_fg_r = VBSS_DEFAULT_COLOUR_FG_R;
	config.vbss.colour_fg_g = VBSS_DEFAULT_COLOUR_FG_G;
	config.vbss.colour_fg_b = VBSS_DEFAULT_COLOUR_FG_B;
	config.vbss.justify = VBSS_DEFAULT_FONT_JUSTIFY;
	config.vbss.font_size = VBSS_DEFAULT_FONT_SIZE;
	strcpy(&config.vbss.font_name[0], VBSS_DEFAULT_FONT_NAME);
	strcpy(&config.vbss.font_face[0], VBSS_DEFAULT_FONT_FACE);

#ifdef HAVE_MPLAYER
	config.vbsm.video_backend = VBSM_VIDEO_BACKEND_MPLAYER;
#endif
#ifdef HAVE_GSTREAMER
	config.vbsm.video_backend = VBSM_VIDEO_BACKEND_GSTREAMER; 
	strcpy(&config.vbsm.gstreamer_video_sink[0] , VBSM_DEFAULT_VIDEO_SINK);
#endif
	config.vbsm.progress_update_msec = VBSM_DEFAULT_PROGRESS_UPDATE;
}

void config_char(char *line, char *param) {
	char *line_rest;
	strtok(line, "=");
	line_rest = strtok(NULL, "$$$");
	sprintf(param, line_rest);
}

int config_int(char *line) {
	char *line_rest;
	strtok(line, "=");
	line_rest = strtok(NULL, "$$$");
	return atoi(line_rest);
}

void write_config() {
	FILE *fp_config = fopen(&config.common.config_file_name[0], "w");
	if (!fp_config)
		error_handler("write_config", "could not open config file", 1);
	fprintf(fp_config, "%s", VBS_CONFIG_HEADER);

	fprintf(fp_config, "MAGIC_KEY=%u\n", config.common.magic_key);
	fprintf(fp_config, "EXPORT_CR=%u\n", config.common.export_cr);
	fprintf(fp_config, "EXPORT_ENCODING=%s\n", &config.common.export_encoding[0]);
	fprintf(fp_config, "IMPORT_ENCODING=%s\n", &config.common.import_encoding[0]);
	fprintf(fp_config, "EXPORT_FILENAME=%s\n", &config.common.export_filename[0]);
	fprintf(fp_config, "LINE_SIZE=%u\n", config.common.line_size);
	fprintf(fp_config, "NETWORK_MODE=%u\n", config.common.network_mode);
	fprintf(fp_config, "SERVER_NAME=%s\n", &config.common.server_name[0]);
	fprintf(fp_config, "SERVER_PORT=%u\n", config.common.tcp_port);

	fprintf(fp_config, "IMPORT_FILENAME=%s\n", &config.vbss.import_filename[0]);
	fprintf(fp_config, "FULL_SCREEN=%u\n", config.vbss.full_screen);
	fprintf(fp_config, "COLOUR_BG_R=%u\n", config.vbss.colour_bg_r);
	fprintf(fp_config, "COLOUR_BG_G=%u\n", config.vbss.colour_bg_g);
	fprintf(fp_config, "COLOUR_BG_B=%u\n", config.vbss.colour_bg_b);
	fprintf(fp_config, "COLOUR_FG_R=%u\n", config.vbss.colour_fg_r);
	fprintf(fp_config, "COLOUR_FG_G=%u\n", config.vbss.colour_fg_g);
	fprintf(fp_config, "COLOUR_FG_B=%u\n", config.vbss.colour_fg_b);
	fprintf(fp_config, "FONT_SIZE=%u\n", config.vbss.font_size);
	fprintf(fp_config, "FONT_NAME=%s\n", config.vbss.font_name);
	fprintf(fp_config, "FONT_FACE=%s\n", config.vbss.font_face);
	fprintf(fp_config, "FONT_JUSTIFY=%u\n", config.vbss.justify);

	fprintf(fp_config, "PROGRESS_UPDATE=%u\n", config.vbsm.progress_update_msec);
	fprintf(fp_config, "VIDEO_BACKEND=%u\n", config.vbsm.video_backend);
	fprintf(fp_config, "VIDEO_SINK=%s\n", config.vbsm.gstreamer_video_sink);

	fclose(fp_config);
}

void read_config() {
	FILE *fp_config = fopen(&config.common.config_file_name[0], "r");
	char *line;
	line = malloc(1024);
	if (!line) 
		error_handler("main","malloc failed", 1);

	while (fgets(line, 1024, fp_config)) {
		if (!(line[0]=='#')) {
			line[strlen(line) - 1] = 0;     // kill '\n'

			if (strstr(line, "MAGIC_KEY"))
				config.common.magic_key = config_int(line);
			else if (strstr(line, "EXPORT_CR")) 
				config.common.export_cr = config_int(line);
			else if (strstr(line, "EXPORT_ENCODING")) 
				config_char(line, &config.common.export_encoding[0]);
			else if (strstr(line, "IMPORT_ENCODING")) 
				config_char(line, &config.common.import_encoding[0]);
			else if (strstr(line, "EXPORT_FILENAME"))
				config_char(line, &config.common.export_filename[0]);
			else if (strstr(line, "LINE_SIZE"))
				config.common.line_size = config_int(line);
			else if (strstr(line, "NETWORK_MODE"))
				config.common.network_mode = config_int(line);
			else if (strstr(line, "SERVER_NAME")) 
				config_char(line, &config.common.server_name[0]);
			else if (strstr(line, "SERVER_PORT")) 
				config.common.tcp_port = config_int(line);

			else if (strstr(line, "FULL_SCREEN"))
				config.vbss.full_screen = config_int(line);
			else if (strstr(line, "IMPORT_FILENAME"))
				config_char(line, &config.vbss.import_filename[0]);
			else if (strstr(line, "COLOUR_BG_R"))
				config.vbss.colour_bg_r = config_int(line);
			else if (strstr(line, "COLOUR_BG_G"))
				config.vbss.colour_bg_g = config_int(line);
			else if (strstr(line, "COLOUR_BG_B"))
				config.vbss.colour_bg_b = config_int(line);
			else if (strstr(line, "COLOUR_FG_R"))
				config.vbss.colour_fg_r = config_int(line);
			else if (strstr(line, "COLOUR_FG_G"))
				config.vbss.colour_fg_g = config_int(line);
			else if (strstr(line, "COLOUR_FG_B"))
				config.vbss.colour_fg_b = config_int(line);
			else if (strstr(line, "FONT_SIZE"))
				config.vbss.font_size = config_int(line);
			else if (strstr(line, "FONT_NAME"))
				config_char(line, &config.vbss.font_name[0]);
			else if (strstr(line, "FONT_FACE"))
				config_char(line, &config.vbss.font_face[0]);
			else if (strstr(line, "FONT_JUSTIFY"))
				config.vbss.justify = config_int(line);

			else if (strstr(line, "PROGRESS_UPDATE"))
				config.vbsm.progress_update_msec = config_int(line);
			else if (strstr(line, "VIDEO_BACKEND"))
				config.vbsm.video_backend = config_int(line);
			else if (strstr(line, "VIDEO_SINK"))
				config_char(line, &config.vbsm.gstreamer_video_sink[0]);
		}
	}
	fclose(fp_config);
}

void check_config(int mode) {
	struct passwd *passwd_entry;
	struct stat stat_buf;
	int mkdir_res, stat_res, errsv;
	FILE *fp_config;
	char dir[MAX_PATH];

	// Create a default in-memory condiguration
	default_config();

	if ((cmdl_config.short_f) && (strlen((const char *) cmdl_config.short_f) > 2)) 
		strcpy(&config.common.config_file_name[0], cmdl_config.short_f);
	
	else {
		if (mode == 0) {
			char _tmp[MAX_PATH];
			get_config_dir(&_tmp[0]);
			sprintf(&config.common.config_file_name[0], "%s%s", &_tmp[0], VBS_CONFIG_FILENAME);
		}
		else if (mode == 1)
			sprintf(&config.common.config_file_name[0], "%s%s%s%s%s", SYSCONFDIR, SLASH, VBS_GLOBAL_CONFIG_DIR, SLASH, VBS_CONFIG_FILENAME);
	}

	get_dir_from_filename (&config.common.config_file_name[0], &dir[0]);

	// First, check config directory
	stat_res = stat(&dir[0], &stat_buf);
	errsv = errno;
	if (stat_res == 0) {
		// Is it a dir?
		if (!S_ISDIR(stat_buf.st_mode))
			error_handler("check_config","config dir exists, but is not a directory", 1);
 	}
	else if (stat_res == -1) {
		// Create if missing
		if (errsv == ENOENT) {
#ifdef HAVE_POSIX
			mkdir_res = mkdir(&dir[0], 0755);
#elif HAVE_WINDOWS
			mkdir_res = mkdir(&dir[0]);
#endif
			if (mkdir_res == 1)
				error_handler("main","config dir creation failed", 1);
		}
		else
			error_handler("check_config","stat of config dir failed", 1);
	}

	// Next, check config file
	stat_res = stat(&config.common.config_file_name[0], &stat_buf);
	errsv = errno;

	if (stat_res == 0) {
		// Is it a file?
		if (!S_ISREG(stat_buf.st_mode)) 
			error_handler("check_config","config file exists but is not a regular file", 1);

		read_config();
	}
	else if (stat_res == -1) {
		// Create if missing
		if (errsv == ENOENT) 
			write_config();
		else 
			error_handler("check_config","stat of config file failed", 1);
	}
}

