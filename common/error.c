/**
 * Error handling functions. 
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

#include "common.h"

/**
 * Error handling function. 
 * @param func[255] char Name of the function where the error occurred.
 * @param error_text[255] char The text of the error message.
 * @param exit_flag bool If TRUE, the process will exit after printing the error message.
 *
 */
void error_handler(char func[256], char error_text[256], bool exit_flag) {
	printf("*** VBS Error: %s in function %s\n", error_text, func);
	fprintf(config.vbsm.log_file_fp, "*** Error: %s in function %s\n", error_text, func);
	fflush(config.vbsm.log_file_fp);
	if (exit_flag)
		exit(1);
}

