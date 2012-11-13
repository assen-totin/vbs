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

int get_host_by_name(char *server_name) {
#ifdef HAVE_WINDOWS
	WSADATA wsaData;
	int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0) {
		error_handler("get_host_by_name", " WSAStartup failed", 0);
		return 0;
	}
#endif
	struct hostent *host_entry = gethostbyname(server_name);
	if (host_entry == NULL) {
		error_handler("get_host_by_name", "No such host", 0);
		return 0;
	}
	else {
		strcpy(&config.common.server_name[0], server_name);
		config.common.host_entry = host_entry;
		return 1;
	}
}

int get_socket() {
	int sockfd;
	struct sockaddr_in serv_addr;

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0) 
		error_handler("get_subtitle", "Could not open socket", 1);

	memset((char *) &serv_addr, '\0' , sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	memcpy((char *)&serv_addr.sin_addr.s_addr, (char *)config.common.host_entry->h_addr, config.common.host_entry->h_length);
	serv_addr.sin_port = htons(config.common.tcp_port);

	if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0) {
		close(sockfd);
		error_handler("get_subtitle", "Could not connect to server", 1);
	}

	return sockfd;
}

int get_subtitle(char *buffer) {
	int sockfd, n;
	char request[config.common.line_size];

	sockfd = get_socket();

	strcpy(&request[0], "42");
	n = send(sockfd, &request[0], strlen(&request[0]), 0);
	if (n < 0) {
		error_handler("get_subtitle", "Could not write to socket", 0);
		close(sockfd);
		return 0;
	}

	memset(buffer, '\0', config.common.line_size);

	n = recv(sockfd, buffer, config.common.line_size - 1, 0);
	if (n < 0) {
		error_handler("get_subtitle", "Could not read from socket", 0);
		close(sockfd);
		return 0;
	}

	close(sockfd);

	return 1;
}


int put_subtitle(char *buffer) {
	int sockfd, n;
	char request[config.common.line_size];
	char buffer2[config.common.line_size + strlen(&config.common.magic_key[0])];

	sockfd = get_socket();

	strcpy(&request[0], buffer);
	fix_new_line(&request[0]);

	memset(&buffer2[0], '\0', sizeof(buffer2));
	sprintf(&buffer2[0], "%s%s", &config.common.magic_key[0], &request[0]);

	n = send(sockfd, &buffer2[0], sizeof(buffer2), 0);
	if (n < 0) {
		error_handler("put_subtitle", "Could not write to socket", 0);
		close(sockfd);
		return 0;
	}

	memset (&buffer2[0], '\0', sizeof(buffer2));

	n = recv(sockfd, &buffer2[0], sizeof(buffer2), 0);
	if (n < 0) {
		error_handler("put_subtitle", "Could not read from socket", 0);
		close(sockfd);
		return 0;
	}

	close(sockfd);
	return 1;
}

