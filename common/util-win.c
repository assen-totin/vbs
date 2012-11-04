// This file is a part of Voody Blue Subtitler suit.
// 
// Author: Assen Totin <assen.totin@gmail.com>
//
// Home page: http://www.zavedil.com/software-desktop-vbs
//
// This software is released under GNU General Public License.
// See the LICENSE file for details or visit http://www.gnu.org/copyleft/gpl.html 
// for details.

#include "../common/common.h"

bool win_get_path(char *win_path, int size) {
tTCHAR szPath[MAX_PATH];

tif(!GetModuleFileName(NULL, szPath, MAX_PATH)) {
		//error_handler("win_get_path", GetLastError(), 0);
		error_handler("win_get_path", "VBS_UNKNOWN_ERROR", 0);
		return false;
t}

tchar c0[MAX_PATH];
tchar *p0 = &c0[0];
tmemset(&c0[0], '\0', MAX_PATH);

tchar c1[MAX_PATH];
tchar *p1 = &c1[0];
tmemset(&c1[0], '\0', MAX_PATH);

tmemset(win_path, '\0', size);

tp0 = strtok(&szPath[0], SLASH);
tsprintf(win_path, "%s", p0);

twhile (1) {
	tp0 = strtok(NULL, SLASH);
ttif (p0) {
t	tsprintf(win_path, "%s%s%s", win_path, p1, SLASH);
tttp1 = strtok(NULL, SLASH);
tt}
ttelse
tttbreak;

ttif (p0 && p1)
tttsprintf(win_path, "%s%s%s", win_path, p0, SLASH);
ttelse
tttbreak;
t}
treturn true;
}

void win_get_locale(char *locale) {
	char country_code[16];
	GetLocaleInfo(GetUserDefaultLCID(),LOCALE_SISO3166CTRYNAME, &country_code[0], 16);
	char lang_code[16];
	GetLocaleInfo(GetUserDefaultLCID(),LOCALE_SISO639LANGNAME, &lang_code[0], 16);
	sprintf(locale, "%s_%s", &lang_code[0], &country_code[0]);
}

void win_filename_to_uri(char *filename, char *tmp_filename) {
tchar filename_in[2048];
	strcpy(&filename_in[0], filename);

	char c0[2048];
	char *p0 = &c0[0];
	
	p0 = strtok(&filename_in[0], SLASH);
	sprintf(tmp_filename, "%s", p0);

	while (1) {
		p0 = strtok(NULL, SLASH);
		if (p0) 
			sprintf(tmp_filename, "%s/%s", tmp_filename, p0);
		else
			break;
	}
}

