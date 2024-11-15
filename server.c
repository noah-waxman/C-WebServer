#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#define PORT 8080

int main() {
	int server_fd = socket(PF_INET, SOCK_STREAM, 0);
	
	// Setup socket
	struct sockaddr_in address;
	memset(&address, 0, sizeof(address));
	address.sin_family = AF_INET; // IPv4
	address.sin_port = htons(PORT); // 8080 
	address.sin_addr.s_addr = INADDR_ANY; // 0.0.0.0/0 - all interfaces

	bind(server_fd, (struct sockaddr *)&address, sizeof(address));
	listen(server_fd, 3);

	while(1) {
		int new_socket = accept(server_fd, NULL, NULL);
		// Message buffer
		char buffer[1024] = {0};

		while(read(new_socket, &buffer, sizeof(&buffer)) > 0) {
			printf("%s\n", buffer);
			memset(buffer, 0, sizeof(buffer));
		}
		close(new_socket);
	}
	return 0;
}