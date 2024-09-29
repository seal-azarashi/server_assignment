#ifndef COMMON_UTILS_H
#define COMMON_UTILS_H

#include <stdio.h>
#include <unistd.h>

#define PORT 8080
#define BUFFER_SIZE 1024
#define LOOPBACK_ADDRESS "127.0.0.1"

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

#endif
