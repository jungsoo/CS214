#include "record.h"
#include "sorted-list.h"
#include <stdio.h>
#include <stdlib.h>

#define TRUE 1
#define FALSE 0

/**
 * Creates a new node with the given data. If the function succeeds, it returns
 * a pointer to the new node; if memory allocation fails, it returns NULL.
 */
Node *create_node(Record *data, Node *next) {
    Node *node = (Node *) malloc(sizeof(struct Node));
    if (node) {
        node->data = data;
        node->next = next;
        node->references = 1;
        return node;
    }
    else
        return NULL;
}

/*
 * Creates a new sorted list of size zero. Returns a new sorted list object on
 * success; on failure, this function returns NULL.
 */
SortedList *create_sortedlist(CompFunc cf) {
    SortedList *list = (SortedList *) malloc(sizeof(struct SortedList));
    if (list) {
        list->head = NULL;
        list->compare = cf;
        return list;
    }
    else
        return NULL;
}

/*
 * Destroys the list pointed to by the given list pointer. Since the list is
 * implemented as a linked list, this method walks the list and frees all of
 * the dynamically allocated list nodes along the way.
 */
void destroy_sortedlist(SortedList *list) {
    if (list) {
        Node *ptr = list->head;
        Node *prev = NULL;
        while (ptr) {
            prev = ptr;
            ptr = ptr->next;
            free(prev);
        }
        free(list);
    }
}

/*
 * Inserts an object into the list, maintaining the invariant that each item is
 * less than or equal to the one after it. If the insertion is successful, the
 * function returns 1. If the insertion fails, or if the list pointer is NULL,
 * then it returns 0.
 */
int insert_sortedlist(SortedList *list, const char *token, const char *filename) {
    Node *new, *ptr, *prev;
    Record *record;
    int c, retval;

    // TODO 86 'record' uninitialized. just make it now

    if (!list) {
        retval = 0;
    }
    else if (!list->head) {
        // First item added to the list. Create a new node
        record = create_record(token, filename, 1);
        new = create_node(record, NULL);
        if (record && new) {
            list->head = new;
            retval = 1;
        }
        else {
            free(record);
            free(new);
            retval = 0;
        }
    }
    else if ((c = list->compare(record, list->head->data)) <= 0) {
        if (c < 0) {
            // Belongs at the head of the list
            record = create_record(token, filename, 1);
            new = create_node(record, list->head);
            if (record && new) {
                list->head = new;
                retval = 1;
            }
            else {
                free(record);
                free(new);
                retval = 0;
            }
        }
        else {
            // Match - update record!
            list->head->data->hits++;
            retval = 1;
        }
    }
    else {
        // Find a match or a place to insert
        ptr = list->head;
        prev = NULL;
        for (; ptr; prev = ptr, ptr = ptr->next) {
            c = list->compare(record, ptr->data);
            if (c == 0) {
                // Update this node and we're done.
                ptr->data->hits++;
                return 1;
            }
            else if (c < 0) {
                // Node should be inserted in here.
                record = create_record(token, filename, 1);
                new = create_node(record, ptr);
                if (record && new) {
                    prev->next = new;
                    return 1;
                }
                else {
                    free(record);
                    free(new);
                    return 0;
                }
            }
        }

        // The item belongs at the end of the list.
        record = create_record(token, filename, 1);
        new = create_node(record, NULL);
        if (record && new) {
            prev->next = new;
            retval = 1;
        }
        else {
            free(record);
            free(new);
            retval = 0;
        }
    }

    return retval;
}

/*
 * Creates a new iterator that allows you to traverse a sorted list. If the
 * allocation succeeds, this function returns a pointer to a new iterator
 * object; otherwise, it returns NULL.
 */
SortedListIterator *create_iter(SortedList *list) {
    if (!list) {
        return NULL;
    }

    SortedListIterator *iterator =
        (SortedListIterator *) malloc(sizeof(struct SortedListIterator));
    if (iterator) {
        iterator->ptr = list->head;
        return iterator;
    }
    else
        return NULL;
}

/*
 * Destroys an iterator, freeing its memory. If the iterator is pointing to a
 * list node that was deleted by the list during iteration, the iterator will
 * free the node if it is the last one pointing to it.
 */
void destroy_iter(SortedListIterator *iter) {
    if (!iter) {
        return;
    }

    free(iter);
}

/*
 * Returns the next item in the iteration. If the end of the list is reached,
 * this function returns NULL.
 *
 * In the face of concurrent modification, this iterator attempts to be as
 * robust as possible: if the current node in the iteration is deleted, a call
 * to this function will return what would have been the next element in the
 * sequence. Additions or deletions to other parts of the list will not affect
 * the iterator.
 *
 * If the list for the given iterator is destroyed, then the result of a call
 * to this function is unsafe and undefined.
 */
Record *next_item(SortedListIterator *iter) {
    if (!iter || !iter->ptr) {
        return NULL;
    }

    Record *data = iter->ptr->data;
    Node *ptr = iter->ptr;
    iter->ptr = iter->ptr->next;
    if (iter->ptr) {
        iter->ptr->references++;
    }

    if (ptr && --ptr->references <= 0) {
        free(ptr);
    }

    return data;
}
