# YouTube(Chapter14)
HuluやNetflixのような、動画共有サービス全般に適用できるアーキテクチャ

<img width="368" alt="image" src="https://github.com/melonoidz/system_design_note/assets/27326835/78b15053-cb92-4b10-9bd3-426d3ae0311c">

構成はシンプル
- クリエイターがが動画をアップロード
- 視聴者はそれをクリックして視聴する
- 内部はシンプルではない

## 興味深い事実(2020年当時の状況)
- 月ごとのアクティブユーザ数：2ビリオン
- USに住む大人の73%はYouTubeを使う
- 2019年の広告収入は15.1ビリオンだが、これは2018年から36%も増加している
- 全モバイルインターネットトラフィックの37%である
- 80言語で使用されている

とてつもなく巨大なシステム  

## Design Process Framework
### Step1: 課題理解，スコープ定義

- 動画視聴以外にもたくさんの機能がある
  - コメント、共有、イイね、保存、チャンネル登録　など
すべてを面接中に設計するのは不可能なので絞ろう

#### 面接時の会話
候補者「どの機能が大切か？」
面接官「ビデオアップロードと視聴」
候補者「サポートすべきデバイスは？」
面接官「モバイルApp、WEBブラウザ、スマートTV」
候補者「DAUの想定は？」
面接官「5ミリオン」
候補者「ユーザは1日どれくらいYouTubeを見る？」
面接官「30分」
候補者「多言語対応すべき？」
面接官「はい」
候補者「ビデオの拡張子はどれに対応すればよい？」
面接官「ほとんど」
候補者「暗号化は？」
面接官「必要」
候補者「ビデオのサイズは？」
面接官「小～中程度のビデオに絞る。最大1GB」
候補者「AWS,GCP,Azureなど既存のインフラを使用してもよい？」
面接官「**良い質問。**　すべてを1から作るのは現実的ではない。」

----

本システムの設計において、我々は以下だけに集中する。

- ビデオアップロードを高速に
- スムーズなビデオ視聴
- ビデオの画質を変更する
- インフラコストを抑える
- 高可用性、スケーラブル、リライアビリティ
- サポートするデバイス：モバイルApp、WEBブラウザ、SmartTV

#### フェルミ推定
多くの仮置きを含むが、会話のためにこれらは大切

- 5DAU
- ユーザは5動画/day視聴する
- 10%のユーザが1動画/dayアップロードする
- 平均ビデオサイズは300MB
- 総ストレージスペース/day = 5ミリオン*10%*300MB=150TB
- CDNの費用
  - CDNがビデオを提供するとき、CDNからデータが送られる際にコストがかかる
  - CloudFront使用で試算：USから100%とすると：5ミリオン*5ビデオ *0.3GB *0.02$ = 150,000$/day
  - 高額であり、無視できないことがわかる。費用を削減することも検討したい。
 
<img width="369" alt="image" src="https://github.com/melonoidz/system_design_note/assets/27326835/2fdbf06b-e84d-4d60-a164-02db523362f5">


### Step2:基本設計
#### 全て自前にはしない理由
- 設計はフルスクラッチではない．特にインタビューの場面では尚更そう．
  - 時間制限がある中で，正しい技術スタックの概要を提示することは，詳細を詰めるよりも大切
- CDNを1から作ることは高コスト
  - NetflixはAWS, FacebookはAkamaiを使用している

#### 大枠のコンテンツ
3つのコンポーネント
![image](https://github.com/melonoidz/system_design_note/assets/27326835/0cce8d20-e786-4054-9a7d-f84849922af0)

- Client：ユーザはYoutubeをPC，スマホ，スマートTVで視聴する．
- CDN：動画はCDNに蓄積され，適宜配信される．
- API Servers：動画のストーミング以外はAPIのやり取りで実施される．
　- おすすめ紹介，動画のURL，サインアップなど

#### 面接官の興味
実はこの2つだけ
- 動画のアップロード導線
- 動画を視聴するまでの導線

#### 動画のアップロード導線
![image](https://github.com/melonoidz/system_design_note/assets/27326835/5cb50eda-4378-42d4-b2a5-383992502366)

- User：ユーザはYoutubeをPC，スマホ，スマートTVで視聴する．
- Load balancer：処理分散
- API Servers：全ユーザはAPIリクエストベースで進む(ビデオ視聴は除く)
- Metadata DB：ビデオメタデータが貯められる．高可用性やパフォーマンス向上のために用いられる
- Metadata cache：ビデオメタデータやユーザのobjectがキャッシュされる
- Original Storage：BLOB storage system．元動画を蓄積する．
  - オブジェクトストレージで構成された、単一障害点の無い、スケーラブルなストレージです
  - https://www.cloudou.net/storage/blob006/
- Transcoded storage：トランスコードとは、あるデータ形式で圧縮・符号化された動画データなどを、同じ形式で画素数などの仕様が異なるデータに符号化しなおすこと。そのBLOBストレージ
- CDN
- Completion queue：ビデオトランスコードのイベントを貯めるMessage Queue
- Completion handler：Completion queueからイベントデータを持ってきたり，metadata cache/DBを更新するワーカー

#### 動画のアップロード導線A,B
この上で，動画アップロードの導線は次の2つに分割される：
A：生のビデオをアップロードする
![image](https://github.com/melonoidz/system_design_note/assets/27326835/b1d8d66b-3640-4804-b7d4-4b11dfa8daf3)


B：ビデオのmetadataを更新する．これにはURL，size，format，ユーザ情報などが含まれる

![image](https://github.com/melonoidz/system_design_note/assets/27326835/455baa3a-dda7-424e-b59d-0976ee7116a7)


#### 動画を視聴するまでの導線



### Step3:詳細設計
### Step4:まとめ
#### Extra
