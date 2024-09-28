#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <errno.h>

#define PORT 8080
#define BUFFER_SIZE 1024

int main()
{
    struct sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = inet_addr("127.0.0.1");
    address.sin_port = htons(PORT);

    int addrlen = sizeof(address);
    int server_fd, new_socket;
    char buffer[BUFFER_SIZE] = {0};
    char *http_response = "HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\nContent-Length: 0\r\n\r\n";
    ssize_t bytes_read, bytes_written;

    // ソケットの作成
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == -1)
    {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // ソケットをアドレスとポートにバインド
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) == -1)
    {
        perror("bind failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    // 接続のリッスン
    if (listen(server_fd, 3) == -1)
    {
        perror("listen");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    printf("Server listening on localhost (127.0.0.1), port %d\n", PORT);

    while (1)
    {
        // 新しい接続を受け入れる
        new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen);
        if (new_socket == -1)
        {
            perror("accept");
            continue; // エラー時も終了せずに次の接続を待つ
        }

        // クライアントからのデータを読み取る
        bytes_read = read(new_socket, buffer, BUFFER_SIZE);
        if (bytes_read == -1)
        {
            perror("read");
            close(new_socket);
            continue;
        }
        else if (bytes_read == 0)
        {
            printf("Client closed the connection\n");
            close(new_socket);
            continue;
        }
        printf("Received a connection\n");

        // 空の本文を持つHTTP 200 OKレスポンスを送信
        size_t response_len = strlen(http_response);
        bytes_written = write(new_socket, http_response, response_len);
        if (bytes_written == -1)
        {
            perror("write");
        }
        else if (bytes_written < response_len)
        {
            fprintf(stderr, "Partial write occurred\n");
        }
        else
        {
            printf("Sent HTTP 200 OK response\n");
        }

        // ソケットを閉じる
        if (close(new_socket) == -1)
        {
            perror("close");
        }
    }

    // メインのサーバーソケットを閉じる
    if (close(server_fd) == -1)
    {
        perror("close");
    }
    return 0;
}
