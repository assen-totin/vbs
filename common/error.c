// This file is a part of Voody Blue Subtitler suit.
// 
// Author: Assen Totin <assen.totin@gmail.com>
//
// Home page: http://www.zavedil.com/software-desktop-vbs
//
// This software is released under GNU General Public License.
// See the LICENSE file for details or visit http://www.gnu.org/copyleft/gpl.html 
// for details.

#include "common.h"

void error_handler(char func[256], char error_text[256], bool exit_flag) {
	printf("*** VBS Error: %s in function %s\n", error_text, func);
	fprintf(config.vbsm.log_file_fp, "*** Error: %s in function %s\n", error_text, func);
	fflush(config.vbsm.log_file_fp);
	if (exit_flag)
		exit(1);
}

