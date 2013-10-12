#include <stdlib.h>
#include <string.h>
#include "hashmap.h"

/**
 * The djb2 hash algorithm for hashing strings.
 */
unsigned int hash(const char *str) {
    unsigned int hash = 5381;
    int c;

    while ((c = *str++)) {
        hash = ((hash << 5) + hash) + c;
    }

    return hash;
}

/**
 * Creates a new record. Returns a pointer to the new object, or NULL if the
 * call fails.
 */
Record *create_record(const char *filename, int hits, Record *next) {
    Record *record = (Record *) malloc(sizeof(struct Record));
    if (record) {
        record->key = (char *) malloc(strlen(filename));
        if (!record->key) {
            free(record);
        }
        else {
            strcpy(record->key, filename);
            record->hits = hits;
            record->next = next;
            return record;
        }
    }

    return NULL;
}

/**
 * Destroys and frees all memory associated with the given record.
 */
void destroy_record(Record *record) {
    if (record) {
        free(record->filename);
        free(record);
    }
}

/**
 * Creates a new node with the given data attributes. Returns a pointer to the
 * new node, or NULL if the call fails.
 */
Node *create_node(const char *key, struct Record **value, Node *next) {
    Node *node = (Node *) malloc(sizeof(struct Node));
    if (node) {
        node->key = (char *) malloc(strlen(key));
        if (!node->key) {
            free(node);
        }
        else {
            strcpy(node->key, key);
            node->records = value;
            node->next = next;
            return node;
        }
    }

    return NULL;
}

/**
 * Destroys and frees all memory associated with the given node. This also
 * destroys any and all records associated with it, even if pointers to these
 * records exist elsewhere.
 */
void destroy_node(Node *node) {
    Node *ptr, *next;
    if (node) {
        for (ptr = node->records; ptr; ) {
            next = ptr->next;
            destroy_record(ptr);
            ptr = next;
        }
        free(node->key);
        free(node);
    }
}

/**
 * Creates a new hash map with the given capacity. Returns a pointer to the new
 * hash map, or NULL if memory allocation failed. Specifying a capacity less
 * than one will also cause this method to do nothing and return NULL.
 */
HashMap *create_hashmap(int capacity) {
    if (capacity < 1) {
        return NULL;
    }

    HashMap *hm = (HashMap *) malloc(sizeof(struct HashMap));
    if (hm) {
        hm->map = (Node *) calloc((size_t) capacity, sizeof(struct Node));
        if (!hm->map) {
            free(hm);
            return NULL;
        }
        else {
            hm->capacity = capacity;
            hm->size = 0;
            return hm;
        }
    }
    else
        return NULL;
}

/**
 * Adds or updates another record for the given key. Returns a nonnegative
 * integer on success and -1 if the call fails.
 * @param key - The name of the token.
 * @param value - The filename where the token was found.
 */
int put_record(HashMap *hashmap, const char *key, const char *value) {
    // TODO edit this to take the records and 2x linked list into account
    int index;
    Node *node;

    if (!hashmap) {
        return -1;
    }

    index = hash(key) % hashmap->capacity;
    node = create_node(key, value, (hashmap->map) + index);
    if (!node) {
        return -1;
    }
    else {
        hashmap->size++;
        return index;
    }
}

/**
 * Returns a pointer to the head of a linked list containing all of the records
 * of files containing the given key. If the key does not exist in the hashmap,
 * this function returns NULL.
 */
Record *get_records(HashMap *hashmap, const char *key) {
    // TODO gdb
    int index;
    Node *ptr;

    if (!hashmap) {
        return NULL;
    }

    index = hash(key) % hashmap->capacity;
    for (ptr = (hashmap->map) + index; ptr; ptr = ptr->next) {
        if (strcmp(key, ptr->key) == 0) {
            return ptr->value;
        }
    }

    return NULL;
}

/**
 * Removes the node with the given key value from the hash map. Returns the
 * value of the deleted object, or NULL if it does not exist in the map.
 */
char *removekey(HashMap *hashmap, const char *key) {
    // TODO gdb
    int index;
    char* result = NULL;
    Node *ptr, *next;

    if (!hashmap) {
        return NULL;
    }

    index = hash(key) % hashmap->capacity;
    if ((hashmap->map) + index == NULL) {
        return NULL;
    }
    else if (strcmp((hashmap->map + index)->key, key) == 0) {
        // Matches the first in the linked list
        // TODO LHS not working
        result = (hashmap->map + index)->value;
        (hashmap->map + index) = (hashmap->map + index)->next;
        hashmap->size--;
    }
    else {
        // Walk the linked list
        next = (hashmap->map + index)->next;
        ptr = (hashmap->map + index);
        for (; next; next = next->next) {
            if (strcmp(key, next->key) == 0) {
                result = next->value;
                ptr->next = next->next;
                destroy_node(next);
                hashmap->size--;
                break;
            }
        }
    }
    return result;
}

/**
 * Frees all dynamic memory associated with the given hash map. Note that the
 * use of all iterators associated with the hashmap after its destruction is
 * extremely unsafe.
 */
void destroy_hashmap(HashMap *hashmap) {
    // TODO gdb
    int i;
    Node *ptr, *next;
    if (hashmap) {
        for (i = 0; i < hashmap->capacity; i++) {
            for (ptr = (hashmap->map + i); ptr; ) {
                next = ptr->next;
                destroy_node(ptr);
                ptr = next;
            }
        }
   }
}

/**
 * Creates a new iterator for the hash map. Returns a pointer to the iterator,
 * or NULL if the call fails.
 */
Iterator *create_iterator(HashMap *hashmap) {
    if (!hashmap) {
        return NULL;
    }

    Iterator *iter = (Iterator *) malloc(sizeof(struct Iterator));
    if (iter) {
        iter->ptr = hashmap->map;
        iter->capacity = hashmap->capacity;
        iter->current = 0;
        return iter;
    }
    else
        return NULL;
}

/**
 * Returns the next node in the iteration, or NULL if the end of the hash map
 * has been reached.
 */
Node *next_node(Iterator *iter) {
    // TODO
    Node *ptr;
    if (!iter) {
        return NULL;
    }

    while (iter->current < iter->capacity) {
        while (iter->ptr == NULL) {

        }
    }
}
