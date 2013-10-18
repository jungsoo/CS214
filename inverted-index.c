#include "index.h"
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

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
        return (int) str[0] + 26;
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
 * Adds or updates another record for the given token.
 */
int put_record(const char *tok, const char *fname) {
    SortedList *list;
    Node *new, *next, *ptr;
    Record *rec;

    if (!list) {
        // TODO create new list instead
        return 0;
    }

    list->size++; // TODO
    if (!list->head) {
        // First item added to the list.
        rec = create_record(token, filename, 0);
        list->head = new;
    }
    else if (list->compare(newObj, list->head->data) >= 0) {
        // Item belongs at the front of the list.
        new->next = list->head;
        list->head = new;
    }
    else {
        Node *next = list->head->next;
        Node *ptr = list->head;
        for (; next; ptr = next, next = next->next) {
            if (list->compare(newObj, next->data) >= 0) {
                // We've found the right place to insert the node.
                new->next = next;
                ptr->next = new;
                return TRUE;
            }
        }

        // The item belongs at the end of the list.
        ptr->next = new;
    }

    return TRUE;
}

/**
 * Adds or updates another record for the given key. Returns a nonnegative
 * integer on success and -1 if the call fails.
 */
int put_record(Index *Index, const char *token, const char *filename) {
    int index;
    Node *node, *ptr;
    Record *record, *rpt;

    if (!index) {
        return -1;
    }

    index = hash(key) % index->capacity;
    if (!(index->map + index)) {
        // Add a new node and record
        record = create_record(filename, 1, NULL);
        node = create_node(token, record, NULL);
        if (record && node) {
            (index->map + index) = node;
            return index;
        }
    }
    else {
        // There's a linked list here; find the one matching the correct token
        ptr = (index->map + index);
        for (; ptr && strcmp(ptr->key, token) != 0; ptr = ptr->next);
        if (ptr) {
            // Look for a record
            rpt = ptr->records;
            for(; rpt && strcmp(rpt->filename, filename) != 0; rpt = rpt->next);
            if (rpt) {
                // Update the record
                rpt->hits++;
            }
            else {
                // Create a new record
                record = create_record(filename, 1, NULL);
                if (record)
            }
        }
        else {
            // Create a new node for this token
        }
    }

    return -1;
}

/**
 * Returns a pointer to the head of a linked list containing all of the records
 * of files containing the given key. If the key does not exist in the index,
 * this function returns NULL.
 */
Record *get_records(Index *index, const char *key) {
    // TODO gdb
    int index;
    Node *ptr;

    if (!index) {
        return NULL;
    }

    index = hash(key) % index->capacity;
    for (ptr = (index->map) + index; ptr; ptr = ptr->next) {
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
char *removekey(Index *index, const char *key) {
    // TODO gdb
    int index;
    char* result = NULL;
    Node *ptr, *next;

    if (!index) {
        return NULL;
    }

    index = hash(key) % index->capacity;
    if ((index->map) + index == NULL) {
        return NULL;
    }
    else if (strcmp((index->map + index)->key, key) == 0) {
        // Matches the first in the linked list
        // TODO LHS not working
        result = (index->map + index)->value;
        (index->map + index) = (index->map + index)->next;
    }
    else {
        // Walk the linked list
        next = (index->map + index)->next;
        ptr = (index->map + index);
        for (; next; next = next->next) {
            if (strcmp(key, next->key) == 0) {
                result = next->value;
                ptr->next = next->next;
                destroy_node(next);
                break;
            }
        }
    }
    return result;
}

/**
 * Frees all dynamic memory associated with the given hash map. Note that the
 * use of all iterators associated with the index after its destruction is
 * extremely unsafe.
 */
void destroy_index(Index *Index) {
    // TODO gdb
    int i;
    Node *ptr, *next;
    if (index) {
        for (i = 0; i < index->capacity; i++) {
            for (ptr = (index->map + i); ptr; ) {
                next = ptr->next;
                destroy_node(ptr);
                ptr = next;
            }
        }
    }
}

///**
// * Creates a new iterator for the hash map. Returns a pointer to the iterator,
// * or NULL if the call fails.
// */
//Iterator *create_hmiter(index *index) {
//    if (!index) {
//        return NULL;
//    }
//
//    Iterator *iter = (Iterator *) malloc(sizeof(struct Iterator));
//    if (iter) {
//        iter->ptr = index->map;
//        iter->capacity = index->capacity;
//        iter->current = 0;
//        return iter;
//    }
//    else
//        return NULL;
//}
//
///**
// * Returns the next node in the iteration, or NULL if the end of the hash map
// * has been reached.
// */
//Node *next_node(Iterator *iter) {
//    // TODO
//    Node *ptr;
//    if (!iter) {
//        return NULL;
//    }
//
//    while (iter->current < iter->capacity) {
//        while (iter->ptr == NULL) {
//
//        }
//    }
//}
