#include <errno.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <pthread.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>

#include "util.h"

#define FILEPATH        "/tmp/bankinfo.bin"
#define MAX_ACCOUNT     (20)
#define MAX_NAME_LEN    (100)
#define FILESIZE        (MAX_ACCOUNT * sizeof(account_t))

typedef struct account {
    pthread_mutex_t mutex;
    char name[MAX_NAME_LEN];
    double balance;
    int is_active;
} account_t;

/*
 * Global Variables
 */
int bank_fd;
account_t *bank;

pthread_mutex_t accounts_mutex = PTHREAD_MUTEX_INITIALIZER;

int open_account(char *accountname) {
    for (int i = 0; i < MAX_ACCOUNT; i++) { 
        if (streq(bank[i].name, accountname)) {
            return -1; // Account name already exists.
        } else if (streq("", bank[i].name)) {
            strncpy(bank[i].name, accountname, MAX_NAME_LEN);
            bank[i].balance = 0;
            bank[i].is_active = 0;

            pthread_mutexattr_t attr;
            pthread_mutexattr_init(&attr);
            pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);
            if (pthread_mutex_init(&bank[i].mutex, &attr)) {
                error("ERROR: Unable to initialize account mutex.");
            }

            return 1; // Account successfully opened.
        }
    }
    return 0; // Bank is at capacity.
}

void client_service(int sock) {
    char request[2048];
    int in_customer_session = 0; // Each client starts off out of customer session.
    int account_index = -1; // Not associated with any account.
    int result;

    char *cmd = (char *)malloc(2048);
    char *arg = (char *)malloc(2048);
    double amount;

    memset(&request, '\0', sizeof(request));

    while(read(sock, request, sizeof(request)) > 0) {
        printf("(%d): %s\n", getpid(), request);
        
        *cmd = '\0';
        *arg = '\0';

        sscanf(request, "%s %s", cmd, arg);

        if (!cmd || !strlen(cmd)) {
            write(sock, request, sprintf(request, "\tBANK: Invalid syntax.\n") + 1);
            continue;
        } else if (streq(cmd, "open")) {
			if (!arg || !strlen(arg)) {
				write(sock, request, sprintf(request, "\tBANK: Invalid syntax.\n") + 1);
                continue;
			}
            pthread_mutex_lock(&accounts_mutex);
            result = open_account(arg);
            if (result == 0) {
				write(sock, request, sprintf(request, "\tBANK: Twenty accounts exist already; cannot create another.\n") + 1);
            } else if (result == -1) {
				write(sock, request, sprintf(request, "\tBANK: Cannot create account %s - account name exists.\n", arg) + 1);
            } else {
                write(sock, request, sprintf(request, "\tBANK: Successfully added account %s.\n", arg) + 1);
            }
            pthread_mutex_unlock(&accounts_mutex);

		} else if (streq(cmd, "start")) {
			if (!arg || !strlen(arg)) {
				write(sock, request, sprintf(request, "\tBANK: Invalid syntax.\n") + 1);
                continue;
			} else if (in_customer_session) {
				write(sock, request, sprintf(request, "\tBANK: Already serving account %s.\n", arg) + 1);
			} 
            
            // Iterate through bank to find the account.
            account_index++;
            for (; account_index <= MAX_ACCOUNT; account_index++) {
                if (streq(arg, bank[account_index].name)) {
                    break;
                }
            }

            if (account_index >= MAX_ACCOUNT) {
                account_index = -1;
				write(sock, request, sprintf(request, "\tBANK: Account %s does not exist.\n", arg) + 1);
                continue;
            } else if (bank[account_index].is_active) {
				write(sock, request, sprintf(request, "\tBANK: Account %s is already in use.\n", arg) + 1);

                // Try to lock every 2 seconds.
                while(pthread_mutex_trylock(&bank[account_index].mutex) != 0){
                    printf("%d: Attempting to lock account %s...\n", getpid(), arg);
                    write(sock, request, sprintf(request, "\tBANK: Waiting to start custom session for account %s...\n", arg) + 1);
                    sleep(2);
                }
			}
            
            in_customer_session = 1;
			bank[account_index].is_active = 1;
            pthread_mutex_lock(&bank[account_index].mutex);
			write(sock, request, sprintf(request, "\tBANK: Successfully connected to account %s.\n", arg) + 1);

		} else if (streq(cmd, "credit")) {
            if (!in_customer_session) {
				write(sock, request, sprintf(request, "\tBANK: Not currently in a customer session.\n") + 1);
            } else if (!arg || !strlen(arg)) {
				write(sock, request, sprintf(request, "\tBANK: Invalid syntax.\n") + 1);
                continue;
			} else if (!(amount = atof(arg)) || (bank[account_index].balance + amount) < 0) {
				write(sock, request, sprintf(request, "\tBANK: Not a valid deposit.\n") + 1);
			} else {
				write(sock, request, sprintf(request, "\tBANK:\n\t\tOld Balance:\t$%.2lf\n\t\tDepositing:\t$%.2lf\n\t\tNew Balance:\t$%.2lf\n", bank[account_index].balance, amount, (bank[account_index].balance + amount)) + 1);
				bank[account_index].balance += amount;
			}

		} else if (streq(cmd, "debit")) {
            if (!in_customer_session) {
				write(sock, request, sprintf(request, "\tBANK: Not currently in a customer session.\n") + 1);
            } else if (!arg || !strlen(arg)) {
				write(sock, request, sprintf(request, "\tBANK: Invalid syntax.\n") + 1);
                continue;
			} else if (!(amount = atof(arg)) || (bank[account_index].balance - amount) < 0) {
				write(sock, request, sprintf(request, "\tBANK: Not a valid withdrawal.\n") + 1);
			} else {
				write(sock, request, sprintf(request, "\tBANK:\n\tOld Balance:\t$%.2lf\n\tWithdrawing:\t$%.2lf\n\tNew Balance:\t$%.2lf\n", bank[account_index].balance, amount, (bank[account_index].balance - amount)) + 1);
				bank[account_index].balance -= amount;
			}

		} else if (streq(cmd, "balance")) {
			if (!in_customer_session) {
				write(sock, request, sprintf(request, "\tBANK: Not currently in a customer session.\n") + 1);
            } else if (arg && strlen(arg)) {
				write(sock, request, sprintf(request, "\tBANK: Invalid syntax.\n") + 1);
                continue;
			} else {
				write(sock, request, sprintf(request, "\tBANK:\n\tBalance: $%.2lf\n", bank[account_index].balance) + 1);
			}

		} else if (streq(cmd, "finish")) {
			if (!in_customer_session) {
				write(sock, request, sprintf(request, "\tBANK: Not currently in a customer session.\n") + 1);
            } else {
				write(sock, request, sprintf(request, "\tBANK: Ending customer session to account %s.\n", bank[account_index].name) + 1);
				bank[account_index].is_active = 0;
                in_customer_session = 0;
				pthread_mutex_unlock(&bank[account_index].mutex);
                account_index = -1;
				printf("\tBANK: Ended customer session and unlocked account mutex.\n");
			}
		} else if (streq(cmd, "exit")) {

            if (in_customer_session) {
                bank[account_index].is_active = 0;
                pthread_mutex_unlock(&bank[account_index].mutex);
            }

			write(sock, request, sprintf(request, "\tBANK: Disconnecting from server and ending client process.\n") + 1);
			break; // End client-service process.
		} else {
			write(sock, request, sprintf(request, "Invalid syntax. Proper commands:\nopen <account_name> \tcreates an account\nstart <account_name>\tstarts a customer session\ncredit <amount>\t\tdebit amount into account\ndebit <amount>\t\twithdraws amount from account\nbalance\t\t\tprints account balance\nfinish\t\t\tends customer session\nexit\t\t\tends client session\n") + 1);
		}
    }
    
    free(cmd);
    free(arg);

    close(sock);
}

void print_account_info(int acc_index) {
    printf("\tAccount Name: %s\n", bank[acc_index].name);
    printf("\tBalance: $%.2lf\n", bank[acc_index].balance);
    if (bank[acc_index].is_active) {
        printf("\tIN SESSION\n");
    }
    printf("\n");
}

void print_bank_info(int signum) {
    printf("--- BANK OF JUNGSOO ---\n\n");
    for (int i = 0; i < MAX_ACCOUNT; i++) {
        if (!streq(bank[i].name, "")) {
            print_account_info(i);
            continue;
        }
        if (i == 0) {
            printf("...Bank is empty. :(\n\n");
        }
    }
    printf("--------- END ---------\n\n");
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
    if (shmdt(bank) == -1) {
        error("ERROR: Unable to shmdt the bank.");
    }
    close(bank_fd);
    printf("\n");
    exit(0);
}

int main() {
    int listen_fd = socket(AF_INET, SOCK_STREAM, 0), connection_fd;
    if (listen_fd < 0) {
        error("ERROR: Unable to open socket.");
    }
    
    struct sockaddr_in serv_addr, cli_addr;
    memset(&serv_addr, '\0', sizeof(serv_addr));

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(__SERVER_PORT__);

    // Display server information every 20 seconds.
    struct sigaction timer;
    sigemptyset(&timer.sa_mask);
    timer.sa_flags = 0;
    timer.sa_handler = print_bank_info;
    sigaction(SIGALRM, &timer, NULL);

    set_timer(20);

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

    // Shared Memory...
    key_t key = ftok(FILEPATH, 'R');
    int shmid = shmget(key, 2048, 0644 | IPC_CREAT);
    bank = shmat(shmid, (void *)0, 0);

    pid_t pid;
    socklen_t cli_len = sizeof(cli_addr);
    while (1) {
        connection_fd = accept(listen_fd, (struct sockaddr *) &cli_addr, &cli_len);
        if (connection_fd < 0) {
            if (errno == EINTR) {
                // Ignore signals, like the alarm signal, as they are not errors.
                continue;
            } else {
                error("ERROR: Unable to accept.");
            }
        }
        
        pid = fork();
        if (pid < 0) {
            error("ERROR: fork() failed!");
        } else if (pid == 0) {
            close(listen_fd);
            printf("SERVER: Connected to a new client (PID: %d)!\n", getpid());
            client_service(connection_fd);
            exit(0);
        } else {
            close(connection_fd);
        }
    }

    close(listen_fd);
    return 0;
}

