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

void help_about(GtkWidget *widget, gpointer window) {
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


void set_encoding_import (GtkWidget *widget, gpointer window) {
	GtkWidget *dialog = gtk_message_dialog_new(GTK_WINDOW(config.vbsm.window),
		GTK_DIALOG_DESTROY_WITH_PARENT | GTK_DIALOG_MODAL,
		GTK_MESSAGE_QUESTION,
		GTK_BUTTONS_OK_CANCEL,
		_("Available Encodings:"));
	gtk_window_set_title(GTK_WINDOW(dialog), _("Select Import Encoding"));

	GtkWidget *combo = gtk_combo_box_text_new();
        int n_enc_entries = sizeof (enc_entries) / sizeof (enc_entries[0]);
	int i;
        for (i=0; i<n_enc_entries; i++) {
                gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(combo), enc_entries[i].name);
                if (strstr(enc_entries[i].name, config.common.import_encoding)) 
                        gtk_combo_box_set_active(GTK_COMBO_BOX(combo), i);
        }

	gtk_container_add(GTK_CONTAINER(gtk_message_dialog_get_message_area(GTK_MESSAGE_DIALOG(dialog))), combo);
	gtk_widget_show(combo);

	if (gtk_dialog_run(GTK_DIALOG(dialog)) == GTK_RESPONSE_OK) {
		sprintf(&config.common.import_encoding[0], "%s", gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(combo)));
		write_config();
	}

	gtk_widget_destroy(dialog);
}


void set_encoding_export (GtkWidget *widget, gpointer window) {
        GtkWidget *dialog = gtk_message_dialog_new(GTK_WINDOW(config.vbsm.window),
                GTK_DIALOG_DESTROY_WITH_PARENT | GTK_DIALOG_MODAL,
                GTK_MESSAGE_QUESTION,
                GTK_BUTTONS_OK_CANCEL,
                _("Available Encodings:"));
        gtk_window_set_title(GTK_WINDOW(dialog), _("Select Export Encoding"));

        GtkWidget *combo = gtk_combo_box_text_new();
        int n_enc_entries = sizeof (enc_entries) / sizeof (enc_entries[0]);
	int i;
        for (i=0; i<n_enc_entries; i++) {
                gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(combo), enc_entries[i].name);
                if (strstr(enc_entries[i].name, config.common.export_encoding))
                        gtk_combo_box_set_active(GTK_COMBO_BOX(combo), i);
        }

        gtk_container_add(GTK_CONTAINER(gtk_message_dialog_get_message_area(GTK_MESSAGE_DIALOG(dialog))), combo);
        gtk_widget_show(combo);

        if (gtk_dialog_run(GTK_DIALOG(dialog)) == GTK_RESPONSE_OK) {
		sprintf(&config.common.export_encoding[0], "%s", gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(combo)));
                write_config();
        }

        gtk_widget_destroy(dialog);
}


void set_newline (GtkWidget *widget, gpointer window) {
        GtkWidget *dialog = gtk_message_dialog_new(GTK_WINDOW(config.vbsm.window),
                GTK_DIALOG_DESTROY_WITH_PARENT | GTK_DIALOG_MODAL,
                GTK_MESSAGE_QUESTION,
                GTK_BUTTONS_OK_CANCEL,
                _("Available Delimiters:"));
        gtk_window_set_title(GTK_WINDOW(dialog), _("Select Newline Delimiter"));

        GtkWidget *combo = gtk_combo_box_text_new();
        gtk_combo_box_text_append_text (GTK_COMBO_BOX_TEXT(combo), "LF");
        gtk_combo_box_text_append_text (GTK_COMBO_BOX_TEXT(combo), "CR/LF");

	int index;
	if (config.common.export_cr == 1)
		index = 1;
	else
		index = 0;

	gtk_combo_box_set_active(GTK_COMBO_BOX(combo), index);

        gtk_container_add(GTK_CONTAINER(gtk_message_dialog_get_message_area(GTK_MESSAGE_DIALOG(dialog))), combo);
	gtk_widget_show(combo);

        if (gtk_dialog_run(GTK_DIALOG(dialog)) == GTK_RESPONSE_OK) {
	        if( strstr(gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(combo)),"CR/LF"))
        	        config.common.export_cr = 1;
	        else
        	        config.common.export_cr = 0;

                write_config();
        }

        gtk_widget_destroy(dialog);
}


void set_network_port (GtkWidget *widget, gpointer window) {
        GtkWidget *dialog = gtk_message_dialog_new(GTK_WINDOW(config.vbsm.window),
                GTK_DIALOG_DESTROY_WITH_PARENT | GTK_DIALOG_MODAL,
                GTK_MESSAGE_QUESTION,
                GTK_BUTTONS_OK_CANCEL,
                _("Current Port:"));
        gtk_window_set_title(GTK_WINDOW(dialog), _("TCP Port"));

        GtkWidget *text = gtk_entry_new();
	char tmp1[16];
	sprintf(&tmp1[0], "%u", config.common.tcp_port);
	gtk_entry_set_text(GTK_ENTRY(text), &tmp1[0]);
        gtk_container_add(GTK_CONTAINER(gtk_message_dialog_get_message_area(GTK_MESSAGE_DIALOG(dialog))), text);
        gtk_widget_show(text);

        if (gtk_dialog_run(GTK_DIALOG(dialog)) == GTK_RESPONSE_OK) {
		config.common.tcp_port = atoi(gtk_entry_get_text(GTK_ENTRY(text)));
                write_config();
        }

        gtk_widget_destroy(dialog);
}


void set_network_server (GtkWidget *widget, gpointer window) {
        GtkWidget *dialog = gtk_message_dialog_new(GTK_WINDOW(config.vbsm.window),
                GTK_DIALOG_DESTROY_WITH_PARENT | GTK_DIALOG_MODAL,
                GTK_MESSAGE_QUESTION,
                GTK_BUTTONS_OK_CANCEL,
                _("Current Server (FQDN):"));
        gtk_window_set_title(GTK_WINDOW(dialog), _("Server Name"));

        GtkWidget *text = gtk_entry_new();
        char tmp1[16];
        sprintf(&tmp1[0], "%s", config.common.server_name);
        gtk_entry_set_text(GTK_ENTRY(text), &tmp1[0]);
        gtk_container_add(GTK_CONTAINER(gtk_message_dialog_get_message_area(GTK_MESSAGE_DIALOG(dialog))), text);
        gtk_widget_show(text);

        if (gtk_dialog_run(GTK_DIALOG(dialog)) == GTK_RESPONSE_OK) {
		if (get_host_by_name((char *) gtk_entry_get_text(GTK_ENTRY(text))) == 0) {
			show_warning_network(NULL, dialog);
		}
		else {
			strcpy(&config.common.server_name[0], (char *) gtk_entry_get_text(GTK_ENTRY(text)));
	                write_config();
			gtk_widget_destroy(dialog);
		}
        }
	else
	        gtk_widget_destroy(dialog);
}


void show_warning_network (GtkWidget *widget, gpointer window) {
	GtkWidget *dialog = gtk_message_dialog_new(GTK_WINDOW(window),
		GTK_DIALOG_DESTROY_WITH_PARENT | GTK_DIALOG_MODAL,
		GTK_MESSAGE_ERROR,
		GTK_BUTTONS_OK,
		_("DNS resolving failed."));
	gtk_window_set_title(GTK_WINDOW(dialog), _("Error"));
	gtk_dialog_run(GTK_DIALOG(dialog));
	gtk_widget_destroy(dialog);
	gtk_widget_destroy(window);
}


void use_network (GtkWidget *widget, gpointer window) {
        GtkWidget *dialog = gtk_message_dialog_new(GTK_WINDOW(config.vbsm.window),
                GTK_DIALOG_DESTROY_WITH_PARENT | GTK_DIALOG_MODAL,
                GTK_MESSAGE_QUESTION,
                GTK_BUTTONS_OK_CANCEL,
                _("Select network mode:"));
        gtk_window_set_title(GTK_WINDOW(dialog), _("Network Mode"));

        GtkWidget *combo = gtk_combo_box_text_new();
	char msg[1024];
	sprintf(&msg[0], "%s %s", "OFF:", _("Network is off"));
	gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(combo), &msg[0]);
	sprintf(&msg[0], "%s %s", "SEND:", _("Send to network"));
	gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(combo), &msg[0]);
	if (can_recv_from_net == 1) {
		sprintf(&msg[0], "%s %s", "RECV:", _("Receive from network"));
		gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(combo), &msg[0]);
	}
	gtk_combo_box_set_active(GTK_COMBO_BOX(combo), config.common.network_mode);

        gtk_container_add(GTK_CONTAINER(gtk_message_dialog_get_message_area(GTK_MESSAGE_DIALOG(dialog))), combo);
        gtk_widget_show(combo);

        if (gtk_dialog_run(GTK_DIALOG(dialog)) == GTK_RESPONSE_OK) {
		if (strstr(gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(combo)),"OFF"))
			config.common.network_mode = 0;
		else if (strstr(gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(combo)),"SEND"))
			config.common.network_mode = 1;
		else
			config.common.network_mode = 2;

                write_config();
        }

        gtk_widget_destroy(dialog);
}


void set_magic_key (GtkWidget *widget, gpointer window) {
        GtkWidget *dialog = gtk_message_dialog_new(GTK_WINDOW(config.vbsm.window),
                GTK_DIALOG_DESTROY_WITH_PARENT | GTK_DIALOG_MODAL,
                GTK_MESSAGE_QUESTION,
                GTK_BUTTONS_OK_CANCEL,
                _("Enter magic key (number between 1 and 2147483647):"));
        gtk_window_set_title(GTK_WINDOW(dialog), _("Set Magic Key"));

        GtkWidget *text = gtk_entry_new();
        char tmp1[16];
        sprintf(&tmp1[0], "%u", config.common.magic_key);
        gtk_entry_set_text(GTK_ENTRY(text), &tmp1[0]);
        gtk_container_add(GTK_CONTAINER(gtk_message_dialog_get_message_area(GTK_MESSAGE_DIALOG(dialog))), text);
        gtk_widget_show(text);

        if (gtk_dialog_run(GTK_DIALOG(dialog)) == GTK_RESPONSE_OK) {
                config.common.magic_key = atoi(gtk_entry_get_text(GTK_ENTRY(text)));
                write_config();
        }

        gtk_widget_destroy(dialog);
}

