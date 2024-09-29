# cszap サーバー構築課題

## server

ルートディレクトリで次のコマンドを実行して起動できます

```sh
gcc server/server.c -o server/server.out && ./server/server.out
```

次のコマンドで疎通確認が可能です (TODO: client 用のファイルが出来たら削除)

```sh
curl -i http://localhost:8080
```
