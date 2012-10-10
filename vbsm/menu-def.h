// This file is a part of Voody Blue Subtitler suit.
// 
// Author: Assen Totin <assen.totin@gmail.com>
//
// Home page: http://bilbo.online.bg/~assen/vbs
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
        {"FileQuit", NULL, "Quit", "<CTRL>Q", NULL, (GCallback)quitDialog},

        {"EditMenu", NULL, "Edit", NULL, NULL, NULL},
        {"EditInsertBefore", NULL, "Insert Before", "<CTRL>B", NULL, (GCallback)insertBefore},
        {"EditInsertAfter", NULL, "Insert After", "<CTRL>N", NULL, (GCallback)insertBefore},
        {"EditDelete", NULL, "Delete", "<CTRL>D", NULL, (GCallback)insertBefore},
        {"EditSeparator1", NULL, NULL, NULL, NULL, NULL},
        {"EditZeroCurent", NULL, "Zero Timing for Current", "<CTRL>Z", NULL, (GCallback)zeroTiming},
        {"EditZeroRest", NULL, "Zero Timing from Current", NULL, NULL, (GCallback)zeroTiming},
        {"EditSeparator2", NULL, NULL, NULL, NULL, NULL},
        {"EditStartTime", NULL, "Set Start Time", NULL, NULL, (GCallback)setTimer},

        {"TextMenu", NULL, "Subtitles", NULL, NULL, NULL},
        {"TextImportPlain", NULL, "Load Plain Text...", "<CTRL>T", NULL, (GCallback)fileDialogOpen},
        {"TextImportSubrip", NULL, "Load SubRip...", "<CTRL>R", NULL, (GCallback)fileDialogOpen},
        {"TextImportEncoding", NULL, "Set Import Encoding...", NULL, NULL, (GCallback)setEncodingImport},
        {"TextSeparator1", NULL, NULL, NULL, NULL, NULL},
        {"TextExportDestination", NULL, "Export Destination...", "<CTRL>E", NULL, (GCallback)fileDialogSave},
        {"TextExportEncoding", NULL, "Set Export Encoding...", NULL, NULL, (GCallback)setEncodingExport},
        {"TextExportNewline", NULL, "Set Export Newline...", NULL, NULL, (GCallback)setNewline},

        {"VideoMenu", NULL, "Video", NULL, NULL, NULL},
        {"VideoImport", NULL, "Load Video...", "<CTRL>V", NULL, (GCallback)fileDialogOpen},
        {"VideoBackend", NULL, "Set Video Backend...", NULL, NULL, (GCallback)set_video_backend},
        {"VideoOutput", NULL, "Set Video Output...", NULL, NULL, (GCallback)set_video_output},

        {"NetworkMenu", NULL, "Network", NULL, NULL, NULL},
        {"NetworkMode", NULL, "Enable/Disable...", NULL, NULL, (GCallback)useNetwork},
        {"NetworkServer", NULL, "Set Server...", NULL, NULL, (GCallback)setNetworkServer},
        {"NetworkPort", NULL, "Set Port...", NULL, NULL, (GCallback)setNetworkPort},
	{"NetworkMagic", NULL, "Set Magic Key...", NULL, NULL, (GCallback)set_magic_key},

        {"HelpMenu", NULL, "Help", NULL, NULL, NULL},
        {"HelpContents", NULL, "Contents...", NULL, NULL, (GCallback)helpContents},
        {"HelpAbout", NULL, "About...", NULL, NULL, (GCallback)helpAbout}
};

