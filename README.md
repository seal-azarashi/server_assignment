# cszap サーバー構築課題

こちらの課題のためのレポジトリです: https://discord.com/channels/1084280443945353267/1256171728502001747/1289242887044862004

## server

MacOS の場合、ルートディレクトリで次のコマンドを実行して起動できます

```sh
gcc server.c -o server.out && ./server.out
```

## client

MacOS の場合、ルートディレクトリで次のコマンドを実行して起動できます

```sh
gcc client.c -o client.out && ./client.out
```

サーバーを立ち上げた上で起動することで動作確認が可能です。

## その他メモ

- try catch で囲んでエラーが発生したら stack trace を出す、みたいなのは出来ないのだろうか？
