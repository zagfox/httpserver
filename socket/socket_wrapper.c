#include <stdio.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>

void perr_exit(const char *s) {
	perror(s);
	exit(1);
}

int socket_safe(int family, int type, int protocol) {
	int fd;
	if ((fd = socket(family, type, protocol)) < 0) 
		perr_exit("socket error");
	return fd;	
}

void bind_safe(int fd, const struct sockaddr *sa, socklen_t len) {
	if (bind(fd, sa, len) < 0)
		perr_exit("bind error");
}

void connect_safe(int fd, const struct sockaddr *sa, socklen_t len) {
	if (connect(fd, sa, len) < 0)
		perr_exit("connect error");
}

void listen_safe(int fd, int backlog) {
	if (listen(fd, backlog) < 0) 
		perr_exit("listen error");
}

int accept_safe(int fd, struct sockaddr *sa, socklen_t *len) {
	int csock;
	while (1) {
		csock = accept(fd, sa, len);
		if (csock >= 0) 
			break;

		if ((errno == ECONNABORTED) || (errno == EINTR))
			continue;
		else
			perr_exit("accept error");
	}
	return csock;
}

void close_safe(int fd) {
	if (close(fd) == -1)
		perr_exit("close error");
}

