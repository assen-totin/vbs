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

int show_subtitle(GtkWidget *subtitle) {

	GChecksum *md5_old = g_checksum_new(G_CHECKSUM_MD5);
	GChecksum *md5_new = g_checksum_new(G_CHECKSUM_MD5);

	const char *buffer_old_p = gtk_label_get_text(GTK_LABEL(subtitle));

	g_checksum_update(md5_old, buffer_old_p, sizeof(*buffer_old_p));
	g_checksum_update(md5_new, &config.vbss.current_sub[0], sizeof(config.vbss.current_sub));

	if(strcmp(g_checksum_get_string(md5_old),g_checksum_get_string(md5_new)) != 0) {
                gtk_label_set_text(GTK_LABEL(subtitle), &config.vbss.current_sub[0]);
		if (config.common.network_mode == 1)
			put_subtitle(&config.vbss.current_sub[0]);
	}

        return 1;
}


// Fetch a subtitle and process it
int proc_subtitle_net() {
        char buffer_new[config.common.line_size];

	while (1) {
	        if (get_subtitle(&buffer_new[0])) {
        	        strcpy(&config.vbss.current_sub[0], &buffer_new[0]);
	        }

		// Sleep 100 ms
		usleep(100000);
	}

        return 1;
}


int proc_subtitle_local() {
	int i;

	config.common.init_timestamp_msec = get_time_msec();

	// RENDER SUBS
	while (1) {
		if (!config.vbss.paused) {
			long curr_time_msec = get_time_msec();
			for (i=0; i<config.vbss.total_subtitles; i++) {
				if (((curr_time_msec - config.common.init_timestamp_msec) >= sub_array[i].time_from) && ((curr_time_msec - config.common.init_timestamp_msec) <= sub_array[i].time_to)) {
					config.common.inside_sub = true;
					strcpy(&config.vbss.current_sub[0], &sub_array[i].sub[0]);
					break;
				}
			}
		
			if (config.common.inside_sub) {
				while (1) {
					long curr_time_msec = get_time_msec();
					if ((curr_time_msec - config.common.init_timestamp_msec) >= sub_array[i].time_to) {
						strcpy(&config.vbss.current_sub[0], "\n");
						config.common.inside_sub = false;
						break;
					}
					else
						// Sleep 100 ms
						usleep(100000);
				}
			}
		}
		// Sleep 100 ms
		usleep(100000);
	}
	return 1;
}


// Main
int main (int argc, char *argv[]) {
	GdkColor colour;
	GtkWidget *window = NULL;
	GtkWidget *subtitle = NULL;
	GThread *thread;
	GError *error = NULL;
	PangoAttrList *attr_list;
	PangoAttribute *attr_size, *attr_colour_fg, *attr_colour_bg;

        // i18n
        setlocale (LC_ALL, "");
        bindtextdomain (PACKAGE_NAME, LOCALEDIR);
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
	if(!g_thread_supported())
		g_thread_init( NULL );
	gdk_threads_init();
	g_log_set_handler ("Gtk", G_LOG_LEVEL_WARNING, (GLogFunc) gtk_false, NULL);
	gtk_init (&argc, &argv);
	g_log_set_handler ("Gtk", G_LOG_LEVEL_WARNING, g_log_default_handler, NULL);

	// Window
	colour.red = 0x0;
	colour.green = 0x0;
	colour.blue = 0x0;
	window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
	if (config.vbss.full_screen == 0) {
		gtk_widget_set_size_request (window, 600, 200);
		config.vbss.font_size = 16;
	}
	else {
		gtk_window_set_decorated (GTK_WINDOW(window), 0);
		gtk_window_fullscreen(GTK_WINDOW(window));	
	}
	gtk_widget_modify_bg(window, GTK_STATE_NORMAL, &colour);
	gtk_widget_realize (window);

	// Main text
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

	subtitle = gtk_label_new("");

	if (config.common.network_mode == 2) 
		strcpy(&config.vbss.current_sub[0], _("Expecting network connection..."));
	else {
		sub_array = import_subtitles_srt(&config.vbss.import_filename[0], &config.vbss.total_subtitles);
		config.common.timestamp_msec = get_time_msec();
		strcpy(&config.vbss.current_sub[0], _("Press SPACE to start playback..."));
		g_signal_connect(window, "key_press_event", (GCallback) on_key_pressed, window);
	}
	gtk_label_set_text(GTK_LABEL(subtitle), &config.vbss.current_sub[0]);

	gtk_label_set_attributes(GTK_LABEL(subtitle), attr_list);

	gtk_container_add(GTK_CONTAINER(window), subtitle);

	// Callbacks
	g_signal_connect(window, "destroy", gtk_main_quit, NULL);

#ifdef HAVE_GTK2
	gdk_threads_add_timeout(100, (GtkFunction) show_subtitle, subtitle);
#elif HAVE_GTK3
	gdk_threads_add_timeout(100, (GSourceFunc) show_subtitle, subtitle);
#endif

	if (config.common.network_mode == 2)
		thread = g_thread_create((GThreadFunc) proc_subtitle_net, NULL, FALSE, &error);	
	else
		thread = g_thread_create((GThreadFunc) proc_subtitle_local, NULL, FALSE, &error);

	if(!thread)
		error_handler("main","Failed to create thread",1);

	// Main loop
	gtk_widget_show_all(window);
	gtk_main();

	// Cleanup
	//pango_attribute_destroy(attr_size);
	//pango_attribute_destroy(attr_colour);
	//pango_attr_list_unref(attr_list);

	gdk_threads_leave();

	return 0;
}
