#ifndef INDEXER_H
#define INDEXER_H

/**
 * Indexes the file descriptor into the inverted index.
 */
int index(HashMap *, FILE *);

/**
 * Indexes a single file into the inverted index.
 */
int index_file(HashMap *, FILE *);

/**
 * Dumps the inverted index into a file.
 */
int dump(HashMap *. FILE *);

#endif
