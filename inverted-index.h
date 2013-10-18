#ifndef INDEX_H
#define INDEX_H

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
int put_record(const char *, const char *);

/**
 * Returns a sorted list containing all records pertaining to a given token. If
 * the token does not exist in the index, this function returns NULL.
 */
SortedList *get_records(Index *, const char *);

/**
 * Frees all dynamic memory associated with the given hash map. Note that the
 * use of all iterators associated with the index after its destruction is
 * extremely unsafe.
 */
void destroy_index(Index *);

/**
 * TODO
 * Definition for a simple iterator for a hash map.
 */
struct Iterator {
    SortedList *ptr;
    int capacity;
    int current;
};

typedef struct Iterator Iterator;

/**
 * Creates a new iterator for the hash map. Returns a pointer to the iterator,
 * or NULL if the call fails.
 */
Iterator *create_hmiter(Index *);

/**
 * Returns the next list in the iteration, or NULL if the end of the hash map
 * has been reached.
 */
SortedList *next_list(Iterator *);

/**
 * Frees all memory associated with the given iterator.
 */
void destroy_hmiter(Iterator *);

#endif
