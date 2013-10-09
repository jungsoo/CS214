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
 * Creates a new item with the given data attributes. Returns a pointer to the
 * new item, or NULL if the call fails.
 */
Item *create_item(const char *key, const char *value, struct Item *next) {
    Item *item = (Item *) malloc(sizeof(struct Item));
    if (item) {
        if (!strcpy(item->key, key) || !strcpy(item->value, value)) {
            return NULL;
        }
        item->next = next;
        return item;
    }
    else
        return NULL;
}

/**
 * Destroys and frees all memory associated with the given item.
 */
void destroy_item(Item *item) {
    free(item);
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
        hm->map = (Item *) calloc((size_t) capacity, sizeof(struct Item));
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
 * Hashes an item into the hash map. Returns a nonnegative integer on success
 * and -1 on an error.
 */
int put(HashMap *hashmap, const char *key, const char *value) {
    int index;
    Item *item;

    if (!hashmap) {
        return -1;
    }

    index = hash(key) % hashmap->capacity;
    item = create_item(key, value, (hashmap->map) + index);
    if (!item) {
        return -1;
    }
    else {
        hashmap->size++;
        return index;
    }
}

/**
 * Gets the value of the item in the hash map with the given key, or NULL if
 * the item cannot be found.
 */
char *get(HashMap *hashmap, const char *key) {
    // TODO gdb
    int index;
    Item *ptr;

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
 * Removes the item with the given key value from the hash map. Returns the
 * value of the deleted object, or NULL if it does not exist in the map.
 */
char *removekey(HashMap *hashmap, const char *key) {
    // TODO gdb
    int index;
    char* result = NULL;
    Item *ptr, *next;

    if (!hashmap) {
        return NULL;
    }

    index = hash(key) % hashmap->capacity;
    if ((hashmap->map) + index == NULL) {
        return NULL;
    }
    else if (strcmp((hashmap->map + index)->key, key) == 0) {
        // Matches the first in the linked list
        result = (hashmap->map + index)->value;
        (hashmap->map + index) = (hashmap->map + index)->next;
        hashmap->size--;
    }
    else {
        // Walk the linked list
        next = (hashmap->map + index)->next;
        ptr = (hashmap->map + index);
        for (;, next; next = next->next) {
            if (strcmp(key, next->key) == 0) {
                result = next->value;
                ptr->next = next->next;
                destroy_item(next);
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
    Item *ptr, next;
    if (!hashmap) {
        return;
    }

    for (i = 0; i < hashmap->capacity; i++) {
        for (ptr = (hashmap->map + i); ptr; ) {
            next = ptr->next;
            destroy_item(ptr);
            ptr = next;
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
 * Returns the next item in the iteration, or NULL if the end of the hash map
 * has been reached.
 */
Item *next_item(Iterator *iter) {
    // TODO
    Item *ptr;
    if (!iter) {
        return NULL;
    }

    while (iter->current < iter->capacity) {
        while (iter->ptr == NULL) {

        }
    }
}
