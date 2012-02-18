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

void writeMPlayer(char *command) {
	fprintf(config.vbsm.pipeWrite, "%s\n", command);
	fflush(config.vbsm.pipeWrite);
}


int getTimePos(int flag) {
	// Ask mplayer about exact position, read it
	if (flag == 1) 
		writeMPlayer("pausing_keep get_time_pos");
	else if (flag == 2) 
		writeMPlayer("get_time_pos");

	// Read its answer -  Mplayer sends a lot of other stuff, jump over it
	char line[256], *partOne, *partTwo;
	double doubleTwo;
	bool goOn = true;
	int res;

	while (goOn && (fgets(&line[0], 255, config.vbsm.pipeRead))) {
		if (strstr(&line[0],"ANS_TIME_POSITION")) {
			// This is our line!
			line[strlen(line) - 1] = 0;     /* kill '\n' */
			// Split by the "=" sign
			partOne = strtok(&line[0], "=");
			partTwo = strtok(NULL,"=");
			doubleTwo = strtod(partTwo, NULL);
			doubleTwo = 1000 * doubleTwo;
			res = (int)doubleTwo;
			goOn = false;
		}
	}
	return res;
}

/*
int getSubNum() {
	// Read Mplayer's answer on sub_load command -  Mplayer sends a lot of other stuff, jump over it
	char line[256], partOneVal[256], partTwoVal[256];
	char *partOne = &partOneVal[0];
	char *partTwo = &partTwoVal[0];

	bool goOn = true;
	int res;

	while (goOn && (fgets(&line[0], 255, config.vbsm.pipeRead))) {
		if (strstr(&line[0],"Added subtitle file")) {
			// This is our line! Split by the ")" sign
			partOne = strtok(&line[0], ")");
			partTwo = strtok(partOne,"(");
			strtok(NULL,"(");
			res = atoi(partTwo);
			goOn = false;
		}
	}

	return res;
}
*/


void *loadVideo(char fileName[1024]) {
	char popenCmd[2048];
	memset(&popenCmd[0],'\0', sizeof(popenCmd));

	if (config.vbsm.mplayer_pid > 0) {
		if (mplayerAlive()) {
			sprintf(popenCmd, "load %s", &fileName[0]); 
			writeMPlayer(popenCmd);
		}

		else {
			config.vbsm.mplayer_pid = 0;
			fclose(config.vbsm.pipeRead);
			fclose(config.vbsm.pipeWrite);
		}
	}
	
	if (config.vbsm.mplayer_pid == 0) {
		int readPipeFD[2], writePipeFD[2];
		pid_t cpid;

		// Pipes
		if (pipe(readPipeFD) == -1) { error_handler("loadVideo","read pipe creation failed", 1);}
		if (pipe(writePipeFD) == -1) { error_handler("loadVideo","write pipe creation failed", 1);}

		cpid = fork();
		if (cpid == -1) {error_handler("loadVideo","fork failed", 1);}

		// Child
		if (cpid == 0) { 
			close(readPipeFD[0]);          /* Close unused read end */
			close(writePipeFD[1]);         /* Close unused write end */
	
			dup2(writePipeFD[0], STDIN_FILENO);
			dup2(readPipeFD[1], STDOUT_FILENO);

			close(writePipeFD[0]);
			close(readPipeFD[1]);

			execlp(VBS_MPLAYER_BINARY, VBS_MPLAYER_BINARY, "-slave", "-quiet", "-osdlevel", "3", &fileName[0], (char *) NULL);
		}

		// Parent
		close(readPipeFD[1]);          /* Close unused write end */
		close(writePipeFD[0]);         /* Close unused read end */

		config.vbsm.pipeRead = fdopen(readPipeFD[0], "r");
		config.vbsm.pipeWrite = fdopen(writePipeFD[1], "w");
		config.vbsm.mplayer_pid = cpid;
	}

	writeMPlayer("pause");
}

