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
};

typedef struct SortedList SortedList;

/*
 * Creates a new sorted list of size zero. Returns a new sorted list object on
 * success; on failure, this function returns NULL.
 */
SortedList *create_sortedlist(CompFunc);

/*
 * Destroys the list pointed to by the given list pointer. Since the list is
 * implemented as a linked list, this method walks the list and frees all of
 * the dynamically allocated list nodes along the way.
 */
void destroy_sortedlist(SortedList *);

/*
 * Inserts an object into the list, maintaining the invariant that each item is
 * less than or equal to the one after it. If the insertion is successful, the
 * function returns 1. If the insertion fails, or if the list pointer is NULL,
 * then it returns 0.
 */
int insert_sortedlist(SortedList *, const char *, const char *);

/*
 * Iterator type for user to "walk" through the list item by item, from
 * beginning to end.
 */
struct SortedListIterator {
    Node *ptr;
};

typedef struct SortedListIterator SortedListIterator;

/*
 * Creates an iterator object that will allow the caller to "walk" through the
 * list from beginning to the end.
 *
 * If the function succeeds, it returns a non-NULL SortedListIterT object.
 * Else, it returns NULL.
 */
SortedListIterator *create_iter(SortedList *);

/*
 * Destroys an iterator object.
 */
void destroy_iter(SortedListIterator *);

/*
 * Returns the next object in the iteration, or NULL if the end of the iteration
 * has been reached.
 */
Record *next_item(SortedListIterator *);

#endif
