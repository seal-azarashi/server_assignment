# cszap サーバー構築課題

こちらの課題のためのレポジトリです: https://discord.com/channels/1084280443945353267/1256171728502001747/1289242887044862004

## server

MacOS の場合、ルートディレクトリで次のコマンドを実行して起動できます

```sh
gcc server/server.c -o server/server.out && ./server/server.out
```

次のコマンドで疎通確認が可能です (TODO: client 用のファイルが出来たら削除)

```sh
curl -i http://localhost:8080
```

## その他メモ

- try catch で囲んでエラーが発生したら stack trace を出す、みたいなのは出来ないのだろうか？
