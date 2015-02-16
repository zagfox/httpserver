#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include "handleHttp.c"


/*
 * arg0 func name (default)
 * arg1 socket port
 */
int main(int argc, char *argv[]) {
    if (argc != 2)
        exit(1);
    int sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    struct sockaddr_in address = {0};
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = htonl(INADDR_ANY);
    address.sin_port = htons(atoi(argv[1]));

    if (bind(sock, (struct sockaddr*) &address, sizeof(address)) < 0) {
        perror("bind failed");
        exit(1);
    }

    if (listen(sock, 128) < 0) {
        perror("listen failed");
        exit(1);
    }

    while (1) {
        struct sockaddr_in client_address = {0};
        socklen_t ca_len = 0;

        int csock = accept(sock, (struct sockaddr*) &client_address, &ca_len);
        if (csock < 0) {
          perror("accept failed");
          exit(1);
        }

        //has problem with this fork
        //if (fork() == 0) {
            char buf[BUFSIZ];
            ssize_t bytes_read = 0;
            ssize_t bytes_sent = 0;
            char *msg = 0;
            int msg_size = 0;

            //do {
                bytes_read = recv(csock, &buf, sizeof(buf)-1, 0);
                if (bytes_read < 0) {
                    perror("recv failed");
                    exit(1);
                }

                printf("data recv:\n%s\n", buf);
                handleRecv(buf, sizeof(buf), &msg, &msg_size);

                if (msg_size != 0) {
                  bytes_sent = send(csock, msg, msg_size, 0);
                  printf("data send: %d | %s\n", msg_size, msg);

                  if (bytes_sent < 0) {
                      perror("sent failed");
                  } 
                }  
                handleSent(&msg, &msg_size);

            //} while (bytes_read > 0 && bytes_sent >= 0);

            close(csock);
            //exit(0);
        //}

    }
    close(sock);
}