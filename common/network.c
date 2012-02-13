#include "common.h"
#include "network.h"

int get_host_by_name(struct hostent *host_entry) {
        host_entry = gethostbyname(config.server_name);
        if (host_entry == NULL) {
                error_handler("get_host_by_name", "No such host", 1);
                return 0;
        }
}

int get_socket() {
	int sockfd;
	struct sockaddr_in serv_addr;
	struct hostent *server = &config.host_entry;

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0) 
		error_handler("get_subtitle", "Could not open socket", 1);

	bzero((char *) &serv_addr, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	bcopy((char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr, server->h_length);
	serv_addr.sin_port = htons(config.tcp_port);

	if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0) {
		close(sockfd);
		error_handler("get_subtitle", "Could not connect to server", 1);
	}

	return sockfd;
}

int get_subtitle(char *buffer) {
	int sockfd, n;
	char request[config.line_size];

	sockfd = get_socket();

	strcpy(request,"42");
	n = write(sockfd, request, strlen(request));
	if (n < 0) {
		error_handler("get_subtitle", "Could not write to socket", 0);
		close(sockfd);
		return 0;
	}

	bzero(buffer, config.line_size);

	n = read(sockfd, buffer, config.line_size - 1);
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
        char request[config.line_size];
	char buffer2[config.line_size + 4];

        get_socket(&sockfd);

        memcpy(&buffer2[0], &config.magic_key, sizeof(config.magic_key));
        memcpy(&buffer2[4], buffer, sizeof(*buffer));

        n = write(sockfd, buffer2, sizeof(buffer2));

        if (n < 0) {
                error_handler("put_subtitle", "Could not write to socket", 0);
                close(sockfd);
                return 0;
        }

        bzero(buffer, config.line_size);

        n = read(sockfd, buffer, config.line_size - 1);
        if (n < 0) {
                error_handler("put_subtitle", "Could not read from socket", 0);
                close(sockfd);
                return 0;
        }

        close(sockfd);

        return 1;
}

