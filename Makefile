# Makefile
# Compiles the indexer.

AR = ar -cr
CC = gcc
CFLAGS = -g

all: index

index: libcontroller indexer.c
	$(CC) $(CFLAGS) -static -Lcontroller -o index indexer.c

libcontroller: controller.o inverted-index.o
	$(AR) libcontroller.a controller.o inverted-index.o

controller.o: libutil controller.c controller.h
	$(CC) $(CFLAGS) -static -Lutil -c controller.c

inverted-index.o: libutil inverted-index.c inverted-index.h
	$(CC) $(CFLAGS) -static -Lutil -c inverted-index.c

libutil: record.o sorted-list.o tokenizer.o
	$(AR) libutil.a record.o sorted-list.o tokenizer.o

record.o: record.c record.h
	$(CC) $(CFLAGS) -c record.c

sorted-list.o: sorted-list.c sorted-list.h
	$(CC) $(CFLAGS) -c sorted-list.c

tokenizer.o: tokenizer.c
	$(CC) $(CFLAGS) -c tokenizer.c

clean:
	rm -f *.o *.a
	rm -f index
