#ifndef COMMON_UTILS_H
#define COMMON_UTILS_H

#include <stdio.h>
#include <unistd.h>

#define PORT "8080"
#define BUFFER_SIZE 1024

void cleanup(int socket_fd);
void check_error(int is_error, const char *message, int socket_fd);

#endif
