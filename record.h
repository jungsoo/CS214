#ifndef RECORD_H
#define RECORD_H

/**
 * Compares two records. Returns -1 if the first is "less than" the second, 1 if
 * the first is "greater than" the second, and 0 if they are "equal".
 * rec1 = rec2.
 */
int reccmp(Record *, Record *) {

/**
 * A basic type that stores indexer-related data.
 */
struct Record {
    char *token;
    char *filename;
    int hits;
};

typedef struct Record Record;

/**
 * Creates a new record. Returns a pointer to the new object, or NULL if the
 * call fails.
 */
Record *create_record(const char *, int, Record *);

/**
 * Destroys and frees all memory associated with the given record.
 */
void destroy_record(Record *);

#endif
