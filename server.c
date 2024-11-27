#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#define PORT 8080

typedef struct {
    int client_socket;
} client_data_t;

void* read_messages(void* arg) {
client_data_t* client_data = (client_data_t*)arg;
int client_socket = client_data->client_socket;
char buffer[1024];

while (1) {
	memset(buffer, 0, sizeof(buffer));
	int bytes_read = read(client_socket, buffer, sizeof(buffer) - 1);
	if (bytes_read <= 0) {
		printf("Client disconnected.\n");
		close(client_socket);
		break;
	}
	printf("Client: %s\n", buffer);
}

free(client_data);
return NULL;
}

void* send_messages(void* arg) {
    client_data_t* client_data = (client_data_t*)arg;
    int client_socket = client_data->client_socket;
    char message[1024];

    while (1) {
        printf("You: ");
        fgets(message, sizeof(message), stdin);
        if (strncmp(message, "exit", 4) == 0) {
            printf("Exiting...\n");
            close(client_socket);
            break;
        }
        send(client_socket, message, strlen(message), 0);
    }

    free(client_data);
    return NULL;
}

int main() {
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == 0) {
        perror("Socket failed");
        return EXIT_FAILURE;
    }

    struct sockaddr_in address;
    memset(&address, 0, sizeof(address));
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr*)&address, sizeof(address)) < 0) {
        perror("Bind failed");
        return EXIT_FAILURE;
    }

    if (listen(server_fd, 3) < 0) {
        perror("Listen failed");
        return EXIT_FAILURE;
    }

    printf("Server listening on port %d...\n", PORT);

    while (1) {
        int client_socket = accept(server_fd, NULL, NULL);
        if (client_socket < 0) {
            perror("Accept failed");
            continue;
        }

        printf("New client connected.\n");

        client_data_t* client_data = malloc(sizeof(client_data_t));
        client_data->client_socket = client_socket;

        pthread_t read_thread, write_thread;
        pthread_create(&read_thread, NULL, read_messages, client_data);
        pthread_create(&write_thread, NULL, send_messages, client_data);

        pthread_detach(read_thread);  // No need to join
        pthread_detach(write_thread);
    }

    close(server_fd);
    return EXIT_SUCCESS;
}
