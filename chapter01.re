= dbtとLightdash

== Lightdashとは

OSSのBIツールである Lightdash は、dbtプロジェクトを起点とした「モデル駆動型BI」を実現できる点に大きな特長があります。dbtで定義したモデルや指標を、そのまま分析基盤における唯一の正として扱えるため、SQLやロジックの重複を避けやすく、指標定義のブレも防ぎやすくなります。OSSとしてセルフホストでき、ベンダーロックインを回避しやすい点も魅力です。さらに、開発者向けのコード管理と、非エンジニアでも扱える探索的UIを両立しており、データチームとビジネス部門の協業を強力に支援します。

lightdashのメリットは以下の通りです。

 * dbtネイティブな設計
Lightdashはdbtプロジェクトを直接読み込み、モデルやメトリクスをそのままセマンティックレイヤーとして利用します。これにより、指標定義の二重管理を避け、分析ロジックの一貫性を保てます。 [Document | Word], [lightdash.com]

 * OSSでセルフホスト可能
オープンソースとして提供されており、自社環境へのデプロイが可能です。プロプライエタリなBIツールに比べ、ベンダーロックインを回避しやすい点が評価されています。 [lightdash.com], [moge.ai]

 * Lookerライクな探索UI
事前定義された指標を使った探索（Explore）やダッシュボード作成が可能で、SQLを書かずに分析できるUIを備えています。非エンジニアでも扱いやすい点が特長です。 [lightdash.com], [moge.ai]

 * 開発者フレンドリーな運用
Gitによるバージョン管理やdbt中心のワークフローと親和性が高く、アナリティクスエンジニアリングの実践に適しています。 [datools.org]
 
またlightdashのデメリットは以下の通りです。

 * dbt前提のツールである
Lightdashはdbtを中心に設計されているため、dbtを採用していない組織では導入メリットが限定的になります。従来型のBIツールの代替として単独利用するのは難しい場合があります。 [lightdash.com], [datools.org]

 * 機能面での成熟度
TableauやPower BIのような長年成熟したBI製品と比べると、高度な可視化や細かな表現力では制約があるとされています。 [lightdash.com], [moge.ai]

 * 運用には技術的知識が必要
OSS版をセルフホストする場合、インフラ構築や認証、アップデート管理などを自分たちで行う必要があり、一定の技術力が求められます。 [moge.ai]

== Lightdashの提供形態

提供形態は主に次の2つです。

 * セルフホスト：自社インフラに導入し、設定や運用を自分たちで管理します。
 * クラウド：Lightdashが提供するSaaSで、環境構築なしに利用開始できます。

セルフホストには、データ統制がしやすくネットワークや権限を自社方針に合わせやすいこと、認証や構成のカスタマイズ性が高く要件に応じて柔軟に設計できること、利用規模によってはクラウド課金より総コストを抑えられる場合があること、といったメリットがあります。一方でセルフホストは、監視・バックアップ・アップデート・障害対応などの運用負荷が増えること、セキュリティ責任が自社に寄るため脆弱性対応や権限設計の質が重要になること、立ち上げに時間がかかり運用が属人化しやすいことがデメリットになります。

クラウド形態には、環境構築やアップデートが不要ですぐに使い始められること、運用負荷を抑えやすく監視や障害対応の工数を削減できること、利用者や利用量に応じてスケールしやすく拡張が容易なこと、といったメリットがあります。一方でクラウド形態は、データや接続要件によって社内規程や法令対応の確認が必要になること、機能や設定の自由度が制限され個別要件に合わせにくい場合があること、利用規模によっては継続課金が総コスト増につながることがある点に注意が必要です。

本書では、ローカルPCにセルフホストという形態で、これから話を進めていきます。

== まずは起動してみよう

まずはlightdashを起動して見ましょう。LightdashはDockerイメージが公開されているので、Dockerを使って起動するのが簡単です。データベースとしてはPostgreSQLを使いましょう。そして気を付ける点として、Lightdashは設定情報を保存する場所として、クラウドストレージを要求します。クラウド上にデプロイする時にはS3などの本物のクラウドストレージを使えばいいのですが、ローカルに構築する時には使えません。そのため、Dockerイメージとして公開されているS3互換のMinIOを使います。

ここでは、複数のDockerイメージをまとめて起動するために、Docker Composeを使います。docker-compose.yamlを以下の通り作って見ましょう（@<list>{docker_compose_yml}）。

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
    ports:
      - "8080:8080"
    volumes:
      - ./dbt_project:/usr/app/dbt

volumes:
  db-data:
  dwh-data:
  minio-data:
//}

いくつか環境変数を参照するようになっています。これらの環境変数は直接シェルで定義することもできますが、.envファイルで準備することも可能です（@<list>{_env}）

//list[_env][.env]{
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
LIGHTDASH_SECRET=my-lightdash-secret-key-change-me
SITE_URL=http://localhost:8080
//}

この二つを用意したら、`docker compose up`コマンドで、コンテナを起動します。コンテナが起動したら、`http://localhost:8080/`にアクセスします。Lightdashが起動していたら、以下の画面が表示されます（@<img>{chapter01/lightdash_first}）。

//image[chapter01/lightdash_first][Lightdashの起動画面]{ 
//}