#include <stdio.h>
#include <stdlib.h>
#include "hashmap.c"

int main(int argc, char **argv) {
    // Strings to hash
    char *s1 = "hello";
    char *s2 = "goodbye";
    printf("%s hashes to %u\n", s1, hash(s1));
    printf("%s hashes to %u\n", s2, hash(s2));

    // Create some data structures
    HashMap *hm = create_hashmap(10);
    if (!hm) {
        fprintf(stderr, "Memory allocation for hashmap failed\n");
        return 1;
    }

    // Add some nodes to the hashmap.
    put(hm, s1, s1);
    put(hm, s2, s2); 
    put(hm, s1, s1);
    return 0;
}
