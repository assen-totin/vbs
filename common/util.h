// This file is a part of Voody Blue Subtitler suit.
// 
// Author: Assen Totin <assen.totin@gmail.com>
//
// Home page: http://www.zavedil.com/software-desktop-vbs
//
// This software is released under GNU General Public License.
// See the LICENSE file for details or visit http://www.gnu.org/copyleft/gpl.html 
// for details.

void fix_new_line(char *);

long get_time_msec();

int convert_time_from_srt(char *, int);

void convert_time_to_srt(unsigned int, char *, int);

struct subtitle_srt *import_subtitles_srt(char *, int *);

void get_locale_prefix(char *res);

void get_icon(char *res);

void get_file_selector_path(char *res);

void get_dir_from_filename (char *filename, char *dir);
