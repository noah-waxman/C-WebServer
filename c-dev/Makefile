CC=clang
CFLAGS=-Wall -Wextra -g
LIBS=-lcurl -lcjson

all: main

main: main.c
	$(CC) $(CFLAGS) -o main main.c api_client.c $(LIBS)

clean:
	rm -f main
