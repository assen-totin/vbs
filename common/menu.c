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

GtkWidget *makeMenu(GtkWidget *window, GtkItemFactoryEntry *menuEntries, int nmenuEntries) {
	GtkAccelGroup *accel_group;
	GtkItemFactory *item_factory;

	accel_group = gtk_accel_group_new();
	item_factory = gtk_item_factory_new(GTK_TYPE_MENU_BAR, "<vbsMainMenu>", accel_group);

	// Last argument: callback_data,
	// gets passed to all the callback functions for all the entries with callback_action != 0

	gtk_item_factory_create_items(item_factory, nmenuEntries, menuEntries, window);

	gtk_window_add_accel_group (GTK_WINDOW (window), accel_group);

	return gtk_item_factory_get_widget (item_factory, "<vbsMainMenu>");
}


void helpAbout(GtkWidget *window) {
	GtkWidget *quitDialog, *quitLabel, *quitFrame;

	quitDialog = gtk_dialog_new_with_buttons (VBS_MENU_ABOUT_TITLE, GTK_WINDOW(window), GTK_DIALOG_MODAL, NULL);

	GtkWidget *buttonCancel = gtk_dialog_add_button (GTK_DIALOG(quitDialog), GTK_STOCK_OK, GTK_RESPONSE_CANCEL);
	gtk_dialog_set_default_response (GTK_DIALOG (quitDialog), GTK_RESPONSE_CANCEL) ;
	g_signal_connect (G_OBJECT(buttonCancel), "clicked", G_CALLBACK (quitDialogCancel), (gpointer) quitDialog);

	quitFrame = gtk_frame_new("");
	quitLabel = gtk_label_new(VBS_MENU_ABOUT_TEXT);
	gtk_container_add (GTK_CONTAINER (quitFrame), quitLabel);
	gtk_container_add (GTK_CONTAINER (GTK_DIALOG(quitDialog)->vbox), quitFrame);

	quitFrame = gtk_frame_new("");
	quitLabel = gtk_label_new(VBS_MENU_ABOUT_TEXT2);
	gtk_container_add (GTK_CONTAINER (quitFrame), quitLabel);
	gtk_container_add (GTK_CONTAINER (GTK_DIALOG(quitDialog)->vbox), quitFrame);

	gtk_widget_show_all (quitDialog);
}


void setEncodingImportOK(GtkWidget *widget, gpointer data) {
	GtkWidget *quitDialog = data;
	sprintf(&config.common.import_encoding[0], "%s", gtk_combo_box_get_active_text(GTK_COMBO_BOX(config.vbsm.menu_widget)));
	write_config();
	gtk_widget_destroy(quitDialog);
}


void setEncodingExportOK(GtkWidget *widget, gpointer data) {
	GtkWidget *quitDialog = data;
	sprintf(&config.common.export_encoding[0], "%s", gtk_combo_box_get_active_text(GTK_COMBO_BOX(config.vbsm.menu_widget)));
	write_config();
	gtk_widget_destroy(quitDialog);
}


void setEncodingImport (GtkWidget *window) {
	GtkWidget *quitDialog, *quitLabel;

	quitDialog = gtk_dialog_new_with_buttons (VBS_MENU_ENCODING_IMPORT, GTK_WINDOW(window), GTK_DIALOG_MODAL, NULL);

	GtkWidget *buttonOK = gtk_dialog_add_button (GTK_DIALOG(quitDialog), GTK_STOCK_OK, GTK_RESPONSE_OK);
	GtkWidget *buttonCancel = gtk_dialog_add_button (GTK_DIALOG(quitDialog), GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL);
	gtk_dialog_set_default_response (GTK_DIALOG (quitDialog), GTK_RESPONSE_OK) ;
	g_signal_connect (G_OBJECT(buttonCancel), "clicked", G_CALLBACK (quitDialogCancel), (gpointer) quitDialog);
	g_signal_connect (G_OBJECT(buttonOK), "clicked", G_CALLBACK (setEncodingImportOK), (gpointer) quitDialog);

	char quitMessage[1024];
	sprintf(quitMessage, "%s\n", VBS_MENU_ENCODING_TITLE);
	quitLabel = gtk_label_new(quitMessage);
	gtk_container_add (GTK_CONTAINER (GTK_DIALOG(quitDialog)->vbox), quitLabel);

	config.vbsm.menu_widget = gtk_combo_box_new_text();
	int nencEntries = sizeof (encEntries) / sizeof (encEntries[0]);
	int i;
	for (i=0; i<nencEntries; i++) {
		gtk_combo_box_append_text (GTK_COMBO_BOX(config.vbsm.menu_widget), encEntries[i].name);
		if (strstr(encEntries[i].name, config.common.import_encoding)) 
			gtk_combo_box_set_active(GTK_COMBO_BOX(config.vbsm.menu_widget), i);
	}
	gtk_container_add (GTK_CONTAINER (GTK_DIALOG(quitDialog)->vbox), config.vbsm.menu_widget);

	gtk_widget_show_all (quitDialog);
}


void setEncodingExport (GtkWidget *window) {
	GtkWidget *quitDialog, *quitLabel;

	quitDialog = gtk_dialog_new_with_buttons (VBS_MENU_ENCODING_EXPORT, GTK_WINDOW(window), GTK_DIALOG_MODAL, NULL);

	GtkWidget *buttonOK = gtk_dialog_add_button (GTK_DIALOG(quitDialog), GTK_STOCK_OK, GTK_RESPONSE_OK);
	GtkWidget *buttonCancel = gtk_dialog_add_button (GTK_DIALOG(quitDialog), GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL);
	gtk_dialog_set_default_response (GTK_DIALOG (quitDialog), GTK_RESPONSE_OK) ;
	g_signal_connect (G_OBJECT(buttonCancel), "clicked", G_CALLBACK (quitDialogCancel), (gpointer) quitDialog);
	g_signal_connect (G_OBJECT(buttonOK), "clicked", G_CALLBACK (setEncodingExportOK), (gpointer) quitDialog);

	char quitMessage[1024];
	sprintf(quitMessage, "%s\n", VBS_MENU_ENCODING_TITLE);
	quitLabel = gtk_label_new(quitMessage);
	gtk_container_add (GTK_CONTAINER (GTK_DIALOG(quitDialog)->vbox), quitLabel);

	config.vbsm.menu_widget = gtk_combo_box_new_text();
	int nencEntries = sizeof (encEntries) / sizeof (encEntries[0]);
	int i;
	for (i=0; i<nencEntries; i++) {
		gtk_combo_box_append_text (GTK_COMBO_BOX(config.vbsm.menu_widget), encEntries[i].name);
		if (strstr(encEntries[i].name, config.common.export_encoding)) 
			gtk_combo_box_set_active(GTK_COMBO_BOX(config.vbsm.menu_widget), i);
	}
	gtk_container_add (GTK_CONTAINER (GTK_DIALOG(quitDialog)->vbox), config.vbsm.menu_widget);

	gtk_widget_show_all (quitDialog);
}


void setNewlineOK(GtkWidget *widget, gpointer data) {
	GtkWidget *quitDialog = data;
	if( strstr(gtk_combo_box_get_active_text(GTK_COMBO_BOX(config.vbsm.menu_widget)),"CR/LF")) 
		config.common.export_cr = 1;
	else 
		config.common.export_cr = 0;
	write_config();
	gtk_widget_destroy(quitDialog);
}


void setNewline (GtkWidget *window) {
	GtkWidget *quitDialog, *quitLabel;

	quitDialog = gtk_dialog_new_with_buttons (VBS_MENU_NEWLINE_TITLE, GTK_WINDOW(window), GTK_DIALOG_MODAL, NULL);

	GtkWidget *buttonOK = gtk_dialog_add_button (GTK_DIALOG(quitDialog), GTK_STOCK_OK, GTK_RESPONSE_OK);
	GtkWidget *buttonCancel = gtk_dialog_add_button (GTK_DIALOG(quitDialog), GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL);
	gtk_dialog_set_default_response (GTK_DIALOG (quitDialog), GTK_RESPONSE_OK) ;
	g_signal_connect (G_OBJECT(buttonCancel), "clicked", G_CALLBACK (quitDialogCancel), (gpointer) quitDialog);
	g_signal_connect (G_OBJECT(buttonOK), "clicked", G_CALLBACK (setNewlineOK), (gpointer) quitDialog);

	char quitMessage[1024];
	sprintf(quitMessage, "%s\n", VBS_MENU_NEWLINE_TEXT);
	quitLabel = gtk_label_new(quitMessage);
	gtk_container_add (GTK_CONTAINER (GTK_DIALOG(quitDialog)->vbox), quitLabel);

	config.vbsm.menu_widget = gtk_combo_box_new_text();
	gtk_combo_box_append_text (GTK_COMBO_BOX(config.vbsm.menu_widget), "LF");
	gtk_combo_box_append_text (GTK_COMBO_BOX(config.vbsm.menu_widget), "CR/LF");
	int index;
	if(config.common.export_cr == 1) {index = 1;}
	else {index = 0;}
	gtk_combo_box_set_active(GTK_COMBO_BOX(config.vbsm.menu_widget), index);
	gtk_container_add (GTK_CONTAINER (GTK_DIALOG(quitDialog)->vbox), config.vbsm.menu_widget);

	gtk_widget_show_all (quitDialog);
}

void setNetworkPortOK(GtkWidget *widget, gpointer data) {
        GtkWidget *quitDialog = data;
        config.common.tcp_port = atoi(gtk_entry_get_text(GTK_ENTRY(config.vbsm.menu_widget)));
        write_config();
        gtk_widget_destroy(quitDialog);
}

void setNetworkPort (GtkWidget *window) {
        GtkWidget *quitDialog, *quitLabel;

        quitDialog = gtk_dialog_new_with_buttons (VBS_MENU_NETWORK_PORT_TITLE, GTK_WINDOW(window), GTK_DIALOG_MODAL, NULL);

        GtkWidget *buttonOK = gtk_dialog_add_button (GTK_DIALOG(quitDialog), GTK_STOCK_OK, GTK_RESPONSE_OK);
        GtkWidget *buttonCancel = gtk_dialog_add_button (GTK_DIALOG(quitDialog), GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL);
        gtk_dialog_set_default_response (GTK_DIALOG (quitDialog), GTK_RESPONSE_OK) ;
        g_signal_connect (G_OBJECT(buttonCancel), "clicked", G_CALLBACK (quitDialogCancel), (gpointer) quitDialog);
        g_signal_connect (G_OBJECT(buttonOK), "clicked", G_CALLBACK (setNetworkPortOK), (gpointer) quitDialog);

        char quitMessage[1024];
        sprintf(&quitMessage[0], "%s\n", VBS_MENU_NETWORK_PORT_TEXT);
        quitLabel = gtk_label_new(quitMessage);
        gtk_container_add (GTK_CONTAINER (GTK_DIALOG(quitDialog)->vbox), quitLabel);

	config.vbsm.menu_widget = gtk_entry_new();
	char tmp1[16];
	sprintf(&tmp1[0], "%u", config.common.tcp_port);
	gtk_entry_set_text(GTK_ENTRY(config.vbsm.menu_widget), &tmp1[0]);
        gtk_container_add (GTK_CONTAINER (GTK_DIALOG(quitDialog)->vbox), config.vbsm.menu_widget);

        gtk_widget_show_all (quitDialog);
}

void setNetworkServerOK(GtkWidget *widget, gpointer data) {
        GtkWidget *quitDialog = data;

	if (get_host_by_name((char *) gtk_entry_get_text(GTK_ENTRY(config.vbsm.menu_widget))) == 0) {
		gtk_label_set_text(GTK_LABEL(config.vbsm.menu_widget2), VBS_MENU_DNS_WARNING_TEXT);
        }
	else {
	        write_config();
	        gtk_widget_destroy(quitDialog);
	}
}

void setNetworkServer (GtkWidget *window) {
        GtkWidget *quitDialog, *quitLabel;

        quitDialog = gtk_dialog_new_with_buttons (VBS_MENU_NETWORK_SERVER_TITLE, GTK_WINDOW(window), GTK_DIALOG_MODAL, NULL);

        GtkWidget *buttonOK = gtk_dialog_add_button (GTK_DIALOG(quitDialog), GTK_STOCK_OK, GTK_RESPONSE_OK);
        GtkWidget *buttonCancel = gtk_dialog_add_button (GTK_DIALOG(quitDialog), GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL);
        gtk_dialog_set_default_response (GTK_DIALOG (quitDialog), GTK_RESPONSE_OK) ;
        g_signal_connect (G_OBJECT(buttonCancel), "clicked", G_CALLBACK(quitDialogCancel), (gpointer) quitDialog);
        g_signal_connect (G_OBJECT(buttonOK), "clicked", G_CALLBACK(setNetworkServerOK), (gpointer) quitDialog);

        char quitMessage[1024];
        sprintf(quitMessage, "%s\n", VBS_MENU_NETWORK_SERVER_TEXT);
        quitLabel = gtk_label_new(quitMessage);
        gtk_container_add (GTK_CONTAINER (GTK_DIALOG(quitDialog)->vbox), quitLabel);

	config.vbsm.menu_widget = gtk_entry_new();
	gtk_entry_set_text(GTK_ENTRY(config.vbsm.menu_widget), &config.common.server_name[0]);
        gtk_container_add(GTK_CONTAINER(GTK_DIALOG(quitDialog)->vbox), config.vbsm.menu_widget);

        config.vbsm.menu_widget2 = gtk_label_new(VBS_MENU_DNS_WARNING_TITLE);
        gtk_container_add(GTK_CONTAINER(GTK_DIALOG(quitDialog)->vbox), config.vbsm.menu_widget2);

        gtk_widget_show_all(quitDialog);
}

void useNetworkOK(GtkWidget *widget, gpointer data) {
        GtkWidget *quitDialog = data;
        if (strstr(gtk_combo_box_get_active_text(GTK_COMBO_BOX(config.vbsm.menu_widget)),"ON"))
		config.common.send_to_network = 1;
        else 
		config.common.send_to_network = 0;
        write_config();
        gtk_widget_destroy(quitDialog);
}


void useNetwork (GtkWidget *window) {
        GtkWidget *quitDialog, *quitLabel;

        quitDialog = gtk_dialog_new_with_buttons (VBS_MENU_ENABLE_NETWORK_TITLE, GTK_WINDOW(window), GTK_DIALOG_MODAL, NULL);

        GtkWidget *buttonOK = gtk_dialog_add_button (GTK_DIALOG(quitDialog), GTK_STOCK_OK, GTK_RESPONSE_OK);
        GtkWidget *buttonCancel = gtk_dialog_add_button (GTK_DIALOG(quitDialog), GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL);
        gtk_dialog_set_default_response (GTK_DIALOG (quitDialog), GTK_RESPONSE_OK) ;
        g_signal_connect (G_OBJECT(buttonCancel), "clicked", G_CALLBACK (quitDialogCancel), (gpointer) quitDialog);
        g_signal_connect (G_OBJECT(buttonOK), "clicked", G_CALLBACK (useNetworkOK), (gpointer) quitDialog);

        char quitMessage[1024];
        sprintf(quitMessage, "%s\n", VBS_MENU_ENABLE_NETWORK_TEXT);
        quitLabel = gtk_label_new(quitMessage);
        gtk_container_add (GTK_CONTAINER (GTK_DIALOG(quitDialog)->vbox), quitLabel);

        config.vbsm.menu_widget = gtk_combo_box_new_text();
        gtk_combo_box_append_text(GTK_COMBO_BOX(config.vbsm.menu_widget), "OFF");
        gtk_combo_box_append_text(GTK_COMBO_BOX(config.vbsm.menu_widget), "ON");
        int index;
        if(config.common.send_to_network == 0)
		index = 0;
        else 
		index = 1;
        gtk_combo_box_set_active(GTK_COMBO_BOX(config.vbsm.menu_widget), index);
        gtk_container_add (GTK_CONTAINER (GTK_DIALOG(quitDialog)->vbox), config.vbsm.menu_widget);

        gtk_widget_show_all (quitDialog);
}



