#@# 最後の原稿生成時のコミット: 00c73b3f9c6d5572b985324ee139ebfc346579c5

= dbtとLightdash

本章では、dbtとLightdashを組み合わせたデータ分析基盤の構築方法を説明します。まずLightdashとはどのようなツールなのかを概説し、他のBIツールと比較しながらその特徴を整理します。続いて、サンプルソースを使ってLightdashをローカル環境へ導入する手順を解説します。最後に、分析の元となるデモデータベースの構成と、dbtプロジェクトによるELT処理の流れを説明します。

この章で紹介するdbtとLightdashのサンプルソースは、以下のGitHubリポジトリから参照してください。

@<tt>{https://github.com/nakaken0629/utilizing-dbt-lightdash}

== Lightdashの概要

Lightdashは、dbtプロジェクトを起点とした「モデル駆動型BI」を実現するオープンソースのBIツールです。dbtで定義したモデルや指標を、そのまま分析基盤における唯一の正として扱えるため、SQLやロジックの重複を避けやすく、指標定義のブレも防ぎやすくなります。OSSとしてセルフホストでき、ベンダーロックインを回避しやすい点も魅力です。さらに、開発者向けのコード管理と、非エンジニアでも扱える探索的UIを両立しており、データチームとビジネス部門の協業を強力に支援します。

=== 動作環境

Lightdashの提供形態は主に次の2種類です。

 * クラウド版
Lightdashが提供するSaaSです。環境構築なしに利用を開始できます。運用や監視はLightdash社が担うため、すぐに使い始めたい場合や運用コストを抑えたい場合に向いています。

 * セルフホスティング版
自社インフラにDockerやKubernetesを使って構築し、設定や運用を自分たちで管理する形態です。データの統制がしやすく、認証や構成のカスタマイズ性が高い点がメリットです。一方で、監視・バックアップ・アップデートなどの運用負荷が増える点には注意が必要です。

本書では、ローカルPCにDockerを使ってセルフホスティング方式で構築する形で話を進めていきます。

== 他のBIツールとの比較

BIツールは多種多様なものが存在します。Lightdashを選ぶにあたって、代表的なBIツールであるTableau、Looker Studio、Redashとのメリット・デメリットを比較します。

=== Tableau

Tableauは世界的に広く利用されているBIツールです。豊富な可視化オプションと直感的なドラッグ&ドロップUIが特長で、データアナリストから経営層まで幅広いユーザーに使われています。

Tableauのメリットは以下の通りです。

 * 豊富な可視化オプション
高度なグラフや地図など、多様な可視化表現が可能です。データの表現力において業界トップクラスとされています。

 * 強力なデータ接続機能
多数のデータソースへの接続に対応しており、様々なデータベースやクラウドサービスと連携できます。

 * 成熟したエコシステム
長年の実績があり、ユーザーコミュニティや学習リソースが充実しています。

一方でTableauのデメリットは以下の通りです。

 * ライセンスコストが高い
プロプライエタリなソフトウェアであり、ライセンス費用が高額になることがあります。特に大規模展開ではコストが課題になります。

 * dbtとの連携が限定的
dbtのモデルや指標をそのまま利用する仕組みがなく、指標定義の二重管理が発生しやすいです。

 * SQLの直接管理が難しい
SQLロジックをGit管理しながら運用するアナリティクスエンジニアリングのワークフローとの親和性が低いです。

=== Looker Studio

Looker Studio（旧Google Data Studio）はGoogleが提供する無料のBIツールです。Googleサービスとの親和性が高く、Google Analytics、BigQuery、Google Sheetsなどと簡単に連携できます。

Looker Studioのメリットは以下の通りです。

 * 無料で利用可能
基本機能は無料で提供されており、スモールスタートに適しています。

 * Googleサービスとの連携が容易
Google Analytics、BigQuery、Google Sheetsなど、Googleのサービスとシームレスに接続できます。

 * ブラウザベースで共有しやすい
レポートをURLで共有でき、チーム内での共有が簡単です。

一方でLooker Studioのデメリットは以下の通りです。

 * 機能の制限
高度な分析や複雑なデータ変換には対応しておらず、大規模なデータ分析には不向きな場合があります。

 * dbtとの連携が限定的
Lightdashのようにdbtモデルをネイティブに読み込む機能はなく、指標の二重管理が発生します。

 * パフォーマンスの問題
大量データを扱う場合にレスポンスが遅くなることがあります。

=== Redash

Redashはオープンソースのデータ可視化ツールで、SQLベースのクエリ実行とダッシュボード作成を主な機能としています。エンジニアやデータアナリストに好まれるツールです。

Redashのメリットは以下の通りです。

 * OSSでセルフホスト可能
オープンソースであり、自社環境に構築することでコストを抑えられます。

 * SQLベースで柔軟
SQLを直接記述して分析できるため、エンジニアやデータアナリストにとって使いやすいです。

 * 多様なデータソース対応
多くのデータベースやAPIに対応しており、様々なデータソースに接続できます。

一方でRedashのデメリットは以下の通りです。

 * 非エンジニアには敷居が高い
SQLの知識が必要であり、ビジネスユーザーが自分で分析するには難しい場合があります。

 * dbtとの連携が限定的
dbtモデルをセマンティックレイヤーとして活用する機能がなく、dbtとの統合が浅いです。

 * メンテナンス状況の懸念
開発の活発さが以前より低下しており、長期的なメンテナンスへの懸念があります。

=== Lightdashを選ぶ理由

これらのBIツールと比較すると、Lightdashは特にdbtを採用しているチームに対して強みを持ちます。dbtのモデルや指標をそのままセマンティックレイヤーとして利用できるため、指標定義の二重管理を避け、データの一貫性を保ちやすい点が最大の特長です。OSSとしてセルフホストできる点はRedashと共通しますが、dbtとの深い統合という点でLightdashが優位に立ちます。

== Lightdashの導入

本節では、サンプルソースを使って、Lightdashをローカル環境に導入する手順を説明します。

=== 前提条件

本章の手順を進めるにあたって、以下の環境が用意されていることを前提とします。

 * dbtプロジェクトが用意されていること
 * Docker Desktopが導入されていること

=== 構成

サンプルの@<tt>{docker-compose.yml}をもとに、Lightdashを動かすために必要なコンテナ構成を説明します（@<img>{chapter01/architecture}）。

//image[chapter01/architecture][構成図]{
//}

各コンテナの役割は以下の通りです。

 * lightdash
Lightdash本体です。dbtプロジェクトを読み込み、BI画面を提供します。

 * db
Lightdashがユーザーやダッシュボードなどのアプリケーション情報を保存するためのPostgreSQLデータベースです。

 * dwh-db
Lightdashが分析クエリを実行する対象となるデータウェアハウスです。本書ではPostgreSQLで代用します。dbtが変換したデータモデルはこのデータベースに格納されます。

 * demo-db
データウェアハウスに取り込むソースデータが保存されているデータベースです。本書では食品をオンラインで販売するECサイトのデータベースを想定しています。

 * minio
S3互換のオブジェクトストレージです。Lightdashはチャートのエクスポートなどにクラウドストレージを必要とするため、ローカル環境ではMinIOで代替します。

 * minio-init
MinIOのバケット作成など初期設定を行うための初期化コンテナです。起動時に一度だけ実行されます。

===[column] なぜMinIOを使うのか？

Lightdashはクラウドストレージ（主にAmazon S3）を、チャート画像のエクスポートやスケジュール配信などの機能に利用します。本番環境ではS3などのクラウドサービスを使えますが、ローカル開発環境ではそのまま使用できません。

MinIOはS3互換のAPIを提供するオープンソースのオブジェクトストレージで、Dockerで手軽に起動できます。LightdashのS3設定にMinIOのエンドポイントを指定することで、ローカル環境でもクラウドストレージを必要とする機能を動作させることができます。

===[/column]

実際の@<tt>{docker-compose.yml}は以下の通りです（@<list>{docker_compose_yml}）。

//list[docker_compose_yml][docker-compose.yml]{
services:
  db:
    image: postgres:15.4
    restart: always
    environment:
      POSTGRES_USER: ${PGUSER}
      POSTGRES_PASSWORD: ${PGPASSWORD}
      POSTGRES_DB: ${PGDATABASE}
    ports:
      - "5433:5432"
    volumes:
      - db-data:/var/lib/postgresql/data

  dwh-db:
    image: postgres:15.4
    restart: always
    environment:
      POSTGRES_USER: ${DWH_PGUSER}
      POSTGRES_PASSWORD: ${DWH_PGPASSWORD}
      POSTGRES_DB: ${DWH_PGDATABASE}
    ports:
      - "5434:5432"
    volumes:
      - dwh-data:/var/lib/postgresql/data
    healthcheck:
      test: ["CMD-SHELL", "pg_isready -U ${DWH_PGUSER} -d ${DWH_PGDATABASE}"]
      interval: 5s
      timeout: 5s
      retries: 10

  demo-db:
    image: postgres:15.4
    restart: always
    environment:
      POSTGRES_USER: ${DEMO_PGUSER}
      POSTGRES_PASSWORD: ${DEMO_PGPASSWORD}
      POSTGRES_DB: ${DEMO_PGDATABASE}
    ports:
      - "5435:5432"
    volumes:
      - demo-data:/var/lib/postgresql/data
    healthcheck:
      test: ["CMD-SHELL", "pg_isready -U ${DEMO_PGUSER} -d ${DEMO_PGDATABASE}"]
      interval: 5s
      timeout: 5s
      retries: 10

  minio:
    image: minio/minio
    restart: always
    environment:
      MINIO_ROOT_USER: minioadmin
      MINIO_ROOT_PASSWORD: minioadmin
    command: server /data --console-address ":9001"
    ports:
      - "9000:9000"
      - "9001:9001"
    volumes:
      - minio-data:/data

  minio-init:
    image: minio/mc
    depends_on:
      - minio
    entrypoint: >
      /bin/sh -c "
      sleep 3;
      mc alias set local http://minio:9000 minioadmin minioadmin;
      mc mb --ignore-existing local/lightdash;
      exit 0;
      "

  lightdash:
    image: lightdash/lightdash:latest
    platform: linux/amd64
    restart: always
    depends_on:
      - db
      - dwh-db
      - minio-init
    environment:
      PGHOST: ${PGHOST}
      PGPORT: ${PGPORT}
      PGUSER: ${PGUSER}
      PGPASSWORD: ${PGPASSWORD}
      PGDATABASE: ${PGDATABASE}
      LIGHTDASH_SECRET: ${LIGHTDASH_SECRET}
      SITE_URL: ${SITE_URL}
      PORT: 8080
      LIGHTDASH_LOG_LEVEL: debug
      LIGHTDASH_QUERY_MAX_LIMIT: 5000
      S3_ENDPOINT: http://minio:9000
      S3_BUCKET: lightdash
      S3_REGION: us-east-1
      S3_ACCESS_KEY: minioadmin
      S3_SECRET_KEY: minioadmin
      S3_FORCE_PATH_STYLE: "true"
      DBT_PROFILES_DIR: /usr/app/dbt
      DBT_PROJECT_DIR: /usr/app/dbt
    ports:
      - "8080:8080"
    volumes:
      - ./dbt_project:/usr/app/dbt

volumes:
  db-data:
  dwh-data:
  demo-data:
  minio-data:
//}

=== 起動

Lightdashを起動するには、まず@<tt>{docker-compose.yml}が参照する環境変数を記述した@<tt>{.env}ファイルを用意します（@<list>{env}）。

//list[env][.env]{
# Lightdash メタデータ用 PostgreSQL (db)
PGHOST=db
PGPORT=5432
PGUSER=lightdash
PGPASSWORD=lightdash_password
PGDATABASE=lightdash

# DWH用 PostgreSQL (dwh-db)
DWH_PGHOST=dwh-db
DWH_PGPORT=5432
DWH_PGUSER=dbt_user
DWH_PGPASSWORD=dbt_password
DWH_PGDATABASE=dbt_warehouse

# Demo用 PostgreSQL (demo-db)
DEMO_PGHOST=demo-db
DEMO_PGPORT=5432
DEMO_PGUSER=demo_user
DEMO_PGPASSWORD=demo_password
DEMO_PGDATABASE=demo_db

# Lightdash
LIGHTDASH_SECRET=my-lightdash-secret-key-change-me
SITE_URL=http://localhost:8080
//}

@<tt>{.env}を用意したら、以下のコマンドで各コンテナをバックグラウンドで起動します（@<list>{docker_compose_up}）。

//list[docker_compose_up][コンテナの起動]{
docker compose up -d
//}

起動後、@<tt>{docker compose ps}で各コンテナの状態を確認します（@<list>{docker_compose_ps}）。

//list[docker_compose_ps][コンテナの状態確認]{
docker compose ps
//}

すべてのコンテナが@<tt>{running}状態になったら、起動は成功です。

===[column] コンテナの停止

作業が終わったら、以下のコマンドでコンテナを停止します。

//list[docker_compose_down][コンテナの停止]{
docker compose down
//}

===[/column]

=== 初期化

初回起動時には、以下の手順に沿ってLightdashの初期化を行います。

 1. @<tt>{http://localhost:8080/}にアクセスし、必要な情報を入力して「Sign Up」ボタンを押す
 2. 管理者の情報を入力して「Next」ボタンを押す
 3. データウェアハウスの選択画面では、PostgreSQLを選ぶ
 4. dbtプロジェクトのアップロード方法については、「Using your CLI」を選択する
 5. 画面に表示されたコマンドをLightdashコンテナの中で実行する
 6. 初期化が完了したことを確認する

==== 1. アカウント登録（Sign Up）

@<tt>{http://localhost:8080/}にアクセスすると、Sign Up画面が表示されます（@<img>{chapter01/lightdash_install_01_signup}）。以下の情報を入力して「Sign Up」ボタンを押します。

//table[signup][サインアップに必要な情報]{
フィールド名	説明
--------------------------
First Name	名前（名）
Last Name	名前（姓）
Email Address	メールアドレス
Password	パスワード
//}

//image[chapter01/lightdash_install_01_signup][Sign Up画面]{
//}

===[column] テスト用のメールアドレスについて

テスト環境では、実在するドメインのメールアドレスを誤って使用しないよう、RFC 2606 で予約されている @<tt>{example.com}（@<tt>{example.org}、@<tt>{example.net} も同様）を使うのがよい習慣です。これらはIANA（インターネット番号割当機関）が管理しており、実際にメールが届くことはないため、誤送信のリスクなく安全に使えます。

===[/column]

==== 2. 組織情報の入力

「Organization name」（組織名）と「What's your role?」（役職）を入力して「Next」ボタンを押します（@<img>{chapter01/lightdash_install_02_nearly}）。特に動作が変わるわけではないので、どんな値を選択したり入力したりしても構いません。その他の選択肢はデフォルトのままで良いです。

//image[chapter01/lightdash_install_02_nearly][組織情報の入力画面][scale=0.5]{
//}

==== 3. データウェアハウスの選択

データウェアハウスの種類を選ぶ画面が表示されます（@<img>{chapter01/lightdash_install_03_select_dwh}）。今回はローカル環境で動作するPostgreSQLを利用するため、「PostgreSQL」を選択します。

//image[chapter01/lightdash_install_03_select_dwh][データウェアハウスの選択画面][scale=0.5]{
//}

==== 4. dbtプロジェクトのアップロード方法を選択

dbtプロジェクトの参照方法を選びます（@<img>{chapter01/lightdash_install_04_how_to_upload_dbt_project}）。「Using your CLI」はLightdashと同じマシン上のdbtプロジェクトを利用する方法で、「Manually」はGitHubに保存されたdbtプロジェクトを参照する方法です。本格的な運用ではGitHubからの参照が一般的ですが、本書はローカル環境での構築を前提とするため、「Using your CLI」を選択します。

//image[chapter01/lightdash_install_04_how_to_upload_dbt_project][アップロード方法の選択画面][scale=0.5]{
//}

==== 5. Lightdash CLIの実行

画面に表示されたコマンドを、Lightdashコンテナの中で実行します。まず以下のコマンドでLightdashコンテナに入ります（@<list>{exec_lightdash}）。成功するとLightdashコンテナのプロンプトが表示されます。

//list[exec_lightdash][Lightdashコンテナへの接続]{
# これは、Dockerのホストマシンのプロンプト
% docker compose exec lightdash bash

# これは、DockerのLightdashコンテナのプロンプト
root@ffe29f795685:/usr/app/packages/backend#
//}

次に、画面に表示されているコマンドをそのままコピーして実行します（@<list>{lightdash_cli}）。

//list[lightdash_cli][Lightdash CLIの実行例]{
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

      http://localhost:8080/createProject/cli?projectUuid=bb0c0d1b-8add-485a-9
d09-9fe64345d255

Done 🕶
//}

//image[chapter01/lightdash_install_05_lightdash_cli][Lightdash CLIの実行結果][scale=0.5]{
//}

==== 6. 初期化の完了

手順5の実行結果の末尾に表示されたURLにアクセスします（@<img>{chapter01/lightdash_install_06_finish}）。Lightdashのダッシュボードが表示されれば、初期化は完了です。

//image[chapter01/lightdash_install_06_finish][初期化完了画面]{
//}

これでLightdashの初期化が終わりました。

== demoデータベース

本章のサンプルでは、食品をオンラインで販売するECサイトのデータベース（demo-db）を用意しています。実際にWebアプリケーションが動いているわけではなく、ECサイトをシミュレートしたデータを生成するシミュレータを用意しています。

demoデータベースのER図を@<img>{chapter01/demo_er}に示します。会員（member）、カテゴリ（category）、食品（food）、購入（purchase）、購入明細（purchase_detail）、会員ステータス変更履歴（member_status_log）の6つのテーブルで構成されています。

//image[chapter01/demo_er][demoデータベースのER図]{
//}

=== demoデータベースのデータ作成方法

以下の手順は、Dockerコンテナが動作している状態で実行してください。

 1. @<tt>{demo}ディレクトリに移動する
 2. @<tt>{uv run init.py}を実行する
 3. @<tt>{uv run seed.py}を実行する

手順2の@<tt>{uv run init.py}は、demo-dbデータベースとテーブルを初期化します。すでにデータベースが存在する場合は一度削除してから再作成するため、既存のデータはすべて消えます。初回実行時や、データをリセットしたいときに実行してください。

手順3の@<tt>{uv run seed.py}は、テストデータ生成のシミュレータです。会員の登録・有料会員への移行・購入・退会といった一連の行動を日付ごとにシミュレートし、その結果をdemo-dbに投入します。

これで、demoデータベースにデータが作成されました。

== dbtプロジェクトの説明

本節では、Lightdashが利用するdbtプロジェクトの構成を説明します。demoデータベースからデータウェアハウスにデータを取り込む方法、そしてdbtによるデータ変換の流れを解説します。

dbtプロジェクトのマート層のER図を@<img>{chapter01/dbt_er}に示します。マート層はスタースキーマに沿って設計されており、購入明細を中心としたファクトテーブル（fct_purchase）と、食品（dim_food）・会員（dim_member）の2つのディメンションテーブルで構成されています。

//image[chapter01/dbt_er][dbtプロジェクトのマート層ER図]{
//}

この後、データウェアハウスにおけるELTの、書き込み（Load）と変換（Transform）について説明していきます。

//note[抽出（Extract）について]{
今回demo-dbは直接参照できるところにあるので、クラウドストレージなどへの抽出（Extract）は省略しています。
//}

=== 読み込み（Load）

dbtはデータウェアハウスへの書き込み（Load）には対応していません。一般的には、TROCCOやAirflowなどといった専用のETL/ELTツールを利用します。

本書では、テスト用に簡単な独自のローダーツールを@<tt>{dbt_project/seeds_loader}ディレクトリに用意しています。なお、dbtには@<tt>{seeds}ディレクトリにCSVファイルを配置してデータを投入する機能もありますが、より実際のデータに近いものを作成したかったため、今回は独自のローダーツールを作成しました。

 * @<tt>{init.py}
データウェアハウス（dwh-db）内に@<tt>{public_raw}スキーマを作成し、データの受け口となるテーブルを用意します。

 * @<tt>{load.py}
demo-dbの各テーブルのデータを、dwh-dbの@<tt>{public_raw}スキーマにそのままコピーします。

実行方法は以下の通りです。

 1. @<tt>{dbt_project/seeds_loader}ディレクトリに移動する
 2. @<tt>{uv run init.py}を実行する
 3. @<tt>{uv run load.py}を実行する

手順2の@<tt>{uv run init.py}は、dwh-db内に@<tt>{public_raw}スキーマを作成します。すでにスキーマが存在する場合は削除して再作成するため、既存データは消えます。

手順3の@<tt>{uv run load.py}は、demo-dbの各テーブルの内容をdwh-dbの@<tt>{public_raw}スキーマにコピーします。実行のたびに既存データを洗い替えします。

=== 変換（Transform）

変換では、dbtの機能を使って、raw層（@<tt>{public_raw}）に読み込まれたデータをstaging層からmarts層へと変換していきます。データのテスト、型変換、クリーニングなどは、今回は省略しています。

dbtはローカルPCで動かすほか、Lightdashが動作するDockerコンテナ上でも動作します。そのため、@<tt>{profiles.yml}に以下の2つのターゲットを用意しています。

 * @<tt>{local}
ローカルPC上でdbtを動かす時のターゲットです。dwh-dbには@<tt>{localhost:5434}で接続します。

 * @<tt>{dev}（デフォルト）
DockerコンテナのLightdash上でdbtを動かす時のターゲットです。dwh-dbにはコンテナ名@<tt>{dwh-db}で接続します。

ローカルPCから変換を実行する場合は、以下の手順に沿って実行してください。

 1. @<tt>{dbt_project}ディレクトリに移動する
 2. @<tt>{uv run dbt run --target local}を実行する
