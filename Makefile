# Makefile
# Compiles the indexer.

AR = ar -cr
CC = gcc
CFLAGS = -Wall -g

all: index

index: libcontroller indexer.c
	$(CC) $(CFLAGS) -static -Lcontroller -o index indexer.c

libcontroller: controller.o
	$(AR) libcontroller.a controller.o

controller.o: libutil controller.c controller.h
	$(CC) $(CFLAGS) -static -Lutil controller.c

libutil: hashmap.o tokenizer.o sorted-list.o
	$(AR) libutil.a hashmap.o tokenizer.o sorted-list.o

hashmap.o: hashmap.c hashmap.h
	$(CC) $(CFLAGS) -c hashmap.c

tokenizer.o: tokenizer.c
	$(CC) $(CFLAGS) -c tokenizer.c

sorted-list.o: sorted-list.c sorted-list.h
	$(CC) $(CFLAGS) -c sorted-list.c

clean:
	rm -f *.o *.a
	rm -f index
