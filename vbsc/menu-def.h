// This file is a part of Voody Blue Subtitler suit.
// 
// Author: Assen Totin <assen.totin@gmail.com>
//
// Home page: http://www.zavedil.com/software-desktop-vbs
//
// This software is released under GNU General Public License.
// See the LICENSE file for details or visit http://www.gnu.org/copyleft/gpl.html 
// for details.

gchar *ui1 =
	"<ui>"
	"  <menubar name='MainMenu'>"
	"    <menu action='FileMenu'>"
	"      <menuitem action='FileQuit'/>"
	"    </menu>"
	"    <menu action='EditMenu'>"
	"      <menuitem action='EditImportDefault'/>"
	"      <menuitem action='EditImportEncoding'/>"
	"    </menu>"
	"    <menu action='NetworkMenu'>"
	"      <menuitem action='NetworkMode'/>"
	"      <menuitem action='NetworkServer'/>"
	"      <menuitem action='NetworkPort'/>"
	"      <menuitem action='NetworkMagic'/>"
	"    </menu>"
	"    <menu action='PlayerMenu'>"
	"      <menuitem action='PlayerFullscreen'/>"
	"      <menuitem action='PlayerFont'/>"
	"      <menuitem action='PlayerFontColour'/>"
	"      <menuitem action='PlayerBgColour'/>"
	"      <menuitem action='PlayerJustify'/>"
	"    </menu>"
	"    <menu action='HelpMenu'>"
	"      <menuitem action='HelpAbout'/>"
	"    </menu>"
	"  </menubar>"
	"</ui>";

GtkActionEntry menu_entries1[] = {
	{"FileMenu", NULL, "File", NULL, NULL, NULL},
	{"FileQuit", GTK_STOCK_QUIT, "Quit", "<CTRL>Q", NULL, (GCallback)quit_dialog},

	{"EditMenu", NULL, "Subtitles", NULL, NULL, NULL},
	{"EditImportDefault", GTK_STOCK_OPEN, "Select Subtitles...", "<CTRL>T", NULL, (GCallback)file_dialog_open},
	{"EditImportEncoding", GTK_STOCK_CONVERT, "Set Import Encoding...", NULL, NULL, (GCallback)set_encoding_import},

	{"NetworkMenu", NULL, "Network", NULL, NULL, NULL},
	{"NetworkMode", GTK_STOCK_NETWORK, "Enable/Disable...", NULL, NULL, (GCallback)use_network},
	{"NetworkServer", NULL, "Set Server...", NULL, NULL, (GCallback)set_network_server},
	{"NetworkPort", NULL, "Set Port...", NULL, NULL, (GCallback)set_network_port},
	{"NetworkMagic", NULL, "Set Magic Key...", NULL, NULL, (GCallback)set_magic_key},

	{"PlayerMenu", NULL, "Player", NULL, NULL, NULL},
	{"PlayerFullscreen", GTK_STOCK_FULLSCREEN, "Full Screen...", NULL, NULL, (GCallback)set_full_screen},
	{"PlayerFont", NULL, "Font Size...", NULL, NULL, (GCallback)select_font},
	{"PlayerFontColour", NULL, "Font Colour...", NULL, NULL, (GCallback)select_colour},
	{"PlayerBgColour", NULL, "Font Background Colour...", NULL, NULL, (GCallback)select_colour},
	{"PlayerJustify", NULL, "Font Justify...", NULL, NULL, (GCallback)select_justify},

	{"HelpMenu", NULL, "Help", NULL, NULL, NULL},
	{"HelpAbout", GTK_STOCK_ABOUT, "Contents...", NULL, NULL, (GCallback)help_about}
};

