#ifndef HASHMAP_H
#define HASHMAP_H

/**
 * A hash function for hashing strings to integer values.
 */
unsigned int hash(const char *);

/**
 * A basic type that stores indexer-related data. It also works as a linked
 * list, chaining together records for the same keyword.
 */
struct Record {
    char *filename;
    int hits;
    struct Record *next;
};

typedef struct Record Record;

/**
 * Creates a new record. Returns a pointer to the new object, or NULL if the
 * call fails.
 */
Record *create_record(const char *, int, Record *);

/**
 * Destroys and frees all memory associated with the given record.
 */
void destroy_record(Record *);

/**
 * Definition for a basic node in the hash map. This implements a
 * two-dimensional linked list, where the record field is another linked list of
 * related records to this node.
 */
struct Node {
    char *key;
    struct Record *records;
    struct Node *next;
};

typedef struct Node Node;

/**
 * Creates a new node with the given data attributes. Returns a pointer to the
 * new node, or NULL if the call fails.
 */
Node *create_node(const char *, struct Record*, Node *);

/**
 * Destroys and frees all memory associated with the given node.
 */
void destroy_node(Node *);

/**
 * A structure for a hash map for storing data as strings. Collisions are
 * handled by chaining the colliding nodes into a linked list. As this is a
 * simple hash map, there is no load balancing and no rehashing.
 */
struct HashMap {
    Node *map;
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
 * Returns a pointer to the head of a linked list containing all of the records
 * of files containing the given key. If the key does not exist in the hashmap,
 * this function returns NULL.
 */
Record *get_records(HashMap *, const char *);

/**
 * Removes the node with the given key value from the hash map. Returns the
 * value of the deleted object, or NULL if it does not exist in the map.
 */
char *removekey(HashMap *, const char *);

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
    Node *ptr;
    int capacity;
    int current;
};

typedef struct Iterator Iterator;

/**
 * Creates a new iterator for the hash map. Returns a pointer to the iterator,
 * or NULL if the call fails.
 */
Iterator *create_iterator(HashMap *);

/**
 * Returns the next node in the iteration, or NULL if the end of the hash map
 * has been reached.
 */
Node *next_node(Iterator *);

/**
 * Frees all memory associated with the given iterator.
 */
void destroy_iterator(Iterator *);

#endif
