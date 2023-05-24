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
#### 質疑応答
要件の理解と明確化を行う

- クローラーの主目的は？
  - サーチエンジンにおけるindexingのため
- 1月あたりどれくらいのWEBページを収集する？
  - 1 billion pages
- コンテンツの種類はどんなのがある？
  - HTMLのみ
- WEBページが追加/編集される場合を考慮する必要がある？
  - はい．どちらも
- WEBからクロールされたHTMLページは貯める必要がある？
  - 最長5年
- 重複しているコンテンツはどう扱う？
  - 無視してかまわない

#### WEBクローラーの設計で大切な観点
- Scalability
  - WEBは巨大なので，並列化などを用いながら効率的にする必要がある
- Robustness
  - BAD HTML，応答がないサーバ，間違っているリンクなど落とし穴がたくさん存在する．これらにも対応できる必要がある
- Politeness
  - 短時間で大量のリクエストを送ってはいけない 
- Extensibility
  - 新しい種類のコンテンツに小規模の変更で対応可能でないといけない．

#### フェルミ推定


### Step2:基本設計
Step1で明確にした要件を基に基本設計をする
![image](https://github.com/melonoidz/system_design_note/assets/27326835/178ca431-16ee-4cba-846c-cd639f386d24)

以下では，各構成要素についてみていく
#### Seed URLs
いわばエントリーポイント．例えば大学のWEBサイトを設定するのが直感的かも．

#### URL Frontier

#### HTML Downloader

#### DNS Resolver

#### Content Parser

#### Content Seen?

#### Content Strage

#### URL Extractor

![image](https://github.com/melonoidz/system_design_note/assets/27326835/62a24fa3-1323-43bb-91e3-fd6c81751e49)

#### URL Filter

#### URL Seen?

#### URL Storage

#### WEB crawler workflow
![image](https://github.com/melonoidz/system_design_note/assets/27326835/c4c97e56-0141-46f4-898d-769969f9d43c)



### Step3:詳細設計
#### 議論する項目
- DFS vs BFS
- URL frontier
- HTML Downloader
- Robustness
- Extensibility
- Detect and avoid problematic content

####  DFS vs BFS

![image](https://github.com/melonoidz/system_design_note/assets/27326835/0da1b583-5e08-4c73-80ba-c3bf47a6bb2f)

#### URL frontier
##### Politeness
![image](https://github.com/melonoidz/system_design_note/assets/27326835/5de74893-cc4b-4f0f-b01e-b540a97f4bb0)
![image](https://github.com/melonoidz/system_design_note/assets/27326835/fcefbf3b-1a4f-4160-9dbd-e212fc192004)

##### Priority
![image](https://github.com/melonoidz/system_design_note/assets/27326835/ad4e590d-928d-437c-910d-725a1cc8c0ff)

![image](https://github.com/melonoidz/system_design_note/assets/27326835/8ace3f0e-28de-4aaf-9478-350bc1161be3)

##### Freshness

##### Storage for URL frontier

#### HTML Downloader

##### Robots.txt
##### Performance optimization
###### 1.Distributed crawl
![image](https://github.com/melonoidz/system_design_note/assets/27326835/f1950c6c-616f-44e8-8c90-8435c8fddf1d)

###### 2.Cache DNS Resolver

###### 3.Locality

###### 4.Short timeout

#### Robustness


#### Extensibility
![image](https://github.com/melonoidz/system_design_note/assets/27326835/2814f63b-1d62-4e4b-9147-3408117e672c)

#### Detect and avoid problematic content

##### 1.Redundant content
##### 2.Spider traps
##### 3.Data noise

### Step4:まとめ
#### Extra
