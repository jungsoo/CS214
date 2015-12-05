#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define __SERVER_PORT__ 6694



void error(char *msg) {
    perror(msg);
    exit(1);
}

void client_service(int sock) {
    char request[2048];

    memset(&request, '0', sizeof(request));
    while(read(sock, request, sizeof(request)) > 0) {
        printf("CLIENT: %s\n", request);
    }

    close(sock);
}

int main() {
    int listen_fd = socket(AF_INET, SOCK_STREAM, 0), connection_fd;
    if (listen_fd < 0) {
        error("ERROR: Unable to open socket.");
    }
    
    struct sockaddr_in serv_addr, cli_addr;
    memset(&serv_addr, '0', sizeof(serv_addr));

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(__SERVER_PORT__);

    if (bind(listen_fd, (struct sockaddr *) &serv_addr, sizeof(serv_addr))) {
        error("ERROR: Unable to bind address/port.");
    }
    listen(listen_fd, 5);
    printf("SERVER: Waiting for a connection...\n");

    pid_t pid;
    socklen_t cli_len = sizeof(cli_addr);
    while (1) {
        connection_fd = accept(listen_fd, (struct sockaddr *) &cli_addr, &cli_len);
        printf("SERVER: Connected to a new client!\n");

        if (connection_fd < 0) {
            error("ERROR: Unable to accept.");
        }
        
        pid = fork();
        if (pid < 0) {
            error("ERROR: fork() failed!");
        } else if (pid == 0) {
            close(listen_fd);
            client_service(connection_fd);
            exit(0);
        } else {
            close(connection_fd);
        }
    }
    close(listen_fd);
    return 0;
}

