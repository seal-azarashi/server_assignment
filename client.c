#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <errno.h>
#include "common_utils.h"

int main()
{
    struct sockaddr_in socket_address;
    socket_address.sin_family = AF_INET;
    socket_address.sin_addr.s_addr = inet_addr(LOOPBACK_ADDRESS);
    socket_address.sin_port = htons(PORT);

    char buffer[BUFFER_SIZE] = {0};
    char *http_request = "GET / HTTP/1.1\r\nHost: localhost\r\n\r\n";
    ssize_t bytes_read, bytes_written;

    int socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_fd == -1)
    {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }
    if (connect(socket_fd, (struct sockaddr *)&socket_address, sizeof(socket_address)) == -1)
    {
        perror("connect failed");
        cleanup(socket_fd);
        exit(EXIT_FAILURE);
    }

    printf("Connected to server on localhost (%s), port %d\n", LOOPBACK_ADDRESS, PORT);

    size_t request_length = strlen(http_request);
    bytes_written = write(socket_fd, http_request, request_length);
    if (bytes_written == -1)
    {
        perror("write failed");
        cleanup(socket_fd);
        exit(EXIT_FAILURE);
    }
    else if (bytes_written < request_length)
    {
        fprintf(stderr, "Partial write occurred\n");
        cleanup(socket_fd);
        exit(EXIT_FAILURE);
    }

    printf("Sent HTTP GET request\n");

    bytes_read = read(socket_fd, buffer, BUFFER_SIZE - 1);
    if (bytes_read == -1)
    {
        perror("read failed");
        cleanup(socket_fd);
        exit(EXIT_FAILURE);
    }
    else if (bytes_read == 0)
    {
        printf("Server closed the connection\n");
    }
    else
    {
        buffer[bytes_read] = '\0'; // Null-terminate the received data
        printf("Received response:\n%s\n", buffer);
    }

    cleanup(socket_fd);
    return 0;
}
