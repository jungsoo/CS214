CC=gcc
CFLAGS=-Wall -g -std=gnu99 -pthread

all: server shmserver client

server: server.c util.h 
	$(CC) $(CFLAGS) -o server server.c

shmserver: shmserver.c util.h 
	$(CC) $(CFLAGS) -o shmserver shmserver.c

client: client.c util.h 
	$(CC) $(CFLAGS) -o client client.c

clean:
	rm -f server shmserver client
