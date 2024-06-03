#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>

#define PORT 8080
#define MAX_NUMBERS 100

int numbers[MAX_NUMBERS];
int count = 0;

void handle_client(int client_socket) {
    char buffer[1024] = {0};
    int valread;
    
    while ((valread = read(client_socket, buffer, sizeof(buffer))) > 0) {
        buffer[valread] = '\0';
        
        if (strncmp(buffer, "POST", 4) == 0) {
            int num;
            sscanf(buffer + 5, "%d", &num); // read the number after "POST "
            if (count < MAX_NUMBERS) {
                numbers[count++] = num;
                write(client_socket, "Penambahan angka berhasil\n", strlen("Penambahan angka berhasil\n"));
            } else {
                write(client_socket, "Array penuh\n", strlen("Array penuh\n"));
            }
        } else if (strncmp(buffer, "GET", 3) == 0) {
            char response[1024] = "";
            for (int i = 0; i < count; i++) {
                char num_str[12];
                sprintf(num_str, "%d ", numbers[i]);
                strcat(response, num_str);
            }
            strcat(response, "\n");
            write(client_socket, response, strlen(response));
        } else {
            write(client_socket, "Bad request\n", strlen("Bad request\n"));
        }
        
        memset(buffer, 0, sizeof(buffer));
    }
    
    close(client_socket);
}

int main(int argc, char const *argv[]) {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }
    
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
    
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);
    
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    
    if (listen(server_fd, 3) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }
    
    while (1) {
        if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0) {
            perror("accept");
            exit(EXIT_FAILURE);
        }
        
        handle_client(new_socket);
    }
    
    return 0;
}
