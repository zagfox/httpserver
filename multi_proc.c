#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#include <signal.h>
#include <memory.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include "handle_http.h"
#include "format.h"

void run_multi_proc(const struct config *cfg) {
	int sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    struct sockaddr_in address = {0};
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = htonl(INADDR_ANY);
    address.sin_port = htons(cfg->port);

	int opt = 1;
	setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    if (bind(sock, (struct sockaddr*) &address, sizeof(address)) < 0) {
        perror("bind failed");
        exit(1);
    }

    if (listen(sock, 128) < 0) {
        perror("listen failed");
        exit(1);
    }

	// handling zombie process
	//signal(SIGCHLD, waitChildren);
	signal(SIGCHLD, SIG_IGN);

	// hanlding request client socket
    while (1) {
        struct sockaddr_in client_address = {0};
        socklen_t ca_len = 0;

        int csock = accept(sock, (struct sockaddr*) &client_address, &ca_len);
        if (csock < 0) {
          perror("accept failed");
          exit(1);
        }

		int pid = fork();
        if (pid == -1) {
			perror("fail to fork");
		} else if (pid == 0) {
		//while (1) {
            char buf[BUFSIZ];
			memset(buf, 0, BUFSIZ);
            ssize_t bytes_read = 0;
            ssize_t bytes_sent = 0;
            char *msg = 0;
            int msg_size = 0;

			bytes_read = recv(csock, &buf, sizeof(buf)-1, 0);
			if (bytes_read < 0) {
				perror("recv failed");
				exit(1);
			} /*else if (bytes_read == 0) {
				printf("the other side has been closed\n");
				break;
			}*/

			//printf("bytes_read: %lu | %s\n", bytes_read, buf);
			handleRecv(buf, bytes_read, &msg, &msg_size);

			if (msg_size != 0) {
			  bytes_sent = send(csock, msg, msg_size, 0);
			  //printf("data send: %lu %d | %s\n", bytes_sent, msg_size, msg);

			  if (bytes_sent < 0) {
				  perror("sent failed");
			  } 
			}  
			handleSent(&msg, &msg_size);
		//}
            close(csock);
			exit(0);
        } else {
            close(csock);
		}

    }
    close(sock);
}
