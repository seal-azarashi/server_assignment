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
