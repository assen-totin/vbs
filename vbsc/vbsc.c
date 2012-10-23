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

	// i18n
	setlocale (LC_ALL, "");
	bindtextdomain (PACKAGE_NAME, LOCALEDIR);
	bind_textdomain_codeset(PACKAGE_NAME, "utf-8");
	textdomain (PACKAGE_NAME);
	
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
	// Only the GTK+ idiots know why menu cannot be built in a function and returned as a widget -
	// like the "deprecated" GtkItemFactoryEntry seamlesly did
	GtkUIManager *p_uiManager = gtk_ui_manager_new();
	GtkActionGroup *p_actionGroup = gtk_action_group_new ("menuActionGroup");
	gtk_action_group_add_actions (p_actionGroup, menu_entries1, G_N_ELEMENTS (menu_entries1), NULL);
	gtk_ui_manager_insert_action_group (p_uiManager, p_actionGroup, 0);
	gtk_ui_manager_add_ui_from_string (p_uiManager, ui1, -1, NULL);
	GtkWidget *menu = gtk_ui_manager_get_widget(p_uiManager, "/MainMenu");
	GtkAccelGroup *accel_group = gtk_ui_manager_get_accel_group(p_uiManager);
	gtk_window_add_accel_group(GTK_WINDOW(config.vbsm.window), accel_group);

	// Main label
	GtkWidget *label = gtk_label_new(_("Use the menu to change default settings."));

	// Create vbox
#ifdef HAVE_GTK2
	vbox = gtk_vbox_new (FALSE, 0);
#elif HAVE_GTK3
	vbox = gtk_box_new( GTK_ORIENTATION_VERTICAL, 0);
#endif

	// Pack entries into vbox
	gtk_box_pack_start(GTK_BOX(vbox), menu, FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(vbox), label, FALSE, FALSE, 0);

	// Add vbox to window
	gtk_container_add(GTK_CONTAINER (config.vbsm.window), vbox);

	gtk_widget_show_all(config.vbsm.window);

	gtk_main();

	return 0;
}

