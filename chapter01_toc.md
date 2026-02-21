（ここに生成した時のコミットハッシュを埋め込む）

# dbtとlightdash

### 他のBIツールとの比較

- 対象BIツール
  - Tableau
  - Looker Studio
  - Redash
- メリットとデメリットという観点でかく

## lightdashの概要

- lightdashの公式サイトから、lightdashの概要に関する記述を見つけて、ここにまとめる

### 動作環境

 - cloud版とセルフホスティング版についてそれぞれ説明を行う
 - 本書ではローカルマシンにDockerを使ってセルフホスティング方式であることを説明する

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
  - minio: Lightdashがクラウドストレージを利用するため、
  - minio-init: minio環境を準備するためのコンテナ

  コラムとして、なぜminioを使うのかを、lightdashとminioの話をインターネットから検索して説明する。

### 起動

- docker-compose.yamlの中で利用する環境変数を、.envとして用意する
- docker compose up -dで各コンテナを起動する
- docker compose psで起動したコンテナを確認する
- ブラウザで、Lightdashの画面を表る（スクリーンショットは、lightdash_first.png）
- docker compose downでコンテナを修了する

