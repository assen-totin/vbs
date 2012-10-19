// This file is a part of Voody Blue Subtitler suit.
// 
// Author: Assen Totin <assen.totin@gmail.com>
//
// Home page: http://bilbo.online.bg/~assen/vbs
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
        "    <menu action='TextMenu'>"
        "      <menuitem action='TextImportDefault'/>"
        "      <menuitem action='TextImportEncoding'/>"
        "      <separator name='TextSeparator1'/>"
        "      <menuitem action='TextExportDefault'/>"
        "      <menuitem action='TextExportEncoding'/>"
        "      <menuitem action='TextExportNewline'/>"
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
        "    </menu>"
        "    <menu action='HelpMenu'>"
        "      <menuitem action='HelpAbout'/>"
        "    </menu>"
        "  </menubar>"
        "</ui>";

GtkActionEntry menu_entries1[] = {
        {"FileMenu", NULL, "File", NULL, NULL, NULL},
        {"FileQuit", GTK_STOCK_QUIT, "Quit", "<CTRL>Q", NULL, (GCallback)quit_dialog},

        {"TextMenu", NULL, "Subtitles", NULL, NULL, NULL},
        {"TextImportDefault", GTK_STOCK_OPEN, "Load Plain Text...", "<CTRL>T", NULL, (GCallback)file_dialog_open},
        {"TextImportEncoding", GTK_STOCK_CONVERT, "Set Import Encoding...", NULL, NULL, (GCallback)setEncodingImport},
        {"TextSeparator1", NULL, NULL, NULL, NULL, NULL},
        {"TextExportDefault", GTK_STOCK_OPEN, "Export Destination...", "<CTRL>E", NULL, (GCallback)file_dialog_save},
        {"TextExportEncoding", GTK_STOCK_CONVERT, "Set Export Encoding...", NULL, NULL, (GCallback)setEncodingExport},
        {"TextExportNewline", NULL, "Set Export Newline...", NULL, NULL, (GCallback)setNewline},

        {"NetworkMenu", NULL, "Network", NULL, NULL, NULL},
        {"NetworkMode", GTK_STOCK_NETWORK, "Enable/Disable...", NULL, NULL, (GCallback)useNetwork},
        {"NetworkServer", NULL, "Set Server...", NULL, NULL, (GCallback)setNetworkServer},
        {"NetworkPort", NULL, "Set Port...", NULL, NULL, (GCallback)setNetworkPort},
        {"NetworkMagic", NULL, "Set Magic Key...", NULL, NULL, (GCallback)set_magic_key},

        {"PlayerMenu", NULL, "Player", NULL, NULL, NULL},
        {"PlayerFullscreen", GTK_STOCK_FULLSCREEN, "Full Screent...", NULL, NULL, (GCallback)set_full_screen},
        {"PlayerFont", NULL, "Font Size...", NULL, NULL, (GCallback)set_font_size},

        {"HelpMenu", NULL, "Help", NULL, NULL, NULL},
        {"HelpAbout", GTK_STOCK_ABOUT, "Contents...", NULL, NULL, (GCallback)help_about}
};

