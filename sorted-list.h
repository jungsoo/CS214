#ifndef SORTED_LIST_H
#define SORTED_LIST_H

#include "record.h"
#include <stdlib.h>

/*
 * Standard comparison function for two records.
 */
typedef int (*CompFunc)(Record *, Record *);

/*
 * Type for a basic linked list node.
 */
struct Node {
    Record *data;
    struct Node *next;
    int references;
};

typedef struct Node Node;

/**
 * Creates a new node with the given data.
 */
Node *create_node(Record *, struct Node *);

/*
 * Sorted list type.
 */
struct SortedList {
    Node *head;
    CompFunc compare;
    int size;
};

typedef struct SortedList SortedList;

/*
 * Iterator type for user to "walk" through the list item by item, from
 * beginning to end.
 */
struct SortedListIterator {
    Node *ptr;
};

typedef struct SortedListIterator SortedListIterator;

/*
 * Creates a new, empty sorted list.  The caller must provide a comparator
 * function that can be used to order objects that will be kept in the list.
 * 
 * If the function succeeds, it returns a (non-NULL) SortedList object.
 * Else, it returns NULL.
 */
SortedList *create_sortedlist(CompFunc);

/*
 * Destroys a list, freeing all dynamically allocated memory.
 */
void destroy_sortedlist(SortedList *);


/*
 * Inserts a given object into a sorted list, maintaining sorted
 * order of all objects in the list.  If the new object is equal to a subset
 * of existing objects in the list, then the subset can be kept in any
 * order.
 *
 * If the function succeeds, it returns 1.  Else, it returns 0.
 */
int insert_sortedlist(SortedList *, char *, char *);

/*
 * Creates an iterator object that will allow the caller to "walk" through the
 * list from beginning to the end.
 *
 * If the function succeeds, it returns a non-NULL SortedListIterT object.
 * Else, it returns NULL.
 */
SortedListIterator *create_sliter(SortedList *);

/*
 * Destroys an iterator object.
 */
void destroy_sliter(SortedListIterator *);

/*
 * Returns the next object in the iteration, or NULL if the end of the iteration
 * has been reached.
 */
Record *next_item(SortedListIterator *);

#endif
