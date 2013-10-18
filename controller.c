#include "controller.h"
#include "inverted-index.c"
#include "record.c"
#include "sorted-list.c"
#include "tokenizer.c"
#include <dirent.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>

/**
 * Creates a new controller. Returns a pointer to the new struct, or NULL if the
 * call fails.
 */
Controller *create_controller() {
    Controller *controller = (Controller *) malloc(sizeof(struct Controller));
    if (controller) {
        controller->index = create_index();
        if (controller->index) {
            return controller;
        }
        else
            free(controller);
    }

    return NULL;
}

/**
 * Destroys and frees all memory associated with the controller.
 */
void destroy_controller(Controller *controller) {
    if (controller) {
        destroy_index(controller->index);
        free(controller);
    }
}

/**
 * Converts a string to lowercase.
 */
void strtolower(char *string) {
    int i;
    for (i = 0; i < strlen(string); i++) {
        string[i] = tolower(string[i]);
    }
}

/**
 * Indexes a single file into the inverted index. Returns zero on failure and a
 * positive number on success.
 */
int index_file(Controller *controller, const char *filename) {
    FILE *file;
    TokenizerT *tokenizer;
    char *text, *token;
    size_t nmemb;

    file = fopen(filename, "r");
    if (!file) {
        return 0;
    }

    // First, read in the file as a string
    fseek(file, 0, SEEK_END);
    nmemb = ftell(file);
    rewind(file);
    text = (char *) malloc(nmemb * sizeof(char));
    fread(text, sizeof(char), nmemb, file);
    fclose(file);

    // Next, tokenize the string and add it to the index
    tokenizer = TKCreate(text);
    if (tokenizer) {
        while ((token = TKGetNextToken(tokenizer)) != NULL) {
            strtolower(token);
            put_record(controller->index, token, filename);
        }
        TKDestroy(tokenizer);
        return 1;
    }
    else
        return 0;
}

/**
 * Recursively indexes all files in the directory into the inverted index.
 * Returns zero on failure and a positive number on success.
 */
int index_dir(Controller *controller, const char *dirname) {
    DIR *dir;
    FILE *file;
    struct dirent *dent;

    dir = opendir(dirname);
    if (!dir) {
        return 0;
    }

    while ((dent = readdir(dir))) {
        size_t size = strlen(dent->d_name) + strlen(dirname);
        char *name = calloc(size, sizeof(char));
        snprintf(name, size, "%s/%s", dirname, dent->d_name);

        if (strncmp(dent->d_name, ".", sizeof(char)) == 0) {
            // Hidden file
            continue;
        }
        else if (is_directory(name)) {
            // Recurse
            index_dir(controller, name);
        }
        else if(is_file(name)) {
            // Index this file
            if (!index_file(controller, name)) {
                fprintf(stderr, "Error while indexing %s\n", name);
            }
        }
    }

    closedir(dir);
    return 1;
}

/**
 * Dumps the contents of the inverted index into the target file. Returns zero
 * on error and a positive number on success.
 */
int dump(Controller *controller, FILE *target) {
    // TODO gdb
    Record *record;
    SortedList *list;
    SortedListIterator *iterator;
    char *token;
    int i, first;

    first = 1;

    // Loop through each list
    for (i = 0; i < 36; i++) {
        token = NULL;
        list = controller->index->lists[i];
        if (!list) {
            continue;
        }

        // Loop through each node in the list
        // TODO check for correctness before debugging
        iterator = create_iter(list);
        while ((record = next_item(iterator)) != NULL) {
            if (token == NULL || strcmp(record->token, token) != 0) {
                // Moving on to the next token
                if (!first) {
                    fprintf(target, "\n</list>\n");
                }

                first = 0;
                token = record->token;
                fprintf(target, "<list> %s\n%s %d", token,
                        record->filename, record->hits);
            }
            else {
                // Continuing with the current token
                fprintf(target, " %s %d", record->filename, record->hits);
            }
        }
        destroy_iter(iterator);
    }

    return 1;
}

/**
 * Returns a positive number if the filename is a readable file; zero otherwise.
 */
int is_file(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file) {
        fclose(file);
        return 1;
    }
    else {
        return 0;
    }
}

/**
 * Returns a positive number if the directory name refers to a valid directory;
 * zero otherwise.
 */
int is_directory(const char *dir) {
    DIR *directory = opendir(dir);
    if (directory) {
        closedir(directory);
        return 1;
    }
    else {
        return 0;
    }
}
