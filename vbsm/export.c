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

void convert_time_srt(unsigned int theTime, char *res, int flag) {

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


gboolean export_subtitles_srt(GtkTreeModel *model, GtkTreePath *path, GtkTreeIter *iter, gpointer userdata) {
	struct exportSub *export_sub = userdata;
	
	gchar *line;
	char *partOne, linePrint[config.common.line_size];
	guint from, to;

	char CrLf[3];
	sprintf(CrLf, "\n");
	if (config.common.export_cr == 1) {sprintf(CrLf, "\r\n");}

	// Get all fields
	gtk_tree_model_get(model, iter, COL_LINE, &line, COL_FROM, &from, COL_TO, &to, -1);

	// Print next number
	export_sub->count++;
	fprintf(export_sub->fp_export, "%u%s", export_sub->count, &CrLf[0]);
	if (config.vbsm.video_backend == VBSM_VIDEO_BACKEND_MPLAYER)
		fprintf(export_sub->fp_mplayer, "%u%s", export_sub->count, &CrLf[0]);

	// Calc & print times
	char timeFrom[32], timeTo[32], timeLine[64];
	convert_time_srt(from, &timeFrom[0], 1);
	convert_time_srt(to, &timeTo[0], 1);

	sprintf(timeLine, "%s --> %s", timeFrom, timeTo);
	fprintf(export_sub->fp_export, "%s%s", timeLine, &CrLf[0]);
	if ((config.vbsm.video_backend == VBSM_VIDEO_BACKEND_MPLAYER) && (to > 0))
		fprintf(export_sub->fp_mplayer, "%s%s", timeLine, &CrLf[0]);

	// Convert back to non-UTF-8 encoding
	gchar *lineCP1251;
	gsize bytes_written;
	if (strlen(line) == 0)
		sprintf(line," ");
	if (strcmp(&config.common.export_encoding[0], "UTF-8") != 0)
		lineCP1251 = g_convert(line, strlen(line), config.common.export_encoding, "UTF-8", NULL, &bytes_written, NULL);
	else
		lineCP1251 = line;

	// Replace | with \n
	char line_fixed[config.common.line_size];
	strcpy(&line_fixed[0], lineCP1251);
	fix_new_line(&line_fixed[0]);
	fprintf(export_sub->fp_export, "%s%s%s", &line_fixed[0], &CrLf[0], &CrLf[0]);

	// For mplayer, only export subtitles which have been timed
	if ((config.vbsm.video_backend == VBSM_VIDEO_BACKEND_MPLAYER) &&  (to > 0)) {
		strcpy(&line_fixed[0], line);
		fix_new_line(&line_fixed[0]);
		fprintf(export_sub->fp_mplayer, "%s%s%s", &line_fixed[0], &CrLf[0], &CrLf[0]);
	}

	g_free(line);
	return FALSE;
}


void export_subtitles() {
	struct exportSub export_sub_val, *export_sub;
	FILE *fp_export, *fp_mplayer;
	GtkTreeModel *model;

	fp_export = fopen(config.common.export_filename, "w");
	if (!fp_export) 
		error_handler("export_subtitles", "failed to export subtitles", 1);

	if (config.vbsm.video_backend == VBSM_VIDEO_BACKEND_MPLAYER) {
		fp_mplayer = fopen(config.vbsm.sub_file_name, "w");
		if (!fp_mplayer)
			error_handler("export_subtitles", "failed to write mplayer subtitles", 1);
	}

	export_sub_val.fp_export = fp_export;

	if (config.vbsm.video_backend == VBSM_VIDEO_BACKEND_MPLAYER)
		export_sub_val.fp_mplayer = fp_mplayer;

	export_sub_val.count = 0;

	export_sub = &export_sub_val;

	model = gtk_tree_view_get_model(GTK_TREE_VIEW(config.vbsm.subtitles_view));
	gtk_tree_model_foreach(model, export_subtitles_srt, export_sub);

	int retval = fclose(fp_export);

	if (config.vbsm.video_backend == VBSM_VIDEO_BACKEND_MPLAYER)
		retval = fclose(fp_mplayer);

	// Do nothing; if this is not present, the GTK widget will pop-up a small window and move the focus to it. WTF?
	fprintf(config.vbsm.log_file_fp, "Wrote exported subtitles - closing file desriptor returned %u\n", retval);
}


