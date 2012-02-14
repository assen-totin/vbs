// This file is a part of Voody Blue Subtitler suit.
// 
// Author: Assen Totin <assen.totin@gmail.com>
//
// Home page: http://bilbo.online.bg/~assen/vbs
//
// This software is released under GNU General Public License.
// See the LICENSE file for details or visit http://www.gnu.org/copyleft/gpl.html 
// for details.

#include "main.h"


void escapeFileName(char *fileName, char *fileNameEscaped) {
	// Escape spaces in file name
	if (strstr(fileName, " ")) {
		char localName[1024];
		sprintf(&localName[0], "%s", fileName);

		char *str1, *token;
		int j;
		for (j = 1, str1 = &localName[0]; ; j++, str1 = NULL) {
			token = strtok(str1, " ");
			if (token == NULL){break;}
			if (j == 1) {sprintf(fileNameEscaped, "%s", token);}
			else {sprintf(fileNameEscaped, "%s\\ %s", fileNameEscaped, token);}
		}
	}
	else {sprintf(fileNameEscaped, "%s", fileName);}
}


void convertTimeSrt(unsigned int theTime, char *res, int flag) {

	div_t qH = div(theTime, 3600000);
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

	if (flag == 1) {sprintf(res, "0%u:%s:%s,%s", qH.quot, unfM, unfS, unfMS);}
	else if (flag == 2) {sprintf(res, "0%u:%s:%s", qH.quot, unfM, unfS);}
}


gboolean exportSubtitlesSrt(GtkTreeModel *model, GtkTreePath *path, GtkTreeIter *iter, gpointer userdata) {
	struct exportSub *export_sub = userdata;
	
	gchar *line;
	char *lineStr, *partOne, linePrint[255];
	guint from, to;

	char CrLf[3];
	sprintf(CrLf, "\n");
	if (counter.config_export_cr) {sprintf(CrLf, "\r\n");}

	// Get all fields
	gtk_tree_model_get(model, iter, COL_LINE, &line, COL_FROM, &from, COL_TO, &to, -1);

	// Print next number
	export_sub->count++;
	fprintf(export_sub->fp, "%u%s", export_sub->count, &CrLf[0]);

	// Calc & print times
	char timeFrom[32], timeTo[32], timeLine[64];
	convertTimeSrt(from, &timeFrom[0], 1);
	convertTimeSrt(to, &timeTo[0], 1);

	sprintf(timeLine, "%s --> %s", timeFrom, timeTo);
	fprintf(export_sub->fp, "%s%s", timeLine, &CrLf[0]);

	// Convert back to CP-1251
	gchar *lineCP1251;
	gsize bytes_written;
	if (strlen(line) == 0){sprintf(line," ");}
	lineCP1251 = g_convert(line, strlen(line), counter.config_export_encoding, "UTF-8", NULL, &bytes_written, NULL);

	// Replace | with \n
	lineStr = lineCP1251;
	partOne = strtok(lineStr,"|");
	sprintf(&linePrint[0], "%s", partOne);

	while(partOne = strtok(NULL,"|")) {
		sprintf(&linePrint[0], "%s%s%s", &linePrint[0],&CrLf[0],partOne);
	}

	fprintf(export_sub->fp, "%s%s%s", &linePrint[0], &CrLf[0], &CrLf[0]);

	g_free(line);
	return FALSE;
}


void exportSubtitles() {
	struct exportSub export_sub_val, *export_sub;
	FILE *fp;
	GtkTreeModel *model;

	fp = fopen (counter.globalExportFile, "w");
	if (!fp) {error_handler("exportSubtitles","failed to export subtitles");}

	export_sub_val.fp = fp;
	export_sub_val.count = 0;

	export_sub = &export_sub_val;

	model = gtk_tree_view_get_model(GTK_TREE_VIEW(view));
	gtk_tree_model_foreach(model, exportSubtitlesSrt, export_sub);

	int retval = fclose(fp);

	// Do nothing; if this is not present, the GTK widget will pop-up a small window and move the focus to it. WTF?
	fprintf(counter.tmpFile, "Wrote exported subtitles - closing file desriptor returned %u\n", retval);
}


