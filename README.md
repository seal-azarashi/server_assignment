# cszap サーバー構築課題

こちらの課題のためのレポジトリです: https://discord.com/channels/1084280443945353267/1256171728502001747/1289242887044862004

## 環境構築 & 実行手順

現状クライアントもサーバーも同一の PC 内で起動されることを想定しています。

まずはサーバーを起動します。MacOS の場合、ルートディレクトリで次のコマンドを実行します。

```sh
make server
```

クライアントを実行します。MacOS の場合、ルートディレクトリで次のコマンドを実行します。

```sh
make client
```

それぞれプリントされる実行結果を確認してください。

## 今後の展望

- 余裕があれば実施することになっている以下の項目を実装に加える
    - thread を使って高速化
    - シグナルでサーバー停止
- サーバー、クライアントそれぞれ想定しているリクエスト or レスポンスへの対処しか出来ないので、相手が意図しない実装になっていても、バッファオーバーフロー等を起こさずに対処できるように工夫する
    - 例えば次のようなことをしようかなと考えている
        - 死活監視用の API として作り変える
        - サーバー側ではルート URI への GET リクエストしか許容せず、そのリクエストには明らかに不要な大きいデータが入っているといった、不正なリクエストと思しき要素があれば中身を確認せず捨てる
        - クライアント側でも同様に、サーバーが生きていることを表すにしては大きすぎるレスポンスが返って来た場合は無視する
    - またリクエストラインやレスポンスの生成を、ライブラリ関数を活用したものに変えると言ったリファクタリングも実施したい
- 環境依存のコードになっていないか精査する
    - 改行コードとか怪しいかもしれない
    - 場合によっては環境差分吸収のために Makefile など用意するかも
- C 言語を書くのが初めてで一般的な慣習に沿った実装に出来ているか自信がないため、さらに複数の実装例を読み、必要なものを今の実装に反映させる
    - rihib さんが挙げてたこれとか: https://i1i2i3.eidos.ic.i.u-tokyo.ac.jp/slides/socket.pdf
- ここまで学んだことをダンプしてドキュメントにまとめるなどしてみる

## その他気になったことメモ

- C では try catch で囲んでエラーが発生したら stack trace を出す、みたいなのは出来ないのだろうか？
