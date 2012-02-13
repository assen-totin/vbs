// This file is a part of Voody Blue Subtitler.
//
// Author: Assen Totin <assen@online.bg>
//
// Home page: http://bilbo.online.bg/~assen/vbs
//
// This software is released under GNU General Public License.
// See the LICENSE file for details or visit http://www.gnu.org/copyleft/gpl.html
// for details.

#include "main.h"


void errorHandler(char function[128], char error[128]) {
	printf("*** VBS Critical Error: %s in function %s\n", error, function);
	exit(1);
}


bool mplayerAlive() {
	if (counter.mplayer_pid == 0) {return false;}
	int status;
	pid_t cpid = waitpid(-1, &status, WNOHANG);
	if (cpid == counter.mplayer_pid) {return false;}
	return true;
}


bool configBool(char *line) {
	char *lineRest;
	int res;
	strtok(line, "=");
	lineRest = strtok(NULL, "$$$");
	res = atoi(lineRest);
	if (res == 0) {return false;}
	return true;
}


void configChar(char *line, char *param) {
	char *lineRest;
	strtok(line, "=");
	lineRest = strtok(NULL, "$$$");
	sprintf(param,lineRest);
}


void writeConfig(bool createDefault) {
	if (createDefault) {
		if (VBS_CONFIG_DEFAULT_CR == 0) {counter.config_export_cr = FALSE;}
		else {counter.config_export_cr = TRUE;}

		int nencEntries = sizeof (encEntries) / sizeof (encEntries[0]);
		int i;
		for (i=0; i<nencEntries; i++) {
			if (encEntries[i].dflt) {
				sprintf(counter.config_export_encoding, "%s", encEntries[i].name);
				sprintf(counter.config_import_encoding, "%s", encEntries[i].name);
			}
		}
	}
	FILE *fpConfig = fopen(counter.configFileName, "w");
	if (!fpConfig) {errorHandler("writeConfig","could not open config file");}
	fprintf(fpConfig, "%s", VBS_CONFIG_HEADER);
	if (counter.config_export_cr) {fprintf(fpConfig, "EXPORT_CR=1\n");}
	else {fprintf(fpConfig, "EXPORT_CR=0\n");}
	fprintf(fpConfig, "EXPORT_ENCODING=%s\n", &counter.config_export_encoding[0]);
	fprintf(fpConfig, "IMPORT_ENCODING=%s\n", &counter.config_import_encoding[0]);
	fclose(fpConfig);
}


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

	// Initalize time-ticks counter, set it to zero
	counter.running = FALSE;
	counter.have_loaded_text = FALSE;
	counter.mplayer_pid = 0;

	// Create temporary file (GTK is buggy ot GCC?)
	sprintf(counter.tmpFileName, "%s/vbsTempFile.XXXXXX", VBS_TMP_DIR);
	int mkstempRes = mkstemp(counter.tmpFileName);
	if (mkstempRes == -1) {errorHandler("main","failed to create temporary file name");}
	counter.tmpFile = fopen(counter.tmpFileName,"w");

	// Check for "~/.vbs", if not, create one
	struct passwd *passwdEntry;
	struct stat statBuf; 
	char vbsDir[1024];
	bool flagConfig = FALSE;
	int mkdirRes, statRes, errsv;
	FILE *fpConfig;

	passwdEntry = getpwuid(getuid());

	sprintf(&vbsDir[0], "%s/%s", passwdEntry->pw_dir, VBS_CONFIG_DIR);
	statRes = stat(vbsDir, &statBuf);
	errsv = errno;
	if (statRes == 0) {
		// Is it a dir?
		if (!S_ISDIR(statBuf.st_mode)) {errorHandler("main",".vbs exists in home dir, but is not a directory");}
	}
	else if (statRes == -1) {
		// Create if missing
		if (errsv == ENOENT) {
			mkdirRes = mkdir(vbsDir,0755);
			if (mkdirRes == 1) {errorHandler("main",".vbs creation failed");}
		}
		else {errorHandler("main","stat of .vbs failed");}
	}

	// Check for config, if not, create one
	sprintf(&counter.configFileName[0], "%s/%s", vbsDir, VBS_CONFIG_FILENAME);
	statRes = stat(counter.configFileName, &statBuf);
	errsv = errno;

	if (statRes == 0) {
		// Is it a file?
		if (!S_ISREG(statBuf.st_mode)) {errorHandler("main","config file exists in .vbs, but is not a regular file");}
		else {fpConfig = fopen(counter.configFileName, "r");}
		// Read from config file
		char *line;
		line = malloc(256);
		if (!line) {errorHandler("main","malloc failed");}

		while (fgets(line, 255, fpConfig)) {
			if (!(line[0]=='#')) {
				line[strlen(line) - 1] = 0;     /* kill '\n' */
				if (strstr(line, "EXPORT_CR")) {counter.config_export_cr = configBool(line);}
				if (strstr(line, "EXPORT_ENCODING")) {configChar(line, &counter.config_export_encoding[0]);}
				if (strstr(line, "IMPORT_ENCODING")) {configChar(line, &counter.config_import_encoding[0]);}
			}
		}
		fclose(fpConfig);
	}
	else if (statRes == -1) {
		// Create if missing
		if (errsv == ENOENT) {writeConfig(TRUE);}
		else {errorHandler("main","stat of config file failed");}
	}

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

	counter.status = status;
	counter.status_context_id = status_context_id;
	counter.inside_sub = FALSE;
	sprintf(counter.globalExportFile, "%s", VBS_DEFAULT_EXPORT_FILE);

	// Progress, will be packed in the middle of the vbox
	progress = gtk_progress_bar_new();
	gtk_progress_bar_set_orientation(GTK_PROGRESS_BAR(progress),GTK_PROGRESS_LEFT_TO_RIGHT);
	counter.progress = progress;

	// Root window
	window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title (GTK_WINDOW (window), "Voody Blue Subtitler");
	gtk_window_set_default_icon_from_file (VBS_ICON, NULL);
	gtk_widget_set_size_request (window, 500, 400);
	g_signal_connect (window, "delete_event", G_CALLBACK(quitDialog), window);

	// Link double-click event
	g_signal_connect(view, "row-activated", (GCallback) view_onRowActivated, window);

	// Key events
	g_signal_connect(view, "key_press_event", (GCallback) view_onKeyPressed, window);

	// Menu
	GtkWidget *menu;
	menu = makeMenu(window);

	// Create vbox
	vbox = gtk_vbox_new (FALSE, 0);

	// Pack entry & scroll into vbox
	gtk_box_pack_start(GTK_BOX(vbox), menu, FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(vbox), status, FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(vbox), progress, FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(vbox), scroll, TRUE, TRUE, 0);

	// Add vbox to window
	gtk_container_add (GTK_CONTAINER (window), vbox);

	gtk_widget_show_all (window);

	// Progress bar check & update function
	g_timeout_add (1000, (GtkFunction) progressBarUpdate, NULL);

	gtk_main ();

	return 0;
}

