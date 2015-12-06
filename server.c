#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/mman.h>
#include <netinet/in.h>

#define __SERVER_PORT__ 6694

typedef struct account {
    char name[100];
    double balance;
    int is_active;
} account_t;

account_t *bank;

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

void print_bank_info(int signum) {
    // TODO
}

void set_timer(int seconds) {
    struct itimerval timer;
    timer.it_value.tv_sec = seconds;
    timer.it_value.tv_usec = 0;
    timer.it_interval.tv_sec = seconds;
    timer.it_interval.tv_usec = 0;
    setitimer(ITIMER_REAL, &timer, NULL);
}

void cleanup(int signum) {
    munmap(bank, getpagesize());
    exit(0);
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

    // Display server information every 20 seconds.
    /*
    struct sigaction timer;
    sigemptyset(&timer.sa_mask);
    timer.sa_flags = 0;
    timer.sa_handler = print_bank_info;
    sigaction(SIGALRM, &timer, NULL);

    set_timer(20);
    */

    // Server exit handler
    /* Do we really need sigaction?
    struct sigaction quit;
    sigemptyset(&quit.sa_mask);
    quit.sa_flags = 0;
    quit.sa_handler = cleanup;
    sigaction(SIGINT, &quit, NULL);
    */
    signal(SIGINT, cleanup);

    if (bind(listen_fd, (struct sockaddr *) &serv_addr, sizeof(serv_addr))) {
        error("ERROR: Unable to bind address/port.");
    }
    listen(listen_fd, 5);
    printf("SERVER: Waiting for a connection...\n");

    // Memory-mapping bank info to file...
    int bank_fd = open("bankfile", O_RDWR | O_CREAT | O_TRUNC, (mode_t)0600);
    if (bank_fd == -1) {
        close(bank_fd);
        error("ERROR: Could not open file.");
    }
    if (lseek(bank_fd, 20*sizeof(account_t), SEEK_SET) == -1) {
        close(bank_fd);
        error("ERROR: Could not call lseek() to stretch file.");
    }
    if (write(bank_fd, "", 1) != 1) {
        close(bank_fd);
        error("ERROR: Could not write last byte of the file.");
    }
    bank = mmap(0, 20*sizeof(account_t), PROT_READ | PROT_WRITE, MAP_SHARED, bank_fd, 0);
    if (bank == MAP_FAILED) {
        close(bank_fd);
        error("ERROR: Could not map to file.");
    }
    // End of memory-map code segment.

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

