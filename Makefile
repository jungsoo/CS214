# Makefile
# Compiles the indexer.

AR = ar -cr
CC = gcc
CFLAGS = -Wall -g

all: index

index: libcontroller.a
	$(CC) $(CFLAGS) -Lcontroller -o index *.c

libcontroller.a: controller.o inverted-index.o
	$(AR) libcontroller.a inverted-index.o controller.o

controller.o: libutil.a controller.c controller.h
	$(CC) $(CFLAGS) -Lutil -c controller.c

inverted-index.o: libutil.a inverted-index.c inverted-index.h
	$(CC) $(CFLAGS) -Lutil -c inverted-index.c

libutil.a: record.o sorted-list.o tokenizer.o
	$(AR) libutil.a record.o sorted-list.o tokenizer.o

record.o: record.c record.h
	$(CC) $(CFLAGS) -c record.c

sorted-list.o: sorted-list.c sorted-list.h
	$(CC) $(CFLAGS) -c sorted-list.c

tokenizer.o: tokenizer.c tokenizer.h
	$(CC) $(CFLAGS) -c tokenizer.c

clean:
	rm -f *.o *.a
	rm -f index
