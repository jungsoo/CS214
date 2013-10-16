#ifndef SORTED_LIST_H
#define SORTED_LIST_H
/*
 * sorted-list.h
 * Structures and prototypes for the sorted list. The only addition to the base
 * code is a struct definition for a linked list node and some other helper
 * functions.
 */

#include <stdlib.h>

/*
 * When your sorted list is used to store objects of some type, since the
 * type is opaque to you, you will need a comparator function to order
 * the objects in your sorted list.
 
 * You can expect a comparator function to return -1 if the 1st object is
 * smaller, 0 if the two objects are equal, and 1 if the 2nd object is
 * smaller.
 *
 * Note that you are not expected to implement any comparator functions.
 * You will be given a comparator function when a new sorted list is
 * created.
 */
typedef int (*CompFunc)(void *, void *);

/*
 * Type for a basic linked list node.
 */
struct Node {
    void *data;
    struct Node *next;
    int references;
};

typedef struct Node Node;

/**
 * Creates a new node with the given data.
 */
Node *create_node(void *, struct Node *);

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
 * If the function succeeds, it returns a (non-NULL) SortedListT object.
 * Else, it returns NULL.
 */
SortedList *create_sortedlist(CompFunc);

/*
 * SLDestroy destroys a list, freeing all dynamically allocated memory.
 *
 * You need to fill in this function as part of your implementation.
 */
void destroy_sortedlist(SortedList *);


/*
 * SLInsert inserts a given object into a sorted list, maintaining sorted
 * order of all objects in the list.  If the new object is equal to a subset
 * of existing objects in the list, then the subset can be kept in any
 * order.
 *
 * If the function succeeds, it returns 1.  Else, it returns 0.
 *
 * You need to fill in this function as part of your implementation.
 */
int insert_sortedlist(SortedList *, void *);

/*
 * SLRemove removes a given object from a sorted list.  Sorted ordering
 * should be maintained.
 *
 * If the function succeeds, it returns 1.  Else, it returns 0.
 *
 * You need to fill in this function as part of your implementation.
 */
int remove_sortedlist(SortedList *, void *);

/*
 * SLCreateIterator creates an iterator object that will allow the caller
 * to "walk" through the list from beginning to the end using SLNextItem.
 *
 * If the function succeeds, it returns a non-NULL SortedListIterT object.
 * Else, it returns NULL.
 *
 * You need to fill in this function as part of your implementation.
 */
SortedListIterator *create_sliter(SortedList *);

/*
 * SLDestroyIterator destroys an iterator object that was created using
 * SLCreateIterator().  Note that this function should destroy the
 * iterator but should NOT affectt the original list used to create
 * the iterator in any way.
 *
 * You need to fill in this function as part of your implementation.
 */
void destroy_sliter(SortedListIterator *);


/*
 * SLNextItem returns the next object in the list encapsulated by the
 * given iterator.  It should return a NULL when the end of the list
 * has been reached.
 *
 * One complication you MUST consider/address is what happens if a
 * sorted list encapsulated within an iterator is modified while that
 * iterator is active.  For example, what if an iterator is "pointing"
 * to some object in the list as the next one to be returned but that
 * object is removed from the list using SLRemove() before SLNextItem()
 * is called.
 *
 * You need to fill in this function as part of your implementation.
 */
void *next_item(SortedListIterator *);

#endif
