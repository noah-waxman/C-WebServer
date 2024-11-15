#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#define PORT 8080

int main() {
	int client = socket(PF_INET, SOCK_STREAM, 0);
	
	// Setup socket
	struct sockaddr_in address;
	memset(&address, 0, sizeof(address));
	address.sin_family = AF_INET;
	address.sin_port = htons(PORT);
	address.sin_addr.s_addr = INADDR_ANY;

    connect(client, (struct sockaddr *)&address, sizeof(address));
    char *message = "hi";
    send(client, message, strlen(message), 0);
    close(client);
    return 0;
}