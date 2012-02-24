// This file is a part of Voody Blue Subtitler suit.
// 
// Author: Assen Totin <assen.totin@gmail.com>
//
// Home page: http://bilbo.online.bg/~assen/vbs
//
// This software is released under GNU General Public License.
// See the LICENSE file for details or visit http://www.gnu.org/copyleft/gpl.html 
// for details.

#include "../common/common.h"

void fixNewline(char *buffer) {
        char *partOne, linePrint[config.common.line_size];

        char CrLf[3];
        sprintf(CrLf, "\n");
        if (config.common.export_cr == 1) {sprintf(CrLf, "\r\n");}

        // Replace | with \n
        partOne = strtok(buffer, "|");
        sprintf(&linePrint[0], "%s", partOne);

        while(partOne = strtok(NULL,"|")) {
                sprintf(&linePrint[0], "%s%s%s", &linePrint[0], &CrLf[0], partOne);
        }

        strcpy(buffer, &linePrint[0]);
}

void split_path(char *path, char *dir, char *file) {
        char *tmp;
        char tmp_old[100];

        strcpy(&tmp_old[0], strtok(path, "/"));

	bzero(dir, sizeof(*dir));

        while(1) {
                tmp = strtok(NULL, "/");
                if(tmp) {
                        sprintf(dir, "%s/%s", dir, &tmp_old[0]);
                        sprintf(&tmp_old[0], "%s", tmp);
                }
                else
                        break;
        }

        strcpy(file, &tmp_old[0]);
}

