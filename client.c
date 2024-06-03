#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#define PORT 8080
#define SERVER_IP "127.0.0.1"

int main() {
    int sock = 0;
    struct sockaddr_in serv_addr;
    char buffer[1024] = {0};

    // Create socket
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("\n Socket creation error \n");
        return -1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    // Convert IPv4 and IPv6 addresses from text to binary form
    if (inet_pton(AF_INET, SERVER_IP, &serv_addr.sin_addr) <= 0) {
        printf("\nInvalid address/ Address not supported \n");
        return -1;
    }

    // Connect to server
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        printf("\nConnection Failed \n");
        return -1;
    }

    // Send POST commands and numbers
    send(sock, "POST", strlen("POST"), 0);
    send(sock, "5", strlen("5"), 0);
    printf("POST 5\n");

    send(sock, "POST", strlen("POST"), 0);
    send(sock, "3", strlen("3"), 0);
    printf("POST 3\n");

    // Send GET command
    send(sock, "GET", strlen("GET"), 0);
    printf("GET\n");

    // Receive response from server
    read(sock, buffer, 1024);
    printf("%s\n", buffer);

    close(sock);
    return 0;
}
