CC=gcc
CFLAGS=-Wall -g -std=gnu99 -pthread

all: server client

server: server.c util.h tokenizer.h tokenizer.c
	$(CC) $(CFLAGS) -o server server.c tokenizer.c

client: client.c util.h 
	$(CC) $(CFLAGS) -o client client.c

clean:
	rm -f server client
