#ifndef HASHMAP_H
#define HASHMAP_H

/**
 * A hash function for hashing strings to integer values.
 */
unsigned int hash(const char *);

/**
 * A structure for a hash map for storing data as strings. As this is a simple
 * hash map, there is no load balancing and no rehashing.
 */
struct HashMap {
    SortedList **lists;
    int capacity;
};

typedef struct HashMap HashMap;

/**
 * Creates a new hash map with the given capacity. Returns a pointer to the new
 * hash map, or NULL if memory allocation failed.
 */
HashMap *create_hashmap(int);

/**
 * Adds or updates another record for the given key. Returns a nonnegative
 * integer on success and -1 if the call fails.
 */
int put_record(HashMap *, const char *, const char *);

/**
 * Returns a sorted list containing all records pertaining to a given token. If
 * the token does not exist in the hashmap, this function returns NULL.
 */
SortedList *get_records(HashMap *, const char *);

/**
 * Frees all dynamic memory associated with the given hash map. Note that the
 * use of all iterators associated with the hashmap after its destruction is
 * extremely unsafe.
 */
void destroy_hashmap(HashMap *);

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
Iterator *create_hmiter(HashMap *);

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
