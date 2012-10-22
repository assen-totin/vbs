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

void zero_timing(GtkAction *action, gpointer param){
	GtkTreeIter iter, sibling;
	GtkTreeSelection *selection;
	GtkTreeModel     *model;
	bool flag = TRUE;

	selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(config.vbsm.subtitles_view));

	if (strstr(gtk_action_get_name(action), "EditZeroCurent")) {
		// Zero current
		if (gtk_tree_selection_get_selected(selection, &model, &iter)) {
			gtk_list_store_set (config.vbsm.mplayer_store, &iter, COL_FROM, 0, COL_TO, 0, -1);
		}
	}
	else if (strstr(gtk_action_get_name(action), "EditZeroRest")) {
		// Zero from current
		if (gtk_tree_selection_get_selected(selection, &model, &iter)) {
			sibling = iter;
			while (flag) {
				// Move to next line
				gtk_tree_selection_unselect_iter(selection, &iter);
				flag = gtk_tree_model_iter_next(model, &iter);
				if (flag) {
					gtk_tree_selection_select_iter(selection, &iter);
					gtk_list_store_set (config.vbsm.mplayer_store, &iter, COL_FROM, 0, COL_TO, 0, -1);
				}
			}
			gtk_tree_selection_select_iter(selection, &sibling);
		}
	}
}


void insert_subtitle(GtkAction *action, gpointer param){
	GtkTreeIter iter, sibling;
	GtkTreeSelection *selection;
	GtkTreeModel     *model;

	selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(config.vbsm.subtitles_view));

	if (strstr(gtk_action_get_name(action), "EditInsertBefore")) {
		// New before current
		if (gtk_tree_selection_get_selected(selection, &model, &sibling)) {
			gtk_list_store_insert_before (config.vbsm.mplayer_store, &iter, &sibling);
			gtk_list_store_set (config.vbsm.mplayer_store, &iter, COL_LINE, " ", COL_FROM, 0, COL_TO, 0, -1);
		}
	}
	else if (strstr(gtk_action_get_name(action), "EditInsertAfter")) {
		// New after current
		if (gtk_tree_selection_get_selected(selection, &model, &sibling)) {
			gtk_list_store_insert_after (config.vbsm.mplayer_store, &iter, &sibling);
			gtk_list_store_set (config.vbsm.mplayer_store, &iter, COL_LINE, " ", COL_FROM, 0, COL_TO, 0, -1);
		}
	}
	else if (strstr(gtk_action_get_name(action), "EditDelete")) {
		// Delete current
		if (gtk_tree_selection_get_selected(selection, &model, &sibling)) {
			gtk_list_store_remove (config.vbsm.mplayer_store, &sibling);
		}
	}
}


void help_contents(GtkWidget *widget, gpointer window) {
        GtkWidget *dialog;

	char *help_text = get_help_text();

	if (!help_text) {
		dialog = gtk_message_dialog_new(GTK_WINDOW(config.vbsm.window),
        	        GTK_DIALOG_DESTROY_WITH_PARENT | GTK_DIALOG_MODAL,
                	GTK_MESSAGE_ERROR,
	                GTK_BUTTONS_OK,
        	        _("Help file not found."));
	        gtk_window_set_title(GTK_WINDOW(dialog), _("Error"));
	}
	else {
		dialog = gtk_message_dialog_new(GTK_WINDOW(config.vbsm.window),
	                GTK_DIALOG_DESTROY_WITH_PARENT | GTK_DIALOG_MODAL,
        	        GTK_MESSAGE_INFO,
                	GTK_BUTTONS_OK,
	                _("Help")
		);
		gtk_window_set_title(GTK_WINDOW(dialog), _("Help"));

		GtkWidget *label = gtk_label_new(help_text);
		gtk_label_set_line_wrap(GTK_LABEL(label), TRUE);

		GtkWidget *scrolled_window = gtk_scrolled_window_new(NULL, NULL);
        	gtk_widget_set_size_request(scrolled_window, 480, 320);

		gtk_scrolled_window_add_with_viewport(GTK_SCROLLED_WINDOW(scrolled_window), label);
        	gtk_container_add(GTK_CONTAINER(gtk_message_dialog_get_message_area(GTK_MESSAGE_DIALOG(dialog))), scrolled_window);
	        gtk_widget_show(label);
		gtk_widget_show(scrolled_window);
	}

	gtk_dialog_run(GTK_DIALOG(dialog));

        gtk_widget_destroy(dialog);

	if (help_text)
		free(help_text);
}


void set_timer(GtkWidget *widget, gpointer window) {
	config.common.init_timestamp_msec = get_time_msec();

	GtkWidget *dialog = gtk_message_dialog_new(GTK_WINDOW(config.vbsm.window),
		GTK_DIALOG_DESTROY_WITH_PARENT | GTK_DIALOG_MODAL,
		GTK_MESSAGE_INFO,
		GTK_BUTTONS_OK,
		_("Internal timer set to zero."), "title");

	gtk_window_set_title(GTK_WINDOW(dialog), _("Set Start Time"));
	gtk_dialog_run(GTK_DIALOG(dialog));
	gtk_widget_destroy(dialog);
}

void quitDialog(GtkWidget *widget, gpointer window) {
	GtkWidget *dialog;
	dialog = gtk_message_dialog_new(GTK_WINDOW(config.vbsm.window),
		GTK_DIALOG_DESTROY_WITH_PARENT | GTK_DIALOG_MODAL,
		GTK_MESSAGE_QUESTION,
		GTK_BUTTONS_OK_CANCEL,
		_("Really quit?"));

	gtk_window_set_title(GTK_WINDOW(dialog), _("Really quit?"));

	if (gtk_dialog_run (GTK_DIALOG (dialog)) == GTK_RESPONSE_OK) {
		if (config.vbsm.video_backend == VBSM_VIDEO_BACKEND_MPLAYER) {
#ifdef HAVE_MPLAYER
			if (mplayer_is_alive()) {
				mplayer_pipe_write("quit");
				int status;
				waitpid(-1, &status, 0);
			}
			unlink(config.vbsm.sub_file_name);
#endif
		}
		fclose(config.vbsm.log_file_fp);
		unlink(config.vbsm.log_file_name);

		gtk_widget_destroy(dialog);

		gtk_main_quit();
	}
	else
		gtk_widget_destroy(dialog);
}


void fileDialogSave(GtkAction *action, gpointer param) {
        GtkWidget *fileDialogWidget;
        char fileDialogTitle[64];
        char fileDialogFile[256];

        fileDialogWidget = gtk_file_chooser_dialog_new ("Chose File", GTK_WINDOW(config.vbsm.window),
                                      GTK_FILE_CHOOSER_ACTION_SAVE,
                                      GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
                                      GTK_STOCK_OPEN, GTK_RESPONSE_ACCEPT,
                                      NULL);

        sprintf(&fileDialogFile[0], "%s/Desktop", g_get_home_dir());
        gtk_file_chooser_set_current_folder (GTK_FILE_CHOOSER (fileDialogWidget), &fileDialogFile[0]);

        if (gtk_dialog_run (GTK_DIALOG (fileDialogWidget)) == GTK_RESPONSE_ACCEPT) {
                char *filename = gtk_file_chooser_get_filename (GTK_FILE_CHOOSER (fileDialogWidget));

		if (strstr(gtk_action_get_name(action), "TextExportDestination")) {
			strcpy(&config.common.export_filename[0], filename);
		}

                g_free (filename);
        }

        gtk_widget_destroy (fileDialogWidget);

}

void fileDialogOpen(GtkAction *action, gpointer param) {
	GtkWidget *fileDialogWidget;
	char fileDialogTitle[64];
	char fileDialogFile[256];

	fileDialogWidget = gtk_file_chooser_dialog_new ("Chose File", GTK_WINDOW(config.vbsm.window),
				      GTK_FILE_CHOOSER_ACTION_OPEN,
				      GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
				      GTK_STOCK_OPEN, GTK_RESPONSE_ACCEPT,
				      NULL);

	sprintf(&fileDialogFile[0], "%s/Desktop", g_get_home_dir());
	gtk_file_chooser_set_current_folder (GTK_FILE_CHOOSER (fileDialogWidget), &fileDialogFile[0]);

	if (gtk_dialog_run (GTK_DIALOG (fileDialogWidget)) == GTK_RESPONSE_ACCEPT) {
		char *filename = gtk_file_chooser_get_filename (GTK_FILE_CHOOSER (fileDialogWidget));

		if (strstr(gtk_action_get_name(action), "TextImportPlain")) {
			// Import Text-only
		        clear_store();
		        import_subtitles(filename, VBS_IMPORT_FILTER_TEXT);
		}
		else if (strstr(gtk_action_get_name(action), "TextImportSubrip")) {
			// Import SRT
		        clear_store();
			import_subtitles(filename, VBS_IMPORT_FILTER_SRT);
		}
	        else if (strstr(gtk_action_get_name(action), "VideoImport")) {
        	        // Load Video
        	        config.vbsm.have_loaded_video = true;
		        if (config.vbsm.video_backend == VBSM_VIDEO_BACKEND_MPLAYER) {
#ifdef HAVE_MPLAYER
		                mplayer_load_video(filename);
#endif
		        }
		        else if (config.vbsm.video_backend == VBSM_VIDEO_BACKEND_GSTREAMER) {
#ifdef HAVE_GSTREAMER
                		gstreamer_init(filename);
#endif
		        }                	
		}

		g_free (filename);
	}

	gtk_widget_destroy (fileDialogWidget);
}

void set_video_backend (GtkWidget *widget, gpointer window) {
        GtkWidget *dialog = gtk_message_dialog_new(GTK_WINDOW(config.vbsm.window),
                GTK_DIALOG_DESTROY_WITH_PARENT | GTK_DIALOG_MODAL,
                GTK_MESSAGE_QUESTION,
                GTK_BUTTONS_OK_CANCEL,
                _("Select video backend:\n(requires restart)"));
        gtk_window_set_title(GTK_WINDOW(dialog), _("Video Backend"));

        GtkWidget *combo = gtk_combo_box_text_new();
        int n_video_backends = sizeof (video_backends) / sizeof (video_backends[0]);
        int i;
        for (i=0; i<n_video_backends; i++) {
                gtk_combo_box_text_append_text (GTK_COMBO_BOX_TEXT(combo), video_backends[i].name);
                if (video_backends[i].num == config.vbsm.video_backend)
                        gtk_combo_box_set_active(GTK_COMBO_BOX(combo), i);
        }

        gtk_container_add(GTK_CONTAINER(gtk_message_dialog_get_message_area(GTK_MESSAGE_DIALOG(dialog))), combo);
        gtk_widget_show(combo);

        if (gtk_dialog_run(GTK_DIALOG(dialog)) == GTK_RESPONSE_OK) {
		for (i=0; i<n_video_backends; i++) {
			if (strstr(&video_backends[i].name[0], (char *) gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(combo)))) {
				config.vbsm.video_backend = video_backends[i].num;
				break;
			}
		}
                write_config();
        }

        gtk_widget_destroy(dialog);
}


void set_video_output (GtkWidget *widget, gpointer window) {
        int i, dialog_type, buttons;
        bool show_menu_output = false;
        char msg[1024];
	GtkWidget *combo;

	int n_video_backends = sizeof (video_backends) / sizeof (video_backends[0]);
        for (i=0; i<n_video_backends; i++) {
                if (video_backends[i].num == config.vbsm.video_backend)
                        if (video_backends[i].show_menu_output)
                                show_menu_output = true;
        }

        if (show_menu_output) {
                strcpy(&msg[0], _("Select video output for GStreamer:\n(requires restart)"));
		dialog_type = GTK_MESSAGE_QUESTION;
		buttons = GTK_BUTTONS_OK_CANCEL;
	}
        else {
                strcpy(&msg[0], _("Video output selection not available for current backend."));
		dialog_type = GTK_MESSAGE_INFO;
		buttons = GTK_BUTTONS_CANCEL;
	}

        GtkWidget *dialog = gtk_message_dialog_new(GTK_WINDOW(config.vbsm.window),
                GTK_DIALOG_DESTROY_WITH_PARENT | GTK_DIALOG_MODAL,
                dialog_type,
		buttons, 
		&msg[0]);
        gtk_window_set_title(GTK_WINDOW(dialog), _("Video Output"));

	int n_video_outputs = sizeof (video_outputs) / sizeof (video_outputs[0]);
        if (show_menu_output) {
		combo = gtk_combo_box_text_new();
                for (i=0; i<n_video_outputs; i++) {
                        gtk_combo_box_text_append_text (GTK_COMBO_BOX_TEXT(combo), video_outputs[i].name);
                        if (strstr(&video_outputs[i].code[0], &config.vbsm.gstreamer_video_sink[0]))
                                gtk_combo_box_set_active(GTK_COMBO_BOX(combo), i);
                }
		gtk_container_add(GTK_CONTAINER(gtk_message_dialog_get_message_area(GTK_MESSAGE_DIALOG(dialog))), combo);
		gtk_widget_show(combo);
        }

        if (gtk_dialog_run(GTK_DIALOG(dialog)) == GTK_RESPONSE_OK) {
	        for (i=0; i<n_video_outputs; i++) {
	                if (strstr(video_outputs[i].name, (char *) gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(combo)))) {
	                        strcpy(&config.vbsm.gstreamer_video_sink[0], &video_outputs[i].code[0]);
        	                break;
                	}
        	}
                write_config();
        }

        gtk_widget_destroy(dialog);
}


char *get_help_text() {
	char *locale = setlocale (LC_ALL, NULL);
	char *lang = strtok(locale, ".");
	char file[1024];
	sprintf(&file[0], "%s/%s/LC_MESSAGES/vbs-help.mo", LOCALEDIR, lang);

	FILE *fp_help = fopen(&file[0], "r");
	if (!fp_help) {
		error_handler("help_contents", "could not open help file", 0);
		return NULL;
	}

	char *line = malloc(1024 * sizeof(char));
	char *help_text = malloc(1024 * sizeof(char));
	if (!line || !help_text)
		error_handler("main","malloc failed", 1);

	int cnt = 1;
	while (fgets(line, 1024, fp_help)) {
		sprintf(help_text, "%s%s", help_text, line);
		void *_tmp = realloc(help_text, (cnt + 1) * 1024 * sizeof(char));
		help_text = (char *) _tmp;
		cnt++;
	}

	free(line);

	return help_text;
}

