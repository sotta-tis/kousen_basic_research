# drogon test for kousen
docker drogon をつかったwebサーバのテスト用開発環境

こまったり、リファレンス欲しかったら下記参照

実行方法やビルドのコマンドについて等も詳しくはこちら参照（ビルドのコマンドはbuild.shでまとめている）

[参照(drogon wiki)](https://github.com/drogonframework/drogon/wiki)

## 依存環境
dockerのみ

## 構成

|コンテナ|ベースイメージ|
|:----:|:--:|
|kousen_test|ubuntu|

## ubuntuコンテナ内の詳細
- drogon
  
/install/　直下にdrogonのビルド用ファイル

触ることはおそらくない

- projectファイル
  
/home/work/kousen　にローカルとマウントされたプロジェクトファイル

プロジェクトの編集、変更、追加はすべてここで行う。

## 使い方
1. gitクローン

くろーんしてくれ


3. dockerのコンテナの立ち上げ 

注意：別にポート80を使っていたら競合してまうから開けとく
```shell
$ docker-compose up -d
```

3. dockerコンテナにアタッチ
コンテナのなかに入ってわいが作ったシェルスクリプトを実行する
 ```shell
$ docker compose exec drogon bash
```



projectのビルド

シェルスクリプト使用

```shell
# ビルドだけ、デプロイなし
$ build.sh
# ビルド、デプロイ
$ debug.sh
```

手動でのデプロイ

projectのbuildまでいってからcmake .. && make する
```shell
# cd /home/work/kousen/build/
# cmake ..
# make
# ./kousen
```

シェルスクリプトやらんと↑これやる

4. localhost:8085接続

これがでたらおけ
![スクリーンショット 2024-04-14 200006](https://github.com/sotta-tis/kousen_test/assets/104351502/441b7870-364d-49d4-b70f-a0ce33a11b9b)
