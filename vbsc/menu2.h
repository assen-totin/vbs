// This file is a part of Voody Blue Subtitler suit.
// 
// Author: Assen Totin <assen.totin@gmail.com>
//
// Home page: http://bilbo.online.bg/~assen/vbs
//
// This software is released under GNU General Public License.
// See the LICENSE file for details or visit http://www.gnu.org/copyleft/gpl.html 
// for details.

void fileDialogOK21( GtkWidget *fileDialogWidget, GtkFileSelection *fs );
void fileDialogOK41( GtkWidget *fileDialogWidget, GtkFileSelection *fs );

void fileDialog(gpointer callback_data, guint callback_action, GtkWidget *widget);

void quitDialogCancel( GtkWidget *widget, gpointer data );
void quitDialog(GtkWidget *window);

void helpContents(GtkWidget *window);

void helpAbout(GtkWidget *window);

void setEncodingImport(GtkWidget *window);
void setEncodingExport(GtkWidget *window);
void setEncodingImportOK(GtkWidget *widget, gpointer data);
void setEncodingExportOK(GtkWidget *widget, gpointer data);

void setNewline (GtkWidget *window);
void setNewlineOK(GtkWidget *widget, gpointer data);

void useNetworkOK(GtkWidget *widget, gpointer data);
void useNetwork (GtkWidget *window);
void setNetworkServerOK(GtkWidget *widget, gpointer data);
void setNetworkServer (GtkWidget *window);
void setNetworkPortOK(GtkWidget *widget, gpointer data);
void setNetworkPort (GtkWidget *window);
void setTimer(GtkWidget *window);

static GtkItemFactoryEntry menuEntries[] = {
	{ "/_File",         "<ALT>F",      NULL,         0, "<Branch>" },
	{ "/File/_Quit", "<CTRL>Q", quitDialog,  11, "<Item>" },

	{ "/_Text",         "<ALT>T",      NULL,         0, "<Branch>" },
	{ "/Text/Default File...", "<CTRL>T", fileDialog,  21, "<Item>" },
	{ "/Text/Set Encoding...", "", setEncodingImport,  23, "<Item>" },

	{ "/_Export",        "<ALT>E",      NULL,         0, "<Branch>" },
	{ "/Export/Default File...", "<CTRL>E", fileDialog,  41, "<Item>" },
	{ "/Export/Set Encoding...", "", setEncodingExport,  42, "<Item>" },
	{ "/Export/Set Newline Char...", "", setNewline,  43, "<Item>" },

        { "/_Network",        "<ALT>N",      NULL,         0, "<Branch>" },
	{ "/Network/Enable Network...", "", useNetwork,  71, "<Item>" },
        { "/Network/Server...", "", setNetworkServer,  72, "<Item>" },
        { "/Network/Port...", "", setNetworkPort,  73, "<Item>" },

	{ "/_Help",        "<ALT>H",      NULL,         0, "<Branch>" },
	{ "/Help/Contents", "", helpContents,  51, "<Item>" },
	{ "/Help/About", "", helpAbout,  52, "<Item>" }
};


GtkWidget *makeMenu(GtkWidget *window);

#define VBS_DEFAULT_PATH "~/Desktop"

#define VBS_IMPORT_TEXTONLY_TITLE "Select Default File"

#define VBS_EXPORT_TITLE "Select Default File"

#define VBS_AVAILABLE_ENCODINGS "Available Encodings:"
#define VBS_SET_ENCODING_IMPORT "Select Import Encoding"
#define VBS_SET_ENCODING_EXPORT "Select Export Encoding"

#define VBS_SET_NEWLINE "Select Newline Delimiter"
#define VBS_AVAILABLE_NEWLINES "Available Delimiters:"

#define VBS_NETWORK_SERVER "Server Name"
#define VBS_NETWORK_SERVER_NAME "Current server:"
#define VBS_NETWORK_SERVER_WARNING "Press OK to change server."
#define VBS_NETWORK_SERVER_WARNING_TEXT "DNS resolving failed."
#define VBS_NETWORK_PORT "TCP Port"
#define VBS_NETWORK_PORT_NAME "Current Port:"
#define VBS_ENABLE_NETWORK "Enable/Disable Network"
#define VBS_ENABLE_NETWORK_NAME "Select network mode:"

#define VBS_QUIT_CONFIRM "Really quit?"
#define VBS_QUIT_MSG "Your subtitles are exported to:"

#define VBS_HELP_ABOUT_TITLE "About Voody Blue Subtitler"
#define VBS_HELP_ABOUT_MSG "\nVoody Blue Subtitler - Master Client\n\nVersion: 2.0\n\nAuthor: Assen Totin <assen.totin@gmail.com>\n\nProgramme home page: http://bilbo.online.bg/~assen/vbs\n\nThis programme is distributed under GNU General Public License.\nSee the LICENSE file from the distribution or visit\nhttp://www.gnu.org/copyleft/gpl.html for details.\n"
#define VBS_HELP_ABOUT_CARD "\nIf you like this program, send me a postcard from your place to:\n\nBulgaria\n1574 Sofia\n74, Otec Paisi Str.\nASSEN TOTIN\n"

#define VBS_HELP_CONTENTS_TITLE "Using Voody Blue Subtitler"
#define VBS_HELP_CONTENTS_MSG "\n1. Use the Text menu to import a text-only or a SubRip file\nand to specify its encoding.\n\n2. Use the Video menu to load video file.\n\n3. Use the Export menu to set an export destination\n(default is /tmp/vbs_export.srt) and to set export encoding\n and newline character.\n\n4. Use the Edit menu to add new subtitle or delete one\n as well as to zero counters.\n\n5. Control keys:\n* spacebar - toggles play/pause\n* b - enter next subtitle\n* m - exit current subtitle\n* n - exit current and enter next\n* s - save immediately\n\n6. Mouse controls:\n* Double click start or end time to go to subtitle\n* Double click text to edit it\n"
