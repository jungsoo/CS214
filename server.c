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
#include "util.h"
#include "tokenizer.h"

#define FILEPATH        "/tmp/bankinfo.bin"
#define MAX_ACCOUNT     (20)
#define FILESIZE        (MAX_ACCOUNT * sizeof(account_t))

typedef struct account {
    pthread_mutex_t mutex;
    char name[100];
    double balance;
    int is_active;
} account_t;

/*
 * Global Variables
 */
int bank_fd;
account_t *bank;

pthread_mutex_t accounts_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t clients_mutex = PTHREAD_MUTEX_INITIALIZER;

void open_account(char *accountname) {
    for (int i = 0; i < MAX_ACCOUNT; i++) { 
        if (streq(bank[i].name, accountname)) {
            return -2; // Account name already exists.
        }
        if (bank[i].name == NULL) {
            bank[i].name = accountname;
            bank[i].balance = 0;
            bank[i].is_active = 0;

            pthread_mutexattr_t attr;
            pthread_mutexattr_init(&attr);
            pthread_mutexattr_settype(%attr, PTHREAD_MUTEX_RECURSIVE);
            if (pthread_mutex_init(&bank[i].mutex, &attr)) {
                error("ERROR: Unable to initialize account mutex.");
            }

            return 1; // Account successfully opened.
        }
    }
    return -1; // Bank is at capacity.
}

void client_service(int sock) {
    char request[2048];
    int in_customer_session = 0; // Each client starts off out of customer session.
    Tokenizer tokenizer;
    int result;
    char *cmd;
    char *accountname;

    memset(&request, '0', sizeof(request));

    while(read(sock, request, sizeof(request)) > 0) {
        printf("(%d): %s\n", getpid(), request);

        tokenizer = createTokenizer(request);

        cmd = getNextToken(tokenizer);
		if (streq(cmd, "open")) {
			if (!(accountname = getNextToken(tokenizer)) || getNextToken(tokenizer)) {
				write(sock, request, sprintf(request, "BANK: Invalid syntax.\n") + 1);
			}
            pthread_mutex_lock(&accounts_mutex);
            result = open_account(accountname);
            if (result == -1) {
				write(sock, request, sprintf(request, "BANK: Twenty accounts exist already; cannot create another.\n") + 1);
            } else if (result == -2) {
				write(sock, request, sprintf(request, "BANK: Cannot create account %s - account name exists.\n", accountname) + 1);
            } else {
                write(sock, request, sprintf(request, "BANK: Successfully added account %s.\n", acc->name) + 1);
            }
            pthread_mutex_unlock(&accounts_mutex);
		} else if (streq(cmd, "start")) {
			if (!(accountname = getNextToken(tokenizer)) || getNextToken(tokenizer)) {
				write(sock, request, sprintf(request, "BANK: Invalid syntax.\n") + 1);
				continue;
			}
            // Iterate through bank to find the account.
            int account_index = 0;
            for (; account_index <= MAX_ACCOUNT; account_index++) {
                if (streq(accountname, bank[account_index].name)) {
                    break;
                }
            }

            if (in_customer_session) {
				write(sock, request, sprintf(request, "BANK: Already serving account %s.\n", accountname) + 1);
			} else if (account_index >= MAX_ACCOUNT) {
				write(sock, request, sprintf(request, "BANK: Account %s does not exist.\n", accountname) + 1);
            } else if (bank[account_index].is_active) {
				write(sock, request, sprintf(request, "BANK: Account %s is already in use.\n", accountname) + 1);
			}
            // TODO: ugh
            
			while(pthread_mutex_trylock(&acc->mutex) != 0){
				printf("Error attempting to lock already locked mutex: %d\n", pthread_mutex_trylock(&acc->mutex));
				write(sock, request, sprintf(request, "Bank: Waiting to start customer session for account %s.\n", token) + 1);
				sleep(2);
			}
			
			client->acc = acc;
			client->acc->active = true;

			write(sock, request, sprintf(request, "Bank: Successfully connected to account %s.\n", token) + 1);
		} 
		else if (streq(token, "deposit")) {
			if (!client->acc || !client->acc->active) {
				write(sock, request, sprintf(request, "Bank: Not currently in a customer session.\n") + 1);
			}
			else if (!(token = getNextToken(tokenizer)) || getNextToken(tokenizer)) 			{
				write(sock, request, sprintf(request, "Bank: Invalid syntax.\n") + 1);
			} 
			else if (!(change = atof(token)) || change <= 0) {
				write(sock, request, sprintf(request, "Bank: Not a valid deposit.\n") + 1);
			}
			else {
				write(sock, request, sprintf(request, "Old Balance:\t$%.2lf\nDepositing:\t$%.2lf\nNew Balance:\t$%.2lf\n", client->acc->balance, change, (client->acc->balance + change)) + 1);
				client->acc->balance += change;
			}
		} 
		else if (streq(token, "withdraw")) {
			if (!client->acc || !client->acc->active) {
				write(sock, request, sprintf(request, "Bank: Not currently in a customer session.\n") + 1);
			}
			else if (!(token = getNextToken(tokenizer)) || getNextToken(tokenizer)) {
				write(sock, request, sprintf(request, "Bank: Invalid syntax.\n") + 1);
			}
			else if (!(change = atof(token)) || change <= 0) {
				write(sock, request, sprintf(request, "Bank: Not a valid withdrawal.\n") + 1);
			}
			else if (change > client->acc->balance) {
				write(sock, request, sprintf(request, "Bank: Not enough funds to withdraw.\n") + 1);
			}
			else {
				write(sock, request, sprintf(request, "Old Balance:\t$%.2lf\nDepositing:\t$%.2lf\nNew Balance:\t$%.2lf\n", client->acc->balance, change, (client->acc->balance - change)) + 1);
				client->acc->balance -= change;
			}
		} 
		else if (streq(token, "query")) {
			if (!client->acc || !client->acc->active) {
				write(sock, request, sprintf(request, "Bank: Not currently in a customer session.\n") + 1);
			}
			else if (getNextToken(tokenizer)) {
				write(sock, request, sprintf(request, "Bank: Invalid syntax.\n") + 1);
			}
			else {
				write(sock, request, sprintf(request, "Balance: $%.2lf\n", client->acc->balance) + 1);
			}
		} 
		else if (streq(token, "end")) {
			if (!client->acc || !client->acc->active) {
				write(sock, request, sprintf(request, "Bank: Not currently in a customer session.\n") + 1);
			} else {
				write(sock, request, sprintf(request, "Bank: Ending customer session to account %s.\n", client->acc->name) + 1);
				client->acc->active = false;
				pthread_mutex_unlock(&client->acc->mutex);
				client->acc = NULL;
			
				printf("Ended customer session and unlocked account mutex\n");
			}
		} 
		else if (streq(token, "quit")) {
			write(sock, request, sprintf(request, "Bank: Disconnecting from Server and ending Client thread.\n") + 1);

			if (client->acc){
				client->acc->active = false;
				pthread_mutex_unlock(&client->acc->mutex);
			}

			pthread_mutex_lock(&clients_mutex);
			removeLinkedNode(client, clients);
			pthread_mutex_unlock(&clients_mutex);
			closeClient(client);
			close(sock);

			destroyTokenizer(tokenizer);
			//Break out of this while loop and return NULL effectively ending the thread.
			break;
		} 
		else {
			write(sock, request, sprintf(request, "Invalid syntax. Proper commands:\ncreate <account_name> \tcreates an account\nserve <account_name>\tstarts a customer session\ndeposit <amount>\tdeposits amount into account\nwithdraw <amount>\twithdraws amount from account\nquery\t\t\tprints account balance\nend\t\t\tends customer session\nquit\t\t\tends client session\n") + 1);
		}

		destroyTokenizer(tokenizer);
	}
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
    if (munmap(bank, FILESIZE) == -1) {
        error("ERROR: Unable to un-mmap the file.");
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
    bank_fd = open(FILEPATH, O_RDWR | O_CREAT | O_TRUNC, (mode_t)0600);
    if (bank_fd == -1) {
        close(bank_fd);
        error("ERROR: Could not open file.");
    }
    if (lseek(bank_fd, FILESIZE, SEEK_SET) == -1) {
        close(bank_fd);
        error("ERROR: Could not call lseek() to stretch file.");
    }
    if (write(bank_fd, "", 1) != 1) {
        close(bank_fd);
        error("ERROR: Could not write last byte of the file.");
    }
    bank = mmap(0, FILESIZE, PROT_READ | PROT_WRITE, MAP_SHARED, bank_fd, 0);
    if (bank == MAP_FAILED) {
        close(bank_fd);
        error("ERROR: Could not map to file.");
    }
    // End of memory-map code segment.

    pid_t pid;
    socklen_t cli_len = sizeof(cli_addr);
    while (1) {
        connection_fd = accept(listen_fd, (struct sockaddr *) &cli_addr, &cli_len);
        if (connection_fd < 0) {
            error("ERROR: Unable to accept.");
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

