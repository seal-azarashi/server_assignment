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
    check_error(socket_fd == -1, "socket failed", socket_fd);
    int connect_result = connect(socket_fd, (struct sockaddr *)&socket_address, sizeof(socket_address));
    check_error(connect_result == -1, "connect failed", socket_fd);

    printf("Connected to server on localhost (%s), port %d\n", LOOPBACK_ADDRESS, PORT);

    size_t request_length = strlen(http_request);
    bytes_written = write(socket_fd, http_request, request_length);
    check_error(bytes_written == -1, "write failed", socket_fd);
    check_error(bytes_written < request_length, "Partial write occurred", socket_fd);

    printf("Sent HTTP GET request\n");

    bytes_read = read(socket_fd, buffer, BUFFER_SIZE - 1);
    check_error(bytes_read == -1, "read failed", socket_fd);

    if (bytes_read == 0)
    {
        printf("Server closed the connection\n");
    }
    else
    {
        buffer[bytes_read] = '\0'; // Null-terminate the received data
        printf("Received response:\n%s\n", buffer);
    }

    cleanup(socket_fd);
    return EXIT_SUCCESS;
}
