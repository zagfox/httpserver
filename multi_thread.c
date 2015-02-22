#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#include <memory.h>

#include <sys/types.h>

#include <pthread.h>

#include "http/handle_http.h"
#include "format.h"
#include "socket/socket_wrapper.c"

void handle_http(void *arg) {
	int csock = *(int*)arg;

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
	} 

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
	close_safe(csock);
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
	while (1) {
		struct sockaddr_in client_address = {0};
		socklen_t ca_len = 0;

		int *csock = malloc(sizeof(int));  //has memory leak
		*csock = accept_safe(sock, (struct sockaddr*) &client_address, &ca_len);
			
		pthread_t thread_id;
		pthread_create(&thread_id, NULL, (void*)&handle_http, (void*)csock);
		//pthread_join(thread_id, NULL);
	}
	close_safe(sock);
}
