# WEB Crawler(Chapter9)
## WEB Crawlerとは？
- 新しくWEBにアップロードされたコンテンツを探すために用いられる
  - WEB page, 画像，動画，PDFなど
- 使用目的は？
  - サーチエンジンのindexing
    - 一番多い使用例 
  - WEB Archiving
    - 将来的に使うかもしれないデータを収集して保存する 
  - WEB Mining
    - 経済分析などに使用される
  - WEB Monitoring
    - 海賊版検知     
![image](https://github.com/melonoidz/system_design_note/assets/27326835/e4a53e5c-bd59-4bfb-baff-f69cc19ade25)

## Framework
### Step1: 課題理解，スコープ定義
- 基本的なアルゴリズムはシンプル
  - 与えられたURLの集合からWEBページを全てダウンロードする
  - WEBページからURLを抽出する
  - 抽出したURLからWEBページを全てダウンロードする
  - 上記を繰り返す
一見単純に見えるが，スケーラブルなクローラーを設計するのは大変

#### フェルミ推定
### Step2:基本設計
### Step3:詳細設計
### Step4:まとめ
#### Extra
