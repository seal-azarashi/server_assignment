"""
クライアントが Partial write, read に耐えれるかをテストするためのサーバーです。
C で作るのが大変だったので、 Claude くんに Python で雑に作ってもらいました...
"""

import socket
import time

def handle_client(client_socket):
    request = client_socket.recv(1024).decode('utf-8')
    print(f"受信したリクエスト: {request}")

    # レスポンスの準備
    response = "HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\n\r\nこれはpartial writeテスト用のレスポンスです。"

    # partial writeをシミュレートするために、レスポンスをチャンクに分割して送信
    chunks = [response[i:i+10] for i in range(0, len(response), 10)]
    
    for chunk in chunks:
        client_socket.send(chunk.encode('utf-8'))
        time.sleep(0.1)  # ネットワーク条件をシミュレートするための小さな遅延

    client_socket.close()

def start_server(host='::1', port=8080):
    server_socket = socket.socket(socket.AF_INET6, socket.SOCK_STREAM)
    server_socket.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
    server_socket.bind((host, port))
    server_socket.listen(1)

    print(f"サーバーが [{host}]:{port} でリッスンしています")

    while True:
        client_socket, addr = server_socket.accept()
        print(f"{addr} からの接続を受け付けました")
        handle_client(client_socket)

if __name__ == "__main__":
    start_server()
