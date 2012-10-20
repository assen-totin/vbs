// This file is a part of Voody Blue Subtitler suit.
// 
// Author: Assen Totin <assen.totin@gmail.com>
//
// Home page: http://bilbo.online.bg/~assen/vbs
//
// This software is released under GNU General Public License.
// See the LICENSE file for details or visit http://www.gnu.org/copyleft/gpl.html 
// for details.

#include "../common/common.h"

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


void split_path(char *path, char *dir, char *file) {
        char *tmp;
        char tmp_old[100];

        strcpy(&tmp_old[0], strtok(path, "/"));

	bzero(dir, sizeof(*dir));

        while(1) {
                tmp = strtok(NULL, "/");
                if(tmp) {
                        sprintf(dir, "%s/%s", dir, &tmp_old[0]);
                        sprintf(&tmp_old[0], "%s", tmp);
                }
                else
                        break;
        }

        strcpy(file, &tmp_old[0]);
}


long get_time_msec() {
	struct timeval curr_time;
	gettimeofday(&curr_time, NULL);
	return (long) (1000 * curr_time.tv_sec + (int) (curr_time.tv_usec / 1000));
}


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


struct subtitle_srt *import_subtitles_srt(char *filename, int *counter) {
	char *time_begin, *time_end;
	char *line_utf8;
	long time_begin_val, time_end_val;
	int counter_line = -1;
	int counter_array = 0;
	gint bytes_written;

	FILE *fp_in = fopen (filename, "r");
	if (!fp_in)
		error_handler("import_subtitles_srt", "failed to open subtitles", 1);

	char *line_in = malloc(config.common.line_size);
	char *line_out = malloc(config.common.line_size);
	char *line_tmp = malloc(config.common.line_size);
	if (!line_in || !line_out || !line_tmp)
		error_handler("import_subtitles_srt", "malloc failed", 1);
	bzero(line_out, config.common.line_size);

	struct subtitle_srt *sub_array = malloc(sizeof(struct subtitle_srt));
	if (!sub_array)
		error_handler("import_subtitles_srt", "malloc failed", 1);

	while (fgets(line_in, config.common.line_size, fp_in)) {
		// An empty line closes subtitle
                if (strlen(line_in) < 3) {
                        if (counter_line > 0) {
				// UTF-8
		                if (strcmp(&config.common.import_encoding[0], "UTF-8") != 0)
                		        line_utf8 = g_convert(line_out, strlen(line_out), "UTF-8", config.common.import_encoding, NULL, &bytes_written, NULL);
		                else
                		        line_utf8 = line_out;

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
                        continue;
                }
                else if (counter_line > 0) {
                        sprintf(line_out, "%s|%s", line_out, line_in);
                        counter_line++;
                        continue;
                }
	}

        free(line_in);
        free(line_out);
        free(line_tmp);

	*counter = counter_array;

	return sub_array;
}

