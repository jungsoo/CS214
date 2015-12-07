#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include "util.h"

int listen_fd;

void output_loop(char *server) {
    char output[2048];

    printf("CLIENT: Ready to receive server output.\n");

    while(read(listen_fd, output, sizeof(output)) > 0) {
        printf("%s", output);
    }
}

void input_loop() {
    printf("CLIENT: Ready to take user input.\n");
    char input[2048];

    while(1) {
        scanf(" %[^\n]", input);
        write(listen_fd, input, strlen(input) + 1);

        /*
        if (streq(input, "exit")) {
            printf("CLIENT: Exiting.\n");
            kill(getppid(), SIGINT);
            exit(0);
        }
        */
        sleep(2);
    }
}

// Thanks BKR
void set_iaddr_str(struct sockaddr_in *sockaddr, char *x, unsigned int port) {
    struct hostent *hostptr;

    memset(sockaddr, 0, sizeof(&sockaddr));
    sockaddr->sin_family = AF_INET;
    sockaddr->sin_port = htons(port);
    if ((hostptr = gethostbyname(x)) == NULL) {
        error("ERROR: Can't get address information!");
    } else {
        bcopy(hostptr->h_addr, (char *) &sockaddr->sin_addr, hostptr->h_length);
    }
}

int main(int argc, char **argv) {
    if (argc != 2) {
        error("ERROR: Server host must be specified as a command line argument.");
    }
    char *serv_name = argv[1];
    
    listen_fd = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in serv_addr;

    set_iaddr_str(&serv_addr, serv_name, __SERVER_PORT__);

    int x;
    while ((x = connect(listen_fd, (const struct sockaddr *) &serv_addr, sizeof(serv_addr)))) {
        printf("CLIENT: Attempting to connect to %s...\n", serv_name);
        perror("");
        sleep(3);
    }

    printf("CLIENT: Connected to %s.\n", serv_name);

    pid_t pid = fork();
    if (pid == 0) { // child
        input_loop();
    } else if (pid > 0) { // parent
        output_loop(serv_name);
    } else { // error
        error("ERROR: fork() failed!");
    }

    close(listen_fd);
    printf("CLIENT: Disconnected from server.\n");
    return 0;
}
