#include "common.h"
#include "config.h"

void default_config() {
	strcpy(&config.file_name[0], VBS_CONFIG_FILENAME);
	config.export_cr = 1;
	strcpy(&config.export_encoding[0], "UTF-8");
	strcpy(&config.import_encoding[0], "UTF-8");
	config.tcp_port = VBS_DEFAULT_TCP_PORT;
	config.line_size = VBS_DEFAULT_LINE_SIZE;
	strcpy(&config.server_name[0], VBS_DEFAULT_SERVER);
	config.test_mode = 1;
	config.use_network = 1;
	config.magic_key = VBS_DEFAULT_MAGIC;
	config.colour_bg_r = VBS_DEFAULT_COLOUR_BG_RED;
	config.colour_bg_g = VBS_DEFAULT_COLOUR_BG_GREEN;
	config.colour_bg_b = VBS_DEFAULT_COLOUR_BG_BLUE;
        config.colour_fg_r = VBS_DEFAULT_COLOUR_FG_RED;
        config.colour_fg_g = VBS_DEFAULT_COLOUR_FG_GREEN;
        config.colour_fg_b = VBS_DEFAULT_COLOUR_FG_BLUE;

	#ifdef VBS_TEST_MODE
		config.test_mode = 1;
	#else
		config.test_mode = 0;
	#endif
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

bool config_bool(char *line) {
	char *line_rest;
	int res;
	strtok(line, "=");
	line_rest = strtok(NULL, "$$$");
	res = atoi(line_rest);
	if (res == 0) {return false;}
	return true;
}

void write_config() {
        FILE *fp_config = fopen(config.file_name, "w");
        if (!fp_config)
                error_handler("write_config", "could not open config file", 1);
        fprintf(fp_config, "%s", VBS_CONFIG_HEADER);
	fprintf(fp_config, "EXPORT_CR=%u\n", config.export_cr);
        fprintf(fp_config, "EXPORT_ENCODING=%s\n", &config.export_encoding[0]);
        fprintf(fp_config, "IMPORT_ENCODING=%s\n", &config.import_encoding[0]);
        fprintf(fp_config, "TCP_PORT=%u\n", config.tcp_port);
        fprintf(fp_config, "LINE_SIZE=%u\n", config.line_size);
        fprintf(fp_config, "SERVER_NAME=%s\n", &config.server_name[0]);
        fprintf(fp_config, "MAGIC=%u\n", config.magic_key);
	fprintf(fp_config, "TEST_MODE=%u\n", config.test_mode);
	fprintf(fp_config, "USE_NETWORK=%u\n", config.use_network);
        fprintf(fp_config, "COLOUR_BG_R=%u\n", config.colour_bg_r);
        fprintf(fp_config, "COLOUR_BG_G=%u\n", config.colour_bg_g);
        fprintf(fp_config, "COLOUR_BG_B=%u\n", config.colour_bg_b);
        fprintf(fp_config, "COLOUR_FG_R=%u\n", config.colour_fg_r);
        fprintf(fp_config, "COLOUR_FG_G=%u\n", config.colour_fg_g);
        fprintf(fp_config, "COLOUR_FG_B=%u\n", config.colour_fg_b);
        fclose(fp_config);
}

void read_config() {
	FILE *fp_config = fopen(config.file_name, "r");
	char *line;
	line = malloc(1024);
	if (!line) 
		error_handler("main","malloc failed", 1);

	while (fgets(line, 1024, fp_config)) {
		if (!(line[0]=='#')) {
			line[strlen(line) - 1] = 0;     /* kill '\n' */
			if (strstr(line, "EXPORT_CR")) 
				config.export_cr = config_bool(line);
			else if (strstr(line, "TEST_MODE")) 
				config.test_mode = config_bool(line);
			else if (strstr(line, "EXPORT_ENCODING")) 
				config_char(line, &config.export_encoding[0]);
			else if (strstr(line, "IMPORT_ENCODING")) 
				config_char(line, &config.import_encoding[0]);
			else if (strstr(line, "SERVER_NAME")) 
				config_char(line, &config.server_name[0]);
			else if (strstr(line, "TCP_PORT")) 
				config.tcp_port = config_int(line);
			else if (strstr(line, "LINE_SIZE")) 
				config.line_size = config_int(line);
			else if (strstr(line, "MAGIC")) 
				config.magic_key = config_int(line);
			else if (strstr(line, "USE_NETWORK"))
				config.use_network = config_int(line);
                        else if (strstr(line, "COLOUR_BG_R"))
                                config.colour_bg_r = config_int(line);
                        else if (strstr(line, "COLOUR_BG_G"))
                                config.colour_bg_g = config_int(line);
                        else if (strstr(line, "COLOUR_BG_B"))
                                config.colour_bg_b = config_int(line);
                        else if (strstr(line, "COLOUR_FG_R"))
                                config.colour_fg_r = config_int(line);
                        else if (strstr(line, "COLOUR_FG_G"))
                                config.colour_fg_g = config_int(line);
                        else if (strstr(line, "COLOUR_FG_B"))
                                config.colour_fg_b = config_int(line);
		}
	}
	fclose(fp_config);
}

void check_config() {
	struct passwd *passwd_entry;
	struct stat stat_buf;
	int mkdir_res, stat_res, errsv;
	FILE *fp_config;

	// Create a default in-memory condiguration
	default_config();

	passwd_entry = getpwuid(getuid());

	// First, check config directory
	#ifdef VBS_LOCAL_CONFIG_DIR
		sprintf(config.file_name, "%s/%s", passwd_entry->pw_dir, VBS_LOCAL_CONFIG_DIR);
	#else
		sprintf(config.file_name, "/etc/%s", VBS_GLOBAL_CONFIG_DIR);
	#endif

	stat_res = stat(config.file_name, &stat_buf);
	errsv = errno;
	if (stat_res == 0) {
		// Is it a dir?
		if (!S_ISDIR(stat_buf.st_mode))
			error_handler("main","config dir exists, but is not a directory", 1);
 	}
	else if (stat_res == -1) {
		// Create if missing
		if (errsv == ENOENT) {
			mkdir_res = mkdir(config.file_name, 0755);
			if (mkdir_res == 1)
				error_handler("main","config dir creation failed", 1);
		}
		else
			error_handler("main","stat of config dir failed", 1);
	}

	// Next, check config file
	#ifdef VBS_LOCAL_CONFIG_DIR
		sprintf(config.file_name, "%s/%s/%s", passwd_entry->pw_dir, VBS_LOCAL_CONFIG_DIR, VBS_CONFIG_FILENAME);
	#else
		sprintf(config.file_name, "/etc/%s/%s", VBS_GLOBAL_CONFIG_DIR, VBS_CONFIG_FILENAME);
	#endif

	stat_res = stat(config.file_name, &stat_buf);
	errsv = errno;

	if (stat_res == 0) {
		// Is it a file?
		if (!S_ISREG(stat_buf.st_mode)) 
			error_handler("main","config file exists but is not a regular file", 1);

		read_config();
	}
	else if (stat_res == -1) {
		// Create if missing
		if (errsv == ENOENT) 
			write_config();
		else 
			error_handler("main","stat of config file failed", 1);
	}
}

