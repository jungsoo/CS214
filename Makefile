# Makefile
# Compiles the indexer.

AR = ar -cr
CC = gcc
CFLAGS = -Wall -g

all: index

index: libhashmap indexer.c
	$(CC) $(CFLAGS) -static -Lhashmap -o index indexer.c

libhashmap: hashmap.o
	$(AR) libhashmap.a hashmap.o

hashmap.o: hashmap.c hashmap.h
	$(CC) $(CFLAGS) -c hashmap.c

clean:
	rm -f *.o *.a
	rm -f index
