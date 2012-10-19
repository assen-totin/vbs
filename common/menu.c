// This file is a part of Voody Blue Subtitler suit.
// 
// Author: Assen Totin <assen.totin@gmail.com>
//
// Home page: http://bilbo.online.bg/~assen/vbs
//
// This software is released under GNU General Public License.
// See the LICENSE file for details or visit http://www.gnu.org/copyleft/gpl.html 
// for details.

#include "common.h"

void quitDialogCancel( GtkWidget *widget, gpointer data ){
        GtkWidget *quitDialog = data;
        gtk_widget_destroy(quitDialog);
}


void help_about() {
	GtkWidget *dialog = gtk_about_dialog_new();
	gtk_about_dialog_set_program_name(GTK_ABOUT_DIALOG(dialog), "VBS");
	gtk_about_dialog_set_version(GTK_ABOUT_DIALOG(dialog), PACKAGE_VERSION); 
	gtk_about_dialog_set_copyright(GTK_ABOUT_DIALOG(dialog), "(c) Assen Totin, 2006-2012");
	gtk_about_dialog_set_website(GTK_ABOUT_DIALOG(dialog), "http://www.zavedil.com/software-desktop-vbs/");

	gtk_about_dialog_set_license (GTK_ABOUT_DIALOG(dialog), _("This programme is distributed under GNU General Public License.\nSee the LICENSE file from the distribution or visit http://www.gnu.org/copyleft/gpl.html for details."));

	gtk_about_dialog_set_comments(GTK_ABOUT_DIALOG(dialog),
	_("If you like this program, send me a postcard from your place to:\n\nBulgaria\n1303 Sofia\n74, Otec Paisi Str.\nAssen Totin\n")
	);

	GdkPixbuf *pixbuf = gdk_pixbuf_new_from_file(VBS_ICON, NULL);
	gtk_about_dialog_set_logo(GTK_ABOUT_DIALOG(dialog), pixbuf);
	g_object_unref(pixbuf), pixbuf = NULL;

	gtk_dialog_run(GTK_DIALOG (dialog));
	gtk_widget_destroy(dialog);
}

void setEncodingImportOK(GtkWidget *widget, gpointer data) {
	GtkWidget *quitDialog = data;
	sprintf(&config.common.import_encoding[0], "%s", gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(config.vbsm.menu_widget)));
	write_config();
	gtk_widget_destroy(quitDialog);
}


void setEncodingExportOK(GtkWidget *widget, gpointer data) {
	GtkWidget *quitDialog = data;
	sprintf(&config.common.export_encoding[0], "%s", gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(config.vbsm.menu_widget)));
	write_config();
	gtk_widget_destroy(quitDialog);
}


void setEncodingImport (GtkAction *action, gpointer param) {
	GtkWidget *quitDialog, *quitLabel;

	quitDialog = gtk_dialog_new_with_buttons (_("Select Import Encoding"), GTK_WINDOW(config.vbsm.window), GTK_DIALOG_MODAL, NULL);

	GtkWidget *buttonOK = gtk_dialog_add_button (GTK_DIALOG(quitDialog), GTK_STOCK_OK, GTK_RESPONSE_OK);
	GtkWidget *buttonCancel = gtk_dialog_add_button (GTK_DIALOG(quitDialog), GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL);
	gtk_dialog_set_default_response (GTK_DIALOG (quitDialog), GTK_RESPONSE_OK) ;
	g_signal_connect (G_OBJECT(buttonCancel), "clicked", G_CALLBACK (quitDialogCancel), (gpointer) quitDialog);
	g_signal_connect (G_OBJECT(buttonOK), "clicked", G_CALLBACK (setEncodingImportOK), (gpointer) quitDialog);

	char quitMessage[1024];
	sprintf(quitMessage, "%s\n", _("Available Encodings:"));
	quitLabel = gtk_label_new(quitMessage);
	gtk_container_add (GTK_CONTAINER(gtk_dialog_get_content_area(GTK_DIALOG(quitDialog))), quitLabel);

	config.vbsm.menu_widget = gtk_combo_box_text_new();
	int nencEntries = sizeof (encEntries) / sizeof (encEntries[0]);
	int i;
	for (i=0; i<nencEntries; i++) {
		gtk_combo_box_text_append_text (GTK_COMBO_BOX_TEXT(config.vbsm.menu_widget), encEntries[i].name);
		if (strstr(encEntries[i].name, config.common.import_encoding)) 
			gtk_combo_box_set_active(GTK_COMBO_BOX(config.vbsm.menu_widget), i);
	}
	gtk_container_add (GTK_CONTAINER(gtk_dialog_get_content_area(GTK_DIALOG(quitDialog))), config.vbsm.menu_widget);

	gtk_widget_show_all (quitDialog);
}


void setEncodingExport (GtkAction *action, gpointer param) {
	GtkWidget *quitDialog, *quitLabel;

	quitDialog = gtk_dialog_new_with_buttons (_("Select Export Encoding"), GTK_WINDOW(config.vbsm.window), GTK_DIALOG_MODAL, NULL);

	GtkWidget *buttonOK = gtk_dialog_add_button (GTK_DIALOG(quitDialog), GTK_STOCK_OK, GTK_RESPONSE_OK);
	GtkWidget *buttonCancel = gtk_dialog_add_button (GTK_DIALOG(quitDialog), GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL);
	gtk_dialog_set_default_response (GTK_DIALOG (quitDialog), GTK_RESPONSE_OK) ;
	g_signal_connect (G_OBJECT(buttonCancel), "clicked", G_CALLBACK (quitDialogCancel), (gpointer) quitDialog);
	g_signal_connect (G_OBJECT(buttonOK), "clicked", G_CALLBACK (setEncodingExportOK), (gpointer) quitDialog);

	char quitMessage[1024];
	sprintf(quitMessage, "%s\n", _("Available Encodings:"));
	quitLabel = gtk_label_new(quitMessage);
	gtk_container_add(GTK_CONTAINER(gtk_dialog_get_content_area(GTK_DIALOG(quitDialog))), quitLabel);

	config.vbsm.menu_widget = gtk_combo_box_text_new();
	int nencEntries = sizeof (encEntries) / sizeof (encEntries[0]);
	int i;
	for (i=0; i<nencEntries; i++) {
		gtk_combo_box_text_append_text (GTK_COMBO_BOX_TEXT(config.vbsm.menu_widget), encEntries[i].name);
		if (strstr(encEntries[i].name, config.common.export_encoding)) 
			gtk_combo_box_set_active(GTK_COMBO_BOX(config.vbsm.menu_widget), i);
	}
	gtk_container_add(GTK_CONTAINER(gtk_dialog_get_content_area(GTK_DIALOG(quitDialog))), config.vbsm.menu_widget);

	gtk_widget_show_all (quitDialog);
}


void setNewlineOK(GtkWidget *widget, gpointer data) {
	GtkWidget *quitDialog = data;
	if( strstr(gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(config.vbsm.menu_widget)),"CR/LF")) 
		config.common.export_cr = 1;
	else 
		config.common.export_cr = 0;
	write_config();
	gtk_widget_destroy(quitDialog);
}


void setNewline (GtkAction *action, gpointer param) {
	GtkWidget *quitDialog, *quitLabel;

	quitDialog = gtk_dialog_new_with_buttons (_("Select Newline Delimiter"), GTK_WINDOW(config.vbsm.window), GTK_DIALOG_MODAL, NULL);

	GtkWidget *buttonOK = gtk_dialog_add_button (GTK_DIALOG(quitDialog), GTK_STOCK_OK, GTK_RESPONSE_OK);
	GtkWidget *buttonCancel = gtk_dialog_add_button (GTK_DIALOG(quitDialog), GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL);
	gtk_dialog_set_default_response (GTK_DIALOG (quitDialog), GTK_RESPONSE_OK) ;
	g_signal_connect (G_OBJECT(buttonCancel), "clicked", G_CALLBACK (quitDialogCancel), (gpointer) quitDialog);
	g_signal_connect (G_OBJECT(buttonOK), "clicked", G_CALLBACK (setNewlineOK), (gpointer) quitDialog);

	char quitMessage[1024];
	sprintf(quitMessage, "%s\n", _("Available Delimiters:"));
	quitLabel = gtk_label_new(quitMessage);
	gtk_container_add(GTK_CONTAINER(gtk_dialog_get_content_area(GTK_DIALOG(quitDialog))), quitLabel);

	config.vbsm.menu_widget = gtk_combo_box_text_new();
	gtk_combo_box_text_append_text (GTK_COMBO_BOX_TEXT(config.vbsm.menu_widget), "LF");
	gtk_combo_box_text_append_text (GTK_COMBO_BOX_TEXT(config.vbsm.menu_widget), "CR/LF");
	int index;
	if(config.common.export_cr == 1) {index = 1;}
	else {index = 0;}
	gtk_combo_box_set_active(GTK_COMBO_BOX(config.vbsm.menu_widget), index);
	gtk_container_add(GTK_CONTAINER(gtk_dialog_get_content_area(GTK_DIALOG(quitDialog))), config.vbsm.menu_widget);

	gtk_widget_show_all (quitDialog);
}

void setNetworkPortOK(GtkWidget *widget, gpointer data) {
        GtkWidget *quitDialog = data;
        config.common.tcp_port = atoi(gtk_entry_get_text(GTK_ENTRY(config.vbsm.menu_widget)));
        write_config();
        gtk_widget_destroy(quitDialog);
}

void setNetworkPort (GtkAction *action, gpointer param) {
        GtkWidget *quitDialog, *quitLabel;

        quitDialog = gtk_dialog_new_with_buttons (_("TCP Port"), GTK_WINDOW(config.vbsm.window), GTK_DIALOG_MODAL, NULL);

        GtkWidget *buttonOK = gtk_dialog_add_button (GTK_DIALOG(quitDialog), GTK_STOCK_OK, GTK_RESPONSE_OK);
        GtkWidget *buttonCancel = gtk_dialog_add_button (GTK_DIALOG(quitDialog), GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL);
        gtk_dialog_set_default_response (GTK_DIALOG (quitDialog), GTK_RESPONSE_OK) ;
        g_signal_connect (G_OBJECT(buttonCancel), "clicked", G_CALLBACK (quitDialogCancel), (gpointer) quitDialog);
        g_signal_connect (G_OBJECT(buttonOK), "clicked", G_CALLBACK (setNetworkPortOK), (gpointer) quitDialog);

        char quitMessage[1024];
        sprintf(&quitMessage[0], "%s\n", _("Current Port:"));
        quitLabel = gtk_label_new(quitMessage);
        gtk_container_add(GTK_CONTAINER(gtk_dialog_get_content_area(GTK_DIALOG(quitDialog))), quitLabel);

	config.vbsm.menu_widget = gtk_entry_new();
	char tmp1[16];
	sprintf(&tmp1[0], "%u", config.common.tcp_port);
	gtk_entry_set_text(GTK_ENTRY(config.vbsm.menu_widget), &tmp1[0]);
        gtk_container_add(GTK_CONTAINER(gtk_dialog_get_content_area(GTK_DIALOG(quitDialog))), config.vbsm.menu_widget);

        gtk_widget_show_all (quitDialog);
}

void setNetworkServerOK(GtkWidget *widget, gpointer data) {
        GtkWidget *quitDialog = data;

	if (get_host_by_name((char *) gtk_entry_get_text(GTK_ENTRY(config.vbsm.menu_widget))) == 0) {
		gtk_label_set_text(GTK_LABEL(config.vbsm.menu_widget2), _("DNS resolving failed."));
        }
	else {
	        write_config();
	        gtk_widget_destroy(quitDialog);
	}
}

void setNetworkServer (GtkAction *action, gpointer param) {
        GtkWidget *quitDialog, *quitLabel;

        quitDialog = gtk_dialog_new_with_buttons (_("Server Name"), GTK_WINDOW(config.vbsm.window), GTK_DIALOG_MODAL, NULL);

        GtkWidget *buttonOK = gtk_dialog_add_button (GTK_DIALOG(quitDialog), GTK_STOCK_OK, GTK_RESPONSE_OK);
        GtkWidget *buttonCancel = gtk_dialog_add_button (GTK_DIALOG(quitDialog), GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL);
        gtk_dialog_set_default_response (GTK_DIALOG (quitDialog), GTK_RESPONSE_OK) ;
        g_signal_connect (G_OBJECT(buttonCancel), "clicked", G_CALLBACK(quitDialogCancel), (gpointer) quitDialog);
        g_signal_connect (G_OBJECT(buttonOK), "clicked", G_CALLBACK(setNetworkServerOK), (gpointer) quitDialog);

        char quitMessage[1024];
        sprintf(quitMessage, "%s\n", _("Current server (FQDN):"));
        quitLabel = gtk_label_new(quitMessage);
        gtk_container_add(GTK_CONTAINER(gtk_dialog_get_content_area(GTK_DIALOG(quitDialog))), quitLabel);

	config.vbsm.menu_widget = gtk_entry_new();
	gtk_entry_set_text(GTK_ENTRY(config.vbsm.menu_widget), &config.common.server_name[0]);
        gtk_container_add(GTK_CONTAINER(gtk_dialog_get_content_area(GTK_DIALOG(quitDialog))), config.vbsm.menu_widget);

        config.vbsm.menu_widget2 = gtk_label_new(_("Press OK to change server."));
        gtk_container_add(GTK_CONTAINER(gtk_dialog_get_content_area(GTK_DIALOG(quitDialog))), config.vbsm.menu_widget2);

        gtk_widget_show_all(quitDialog);
}

void useNetworkOK(GtkWidget *widget, gpointer data) {
        GtkWidget *quitDialog = data;
        if (strstr(gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(config.vbsm.menu_widget)),"OFF"))
		config.common.network_mode = 0;
        else if (strstr(gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(config.vbsm.menu_widget)),"SEND"))
		config.common.network_mode = 1;
	else
		config.common.network_mode = 2;
        write_config();
        gtk_widget_destroy(quitDialog);
}


void useNetwork (GtkAction *action, gpointer param) {
        GtkWidget *quitDialog, *quitLabel;

        quitDialog = gtk_dialog_new_with_buttons (_("Network Mode"), GTK_WINDOW(config.vbsm.window), GTK_DIALOG_MODAL, NULL);

        GtkWidget *buttonOK = gtk_dialog_add_button (GTK_DIALOG(quitDialog), GTK_STOCK_OK, GTK_RESPONSE_OK);
        GtkWidget *buttonCancel = gtk_dialog_add_button (GTK_DIALOG(quitDialog), GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL);
        gtk_dialog_set_default_response (GTK_DIALOG (quitDialog), GTK_RESPONSE_OK) ;
        g_signal_connect (G_OBJECT(buttonCancel), "clicked", G_CALLBACK (quitDialogCancel), (gpointer) quitDialog);
        g_signal_connect (G_OBJECT(buttonOK), "clicked", G_CALLBACK (useNetworkOK), (gpointer) quitDialog);

        char quitMessage[1024];
        sprintf(quitMessage, "%s\n", _("Select network mode:"));
        quitLabel = gtk_label_new(quitMessage);
        gtk_container_add(GTK_CONTAINER(gtk_dialog_get_content_area(GTK_DIALOG(quitDialog))), quitLabel);

        config.vbsm.menu_widget = gtk_combo_box_text_new();
	sprintf(quitMessage, "%s %s", "OFF:", _("Network is off"));
        gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(config.vbsm.menu_widget), &quitMessage[0]);
	sprintf(quitMessage, "%s %s", "SEND:", _("Send to network"));
        gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(config.vbsm.menu_widget), &quitMessage[0]);

	if (can_recv_from_net == 1) {
		sprintf(quitMessage, "%s %s", "RECV:", _("Receive from network"));
		gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(config.vbsm.menu_widget), &quitMessage[0]);
	}

        gtk_combo_box_set_active(GTK_COMBO_BOX(config.vbsm.menu_widget), config.common.network_mode);

        gtk_container_add(GTK_CONTAINER(gtk_dialog_get_content_area(GTK_DIALOG(quitDialog))), config.vbsm.menu_widget);

        gtk_widget_show_all (quitDialog);
}

