#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#define PORT 8080

int main() {
	// Create client socket
	int client = socket(PF_INET, SOCK_STREAM, 0);
	
	// Setup socket
	struct sockaddr_in address;
	address.sin_family = AF_INET; // IPv4
	address.sin_port = htons(PORT); // 8080
	address.sin_addr.s_addr = INADDR_ANY; // 0.0.0.0/0 - all interfaces

	connect(client, (struct sockaddr *)&address, sizeof(address));
	char message[50];
	while(1) {
		printf("$: ");
		scanf("%s", message);
		send(client, message, strlen(message), 0);
		memset(message, 0, sizeof(message));
	}

	close(client);
    return 0;
}