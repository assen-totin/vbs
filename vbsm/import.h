// This file is a part of Voody Blue Subtitler suit.
// 
// Author: Assen Totin <assen.totin@gmail.com>
//
// Home page: http://bilbo.online.bg/~assen/vbs
//
// This software is released under GNU General Public License.
// See the LICENSE file for details or visit http://www.gnu.org/copyleft/gpl.html 
// for details.

void clearStore();

void importText(char *importTextFile, int importFlag);

void importFilterSrt(char *importTextFile);

unsigned int convertTime(char *inTime);

bool haveLoadedText(GtkWidget *window);

