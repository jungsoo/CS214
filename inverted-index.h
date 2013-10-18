#ifndef INDEX_H
#define INDEX_H

#include "sorted-list.h"

/**
 * A hash function for hashing strings to integer values.
 */
int hash(const char *);

/**
 * A structure represented an inverted index. It's an array of sorted lists,
 * which stores information about tokens in sorted order.
 */
struct Index {
    SortedList *lists[36];
};

typedef struct Index Index;

/**
 * Returns a pointer to a new inverted index, or NULL if the call fails.
 */
Index *create_index();

/**
 * Adds or updates another record for the given key.
 */
int put_record(Index *, const char *, const char *);

/**
 * Frees all dynamic memory associated with the given hash map. Note that the
 * use of all iterators associated with the index after its destruction is
 * extremely unsafe.
 */
void destroy_index(Index *);

#endif
