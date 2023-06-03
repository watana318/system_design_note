# Rate Limiter
- ネットワークシステムにおいて，クライアントorサービスから送られたトラフィック流量を制御する機能。
  - 特定期間に送られたトラフィックを制限する
  - APIからの呼び出しが閾値を超えたら遮断する
- 例：
  - 2posts/sec以上NGにする
  - 同IPアドレスから10アカウント/day以上を禁止する
  - 同デバイスからのrewardsは5回/週までとする
- メリット
  - Dos攻撃から防御する
    - 有名TechカンパニーのAPIの大半は装備済み
      - Twitter：300tweet / 3hours
      - Google docs read request：300/user/60sec
  - コスト削減
    - サーバ台数を減らせたり, 他のAPIへリソースを捻出できる
  - サーバ高負荷から守る
    - botなどから守る

## Design Process Framework
### Step1: 課題理解，スコープ定義
#### 質疑応答
- クライアントサイド or サーバサイド?
  - サーバサイド API Rate Limiter
- 何を弾くか? IP・userID・other?
  - 臨機応変に
- システムの規模は? 小規模スタートアップ or Big Tech?
  - 巨大
- 分散環境でも動作する?
  - YES
- 独立サービス? or コードの実装の一部(1関数)として?
  - まかせます
- ユーザに制限を通知する?
  - はい
#### 仕様まとめ
- Limit超過を正確に検知する
- 低遅延(Low latency)
  - HTTPの応答時間は遅くしてはいけない
- できる限りメモリ消費を抑える
- Rate Limitingを複数サーバorプロセスにて共有される
- 例外ハンドリング
- リクエストが詰まった時の例外を明確にする
- 高Fault Tolerance
  - オフライン時の挙動?

### Step2:基本設計
#### Rate Limiterの実装箇所
- Clientサイドに実装する場合
  - クライアント側のリクエストは偽装しやすいため信頼性が低い
  - 制御しにくい
  - あまりよくない
- Serverサイドに実装する場合
  - 下図

<img width="340" alt="image" src="https://github.com/melonoidz/system_design_note/assets/27326835/3c93dc15-e510-442b-8d9f-e0117369de69">

- その他の方法
  - ミドルウェアとして作成する

<img width="339" alt="image" src="https://github.com/melonoidz/system_design_note/assets/27326835/2944e686-abf8-49db-af21-a6b89247933f">

##### ミドルウェア配置設計におけるRate Limiterの挙動
- APIが2rec/1secを許可している状態で，Clientが3rec/secをサーバに送信する場合
  - 3つめのリクエストは弾かれる
  - HTTP 429を返す　ユーザが送信した要求が多すぎることを示す

<img width="323" alt="image" src="https://github.com/melonoidz/system_design_note/assets/27326835/98d0e932-2d27-4c92-8d1f-06788fa84628">

#### クラウドサービスの利用検討
- Rate Limiterは通常，APIゲートウェイ内に実装されている
  - SSLターミネーション，認証，IPホワイトリストなどと合わせて実装されている
- Rate Limitierを設計する際に自問すべき内容
  - サーバ側 or ゲートウェイのどこに実装すべきか？
    - 会社の状況による
  - 判断の目安
    - テクノロジースタック
      - プログラミング言語
      - キャッシュサービス
    - ビジネスニーズ
      - サーバ側で完結するなら自由，外部サービスを使うなら制限あるかも
  - 独自実装は時間がかかる
    - 商用APIゲートウェイ使うのも結構あり

#### アルゴリズムの検討
- Rate Limiter実装にはいくつかの方法がある
  - Token Bucket
  - Leaking Bucket
  - Fixed window counter
  - Sliding window log
  - Sliding window counter

##### Token Bucket
- Token Bucket：予め定義された容量を持つコンテナ
  - 容量を超すと溢れる
- Amazon, Stripeが使用している
- Parameter
  - Bucket size
  - Refill rate


<img width="218" alt="image" src="https://github.com/melonoidz/system_design_note/assets/27326835/578634d1-0ee7-4223-81fb-5e0041430320">


### Step3:詳細設計
### Step4:まとめ
#### Extra
