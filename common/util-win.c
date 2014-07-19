/**
 * MS Windows-specific utility functions. 
 * @author Assen Totin assen.totin@gmail.com
 * 
 * Created for the Voody Blue Subtitler suit, copyright (C) 2014 Assen Totin, assen.totin@gmail.com 
 * 
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#include "../common/common.h"

/**
 * Read a directory path. 
 * @param *win_path char Pointer to write the path to.
 * @param size int Size of the buffer the pointer points to.
 * @returns bool TRUE on success, FALSE on failure.
 */
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

/**
 * Get current locale. 
 * @param *locale char Pointer to write the locale name to.
 */
void win_get_locale(char *locale) {
	char country_code[16];
	GetLocaleInfo(GetUserDefaultLCID(),LOCALE_SISO3166CTRYNAME, &country_code[0], 16);
	char lang_code[16];
	GetLocaleInfo(GetUserDefaultLCID(),LOCALE_SISO639LANGNAME, &lang_code[0], 16);
	sprintf(locale, "%s_%s", &lang_code[0], &country_code[0]);
}

/**
 * Convert filename to URI.
 * @param *filename char Filename to convert.
 * @param *tmp_filename char Pointer to write the URI to.
 */
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

