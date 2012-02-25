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
	GtkWidget *window, *scroll;
	GtkWidget *status, *vbox, *progress;

        // Check for alternative config
        get_cmdl_config(argc, argv);

        // Set up config from defaults
        check_config(0);

	// GTK Init
	gtk_init (&argc, &argv);

	// Root window
	window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title (GTK_WINDOW (window), "Voody Blue Subtitler Configurator");
	gtk_window_set_default_icon_from_file (VBS_ICON, NULL);
	gtk_widget_set_size_request (window, 400, 100);
	g_signal_connect (window, "delete_event", G_CALLBACK(quit_dialog), window);

	// Menu
	GtkWidget *menu = makeMenu(window, &menu_entries[0], VBSC_MENU_COUNT);

	// Main label
	GtkWidget *label = gtk_label_new(VBSC_WELCOME_TEXT);

	// Create vbox
	vbox = gtk_vbox_new (FALSE, 0);

	// Pack entry & scroll into vbox
	gtk_box_pack_start(GTK_BOX(vbox), menu, FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(vbox), label, FALSE, FALSE, 0);

	// Add vbox to window
	gtk_container_add(GTK_CONTAINER (window), vbox);

	gtk_widget_show_all(window);

	gtk_main();

	return 0;
}

