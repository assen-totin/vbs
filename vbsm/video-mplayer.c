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

bool mplayer_is_alive() {
	if (!config.vbsm.mplayer_pipe_write)
		return false;
	if (config.vbsm.mplayer_pid == 0)
		return false;
	int status;
	pid_t cpid = waitpid(-1, &status, WNOHANG);
	if (cpid == config.vbsm.mplayer_pid)
		return false;
	return true;
}


void mplayer_pipe_write(char *command) {
	fprintf(config.vbsm.mplayer_pipe_write, "%s\n", command);
	fflush(config.vbsm.mplayer_pipe_write);
}


int mplayer_get_time_pos(int flag) {
	// Ask mplayer about exact position, read it
	if (flag == 1) 
		mplayer_pipe_write("pausing_keep get_time_pos");
	else if (flag == 2) 
		mplayer_pipe_write("get_time_pos");

	// Read its answer -  Mplayer sends a lot of other stuff, jump over it
	char line[256], *part_one, *part_two;
	double double_two;
	bool go_on = true;
	int res;

	while (go_on && (fgets(&line[0], 255, config.vbsm.mplayer_pipe_read))) {
		if (strstr(&line[0],"ANS_TIME_POSITION")) {
			// This is our line!
			line[strlen(line) - 1] = 0;     // kill '\n'
			// Split by the "=" sign
			part_one = strtok(&line[0], "=");
			part_two = strtok(NULL,"=");
			double_two = strtod(part_two, NULL);
			double_two = 1000 * double_two;
			res = (int)double_two;
			go_on = false;
		}
	}
	return res;
}

int mplayer_get_time_length() {
	char line[256], *part_one, *part_two;
	double double_two;
	int res;
	mplayer_pipe_write("get_time_length");
	while (fgets(&line[0], 255, config.vbsm.mplayer_pipe_read)) {
		if (strstr(&line[0],"ANS_LENGTH")) {
			line[strlen(line) - 1] = 0;     // kill '\n'
			part_one = strtok(&line[0], "=");
			part_two = strtok(NULL,"=");
			double_two = strtod(part_two, NULL);
			res = (int)double_two;
			break;
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

	while (goOn && (fgets(&line[0], 255, config.vbsm.mplayer_pipe_read))) {
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


void mplayer_load_video(char fileName[1024], int *import_error_flag) {
	char popenCmd[2048];
	memset(&popenCmd[0],'\0', sizeof(popenCmd));

	if (config.vbsm.mplayer_pid > 0) {
		if (mplayer_is_alive()) {
			sprintf(popenCmd, "load %s", &fileName[0]); 
			mplayer_pipe_write(popenCmd);
		}

		else {
			config.vbsm.mplayer_pid = 0;
			fclose(config.vbsm.mplayer_pipe_read);
			fclose(config.vbsm.mplayer_pipe_write);
		}
	}
	
	if (config.vbsm.mplayer_pid == 0) {
		int readPipeFD[2], writePipeFD[2];
		pid_t cpid;

		// Pipes
		if (pipe(readPipeFD) == -1) 
			error_handler("mplayer_load_video","read pipe creation failed", 1);
		if (pipe(writePipeFD) == -1) 
			error_handler("mplayer_load_video","write pipe creation failed", 1);

		cpid = fork();
		if (cpid == -1) 
			error_handler("mplayer_load_video","fork failed", 1);

		// Child
		if (cpid == 0) { 
			close(readPipeFD[0]);	  // Close unused read end 
			close(writePipeFD[1]);	 // Close unused write end
	
			dup2(writePipeFD[0], STDIN_FILENO);
			dup2(readPipeFD[1], STDOUT_FILENO);

			close(writePipeFD[0]);
			close(readPipeFD[1]);

			execlp(VBSM_MPLAYER_BINARY, VBSM_MPLAYER_BINARY, "-slave", "-quiet", "-input", "nodefault-bindings:file=/dev/null:conf=/dev/null", "-utf8", "-osdlevel", "3", &fileName[0], (char *) NULL);
		}

		// Parent
		close(readPipeFD[1]);	  // Close unused write end
		close(writePipeFD[0]);	 // Close unused read end

		config.vbsm.mplayer_pipe_read = fdopen(readPipeFD[0], "r");
		config.vbsm.mplayer_pipe_write = fdopen(writePipeFD[1], "w");
		config.vbsm.mplayer_pid = cpid;
	}

	config.vbsm.film_duration = mplayer_get_time_length();
	mplayer_pipe_write("pause");
}

void mplayer_goto(int new_time, bool pause) {
	if (mplayer_is_alive()) {
		char command[255];
		sprintf(command, "pausing_keep seek %u 2", new_time);
		mplayer_pipe_write(command);

		// Tell mplayer to load subtitles as they exist now
		mplayer_pipe_write("pausing_keep sub_remove");
		sprintf(command, "pausing_keep sub_load %s", &config.vbsm.sub_file_name[0]);
		mplayer_pipe_write(command);

// This *should* normally work;
// However, my mplayer crashes with "signal 11 in sub_find" when executing "sub_select"
// after getSubNum().
// Since we clear all subs before reloading, the new level should always be 1.
//    subNum = getSubNum();
		int subNum = 1;
		sprintf(command, "pausing_keep sub_select %u", subNum);
		mplayer_pipe_write(command);

		if (pause)
			mplayer_pipe_write("pause");
	}
}
