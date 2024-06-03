#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080

void send_request(const char *message) {
    int sock = 0, valread;
    struct sockaddr_in serv_addr;
    char buffer[1024] = {0};
    
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("\n Socket creation error \n");
        return;
    }
    
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
    
    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
        printf("\nInvalid address/ Address not supported \n");
        return;
    }
    
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        printf("\nConnection Failed \n");
        return;
    }
    
    send(sock, message, strlen(message), 0);
    valread = read(sock, buffer, 1024);
    printf("%s", buffer);
    close(sock);
}

int main(int argc, char const *argv[]) {
    if (argc < 2) {
        printf("Usage: %s <COMMAND> [ARG]\n", argv[0]);
        return 1;
    }
    
    if (strcmp(argv[1], "POST") == 0 && argc == 3) {
        char message[1024];
        snprintf(message, sizeof(message), "POST %s", argv[2]);
        send_request(message);
    } else if (strcmp(argv[1], "GET") == 0) {
        send_request("GET");
    } else {
        printf("Usage: %s <COMMAND> [ARG]\n", argv[0]);
    }
    
    return 0;
}
