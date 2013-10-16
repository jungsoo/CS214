/*
 * sorted-list.c
 * An implementation of a sorted list using linked lists.
 * Kyle Suarez
 */

#include <stdio.h>
#include <stdlib.h>
#include "sorted-list.h"

#define TRUE 1
#define FALSE 0

/**
 * Creates a new node with the given data. If the function succeeds, it returns
 * a pointer to the new node; if memory allocation fails, it returns NULL.
 */
Node *create_node(void *data, Node *next) {
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
        list->size = 0;
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
 * greater than or equal to the one after it. If the insertion is successful,
 * the function returns 1. If the insertion fails, or if the list pointer is
 * NULL, then it returns 0.
 */
int insert_sortedlist(SortedList *list, void *newObj) {
    if (!list) {
        return FALSE;
    }

    Node *new = create_node(newObj, NULL);
    if (!new) {
        return FALSE;
    }

    list->size++;
    if (!list->head) {
        // First item added to the list.
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

/*
 * Deletes the given object from the list, maintaining sorted order. If the
 * deletion succeeds, this function returns 1; otherwise it returns 0 (for
 * instance, when the item was not found in the list). If list is a null
 * pointer, nothing happens and the function returns 0.
 *
 * If an iterator is pointing to the object to be deleted, this method will not
 * free memory. The item is safely removed from the list, but it is the
 * iterator's responsibility to free memory after it is done with it.
 */
int remove_sortedlist(SortedList *list, void *newObj) {
    Node *del;
    if (!list || !list->head) {
        return FALSE;
    }
    else if (list->compare(newObj, list->head->data) == 0) {
        // Delete the head.
        del = list->head;
        list->head = list->head->next;
        list->size--;
        if (--del->references <= 0) {
            free(del);
        }
        return TRUE;
    }
    else {
        Node *next = list->head->next;
        Node *ptr = list->head;
        for (; next; ptr = next, next = next->next) {
            if (list->compare(newObj, next->data) == 0) {
                del = next;
                ptr->next = next->next;
                if (--del->references <= 0) {
                    free(del);
                }
                return TRUE;
            }
        }

        // Item not found.
        return FALSE;
    }
}

/*
 * Creates a new iterator that allows you to traverse a sorted list. If the
 * allocation succeeds, this function returns a pointer to a new iterator
 * object; otherwise, it returns NULL.
 */
SortedListIterator *create_sliter(SortedList *list) {
    if (!list) {
        return NULL;
    }

    SortedListIterator *iterator =
        (SortedListIterator *) malloc(sizeof(struct SortedListIterator));
    if (iterator) {
        iterator->ptr = list->head;
        if (iterator->ptr) {
            iterator->ptr->references++;
        }
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
void destroy_sliter(SortedListIterator *iter) {
    if (!iter) {
        return;
    }

    Node *ptr = iter->ptr;
    if (ptr && --ptr->references <= 0) {
        free(ptr);
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
void *next_item(SortedListIterator *iter) {
    if (!iter || !iter->ptr) {
        return NULL;
    }

    void *data = iter->ptr->data;
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
