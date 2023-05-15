# URL Shorter(Chapter8)
## Framework
### Step1:課題理解，スコープ定義
- 例えば？
  - "q=XXX&C=YYY..." -> "y7kqft"
- どれくらい短くする？
  - できる限り 
- サービスへのアクセス数？
  - 100 million URLs generated/day
- 使用可能文字
  - 0-9, a-z, A-Z
- 短縮したURLは編集可能(削除，更新できる)？
  - 簡単のためナシ
  - 通常のユースケース
    - 1:URL短縮
      - long URL -> return much shorter URL
    - 2:元URLへのアクセス
      - shorter URL -> original URL
    - 3:可用性
      - 高可用性，伸縮性，フォールトトレランスの考慮
#### フェルミ推定
- ★求めたいもの：必要なストレージサイズ
- 書き込み操作の回数
  - 100 million URLs /day
- 1secでは？
  - 100 million/24/3600=1160回
- 読み込み操作の回数
  - 書き：読み=1:10 とすると 1160*10=11600回/sec
- サービスの動作期間におけるレコード数
  - 10年とすると
    - 100 million *365 *10 =36.5 billionレコード　必要
- 平均URL長=100 byteと仮定
- ★よって，必要なストレージサイズは
  - 365 billion * 100bytes * 10years = 365TB 
### Step2:基本設計
- API Endpoints
  - 目的と実装形式
    - client/server間を繋ぐために準備する
    - REST形式
  - 1:URL shortening
```
POST api/v1/data/shorten
- request parameter:{longUrl: longURLString}
- return short URL
```
  - 2:URL redirecting
```
GET api/v1/shortUrl
- return longURL for HTTP redirection
```
- URL Redirecting
 - tinyurl要求を受け取った後，301を返却する
![8-1](https://github.com/melonoidz/system_design_note/assets/27326835/86ab0271-9ea1-4d0b-aae4-4ee815776844)
 - Client/Server間の通信の詳細は下図
![8-2](https://github.com/melonoidz/system_design_note/assets/27326835/6412e858-7074-4ecc-b96d-3d26e0997f0a)
 - ★補足
   - 301 redirect
     - 永続的
   - 302 redirect
     - 一時的
- URL shortening flows
  - "www.tinyurl.com/{hashValue}" となると仮定する
    - hash関数fを定義する必要がある
      - ![8-3](https://github.com/melonoidz/system_design_note/assets/27326835/fe6637cc-6b30-495b-b716-fea474a57eb7)
    - 関数fが満たす要件
      - 各longURLは一意のhashValueになる(=衝突しない)
      - 各hashValueは一意のlongURLに復元できる

### Step3:詳細設計
- Data model
  - hash Tableに全て保存する方法
    - とっかかりとしてはOK
    - RDBに<shortURL, longURL>で持たせるのが良い
![8-4](https://github.com/melonoidz/system_design_note/assets/27326835/ca3ce7b0-ffaa-433a-8274-be47839ba180)
- Hash function
  - long -> shortに変換する機能. hashValueとしても知られている
    - ★：HashValueの長さの設定
      - 文字数：0-9, a-z, A-Zの62文字
      - 62^n >= 365billionとなるnを探す
        - n=7
  - ★：アルゴリズム
    - 2つ
      - Hash + collision resolution
        - 有名アルゴリズム
          - CRC32
          - MD5
          - SHA-1
![t8-2](https://github.com/melonoidz/system_design_note/assets/27326835/37ff9d14-c34d-4c51-a2d9-524fdeb5c91e)
        - ★★：CRC32ですら少し長い．どうする？
          - アプローチ1：最初の7文字だけ集める
            - 懸念点：hashの衝突 
              - DB内の探索で衝突を防ぎたい
![8-5](https://github.com/melonoidz/system_design_note/assets/27326835/8a4ddb59-832c-4c44-8716-1bae4358362e)
                - 全リクエストに対して行うのはコスト高
                - Bloom Filterという手法で回避できる 
      - Base62 conversion
        - 0-0,9-9,10-a,11-b,...61-Zとみなす
        - 11157=2*62^2+55*62^1+59*62^0 = [2,55,59]->[2,T,X]
![8-6](https://github.com/melonoidz/system_design_note/assets/27326835/d6e22e68-7359-45fc-af65-b2b95ebf732d)
        - ".com/2TX" となる
    - 性能比較
![t8-3](https://github.com/melonoidz/system_design_note/assets/27326835/27487ea1-4189-4a1c-99ff-bdefd3425b5d)
      - Base62は次のURLを悟られやすいが，生成されるIDはUnique
#### URL shorteningの詳細設計
- Base62を採用したフロー
![8-7](https://github.com/melonoidz/system_design_note/assets/27326835/c4093bb8-0ccc-4a43-a7cd-5147700991af)
    - 4:uniqueID(=primary key)はUnique ID Generatorで生成する
  - 例：
    - longURL="/tuwejwejwer"
    - Unique ID = 200921592
    - (Base62) = "zn9edcu"
![t8-4](https://github.com/melonoidz/system_design_note/assets/27326835/3e552280-d45f-498f-8374-b496bc0a20a6)
      - UniqueID Generatorはchap7参照
#### URL redirectingの詳細設計
- 読み込み頻度>書き込み頻度の場合，キャッシュがあったほうが効率が良い
![8-8](https://github.com/melonoidz/system_design_note/assets/27326835/4bd78b90-e010-4a42-b468-0dcd873a9894)
### Step4:まとめ
- この章で扱ったこと
  - API design
  - data model
  - hash function
  - URL shortening
  - URL redirecting
#### Extra
- Rate Limiter
  - 使用ユーザが膨大であった場合への対処
  - Chap4
- WEB Server Scaling
  - WEB tierが静的なので，サーバのスケールが容易
- DB Scaling
  - レプリケーション/shardingの検討
- 分析可能性について
  - 誰が使っているか？　などの分析
- 可用性，継続性，伸縮性
  - 巨大システムでは検討必須の項目．
  - Chap1
