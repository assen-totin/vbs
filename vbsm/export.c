// This file is a part of Voody Blue Subtitler suit.
// 
// Author: Assen Totin <assen.totin@gmail.com>
//
// Home page: http://www.zavedil.com/software-desktop-vbs
//
// This software is released under GNU General Public License.
// See the LICENSE file for details or visit http://www.gnu.org/copyleft/gpl.html 
// for details.

#include "../common/common.h"

gboolean export_subtitles_srt(GtkTreeModel *model, GtkTreePath *path, GtkTreeIter *iter, gpointer userdata) {
	struct exportSub *export_sub = userdata;
	
	gchar *line;
	char *part_one, line_print[config.common.line_size];
	guint from, to;

	char cr_lf[3];
	if (config.common.export_cr == 1) 
		sprintf(cr_lf, "\r\n");
	else if (config.common.export_cr == 0)
		sprintf(cr_lf, "\n");
	else 
		sprintf(cr_lf, "\r");

	// Get all fields
	gtk_tree_model_get(model, iter, COL_LINE, &line, COL_FROM, &from, COL_TO, &to, -1);

	// Print next number
	export_sub->count++;
	fprintf(export_sub->fp_export, "%u%s", export_sub->count, &cr_lf[0]);
	if (config.vbsm.video_backend == VBSM_VIDEO_BACKEND_MPLAYER)
		fprintf(export_sub->fp_mplayer, "%u%s", export_sub->count, &cr_lf[0]);

	// Calc & print times
	char time_from[32], time_to[32], time_line[64];
	convert_time_to_srt(from, &time_from[0], TIME_MSEC);
	convert_time_to_srt(to, &time_to[0], TIME_MSEC);

	sprintf(&time_line[0], "%s --> %s", &time_from[0], &time_to[0]);
	fprintf(export_sub->fp_export, "%s%s", &time_line[0], &cr_lf[0]);
	if ((config.vbsm.video_backend == VBSM_VIDEO_BACKEND_MPLAYER) && (to > 0))
		fprintf(export_sub->fp_mplayer, "%s%s", &time_line[0], &cr_lf[0]);

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
	fprintf(export_sub->fp_export, "%s%s%s", &line_fixed[0], &cr_lf[0], &cr_lf[0]);

	// For mplayer, only export subtitles which have been timed
	if ((config.vbsm.video_backend == VBSM_VIDEO_BACKEND_MPLAYER) &&  (to > 0)) {
		strcpy(&line_fixed[0], line);
		fix_new_line(&line_fixed[0]);
		fprintf(export_sub->fp_mplayer, "%s%s%s", &line_fixed[0], &cr_lf[0], &cr_lf[0]);
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


