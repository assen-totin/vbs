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

#include "menu-def.h"

int main (int argc, char **argv){
	GtkWidget *status, *vbox, *progress;

        // Check for alternative config
        get_cmdl_config(argc, argv);

        // Set up config from defaults
        check_config(0);
	can_recv_from_net = 1;

	// GTK Init
	gtk_init (&argc, &argv);

	// Root window
	config.vbsm.window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title (GTK_WINDOW (config.vbsm.window), "Voody Blue Subtitler Configurator");
	gtk_window_set_default_icon_from_file (VBS_ICON, NULL);
	gtk_widget_set_size_request (config.vbsm.window, 400, 100);
	g_signal_connect (config.vbsm.window, "delete_event", G_CALLBACK(quit_dialog), config.vbsm.window);

	// Menu
	GtkWidget *menu = make_menu(ui1, menu_entries1);

	// Main label
	GtkWidget *label = gtk_label_new(VBSC_WELCOME_TEXT);

	// Create vbox
	vbox = gtk_vbox_new (FALSE, 0);

	// Pack entries into vbox
	gtk_box_pack_start(GTK_BOX(vbox), menu, FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(vbox), label, FALSE, FALSE, 0);

	// Add vbox to window
	gtk_container_add(GTK_CONTAINER (config.vbsm.window), vbox);

	gtk_widget_show_all(config.vbsm.window);

	gtk_main();

	return 0;
}

