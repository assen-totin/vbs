// This file is a part of Voody Blue Subtitler suit.
// 
// Author: Assen Totin <assen.totin@gmail.com>
//
// Home page: http://www.zavedil.com/software-desktop-vbs
//
// This software is released under GNU General Public License.
// See the LICENSE file for details or visit http://www.gnu.org/copyleft/gpl.html 
// for details.

struct exportSub {
	FILE *fp_export;
	FILE *fp_mplayer;
	short unsigned count;
};

gboolean export_subtitles_srt(GtkTreeModel *, GtkTreePath *, GtkTreeIter *, gpointer);

void export_subtitles();

