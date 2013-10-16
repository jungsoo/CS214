#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include "controller.c"

/**
 * Prints this program's usage to standard out.
 */
void show_usage(void) {
    printf("Usage: index <index-filename> [<filename> | <dirname>]\n");
}

/**
 * Runs the program.
 */
int main(int argc, char **argv) {
    FILE *indexfile, *targetfile;
    DIR *targetdir;
    Controller *controller;

    if (argc == 2 && strcmp(argv[1], "-h")) {
        show_usage();
        return 0;
    }
    else if (argc != 3) {
        fprintf(stderr, "index: error: unexpected number of arguments.\n");
        show_usage();
        return 1;
    }

    indexfile = fopen(argv[1], "w");
    if (!indexfile) {
        fprintf(stderr, "index: error: Could not access index file %s\n",
                    argv[1]);
        return 1;
    }

    targetdir = opendir(argv[2]);
    if (targetdir) {
        // Index the directory
        // TODO dirwalk
    }
    else {
        // Not a directory
        targetfile = fopen(argv[2], "r");
        if (targetfile) {
            // TODO index the file
        }
        else {
            fprintf(stderr, "index: error: Could not access target file "
                            "or directory\n%s", argv[2]);
            return 1;
        }
    }

    return 0;
}
