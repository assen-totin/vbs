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

void formatCellFrom(GtkTreeViewColumn *col, GtkCellRenderer *renderer, GtkTreeModel *model, GtkTreeIter *iter, gpointer user_data){
	gint from;
	gchar res[32];
	gtk_tree_model_get(model, iter, COL_FROM, &from, -1);
	convertTimeSrt(from, &res[0], 2);
	g_object_set(renderer, "text", res, NULL);
}


void formatCellTo(GtkTreeViewColumn *col, GtkCellRenderer *renderer, GtkTreeModel *model, GtkTreeIter *iter, gpointer user_data){
	gint from;
	gchar res[32];
	gtk_tree_model_get(model, iter, COL_TO, &from, -1);
	convertTimeSrt(from, &res[0], 2);
	g_object_set(renderer, "text", res, NULL);
}


void *create_view_and_model (void){
	GtkCellRenderer     *renderer;
	GtkTreeModel        *model;
	GtkTreeIter    iter;
	GtkTreeViewColumn *column;

	view = gtk_tree_view_new();

	store = gtk_list_store_new(NUM_COLS, G_TYPE_STRING, G_TYPE_UINT, G_TYPE_UINT);

	/* --- Column #1 --- */
	renderer = gtk_cell_renderer_text_new();
	gtk_tree_view_insert_column_with_attributes (GTK_TREE_VIEW (view), -1, "From", renderer, "text", COL_FROM, NULL);
	column = gtk_tree_view_get_column (GTK_TREE_VIEW (view), 0);
	gtk_tree_view_column_set_cell_data_func(column, renderer, formatCellFrom, NULL, NULL);

	/* --- Column #2 --- */
	renderer = gtk_cell_renderer_text_new();
	gtk_tree_view_insert_column_with_attributes (GTK_TREE_VIEW (view), -1, "To", renderer, "text", COL_TO, NULL);
	column = gtk_tree_view_get_column (GTK_TREE_VIEW (view), 1);
	gtk_tree_view_column_set_cell_data_func(column, renderer, formatCellTo, NULL, NULL);

	/* --- Column #3 --- */
	renderer = gtk_cell_renderer_text_new();
	gtk_tree_view_insert_column_with_attributes (GTK_TREE_VIEW (view), -1, "Text", renderer, "text", COL_LINE, NULL);
	// Editable cells
	g_object_set(renderer, "editable", TRUE, NULL);
	g_signal_connect(renderer, "edited", (GCallback) view_cellEdit, view);

	gtk_list_store_append (store, &iter);
	gtk_list_store_set (store, &iter, COL_LINE, " ", COL_FROM, 0, COL_TO, 0, -1);

	model = GTK_TREE_MODEL(store);
	gtk_tree_view_set_model (GTK_TREE_VIEW (view), model);

	/* The tree view has acquired its own reference to the
	*  model, so we can drop ours. That way the model will
	*  be freed automatically when the tree view is destroyed */

	g_object_unref (model);
}


int main (int argc, char **argv){
	GtkWidget *window, *scroll;
	GtkWidget *status, *vbox, *progress;

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
	sprintf(config.vbsm.logFileName, "%s/vbsLogFile.XXXXXX", VBS_TMP_DIR);
	int mkstempRes = mkstemp(config.vbsm.logFileName);
	if (mkstempRes == -1) {error_handler("main","failed to create log file name",1 );}
	config.vbsm.logFile = fopen(config.vbsm.logFileName, "w");

	// Create tmp subtites file for mplayer
	sprintf(config.vbsm.sub_file_name, "%s/vbsTempFile.XXXXXX", VBS_TMP_DIR);
	mkstempRes = mkstemp(config.vbsm.sub_file_name);
	if (mkstempRes == -1) {error_handler("main","failed to create temporary sub file name",1 );}

	// GTK Init
	gtk_init (&argc, &argv);

	// Create "view", set options
	create_view_and_model();

	// Only one row selected
	GtkTreeSelection *selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(view));
	gtk_tree_selection_set_mode(selection, GTK_SELECTION_SINGLE);

	// Scroll, will contain "view", will be packed in the bottom of the vbox
	scroll = gtk_scrolled_window_new (NULL,NULL);
	gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (scroll),  GTK_POLICY_AUTOMATIC, GTK_POLICY_ALWAYS);
	gtk_container_add (GTK_CONTAINER (scroll), view);

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
	window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title (GTK_WINDOW (window), "Voody Blue Subtitler");
	gtk_window_set_default_icon_from_file (VBS_ICON, NULL);
	gtk_widget_set_size_request (window, 500, 400);
	g_signal_connect (window, "delete_event", G_CALLBACK(quitDialog), window);

	// Link double-click event
	g_signal_connect(view, "row-activated", (GCallback) on_clicked_row, window);

	// Key events
	g_signal_connect(view, "key_press_event", (GCallback) on_pressed_key, window);

	// Menu
	can_recv_from_net = 0;
	GtkWidget *menu = makeMenu(window, &menuEntries[0], VBSM_MENU_COUNT);

	// Create vbox
	vbox = gtk_vbox_new (FALSE, 0);

	// Pack entry & scroll into vbox
	gtk_box_pack_start(GTK_BOX(vbox), menu, FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(vbox), status, FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(vbox), progress, FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(vbox), scroll, TRUE, TRUE, 0);

	// Add vbox to window
	gtk_container_add(GTK_CONTAINER (window), vbox);

	gtk_widget_show_all(window);

	// Progress bar check & update function
	g_timeout_add(1000, (GtkFunction) progress_bar_update, NULL);

	gtk_main();

	return 0;
}

