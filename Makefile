CC=gcc
CFLAGS=-Wall -g -pthread

all: server client

server: server.c util.h tokenizer.h
	$(CC) $(CFLAGS) -o server server.c

client: client.c util.h tokenizer.h
	$(CC) $(CFLAGS) -o client client.c

clean:
	rm -f server client
