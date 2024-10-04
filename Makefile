CC = gcc
COMMON_UTILS = common_utils.c

.PHONY: server client

server: server.c $(COMMON_UTILS)
	$(CC) -o server.out $^
	./server.out

client: client.c $(COMMON_UTILS)
	$(CC) -o client.out $^
	./client.out
