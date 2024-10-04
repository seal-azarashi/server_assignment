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

    int address_length = sizeof(socket_address);
    char buffer[BUFFER_SIZE] = {0};
    char *http_response = "HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\nContent-Length: 0\r\n\r\n";
    ssize_t bytes_read, bytes_written;

    int socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    check_error(socket_fd == -1, "socket failed", socket_fd);
    int bind_result = bind(socket_fd, (struct sockaddr *)&socket_address, sizeof(socket_address));
    check_error(bind_result == -1, "bind failed", socket_fd);
    int listen_result = listen(socket_fd, 3);
    check_error(listen_result == -1, "listen failed", socket_fd);

    printf("Server listening on %s, port %d\n", LOOPBACK_ADDRESS, PORT);

    while (1)
    {
        int client_socket_fd = accept(socket_fd, (struct sockaddr *)&socket_address, (socklen_t *)&address_length);
        if (client_socket_fd == -1)
        {
            perror("accept failed");
            continue;
        }
        bytes_read = read(client_socket_fd, buffer, BUFFER_SIZE);
        if (bytes_read == -1)
        {
            perror("read failed");
            cleanup(client_socket_fd);
            continue;
        }
        else if (bytes_read == 0)
        {
            printf("Client closed the connection\n");
            cleanup(client_socket_fd);
            continue;
        }

        printf("Received a connection\n");

        size_t response_length = strlen(http_response);
        bytes_written = write(client_socket_fd, http_response, response_length);
        if (bytes_written == -1)
        {
            perror("write failed");
        }
        else if (bytes_written < response_length)
        {
            fprintf(stderr, "Partial write occurred\n");
        }
        else
        {
            printf("Sent HTTP 200 OK response\n");
        }

        cleanup(client_socket_fd);
    }

    cleanup(socket_fd);
    return EXIT_SUCCESS;
}
