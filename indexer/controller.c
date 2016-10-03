#include "controller.h"
#include "inverted-index.h"
#include "record.h"
#include "sorted-list.h"
#include "tokenizer.h"
#include <ctype.h>
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
        else {
            free(controller);
        }
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
 * Indexes a single file into the inverted index.
 */
void index_file(Controller *controller, const char *filename) {
    FILE *file;
    TokenizerT *tokenizer;
    char *text, *token;
    size_t nmemb;

    file = fopen(filename, "r");
    if (!file) {
        fprintf(stderr, "Failed to open '%s' for indexing.\n", filename);
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
    }
    else {
        fprintf(stderr, "An error occurred while allocating memory.\n");
    }
}

/**
 * Recursively indexes all files in the directory into the inverted index.
 */
void index_dir(Controller *controller, const char *dirname) {
    DIR *dir;
    struct dirent *dent;

    dir = opendir(dirname);
    if (!dir) {
        fprintf(stderr,
                "Failed to open directory '%s' for indexing.\n",
                dirname);
    }
    else {
        // An implementation of dirwalk
        while ((dent = readdir(dir))) {
            size_t size = strlen(dent->d_name) + strlen(dirname) + 2;
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
                index_file(controller, name);
            }
        }
        closedir(dir);
    }
}

/**
 * Dumps the contents of the inverted index into the target file. If an
 * inverted-index file has been created, return a positive number; if no tokens
 * were found, return 0.
 */
int is_first_tok = 1;
int dump(Controller *controller, FILE *target) {
    if (controller->index && controller->index->root) {
        fprintf(target, "{\"list\" : [\n");
        dump_helper(controller->index->root, target);
    } else {
        return 0;
    }
    fprintf(target, "\n]}\n");
    return 1;
}

void dump_helper(TrieNode *node, FILE *target) {

    if (node->records) {
        SortedListIterator *iterator = create_iter(node->records);
        Record *record;
        if (is_first_tok) {
            fprintf(target, "\t{\"%s\" : [\n", node->substring);
            is_first_tok = 0;
        } else {
            fprintf(target, ",\n\t{\"%s\" : [\n", node->substring);
        }
    
        if ((record = next_item(iterator))) {
            fprintf(target, "\t\t{\"%s\" : %d}", record->filename, record->hits);
        }
        while ((record = next_item(iterator))) {
            fprintf(target, ",\n\t\t{\"%s\" : %d}", record->filename, record->hits);
        }
        fprintf(target, "\n\t]}");
        destroy_iter(iterator);
    }

    int i;
    for (i = 0; i < 36; i++) {
        if (node->children[i]) {
            dump_helper(node->children[i], target);
        }
    }
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

