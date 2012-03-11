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

void zeroTiming(gpointer callback_data, guint callback_action, GtkWidget *window){
	GtkTreeIter iter, sibling;
	GtkTreeSelection *selection;
	GtkTreeModel     *model;
	bool flag = TRUE;

	selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(view));

	switch (callback_action) {
		case 64:
			// Zero current
			if (gtk_tree_selection_get_selected(selection, &model, &iter)) {
				gtk_list_store_set (store, &iter, COL_FROM, 0, COL_TO, 0, -1);
			}
			break;

		case 65:
			// Zero from current
			if (gtk_tree_selection_get_selected(selection, &model, &iter)) {
				sibling = iter;
				while (flag) {
					// Move to next line
					gtk_tree_selection_unselect_iter(selection, &iter);
					flag = gtk_tree_model_iter_next(model, &iter);
					if (flag) {
						gtk_tree_selection_select_iter(selection, &iter);
						gtk_list_store_set (store, &iter, COL_FROM, 0, COL_TO, 0, -1);
					}
				}
				gtk_tree_selection_select_iter(selection, &sibling);
			}
			break;
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
	char videoFile[1024];
	if (strlen(gtk_file_selection_get_filename (GTK_FILE_SELECTION (fs))) > 512) {error_handler("fileDialogOK31","Filename too long.", 1);}
	sprintf(videoFile, "%s", gtk_file_selection_get_filename (GTK_FILE_SELECTION (fs)));
	loadVideo(videoFile);
}


void fileDialogOK41( GtkWidget *fileDialogWidget, GtkFileSelection *fs ) {
	// Stupid, but global variables need care.
	if (strlen(gtk_file_selection_get_filename (GTK_FILE_SELECTION (fs))) > 512) {error_handler("fileDialogOK41","Filename too long.",1);}
	sprintf(&config.common.export_filename[0], "%s", gtk_file_selection_get_filename (GTK_FILE_SELECTION (fs)));
}


void insertBefore(gpointer callback_data, guint callback_action, GtkWidget *window){
	GtkTreeIter iter, sibling;
	GtkTreeSelection *selection;
	GtkTreeModel     *model;

	selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(view));

	switch (callback_action) {
		case 61:
			// New before current
			if (gtk_tree_selection_get_selected(selection, &model, &sibling)) {
				gtk_list_store_insert_before (store, &iter, &sibling);
				gtk_list_store_set (store, &iter, COL_LINE, " ", COL_FROM, 0, COL_TO, 0, -1);
			}
			break;
		case 62:
			// New after current
			if (gtk_tree_selection_get_selected(selection, &model, &sibling)) {
				gtk_list_store_insert_after (store, &iter, &sibling);
				gtk_list_store_set (store, &iter, COL_LINE, " ", COL_FROM, 0, COL_TO, 0, -1);
			}
			break;
		case 63:
			// Delete current
			if (gtk_tree_selection_get_selected(selection, &model, &sibling)) {
				gtk_list_store_remove (store, &sibling);
			}
			break;
	}
}


void helpContents(GtkWidget *window) {
	GtkWidget *quitDialog, *quitLabel, *quitFrame;

	quitDialog = gtk_dialog_new_with_buttons (VBS_MENU_HELP_TITLE, GTK_WINDOW(window), GTK_DIALOG_MODAL, NULL);

	GtkWidget *buttonCancel = gtk_dialog_add_button (GTK_DIALOG(quitDialog), GTK_STOCK_OK, GTK_RESPONSE_CANCEL);
	gtk_dialog_set_default_response (GTK_DIALOG (quitDialog), GTK_RESPONSE_CANCEL) ;
	g_signal_connect (G_OBJECT(buttonCancel), "clicked", G_CALLBACK (quitDialogCancel), (gpointer) quitDialog);

	quitFrame = gtk_frame_new("");
	quitLabel = gtk_label_new(VBS_MENU_HELP_TEXT);
	gtk_container_add (GTK_CONTAINER (quitFrame), quitLabel);
	gtk_container_add (GTK_CONTAINER (GTK_DIALOG(quitDialog)->vbox), quitFrame);

	gtk_widget_show_all (quitDialog);
}


void setTimer(GtkWidget *window) {
	config.common.init_timestamp = time(NULL);

        GtkWidget *quitDialog, *quitLabel, *quitFrame;

        quitDialog = gtk_dialog_new_with_buttons (VBS_MENU_TIMER_TITLE, GTK_WINDOW(window), GTK_DIALOG_MODAL, NULL);

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

	if (mplayerAlive()) {
		writeMPlayer("quit");
		int status;
		waitpid(-1, &status, 0);
	}

	fclose(config.vbsm.logFile);
	unlink(config.vbsm.logFileName);

        unlink(config.vbsm.mplayerSubFileName);

	gtk_main_quit();
}

void quitDialog(GtkWidget *window) {
	GtkWidget *quitDialog, *quitLabel;
	char quitMessage[1024];

	sprintf(quitMessage, "%s %s\n", VBS_MENU_QUIT_TEXT, &config.common.export_filename[0]); 

	quitDialog = gtk_dialog_new_with_buttons (VBS_MENU_QUIT_TITLE, GTK_WINDOW(window), GTK_DIALOG_MODAL, NULL);

	GtkWidget *buttonOK = gtk_dialog_add_button (GTK_DIALOG(quitDialog), GTK_STOCK_OK, GTK_RESPONSE_OK);
	GtkWidget *buttonCancel = gtk_dialog_add_button (GTK_DIALOG(quitDialog), GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL);
	gtk_dialog_set_default_response (GTK_DIALOG (quitDialog), GTK_RESPONSE_CANCEL) ;

	quitLabel = gtk_label_new(quitMessage);

	gtk_container_add (GTK_CONTAINER (GTK_DIALOG(quitDialog)->vbox), quitLabel);

	g_signal_connect (G_OBJECT(buttonOK), "clicked", G_CALLBACK (quitDialogOK), (gpointer) quitDialog);

	g_signal_connect (G_OBJECT(buttonCancel), "clicked", G_CALLBACK (quitDialogCancel), (gpointer) quitDialog);

	gtk_widget_show_all (quitDialog);
}


void fileDialog(gpointer callback_data, guint callback_action, GtkWidget *window) {
	GtkWidget *fileDialogWidget;
	char fileDialogTitle[64];
	char fileDialogFile[256];

	switch (callback_action) {
		case 21:
			// Import Text-only
			sprintf(fileDialogTitle, "%s", VBS_MENU_IMPORT_TEXTONLY_TITLE);
			sprintf(fileDialogFile, "%s/", VBS_MENU_DEFAULT_PATH);
			break;
		case 22:
			// Import SRT
			sprintf(fileDialogTitle, "%s", VBS_MENU_IMPORT_SRT_TITLE);
			sprintf(fileDialogFile, "%s/", VBS_MENU_DEFAULT_PATH);
			break;
		case 31:
			// Load Video
			sprintf(fileDialogTitle, "%s", VBS_MENU_IMPORT_VIDEO_TITLE);
			sprintf(fileDialogFile, "%s/", VBS_MENU_DEFAULT_PATH);
			break;
		case 41:
			// Export Dialog
			sprintf(fileDialogTitle, "%s", VBS_MENU_EXPORT_TITLE);
			sprintf(fileDialogFile, "%s/", VBS_MENU_DEFAULT_PATH);
			break;
	}

	fileDialogWidget = gtk_file_selection_new (fileDialogTitle);

	/* Connect the ok_button to file_ok_sel function */
	switch (callback_action) {
		case 21:
			// Import Text-only
			g_signal_connect (G_OBJECT (GTK_FILE_SELECTION (fileDialogWidget)->ok_button), "clicked", G_CALLBACK (fileDialogOK21), (gpointer) fileDialogWidget);
			break;
		case 22:
			// Import SRT
			g_signal_connect (G_OBJECT (GTK_FILE_SELECTION (fileDialogWidget)->ok_button), "clicked", G_CALLBACK (fileDialogOK22), (gpointer) fileDialogWidget);
			break;
		case 31:
			// Load Video
			g_signal_connect (G_OBJECT (GTK_FILE_SELECTION (fileDialogWidget)->ok_button), "clicked", G_CALLBACK (fileDialogOK31), (gpointer) fileDialogWidget);
			break;
		case 41:
			// Export Dialog
			g_signal_connect (G_OBJECT (GTK_FILE_SELECTION (fileDialogWidget)->ok_button), "clicked", G_CALLBACK (fileDialogOK41), (gpointer) fileDialogWidget);
			break;
	}
	g_signal_connect_swapped (G_OBJECT (GTK_FILE_SELECTION (fileDialogWidget)->ok_button), "clicked", G_CALLBACK (gtk_widget_destroy), G_OBJECT (fileDialogWidget));

	/* Connect the cancel_button to destroy the widget */
	g_signal_connect_swapped (G_OBJECT (GTK_FILE_SELECTION (fileDialogWidget)->cancel_button), "clicked", G_CALLBACK (gtk_widget_destroy), G_OBJECT (fileDialogWidget));

	/* Lets set the filename, as if this were a save dialog, and we are giving a default filename */
	gtk_file_selection_set_filename (GTK_FILE_SELECTION(fileDialogWidget), fileDialogFile);

	gtk_widget_show (fileDialogWidget);
}

