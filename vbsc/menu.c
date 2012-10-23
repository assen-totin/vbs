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

void quit_dialog(GtkWidget *widget, gpointer window) {
	GtkWidget *dialog;
	dialog = gtk_message_dialog_new(GTK_WINDOW(config.vbsm.window),
		GTK_DIALOG_DESTROY_WITH_PARENT | GTK_DIALOG_MODAL,
		GTK_MESSAGE_QUESTION,
		GTK_BUTTONS_OK_CANCEL,
		_("Really quit?"));

	gtk_window_set_title(GTK_WINDOW(dialog), _("Really quit?"));

	if (gtk_dialog_run (GTK_DIALOG (dialog)) == GTK_RESPONSE_OK) {
		gtk_widget_destroy(dialog);
		gtk_main_quit();
	}
	else 
		gtk_widget_destroy(dialog);
}


void file_dialog_save(GtkAction *action, gpointer param) {
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

		if (strstr(gtk_action_get_name(action), "EditExportDefault")) {
			strcpy(&config.common.export_filename[0], filename);
		}

		g_free (filename);
	}

	gtk_widget_destroy (fileDialogWidget);
}


void file_dialog_open(GtkAction *action, gpointer param) {
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
		if (strstr(gtk_action_get_name(action), "EditImportDefault")) {
			// Import Text-only
			strcpy(&config.vbss.import_filename[0], filename);
		}
		g_free (filename);
	}
	
	gtk_widget_destroy (fileDialogWidget);
}


void select_font(GtkWidget *widget, gpointer window) {
	GtkWidget *dialog;
	gchar *fontname;
	PangoFontDescription *font_desc;
#ifdef HAVE_GTK2
	dialog = gtk_font_selection_dialog_new(_("Select Font"));
	gtk_font_selection_dialog_set_preview_text(GTK_FONT_SELECTION_DIALOG(dialog), _("Quick brown fox jumps over the lazy dog..."));
	char set_font[128];
	sprintf(&set_font[0], "%s %s %u", config.vbss.font_name, config.vbss.font_face, config.vbss.font_size);
	gtk_font_selection_dialog_set_font_name(GTK_FONT_SELECTION_DIALOG(dialog), &set_font[0]);

#elif HAVE_GTK3
	dialog = gtk_font_chooser_dialog_new(_("Select Font"), window);
	gtk_font_chooser_set_preview_text(GTK_FONT_CHOOSER(dialog), _("Quick brown fox jumps over the lazy dog..."));
	char set_font[128];
	sprintf(&set_font[0], "%s %s %u", config.vbss.font_name, config.vbss.font_face, config.vbss.font_size);
	gtk_font_chooser_set_font(GTK_FONT_CHOOSER(dialog), &set_font[0]);
#endif

	int res = gtk_dialog_run(GTK_DIALOG(dialog));
	if ((res == GTK_RESPONSE_OK) || (res == GTK_RESPONSE_APPLY)) {
#ifdef HAVE_GTK2
		fontname = gtk_font_selection_dialog_get_font_name(GTK_FONT_SELECTION_DIALOG(dialog));
#elif HAVE_GTK3
		fontname = gtk_font_chooser_get_font(GTK_FONT_CHOOSER(dialog));
#endif

		font_desc = pango_font_description_from_string(fontname);
		sprintf(&config.vbss.font_name[0], "%s", pango_font_description_get_family(font_desc));
		config.vbss.font_size = pango_font_description_get_size(font_desc) / 1024;

		gint weight = pango_font_description_get_weight(font_desc);
		if (pango_font_description_get_style(font_desc) ==  PANGO_STYLE_NORMAL) {
			if (weight <= 500)
				sprintf(&config.vbss.font_face[0], "%s", "Regular\n");
			else
				sprintf(&config.vbss.font_face[0], "%s", "Bold\n");
		}
		else {
			if (weight <= 500)
				sprintf(&config.vbss.font_face[0], "%s", "Italic\n");
			else
				sprintf(&config.vbss.font_face[0], "%s", "Bold Italic\n");
		}

		g_free(fontname);
		write_config();
	}
	gtk_widget_destroy(dialog);
}


void select_colour(GtkAction *action, gpointer window) {
	GtkResponseType result;
	GtkWidget *dialog, *coloursel;

	char title[128];
	if (strstr(gtk_action_get_name(action), "PlayerFontColour")) 
		strcpy(&title[0], _("Font Color"));
	else if (strstr(gtk_action_get_name(action), "PlayerBgColour"))
		strcpy(&title[0], _("Background Color"));

#ifdef HAVE_GTK2
	GdkColor colour2;
	if (strstr(gtk_action_get_name(action), "PlayerFontColour")) {
		colour2.red = config.vbss.colour_fg_r;
		colour2.green = config.vbss.colour_fg_g;
		colour2.blue = config.vbss.colour_fg_b;
	}
	else if (strstr(gtk_action_get_name(action), "PlayerBgColour")) {
		colour2.red = config.vbss.colour_bg_r;
		colour2.green = config.vbss.colour_bg_g;
		colour2.blue = config.vbss.colour_bg_b;
	}
	dialog = gtk_color_selection_dialog_new(&title[0]);
	coloursel = gtk_color_selection_dialog_get_color_selection(GTK_COLOR_SELECTION_DIALOG(dialog));
	gtk_color_selection_set_current_color (GTK_COLOR_SELECTION(coloursel), &colour2);
#elif HAVE_GTK3
	GdkRGBA colour3;
	if (strstr(gtk_action_get_name(action), "PlayerFontColour")) {
		colour3.red = config.vbss.colour_fg_r / 65536;
		colour3.green = config.vbss.colour_fg_g / 65536;
		colour3.blue = config.vbss.colour_fg_b / 65536;
		colour3.alpha = 1;
	}
	else if (strstr(gtk_action_get_name(action), "PlayerBgColour")) {
		colour3.red = config.vbss.colour_bg_r / 65536;
		colour3.green = config.vbss.colour_bg_g / 65536;
		colour3.blue = config.vbss.colour_bg_b / 65536;
		colour3.alpha = 1;
	}
	dialog = gtk_color_chooser_dialog_new(&title[0], window);
	// Not working in 3.4?
	g_object_set (G_OBJECT(dialog), "show-editor", TRUE, NULL);
	gtk_color_chooser_set_rgba(GTK_COLOR_CHOOSER(dialog), &colour3);
#endif

	result = gtk_dialog_run(GTK_DIALOG(dialog));

	if (result == GTK_RESPONSE_OK) {
#ifdef HAVE_GTK2
		gtk_color_selection_get_current_color(GTK_COLOR_SELECTION(coloursel), &colour2);
		if (strstr(gtk_action_get_name(action), "PlayerFontColour")) {
			config.vbss.colour_fg_r = colour2.red;
			config.vbss.colour_fg_g = colour2.green;
			config.vbss.colour_fg_b = colour2.blue;
		}
		else if (strstr(gtk_action_get_name(action), "PlayerBgColour")) {
			config.vbss.colour_bg_r = colour2.red;
			config.vbss.colour_bg_g = colour2.green;
			config.vbss.colour_bg_b = colour2.blue;
		}
#elif HAVE_GTK3
		gtk_color_chooser_get_rgba(GTK_COLOR_CHOOSER(dialog), &colour3);
		if (strstr(gtk_action_get_name(action), "PlayerFontColour")) {
			config.vbss.colour_fg_r = (int) (colour3.red * 65536);
			config.vbss.colour_fg_g = (int) (colour3.green * 65536);
			config.vbss.colour_fg_b = (int) (colour3.blue * 65536);
		}
		else if (strstr(gtk_action_get_name(action), "PlayerBgColour")) {
			config.vbss.colour_bg_r = (int) (colour3.red * 65536);
			config.vbss.colour_bg_g = (int) (colour3.green * 65536);
			config.vbss.colour_bg_b = (int) (colour3.blue * 65536);
		}
#endif
		write_config();		
	} 
	gtk_widget_destroy(dialog);
}


void select_justify (GtkWidget *widget, gpointer window) {
	GtkWidget *dialog = gtk_message_dialog_new(GTK_WINDOW(config.vbsm.window),
		GTK_DIALOG_DESTROY_WITH_PARENT | GTK_DIALOG_MODAL,
		GTK_MESSAGE_QUESTION,
		GTK_BUTTONS_OK_CANCEL,
		_("Text justification:"));
	gtk_window_set_title(GTK_WINDOW(dialog), _("Set Text Justification"));

	GtkWidget *combo = gtk_combo_box_text_new();
	char text[128];
	sprintf(&text[0], "%s: %s", "LEFT", _("Left aligned"));
	gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(combo), &text[0]);
	sprintf(&text[0], "%s: %s", "RIGHT", _("Right aligned"));
	gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(combo), &text[0]);
	sprintf(&text[0], "%s: %s", "CENTER", _("Centered"));
	gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(combo), &text[0]);
	sprintf(&text[0], "%s: %s", "FILL", _("Filled"));
	gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(combo), &text[0]);
	gtk_combo_box_set_active(GTK_COMBO_BOX(combo), config.vbss.justify);

	gtk_container_add(GTK_CONTAINER(gtk_message_dialog_get_message_area(GTK_MESSAGE_DIALOG(dialog))), combo);
	gtk_widget_show(combo);

	if (gtk_dialog_run(GTK_DIALOG(dialog)) == GTK_RESPONSE_OK) {
		if (strstr(gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(combo)),"LEFT"))
			config.vbss.justify = VBSS_JUSTIFY_LEFT;
		else if (strstr(gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(combo)),"RIGHT"))
			config.vbss.justify = VBSS_JUSTIFY_RIGHT;
		else if (strstr(gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(combo)),"CENTER"))
			config.vbss.justify = VBSS_JUSTIFY_CENTER;
		else if (strstr(gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(combo)),"FILL"))
			config.vbss.justify = VBSS_JUSTIFY_FILL;

		write_config();
	}

	gtk_widget_destroy(dialog);
}


void set_full_screen (GtkWidget *widget, gpointer window) {
	GtkWidget *dialog = gtk_message_dialog_new(GTK_WINDOW(config.vbsm.window),
		GTK_DIALOG_DESTROY_WITH_PARENT | GTK_DIALOG_MODAL,
		GTK_MESSAGE_QUESTION,
		GTK_BUTTONS_OK_CANCEL,
		_("Full-Screen mode:"));
	gtk_window_set_title(GTK_WINDOW(dialog), _("Set Full-Screen"));

	GtkWidget *combo = gtk_combo_box_text_new();
	gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(combo), "OFF");
	gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(combo), "ON");
	int index;
	if(config.vbss.full_screen == 0)
		index = 0;
	else
		index = 1;
	gtk_combo_box_set_active(GTK_COMBO_BOX(combo), index);

	gtk_container_add(GTK_CONTAINER(gtk_message_dialog_get_message_area(GTK_MESSAGE_DIALOG(dialog))), combo);
	gtk_widget_show(combo);

	if (gtk_dialog_run(GTK_DIALOG(dialog)) == GTK_RESPONSE_OK) {
		if (strstr(gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(combo)),"ON"))
			config.vbss.full_screen = 1;
		else
			config.vbss.full_screen = 0;
		write_config();
	}

	gtk_widget_destroy(dialog);
}


