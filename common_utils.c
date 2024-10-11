#include <stdlib.h>
#include "common_utils.h"

void cleanup(int socket_fd)
{
    if (socket_fd == -1)
    {
        return;
    }

    int close_result = close(socket_fd);
    if (close_result == -1)
    {
        perror("Error closing socket");
    }
}

void check_error(int is_error, const char *message, int socket_fd)
{
    if (is_error)
    {
        perror(message);
        cleanup(socket_fd);
        exit(EXIT_FAILURE);
    }
}

ssize_t write_all(int socket, const char *buffer, size_t length)
{
    size_t total_written = 0;
    ssize_t bytes_written;
    while (total_written < length)
    {
        bytes_written = write(socket, buffer + total_written, length - total_written);
        if (bytes_written == -1)
        {
            return -1;
        }
        total_written += bytes_written;
    }
    return total_written;
}
