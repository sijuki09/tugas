#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

#define PORT 8080
#define MAX_CLIENTS 5
#define MAX_NUMBERS 10

int main() {
    int server_fd, new_socket, valread;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    char buffer[1024] = {0};
    int numbers[MAX_NUMBERS];
    int num_count = 0;

    // Initialize numbers array
    for (int i = 0; i < MAX_NUMBERS; i++) {
        numbers[i] = 0;
    }

    // Create socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // Set socket options
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    // Bind socket to port
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    // Listen for incoming connections
    if (listen(server_fd, MAX_CLIENTS) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    while (1) {
        // Accept incoming connection
        if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0) {
            perror("accept");
            exit(EXIT_FAILURE);
        }

        // Receive command from client
        valread = read(new_socket, buffer, 1024);

        if (strncmp(buffer, "POST", 4) == 0) {
            // Receive number from client
            valread = read(new_socket, buffer, 1024);
            int num = atoi(buffer);
            numbers[num_count++] = num;
            send(new_socket, "Penambahan angka berhasil\n", strlen("Penambahan angka berhasil\n"), 0);
        } else if (strncmp(buffer, "GET", 3) == 0) {
            // Send all numbers to client
            char response[1024];
            strcpy(response, "");
            for (int i = 0; i < num_count; i++) {
                char num_str[10];
                sprintf(num_str, "%d ", numbers[i]);
                strcat(response, num_str);
            }
            send(new_socket, response, strlen(response), 0);
        } else {
            // Invalid command
            send(new_socket, "Bad request\n", strlen("Bad request\n"), 0);
        }
    }

    return 0;
}
