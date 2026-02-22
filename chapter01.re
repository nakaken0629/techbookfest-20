#@# 最後の原稿生成時のコミット: 5380e39a9d4d70576456ee0d84b686e7765740cc

= dbtとLightdash

== 他のBIツールとの比較

BIツールは多種多様なものが存在します。Lightdashを選ぶにあたって、代表的なBIツールであるTableau、Looker Studio、Redashとの比較を行います。

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

== Lightdashの概要

OSSのBIツールであるLightdashは、dbtプロジェクトを起点とした「モデル駆動型BI」を実現できる点に大きな特長があります。dbtで定義したモデルや指標を、そのまま分析基盤における唯一の正として扱えるため、SQLやロジックの重複を避けやすく、指標定義のブレも防ぎやすくなります。OSSとしてセルフホストでき、ベンダーロックインを回避しやすい点も魅力です。さらに、開発者向けのコード管理と、非エンジニアでも扱える探索的UIを両立しており、データチームとビジネス部門の協業を強力に支援します。

Lightdashのメリットは以下の通りです。

 * dbtネイティブな設計
Lightdashはdbtプロジェクトを直接読み込み、モデルやメトリクスをそのままセマンティックレイヤーとして利用します。これにより、指標定義の二重管理を避け、分析ロジックの一貫性を保てます。

 * OSSでセルフホスト可能
オープンソースとして提供されており、自社環境へのデプロイが可能です。プロプライエタリなBIツールに比べ、ベンダーロックインを回避しやすい点が評価されています。

 * Lookerライクな探索UI
事前定義された指標を使った探索（Explore）やダッシュボード作成が可能で、SQLを書かずに分析できるUIを備えています。非エンジニアでも扱いやすい点が特長です。

 * 開発者フレンドリーな運用
Gitによるバージョン管理やdbt中心のワークフローと親和性が高く、アナリティクスエンジニアリングの実践に適しています。

またLightdashのデメリットは以下の通りです。

 * dbt前提のツールである
Lightdashはdbtを中心に設計されているため、dbtを採用していない組織では導入メリットが限定的になります。従来型のBIツールの代替として単独利用するのは難しい場合があります。

 * 機能面での成熟度
TableauやPower BIのような長年成熟したBI製品と比べると、高度な可視化や細かな表現力では制約があるとされています。

 * 運用には技術的知識が必要
OSS版をセルフホストする場合、インフラ構築や認証、アップデート管理などを自分たちで行う必要があり、一定の技術力が求められます。

=== 動作環境

提供形態は主に次の2つです。

 * セルフホスト：自社インフラに導入し、設定や運用を自分たちで管理します。
 * クラウド：Lightdashが提供するSaaSで、環境構築なしに利用開始できます。

セルフホストには、データ統制がしやすくネットワークや権限を自社方針に合わせやすいこと、認証や構成のカスタマイズ性が高く要件に応じて柔軟に設計できること、利用規模によってはクラウド課金より総コストを抑えられる場合があること、といったメリットがあります。一方でセルフホストは、監視・バックアップ・アップデート・障害対応などの運用負荷が増えること、セキュリティ責任が自社に寄るため脆弱性対応や権限設計の質が重要になること、立ち上げに時間がかかり運用が属人化しやすいことがデメリットになります。

クラウド形態には、環境構築やアップデートが不要ですぐに使い始められること、運用負荷を抑えやすく監視や障害対応の工数を削減できること、利用者や利用量に応じてスケールしやすく拡張が容易なこと、といったメリットがあります。一方でクラウド形態は、データや接続要件によって社内規程や法令対応の確認が必要になること、機能や設定の自由度が制限され個別要件に合わせにくい場合があること、利用規模によっては継続課金が総コスト増につながることがある点に注意が必要です。

本書では、ローカルPCにセルフホストという形態で、これから話を進めていきます。

== Lightdashの導入

本節では、サンプルソース（@<tt>{sample/utilizing-dbt-lightdash}）を使って、Lightdashをローカル環境に導入する手順を説明します。

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

# Lightdash
LIGHTDASH_SECRET=this-is-a-secret-key
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

「Organization name」（組織名）と「What's your role?」（役職）を入力して「Next」ボタンを押します（@<img>{chapter01/lightdash_install_02_nearly}）。どちらの値を入力しても動作は変わりませんので、自由に選択してください。その他の選択肢はデフォルトのままで問題ありません。

//image[chapter01/lightdash_install_02_nearly][組織情報の入力画面]{
//}

==== 3. データウェアハウスの選択

データウェアハウスの種類を選ぶ画面が表示されます（@<img>{chapter01/lightdash_install_03_select_dwh}）。今回はローカル環境で動作するPostgreSQLを利用するため、「PostgreSQL」を選択します。

//image[chapter01/lightdash_install_03_select_dwh][データウェアハウスの選択画面]{
//}

==== 4. dbtプロジェクトのアップロード方法を選択

dbtプロジェクトの参照方法を選びます（@<img>{chapter01/lightdash_install_04_how_to_upload_dbt_project}）。「Using your CLI」はLightdashと同じマシン上のdbtプロジェクトを利用する方法で、「Manually」はGitHubに保存されたdbtプロジェクトを参照する方法です。本格的な運用ではGitHubからの参照が一般的ですが、本書はローカル環境での構築を前提とするため、「Using your CLI」を選択します。

//image[chapter01/lightdash_install_04_how_to_upload_dbt_project][アップロード方法の選択画面]{
//}

==== 5. Lightdash CLIの実行

画面に表示されたコマンドを、Lightdashコンテナの中で実行します。まず以下のコマンドでLightdashコンテナに入ります（@<list>{exec_lightdash}）。成功するとLightdashコンテナのプロンプトが表示されます。

//list[exec_lightdash][Lightdashコンテナへの接続]{
# ホストマシンのプロンプトで実行する
% docker compose exec lightdash bash

# 成功するとLightdashコンテナのプロンプトが表示される
root@ffe29f795685:/usr/app/packages/backend#
//}

次に、画面に表示されているコマンドをそのままコピーして実行します（@<list>{lightdash_cli}）。

//list[lightdash_cli][Lightdash CLIの実行例]{
root@ffe29f795685:/usr/app/packages/backend# npm install -g @lightdash/cli@0.2474.1
（インストールログ略）
root@ffe29f795685:/usr/app/packages/backend# lightdash login http://localhost:8080 --token ldpat_XXXX

  ✅️ Login successful

Now you can add your first project to lightdash by doing:

  ⚡️ lightdash deploy --create

Done 🕶
root@ffe29f795685:/usr/app/packages/backend# lightdash deploy --create

- SUCCESS> stg_customers
- SUCCESS> stg_orders
- SUCCESS> stg_products
- SUCCESS> int_orders_with_products
- SUCCESS> fct_orders
- SUCCESS> dim_customers

Compiled 6 explores, SUCCESS=6 ERRORS=0

? Add a project name or press enter to use the default: [Demo ec]
? Do you confirm Lightdash can store your warehouse credentials so you can run queries in Lightdash? Yes
? Do you want to save this answer for next time? Yes
✔   New project Demo ec created

Successfully deployed project:

      ⚡️ http://localhost:8080/createProject/cli?projectUuid=XXXX

Done 🕶
//}

//image[chapter01/lightdash_install_05_lightdash_cli][Lightdash CLIの実行結果]{
//}

==== 6. 初期化の完了

手順5の実行結果の末尾に表示されたURLにアクセスします（@<img>{chapter01/lightdash_install_06_finish}）。Lightdashのダッシュボードが表示されれば、初期化は完了です。

//image[chapter01/lightdash_install_06_finish][初期化完了画面]{
//}

これでLightdashの初期化が完了しました。

