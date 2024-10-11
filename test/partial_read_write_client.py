"""
サーバーが Partial write, read に耐えれるかをテストするためのクライアントです。
C で作るのが大変だったので、 Claude くんに Python で雑に作ってもらいました...
"""

import socket
import time
import sys

def send_request_in_chunks(sock, request, chunk_size=10):
    try:
        for i in range(0, len(request), chunk_size):
            chunk = request[i:i+chunk_size]
            bytes_sent = sock.send(chunk.encode('utf-8'))
            print(f"送信: {bytes_sent} バイト")
            time.sleep(0.1)
    except BrokenPipeError:
        print("エラー: サーバーが接続を閉じました。")
    except Exception as e:
        print(f"エラー: 送信中に問題が発生しました - {e}")

def receive_response_in_chunks(sock, chunk_size=10):
    response = b''
    try:
        while True:
            chunk = sock.recv(chunk_size)
            if not chunk:
                break
            response += chunk
            print(f"受信: {len(chunk)} バイト")
            time.sleep(0.1)
    except Exception as e:
        print(f"エラー: 受信中に問題が発生しました - {e}")
    return response.decode('utf-8')

def test_server(host='::1', port=8080):
    try:
        sock = socket.socket(socket.AF_INET6, socket.SOCK_STREAM)
        sock.settimeout(5)  # 5秒のタイムアウトを設定
        sock.connect((host, port))
        print(f"サーバー {host}:{port} に接続しました。")

        request = "GET / HTTP/1.1\r\nHost: localhost\r\n\r\n"
        print("リクエストの送信を開始:")
        send_request_in_chunks(sock, request)
        print("リクエストの送信完了")

        print("レスポンスの受信を開始:")
        response = receive_response_in_chunks(sock)
        print("受信したレスポンス:")
        print(response)
    except socket.timeout:
        print("エラー: サーバーへの接続がタイムアウトしました。")
    except ConnectionRefusedError:
        print("エラー: サーバーへの接続が拒否されました。サーバーが実行中かどうか確認してください。")
    except Exception as e:
        print(f"エラー: 予期せぬ問題が発生しました - {e}")
    finally:
        sock.close()

if __name__ == "__main__":
    if len(sys.argv) > 2:
        test_server(sys.argv[1], int(sys.argv[2]))
    else:
        test_server()
