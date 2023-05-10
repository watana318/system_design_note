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
 request parameter:{longUrl: longURLString}
 return short URL
```
      - 2:URL redirecting
- URL Redirecting
- URL shortening flows
### Step3:詳細設計
### Step4:まとめ