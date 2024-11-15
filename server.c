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
	address.sin_family = AF_INET;
	address.sin_port = htons(PORT);
	address.sin_addr.s_addr = INADDR_ANY;

	bind(server_fd, (struct sockaddr *)&address, sizeof(address));
	listen(server_fd, 3);
	while(1) {
		int new_socket = accept(server_fd, NULL, NULL);

		char buffer[1024] = {0};
		read(new_socket, &buffer, sizeof(&buffer));
		printf("%s\n", buffer);
		write(new_socket, &buffer, strlen(buffer));

		close(new_socket);
	}
}