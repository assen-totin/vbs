/*
 * This code is licensed under the terms of GNU General Public License version 3.
 * You can find the license at http://www.gnu.org
 *
 * Author: Assen Totin, assen dot totin at gmail dot com
 */

#include "../common/common.h"
#include "server.h" 

// Check magic key
unsigned long magic_key(int byte_first, int byte_last, unsigned char buffer[config.line_size]) {
	unsigned int cnt = 0;
	int i = 0;
	unsigned long res = 0;

        for (i=byte_first; i<=byte_last; i++){
                res += buffer[i] * powf(256,cnt);
                cnt++;
        }

	if (res == config.magic_key)
		return 1;

	return 0;
}

int main() {
	int sockfd, newsockfd, retPID, shm_id, unix_time;
	char *shm_at;
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
	signal(SIGHUP, SIG_IGN);
	umask(0);
	chdir("/");

	check_config();

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0) {
		syslog(LOG_CRIT, "Failed to create socket!");
		exit(1);
	}

	bzero((char *) &serv_addr, sizeof(serv_addr));

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	serv_addr.sin_port = htons(config.tcp_port);
	if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
		syslog(LOG_CRIT, "Failed to bind to TCP port!");
		exit(1);
	}

	listen(sockfd, QUEUE_SIZE);

	// Shared memory
	unix_time = time(NULL);
	shm_id = shmget(unix_time, config.line_size, IPC_CREAT|IPC_EXCL); 
	shm_at = (char *)shmat(shm_id, 0, 0);
	strcpy(shm_at, "Initial test message");

	syslog(LOG_CRIT, "Started OK");

	while(1) {
		newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
		if (newsockfd >= 0) {
			pid = fork();
			if (pid == 0) {
				// FORKED ACTION HERE
				char buffer[config.line_size];
				int n;

				shm_id = shmget(unix_time, config.line_size, IPC_CREAT);
				shm_at = (char *)shmat(shm_id, 0, 0);

				bzero(buffer, sizeof(buffer));

				n = read(newsockfd, buffer, sizeof(buffer)-1);
				if (n < 0) {
					syslog(LOG_CRIT, "Error reading from socket!");
				}
				else {
					// DO SOMETHING WITH 'buffer'
					syslog(LOG_CRIT, buffer);
					if (magic_key(0, 3, buffer)) {
						syslog(LOG_CRIT, "Got magic!");
						strcpy(shm_at, buffer + 4);
					}

					// WRITE BACK
					n = write(newsockfd, shm_at, config.line_size);
					if (n < 0) {
						syslog(LOG_CRIT, "Error writing to socket!");
					}
				}

				// Detach shared memory
				shmdt(shm_at);

				// Close new FD
				close(newsockfd);

				exit(0);
			}
		}

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
