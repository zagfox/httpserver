#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#include <sys/types.h>
#include <memory.h>
#include <pthread.h>

#include "format.h"
#include "http/handle_http.h"
#include "socket/socket_wrapper.c"

#define THREADS_NUM 4

void handle_http(void *arg) {
	int sock = *(int*)arg;
	while (1) {
		struct sockaddr_in client_address = {0};
		socklen_t ca_len = 0;

		int csock = accept_safe(sock, (struct sockaddr*) &client_address, &ca_len);

		char buf[BUFSIZ];
		memset(buf, 0, BUFSIZ);
		ssize_t bytes_read = 0;
		ssize_t bytes_sent = 0;
		char *msg = NULL;
		int msg_size = 0;

		bytes_read = read_safe(csock, &buf, sizeof(buf)-1);
		if (bytes_read < 0) {
			perror("recv failed");
			exit(1);
		} 

		handleRecv(buf, bytes_read, &msg, &msg_size);

		if (msg_size != 0) {
			bytes_sent = write_safe(csock, msg, msg_size);
			if (bytes_sent < 0) {
				perror("sent failed");
			}
		}

		handleSent(&msg, &msg_size);
		close_safe(csock);
	}
}

void run_multi_thread(const struct config *cfg) {
	int sock = socket_safe(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	struct sockaddr_in address = {0};
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = htonl(INADDR_ANY);
	address.sin_port = htons(cfg->port);

	int opt = 1;
	setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

	bind_safe(sock, (struct sockaddr*) &address, sizeof(address));

	listen_safe(sock, 128);

	// hanlding request client socket
	//fork();
	//fork();
	pthread_t threads[THREADS_NUM];
	int i;
	for (i = 0; i < THREADS_NUM; ++i) {
		pthread_create(&threads[i], NULL, (void*)&handle_http, (void*)&sock);
	}

	for (i = 0; i < THREADS_NUM; ++i) {
		pthread_join(threads[i], NULL);
	}

	close_safe(sock);
}
