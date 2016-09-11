// This file is a part of Voody Blue Subtitler suit.
// 
// Author: Assen Totin <assen.totin@gmail.com>
//
// Home page: http://www.zavedil.com/software-desktop-vbs
//
// This software is released under GNU General Public License.
// See the LICENSE file for details or visit http://www.gnu.org/copyleft/gpl.html 
// for details.

#include "../common/common.h"

int show_subtitle(GtkWidget *subtitle) {

	if (config.common.network_mode == 2)
		proc_subtitle_net();
	else
		proc_subtitle_local();

	const char *buffer_old_p = gtk_label_get_text(GTK_LABEL(subtitle));

	if(strcmp(buffer_old_p, &config.vbss.current_sub[0]) != 0) {
		gtk_label_set_text(GTK_LABEL(subtitle), &config.vbss.current_sub[0]);
		if (config.common.network_mode == 1)
			put_subtitle(&config.vbss.current_sub[0]);
	}

	return 1;
}


// Fetch a subtitle and process it
int proc_subtitle_net() {
	char buffer_new[config.common.line_size + 2];
	int n = -1;

	char request[config.common.line_size];
	strcpy(&request[0], "42");

	if (config.vbss.socketfd >= -1)
		n = send(config.vbss.socketfd, &request[0], strlen(&request[0]), 0);

	if (n < 0) {
		error_handler("proc_subtitle_net", "Could not write to socket", 0);
		if (config.vbss.socketfd >= -1)
			close(config.vbss.socketfd);
		config.vbss.socketfd = get_socket();
		return 0;
	}

	memset(&buffer_new[0], '\0', config.common.line_size + 2);

	n = recv(config.vbss.socketfd, &buffer_new[0], config.common.line_size, 0);
	if (n < 0) {
		error_handler("proc_subtitle_net", "Could not read from socket", 0);
		close(config.vbss.socketfd);
		config.vbss.socketfd = get_socket();
		return 0;
	}

	strncpy(&config.vbss.current_sub[0], &buffer_new[0], 1024);

	return 1;
}


int proc_subtitle_local() {
	int i;
	if (!config.vbss.paused) {
		long curr_time_msec = get_time_msec();
		for (i=0; i<config.vbss.total_subtitles; i++) {
			if (((curr_time_msec - config.common.init_timestamp_msec) >= sub_array[i].time_from) && ((curr_time_msec - config.common.init_timestamp_msec) <= sub_array[i].time_to)) {
				if (!config.common.inside_sub) {
					config.common.inside_sub = true;
					fix_new_line(&sub_array[i].sub[0]);
					strcpy(&config.vbss.current_sub[0], &sub_array[i].sub[0]);
				}
				break;
			}
		}
		if (config.common.inside_sub) {
			if (i == config.vbss.total_subtitles) {
				strcpy(&config.vbss.current_sub[0], "\n");
				config.common.inside_sub = false;
			}
		}
	}
	return 1;
}


// Main
int main (int argc, char *argv[]) {
	GtkWidget *window = NULL;
	GtkWidget *subtitle = NULL;
	GError *error = NULL;
	PangoAttrList *attr_list;
	PangoAttribute *attr_size, *attr_family, *attr_style, *attr_weight, *attr_colour_fg, *attr_colour_bg;
	int import_error_flag = 0;

	// i18n
	char locale_path[MAX_PATH];
	get_locale_prefix(&locale_path[0]);
	setlocale (LC_ALL, "");
	bindtextdomain(PACKAGE_NAME, &locale_path[0]);
	bind_textdomain_codeset(PACKAGE_NAME, "utf-8");
	textdomain (PACKAGE_NAME);

	// Check for alternative config
	get_cmdl_config(argc, argv);

	// Set up config from defaults
	check_config(0);

	// Config changes
	if ((config.common.network_mode == 1) || (config.common.network_mode == 2))
		get_host_by_name(&config.common.server_name[0]);
	if ((config.common.network_mode == 0) || (config.common.network_mode == 1)) {
		config.common.inside_sub = false;
		config.vbss.paused = true;
	}

	// Init GTK
	gtk_init (&argc, &argv);

	// Window
#ifdef HAVE_GTK2
	GdkColor colour2;
	colour2.red = 0x0;
	colour2.green = 0x0;
	colour2.blue = 0x0;
#elif HAVE_GTK3
	GdkRGBA colour3;
	colour3.red = 0;
	colour3.green = 0;
	colour3.blue = 0;
	colour3.alpha = 1;
#endif
	window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
	if (config.vbss.full_screen == 0) {
		gtk_widget_set_size_request (window, 600, 200);
		config.vbss.font_size = 16;
	}
	else {
		gtk_window_set_decorated (GTK_WINDOW(window), 0);
		gtk_window_fullscreen(GTK_WINDOW(window));	
	}
#ifdef HAVE_GTK2
	gtk_widget_modify_bg(window, GTK_STATE_NORMAL, &colour2);
#elif _HAVE_GTK3
	// Not working for GTK3 when the theme uses images for background.
	// https://bugzilla.gnome.org/show_bug.cgi?id=656461
	// Workraround code added below
	gtk_widget_override_background_color(window, GTK_STATE_NORMAL, &colour3);
#endif

	gtk_widget_realize (window);

	// Main text
	attr_list = pango_attr_list_new();

	attr_size = pango_attr_size_new(config.vbss.font_size * PANGO_SCALE);
	attr_size->start_index = 0;
	attr_size->end_index = G_MAXUINT;
	pango_attr_list_insert(attr_list, attr_size);

	attr_family = pango_attr_family_new(&config.vbss.font_name[0]);
	attr_family->start_index = 0;
	attr_family->end_index = G_MAXUINT;
	pango_attr_list_insert(attr_list, attr_family);

	int pango_style;
	if (strstr(&config.vbss.font_face[0], "Italic"))
		pango_style = PANGO_STYLE_ITALIC;
	else 
		pango_style = PANGO_STYLE_NORMAL;
	attr_style = pango_attr_style_new(pango_style);
	attr_style->start_index = 0;
	attr_style->end_index = G_MAXUINT;
	pango_attr_list_insert(attr_list, attr_style);

	int pango_weight;
	if (strstr(&config.vbss.font_face[0], "Bold"))
		pango_weight = PANGO_WEIGHT_BOLD;
	else
		pango_weight = PANGO_WEIGHT_NORMAL;
	attr_weight = pango_attr_weight_new(pango_weight);
	attr_weight->start_index = 0;
	attr_weight->end_index = G_MAXUINT;
	pango_attr_list_insert(attr_list, attr_weight);

	attr_colour_fg = pango_attr_foreground_new(config.vbss.colour_fg_r, config.vbss.colour_fg_g, config.vbss.colour_fg_b);
	attr_colour_fg->start_index = 0;
	attr_colour_fg->end_index = G_MAXUINT;
	pango_attr_list_insert(attr_list, attr_colour_fg);

	attr_colour_bg = pango_attr_background_new(config.vbss.colour_bg_r, config.vbss.colour_bg_g, config.vbss.colour_bg_b);
	attr_colour_bg->start_index = 0;
	attr_colour_bg->end_index = G_MAXUINT;
	pango_attr_list_insert(attr_list, attr_colour_bg);

	subtitle = gtk_label_new("");

	if (config.common.network_mode == 2) { 
		strcpy(&config.vbss.current_sub[0], _("Expecting network connection..."));
		config.vbss.socketfd = get_socket(); 
	}
	else {
		sub_array = import_subtitles_srt(&config.vbss.import_filename[0], &config.vbss.total_subtitles, &import_error_flag);
		if (import_error_flag)
			error_handler("main", _("Unable to import file: check file format"), 1);
		config.common.timestamp_msec = get_time_msec();
		config.common.init_timestamp_msec = config.common.timestamp_msec;
		strcpy(&config.vbss.current_sub[0], _("Press SPACE to start playback..."));
		g_signal_connect(window, "key_press_event", (GCallback) on_key_pressed, window);
	}
	gtk_label_set_text(GTK_LABEL(subtitle), &config.vbss.current_sub[0]);

	gtk_label_set_attributes(GTK_LABEL(subtitle), attr_list);

	if (config.vbss.justify == VBSS_JUSTIFY_LEFT)
		gtk_label_set_justify(GTK_LABEL(subtitle), GTK_JUSTIFY_LEFT);
	else if (config.vbss.justify == VBSS_JUSTIFY_RIGHT)
		gtk_label_set_justify(GTK_LABEL(subtitle), GTK_JUSTIFY_RIGHT);
	else if (config.vbss.justify == VBSS_JUSTIFY_CENTER)
		gtk_label_set_justify(GTK_LABEL(subtitle), GTK_JUSTIFY_CENTER);
	else if (config.vbss.justify == VBSS_JUSTIFY_FILL)
		gtk_label_set_justify(GTK_LABEL(subtitle), GTK_JUSTIFY_FILL);

	gtk_container_add(GTK_CONTAINER(window), subtitle);

	// Callbacks
	g_signal_connect(window, "destroy", gtk_main_quit, NULL);

#ifdef HAVE_GTK2
	g_timeout_add(config.vbss.sub_update_msec, (GtkFunction) show_subtitle, subtitle);
#elif HAVE_GTK3
	g_timeout_add(config.vbss.sub_update_msec, (GSourceFunc) show_subtitle, subtitle);
#endif

#ifdef HAVE_GTK3
	// Workaround code for gtk_widget_override_background_color() not working on GTK3 when theme uses images for window background.
	GdkScreen *screen = gdk_screen_get_default();
	GtkCssProvider *css_provider = gtk_css_provider_get_default();
	gtk_css_provider_load_from_data(css_provider, "GtkWindow {background-color: #000;}", -1, NULL);
	gtk_style_context_add_provider_for_screen(screen, GTK_STYLE_PROVIDER(css_provider), GTK_STYLE_PROVIDER_PRIORITY_USER);
#endif
	
	// Main loop
	gtk_widget_show_all(window);

	gtk_main();

	// Cleanup
	pango_attr_list_unref(attr_list);

	return 0;
}
