# About
- System Design Interviewの例題を学習する
  - システム構成を，単一サーバからスケールアップしていく

# Single Server Setup
- 単純な構成を示す
  - ![image](https://github.com/melonoidz/system_design_note/assets/27326835/f96a577c-c92c-4cc7-8484-32b431ae0108)
  - ![image](https://github.com/melonoidz/system_design_note/assets/27326835/a95fb7c8-1a8e-4df8-be40-38a51d566e43)

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
  - ![image](https://github.com/melonoidz/system_design_note/assets/27326835/8f9ec339-5cfb-4511-84e0-35c829b5d15b)
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
    - ![image](https://github.com/melonoidz/system_design_note/assets/27326835/c9a1374b-8c71-411a-a882-29935c68610e)

# DBレプリケーションの検討
- 上記の仕様ではDBが1つしかないため，FailOverに未対応
　- DBをmaster/slaveに分割することで，これに対応
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
    - ![image](https://github.com/melonoidz/system_design_note/assets/27326835/a70036e2-6b50-45ef-ab3c-fb29e6b3db43)
 
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
    - ![image](https://github.com/melonoidz/system_design_note/assets/27326835/a930350a-fe6f-4d37-8d2b-292cc3d81f96)
- CDN
  -  



- WEBサーバはDB更新をMasterDBへ行う
