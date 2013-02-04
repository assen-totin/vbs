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

bool have_loaded_text(){
	if (config.vbsm.have_loaded_text) 
		return true;
	else {
		show_warning_subtitles();
		return false;
	}
}


void clear_store() {
	GtkTreeIter iter;
	bool flag = TRUE;

	GtkTreeModel *model = GTK_TREE_MODEL(config.vbsm.mplayer_store);
	gtk_tree_model_get_iter_first(model, &iter);

	while (flag) 
		flag = gtk_list_store_remove (config.vbsm.mplayer_store, &iter);
}


void import_subtitles(char *filename, int file_format, int *imp_err_flag) {
	struct subtitle_srt *sub_array;
	GtkTreeIter iter;
	int i, total_subs, import_error_flag = 0;

	if (file_format == VBS_IMPORT_FILTER_TEXT)
		sub_array = import_subtitles_text(filename, &total_subs, &import_error_flag);
	else if (file_format == VBS_IMPORT_FILTER_SRT)
		sub_array = import_subtitles_srt(filename, &total_subs, &import_error_flag);

	if (import_error_flag) {
		*imp_err_flag = 1;
		return;
	}

	for (i=0; i<total_subs; i++) {
		gtk_list_store_append (config.vbsm.mplayer_store, &iter);
		gtk_list_store_set (config.vbsm.mplayer_store, &iter, COL_LINE, &sub_array[i].sub[0], COL_FROM, sub_array[i].time_from, COL_TO, sub_array[i].time_to, -1);
	}
	
	GtkTreeModel *model = GTK_TREE_MODEL(config.vbsm.mplayer_store);
	GtkTreeSelection *selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(config.vbsm.subtitles_view));
	gtk_tree_model_get_iter_first(model, &iter);
	gtk_tree_selection_select_iter(selection, &iter);

	config.vbsm.have_loaded_text = true;
	export_subtitles();

	free(sub_array);

        gtk_widget_grab_focus(config.vbsm.subtitles_view);
}


struct subtitle_srt *import_subtitles_text(char *filename, int *counter, int *import_error_flag) {
	int counter_array = 0;
	char *line_utf8;
	gsize bytes_written;

	FILE *fp_in = fopen (filename, "r");
	if (!fp_in)
		error_handler("import_subtitles_text", "failed to open subtitles", 1);

	char *line_in = malloc(config.common.line_size);
	char *line_tmp = malloc(config.common.line_size);
	if (!line_in || !line_tmp)
		error_handler("import_subtitles_text", "malloc failed", 1);

	struct subtitle_srt *sub_array = malloc(sizeof(struct subtitle_srt));
	if (!sub_array)
		error_handler("import_subtitles_srt", "malloc failed", 1);

	while (fgets(line_in, config.common.line_size, fp_in)) {
                // Check for wrong file
                if (strlen(line_in) > (config.common.line_size - 4)) {
                        *import_error_flag = 1;
                        return sub_array;
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

		// UTF-8
		if (strcmp(&config.common.import_encoding[0], "UTF-8") != 0)
			line_utf8 = g_convert(line_in, strlen(line_in), "UTF-8", config.common.import_encoding, NULL, &bytes_written, NULL);
		else
			line_utf8 = line_in;

		// Add subtitles to the array		
		void *_tmp = realloc(sub_array, ((counter_array + 1) * sizeof(struct subtitle_srt)));
		if (!_tmp)
			error_handler("import_subtitles_text", "realloc failed", 1);
		sub_array = (struct subtitle_srt *)_tmp;
		sub_array[counter_array].time_from = 0;
		sub_array[counter_array].time_to = 0;
		strcpy(&sub_array[counter_array].sub[0], line_utf8);
		counter_array++;
	}

	free(line_in);
	free(line_tmp);

	*counter = counter_array;

	return sub_array;
}

