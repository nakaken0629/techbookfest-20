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

サンプルのdocker-compose.ymlをもとに、構成を説明する。ただしdocker-compose.ymlのソース自体は長いので省略する。

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
- images/chapter01/lightdash_install_02_nearly.pngを挿入する(scale=0.5)

3. については、以下の説明を入れる
- 今回はローカル環境で動作するPostgreSQLを利用するため、PostgreSQLを選択する。
- images/chapter01/lightdash_install_03_select_dwh.pngを挿入する(scale=0.5)。

4. については、以下の説明を入れる
- "Using your CLI"は、Lightdashと同じマシンに存在しているdbtプロジェクトを利用する。"Manually"は、GitHubに保存されたdbtプロジェクトを参照する。本格導入する場合は、GitHubから参照することが多いが、本書ではローカル環境で構築しているので前者を選択する。
- images/chapter01/lightdash_install_04_how_to_upload_dbt_project.pngを挿入する(scale=0.5)

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

- images/chapter01/lightdash_install_05_lightdash_cli.pngを挿入する(scale=0.5)

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

dbtのモデル情報を元に、ER図を作成する(縦長になるので、scale=0.5で描画する)。
マート層はスタースキーマに沿って設計されていることを説明する。

この後、データウェアハウスにおけるELTの、書き込み（Load）と変換（Transform）について説明していく。

説明：
今回demoデータベースは直接参照できるところにあるので、クラウドストレージなどへの抽出（Extract）は省略しています。

### .env.local

.envファイルは、Dockerのコンテナ内で動作する時の設定ですが、読み込みと変換はローカルPC上で行うものです。
そのため、別途.env.localファイルを用意して、接続先の設定などを行います。

（ここからソースコード）
サンプルの.env.localファイルの内容
（ここまでソースコード）

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

これで、Lightdashで参照するデータが、データウェアハウス上に作成されました。

## Lightdashの使い方

Lightdashの使い方は次の通り

1. クエリを作成する
2. クエリを元にチャートを作成する
3. 複数のチャートを組み合わせてダッシュボードを作成する

それぞれについて説明する
- クエリとは？
- チャートとは？
- ダッシュボードとは？

### クエリの作成方法

クエリを作成するときは、画面左上のNewメニューからChartを選択する。
基本的にクエリとチャートは同一画面で作るので、そういう操作になっている。

images/chapter01/query_1_choise_table.pngを挿入する

まず初めにクエリの基準となるテーブルを一つ選ぶ。
これは、スタースキーマの中心に来るべきテーブルを選ぶことになる。

コラム
左側のテーブル一覧に、マート層以外のモデルも表示されていることがある。その場合は、"Project Settings - Table Configuration"から、表示するテーブルを絞り込むことができることを説明する。

images/chapter01/query_2_choise_columns.pngを挿入する

ここで「購入」テーブルを選ぶ。
すると、項目を選べるようになる。
まずは、以下の二つの項目を選ぶ。選ぶときは項目名をクリックする。

 - 購入/Dimenstions/購入日時/Monthly
 - 購入/Metrics/売上合計

選んだ状態で"Run Query(500)"ボタンをクリックする。
すると、検索結果がテーブル形式で表示される。
購入日時の並び順は、列名の横の矢印を開くと、変えるためのメニューが表示される

images/chapter01/query_3_query_result.pngを挿入

これでクエリが作成されました。

### チャートの作成方法

表形式の上のChartというカテゴリを開く。
すると、棒グラフが表示されている。
Lightdashでは、クエリを作成すると、棒グラフのチャートが自動的に出来上がる。

images/chapter01/chart-1-default.pngを挿入

"Configure"ボタンをクリックすると、グラフの種類や設定を変える画面が出てくる。
チャートの種類を変えたり、グラフのスケールや凡例の説明などを変更することができる。

最後に"Save Chart"ボタンをクリックすると、クエリとチャートを保存することができる。

images/chapter01/chart-3-save.pngを挿入(scale=0.5)

### ダッシュボードの作成方法

クエリを作成するときは、画面左上のNewメニューからDashboardを選択する。

images/chapter01/dashboard-1-input-name.pngを挿入(scale=0.5)

名前を入力するダイアログが表示されるので、任意の名前を入力すると、空のダイアログが作成される。
"Add file"ボタンをクリックすると、ダッシュボードに追加できるオブジェクトが選択できる。
先ほど保存したチャートの他に、見た目を整えるためのテキストやマークダウンなども入力することができる。

images/chapter01/dashboard-2-create-empty-dashboard.pngを挿入

チャートなどを追加したダッシュボードはレイアウトを変更することができます。
レイアウトはグリッド方式になっているので、マウスでドラッグして移動したりサイズを変えたりすることができる。
好みのレイアウトになったら、"Save changes"ボタンを押して保存する。

images/chapter01/dashboard-3-finish.pngを挿入

これでダッシュボードが完成しました。

## セマンティックレイヤー

https://docs.lightdash.com/guides/lightdash-semantic-layer
上記のページを参考に、以下の内容を記載する

- セマンティックレイヤーとは何か？
- Lightdashにおけるセマンティックレイヤー
  - メトリクス
  - ディメンション
  - テーブル
- 組み込み方法
  - dbtのモデル定義に追記する方法と、独自のYAMLファイルを使う方法の二つがある。本書ではdbt projectが存在するため前者を採用している。

この後、購入モデルを使って、説明していく

### メトリクス

- メトリクスの説明
- 列がメトリクスである目安として、SQLでCOUNT, SUM, MAXなどの集合関数で使われる項目かどうか
- fct_purchaseに定義されているメトリクスの一覧（表形式）

- モデルに定義するメトリクスの例。購入明細単位ではなく購入単位の合計数
（ソースコードここから。書式はYAML。名前は"DISTINCTを指定した件数"）
models:
  - name: fct_purchase
    description: "購入ファクトテーブル（購入明細粒度）"
    config:
      meta:
        # 中略
        metrics:
          num_purchases:
            type: count_distinct
            sql: "${TABLE}.purchase_id"
            label: "購入回数"
            description: "ユニークな購入件数"
  # 以下略
（ソースコードここまで）

images/chapter01/semanticlayer-1-example-dimension-model.pngを挿入（scale=0.5）

- 列に定義するメトリクスの例。
（ソースコードここから。書式はYAML。名前は"小計の合計"）
models:
  - name: fct_purchase
    # 中略
    columns:
      # 中略
      - name: subtotal
        # 中略
        config:
          meta:
            # 中略
            metrics:
              total_revenue:
                type: sum
                label: "売上合計"
                format: "¥#,##0"
                description: "購入明細の小計合計"
  # 以下略
（ソースコードここまで）

images/chapter01/semanticlayer-2-example-dimension-column.pngを挿入（scale=0.5）

### ディメンション

- ディメンションの説明
- 列がディメンションである目安として、SQLでGROUP BYで使われる項目かどうか
- fct_purchaseに定義されているディメンションの一覧（表形式）

- モデルに定義するディメンションの例。ディメンション列を、"購入情報"と"金額・数量"の二つに分類している
（ソースコードここから。書式はYAML。名前は"ディメンション列のグルーピング"）
models:
  - name: fct_purchase
    description: "購入ファクトテーブル（購入明細粒度）"
    config:
      meta:
        label: "購入"
        primary_key: id
        group_details:
          purchase_info:
            label: "購入情報"
          amounts:
            label: "金額・数量"
  # 以下略
（ソースコードここまで）

images/chapter01/semanticlayer-3-example-metrics-model.pngを挿入（scale=0.5）

- 列に定義するディメンションの例。列を日付として定義して、group byの単位を指定している。
（ソースコードここから。書式はYAML。名前は"日付列の集計単位"）
models:
  - name: fct_purchase
      # 中略
    columns:
      # 中略
      - name: purchased_at
        description: "購入日時"
        config:
          meta:
            dimension:
              type: timestamp
              label: "購入日時"
              time_intervals: ['RAW', 'DAY', 'WEEK', 'MONTH', 'QUARTER', 'YEAR']
              groups: ["purchase_info"]
  # 以下略
（ソースコードここまで）

images/chapter01/semanticlayer-4-example-metrics-column.pngを挿入（scale=0.5）

### テーブル

- テーブル（主にジョイン）の説明
- fct_purchaseにジョインされているテーブルの一覧（表形式）

- ジョインの例。会員(member)と食品(food)とジョインしている。
（ソースコードここから。書式はYAML。名前は"ジョイン"）
models:
  - name: fct_purchase
    description: "購入ファクトテーブル（購入明細粒度）"
    config:
      meta:
        # 中略
        joins:
          - join: dim_member
            type: left
            sql_on: "${fct_purchase.member_id} = ${dim_member.id}"
            relationship: many-to-one
          - join: dim_food
            type: left
            sql_on: "${fct_purchase.food_id} = ${dim_food.id}"
            relationship: many-to-one
  # 以下略
（ソースコードここまで）

images/chapter01/semanticlayer-5-example-table.pngを挿入

### メトリクスの活用例（ドリルダウン）

