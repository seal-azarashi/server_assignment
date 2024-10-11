#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <errno.h>
#include "common_utils.h"

ssize_t read_request(int socket, char *buffer, size_t max_size)
{
    size_t total_read = 0;
    ssize_t bytes_read;
    while (total_read < max_size)
    {
        bytes_read = read(socket, buffer + total_read, max_size - total_read);
        if (bytes_read == -1)
        {
            return -1;
        }
        if (bytes_read == 0)
        {
            break;
        }

        total_read += bytes_read;
        if (strstr(buffer, "\r\n\r\n") != NULL) // シンプルな GET リクエストのみが来る想定
        {
            break;
        }
    }
    return total_read;
}

int main()
{
    struct sockaddr_in6 socket_address;
    socket_address.sin6_family = AF_INET6;
    socket_address.sin6_addr = in6addr_any;
    socket_address.sin6_port = htons(atoi(PORT));

    int address_length = sizeof(socket_address);
    char buffer[MAX_MESSAGE_SIZE] = {0};
    char *http_response = "HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\nContent-Length: 0\r\n\r\n";
    ssize_t bytes_read, bytes_written;

    int socket_fd = socket(AF_INET6, SOCK_STREAM, 0);
    check_error(socket_fd == -1, "socket failed", socket_fd);

    int ipv6only = 0;
    int setsockopt_result = setsockopt(socket_fd, IPPROTO_IPV6, IPV6_V6ONLY, &ipv6only, sizeof(ipv6only));
    check_error(setsockopt_result == -1, "setsockopt failed", socket_fd);

    int bind_result = bind(socket_fd, (struct sockaddr *)&socket_address, sizeof(socket_address));
    check_error(bind_result == -1, "bind failed", socket_fd);

    // TODO: backlog の値は暫定で、スレッドの導入時に非機能要件を整理して再度値を決定する。
    int listen_result = listen(socket_fd, 3);
    check_error(listen_result == -1, "listen failed", socket_fd);

    printf("Server listening on port %s (IPv4 and IPv6)\n", PORT);

    while (1)
    {
        int client_socket_fd = accept(socket_fd, (struct sockaddr *)&socket_address, (socklen_t *)&address_length);
        if (client_socket_fd == -1)
        {
            perror("accept failed");
            continue;
        }
        bytes_read = read_request(client_socket_fd, buffer, MAX_MESSAGE_SIZE - 1);
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
        buffer[bytes_read] = '\0';
        printf("Received request:\n%s\n", buffer);

        size_t response_length = strlen(http_response);
        bytes_written = write_all(client_socket_fd, http_response, response_length);
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
