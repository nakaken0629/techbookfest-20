# dbtとlightdash

ここにあいさつと、この章の構成に沿ってどのようなことを説明するのかを記載する。

## Lightdashの概要

- lightdashの公式サイトから、lightdashの概要に関する記述を見つけて、ここにまとめる
- この章で紹介するdbtとlightdashのソースコードは、https://github.com/nakaken0629/utilizing-dbt-lightdashからダウンロードできることを説明する

### 動作環境

 - cloud版とセルフホスティング版についてそれぞれ説明を行う
 - 本書ではローカルマシンにDockerを使ってセルフホスティング方式であることを説明する

## 他のBIツールとの比較

- 対象BIツール
  - Tableau
  - Looker Studio
  - Redash
- メリットとデメリットという観点で書く

## lightdashの導入

以下の情報とサンプルソースを利用して、lightdashを導入する手順の説明と動作確認までをこなう

### 前提条件

- dbtプロジェクトが用意されていること
- Docker Desktopが導入されていること

### 構成

サンプルのdocker-compose.ymlをもとに、構成を説明する

- 構成図をかく
- 各コンテナに関する簡単な説明
  - lightdash: Lightdash本体
  - db: Lightdashの本体の情報を保存するデータベース
  - db-dwh: Lightdashが参照するデータウェアハウス
  - db-demo: データウェアハウスに取り込むソースデータが保存されているデータベース（ECサイトのデータベースを想定）
  - minio: Lightdashがクラウドストレージを利用するため、
  - minio-init: minio環境を準備するためのコンテナ

  コラムとして、なぜminioを使うのかを、lightdashとminioの話をインターネットから検索して説明する。

### 起動

- docker-compose.yamlの中で利用する環境変数を、.envとして用意する
- docker compose up -dで各コンテナを起動する
- docker compose psで起動したコンテナを確認する

コラムとして、終了するときには、docker compose downでコンテナを終了することを説明する。

### 初期化

初回起動時に、以下の手順に沿ってLightdashの初期化を行う

1. http://localhost:8080/にアクセスする。必要な情報を入力して"Sign Up"ボタンを押す。
2. 管理者の情報を入力して"Next"ボタンを押す
3. データウェアハウスの選択画面では、PostgreSQLを選ぶ。
4. dbtプロジェクトのアップロード方法については、"Using your CLI"を選択する。
5. 画面に表示されたコマンドを、lightdashコンテナの中で実行する
6. 初期化が完了したことを確認する

1. については、以下の説明を入れる
- 必要な情報は"First Name","Last Name","Email Address","Password"の四つ。表形式で挿入する。
- images/chapter01/lightdash_install_01_signup.pngを挿入する。

コラムとして、テスト用のメールアドレスには、example.comドメインを使うことが良いことの簡単な説明を、インターネットから情報を検索して記載する。

2. については、以下の説明を入れる

- 必要な情報は”Organization name"と"What's your role?"の二つ。特に動作が変わるわけではないので、どんな値を選択したり入力したりしても良い。その他の選択肢はデフォルトのままで良い
- images/chapter01/lightdash_install_02_nearly.pngを挿入する(
  scale=0.5)

3. については、以下の説明を入れる
- 今回はローカル環境で動作するPostgreSQLを利用するため、PostgreSQLを選択する。
- images/chapter01/lightdash_install_03_select_dwh.pngを挿入する(
  scale=0.5)。

4. については、以下の説明を入れる
- "Using your CLI"は、Lightdashと同じマシンに存在しているdbtプロジェクトを利用する。"Manually"は、GitHubに保存されたdbtプロジェクトを参照する。本格導入する場合は、GitHubから参照することが多いが、本書ではローカル環境で構築しているので前者を選択する。
- images/chapter01/lightdash_install_04_how_to_upload_dbt_project.pngを挿入する(
  scale=0.5)

5. については、以下の説明を入れる

- lightdashのコンテナに入るためには、以下のコマンドを実行する。成功すると、lightdashコンテナのプロンプトが表示される。

（ここからプログラムリスト。言語指定はなし）
# これは、Dockerのホストマシンのプロンプト
% docker compose exec lightdash bash

# これは、DockerのLightdashコンテナのプロンプト
root@ffe29f795685:/usr/app/packages/backend# 
（ここまでプログラムリスト）

- 画面上に表示されているコマンドを、そのままコピーして実行する。

（ここからプログラムリスト。言語指定はなし。文字がはみ出さないよう改行は厳格に維持する）
root@ffe29f795685:/usr/app/packages/backend# npm install -g @lightdash/cli@0.24
74.1
npm warn deprecated node-domexception@1.0.0: Use your platform's native DOMExce
ption instead
npm warn deprecated q@1.5.1: You or someone you depend on is using Q, the JavaS
cript Promise library that gave JavaScript developers strong feelings about pro
mises. They can almost certainly migrate to the native JavaScript promise now. 
Thank you literally everyone for joining me in this bet against the odds. Be ex
cellent to each other.
（中略）
root@ffe29f795685:/usr/app/packages/backend# lightdash login http://localhost:8
080 --token ldpat_35213b4f510ba6f2bd49104f61d35a8b

  Login successful

Now you can add your first project to lightdash by doing: 

  lightdash deploy --create

Done 🕶
root@ffe29f795685:/usr/app/packages/backend# lightdash deploy --create


- SUCCESS> stg_member_status_log 
- SUCCESS> stg_purchase_detail 
- SUCCESS> stg_food 
- SUCCESS> stg_category 
- SUCCESS> stg_member 
- SUCCESS> stg_purchase 
- SUCCESS> fct_purchase 
- SUCCESS> dim_food 
- SUCCESS> dim_member 

Compiled 9 explores, SUCCESS=9 ERRORS=0

? Add a project name or press enter to use the default: [Demo ec]  

? Do you confirm Lightdash can store your warehouse credentials so you can run
queries in Lightdash? Yes
? Do you want to save this answer for next time? Yes
   New project Demo ec created

Successfully deployed project:

      http://localhost:8080/createProject/cli?projectUuid=bb0c0d1b-8add-485a-9d09-9fe64345d255

Done 🕶
（ここまでプログラムリスト）

- images/chapter01/lightdash_install_05_lightdash_cli.pngを挿入する(
  scale=0.5)

6. については以下の説明を入れる
- 5. の実行結果の一番最後に表示されたURLにアクセスする
- images/chapter01/lightdash_install_06_finish.pngを挿入する

これでLightdashの初期化が終わりました。

## demoデータベース

食品をオンラインで販売するECサイトのデータベース(demo-db)が用意してあることを説明する。
demo/models.mdを元に、ER図を作成する

実際にWebアプリケーションがあるわけではく、それっぽいデータが作成されるシミュレータを用意していることを説明する。

### demoデータベースのデータ作成方法

以下の手順は、dockerコンテナが動作している状態で実行することを説明しておく。

1. demoディレクトリに移動する。
2. uv run init.pyを実行する→何が起きるかを説明する
3. uv run seed.pyを実行する→何が起きるかを説明する

これで、demoデータベースにデータが作成されました。

## dbtプロジェクトの説明

lightdashが利用するdbtプロジェクトを用意してあること。
この節では、デモ用のデータベースからデータウェアハウスにデータを取り込む方法について説明していることを記載する。

dbtのモデル情報を元に、ER図を作成する。
マート層はスタースキーマに沿って設計されていることを説明する。

この後、データウェアハウスにおけるELTの、書き込み（Load）と変換（Transform）について説明していく。

説明：
今回demoデータベースは直接参照できるところにあるので、クラウドストレージなどへの抽出（Extract）は省略しています。

### 読み込み（Load）

以下のことを説明する。

- 書き込みはdbtでは対応していない
- 一般的には、TROCCOやAirflowなどといった専用のツールを利用する。
- テスト用にはseedsディレクトリにcsvなどを用意する方法もあるが、もう少し凝ったデータを作成したかったので、代わりに簡単な独自のローダーツールを作成している(dbt_project/seeds_loader)
  - init.py: raw層用のスキーマと、読み込み先のテーブルを作成する
  - load.py: demoテーブルから、読み込み先のテーブルにデータを書き込む

実行方法について説明する

1. dbt_project/seeds_loaderディレクトリに移動する
2. uv run init.pyを実行する→何が起きるかを説明する
3. uv run load.pyを実行する→何が起きるかを説明する

### 変換（Transform）

以下について説明する

- 変換では、dbtの機能を使って、raw層に読み込まれたデータをstaging層から
marts層へと変換していく
- データのテスト、型変換、クリーニングなどは、今回は省略している。
- dbtはローカルPCで動かすほか、Lightdashが動作するDockerコンテナ上でも動作する。そのため、二つのTargetをprofiles.ymlに用意している。
  - local: ローカルPC上で動かす時のターゲット
  - dev(デフォルト): Dockerコンテナ上で動かす時のターゲット

実行方法について説明する

1. dbt_projectディレクトリに移動する
2. uv run dbt run --target localを実行する
