# dbtに関するあれこれ（仮）

技術書典向け技術書の原稿リポジトリです。

- **著者**: 岩下煕一・中垣健志
- **発行**: DATUM STUDIO・ちゅらデータ技術書典支部
- **発行予定日**: 2026年4月22日（技術書典）

## 内容

dbtとLightdashを活用したデータ分析基盤の構築について解説します。

## ファイル構成

```
.
├── catalog.yml        # 章構成の定義
├── config.yml         # Re:VIEW ビルド設定
├── preface.re         # まえがき
├── chapter01.re       # 第1章: dbtとLightdash
├── chapter02.re       # 第2章
├── chapter03.re       # 第3章
├── postscript.re      # あとがき
├── images/            # 画像ファイル
├── sty/               # LaTeX スタイルファイル
├── sample/            # サンプルコード
├── Dockerfile         # Re:VIEW ビルド環境
├── docker-compose.yml # Docker Compose 設定
└── Makefile           # ビルドコマンド
```

## ビルド方法

Docker が必要です。

### PDF 生成

```bash
make pdf
```

### EPUB 生成

```bash
make epub
```

### Web（HTML）生成

```bash
make web
```

### テキスト生成

```bash
make text
```

### クリーンアップ

```bash
make clean
```

## 執筆について

原稿は [Re:VIEW](https://reviewml.org/) 形式で記述します。各章の構成メモは `chapter0N_toc.md` ファイルに記載します。

### 原稿生成フロー

```
chapter0N_toc.md  ──(Claude Code)──▶  chapter0N.re  ──(make)──▶  原稿 (PDF/EPUB)
   構成メモ                              Re:VIEW 形式                最終成果物
```

1. **`chapter0N_toc.md`** — 各章の構成・見出し・内容メモを Markdown で記述します
2. **Claude Code** — `*_toc.md` を元に Re:VIEW 形式の `*.re` ファイルを生成・更新します
3. **`chapter0N.re`** — Re:VIEW 形式の原稿ファイルです（直接編集も可）
4. **`make`** — Docker 上の Re:VIEW でビルドし、PDF/EPUB などの最終原稿を生成します

## ライセンス

(C) 2026 DATUM STUDIO・ちゅらデータ技術書典支部
