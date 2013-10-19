#include "controller.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

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
    Controller *controller;
    FILE *indexfile;
    int result;

    // Check for proper input
    if (argc == 2) {
        if (strcmp(argv[1], "-h") == 0 || strcmp(argv[1], "--help") == 0) {
            show_usage();
            return 0;
        }
        else {
            fprintf(stderr, "index: error: Too few arguments.\n");
            show_usage();
            return 1;
        }
    }
    else if (argc != 3) {
        fprintf(stderr, "index: error: Unexpected number of arguments.\n");
        show_usage();
        return 1;
    }

    // Cannot have the indexed file and target be the same
    if (strcmp(argv[1], argv[2]) == 0) {
        fprintf(stderr, "index: error: Target index file and file to be"
                " indexed are the same.\n");
        return 1;
    }

    // See if the index file exists
    if (access(argv[1], F_OK) != -1) {
        fprintf(stderr,
                "index: error: Target index file '%s' already exists.\n",
                argv[1]);
        return 1;
    }

    // Create the controller
    controller = create_controller();
    if (!controller) {
        fprintf(stderr, "An error occurred while allocating memory.\n");
        return 1;
    }

    // Open the index file for writing
    indexfile = fopen(argv[1], "w");
    if (!indexfile) {
        fprintf(stderr,
                "index: error: Failed to open index file '%s' for writing\n",
                argv[1]);
        destroy_controller(controller);
        return 1;
    }

    // Next, perform the indexing
    if (is_directory(argv[2])) {
        index_dir(controller, argv[2]);
    }
    else if (is_file(argv[2])) {
        index_file(controller, argv[2]);
    }
    else {
        fprintf(stderr,
                "index: error: '%s' is not a regular file or directory.\n",
                argv[2]);
        fclose(indexfile);
        destroy_controller(controller);
        return 1;
    }

    // Finally, dump the inverted index to file
    result = dump(controller, indexfile);
    if (result) {
        printf("Output written to %s.\n", argv[1]);
    }
    else {
        printf("indexer: warning: No tokens found. Nothing has been written to"
               "the output file.\n");
    }

    // Clean stuff up before exiting
    destroy_controller(controller);
    fclose(indexfile);
    return result;
}
