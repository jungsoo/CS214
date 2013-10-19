#include "inverted-index.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

/**
 * A custom hash function that returns an index based on a string's first
 * letter. Returns a valid index for the inverted index, or -1 if the string
 * starts with an inappropriate character (something non-alphanumeric).
 */
int hash(const char *str) {
    if isalpha(str[0]) {
        return (int) (tolower(str[0]) - 'a');
    }
    else if (isdigit(str[0])) {
        return (int) (str[0] - '0') + 26;
    }
    else
        return -1;
}

/**
 * Returns a pointer to a new inverted index, or NULL if the call fails.
 */
Index *create_index() {
    return (Index *) malloc(sizeof(struct Index));
}

/**
 * Adds or updates another record for the given token in the inverted index.
 */
int put_record(Index *index, const char *tok, const char *fname) {
    SortedList *list;
    int i;

    i = hash(tok);
    if (i == -1) {
        // This isn't a valid token and has no place in the index.
        printf("Token is not valid; skipping.\n");
        return 0;
    }
    else {
        // Simply add or update the appropriate sorted list.
        list = index->lists[i];
        if (list == NULL) {
            printf("Created a new sorted list for this letter.\n");
            index->lists[i] = create_sortedlist(reccmp);
        }
        return insert_sortedlist(index->lists[i], tok, fname);
    }
}

/**
 * Frees all dynamic memory associated with the given hash map. Note that the
 * use of all iterators associated with the index after its destruction is
 * extremely unsafe.
 */
void destroy_index(Index *index) {
    int i;

    for (i = 0; i < 36; i++) {
        destroy_sortedlist(index->lists[i]);
    }
    free(index);
}
