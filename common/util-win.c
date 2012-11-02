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
        TCHAR szPath[MAX_PATH];

        if(!GetModuleFileName(NULL, szPath, MAX_PATH)) {
		//error_handler("win_get_path", GetLastError(), 0);
		error_handler("win_get_path", "VBS_UNKNOWN_ERROR", 0);
		return false;
        }

        char c0[MAX_PATH];
        char *p0 = &c0[0];
        memset(&c0[0], '\0', MAX_PATH);

        char c1[MAX_PATH];
        char *p1 = &c1[0];
        memset(&c1[0], '\0', MAX_PATH);

        memset(win_path, '\0', size);

        p0 = strtok(&szPath[0], SLASH);
        sprintf(win_path, "%s", p0);

        while (1) {
	        p0 = strtok(NULL, SLASH);
                if (p0) {
        	        sprintf(win_path, "%s%s%s", win_path, p1, SLASH);
                        p1 = strtok(NULL, SLASH);
                }
                else
                        break;

                if (p0 && p1)
                        sprintf(win_path, "%s%s%s", win_path, p0, SLASH);
                else
                        break;
        }
        return true;
}

void win_get_locale(char *locale) {
	char country_code[16];
	GetLocaleInfo(GetUserDefaultLCID(),LOCALE_SISO3166CTRYNAME, &country_code[0], 16);
	char lang_code[16];
	GetLocaleInfo(GetUserDefaultLCID(),LOCALE_SISO639LANGNAME, &lang_code[0], 16);
	sprintf(locale, "%s_%s", &lang_code[0], &country_code[0]);
}

void win_filename_to_uri(char *filename, char *tmp_filename) {
        char filename_in[2048];
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

