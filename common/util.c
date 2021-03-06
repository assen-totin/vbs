/**
 * Common utility functions. 
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

#include "../common/common.h"

/**
 * Remove the EOL character from a line. 
 * @param *buffer char Line with an EOL character.
 */
void fix_new_line(char *buffer) {
	char *part_one, line_print[config.common.line_size];

	char cr_lf[3];
	if (config.common.export_cr == 1) 
		sprintf(cr_lf, "\r\n");
	else
		sprintf(cr_lf, "\n");

	// Replace | with \n
	part_one = strtok(buffer, "|");
	sprintf(&line_print[0], "%s", part_one);

	while(part_one = strtok(NULL,"|")) {
		sprintf(&line_print[0], "%s%s%s", &line_print[0], &cr_lf[0], part_one);
	}

	strcpy(buffer, &line_print[0]);
}

/**
 * Get current time as UNIX timestamp with milliseconds. 
 * @returns long Current time as UNIX timestamp with milliseconds. 
 */
long get_time_msec() {
	struct timeval curr_time;
	gettimeofday(&curr_time, NULL);
	return (long) (1000 * curr_time.tv_sec + (int) (curr_time.tv_usec / 1000));
}

/**
 * Convert time from SRT format into UNIX timestamp with resolution of a second or a millisecond. 
 * @param *in_time char Character buffer with the time in SRT format.
 * @param format int Desired output resolution, seconds or milliseconds.
 * @returns int The input parsed and converted to UNIX timestamp.
 */
int convert_time_from_srt(char *in_time, int format) {
	char *str_h = strtok(in_time, ":");
	int res_h = atoi(str_h);

	char *str_m = strtok(NULL, ":");
	int res_m = atoi(str_m);

	char *str_s = strtok(NULL, ",");
	int res_s = atoi(str_s);

	char *str_ms = strtok(NULL, ",");
	int res_ms = atoi(str_ms);

	if (format == TIME_SEC)
		return (res_h*3600 + res_m*60 + res_s);
	else if (format == TIME_MSEC)
		return (res_h*3600000 + res_m*60000 + res_s*1000 + res_ms);
}

/**
 * Convert time to SRT format from UNIX timestamp with resolution of a second or a millisecond. 
 * @param the_time unsigned int The time as UNIX timestamp.
 * @param *res char Pointer to write the result to.
 * @param format int Desired output resolution, seconds or milliseconds.
 */
void convert_time_to_srt(unsigned int the_time, char *res, int format) {

	div_t qH = div(the_time, 3600000);
	div_t qM = div(qH.rem, 60000);
	div_t qS = div(qM.rem, 1000);

	char unfM[8], unfS[8], unfMS[8];

	if (qM.quot > 9) {sprintf(unfM, "%u", qM.quot);}
	else {sprintf(unfM, "0%u", qM.quot);}

	if (qS.quot > 9) {sprintf(unfS, "%u", qS.quot);}
	else {sprintf(unfS, "0%u", qS.quot);}

	if (qS.rem > 99) {sprintf(unfMS, "%u", qS.rem);}
	else if (qS.rem > 9) {sprintf(unfMS, "0%u", qS.rem);}
	else {sprintf(unfMS, "00%u", qS.rem);}

	if (format == TIME_MSEC)
		sprintf(res, "0%u:%s:%s,%s", qH.quot, unfM, unfS, unfMS);
	else if (format == TIME_SEC)
		sprintf(res, "0%u:%s:%s", qH.quot, unfM, unfS);
}

/**
 * Import subtitles in SRT format from file.
 * @param *filename char The file to read from.
 * @param *counter int Pointer to write the number of subtitles to.
 * @param *import_error_flag int Pointer to write to in case import encounters an error.
 */
struct subtitle_srt *import_subtitles_srt(char *filename, int *counter, int *import_error_flag) {
	char *time_begin, *time_end;
	char *line_utf8;
	long time_begin_val, time_end_val;
	int counter_line = -1;
	int counter_array = 0;
	gsize bytes_written;

	FILE *fp_in = fopen (filename, "r");
	if (!fp_in)
		error_handler("import_subtitles_srt", "failed to open subtitles", 1);

	char *line_in = malloc(config.common.line_size);
	char *line_out = malloc(config.common.line_size);
	char *line_tmp = malloc(config.common.line_size);
	if (!line_in || !line_out || !line_tmp)
		error_handler("import_subtitles_srt", "malloc failed", 1);
	memset(line_out, '\0', config.common.line_size);

	struct subtitle_srt *sub_array = malloc(sizeof(struct subtitle_srt));
	if (!sub_array)
		error_handler("import_subtitles_srt", "malloc failed", 1);

	while (fgets(line_in, config.common.line_size, fp_in)) {
		// Check for wrong file
		if (strlen(line_in) > (config.common.line_size - 4)) {
			*import_error_flag = 1;
			return sub_array;
		}

		// An empty line closes subtitle
		if (strlen(line_in) < 3) {
			if (counter_line > 0) {
				// UTF-8: always call even if input is UTF-8 in order to filter out improper files
				line_utf8 = g_convert(line_out, strlen(line_out), "UTF-8", config.common.import_encoding, NULL, &bytes_written, NULL);

		                // Check valid
                		if (!line_utf8) {
		                        *import_error_flag = 1;
                		        return sub_array;
		                }

				// Add new subtitle to the array
				void *_tmp = realloc(sub_array, ((counter_array + 1) * sizeof(struct subtitle_srt)));
				if (!_tmp)
					error_handler("import_subtitles_srt", "realloc failed", 1);
				sub_array = (struct subtitle_srt *)_tmp;
				sub_array[counter_array].time_from = time_begin_val;
				sub_array[counter_array].time_to = time_end_val;
				strcpy(&sub_array[counter_array].sub[0], line_utf8);
				counter_array++;
				counter_line = -1;

				g_free(line_utf8);
			}
			continue;
		}

		// Kill newlines
		if (strstr(line_in, "\r"))
			strcpy(line_tmp, strtok(line_in, "\r"));
		else
			strcpy(line_tmp, line_in);
		if (strstr(line_tmp, "\n"))
			strcpy(line_in, strtok(line_tmp, "\n"));
		else
			strcpy(line_in, line_tmp);

		// Next Line will be a subtitle line only if current line includes timing
		if (strstr(line_in,"-->")) {
			time_begin = strtok(line_in, "-->");
			time_end = strtok(NULL, "-->");

			strcpy(line_tmp, time_begin);
			time_begin_val = convert_time_from_srt(line_tmp, TIME_MSEC);

			strcpy(line_tmp, time_end);
			time_end_val = convert_time_from_srt(line_tmp, TIME_MSEC);

			counter_line = 0;

			continue;
		}

		if (counter_line == 0) {
			strcpy(line_out, line_in);
			counter_line++;
		}
		else if (counter_line > 0) {
			sprintf(line_out, "%s|%s", line_out, line_in);
			counter_line++;
		}
	}

	free(line_in);
	free(line_out);
	free(line_tmp);

	*counter = counter_array;

	return sub_array;
}

/**
 * Get the directory where locale files reside.
 * @param *res char Pointer to write the prefix to.
 */
void get_locale_prefix(char *res) {
#ifdef HAVE_POSIX
	strcpy(res, LOCALEDIR);
#elif HAVE_WINDOWS
	char win_path[MAX_PATH];
	win_get_path(&win_path[0], sizeof(win_path)); 
	sprintf(res, "%s%s", &win_path[0], LOCALEDIR);
#endif
}

/**
 * Get programme's icon file name.
 * @param *res char Pointer to write the filename to.
 */
void get_icon(char *res) {
#ifdef HAVE_POSIX
	strcpy(res, VBS_ICON);
#elif HAVE_WINDOWS
	char win_path[MAX_PATH];
	win_get_path(&win_path[0], sizeof(win_path));
	sprintf(res, "%s%s%s", &win_path[0], SLASH, VBS_ICON);
#endif
}

/**
 * Get the path for the file selector dialogue.
 * @param *res char Pointer to write the path to.
 */
void get_file_selector_path(char *res) {
#ifdef HAVE_POSIX
	sprintf(res, "%s%s%s", g_get_home_dir(), SLASH, "Desktop");
#elif HAVE_WINDOWS
	sprintf(res, "%s", g_get_home_dir());
#endif

}

/**
 * Get programme's configuration directory path.
 * @param *res char Pointer to write the directory path to.
 */
void get_config_dir(char *res) {
	sprintf(res, "%s%s%s-%s%s", g_get_home_dir(), SLASH, VBS_LOCAL_CONFIG_DIR, PACKAGE_VERSION, SLASH);
}

/**
 * Extract a directory name from the full path.
 * @param *filename char The path to parse.
 * @param *dir char Pointer to write the directory to.
 */
void get_dir_from_filename (char *filename, char *dir) {
	char filename_in[MAX_PATH];
	strcpy(&filename_in[0], filename);

	char c0[MAX_PATH];
	char *p0 = &c0[0];
	memset(&c0[0], '\0', MAX_PATH);

	char c1[MAX_PATH];
	char *p1 = &c1[0];
	memset(&c1[0], '\0', MAX_PATH);

	p0 = strtok(&filename_in[0], SLASH);
#ifdef HAVE_POSIX
	sprintf(dir, "/%s", p0);
#elif HAVE_WINDOWS
	sprintf(dir, "%s", p0);
#endif

	while (1) {
		p0 = strtok(NULL, SLASH);
		if (p0) {
			sprintf(dir, "%s%s%s", dir, p1, SLASH);
			p1 = strtok(NULL, SLASH);
		}
		else
			break;

		if (p0 && p1)
			sprintf(dir, "%s%s%s", dir, p0, SLASH);
		else
			break;
	}
}

/**
 * Delete old log files.
 */

void del_old_logs() {
	GStatBuf stat_buf;
	struct vbs_stat_struct tmp_stat;
	const gchar *filename;
	int i, j, file_counter = 0;

	bzero (&stat_buf, sizeof(stat_buf));

	struct vbs_stat_struct *file_array = malloc(sizeof(struct vbs_stat_struct));
	if (!file_array)
		error_handler("del_old_logs", "malloc failed", 1);

	char dir_name[MAX_PATH];
	get_dir_from_filename (&config.vbsm.log_file_name[0], &dir_name[0]);
	GDir *dir = g_dir_open(&dir_name[0], 0, NULL);
	while (1) {
		if (filename = g_dir_read_name(dir)) {
			if (strstr(filename, VBSM_LOG_FILE)) {
				g_stat(filename, &stat_buf);
				void *_tmp = realloc(file_array, ((file_counter + 1) * sizeof(struct vbs_stat_struct)));
				if (!_tmp)
					error_handler("del_old_logs", "realloc failed", 1);
				file_array = (struct vbs_stat_struct *)_tmp;
				file_array[file_counter].mtime = stat_buf.st_mtime;
				sprintf(&file_array[file_counter].filename[0], "%s", filename);
				file_counter++;
			}
		}
		else
			break;
	}

	// Sort by mtime desc
	for (i=0; i<file_counter; i++ ) {
		for (j=0; j<file_counter; j++) {
			if (file_array[i].mtime > file_array[j].mtime) {
				tmp_stat = file_array[i];
				file_array[i] = file_array[j];
				file_array[j] = tmp_stat;
			}
		}
	}

	// Remove all but the newest N
	char file_to_del[1024];
	for (i=VBSM_KEEP_LOGS; i< file_counter; i++){
		sprintf(&file_to_del[0], "%s%s%s", &dir_name[0], SLASH, &file_array[i].filename[0]);
		g_remove(&file_to_del[0]);
	}

	g_dir_close(dir);
	free(file_array);
}



