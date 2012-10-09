// This file is a part of Voody Blue Subtitler suit.
// 
// Author: Assen Totin <assen.totin@gmail.com>
//
// Home page: http://bilbo.online.bg/~assen/vbs
//
// This software is released under GNU General Public License.
// See the LICENSE file for details or visit http://www.gnu.org/copyleft/gpl.html 
// for details.

void quitDialogCancel(GtkWidget *widget, gpointer data);
void helpAbout(GtkAction *action, gpointer param);
void setEncodingImportOK(GtkWidget *widget, gpointer data);
void setEncodingExportOK(GtkWidget *widget, gpointer data);
void setEncodingImport (GtkAction *action, gpointer param);
void setEncodingExport (GtkAction *action, gpointer param);
void setNewlineOK(GtkWidget *widget, gpointer data);
void setNewline (GtkAction *action, gpointer param);
void setNetworkPortOK(GtkWidget *widget, gpointer data);
void setNetworkPort (GtkAction *action, gpointer param);
void setNetworkServerOK(GtkWidget *widget, gpointer data);
void setNetworkServer (GtkAction *action, gpointer param);
void useNetworkOK(GtkWidget *widget, gpointer data);
void useNetwork (GtkAction *action, gpointer param);

#define VBS_MENU_DEFAULT_PATH "~/Desktop"

#define VBS_MENU_ABOUT_TITLE "About Voody Blue Subtitler"
#define VBS_MENU_ABOUT_TEXT "\nVoody Blue Subtitler - Master Client\n\nVersion: 2.0\n\nAuthor: Assen Totin <assen.totin@gmail.com>\n\nProgramme home page: http://bilbo.online.bg/~assen/vbs\n\nThis programme is distributed under GNU General Public License.\nSee the LICENSE file from the distribution or visit\nhttp://www.gnu.org/copyleft/gpl.html for details.\n"
#define VBS_MENU_ABOUT_TEXT2 "\nIf you like this program, send me a postcard from your place to:\n\nBulgaria\n1303 Sofia\n74, Otec Paisi Str.\nAssen Totin\n"

#define VBS_MENU_ENCODING_TITLE "Available Encodings:"
#define VBS_MENU_ENCODING_IMPORT "Select Import Encoding"
#define VBS_MENU_ENCODING_EXPORT "Select Export Encoding"

#define VBS_MENU_NEWLINE_TITLE "Select Newline Delimiter"
#define VBS_MENU_NEWLINE_TEXT "Available Delimiters:"

#define VBS_MENU_NETWORK_PORT_TITLE "TCP Port"
#define VBS_MENU_NETWORK_PORT_TEXT "Current Port:"
#define VBS_MENU_NETWORK_SERVER_TITLE "Server Name"
#define VBS_MENU_NETWORK_SERVER_TEXT "Current server (FQDN):"
#define VBS_MENU_DNS_WARNING_TITLE "Press OK to change server."
#define VBS_MENU_DNS_WARNING_TEXT "DNS resolving failed."
#define VBS_MENU_ENABLE_NETWORK_TITLE "Network Mode"
#define VBS_MENU_ENABLE_NETWORK_TEXT "Select network mode:"

