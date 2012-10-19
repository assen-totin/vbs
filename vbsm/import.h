// This file is a part of Voody Blue Subtitler suit.
// 
// Author: Assen Totin <assen.totin@gmail.com>
//
// Home page: http://bilbo.online.bg/~assen/vbs
//
// This software is released under GNU General Public License.
// See the LICENSE file for details or visit http://www.gnu.org/copyleft/gpl.html 
// for details.

void clear_store();

void import_text(char *, int);

void import_filter_srt(char *);

unsigned int convert_time_sec(char *);

bool have_loaded_text(GtkWidget *);

