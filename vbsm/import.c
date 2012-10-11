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

bool haveLoadedText(GtkWidget *window){
	if (config.vbsm.have_loaded_text) 
		return true;
	else {
		warnDialog(window, "No text loaded!");
		return false;
	}
}


void clearStore() {
	GtkTreeModel     *model;
	GtkTreeIter       iter;
	bool flag = TRUE;

	model = GTK_TREE_MODEL(config.vbsm.mplayer_store);
	gtk_tree_model_get_iter_first(model, &iter);

	while (flag) {flag = gtk_list_store_remove (config.vbsm.mplayer_store, &iter);}
}


unsigned int convertTime(char *inTime) {
	char *strHrs, *strMin, *strTmp, *strSec, *strMil;
	strHrs = strtok(inTime, ":");
	strMin = strtok(NULL, ":");
	strTmp = strtok(NULL, ":");
	strSec = strtok(strTmp, ",");
	strMil = strtok(NULL, ",");

	int resHrs = atoi(strHrs);
	int resMin = atoi(strMin);
	int resSec = atoi(strSec);
	int resMil = atoi(strMil);

	int res = resHrs*3600000 + resMin*60000 + resSec*1000 + resMil;

	return res;
}


void importFilterSrt(char *importTextFile) {
	if (!importTextFile) 
		error_handler("importFilterSrt","failed to open subtitles", 1);

	FILE *fpIn = fopen (importTextFile, "r");

	char tmpName[32];
	sprintf(tmpName, "%s/vbsTempFile.XXXXXX", VBS_TMP_DIR);
	int mkstempRes = mkstemp(tmpName);
	if (mkstempRes == -1) 
		error_handler("importFilterSrt","failed to create temporary file", 1);

	FILE *fpOut = fopen (tmpName, "w");

	char *line_in = malloc(config.common.line_size);
	if (!line_in) 
		error_handler("importFilterSrt","malloc failed", 1);

        char *line_tmp = malloc(config.common.line_size);
        if (!line_tmp)
                error_handler("proc_subtitle_local","malloc failed", 1);

	char *tmp1, *tmp2;
	char *timeBegin, *timeEnd;
	unsigned int timeBeginVal, timeEndVal;
	int counter = -1;

	while (fgets(line_in, config.common.line_size, fpIn)) {
                // An empty line closes subtitle
                if (strlen(line_in) < 3) {
                        if (counter > 0) {
				fprintf(fpOut, "\n");
                                counter = -1;
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
                        timeBegin = strtok(line_in, "-->");
                        timeEnd = strtok(NULL, "-->");

                        strcpy(line_tmp, timeBegin);
                        timeBeginVal = convertTime(line_tmp);

                        strcpy(line_tmp, timeEnd);
                        timeEndVal = convertTime(line_tmp);

			fprintf(fpOut, "%u %u ", timeBeginVal, timeEndVal);

			counter = 0;

                        continue;
                }

                if (counter == 0) {
			fprintf(fpOut, "%s", line_in);
                        counter++;
                        continue;
                }

                else if (counter > 0) {
			fprintf(fpOut, "|%s", line_in);
                        counter++;
                        continue;
                }
	}

	fclose(fpIn);
	fclose(fpOut);

	free(line_in);
	free(line_tmp);

	sprintf(importTextFile, "%s", tmpName);
}


void importText(char *importTextFile, int importFlag) {
	// Set import filter
	if (importFlag == VBS_IMPORT_FILTER_SRT) 
		importFilterSrt(importTextFile);

	GtkTreeIter iter;

	char *line;
	line = malloc(config.common.line_size);
	if (!line) {error_handler("create_and_fill_model","malloc failed",1);}

	FILE *fp;
	fp = fopen (importTextFile, "r");
	if (!fp) {error_handler("importText","failed to open subtitles", 1);}

	int unsigned timeFrom = 0, timeTo = 0; 
	char *timeFromStr, *timeToStr, *lineRest, lineCopy[config.common.line_size];
	gchar *lineUTF8;
	gsize bytes_written;

	while (fgets(line, config.common.line_size, fp)) {
		line[strlen(line) - 1] = 0;     /* kill '\n' */

		// Kill any remaining <CR>
		char *CR;
		while (CR = strstr(line, "\r")) {strncpy(CR, " ", 1);}

		// STR tmp file parser
		if (importFlag == VBS_IMPORT_FILTER_SRT) {
			sprintf(lineCopy, "%s", line);
			timeFromStr = strtok(&lineCopy[0], " ");
			timeToStr = strtok(NULL, " ");
			lineRest = strtok(NULL, "$$$");			/* Just a non-existing delimiter to obtain the rest of the line */
			timeFrom = atoi(timeFromStr);
			timeTo = atoi(timeToStr);
		}
		else {
			lineRest = line;
		}

		if (lineRest) {
			if (strcmp(&config.common.import_encoding[0], "UTF-8") != 0) 
				lineUTF8 = g_convert(lineRest, strlen(lineRest), "UTF-8", config.common.import_encoding, NULL, &bytes_written, NULL);
			else
				lineUTF8 = lineRest;
		}

		// Append row
		gtk_list_store_append (config.vbsm.mplayer_store, &iter);
		gtk_list_store_set (config.vbsm.mplayer_store, &iter, COL_LINE, lineUTF8, COL_FROM, timeFrom, COL_TO, timeTo, -1);
	}

	fclose(fp);
	free(line);

	if (importFlag == VBS_IMPORT_FILTER_SRT)
		unlink(importTextFile);
	
	GtkTreeSelection *selection;
	GtkTreeModel     *model;
	model = GTK_TREE_MODEL(config.vbsm.mplayer_store);

	selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(config.vbsm.subtitles_view));

	gtk_tree_model_get_iter_first(model, &iter);
	gtk_tree_selection_select_iter(selection, &iter);

	config.vbsm.have_loaded_text = true;
	export_subtitles();

}

