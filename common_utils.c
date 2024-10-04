#include <stdlib.h>
#include "common_utils.h"

void cleanup(int socket_fd)
{
    if (socket_fd == -1)
    {
        return;
    }

    if (close(socket_fd) == -1)
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
