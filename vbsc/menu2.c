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

void file_dialog_ok_21( GtkWidget *fileDialogWidget, GtkFileSelection *fs ) {
	if (strlen(gtk_file_selection_get_filename (GTK_FILE_SELECTION (fs))) > 512) {error_handler("file_dialog_ok_21","Filename too long.", 1);}
	sprintf(&config.common.import_filename[0], "%s", gtk_file_selection_get_filename (GTK_FILE_SELECTION (fs)));
}


void file_dialog_ok_41( GtkWidget *fileDialogWidget, GtkFileSelection *fs ) {
	// Stupid, but global variables need care.
	if (strlen(gtk_file_selection_get_filename (GTK_FILE_SELECTION (fs))) > 512) {error_handler("file_dialog_ok_41","Filename too long.",1);}
	sprintf(&config.common.export_filename[0], "%s", gtk_file_selection_get_filename (GTK_FILE_SELECTION (fs)));
}


static void quit_dialog_ok( GtkWidget *widget, gpointer data ){
	GtkWidget *quitDialog = data;
	gtk_widget_destroy(quitDialog);
	gtk_main_quit();
}

void quit_dialog(GtkWidget *window) {
	GtkWidget *quitDialog, *quitLabel;
	char quitMessage[1024];

	sprintf(quitMessage, "%s %s\n", VBS_MENU_QUIT_TEXT, &config.common.export_filename[0]); 

	quitDialog = gtk_dialog_new_with_buttons (VBS_MENU_QUIT_TITLE, GTK_WINDOW(window), GTK_DIALOG_MODAL, NULL);

	GtkWidget *buttonOK = gtk_dialog_add_button (GTK_DIALOG(quitDialog), GTK_STOCK_OK, GTK_RESPONSE_OK);
	GtkWidget *buttonCancel = gtk_dialog_add_button (GTK_DIALOG(quitDialog), GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL);
	gtk_dialog_set_default_response (GTK_DIALOG (quitDialog), GTK_RESPONSE_CANCEL) ;

	quitLabel = gtk_label_new(quitMessage);

	gtk_container_add (GTK_CONTAINER (GTK_DIALOG(quitDialog)->vbox), quitLabel);

	g_signal_connect (G_OBJECT(buttonOK), "clicked", G_CALLBACK (quit_dialog_ok), (gpointer) quitDialog);

	g_signal_connect (G_OBJECT(buttonCancel), "clicked", G_CALLBACK (quitDialogCancel), (gpointer) quitDialog);

	gtk_widget_show_all (quitDialog);
}


void file_dialog(gpointer callback_data, guint callback_action, GtkWidget *window) {
	GtkWidget *fileDialogWidget;
	char fileDialogTitle[64];
	char fileDialogFile[256];

	switch (callback_action) {
		case 21:
			// Import Text-only
			sprintf(fileDialogTitle, "%s", VBS_MENU_IMPORT_TEXTONLY_TITLE);
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
			g_signal_connect (G_OBJECT (GTK_FILE_SELECTION (fileDialogWidget)->ok_button), "clicked", G_CALLBACK (file_dialog_ok_21), (gpointer) fileDialogWidget);
			break;
		case 41:
			// Export Dialog
			g_signal_connect (G_OBJECT (GTK_FILE_SELECTION (fileDialogWidget)->ok_button), "clicked", G_CALLBACK (file_dialog_ok_41), (gpointer) fileDialogWidget);
			break;
	}
	g_signal_connect_swapped (G_OBJECT (GTK_FILE_SELECTION (fileDialogWidget)->ok_button), "clicked", G_CALLBACK (gtk_widget_destroy), G_OBJECT (fileDialogWidget));

	/* Connect the cancel_button to destroy the widget */
	g_signal_connect_swapped (G_OBJECT (GTK_FILE_SELECTION (fileDialogWidget)->cancel_button), "clicked", G_CALLBACK (gtk_widget_destroy), G_OBJECT (fileDialogWidget));

	/* Lets set the filename, as if this were a save dialog, and we are giving a default filename */
	gtk_file_selection_set_filename (GTK_FILE_SELECTION(fileDialogWidget), fileDialogFile);

	gtk_widget_show (fileDialogWidget);
}

