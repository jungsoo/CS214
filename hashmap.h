#ifndef HASHMAP_H
#define HASHMAP_H

/**
 * A hash function for hashing strings to integer values.
 */
unsigned long hash(char *);

/**
 * Definition for a basic item in the hash map. Also doubles as a linked list
 * node with a reference to the next item.
 */
struct Item {
    void *key;
    void *value;
    struct Item *next;
};

typedef struct Item Item;

/**
 * Creates a new item with the given data attributes. Returns a pointer to the
 * new item, or NULL if the call fails.
 */
Item *create_item(void *, void *, Item *);

/**
 * Destroys and frees all memory associated with the given item.
 */
void destroy_item(Item *);

/**
 * A structure for a hash map for storing variable types of data. Collisions are
 * handled by chaining the colliding items into a linked list. As this is a
 * simple hash map, there is no load balancing and no rehashing.
 */
struct HashMap {
    Item *map;
    int capacity;
    int size;
};

typedef struct HashMap HashMap;

/**
 * Creates a new hash map with the given capacity. Returns a pointer to the new
 * hash map, or NULL if memory allocation failed.
 */
HashMap *create_hashmap(int);

/**
 * Hashes an item into the hash map. Returns zero on error and a positive
 * integer on success.
 */
int put(HashMap *, void *, void *);

/**
 * Gets the item in the hash map with the given key, or NULL if the item cannot
 * be found.
 */
void *get(HashMap *, void *);

/**
 * Removes the item with the given key value from the hash map. Returns a
 * pointer to the deleted object, or NULL if it does not exist in the map.
 */
void *remove(HashMap *, void *);

/**
 * Frees all dynamic memory associated with the given hash map. Note that the
 * use of all iterators associated with the hashmap after its destruction is
 * extremely unsafe.
 */
void destroy_hashmap(HashMap *);

/**
 * Definition for a simple iterator for a hash map.
 */
struct Iterator {
    Item *ptr;
    int current;
}

typedef struct Iterator Iterator;

/**
 * Creates a new iterator for the hash map. Returns a pointer to the iterator,
 * or NULL if the call fails.
 */
Iterator *create_iterator(HashMap *);

/**
 * Returns the next item in the iteration, or NULL if the end of the hash map
 * has been reached.
 */
Item *next_item(Iterator *);

/**
 * Frees all memory associated with the given iterator.
 */
void destroy_iterator(Iterator *);

#endif
