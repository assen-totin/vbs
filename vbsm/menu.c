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

void zeroTiming(GtkAction *action, gpointer param){
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


void insertBefore(GtkAction *action, gpointer param){
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


void helpContents(GtkAction *action, gpointer param) {
	GtkWidget *quitDialog, *quitLabel, *quitFrame;

	quitDialog = gtk_dialog_new_with_buttons (_("Using VBS"), GTK_WINDOW(config.vbsm.window), GTK_DIALOG_MODAL, NULL);

	GtkWidget *buttonCancel = gtk_dialog_add_button (GTK_DIALOG(quitDialog), GTK_STOCK_OK, GTK_RESPONSE_CANCEL);
	gtk_dialog_set_default_response (GTK_DIALOG (quitDialog), GTK_RESPONSE_CANCEL) ;
	g_signal_connect (G_OBJECT(buttonCancel), "clicked", G_CALLBACK (quitDialogCancel), (gpointer) quitDialog);

	quitFrame = gtk_frame_new("");
	quitLabel = gtk_label_new(VBS_MENU_HELP_TEXT);
	gtk_container_add(GTK_CONTAINER(quitFrame), quitLabel);
	gtk_container_add(GTK_CONTAINER(gtk_dialog_get_content_area(GTK_DIALOG(quitDialog))), quitFrame);

	gtk_widget_show_all (quitDialog);
}


void setTimer(GtkAction *action, gpointer param) {
	config.common.init_timestamp = time(NULL);

        GtkWidget *quitDialog, *quitLabel, *quitFrame;

        quitDialog = gtk_dialog_new_with_buttons (_("Set Start Time"), GTK_WINDOW(config.vbsm.window), GTK_DIALOG_MODAL, NULL);

        GtkWidget *buttonCancel = gtk_dialog_add_button (GTK_DIALOG(quitDialog), GTK_STOCK_OK, GTK_RESPONSE_CANCEL);
        gtk_dialog_set_default_response (GTK_DIALOG (quitDialog), GTK_RESPONSE_CANCEL) ;
        g_signal_connect (G_OBJECT(buttonCancel), "clicked", G_CALLBACK (quitDialogCancel), (gpointer) quitDialog);

        quitFrame = gtk_frame_new("");
        quitLabel = gtk_label_new(_("Internal timer set to zero."));
        gtk_container_add(GTK_CONTAINER(quitFrame), quitLabel);
        gtk_container_add(GTK_CONTAINER(gtk_dialog_get_content_area(GTK_DIALOG(quitDialog))), quitFrame);

        gtk_widget_show_all (quitDialog);
}

static void quitDialogOK( GtkWidget *widget, gpointer data ){
	GtkWidget *quitDialog = data;
	gtk_widget_destroy(quitDialog);

	if (config.vbsm.video_backend == VBSM_VIDEO_BACKEND_MPLAYER) {
#ifdef HAVE_MPLAYER
		if (mplayer_is_alive()) {
			mplayer_pipe_write("quit");
			int status;
			waitpid(-1, &status, 0);
		}
#endif
	}

	fclose(config.vbsm.log_file_fp);
	unlink(config.vbsm.log_file_name);

        unlink(config.vbsm.sub_file_name);

	gtk_main_quit();
}

void quitDialog(GtkAction *action, gpointer param) {
	GtkWidget *quitDialog, *quitLabel;
	char quitMessage[1024];

	sprintf(quitMessage, "%s %s\n", _("Your subtitles are exported to:"), &config.common.export_filename[0]); 

	quitDialog = gtk_dialog_new_with_buttons (_("Really quit?"), GTK_WINDOW(config.vbsm.window), GTK_DIALOG_MODAL, NULL);

	GtkWidget *buttonOK = gtk_dialog_add_button (GTK_DIALOG(quitDialog), GTK_STOCK_OK, GTK_RESPONSE_OK);
	GtkWidget *buttonCancel = gtk_dialog_add_button (GTK_DIALOG(quitDialog), GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL);
	gtk_dialog_set_default_response (GTK_DIALOG (quitDialog), GTK_RESPONSE_CANCEL) ;

	quitLabel = gtk_label_new(quitMessage);

	gtk_container_add(GTK_CONTAINER(gtk_dialog_get_content_area(GTK_DIALOG(quitDialog))), quitLabel);

	g_signal_connect (G_OBJECT(buttonOK), "clicked", G_CALLBACK (quitDialogOK), (gpointer) quitDialog);

	g_signal_connect (G_OBJECT(buttonCancel), "clicked", G_CALLBACK (quitDialogCancel), (gpointer) quitDialog);

	gtk_widget_show_all (quitDialog);
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
		        clearStore();
		        importText(filename, VBS_IMPORT_FILTER_TEXT);
		}
		else if (strstr(gtk_action_get_name(action), "TextImportSubrip")) {
			// Import SRT
		        clearStore();
			importText(filename, VBS_IMPORT_FILTER_SRT);
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

void set_video_backend (GtkAction *action, gpointer param) {
        GtkWidget *quitDialog, *quitLabel;

        quitDialog = gtk_dialog_new_with_buttons (_("Video Backend"), GTK_WINDOW(config.vbsm.window), GTK_DIALOG_MODAL, NULL);

        GtkWidget *buttonOK = gtk_dialog_add_button (GTK_DIALOG(quitDialog), GTK_STOCK_OK, GTK_RESPONSE_OK);
        GtkWidget *buttonCancel = gtk_dialog_add_button (GTK_DIALOG(quitDialog), GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL);
        gtk_dialog_set_default_response (GTK_DIALOG (quitDialog), GTK_RESPONSE_OK) ;
        g_signal_connect (G_OBJECT(buttonCancel), "clicked", G_CALLBACK (quitDialogCancel), (gpointer) quitDialog);
        g_signal_connect (G_OBJECT(buttonOK), "clicked", G_CALLBACK (set_video_backend_ok), (gpointer) quitDialog);

        char quitMessage[1024];
        sprintf(quitMessage, "%s\n", _("Select video backend:\n(requires restart)"));
        quitLabel = gtk_label_new(quitMessage);
        gtk_container_add(GTK_CONTAINER(gtk_dialog_get_content_area(GTK_DIALOG(quitDialog))), quitLabel);

        config.vbsm.menu_widget = gtk_combo_box_text_new();

        int n_video_backends = sizeof (video_backends) / sizeof (video_backends[0]);
        int i;
        for (i=0; i<n_video_backends; i++) {
                gtk_combo_box_text_append_text (GTK_COMBO_BOX_TEXT(config.vbsm.menu_widget), video_backends[i].name);
                if (video_backends[i].num == config.vbsm.video_backend)
                        gtk_combo_box_set_active(GTK_COMBO_BOX(config.vbsm.menu_widget), i);
        }
        gtk_container_add(GTK_CONTAINER(gtk_dialog_get_content_area(GTK_DIALOG(quitDialog))), config.vbsm.menu_widget);

        gtk_widget_show_all (quitDialog);
}


void set_video_backend_ok(GtkWidget *widget, gpointer data) {
        GtkWidget *quitDialog = data;
	int i;
	char selected[1024];
        sprintf(&selected[0], "%s", gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(config.vbsm.menu_widget)));
	int n_video_backends = sizeof (video_backends) / sizeof (video_backends[0]);
	for (i=0; i<n_video_backends; i++) {
		if (strstr(&video_backends[i].name[0], &selected[0])) {
			config.vbsm.video_backend = video_backends[i].num;
			break;
		}
	}
        write_config();
        gtk_widget_destroy(quitDialog);
}


void set_video_output (GtkAction *action, gpointer param) {
        GtkWidget *quitDialog, *quitLabel;
	int i;
	bool show_menu_output = false;
	char quitMessage[1024];

	int n_video_backends = sizeof (video_backends) / sizeof (video_backends[0]);
	for (i=0; i<n_video_backends; i++) {
		if (video_backends[i].num == config.vbsm.video_backend)
			if (video_backends[i].show_menu_output)
				show_menu_output = true;
	}

        quitDialog = gtk_dialog_new_with_buttons (_("Video Output"), GTK_WINDOW(config.vbsm.window), GTK_DIALOG_MODAL, NULL);

	if (show_menu_output) {
		GtkWidget *buttonOK = gtk_dialog_add_button (GTK_DIALOG(quitDialog), GTK_STOCK_OK, GTK_RESPONSE_OK);
		gtk_dialog_set_default_response (GTK_DIALOG (quitDialog), GTK_RESPONSE_OK); 
	        g_signal_connect (G_OBJECT(buttonOK), "clicked", G_CALLBACK (set_video_output_ok), (gpointer) quitDialog);
	}
        GtkWidget *buttonCancel = gtk_dialog_add_button (GTK_DIALOG(quitDialog), GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL);
        g_signal_connect (G_OBJECT(buttonCancel), "clicked", G_CALLBACK (quitDialogCancel), (gpointer) quitDialog);

	if (show_menu_output)
	        sprintf(quitMessage, "%s\n", _("Select video output for GStreamer:\n(requires restart)"));
	else
		sprintf(quitMessage, "%s\n", _("Video output selection not available for current backend."));
        quitLabel = gtk_label_new(quitMessage);
        gtk_container_add(GTK_CONTAINER(gtk_dialog_get_content_area(GTK_DIALOG(quitDialog))), quitLabel);

	if (show_menu_output) {
	        config.vbsm.menu_widget = gtk_combo_box_text_new();

        	int n_video_outputs = sizeof (video_outputs) / sizeof (video_outputs[0]);
	        for (i=0; i<n_video_outputs; i++) {
        	        gtk_combo_box_text_append_text (GTK_COMBO_BOX_TEXT(config.vbsm.menu_widget), video_outputs[i].name);
                	if (strstr(&video_outputs[i].code[0], &config.vbsm.gstreamer_video_sink[0]))
                        	gtk_combo_box_set_active(GTK_COMBO_BOX(config.vbsm.menu_widget), i);
	        }
        	gtk_container_add(GTK_CONTAINER(gtk_dialog_get_content_area(GTK_DIALOG(quitDialog))), config.vbsm.menu_widget);
	}

        gtk_widget_show_all (quitDialog);
}


void set_video_output_ok(GtkWidget *widget, gpointer data) {
        GtkWidget *quitDialog = data;
        int i;
        char selected[1024];
        sprintf(&selected[0], "%s", gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(config.vbsm.menu_widget)));
	int n_video_outputs = sizeof (video_outputs) / sizeof (video_outputs[0]);
        for (i=0; i<n_video_outputs; i++) {
                if (strstr(video_outputs[i].name, &selected[0])) {
                        strcpy(&config.vbsm.gstreamer_video_sink[0], &video_outputs[i].code[0]);
                        break;
                }
        }
        write_config();
        gtk_widget_destroy(quitDialog);
}

void set_magic_key_ok(GtkWidget *widget, gpointer data) {
        GtkWidget *quitDialog = data;
        config.common.magic_key = atoi(gtk_entry_get_text(GTK_ENTRY(config.vbsm.menu_widget)));
        write_config();
        gtk_widget_destroy(quitDialog);
}

void set_magic_key (GtkAction *action, gpointer param) {
        GtkWidget *quitDialog, *quitLabel;

        quitDialog = gtk_dialog_new_with_buttons (VBSC_MENU_MAGIC_KEY_TITLE, GTK_WINDOW(config.vbsm.window), GTK_DIALOG_MODAL, NULL);

        GtkWidget *buttonOK = gtk_dialog_add_button (GTK_DIALOG(quitDialog), GTK_STOCK_OK, GTK_RESPONSE_OK);
        GtkWidget *buttonCancel = gtk_dialog_add_button (GTK_DIALOG(quitDialog), GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL);
        gtk_dialog_set_default_response (GTK_DIALOG (quitDialog), GTK_RESPONSE_OK) ;
        g_signal_connect (G_OBJECT(buttonCancel), "clicked", G_CALLBACK (quitDialogCancel), (gpointer) quitDialog);
        g_signal_connect (G_OBJECT(buttonOK), "clicked", G_CALLBACK (set_magic_key_ok), (gpointer) quitDialog);

        char quitMessage[1024];
        sprintf(&quitMessage[0], "%s\n", VBSC_MENU_MAGIC_KEY_TEXT);
        quitLabel = gtk_label_new(quitMessage);
        gtk_container_add(GTK_CONTAINER(gtk_dialog_get_content_area(GTK_DIALOG(quitDialog))), quitLabel);

        config.vbsm.menu_widget = gtk_entry_new();
        char tmp1[16];
        sprintf(&tmp1[0], "%u", config.common.magic_key);
        gtk_entry_set_text(GTK_ENTRY(config.vbsm.menu_widget), &tmp1[0]);
        gtk_container_add(GTK_CONTAINER(gtk_dialog_get_content_area(GTK_DIALOG(quitDialog))), config.vbsm.menu_widget);

        gtk_widget_show_all (quitDialog);
}

