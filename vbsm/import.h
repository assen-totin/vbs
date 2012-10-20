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

int import_subtitles_text(char *, struct subtitle_srt *);

void import_subtitles(char *, int);

bool have_loaded_text(GtkWidget *);

