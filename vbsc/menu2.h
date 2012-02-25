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

void set_font_size (GtkWidget *window);
void set_font_size_ok(GtkWidget *widget, gpointer data);
void set_magic_key (GtkWidget *window);
void set_magic_key_ok(GtkWidget *widget, gpointer data);
void set_full_screen (GtkWidget *window);
void set_full_screen_ok(GtkWidget *widget, gpointer data);
void use_network_2 (GtkWidget *window);
void use_network_2_ok(GtkWidget *widget, gpointer data);

#define VBSC_MENU_COUNT 20

#define VBSC_MENU_MAGIC_KEY_TITLE "Set Magic Key"
#define VBSC_MENU_MAGIC_KEY_TEXT "Enter magic key (a number between 1 and 2147483647):"
#define VBSC_MENU_FONT_SIZE_TITLE "Set Font Size"
#define VBSC_MENU_FONT_SIZE_TEXT "Enter font size (in points):"
#define VBSC_MENU_FULL_SCREEN_TITLE "Set Full-Screen"
#define VBSC_MENU_FULL_SCREEN_TEXT "Full-Screen mode:"
#define VBSC_MENU_RECV_TITLE "Set Receive From Network"
#define VBSC_MENU_RECV_TEXT "Receive from network: "
 
static GtkItemFactoryEntry menu_entries[VBSC_MENU_COUNT] = {
	{ "/_File",         "<ALT>F",      NULL,         0, "<Branch>" },
	{ "/File/_Quit", "<CTRL>Q", quit_dialog,  11, "<Item>" },

	{ "/Text _Import",         "<ALT>I",      NULL,         0, "<Branch>" },
	{ "/Text Import/Default File...", "", file_dialog,  21, "<Item>" },
	{ "/Text Import/Set Encoding...", "", setEncodingImport,  23, "<Item>" },

	{ "/Text _Export",        "<ALT>E",      NULL,         0, "<Branch>" },
	{ "/Text Export/Default File...", "", file_dialog,  41, "<Item>" },
	{ "/Text Export/Set Encoding...", "", setEncodingExport,  42, "<Item>" },
	{ "/Text Export/Set Newline Char...", "", setNewline,  43, "<Item>" },

        { "/_Network",        "<ALT>N",      NULL,         0, "<Branch>" },
	{ "/Network/Send to Network...", "", useNetwork,  71, "<Item>" },
	{ "/Network/Receive from Network...", "", use_network_2,  75, "<Item>" },
        { "/Network/Server...", "", setNetworkServer,  72, "<Item>" },
        { "/Network/Port...", "", setNetworkPort,  73, "<Item>" },
	{ "/Network/Magic Key...", "", set_magic_key,  74, "<Item>" },

	{ "/_Player",        "<ALT>P",      NULL,         0, "<Branch>" },
	{ "/Player/Full Screen...", "", set_full_screen,  81, "<Item>" },
	{ "/Player/Font Size...", "", set_font_size,  82, "<Item>" },

	{ "/_Help",        "<ALT>H",      NULL,         0, "<Branch>" },
	{ "/Help/About", "", helpAbout,  52, "<Item>" }
};


