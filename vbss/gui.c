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

// Fetch a subtitle and process it
int proc_subtitle(GtkWidget *subtitle) {
	char buffer_new[config.common.line_size], md5_old[17], md5_new[17];
	const char *buffer_old_p;

	bzero((char *) &md5_new[0], sizeof(md5_new));

	if (get_subtitle(&buffer_new[0])) {
		buffer_old_p = gtk_label_get_text(GTK_LABEL(subtitle));

		MD5(buffer_old_p, sizeof(*buffer_old_p), &md5_old[0]);
		MD5(&buffer_new[0], sizeof(buffer_new), &md5_new[0]);

		if (strcmp(&md5_old[0], &md5_new[0]) != 0)
			gtk_label_set_text(GTK_LABEL(subtitle), &buffer_new[0]);
	}

	return 1;
}

// Main
int main (int argc, char *argv[]) {
	GdkColor colour;
	GtkWidget *window = NULL;
	GtkWidget *subtitle = NULL;
	PangoAttrList *attr_list;
	PangoAttribute *attr_size, *attr_colour_fg, *attr_colour_bg;

	// Set up config from defaults
	check_config();
	get_host_by_name(&config.common.server_name[0]);

	/*** Initialize GTK+ ***/
	g_log_set_handler ("Gtk", G_LOG_LEVEL_WARNING, (GLogFunc) gtk_false, NULL);
	gtk_init (&argc, &argv);
	g_log_set_handler ("Gtk", G_LOG_LEVEL_WARNING, g_log_default_handler, NULL);

	/*** Make that Window!!! ***/
	colour.red = 0x0;
	colour.green = 0x0;
	colour.blue = 0x0;
	window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
	if (config.common.test_mode)
		gtk_widget_set_size_request (window, 600, 200);
	else {
		gtk_window_set_decorated (GTK_WINDOW(window), 0);
		gtk_window_fullscreen(GTK_WINDOW(window));	
	}
	gtk_widget_modify_bg(window, GTK_STATE_NORMAL, &colour);
	gtk_widget_realize (window);

	/* MAIN TEXT COMES HERE */
	attr_list = pango_attr_list_new();
	attr_size = pango_attr_size_new(config.vbss.font_size * PANGO_SCALE);
	attr_size->start_index = 0;
	attr_size->end_index = G_MAXUINT;
	pango_attr_list_insert(attr_list, attr_size);

	attr_colour_bg = pango_attr_foreground_new(config.vbss.colour_fg_r, config.vbss.colour_fg_g, config.vbss.colour_fg_b);
	attr_colour_bg->start_index = 0;
	attr_colour_bg->end_index = G_MAXUINT;
	pango_attr_list_insert(attr_list, attr_colour_bg);

        attr_colour_bg = pango_attr_background_new(config.vbss.colour_bg_r, config.vbss.colour_bg_g, config.vbss.colour_bg_b);
        attr_colour_bg->start_index = 0;
        attr_colour_bg->end_index = G_MAXUINT;
        pango_attr_list_insert(attr_list, attr_colour_bg);

        subtitle = gtk_label_new("Expecting connection...");

	gtk_label_set_attributes(GTK_LABEL(subtitle), attr_list);

	gtk_container_add(GTK_CONTAINER(window), subtitle);

	/*** Callbacks ***/
	g_signal_connect(window, "destroy", gtk_main_quit, NULL);
	g_timeout_add (1000, (GtkFunction) proc_subtitle, subtitle);

	/*** Enter the main loop ***/
	gtk_widget_show_all(window);
	gtk_main();

	/* CLEANUP */
	//pango_attribute_destroy(attr_size);
	//pango_attribute_destroy(attr_colour);
	//pango_attr_list_unref(attr_list);

	return 0;
}
