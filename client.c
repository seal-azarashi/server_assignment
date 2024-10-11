#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <errno.h>
#include "common_utils.h"

int receive_response(int socket, char *buffer, size_t max_size)
{
    size_t total_received = 0;
    while (1)
    {
        ssize_t received = read(socket, buffer + total_received, max_size - total_received);
        if (received == -1)
        {
            return -1;
        }
        if (received == 0)
        {
            break;
        }

        total_received += received;
        if (total_received >= max_size)
        {
            break;
        }
    }
    return total_received;
}

int main(int argc, char *argv[])
{
    const char *host = (argc > 1) ? argv[1] : "::1";

    struct sockaddr_in6 socket_address;
    socket_address.sin6_family = AF_INET6;
    inet_pton(AF_INET6, host, &socket_address.sin6_addr);
    socket_address.sin6_port = htons(atoi(PORT));

    char buffer[BUFFER_SIZE] = {0};
    char *http_request = "GET / HTTP/1.1\r\nHost: localhost\r\n\r\n";
    char response[MAX_MESSAGE_SIZE] = {0};

    int socket_fd = socket(AF_INET6, SOCK_STREAM, 0);
    check_error(socket_fd == -1, "socket failed", socket_fd);

    int connect_result = connect(socket_fd, (struct sockaddr *)&socket_address, sizeof(socket_address));
    check_error(connect_result == -1, "connect failed", socket_fd);

    printf("Connected to server on %s, port %s\n", host, PORT);

    size_t request_length = strlen(http_request);
    int bytes_written = write_all(socket_fd, http_request, request_length);
    check_error(bytes_written == -1, "write failed", socket_fd);

    printf("Sent HTTP GET request\n");

    ssize_t total_received = receive_response(socket_fd, response, MAX_MESSAGE_SIZE - 1);
    check_error(total_received == -1, "read failed", socket_fd);

    if (total_received == 0)
    {
        printf("Server closed the connection\n");
    }
    else
    {
        response[total_received] = '\0'; // Null-terminate the received data
        printf("Received response:\n%s\n", response);
    }

    cleanup(socket_fd);
    return EXIT_SUCCESS;
}
