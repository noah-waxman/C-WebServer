all: server client

server: server.c
	clang server.c -o server

client: client.c
	clang client.c -o client

clean:
	rm -f server client
