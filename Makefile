# Makefile
# Compiles the indexer.

CC=gcc
CFLAGS=-Wall -g

all: index

index: indexer.c indexer.h
	$(CC) $(CFLAGS) -o index indexer.c

clean:
	rm -f *.o
