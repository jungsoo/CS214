#ifndef INDEX_H
#define INDEX_H

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "sorted-list.h"

typedef struct _TrieNode TrieNode;
struct _TrieNode {
    TrieNode *children[36];
    SortedList *records;
    const char *substring;
};

/**
 * The inverted index represented by a trie data structure.
 */
struct _TrieIndex {
    TrieNode * root;
};
typedef struct _TrieIndex TrieIndex;


/**
 * Returns a pointer to a new node, or NULL if the call fails.
 */
TrieNode *create_trienode(const char *);

/**
 * Returns a pointer to a new inverted index, or NULL if the call fails.
 */
TrieIndex *create_index();

/**
 * Adds or updates another record for the given key.
 */
int put_record(TrieIndex *, const char *, const char *);
int child_index(char);
int put_helper(TrieNode *, const char *, const char *);

/**
 * Frees all dynamic memory associated with the given hash map. Note that the
 * use of all iterators associated with the index after its destruction is
 * extremely unsafe.
 */
void destroy_index(TrieIndex *);
int children_empty(TrieNode **);
void destroy_helper(TrieNode *);

#endif
