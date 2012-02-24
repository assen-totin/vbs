// This file is a part of Voody Blue Subtitler suit.
// 
// Author: Assen Totin <assen.totin@gmail.com>
//
// Home page: http://bilbo.online.bg/~assen/vbs
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
	config.common.init_timestamp = time(NULL);
	config.common.magic_key = VBS_DEFAULT_MAGIC_KEY;
	strcpy(&config.common.config_file_name[0], VBS_CONFIG_FILENAME);
	config.common.export_cr = VBS_DEFAULT_CR;
	strcpy(&config.common.export_encoding[0], "UTF-8");
	strcpy(&config.common.import_encoding[0], "UTF-8");
        strcpy(&config.common.import_filename[0], VBS_IMPORT_FILENAME);
        strcpy(&config.common.export_filename[0], VBS_EXPORT_FILENAME);
	config.common.line_size = VBS_DEFAULT_LINE_SIZE;
	config.common.send_to_network = 0;
	config.common.recv_from_network = 0;
	strcpy(&config.common.server_name[0], VBS_DEFAULT_SERVER);
	config.common.tcp_port = VBS_DEFAULT_TCP_PORT;

	config.vbss.full_screen = VBS_FULL_SCREEN;
	config.vbss.colour_bg_r = VBS_DEFAULT_COLOUR_BG_RED;
	config.vbss.colour_bg_g = VBS_DEFAULT_COLOUR_BG_GREEN;
	config.vbss.colour_bg_b = VBS_DEFAULT_COLOUR_BG_BLUE;
        config.vbss.colour_fg_r = VBS_DEFAULT_COLOUR_FG_RED;
        config.vbss.colour_fg_g = VBS_DEFAULT_COLOUR_FG_GREEN;
        config.vbss.colour_fg_b = VBS_DEFAULT_COLOUR_FG_BLUE;
	config.vbss.font_size = VBS_DEFAULT_FONT_SIZE;
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
        FILE *fp_config = fopen(config.common.config_file_name, "w");
        if (!fp_config)
                error_handler("write_config", "could not open config file", 1);
        fprintf(fp_config, "%s", VBS_CONFIG_HEADER);

	fprintf(fp_config, "FULL_SCREEN=%u\n", config.vbss.full_screen);
	fprintf(fp_config, "MAGIC_KEY=%u\n", config.common.magic_key);
	fprintf(fp_config, "EXPORT_CR=%u\n", config.common.export_cr);
        fprintf(fp_config, "EXPORT_ENCODING=%s\n", &config.common.export_encoding[0]);
        fprintf(fp_config, "IMPORT_ENCODING=%s\n", &config.common.import_encoding[0]);
        fprintf(fp_config, "IMPORT_FILENAME=%s\n", &config.common.import_filename[0]);
        fprintf(fp_config, "EXPORT_FILENAME=%s\n", &config.common.export_filename[0]);
	fprintf(fp_config, "LINE_SIZE=%u\n", config.common.line_size);
	fprintf(fp_config, "SEND_TO_NETWORK=%u\n", config.common.send_to_network);
	fprintf(fp_config, "RECV_FROM_NETWORK=%u\n", config.common.recv_from_network);
	fprintf(fp_config, "SERVER_NAME=%s\n", &config.common.server_name[0]);
        fprintf(fp_config, "TCP_PORT=%u\n", config.common.tcp_port);

        fprintf(fp_config, "COLOUR_BG_R=%u\n", config.vbss.colour_bg_r);
        fprintf(fp_config, "COLOUR_BG_G=%u\n", config.vbss.colour_bg_g);
        fprintf(fp_config, "COLOUR_BG_B=%u\n", config.vbss.colour_bg_b);
        fprintf(fp_config, "COLOUR_FG_R=%u\n", config.vbss.colour_fg_r);
        fprintf(fp_config, "COLOUR_FG_G=%u\n", config.vbss.colour_fg_g);
        fprintf(fp_config, "COLOUR_FG_B=%u\n", config.vbss.colour_fg_b);
	fprintf(fp_config, "FONT_SIZE=%u\n", config.vbss.font_size);

        fclose(fp_config);
}

void read_config() {
	FILE *fp_config = fopen(config.common.config_file_name, "r");
	char *line;
	line = malloc(1024);
	if (!line) 
		error_handler("main","malloc failed", 1);

	while (fgets(line, 1024, fp_config)) {
		if (!(line[0]=='#')) {
			line[strlen(line) - 1] = 0;     /* kill '\n' */

			if (strstr(line, "FULL_SCREEN"))
				config.vbss.full_screen = config_int(line);
                        else if (strstr(line, "MAGIC_KEY"))
                                config.common.magic_key = config_int(line);
			else if (strstr(line, "EXPORT_CR")) 
				config.common.export_cr = config_int(line);
			else if (strstr(line, "EXPORT_ENCODING")) 
				config_char(line, &config.common.export_encoding[0]);
			else if (strstr(line, "IMPORT_ENCODING")) 
				config_char(line, &config.common.import_encoding[0]);
                        else if (strstr(line, "EXPORT_FILENAME"))
                                config_char(line, &config.common.export_filename[0]);
                        else if (strstr(line, "IMPORT_FILENAME"))
                                config_char(line, &config.common.import_filename[0]);
                        else if (strstr(line, "LINE_SIZE"))
                                config.common.line_size = config_int(line);
                        else if (strstr(line, "SEND_TO_NETWORK"))
                                config.common.send_to_network = config_int(line);
                        else if (strstr(line, "RECV_FROM_NETWORK"))
                                config.common.recv_from_network = config_int(line);
			else if (strstr(line, "SERVER_NAME")) 
				config_char(line, &config.common.server_name[0]);
			else if (strstr(line, "TCP_PORT")) 
				config.common.tcp_port = config_int(line);

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
		}
	}
	fclose(fp_config);
}

void check_config() {
	struct passwd *passwd_entry;
	struct stat stat_buf;
	int mkdir_res, stat_res, errsv;
	FILE *fp_config;
	char dir[1024], file[1024], tmp[1024];

	// Create a default in-memory condiguration
	default_config();

	if ((cmdl_config.short_f) && (strlen((const char *) cmdl_config.short_f) > 2)) {
		strcpy(&config.common.config_file_name[0], cmdl_config.short_f);
	}
	else {
		passwd_entry = getpwuid(getuid());

		#ifdef VBS_LOCAL_CONFIG_DIR
			sprintf(config.common.config_file_name, "%s/%s/%s", passwd_entry->pw_dir, VBS_LOCAL_CONFIG_DIR, VBS_CONFIG_FILENAME);
		#else
			sprintf(config.common.config_file_name, "/etc/%s/%s", VBS_GLOBAL_CONFIG_DIR, VBS_CONFIG_FILENAME);
		#endif
	}

	strcpy(&tmp[0], &config.common.config_file_name[0]);
	split_path(&tmp[0], &dir[0], &file[0]);

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
			mkdir_res = mkdir(&dir[0], 0755);
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
