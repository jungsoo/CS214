#include "inverted-index.h"

/**
 * Returns a pointer to a new node, or NULL if the call fails.
 */
TrieNode *create_trienode(const char *substring) {
    TrieNode *node = (TrieNode *)malloc(sizeof(struct _TrieNode));
    node->substring = substring;
    return node;
}

/**
 * Returns a pointer to a new inverted index, or NULL if the call fails.
 */
TrieIndex *create_index() {
    TrieIndex *index = (TrieIndex *)malloc(sizeof(struct _TrieIndex));
    char *empty = (char *)malloc(sizeof(char));
    *empty = '\0';
    index->root = create_trienode(empty);

    return index;
}

int child_i(char ch) {
    if (isdigit(ch)) {
        return ch - '0';
    } else if (isalpha(ch)) {
        return ch - 'a';
    } else if (ch == '\0') {
        return -1;
    } else {
        printf("fuck you\n");
        abort();
    }
}

int put_record(TrieIndex *index, const char *tok, const char *fname) {
    return put_helper(index->root, tok, fname);
}

int put_helper(TrieNode *curr, const char *tok, const char *fname) {
    int token_index = strlen(curr->substring);
    if (strcmp(curr->substring, tok) == 0 || tok[token_index] == '\0') {
        if (curr->records == NULL) {
            curr->records = create_sortedlist(reccmp);
        }
        return insert_sortedlist(curr->records, tok, fname);
    } else {
        int child_index = child_i(tok[token_index + 1]);

        if (curr->children[child_index] == NULL) {
            char *new_substr = (char *)malloc(sizeof(char) * (token_index + 1));
            strncpy(new_substr, tok, token_index + 1);
            curr->children[child_index] = create_trienode(new_substr);
        }
        return put_helper(curr->children[child_index], tok, fname);
    }
}

/**
 * Frees all dynamic memory associated with the given trie.
 */
void destroy_index(TrieIndex *index) {
    destroy_helper(index->root);
    return free(index);
}

int children_empty(TrieNode **children) {
    for (int i = 0; i < 36; i++) {
        if (children[i] != NULL) {
            return 0;
        }
    }
    return 1;
}

void destroy_helper(TrieNode *node) {
    if(children_empty(node->children)) {
        free((char *)node->substring);
        if (node->records) {
            destroy_sortedlist(node->records);
        }
        return free(node);
    }
    for (int i = 0; i < 36; i++) {
        if (node->children[i]) {
            destroy_helper(node->children[i]);
            return;
        }
    }

    free((char *)node->substring);
    if (node->records) {
        destroy_sortedlist(node->records);
    }
    return free(node);
}

