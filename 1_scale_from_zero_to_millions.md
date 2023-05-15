# About
- System Design Interviewの例題を学習する
  - システム構成を，単一サーバからスケールアップしていく

# Single Server Setup
- 単純な構成を示す
  - 1-1
  - 1-2
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

# DataBase
- DBを追加しよう
  - 1-3
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

# 水平スケーリング/垂直スケーリング
- 垂直スケーリング
  - Scale up
    - CPUを良いものにする
- 水平スケーリング
  - Scale out
    - サーバの数を増やす

# ロードバランサ
- Trafficの分散を実現する
  - 1-4

# DBレプリケーション
- 