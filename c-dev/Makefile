CC=gcc
CFLAGS=-Wall -Wextra -g
LIBS=-lcurl -lcjson

all: main

main: main.c
	$(CC) $(CFLAGS) -o main main.c api_client.c media_collection.c $(LIBS)

clean:
	rm -f main
