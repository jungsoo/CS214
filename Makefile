# Makefile
# Compiles the indexer.

AR = ar -cr
CC = gcc
CFLAGS = -Wall -g

all: index

index: libutil indexer.c
	$(CC) $(CFLAGS) -static -Lutil -o index indexer.c

libutil: hashmap.o tokenizer.o
	$(AR) libutil.a hashmap.o tokenizer.o

hashmap.o: hashmap.c hashmap.h
	$(CC) $(CFLAGS) -c hashmap.c

tokenizer.o: tokenizer.c
	$(CC) $(CFLAGS) -c tokenizer.c

clean:
	rm -f *.o *.a
	rm -f index
