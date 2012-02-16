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
	if (config.vbsm.have_loaded_text) {return true;}
	else {
		warnDialog(window, "No text loaded!");
		return false;
	}
}


void clearStore() {
	GtkTreeModel     *model;
	GtkTreeIter       iter;
	bool flag = TRUE;

	model = GTK_TREE_MODEL(store);
	gtk_tree_model_get_iter_first(model, &iter);

	while (flag) {flag = gtk_list_store_remove (store, &iter);}
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
	if (!importTextFile) {error_handler("importFilterSrt","failed to open subtitles", 1);}
	FILE *fpIn = fopen (importTextFile, "r");

	char tmpName[32];
	sprintf(tmpName, "%s/vbsTempFile.XXXXXX", VBS_TMP_DIR);
	int mkstempRes = mkstemp(tmpName);
	if (mkstempRes == -1) {error_handler("importFilterSrt","failed to create temporary file", 1);}

	FILE *fpOut = fopen (tmpName, "w");

	char *line;
	line = malloc(256);
	if (!line) {error_handler("importFilterSrt","malloc failed", 1);}

	bool isNextLineSubt = false;
	bool wasPrevLineSubt = false;
	char *isLineTiming;
	char *timeBegin, *timeEnd;
	unsigned int timeBeginVal, timeEndVal;

	while (fgets(line, 255, fpIn)) {
		line[strlen(line) - 1] = 0;     /* kill '\n' */
		// An edmpty line closes subtitle
		if (strlen(line) < 2) {
			if (wasPrevLineSubt) {fprintf(fpOut, "\n", line);}
			isNextLineSubt = false;
			wasPrevLineSubt = false;
		}

		// Next Line will be a subtitle line only if current line includes timing
		else if (isLineTiming = strstr(line,"-->")) {
			timeBegin = strtok(line, "-->");
			timeEnd = strtok(NULL,"-->");
			timeBeginVal = convertTime(timeBegin);
			timeEndVal = convertTime(timeEnd);
			fprintf(fpOut, "%u %u ", timeBeginVal, timeEndVal);

			isNextLineSubt = true;
			wasPrevLineSubt = false;
		}

		else if (wasPrevLineSubt) {
			fprintf(fpOut, "|%s", line);
		}

		else if (isNextLineSubt) {
			fprintf(fpOut, "%s", line);
			wasPrevLineSubt = true;
		}
	}

	fclose(fpIn);
	fclose(fpOut);
	free(line);

	sprintf(importTextFile, "%s", tmpName);
}


void importText(char *importTextFile, int importFlag) {
	// Set import filter
	if (importFlag == VBS_IMPORT_FILTER_SRT) {importFilterSrt(importTextFile);}

	GtkTreeIter iter;

	char *line;
	line = malloc(256);
	if (!line) {error_handler("create_and_fill_model","malloc failed",1);}

	FILE *fp;
	fp = fopen (importTextFile, "r");
	if (!fp) {error_handler("importText","failed to open subtitles", 1);}

	int unsigned timeFrom = 0, timeTo = 0; 
	char *timeFromStr, *timeToStr, *lineRest, lineCopy[256];
	gchar *lineUTF8;
	gsize bytes_written;

	while (fgets(line, 255, fp)) {
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
		gtk_list_store_append (store, &iter);
		gtk_list_store_set (store, &iter, COL_LINE, lineUTF8, COL_FROM, timeFrom, COL_TO, timeTo, -1);
	}
	fclose(fp);
	free(line);

	if (importFlag == VBS_IMPORT_FILTER_SRT) {unlink(importTextFile);}

	GtkTreeSelection *selection;
	GtkTreeModel     *model;
	model = GTK_TREE_MODEL(store);

	selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(view));

	gtk_tree_model_get_iter_first(model, &iter);
	gtk_tree_selection_select_iter(selection, &iter);

	config.vbsm.have_loaded_text = true;
	exportSubtitles();
}

