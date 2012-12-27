// This file is a part of Voody Blue Subtitler suit.
// 
// Author: Assen Totin <assen.totin@gmail.com>
//
// Home page: http://www.zavedil.com/software-desktop-vbs
//
// This software is released under GNU General Public License.
// See the LICENSE file for details or visit http://www.gnu.org/copyleft/gpl.html 
// for details.

gchar *ui = 
	"<ui>"
	"  <menubar name='MainMenu'>"
	"    <menu action='FileMenu'>"
	"      <menuitem action='FileQuit'/>"
	"    </menu>"
	"    <menu action='EditMenu'>"
	"      <menuitem action='EditInsertBefore'/>"
	"      <menuitem action='EditInsertAfter'/>"
	"      <menuitem action='EditDelete'/>"
	"      <separator name='EditSeparator1'/>"
	"      <menuitem action='EditZeroCurent'/>"
	"      <menuitem action='EditZeroRest'/>"
	"      <separator name='EditSeparator2'/>"
	"      <menuitem action='EditShiftSame'/>"
	"      <menuitem action='EditShiftExpand'/>"
	"      <separator name='EditSeparator3'/>"
	"      <menuitem action='EditStartTime'/>"
	"    </menu>"
	"    <menu action='TextMenu'>"
	"      <menuitem action='TextImportPlain'/>"
	"      <menuitem action='TextImportSubrip'/>"
	"      <menuitem action='TextImportEncoding'/>"
	"      <separator name='TextSeparator1'/>"
	"      <menuitem action='TextExportDestination'/>"
	"      <menuitem action='TextExportEncoding'/>"
	"      <menuitem action='TextExportNewline'/>"
	"    </menu>"
#ifdef HAVE_GSTREAMER
	"    <menu action='VideoMenu'>"
	"      <menuitem action='VideoImport'/>"
	"      <menuitem action='VideoBackend'/>"
	"      <menuitem action='VideoOutput'/>"
	"    </menu>"
#elif HAVE_MPLAYER
	"    <menu action='VideoMenu'>"
	"      <menuitem action='VideoImport'/>"
	"      <menuitem action='VideoBackend'/>"
	"    </menu>"
#endif
	"    <menu action='NetworkMenu'>"
	"      <menuitem action='NetworkMode'/>"
	"      <menuitem action='NetworkServer'/>"
	"      <menuitem action='NetworkPort'/>"
	"      <menuitem action='NetworkMagic'/>"
	"    </menu>"
	"    <menu action='HelpMenu'>"
	"      <menuitem action='HelpContents'/>"
	"      <menuitem action='HelpAbout'/>"
	"    </menu>"
	"  </menubar>"
	"</ui>";

GtkActionEntry menu_entries[] = {
	{"FileMenu", NULL, "File", NULL, NULL, NULL},
	{"FileQuit", GTK_STOCK_QUIT, "Quit", "<CTRL>Q", NULL, (GCallback)quitDialog},

	{"EditMenu", NULL, "Edit", NULL, NULL, NULL},
	{"EditInsertBefore", GTK_STOCK_ADD, "Insert Before", "Insert", NULL, (GCallback)insert_subtitle},
	{"EditInsertAfter", GTK_STOCK_ADD, "Insert After", "<CTRL>N", NULL, (GCallback)insert_subtitle},
	{"EditDelete", GTK_STOCK_DELETE, "Delete", NULL, NULL, (GCallback)insert_subtitle},
	{"EditSeparator1", NULL, NULL, NULL, NULL, NULL},
	{"EditZeroCurent", NULL, "Zero Timing for Current", NULL, NULL, (GCallback)zero_timing},
	{"EditZeroRest", NULL, "Zero Timing from Current", "<CTRL>0", NULL, (GCallback)zero_timing},
	{"EditSeparator2", NULL, NULL, NULL, NULL, NULL},
	{"EditShiftSame", NULL, "Shift Timing for Current", NULL, NULL, (GCallback)shift_timing},
	{"EditShiftExpand", NULL, "Expand Timing from Current", NULL, NULL, (GCallback)shift_timing},
	{"EditSeparator3", NULL, NULL, NULL, NULL, NULL},
	{"EditStartTime", NULL, "Set Start Time", NULL, NULL, (GCallback)set_timer},

	{"TextMenu", NULL, "Subtitles", NULL, NULL, NULL},
	{"TextImportPlain", GTK_STOCK_OPEN, "Load Plain Text...", "<CTRL>T", NULL, (GCallback)fileDialogOpen},
	{"TextImportSubrip", GTK_STOCK_OPEN, "Load SubRip...", "<CTRL>R", NULL, (GCallback)fileDialogOpen},
	{"TextImportEncoding", GTK_STOCK_CONVERT, "Set Import Encoding...", NULL, NULL, (GCallback)set_encoding_import},
	{"TextSeparator1", NULL, NULL, NULL, NULL, NULL},
	{"TextExportDestination", GTK_STOCK_SAVE_AS, "Export Destination...", "<CTRL>E", NULL, (GCallback)fileDialogSave},
	{"TextExportEncoding", GTK_STOCK_CONVERT, "Set Export Encoding...", NULL, NULL, (GCallback)set_encoding_export},
	{"TextExportNewline", NULL, "Set Export Newline...", NULL, NULL, (GCallback)set_newline},

	{"VideoMenu", NULL, "Video", NULL, NULL, NULL},
	{"VideoImport", GTK_STOCK_OPEN, "Load Video...", NULL, NULL, (GCallback)fileDialogOpen},
	{"VideoBackend", NULL, "Set Video Backend...", NULL, NULL, (GCallback)set_video_backend},
	{"VideoOutput", NULL, "Set Video Output...", NULL, NULL, (GCallback)set_video_output},

	{"NetworkMenu", NULL, "Network", NULL, NULL, NULL},
	{"NetworkMode", GTK_STOCK_NETWORK, "Enable/Disable...", NULL, NULL, (GCallback)use_network},
	{"NetworkServer", NULL, "Set Server...", NULL, NULL, (GCallback)set_network_server},
	{"NetworkPort", NULL, "Set Port...", NULL, NULL, (GCallback)set_network_port},
	{"NetworkMagic", NULL, "Set Magic Key...", NULL, NULL, (GCallback)set_magic_key},

	{"HelpMenu", NULL, "Help", NULL, NULL, NULL},
	{"HelpContents", GTK_STOCK_HELP, "Contents...", NULL, NULL, (GCallback)help_contents},
	{"HelpAbout", GTK_STOCK_ABOUT, "About...", NULL, NULL, (GCallback)help_about}
};

