#include <string.h>

#define __SERVER_PORT__ 6694

void error(const char *msg) {
    perror(msg);
    exit(1);
}

int streq(char *a, char *b) {
    return strcmp(a, b) == 0;
}
