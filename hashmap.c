#include <stdlib.h>
#include "hashmap.h"

/**
 * The djb2 hash algorithm for hashing strings.
 */
unsigned long hash(char *str) {
    unsigned long hash = 5381;
    int c;

    while (c = *str++) {
        hash = ((hash << 5) + hash) + c;
    }

    return hash;
}

/**
 * Creates a new item with the given data attributes. Returns a pointer to the
 * new item, or NULL if the call fails.
 */
Item *create_item(void *key, void *value, Item *next) {
    Item *item = (Item *) malloc(sizeof(struct Item));
    if (item) {
        item->key = key;
        item->value = value;
        item->next = next;
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

int put(HashMap *hashmap, void *key, void *value)
