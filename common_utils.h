#ifndef COMMON_UTILS_H
#define COMMON_UTILS_H

#include <stdio.h>
#include <unistd.h>

#define PORT "8080"
#define BUFFER_SIZE 1024
#define MAX_MESSAGE_SIZE 4096 // 死活監視用の API に対して送る想定で、 4KiB あれば溢れないと考えている

void cleanup(int socket_fd);
void check_error(int is_error, const char *message, int socket_fd);
ssize_t write_all(int socket, const char *buffer, size_t length);

#endif
