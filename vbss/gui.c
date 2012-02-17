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

unsigned int convert_time_from_srt(char *in_time) {
        char *str_h, *str_m, *str_tmp, *str_s;
        str_h = strtok(in_time, ":");
        str_m = strtok(NULL, ":");
        str_tmp = strtok(NULL, ":");
        str_s = strtok(str_tmp, ",");

        int res_h = atoi(str_h);
        int res_m = atoi(str_m);
        int res_s = atoi(str_s);

        int res = res_h*3600 + res_m*60 + res_s;

        return res;
}


// Fetch a subtitle and process it
int proc_subtitle_net(GtkWidget *subtitle) {
	char buffer_new[config.common.line_size], md5_old[17], md5_new[17];
	const char *buffer_old_p;

	bzero((char *) &md5_new[0], sizeof(md5_new));

	// Network subtitles
	if (get_subtitle(&buffer_new[0])) {
		buffer_old_p = gtk_label_get_text(GTK_LABEL(subtitle));

		MD5(buffer_old_p, sizeof(*buffer_old_p), &md5_old[0]);
		MD5(&buffer_new[0], sizeof(buffer_new), &md5_new[0]);

		if (strcmp(&md5_old[0], &md5_new[0]) != 0)
			gtk_label_set_text(GTK_LABEL(subtitle), &buffer_new[0]);
	}

	return 1;
}

int proc_subtitle_local(GtkWidget *subtitle) {
	// Exit, unless this is the first call to this function
	if (config.common.running)
		return 1;

	config.common.import_fp = fopen(config.common.import_filename, "r");
        if (!config.common.import_fp)
		error_handler("proc_subtitle_local","failed to open subtitles", 1);

	config.common.running = true;

	char *line_in = malloc(config.common.line_size);
	if (!line_in) 
		error_handler("proc_subtitle_local","malloc failed", 1);

	char *line_out = malloc(config.common.line_size);
	if (!line_out) 
		error_handler("proc_subtitle_local","malloc failed", 1);

	struct vbss_sub subs[1000];
	int a = 0, i;
	bool sub_is_ready = false;
	bool isNextLineSubt = false;
	bool wasPrevLineSubt = false;
	char *isLineTiming;
	char *timeBegin, *timeEnd;
	unsigned int timeBeginVal, timeEndVal;

	// LOAD SUBS FROM FILE
	bzero(line_out, config.common.line_size);
	while (fgets(line_in, config.common.line_size, config.common.import_fp)) {
		//line_in[strlen(line_in) - 1] = 0;     /* kill '\n' */

		// An empty line closes subtitle
		if ((strlen(line_in) < 3) && (isNextLineSubt)) {
			isNextLineSubt = false;
			sub_is_ready = true;
		}
		// Next Line will be a subtitle line only if current line includes timing
		else if (isLineTiming = strstr(line_in,"-->")) {
			timeBegin = strtok(line_in, "-->");
			timeEnd = strtok(NULL,"-->");
			timeBeginVal = convert_time_from_srt(timeBegin);
			timeEndVal = convert_time_from_srt(timeEnd);

			isNextLineSubt = true;
			sub_is_ready = false;
		}
		else if (isNextLineSubt) {
			sprintf(line_out, "%s%s", line_out, line_in);
			sub_is_ready = false;
		}

		if (sub_is_ready) {
			sub_is_ready = false;
			isNextLineSubt = false;
			subs[a].time_from = timeBeginVal;
			subs[a].time_to = timeEndVal;
			strcpy(&subs[a].sub[0], line_out);
			a++;
			bzero(line_out, config.common.line_size);
		}
	}

	free(line_in);
	free(line_out);

	config.common.init_timestamp = time(NULL);

/*
char b[100], c[100];
sprintf(&b[0], "%u", a);
error_handler("1", b, 0);
*/
	// RENDER SUBS
	while (1) {
		if (!config.vbss.paused) {
			for (i=0; i<a; i++) {
				time_t curr_timestamp = time(NULL);
//char d[100], e[100];
//sprintf(&d[0], "%u %u", subs[i].time_from, subs[i].time_to);
//sprintf(&e[0], "%u", (curr_timestamp - config.common.init_timestamp));
//error_handler(d, e, 0);

				if ((curr_timestamp - config.common.init_timestamp) == subs[i].time_from) {
					config.common.inside_sub = true;
					gtk_label_set_text(GTK_LABEL(subtitle), &subs[i].sub[0]);
					break;
				}
			}
		
			if (config.common.inside_sub) {
				while (1) {
					time_t curr_timestamp = time(NULL);
					if ((curr_timestamp - config.common.init_timestamp) == subs[i].time_to) {
						gtk_label_set_text(GTK_LABEL(subtitle), "\n");
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

void on_key_pressed (GtkTreeView *view, GdkEventKey *event, gpointer userdata) {
        GtkWidget *window = userdata;
        switch ( event->keyval ) {
                case GDK_space:
                        on_space_pressed(window);
                        break;
        }
}

void on_space_pressed (GtkWidget *window) {
	if (!config.common.inside_sub) {
		if (!config.vbss.paused) {
			config.vbss.paused = true;
			config.common.timestamp = time(NULL);
		}
		else {
			config.vbss.paused = false;
			time_t curr_timestamp = time(NULL);
			config.common.init_timestamp += (curr_timestamp - config.common.timestamp);
		}
	}
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
	config.common.inside_sub = false;
	config.vbss.paused = false;
	if (config.common.use_network == 1) 
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

	if (config.common.use_network == 1)
	        subtitle = gtk_label_new(VBSS_EXPECTING_CONNECTION);
	else
		subtitle = gtk_label_new(VBSS_NETWORK_OFF);

	gtk_label_set_attributes(GTK_LABEL(subtitle), attr_list);

	gtk_container_add(GTK_CONTAINER(window), subtitle);

	/*** Callbacks ***/
	g_signal_connect(window, "destroy", gtk_main_quit, NULL);

        // Key events
        g_signal_connect(window, "key_press_event", (GCallback) on_key_pressed, window);

	if (config.common.use_network == 1)
		g_timeout_add(1000, (GtkFunction) proc_subtitle_net, subtitle);
	else 
		g_timeout_add(5000, (GtkFunction) proc_subtitle_local, subtitle);

	/*** Enter the main loop ***/
	gtk_widget_show_all(window);
	gtk_main();

	/* CLEANUP */
	//pango_attribute_destroy(attr_size);
	//pango_attribute_destroy(attr_colour);
	//pango_attr_list_unref(attr_list);

	return 0;
}
