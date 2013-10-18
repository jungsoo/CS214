#include "record.h"
#include <stdlib.h>
#include <string.h>

/**
 * Compares two records. Returns -1 if rec1 < rec2, 1 if rec1 > rec2, and 0 if
 * rec1 = rec2.
 */
int reccmp(Record *r1, Record *r2) {
    int result;
    Record *rec1 = (Record *) r1;
    Record *rec2 = (Record *) r2;

    result = strcmp(rec1->token, rec2->token);
    return result == 0 ? strcmp(rec1->filename, rec2->filename) : result;
}

/**
 * Creates a new record. Returns a pointer to the new object, or NULL if the
 * call fails.
 */
Record *create_record(const char *token, const char *filename, int hits) {
    Record *record = (Record *) malloc(sizeof(struct Record));
    if (record) {
        record->token = (char *) malloc(strlen(token) + 1);
        record->filename = (char *) malloc(strlen(filename) + 1);
        if (!record->token || !record->filename) {
            free(record);
        }
        else {
            strcpy(record->filename, filename);
            strcpy(record->token, token);
            record->hits = hits;
            return record;
        }
    }

    return NULL;
}

/**
 * Destroys and frees all memory associated with the given record.
 */
void destroy_record(Record *record) {
    if (record) {
        free(record->token);
        free(record->filename);
        free(record);
    }
}
