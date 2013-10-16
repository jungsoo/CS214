#include "hashmap.c"
#include "tokenizer.c"
#include <dirent.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>

#define MAXBUFSIZE 10000
#define TRUE 1
#define FALSE 0

/**
 * Creates a new controller. Returns a pointer to the new struct, or NULL if the
 * call fails.
 */
Controller *create_controller() {
    Controller *controller;
    HashMap *hashmap;

    controller = (Controller *) malloc(sizeof(struct Controller));
    if (controller) {
        hashmap = create_hashmap(100);
        if (hashmap) {
            controller->hashmap = hashmap;
        }
        else {
            free(controller);
            return NULL;
        }
    }
    else
        return NULL;
}

/**
 * Destroys and frees all memory associated with the controller.
 */
void destroy_controller(Controller *controller) {
    if (controller) {
        destroy_hashmap(controller->hashmap);
        free(controller);
    }
}

/**
 * Indexes a single file into the inverted index. Returns zero on failure and a
 * positive number on success.
 */
int index_file(Controller *controller, FILE *file) {
    TokenizerT *tokenizer;
    char *text, token;
    size_t nmemb;

    // First, read in the file as a string
    fseek(file, 0, SEEK_END);
    nmemb = ftell(file);
    fseek(file, 0 , SEEK_SET);
    text = (char *) malloc(nmemb * sizeof(char));
    fread(text, sizeof(char), nmemb, file);
    fclose(file);

    // Next, tokenize the string and add it to the index
    tokenizer = TKCreate(text);
    if (tokenizer) {
        while ((token = TKGetNextToken(tokenizer)) != NULL) {
            // TODO
        }
    }

    TKDestroy(tokenizer);
    return 0;
}

/**
 * Recursively indexes all files in the directory into the inverted index.
 * Returns zero on failure and a positive number on success.
 */
int index_dir(Controller *controller, DIR *dir) {
    DIR *subdir;
    FILE *file;
    struct dirent *dent;
    int retval;

    retval = 1;
    while ((dent = readdir(dir))) {
        if (strncmp(dent->d_name, ".", sizeof(char)) == 0) {
            // Hidden file
            continue;
        }
        else if (is_directory(dent->dname)) {
            // Recurse
            subdir = opendir(dent->dname);
            if (subdir) {
                index_dir(controller, opendir(dent->dname));
            }
            else {
                retval = 0;
                break;
            }
        }
        else if(is_file(dent->d_name)) {
            // Compatible with non-Linux, non-BSD by not using d_type
            if (!index_file(controller, dent->dname)) {
                retval = 0;
                break;
            }
        }
        else
            continue;
    }

    closedir(dir);
    return retval;
}

/**
 * Dumps the contents of the inverted index into the target file. Returns zero
 * on error and a positive number on success.
 */
int dump(Controller *controller, FILE *target) {
    // TODO
    char *buffer;

    buffer = (char *) calloc(MAXBUFSIZE, sizeof(char));
    if (!buffer) {
        return 0;
    }

    
    return 1;
}

/**
 * Returns a positive number if the filename is a readable file; zero otherwise.
 */
int is_file(const char *file) {
    struct stat s;
    if (stat(file, &s) == 0 && s.st_mode & S_IFREG) {
        return 1;
    }
    else {
        return 0
    }
}

/**
 * Returns a positive number if the directory name refers to a valid directory;
 * zero otherwise.
 */
int is_directory(const char *dir) {
    struct stat s;
    if (stat(file, &s) == 0 && s.st_mode & S_IFDIR) {
        return 1;
    }
    else {
        return 0
    }
}
