/**
 * Network-related functions. 
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
 * Resolve hostname to IP address. 
 * @param *server_name char The name to resolve.
 * @returns int 1 on success, 0 on error.
 */
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

/**
 * Obtain TCP socket. 
 * @returns int Socket file descriptor.
 */
int get_socket() {
	int sockfd;
	struct sockaddr_in serv_addr;

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0) 
		error_handler("get_socket", "Could not open socket", 1);

	memset((char *) &serv_addr, '\0' , sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	memcpy((char *)&serv_addr.sin_addr.s_addr, (char *)config.common.host_entry->h_addr, config.common.host_entry->h_length);
	serv_addr.sin_port = htons(config.common.tcp_port);

	if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0) {
		close(sockfd);
		error_handler("get_socket", "Could not connect to server", 1);
	}

	return sockfd;
}

/**
 * Send subtitle over the network. 
 * @param *buffer char The text to send.
 * @returns int 1 on success, 0 on error.
 */
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

