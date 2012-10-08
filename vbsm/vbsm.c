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

int main (int argc, char **argv){
	GtkWidget *window, *vbox, *status, *progress;
	GtkWidget *mplayer_scroll;
	GtkTreeSelection *mplayer_selection;

        // Check for alternative config
        get_cmdl_config(argc, argv);

        // Set up config from defaults
        check_config(0);
        if (config.common.network_mode == 1)
                get_host_by_name(&config.common.server_name[0]);

	// Initalize time-ticks counter, set it to zero
	config.common.running = FALSE;
	config.vbsm.have_loaded_text = FALSE;
	config.vbsm.mplayer_pid = 0;
	config.common.init_timestamp = time(NULL);

	// Create log file
	sprintf(config.vbsm.log_file_name, "%s/vbsLogFile.XXXXXX", VBS_TMP_DIR);
	int mkstempRes = mkstemp(config.vbsm.log_file_name);
	if (mkstempRes == -1) {error_handler("main","failed to create log file name",1 );}
	config.vbsm.log_file_fp = fopen(config.vbsm.log_file_name, "w");

	// Create tmp subtites file for mplayer
	if (config.vbsm.video_backend == VBSM_VIDEO_BACKEND_MPLAYER) {
		sprintf(config.vbsm.sub_file_name, "%s/vbsTempFile.XXXXXX", VBS_TMP_DIR);
		mkstempRes = mkstemp(config.vbsm.sub_file_name);
		if (mkstempRes == -1) {error_handler("main","failed to create temporary sub file name",1 );}
	}

	// GTK Init
	gtk_init (&argc, &argv);

	// Create "view", set options
	create_view_and_model();

	// Only one row selected
        mplayer_selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(config.vbsm.mplayer_view));
        gtk_tree_selection_set_mode(mplayer_selection, GTK_SELECTION_SINGLE);

	// MPlayer scroll, will contain the mplayer "view", will be packed in the bottom of the vbox
	mplayer_scroll = gtk_scrolled_window_new (NULL,NULL);
	gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (mplayer_scroll),  GTK_POLICY_AUTOMATIC, GTK_POLICY_ALWAYS);
	gtk_container_add (GTK_CONTAINER (mplayer_scroll), config.vbsm.mplayer_view);

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
	gtk_progress_bar_set_orientation(GTK_PROGRESS_BAR(progress),GTK_PROGRESS_LEFT_TO_RIGHT);
	config.vbsm.progress = progress;

	// Root window
	int window_width = 500, window_height = 400;
        GdkScreen *gdk_screen = gdk_screen_get_default();
        int screen_width = gdk_screen_get_width(gdk_screen);
        int screen_height = gdk_screen_get_height(gdk_screen);

        window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
        gtk_window_set_title (GTK_WINDOW (window), "Voody Blue Subtitler");
        gtk_window_set_default_icon_from_file (VBS_ICON, NULL);
        if (config.vbsm.video_backend == VBSM_VIDEO_BACKEND_GSTREAMER) {
                GdkScreen *gdk_screen = gdk_screen_get_default();
		if (gdk_screen) {
	                int screen_width = gdk_screen_get_width(gdk_screen);
        	        int screen_height = gdk_screen_get_height(gdk_screen);
	                window_width = (int) (0.8 * screen_width);
                	window_height = (int) (0.9 * screen_height);
		}
        }
        gtk_widget_set_size_request (window, window_width, window_height);
        g_signal_connect (window, "delete_event", G_CALLBACK(quitDialog), window);

	// Link double-click event
	g_signal_connect(config.vbsm.mplayer_view, "row-activated", (GCallback) on_clicked_row, window);

	// Key events
	g_signal_connect(config.vbsm.mplayer_view, "key_press_event", (GCallback) on_pressed_key, window);

	// Menu
	can_recv_from_net = 0;
	GtkWidget *menu = makeMenu(window, &menuEntries[0], VBSM_MENU_COUNT);

	// Create vbox
	vbox = gtk_vbox_new (FALSE, 0);

	// Pack entry & scroll into vbox
	gtk_box_pack_start(GTK_BOX(vbox), menu, FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(vbox), status, FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(vbox), progress, FALSE, FALSE, 0);
	if (config.vbsm.video_backend == VBSM_VIDEO_BACKEND_GSTREAMER) {
		gtk_box_pack_start(GTK_BOX(vbox), config.vbsm.gstreamer_widget_player, TRUE, TRUE, 0);
	}
	gtk_box_pack_start(GTK_BOX(vbox), mplayer_scroll, TRUE, TRUE, 0);

	// Add vbox to window
	gtk_container_add(GTK_CONTAINER (window), vbox);

	gtk_widget_show_all(window);

	// Progress bar check & update function
	g_timeout_add(1000, (GtkFunction) progress_bar_update, NULL);

	gtk_main();

	return 0;
}

