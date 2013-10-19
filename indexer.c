#include "controller.h"
#include <stdio.h>
#include <stdlib.h>

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
    if (argc == 2 && strcmp(argv[1], "-h")) {
        show_usage();
        return 0;
    }
    else if (argc != 3) {
        fprintf(stderr, "index: error: unexpected number of arguments.\n");
        show_usage();
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
                "index: error: '%s' is not a regular file or directory\n",
                argv[2]);
        return 1;
    }

    // Finally, dump the inverted index to file
    result = dump(controller, indexfile);
    destroy_controller(controller);
    fclose(indexfile);
    return result;
}
