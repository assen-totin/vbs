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

int convert_time_from_srt(char *in_time) {
        char *str_h, *str_m, *str_tmp, *str_s;

        str_h = strtok(in_time, ":");
	int res_h = atoi(str_h);

        str_m = strtok(NULL, ":");
	int res_m = atoi(str_m);

        str_tmp = strtok(NULL, ":");
        str_s = strtok(str_tmp, ",");
        int res_s = atoi(str_s);

        int res = res_h*3600 + res_m*60 + res_s;

        return res;
}

void load_srt() {
	config.common.import_fp = fopen(config.common.import_filename, "r");
        if (!config.common.import_fp)
		error_handler("proc_subtitle_local","failed to open subtitles", 1);

	char *line_in = malloc(config.common.line_size);
	if (!line_in) 
		error_handler("proc_subtitle_local","malloc failed", 1);

	char *line_out = malloc(config.common.line_size);
	if (!line_out) 
		error_handler("proc_subtitle_local","malloc failed", 1);

        char *line_tmp = malloc(config.common.line_size);
        if (!line_tmp)
                error_handler("proc_subtitle_local","malloc failed", 1);

	char *timeBegin, *timeEnd;
	char *tmp1, *tmp2;
	int timeBeginVal, timeEndVal;
	int counter = -1;

	// LOAD SUBS FROM FILE
	bzero(line_out, config.common.line_size);
	while (fgets(line_in, config.common.line_size, config.common.import_fp)) {
                // An empty line closes subtitle
                if (strlen(line_in) < 3) {
			if (counter > 0) {
	                        subs[config.vbss.local_subs_count].time_from = timeBeginVal;
        	                subs[config.vbss.local_subs_count].time_to = timeEndVal;
                	        strcpy(&subs[config.vbss.local_subs_count].sub[0], line_out);
                        	config.vbss.local_subs_count++;
				counter = -1;
			}
			continue;
                }

		// Kill newlines
		if (strstr(line_in, "\r"))
			strcpy(line_tmp, strtok(line_in, "\r"));
		else
			strcpy(line_tmp, line_in);
		if (strstr(line_tmp, "\n"))
			strcpy(line_in, strtok(line_tmp, "\n"));
		else
			strcpy(line_in, line_tmp);

		// Next Line will be a subtitle line only if current line includes timing
		if (strstr(line_in,"-->")) {
			timeBegin = strtok(line_in, "-->");
			timeEnd = strtok(NULL, "-->");

			strcpy(line_tmp, timeBegin);
			timeBeginVal = convert_time_from_srt(line_tmp);

			strcpy(line_tmp, timeEnd);
			timeEndVal = convert_time_from_srt(line_tmp);

			counter = 0;

			continue;
		}

		if (counter == 0) {
			strcpy(line_out, line_in);
			counter++;
			continue;
		}

		else if (counter > 0) { 
			sprintf(line_out, "%s\n%s", line_out, line_in);
			counter++;
			continue;
		}
	}

	free(line_in);
	free(line_out);
	free(line_tmp);
}

int show_subtitle(GtkWidget *subtitle) {

	GChecksum *md5_old = g_checksum_new(G_CHECKSUM_MD5);
	GChecksum *md5_new = g_checksum_new(G_CHECKSUM_MD5);

	const char *buffer_old_p = gtk_label_get_text(GTK_LABEL(subtitle));

	g_checksum_update(md5_old, buffer_old_p, sizeof(*buffer_old_p));
	g_checksum_update(md5_new, &current_sub[0], sizeof(current_sub));

	if(strcmp(g_checksum_get_string(md5_old),g_checksum_get_string(md5_new)) != 0) {
                gtk_label_set_text(GTK_LABEL(subtitle), &current_sub[0]);
		if (config.common.network_mode == 1)
			put_subtitle(&current_sub[0]);
	}

        return 1;
}


// Fetch a subtitle and process it
int proc_subtitle_net() {
        char buffer_new[config.common.line_size];

	while (1) {
	        if (get_subtitle(&buffer_new[0])) {
        	        strcpy(&current_sub[0], &buffer_new[0]);
	        }

		sleep(1);
	}

        return 1;
}

int proc_subtitle_local() {
	int i;

	config.common.init_timestamp = time(NULL);

	// RENDER SUBS
	while (1) {
		if (!config.vbss.paused) {
			for (i=0; i<config.vbss.local_subs_count; i++) {
				time_t curr_timestamp = time(NULL);
				if (((curr_timestamp - config.common.init_timestamp) >= subs[i].time_from)&&((curr_timestamp - config.common.init_timestamp) <= subs[i].time_to)) {
					config.common.inside_sub = true;
					strcpy(&current_sub[0], &subs[i].sub[0]);
					break;
				}
			}
		
			if (config.common.inside_sub) {
				while (1) {
					time_t curr_timestamp = time(NULL);
					if ((curr_timestamp - config.common.init_timestamp) >= subs[i].time_to) {
						strcpy(&current_sub[0], "\n");
						config.common.inside_sub = false;
						break;
					}
					else 
						sleep(1);
				}
			}
		}
		sleep(1);
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
		config.vbss.local_subs_count = 0;
	}

	/*** Initialize GTK+ ***/
	if(!g_thread_supported())
		g_thread_init( NULL );
	gdk_threads_init();
	g_log_set_handler ("Gtk", G_LOG_LEVEL_WARNING, (GLogFunc) gtk_false, NULL);
	gtk_init (&argc, &argv);
	g_log_set_handler ("Gtk", G_LOG_LEVEL_WARNING, g_log_default_handler, NULL);

	/*** Make that Window!!! ***/
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

	subtitle = gtk_label_new("");

	if (config.common.network_mode == 2) 
		strcpy(&current_sub[0], VBSS_EXPECTING_CONNECTION);
	else {
		load_srt();
		config.common.timestamp = time(NULL);
		strcpy(&current_sub[0], VBSS_NETWORK_OFF);
		g_signal_connect(window, "key_press_event", (GCallback) on_key_pressed, window);
	}
	gtk_label_set_text(GTK_LABEL(subtitle), &current_sub[0]);

	gtk_label_set_attributes(GTK_LABEL(subtitle), attr_list);

	gtk_container_add(GTK_CONTAINER(window), subtitle);

	/*** Callbacks ***/
	g_signal_connect(window, "destroy", gtk_main_quit, NULL);

	gdk_threads_add_timeout(100, show_subtitle, subtitle);

	if (config.common.network_mode == 2)
		thread = g_thread_create((GThreadFunc) proc_subtitle_net, NULL, FALSE, &error);	
	else
		thread = g_thread_create((GThreadFunc) proc_subtitle_local, NULL, FALSE, &error);

	if(!thread)
		error_handler("main","Failed to create thread",1);

	/*** Enter the main loop ***/
	gtk_widget_show_all(window);
	gtk_main();

	/* CLEANUP */
	//pango_attribute_destroy(attr_size);
	//pango_attribute_destroy(attr_colour);
	//pango_attr_list_unref(attr_list);

	gdk_threads_leave();

	return 0;
}
