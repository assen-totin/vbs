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

	selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(config.vbsm.mplayer_view));

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


void fileDialogOK21( GtkWidget *fileDialogWidget, GtkFileSelection *fs ) {
	char importTextFile[1024];
	if (strlen(gtk_file_selection_get_filename (GTK_FILE_SELECTION (fs))) > 512) {error_handler("fileDialogOK21","Filename too long.", 1);}
	sprintf(importTextFile, "%s", gtk_file_selection_get_filename (GTK_FILE_SELECTION (fs)));
	clearStore();
	importText(&importTextFile[0], VBS_IMPORT_FILTER_TEXT);
}


void fileDialogOK22( GtkWidget *fileDialogWidget, GtkFileSelection *fs ) {
	char importTextFile[1024];
	if (strlen(gtk_file_selection_get_filename (GTK_FILE_SELECTION (fs))) > 512) {error_handler("fileDialogOK22","Filename too long.",1);}
	sprintf(importTextFile, "%s", gtk_file_selection_get_filename (GTK_FILE_SELECTION (fs)));
	clearStore();
	importText(&importTextFile[0], VBS_IMPORT_FILTER_SRT);
}


void fileDialogOK31( GtkWidget *fileDialogWidget, GtkFileSelection *fs ) {
	char video_file_name[1024];
	if (strlen(gtk_file_selection_get_filename (GTK_FILE_SELECTION (fs))) > 512) {error_handler("fileDialogOK31","Filename too long.", 1);}
	sprintf(video_file_name, "%s", gtk_file_selection_get_filename (GTK_FILE_SELECTION (fs)));
	if (config.vbsm.video_backend == VBSM_VIDEO_BACKEND_MPLAYER) {mplayer_load_video(video_file_name);}
	else if (config.vbsm.video_backend == VBSM_VIDEO_BACKEND_GSTREAMER) {
#ifdef HAVE_GSTREAMER
		gstreamer_init(video_file_name);
#endif
	}
}


void fileDialogOK41( GtkWidget *fileDialogWidget, GtkFileSelection *fs ) {
	// Stupid, but global variables need care.
	if (strlen(gtk_file_selection_get_filename (GTK_FILE_SELECTION (fs))) > 512) {error_handler("fileDialogOK41","Filename too long.",1);}
	sprintf(&config.common.export_filename[0], "%s", gtk_file_selection_get_filename (GTK_FILE_SELECTION (fs)));
}


void insertBefore(GtkAction *action, gpointer param){
	GtkTreeIter iter, sibling;
	GtkTreeSelection *selection;
	GtkTreeModel     *model;

	selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(config.vbsm.mplayer_view));

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

	quitDialog = gtk_dialog_new_with_buttons (VBS_MENU_HELP_TITLE, GTK_WINDOW(config.vbsm.window), GTK_DIALOG_MODAL, NULL);

	GtkWidget *buttonCancel = gtk_dialog_add_button (GTK_DIALOG(quitDialog), GTK_STOCK_OK, GTK_RESPONSE_CANCEL);
	gtk_dialog_set_default_response (GTK_DIALOG (quitDialog), GTK_RESPONSE_CANCEL) ;
	g_signal_connect (G_OBJECT(buttonCancel), "clicked", G_CALLBACK (quitDialogCancel), (gpointer) quitDialog);

	quitFrame = gtk_frame_new("");
	quitLabel = gtk_label_new(VBS_MENU_HELP_TEXT);
	gtk_container_add (GTK_CONTAINER (quitFrame), quitLabel);
	gtk_container_add (GTK_CONTAINER (GTK_DIALOG(quitDialog)->vbox), quitFrame);

	gtk_widget_show_all (quitDialog);
}


void setTimer(GtkAction *action, gpointer param) {
	config.common.init_timestamp = time(NULL);

        GtkWidget *quitDialog, *quitLabel, *quitFrame;

        quitDialog = gtk_dialog_new_with_buttons (VBS_MENU_TIMER_TITLE, GTK_WINDOW(config.vbsm.window), GTK_DIALOG_MODAL, NULL);

        GtkWidget *buttonCancel = gtk_dialog_add_button (GTK_DIALOG(quitDialog), GTK_STOCK_OK, GTK_RESPONSE_CANCEL);
        gtk_dialog_set_default_response (GTK_DIALOG (quitDialog), GTK_RESPONSE_CANCEL) ;
        g_signal_connect (G_OBJECT(buttonCancel), "clicked", G_CALLBACK (quitDialogCancel), (gpointer) quitDialog);

        quitFrame = gtk_frame_new("");
        quitLabel = gtk_label_new(VBS_MENU_TIMER_TEXT);
        gtk_container_add (GTK_CONTAINER (quitFrame), quitLabel);
        gtk_container_add (GTK_CONTAINER (GTK_DIALOG(quitDialog)->vbox), quitFrame);

        gtk_widget_show_all (quitDialog);
}

static void quitDialogOK( GtkWidget *widget, gpointer data ){
	GtkWidget *quitDialog = data;
	gtk_widget_destroy(quitDialog);

	if ((config.vbsm.video_backend == VBSM_VIDEO_BACKEND_MPLAYER) && (mplayer_is_alive())) {
		mplayer_pipe_write("quit");
		int status;
		waitpid(-1, &status, 0);
	}

	fclose(config.vbsm.log_file_fp);
	unlink(config.vbsm.log_file_name);

        unlink(config.vbsm.sub_file_name);

	gtk_main_quit();
}

void quitDialog(GtkAction *action, gpointer param) {
	GtkWidget *quitDialog, *quitLabel;
	char quitMessage[1024];

	sprintf(quitMessage, "%s %s\n", VBS_MENU_QUIT_TEXT, &config.common.export_filename[0]); 

	quitDialog = gtk_dialog_new_with_buttons (VBS_MENU_QUIT_TITLE, GTK_WINDOW(config.vbsm.window), GTK_DIALOG_MODAL, NULL);

	GtkWidget *buttonOK = gtk_dialog_add_button (GTK_DIALOG(quitDialog), GTK_STOCK_OK, GTK_RESPONSE_OK);
	GtkWidget *buttonCancel = gtk_dialog_add_button (GTK_DIALOG(quitDialog), GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL);
	gtk_dialog_set_default_response (GTK_DIALOG (quitDialog), GTK_RESPONSE_CANCEL) ;

	quitLabel = gtk_label_new(quitMessage);

	gtk_container_add (GTK_CONTAINER (GTK_DIALOG(quitDialog)->vbox), quitLabel);

	g_signal_connect (G_OBJECT(buttonOK), "clicked", G_CALLBACK (quitDialogOK), (gpointer) quitDialog);

	g_signal_connect (G_OBJECT(buttonCancel), "clicked", G_CALLBACK (quitDialogCancel), (gpointer) quitDialog);

	gtk_widget_show_all (quitDialog);
}


void fileDialog(GtkAction *action, gpointer param) {
	GtkWidget *fileDialogWidget;
	char fileDialogTitle[64];
	char fileDialogFile[256];

	if (strstr(gtk_action_get_name(action), "TextImportPlain")) {
		// Import Text-only
		sprintf(fileDialogTitle, "%s", VBS_MENU_IMPORT_TEXTONLY_TITLE);
		sprintf(fileDialogFile, "%s/", VBS_MENU_DEFAULT_PATH);
	}
	else if (strstr(gtk_action_get_name(action), "TextImportSubrip")) {
		// Import SRT
		sprintf(fileDialogTitle, "%s", VBS_MENU_IMPORT_SRT_TITLE);
		sprintf(fileDialogFile, "%s/", VBS_MENU_DEFAULT_PATH);
	}
	else if (strstr(gtk_action_get_name(action), "VideoImport")) {
		// Load Video
		sprintf(fileDialogTitle, "%s", VBS_MENU_IMPORT_VIDEO_TITLE);
		sprintf(fileDialogFile, "%s/", VBS_MENU_DEFAULT_PATH);
	}
	else if (strstr(gtk_action_get_name(action), "TextExportDestination")) {
		// Export Dialog
		sprintf(fileDialogTitle, "%s", VBS_MENU_EXPORT_TITLE);
		sprintf(fileDialogFile, "%s/", VBS_MENU_DEFAULT_PATH);
	}

	fileDialogWidget = gtk_file_selection_new (fileDialogTitle);

	/* Connect the ok_button to file_ok_sel function */
	if (strstr(gtk_action_get_name(action), "TextImportPlain"))
		// Import Text-only
		g_signal_connect (G_OBJECT (GTK_FILE_SELECTION (fileDialogWidget)->ok_button), "clicked", G_CALLBACK (fileDialogOK21), (gpointer) fileDialogWidget);
	else  if (strstr(gtk_action_get_name(action), "TextImportSubrip"))
			// Import SRT
			g_signal_connect (G_OBJECT (GTK_FILE_SELECTION (fileDialogWidget)->ok_button), "clicked", G_CALLBACK (fileDialogOK22), (gpointer) fileDialogWidget);
	else if (strstr(gtk_action_get_name(action), "VideoImport"))
			// Load Video
			g_signal_connect (G_OBJECT (GTK_FILE_SELECTION (fileDialogWidget)->ok_button), "clicked", G_CALLBACK (fileDialogOK31), (gpointer) fileDialogWidget);
	else if (strstr(gtk_action_get_name(action), "TextExportDestination"))
			// Export Dialog
			g_signal_connect (G_OBJECT (GTK_FILE_SELECTION (fileDialogWidget)->ok_button), "clicked", G_CALLBACK (fileDialogOK41), (gpointer) fileDialogWidget);

	g_signal_connect_swapped (G_OBJECT (GTK_FILE_SELECTION (fileDialogWidget)->ok_button), "clicked", G_CALLBACK (gtk_widget_destroy), G_OBJECT (fileDialogWidget));

	/* Connect the cancel_button to destroy the widget */
	g_signal_connect_swapped (G_OBJECT (GTK_FILE_SELECTION (fileDialogWidget)->cancel_button), "clicked", G_CALLBACK (gtk_widget_destroy), G_OBJECT (fileDialogWidget));

	/* Lets set the filename, as if this were a save dialog, and we are giving a default filename */
	gtk_file_selection_set_filename (GTK_FILE_SELECTION(fileDialogWidget), fileDialogFile);

	gtk_widget_show (fileDialogWidget);
}

void set_video_backend (GtkAction *action, gpointer param) {
        GtkWidget *quitDialog, *quitLabel;

        quitDialog = gtk_dialog_new_with_buttons (VBS_MENU_VIDEO_BACKEND_TITLE, GTK_WINDOW(config.vbsm.window), GTK_DIALOG_MODAL, NULL);

        GtkWidget *buttonOK = gtk_dialog_add_button (GTK_DIALOG(quitDialog), GTK_STOCK_OK, GTK_RESPONSE_OK);
        GtkWidget *buttonCancel = gtk_dialog_add_button (GTK_DIALOG(quitDialog), GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL);
        gtk_dialog_set_default_response (GTK_DIALOG (quitDialog), GTK_RESPONSE_OK) ;
        g_signal_connect (G_OBJECT(buttonCancel), "clicked", G_CALLBACK (quitDialogCancel), (gpointer) quitDialog);
        g_signal_connect (G_OBJECT(buttonOK), "clicked", G_CALLBACK (set_video_backend_ok), (gpointer) quitDialog);

        char quitMessage[1024];
        sprintf(quitMessage, "%s\n", VBS_MENU_VIDEO_BACKEND_TEXT);
        quitLabel = gtk_label_new(quitMessage);
        gtk_container_add (GTK_CONTAINER (GTK_DIALOG(quitDialog)->vbox), quitLabel);

        config.vbsm.menu_widget = gtk_combo_box_new_text();

        int n_video_backends = sizeof (video_backends) / sizeof (video_backends[0]);
        int i;
        for (i=0; i<n_video_backends; i++) {
                gtk_combo_box_append_text (GTK_COMBO_BOX(config.vbsm.menu_widget), video_backends[i].name);
                if (video_backends[i].num == config.vbsm.video_backend)
                        gtk_combo_box_set_active(GTK_COMBO_BOX(config.vbsm.menu_widget), i);
        }
        gtk_container_add (GTK_CONTAINER (GTK_DIALOG(quitDialog)->vbox), config.vbsm.menu_widget);

        gtk_widget_show_all (quitDialog);
}


void set_video_backend_ok(GtkWidget *widget, gpointer data) {
        GtkWidget *quitDialog = data;
	int i;
	char selected[1024];
        sprintf(&selected[0], "%s", gtk_combo_box_get_active_text(GTK_COMBO_BOX(config.vbsm.menu_widget)));
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

        quitDialog = gtk_dialog_new_with_buttons (VBS_MENU_VIDEO_OUTPUT_TITLE, GTK_WINDOW(config.vbsm.window), GTK_DIALOG_MODAL, NULL);

	if (show_menu_output) {
		GtkWidget *buttonOK = gtk_dialog_add_button (GTK_DIALOG(quitDialog), GTK_STOCK_OK, GTK_RESPONSE_OK);
		gtk_dialog_set_default_response (GTK_DIALOG (quitDialog), GTK_RESPONSE_OK); 
	        g_signal_connect (G_OBJECT(buttonOK), "clicked", G_CALLBACK (set_video_output_ok), (gpointer) quitDialog);
	}
        GtkWidget *buttonCancel = gtk_dialog_add_button (GTK_DIALOG(quitDialog), GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL);
        g_signal_connect (G_OBJECT(buttonCancel), "clicked", G_CALLBACK (quitDialogCancel), (gpointer) quitDialog);

	if (show_menu_output)
	        sprintf(quitMessage, "%s\n", VBS_MENU_VIDEO_OUTPUT_TEXT);
	else
		sprintf(quitMessage, "%s\n", VBS_MENU_VIDEO_OUTPUT_NOENT);
        quitLabel = gtk_label_new(quitMessage);
        gtk_container_add (GTK_CONTAINER (GTK_DIALOG(quitDialog)->vbox), quitLabel);

	if (show_menu_output) {
	        config.vbsm.menu_widget = gtk_combo_box_new_text();

        	int n_video_outputs = sizeof (video_outputs) / sizeof (video_outputs[0]);
	        for (i=0; i<n_video_outputs; i++) {
        	        gtk_combo_box_append_text (GTK_COMBO_BOX(config.vbsm.menu_widget), video_outputs[i].name);
                	if (strstr(&video_outputs[i].code[0], &config.vbsm.gstreamer_video_sink[0]))
                        	gtk_combo_box_set_active(GTK_COMBO_BOX(config.vbsm.menu_widget), i);
	        }
        	gtk_container_add (GTK_CONTAINER (GTK_DIALOG(quitDialog)->vbox), config.vbsm.menu_widget);
	}

        gtk_widget_show_all (quitDialog);
}


void set_video_output_ok(GtkWidget *widget, gpointer data) {
        GtkWidget *quitDialog = data;
        int i;
        char selected[1024];
        sprintf(&selected[0], "%s", gtk_combo_box_get_active_text(GTK_COMBO_BOX(config.vbsm.menu_widget)));
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
        gtk_container_add (GTK_CONTAINER (GTK_DIALOG(quitDialog)->vbox), quitLabel);

        config.vbsm.menu_widget = gtk_entry_new();
        char tmp1[16];
        sprintf(&tmp1[0], "%u", config.common.magic_key);
        gtk_entry_set_text(GTK_ENTRY(config.vbsm.menu_widget), &tmp1[0]);
        gtk_container_add (GTK_CONTAINER (GTK_DIALOG(quitDialog)->vbox), config.vbsm.menu_widget);

        gtk_widget_show_all (quitDialog);
}

