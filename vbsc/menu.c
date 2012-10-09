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
	write_config();
}


void file_dialog_ok_41( GtkWidget *fileDialogWidget, GtkFileSelection *fs ) {
	if (strlen(gtk_file_selection_get_filename (GTK_FILE_SELECTION (fs))) > 512) {error_handler("file_dialog_ok_41","Filename too long.",1);}
	sprintf(&config.common.export_filename[0], "%s", gtk_file_selection_get_filename (GTK_FILE_SELECTION (fs)));
	write_config();
}

static void quit_dialog_ok( GtkWidget *widget, gpointer data ){
	GtkWidget *quitDialog = data;
	gtk_widget_destroy(quitDialog);
	gtk_main_quit();
}

void quit_dialog(GtkAction *action, gpointer param) {
	GtkWidget *quitDialog, *quitLabel;

	quitDialog = gtk_dialog_new_with_buttons (VBS_MENU_QUIT_TITLE, GTK_WINDOW(config.vbsm.window), GTK_DIALOG_MODAL, NULL);

	GtkWidget *buttonOK = gtk_dialog_add_button (GTK_DIALOG(quitDialog), GTK_STOCK_OK, GTK_RESPONSE_OK);
	GtkWidget *buttonCancel = gtk_dialog_add_button (GTK_DIALOG(quitDialog), GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL);
	gtk_dialog_set_default_response (GTK_DIALOG (quitDialog), GTK_RESPONSE_CANCEL) ;

	quitLabel = gtk_label_new(VBS_MENU_QUIT_TITLE);

	gtk_container_add (GTK_CONTAINER (GTK_DIALOG(quitDialog)->vbox), quitLabel);

	g_signal_connect (G_OBJECT(buttonOK), "clicked", G_CALLBACK (quit_dialog_ok), (gpointer) quitDialog);

	g_signal_connect (G_OBJECT(buttonCancel), "clicked", G_CALLBACK (quitDialogCancel), (gpointer) quitDialog);

	gtk_widget_show_all (quitDialog);
}


void file_dialog(GtkAction *action, gpointer param) {
	GtkWidget *fileDialogWidget;
	char fileDialogTitle[64];
	char fileDialogFile[256];

	if (strstr(gtk_action_get_name(action), "EditImportDefault")) {
		// Import Text-only
		sprintf(fileDialogTitle, "%s", VBS_MENU_IMPORT_TEXTONLY_TITLE);
		sprintf(fileDialogFile, "%s/", VBS_MENU_DEFAULT_PATH);
	}
	else if (strstr(gtk_action_get_name(action), "EditExportDefault")) {
		// Export Dialog
		sprintf(fileDialogTitle, "%s", VBS_MENU_EXPORT_TITLE);
		sprintf(fileDialogFile, "%s/", VBS_MENU_DEFAULT_PATH);
	}

	fileDialogWidget = gtk_file_selection_new (fileDialogTitle);

	/* Connect the ok_button to file_ok_sel function */
	if (strstr(gtk_action_get_name(action), "EditImportDefault")) 
		// Import Text-only
		g_signal_connect (G_OBJECT (GTK_FILE_SELECTION (fileDialogWidget)->ok_button), "clicked", G_CALLBACK (file_dialog_ok_21), (gpointer) fileDialogWidget);
	else if (strstr(gtk_action_get_name(action), "EditExportDefault"))
		// Export Dialog
		g_signal_connect (G_OBJECT (GTK_FILE_SELECTION (fileDialogWidget)->ok_button), "clicked", G_CALLBACK (file_dialog_ok_41), (gpointer) fileDialogWidget);

	g_signal_connect_swapped (G_OBJECT (GTK_FILE_SELECTION (fileDialogWidget)->ok_button), "clicked", G_CALLBACK (gtk_widget_destroy), G_OBJECT (fileDialogWidget));

	/* Connect the cancel_button to destroy the widget */
	g_signal_connect_swapped (G_OBJECT (GTK_FILE_SELECTION (fileDialogWidget)->cancel_button), "clicked", G_CALLBACK (gtk_widget_destroy), G_OBJECT (fileDialogWidget));

	/* Lets set the filename, as if this were a save dialog, and we are giving a default filename */
	gtk_file_selection_set_filename (GTK_FILE_SELECTION(fileDialogWidget), fileDialogFile);

	gtk_widget_show (fileDialogWidget);
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

void set_font_size_ok(GtkWidget *widget, gpointer data) {
        GtkWidget *quitDialog = data;
        config.vbss.font_size = atoi(gtk_entry_get_text(GTK_ENTRY(config.vbsm.menu_widget)));
        write_config();
        gtk_widget_destroy(quitDialog);
}

void set_font_size (GtkAction *action, gpointer param) {
        GtkWidget *quitDialog, *quitLabel;

        quitDialog = gtk_dialog_new_with_buttons (VBSC_MENU_FONT_SIZE_TITLE, GTK_WINDOW(config.vbsm.window), GTK_DIALOG_MODAL, NULL);

        GtkWidget *buttonOK = gtk_dialog_add_button (GTK_DIALOG(quitDialog), GTK_STOCK_OK, GTK_RESPONSE_OK);
        GtkWidget *buttonCancel = gtk_dialog_add_button (GTK_DIALOG(quitDialog), GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL);
        gtk_dialog_set_default_response (GTK_DIALOG (quitDialog), GTK_RESPONSE_OK) ;
        g_signal_connect (G_OBJECT(buttonCancel), "clicked", G_CALLBACK (quitDialogCancel), (gpointer) quitDialog);
        g_signal_connect (G_OBJECT(buttonOK), "clicked", G_CALLBACK (set_font_size_ok), (gpointer) quitDialog);

        char quitMessage[1024];
        sprintf(&quitMessage[0], "%s\n", VBSC_MENU_FONT_SIZE_TEXT);
        quitLabel = gtk_label_new(quitMessage);
        gtk_container_add (GTK_CONTAINER (GTK_DIALOG(quitDialog)->vbox), quitLabel);

        config.vbsm.menu_widget = gtk_entry_new();
        char tmp1[16];
        sprintf(&tmp1[0], "%u", config.vbss.font_size);
        gtk_entry_set_text(GTK_ENTRY(config.vbsm.menu_widget), &tmp1[0]);
        gtk_container_add (GTK_CONTAINER (GTK_DIALOG(quitDialog)->vbox), config.vbsm.menu_widget);

        gtk_widget_show_all (quitDialog);
}

void set_full_screen_ok(GtkWidget *widget, gpointer data) {
        GtkWidget *quitDialog = data;
        if (strstr(gtk_combo_box_get_active_text(GTK_COMBO_BOX(config.vbsm.menu_widget)),"ON"))
                config.vbss.full_screen = 1;
        else
                config.vbss.full_screen = 0;
        write_config();
        gtk_widget_destroy(quitDialog);
}


void set_full_screen (GtkAction *action, gpointer param) {
        GtkWidget *quitDialog, *quitLabel;

        quitDialog = gtk_dialog_new_with_buttons (VBSC_MENU_FULL_SCREEN_TITLE, GTK_WINDOW(config.vbsm.window), GTK_DIALOG_MODAL, NULL);

        GtkWidget *buttonOK = gtk_dialog_add_button (GTK_DIALOG(quitDialog), GTK_STOCK_OK, GTK_RESPONSE_OK);
        GtkWidget *buttonCancel = gtk_dialog_add_button (GTK_DIALOG(quitDialog), GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL);
        gtk_dialog_set_default_response (GTK_DIALOG (quitDialog), GTK_RESPONSE_OK) ;
        g_signal_connect (G_OBJECT(buttonCancel), "clicked", G_CALLBACK (quitDialogCancel), (gpointer) quitDialog);
        g_signal_connect (G_OBJECT(buttonOK), "clicked", G_CALLBACK (set_full_screen_ok), (gpointer) quitDialog);

        char quitMessage[1024];
        sprintf(quitMessage, "%s\n", VBSC_MENU_FULL_SCREEN_TEXT);
        quitLabel = gtk_label_new(quitMessage);
        gtk_container_add (GTK_CONTAINER (GTK_DIALOG(quitDialog)->vbox), quitLabel);

        config.vbsm.menu_widget = gtk_combo_box_new_text();
        gtk_combo_box_append_text(GTK_COMBO_BOX(config.vbsm.menu_widget), "OFF");
        gtk_combo_box_append_text(GTK_COMBO_BOX(config.vbsm.menu_widget), "ON");
        int index;
        if(config.vbss.full_screen == 0)
                index = 0;
        else
                index = 1;
        gtk_combo_box_set_active(GTK_COMBO_BOX(config.vbsm.menu_widget), index);
        gtk_container_add (GTK_CONTAINER (GTK_DIALOG(quitDialog)->vbox), config.vbsm.menu_widget);

        gtk_widget_show_all (quitDialog);
}

