#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "hashmap.h"
#include "tokenizer.h"
#include <dirent.h>
#include <stdio.h>
#include <sys/types.h>

/**
 * Type definition for a basic controller. Allows for flexibility of background
 * model changes without affecting the front end.
 */
struct Controller {
    HashMap *hashmap;
};

typedef struct Controller Controller;

/**
 * Creates a new controller. Returns a pointer to the new struct, or NULL if the
 * call fails.
 */
Controller *create_controller();

/**
 * Destroys and frees all memory associated with the controller.
 */
void destroy_controller(Controller *);

/**
 * Indexes a single file into the inverted index.
 */
int index_file(Controller *, FILE *);

/**
 * Recursively indexes all files in the directory into the inverted index.
 */
int index_dir(Controller *, DIR *);

/**
 * Dumps the contents of the inverted index into the target file.
 */
int dump(Controller *, FILE *);

/**
 * Determines whether or not a filename refers to a regular file.
 */
int is_file(const char *);

/**
 * Determines whether or not a pathname refers to a valid directory.
 */
int is_directory(const char *);
#endif
