// This file is a part of Voody Blue Subtitler.
//
// Author: Assen Totin <assen@online.bg>
//
// Home page: http://bilbo.online.bg/~assen/vbs
//
// This software is released under GNU General Public License.
// See the LICENSE file for details or visit http://www.gnu.org/copyleft/gpl.html
// for details.

struct exportSub {
 FILE *fp;
 short unsigned count;
};

gboolean exportSubtitlesSrt(GtkTreeModel *model, GtkTreePath *path, GtkTreeIter *iter, gpointer userdata);

void exportSubtitles();

void convertTimeSrt(unsigned int theTime, char *res, int flag);

void escapeFileName(char *fileName, char *fileNameEscaped);

#define VBS_DEFAULT_EXPORT_FILE "/tmp/vbs_export.srt"

