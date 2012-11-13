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

// Check magic key
int check_magic_key(int byte_first, int byte_last, unsigned char buffer[config.common.line_size]) {
	unsigned int cnt = 0;
	int i = 0;
	unsigned long res = 0;

	for (i=byte_first; i<=byte_last; i++){
		res += buffer[i] * powf(256,cnt);
		cnt++;
	}

	if (res == config.common.magic_key)
		return 1;

	return 0;
}

void signal_handler(int sig) {
	switch(sig){
		case SIGHUP:
			break;	
		case SIGTERM:
			// Detach and destroy shared memory
			shmdt(shm_at);
			shmctl(unix_time, IPC_RMID);
			// Close original FD
			close(sockfd);
			exit(0);
			break;		
	}	
}

int main() {
	int newsockfd, retPID, shm_id;
	struct sockaddr_in serv_addr, cli_addr;
	socklen_t clilen = sizeof(cli_addr);

	// Init syslog
	const char *ident = "VBS daemon";
	openlog(ident, LOG_ODELAY|LOG_CONS, LOG_DAEMON);

	// Be cool - be a daemon
	pid_t pid;
	pid = fork();
	if (pid < 0) {
		syslog(LOG_CRIT, "Failed to fork!");
		exit(1);
	}
	if (pid > 0) {exit(0);}
	setsid();
	close(STDIN_FILENO);
	close(STDOUT_FILENO);
	close(STDERR_FILENO);
	signal(SIGHUP, signal_handler);
	umask(0);
	chdir("/");

	check_config(1);

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0) {
		syslog(LOG_CRIT, "Failed to create socket!");
		exit(1);
	}

	bzero((char *) &serv_addr, sizeof(serv_addr));

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	serv_addr.sin_port = htons(config.common.tcp_port);
	if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
		syslog(LOG_CRIT, "Failed to bind to TCP port!");
		exit(1);
	}

	listen(sockfd, QUEUE_SIZE);

	// Shared memory
	unix_time = time(NULL);
	shm_id = shmget(unix_time, config.common.line_size, IPC_CREAT|IPC_EXCL); 
	shm_at = (char *)shmat(shm_id, 0, 0);
	strcpy(shm_at, "Initial server message");

	syslog(LOG_CRIT, "Started OK");

	while(1) {
		newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
		if (newsockfd >= 0) {
			pid = fork();
			if (pid == 0) {
				// FORKED ACTION HERE
				char buffer[config.common.line_size];
				int n;

				shm_id = shmget(unix_time, config.common.line_size, IPC_CREAT);
				shm_at = (char *)shmat(shm_id, 0, 0);

				bzero(buffer, sizeof(buffer));

				n = recv(newsockfd, buffer, sizeof(buffer)-1, 0);
				if (n < 0) {
					syslog(LOG_CRIT, "Error reading from socket!");
				}
				else {
					// DO SOMETHING WITH 'buffer'
					//syslog(LOG_CRIT, buffer);
					if (check_magic_key(0, 3, buffer) == 1) {
						//syslog(LOG_CRIT, "Got magic!");
						strcpy(shm_at, buffer + 4);
					}

					// WRITE BACK
					n = send(newsockfd, shm_at, config.common.line_size, 0);
					if (n < 0) {
						syslog(LOG_CRIT, "Error writing to socket!");
					}
				}

				// Detach shared memory
				shmdt(shm_at);

				// Close new FD
				sleep(5);
				close(newsockfd);
				close(sockfd);

				exit(0);
			}
		}

		close(newsockfd);
		// Collect dead bones
		retPID = 1;
		while(retPID) {
			retPID = 0;
			retPID = waitpid(-1, NULL, WNOHANG);
		}

	}

	// Detach and destroy shared memory
	shmdt(shm_at);
	shmctl(unix_time, IPC_RMID);

	// Close original FD
	close(sockfd);

	return 0; 
}
