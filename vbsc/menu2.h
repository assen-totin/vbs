// This file is a part of Voody Blue Subtitler suit.
// 
// Author: Assen Totin <assen.totin@gmail.com>
//
// Home page: http://bilbo.online.bg/~assen/vbs
//
// This software is released under GNU General Public License.
// See the LICENSE file for details or visit http://www.gnu.org/copyleft/gpl.html 
// for details.

void file_dialog(gpointer callback_data, guint callback_action, GtkWidget *widget);
void file_dialog_ok_21( GtkWidget *fileDialogWidget, GtkFileSelection *fs );
void file_dialog_ok_41( GtkWidget *fileDialogWidget, GtkFileSelection *fs );

void quit_dialog(GtkWidget *window);
static void quit_dialog_ok( GtkWidget *widget, gpointer data );

#define VBSC_MENU_COUNT 15
static GtkItemFactoryEntry menu_entries[VBSC_MENU_COUNT] = {
	{ "/_File",         "<ALT>F",      NULL,         0, "<Branch>" },
	{ "/File/_Quit", "<CTRL>Q", quit_dialog,  11, "<Item>" },

	{ "/_Text",         "<ALT>T",      NULL,         0, "<Branch>" },
	{ "/Text/Default File...", "<CTRL>T", file_dialog,  21, "<Item>" },
	{ "/Text/Set Encoding...", "", setEncodingImport,  23, "<Item>" },

	{ "/_Export",        "<ALT>E",      NULL,         0, "<Branch>" },
	{ "/Export/Default File...", "<CTRL>E", file_dialog,  41, "<Item>" },
	{ "/Export/Set Encoding...", "", setEncodingExport,  42, "<Item>" },
	{ "/Export/Set Newline Char...", "", setNewline,  43, "<Item>" },

        { "/_Network",        "<ALT>N",      NULL,         0, "<Branch>" },
	{ "/Network/Enable Network...", "", useNetwork,  71, "<Item>" },
        { "/Network/Server...", "", setNetworkServer,  72, "<Item>" },
        { "/Network/Port...", "", setNetworkPort,  73, "<Item>" },

	{ "/_Help",        "<ALT>H",      NULL,         0, "<Branch>" },
	{ "/Help/About", "", helpAbout,  52, "<Item>" }
};


