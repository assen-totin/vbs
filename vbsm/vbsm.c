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

#include "menu-def.h"

int main (int argc, char **argv){
	GtkWidget *vbox, *status, *progress;
	GtkWidget *subtitles_scroll;
	GtkTreeSelection *subtitles_selection;

	// i18n
#ifdef HAVE_POSIX
	setlocale (LC_ALL, "");
#endif
#ifdef HAVE_WINDOWS
	char locale[32];
	win_get_locale(&locale[0]);
	char locale_env[32];
	sprintf(&locale_env[0], "LANG=%s.utf8", &locale[0]);
	putenv(&locale_env[0]);
	gtk_disable_setlocale();
#endif
	char locale_path[MAX_PATH];
	get_locale_prefix(&locale_path[0]);
	bindtextdomain(PACKAGE_NAME, &locale_path[0]);
	bind_textdomain_codeset(PACKAGE_NAME, "utf-8");
	textdomain (PACKAGE_NAME);

	// Check for alternative config
	get_cmdl_config(argc, argv);

	// Set up config from defaults
	check_config(0);
	if (config.common.network_mode == 1)
		get_host_by_name(&config.common.server_name[0]);

	// Initalize time-ticks counter, set it to zero
	config.common.running = FALSE;
	config.vbsm.have_loaded_text = FALSE;
	config.vbsm.have_loaded_video = FALSE;
	config.vbsm.mplayer_pid = 0;
	config.common.init_timestamp_msec = get_time_msec();

	// Create log file
	char _tmp[MAX_PATH];
	get_config_dir(&_tmp[0]);
	sprintf(&config.vbsm.log_file_name[0], "%s%s.XXXXXX", &_tmp[0], VBSM_LOG_FILE);
	int mkstempRes = g_mkstemp(config.vbsm.log_file_name);
	if (mkstempRes == -1) 
		error_handler("main","failed to create log file name",1 );
	config.vbsm.log_file_fp = fopen(config.vbsm.log_file_name, "w");
	del_old_logs();

	// Create tmp subtites file for mplayer
	if (config.vbsm.video_backend == VBSM_VIDEO_BACKEND_MPLAYER) {
		sprintf(&config.vbsm.sub_file_name[0], "%s%s.XXXXXX", &_tmp[0], VBSM_MPLAYER_SUB_FILE);
		mkstempRes = g_mkstemp(config.vbsm.sub_file_name);
		if (mkstempRes == -1) 
			error_handler("main","failed to create temporary sub file name",1 );
	}

	// GTK Init
	gtk_init (&argc, &argv);

	// Create "view", set options
	create_view_and_model();

	// Only one row selected
	subtitles_selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(config.vbsm.subtitles_view));
	gtk_tree_selection_set_mode(subtitles_selection, GTK_SELECTION_SINGLE);

	// MPlayer scroll, will contain the mplayer "view", will be packed in the bottom of the vbox
	subtitles_scroll = gtk_scrolled_window_new (NULL,NULL);
	gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (subtitles_scroll),  GTK_POLICY_AUTOMATIC, GTK_POLICY_ALWAYS);
	gtk_container_add (GTK_CONTAINER (subtitles_scroll), config.vbsm.subtitles_view);

	if (config.vbsm.video_backend == VBSM_VIDEO_BACKEND_GSTREAMER) {
		// Video widget
		config.vbsm.gstreamer_widget_player = gtk_drawing_area_new();
		gtk_widget_set_size_request (config.vbsm.gstreamer_widget_player, 0x200, 0x100);
	}

	// Status, display status, will be packed in the top of the vbox
	status = gtk_statusbar_new();
	guint status_context_id = gtk_statusbar_get_context_id(GTK_STATUSBAR(status), "Current status: ");
	gtk_statusbar_push(GTK_STATUSBAR(status), status_context_id, "Status: PAUSED");

	config.vbsm.status = status;
	config.vbsm.status_context_id = status_context_id;
	config.common.inside_sub = FALSE;

	// Progress, will be packed in the middle of the vbox
	progress = gtk_progress_bar_new();
#ifdef HAVE_GTK2
	gtk_progress_bar_set_orientation(GTK_PROGRESS_BAR(progress), GTK_PROGRESS_LEFT_TO_RIGHT);
#elif HAVE_GTK3
	gtk_orientable_set_orientation(GTK_ORIENTABLE(progress), GTK_ORIENTATION_HORIZONTAL);
	gtk_progress_bar_set_inverted(GTK_PROGRESS_BAR(progress), FALSE);
	gtk_progress_bar_set_show_text(GTK_PROGRESS_BAR(progress), TRUE);
	gtk_progress_bar_set_text(GTK_PROGRESS_BAR(progress), "");
#endif
	config.vbsm.progress = progress;

	// Root window
	int window_width = 500, window_height = 400;
	GdkScreen *gdk_screen = gdk_screen_get_default();
	int screen_width = gdk_screen_get_width(gdk_screen);
	int screen_height = gdk_screen_get_height(gdk_screen);

	config.vbsm.window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title (GTK_WINDOW (config.vbsm.window), "Voody Blue Subtitler");

	char vbs_icon[1024];
	get_icon(&vbs_icon[0]);
	gtk_window_set_default_icon_from_file (&vbs_icon[0], NULL);

	if (config.vbsm.video_backend == VBSM_VIDEO_BACKEND_GSTREAMER) {
		GdkScreen *gdk_screen = gdk_screen_get_default();
		if (gdk_screen) {
			int screen_width = gdk_screen_get_width(gdk_screen);
			int screen_height = gdk_screen_get_height(gdk_screen);
			window_width = (int) (0.8 * screen_width);
			window_height = (int) (0.9 * screen_height);
		}
	}
	gtk_widget_set_size_request (config.vbsm.window, window_width, window_height);
	g_signal_connect (config.vbsm.window, "delete_event", G_CALLBACK(quitDialog), config.vbsm.window);

	// Link double-click event
	g_signal_connect(config.vbsm.subtitles_view, "row-activated", (GCallback) on_clicked_row, config.vbsm.window);

	// Key events
	g_signal_connect(config.vbsm.subtitles_view, "key_press_event", (GCallback) on_pressed_key, config.vbsm.window);

	// Menu
	config.common.can_recv_from_net = false;
	// Only the GTK+ idiots know why menu cannot be built in a function and returned as a widget -
	// like the "deprecated" GtkItemFactoryEntry seamlesly did
	GtkUIManager *p_uiManager = gtk_ui_manager_new ();
	GtkActionGroup *p_actionGroup = gtk_action_group_new ("menuActionGroup");
	gtk_action_group_add_actions (p_actionGroup, menu_entries, G_N_ELEMENTS (menu_entries), NULL);
	gtk_ui_manager_insert_action_group (p_uiManager, p_actionGroup, 0);
	gtk_ui_manager_add_ui_from_string (p_uiManager, ui, -1, NULL);
	GtkWidget *menu = gtk_ui_manager_get_widget(p_uiManager, "/MainMenu");
	GtkAccelGroup *accel_group = gtk_ui_manager_get_accel_group(p_uiManager);
	gtk_window_add_accel_group(GTK_WINDOW(config.vbsm.window), accel_group);

	// Create vbox
#ifdef HAVE_GTK2
	vbox = gtk_vbox_new (FALSE, 0);
#elif HAVE_GTK3
	vbox = gtk_box_new( GTK_ORIENTATION_VERTICAL, 0);
#endif

	// Pack entry & scroll into vbox
	gtk_box_pack_start(GTK_BOX(vbox), menu, FALSE, FALSE, 0);
	if (config.vbsm.video_backend == VBSM_VIDEO_BACKEND_GSTREAMER) {
		gtk_box_pack_start(GTK_BOX(vbox), config.vbsm.gstreamer_widget_player, TRUE, TRUE, 0);
	}
	gtk_box_pack_start(GTK_BOX(vbox), progress, FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(vbox), subtitles_scroll, TRUE, TRUE, 0);
	gtk_box_pack_start(GTK_BOX(vbox), status, FALSE, FALSE, 0);

	// Add vbox to window
	gtk_container_add(GTK_CONTAINER (config.vbsm.window), vbox);

	gtk_widget_show_all(config.vbsm.window);

	// Progress bar check & update function
	g_timeout_add(config.vbsm.progress_update_msec, progress_bar_update, NULL);

	gtk_main();

	return 0;
}

