# About
- System Design Interviewの例題を学習する
  - システム構成を，単一サーバからスケールアップしていく

# Single Server Setup
- 単純な構成を示す
![image](https://github.com/melonoidz/system_design_note/assets/27326835/f96a577c-c92c-4cc7-8484-32b431ae0108)
![image](https://github.com/melonoidz/system_design_note/assets/27326835/a95fb7c8-1a8e-4df8-be40-38a51d566e43)

  - DNSにより名前解決し，HTMLまたはJSONによりレンダリングする
## 検討事項1：traffic source
- Web Serverはどこからアクセスされるか？
  - WEB APP
    - サーバサイド(ビジネスロジックやストレージをhandleしているJavaなど) or クライアントサイド(JavaScriptなど)からアクセスされる 
  - Mobile APP
    - JSON formatが多い
  ```
  GET/users/12-Retrieve user object
  {
    "id":12,
    "firstName":"John",
    "lastName":"Smith"
  }
  ```

# DataBaseの追加
- DBを追加しよう
![image](https://github.com/melonoidz/system_design_note/assets/27326835/8f9ec339-5cfb-4511-84e0-35c829b5d15b)
## どのDBを使うか？
- RDB
  - MySQLなど
  - table/rows管理
- No-RDB(NoSQL)
  - DynamoDBなど
  - ★4カテゴリに分かれる
    - key-value stores
    - graph stores
    - column stores
    - document stores
- 基本的には枯れた技術であるRDBが良いが，以下の場合はNoSQLが良い
  - 遅延を抑えたい
  - 構造的なデータではない
  - シリアライズ/デシリアライズ程度でしかデータを扱わない
  - 多量のデータを保存する

# 水平スケーリング/垂直スケーリングの検討
- 垂直スケーリング
  - Scale up
    - CPUを良いものにする
- 水平スケーリング
  - Scale out
    - サーバの数を増やす
- トラフィックが遅いとき
  - 垂直スケーリングが単純で良い
    -　ただし，強化は有限
  -　★冗長性がない
    - 強化作業時にAPPを落としてしまう 　　     

# ロードバランサの追加
- 昔の設計では，WEBサーバは直接つないでいた
  -  Webサーバがオフラインの時、ユーザはアクセス不可
  -  ユーザが増加したとき、通信が遅くなる or サーバが落ちる可能性がある
- ロードバランサの導入
  - Trafficの分散を実現する
  - Private IPの使用でセキュリティ強化
  - サーバ1がオフラインになった時、2に回る
  - 事前に分散させるのでサーバ増加させやすい
![image](https://github.com/melonoidz/system_design_note/assets/27326835/c9a1374b-8c71-411a-a882-29935c68610e)

# DBレプリケーションの検討
- 上記の仕様ではDBが1つしかないため，FailOverに未対応
　- DBをmaster/slaveに分割することで，これに対応
  - WEBサーバはDB更新をMasterDBへ行う
- DBレプリケーション
  - master：書き込みのみ許可
    - データ更新はこちら 
  - slave： masterからコピーされたデータ
    - 読み込みのみ許可
- メリット
　 - パフォーマンスが良い
 　  - Queryを並列で処理しやすい
   - Reliablity
     - 一部分が壊れてもなんとかなる
  - 高可用性
    - 一部オフラインでもOK
    - 他からコピーしてくる
- Replication処理は複雑
![image](https://github.com/melonoidz/system_design_note/assets/27326835/09382595-a59d-4401-b38e-1657de9a858f)

# ここまでのまとめ(Single+LB+DB[replication])
- 仕様
  - LBのIPアドレスはDNSから取得する
  - ユーザはこのIPアドレスでLBに接続する
  - HTTPリクエストはサーバ1or2に向かう
  - WEBはユーザのデータをSlaveDBから読み込む
- 残項目
  - 応答速度の向上 
    - cacheの利用 
![image](https://github.com/melonoidz/system_design_note/assets/27326835/f2d4baa2-c439-49c9-b717-b434e5a6face)

# Cacheの検討
- Cacheは既に参照したものをメモリに蓄積する
  - プログラムの性能は向上する
  - Webサーバはまずキャッシュを探しに行く ない場合にDBアクセス
  - たいていの場合APIで提供されている
  - キャッシュサーバはどこに置くべきか？
    - DBの手前
![image](https://github.com/melonoidz/system_design_note/assets/27326835/a70036e2-6b50-45ef-ab3c-fb29e6b3db43)
 
- Cache使用の検討にあたって
  - データには頻繁にアクセスされるが，たまにしか更新されない場合
  - 大切なデータはstoreにも保存されるべき
    - さもないと：サーバが再起動されたとき，メモリ内の全データが消える
  - Expiration policy(消費期限)
    - 短すぎてもダメ
  - Consistency
    - storeとcacheのデータ一貫性の担保は難しい
  - Mitigating failures
    - 単一キャッシュサーバの場合，SPOF(A single point of failure)の可能性がある
      - ボトルネック
      - 複数キャッシュサーバを置くのが良い 
![image](https://github.com/melonoidz/system_design_note/assets/27326835/a930350a-fe6f-4d37-8d2b-292cc3d81f96)
- CDN
  -  静的コンテンツを運ぶためのネットワーク
    -  WEBコンテンツを効率的かつ高速に配信できるように工夫されたネットワーク
      -  世界中に配置されたキャッシュサーバを利用
    -  動的コンテンツのキャッシュについては範囲外
      -  リクエストパス、クエリstringなどで扱える
  -  高レベルで見たCDNの動作
    -  キャッシュサーバの距離が遠ければ速度が遅くなる
![image](https://github.com/melonoidz/system_design_note/assets/27326835/5f53093e-03b2-41ea-b868-961d689560bc)
![image](https://github.com/melonoidz/system_design_note/assets/27326835/4c330892-d6d2-44a0-b32d-5e989c230510)
 - CDNの動作フロー
  - 1: ユーザがimage.pngをimageURLから取得する
    - URLのdomainはCDN providerから供給されている
  - 2: CDNサーバのキャッシュにimage.pngがないとき、ストレージからoriginを要求する
  - 3: Time-to-live(TTL)付きのHTTPヘッダ付きのimage.pngを返す
    - TODO：どのくらいキャッシュに残すか
  - 4: CDNキャッシュがユーザAに画像を返す
  - 5,6:ユーザＢも同様
- CDN使用に関する検討
  -  コスト: 外部サービスを使うので、重大なメリットがないなら使わなくてもよい
  -  適切なキャッシュ消費期限を設定するべき
  -  CDN fallbackの考慮：システムはCDNの一時的な障害があった場合に対処できるべきだ
  -  Invalidating files：CDNキャッシュが切れる前に、CDNから独自にファイル削除ができること
    -  APIにて提供されている
  -  Object versioningの使用
    -  objectの異なるバージョンを提供するため
    -  URLに付与　v=2　など

# CDN+cache反映後の姿
- 静的アセットはCDNからfetchする
  - WEBサーバからはもらわない
- キャッシュからDBを高速ロード
  - TODO:キャッシュは2箇所ということ？
![image](https://github.com/melonoidz/system_design_note/assets/27326835/68432a62-966b-42ed-a7df-63e48f82ba76)

# Stateless WEB Tier
- 水平スケーリングを検討する
  - ユーザセッションデータなどをWEBから移動させる必要がある
  - 良い方法：RDB or NoSQLなどの持続可能Storeに移すこと
- Stateless web tier
  - クラスタ内のどのWEBサーバもDBから静的データにアクセスできること
- Statefull architecture
  - Stateful serverとStateless serverの違い
    - Statefulは顧客データ(静的)を1リクエストとして保持
    - Statelessは静的データを持たないまま
  - 挙動
    - ユーザAのセッションとプロファイルはサーバ1に保存
    - ユーザAの認証のために、HTTPリクエストはサーバ1を経由する
    - サーバ2など、ほかのサーバからリクエストが送られたとき、認証失敗する
      - サーバ3も同様
  - 問題点
    - 同じサーバを経由する必要がある	
    - バランスが良くない
![image](https://github.com/melonoidz/system_design_note/assets/27326835/f18fffe3-d7de-48db-ab74-ada900897243)
  - HTTPリクエストはどこかのWEBサーバに送られる
    - 静的データは共有Strageから取り出される
    - シンプルかつロバスト
![image](https://github.com/melonoidz/system_design_note/assets/27326835/d6cd5a30-60a6-461a-aeb0-215b3c85386d)

# Stateless Server追加後のWEBシステム構成図
- セッションデータはWEB層から排除
  - 継続的データストアに蓄積
  - RDB, NoSQL
    - NoSQLは可変性が高い
  - 複数データセンタを持つことは重要
    - NoSQL=Stateless Server といえる
![image](https://github.com/melonoidz/system_design_note/assets/27326835/6e0dc731-fcd3-458c-a97e-1347676c0914)

# Data Centerの設置
- 2つのデータセンタを整備すると以下
  - 通常，ユーザは近い地域のデータセンタを使う
    - geoDNS-routed
    - IPアドレスから推定している
![image](https://github.com/melonoidz/system_design_note/assets/27326835/0bf09ed6-f7fa-4347-b437-5655649ab889)

## 片方のData Centerがダメな時
- 100%片方に流れる
![image](https://github.com/melonoidz/system_design_note/assets/27326835/eba04094-ec11-4272-b01a-978b296f55d9)

## 複数Data Centerを設置する際の技術的課題
- トラフィックの方向再設定
  - 正しいデータセンタに繋ぐための効果的なツールが必要
    - GeoDNS
- データSync
  - 異なる地域にいるユーザは異なるDB,cacheを使えるべき
    - そうでない場合，failoverするとダメになってしまう
    - 複数方向でデータレプリカをするのが定石
      - Netflixがやり方を研究していた
- テスト&デプロイ
  - 複数データセンタを立ち上げる場合，異なる地域からAppテストをするべき
- システムのスケールアップのためにはコンポーネントを分離することが大切
  - ★Message Queueがキーストラテジ
  - (分散システム)

# Message Queue
- サーバーレスおよびマイクロサービスアーキテクチャで使われる非同期サービス対サービスの通信形態
  - メッセージは、処理され、削除されるまではキューに格納される 
  - 各メッセージは、単一のコンシューマーによって一度だけ処理される
  - 重い処理の分割、バッファーまたはバッチ処理、およびスパイクの多いワークロードを円滑にすることができる
![image](https://github.com/melonoidz/system_design_note/assets/27326835/c65a1424-20a7-4a17-b2dd-e34387d40cf0)

## 使用例
- Queueのサイズが大きくなればworkerを増やす
  - 小さくなればworkerは減らす
![image](https://github.com/melonoidz/system_design_note/assets/27326835/ca065780-f4f4-41a8-a941-81ad2fef7da8)

# Logging, Metrics, Automation
- スモールシステムの場合，logging, metrics, automationは不必要
  - 大きいシステムの場合は投資すべき！
- Metrics：システムの健康状態を確認できる
  - Host Level Metrics：CPU, Memory, disk I/O
  - Aggregated level metrics：DB層，Cache層の全体的なパフォーマンス
  - Key business metrics：DAU，retention, revenue
- Automation：CIシステムなど
  - システムが大きく複雑になるにつれ，自動ビルドなどは効果を発揮する
  - デプロイなども

# MQ+Loggingなどを追加したシステム構成図
- MQはFailure耐性を向上させている
![image](https://github.com/melonoidz/system_design_note/assets/27326835/3024a903-03f4-4d5e-94e3-52a106b12a56)

# Data Scaling
- Scalingの種類(水平/垂直)
![image](https://github.com/melonoidz/system_design_note/assets/27326835/0d267c25-2df4-4e10-92f8-4e3b16bb57be)
- sharded DB
![image](https://github.com/melonoidz/system_design_note/assets/27326835/c3a1cb0c-f2c6-48a6-aef4-dd50d612fcdf)
![image](https://github.com/melonoidz/system_design_note/assets/27326835/0bd99fe7-ed53-41ea-9082-215252bab102)

# 全て組み合わせたシステム構成図
![image](https://github.com/melonoidz/system_design_note/assets/27326835/aa6f06d9-756a-4afd-9108-7fb08e7ac7e5)
## 大量のユーザに対処するために
- 以下の観点が大切
  - WEB Tierを静的に保つ
  - 全Tierで冗長性を構築する
  - できる限りCasheを用いる
  - 複数データセンタをサポートする
  - CDNにて静的アセットをHostする
  - ShardingによってData TierをScaleさせる
  - システムを監視し，自動化ツールを導入する
