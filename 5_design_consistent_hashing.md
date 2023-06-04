# Consistent Hashing
- 水平スケーリングの際に検討が必要な、分散リクエストに対応するためのシステム.
- Wikipedia
```
ハッシュテーブルのサイズが変更された時、Kをキーの数、nをスロット数とすると、平均K/n個のキーのマッピングの変更のみでハッシュテーブルの機能を提供することのできる特殊なハッシュ法である。
それに対して、その他の多くのハッシュ法では、キーとスロット間のマッピングがモジュラ演算によって定義されているため、ハッシュテーブルのスロット数が変化するとほぼすべてのキーが再マッピングされてしまう。
分散システムの一形態である分散キャッシュなどで利用されている。
```

 
## The Rehashing problem
N個のキャッシュサーバを持っていた時にロードバランスするための一般的な方法
```
serverIndex=hash(key)%N
```
- 例えば，8Key・4サーバの場合は下図

![image](https://github.com/melonoidz/system_design_note/assets/27326835/b4282061-1bc1-42dd-9403-3ad5463f51a7)

図示すると以下

![image](https://github.com/melonoidz/system_design_note/assets/27326835/529b3309-e2ad-44e1-8752-52673ca961e3)

サーバが増減したときにいくつか問題が発生する.
mod4⇒mod3に変化する
![image](https://github.com/melonoidz/system_design_note/assets/27326835/f9900b70-6bd8-4564-a15a-3234de00f236)

![image](https://github.com/melonoidz/system_design_note/assets/27326835/6ecbccb6-66e6-4e96-8d81-969a28e7f25d)

Keyの配置が変わるので，fetch時に異常になる．再配置を試みると，ほぼ全て再配置される．

## Consistent hashing
- Wikipedia：冒頭参照

### Hash space and Hash ring
- Consistent hashingの動作を観察する．f:=SHA-1とする．
X0=0,...Xn=2^160-1とする．
![image](https://github.com/melonoidz/system_design_note/assets/27326835/6c187f7e-7032-48fc-9422-49587778fabd)

これを円環にする.

![image](https://github.com/melonoidz/system_design_note/assets/27326835/0fe39018-bee7-41ac-9ded-40e64e538bf8)

### Hash servers
- 先ほどと同様のHash関数fを考える．
- Hash ring上に4台のサーバを配置すると以下になる：

![image](https://github.com/melonoidz/system_design_note/assets/27326835/54a22def-bac8-4174-a929-4c5fb1f0e5bd)

###  Hash keys
- 上記のHash関数はMOD操作がないので，冒頭のRehashing problemとは異なる．
  - 4つのキャッシュkeysを円環上にマッピングすると下図：

![image](https://github.com/melonoidz/system_design_note/assets/27326835/f323d4a3-b7db-467a-8bfa-d780c6460b7d)

### Server lookup
- Keyに対して時計回りにサーバ探索を行う．
  - Key0はServer0に蓄積される．
  - Key1はServer1に蓄積される．
  - ete

![image](https://github.com/melonoidz/system_design_note/assets/27326835/844650eb-2e59-4a2d-93fa-14ef3d8084a5)

### Add a server
- 新しいサーバが配置されたときの挙動は？
- 例
  - サーバ4が追加されたとき，Key0のみが再配置される．
  - Key1,2,3は同じサーバに居続ける
  - 時計回りに見て最近接なサーバはどれか？

![image](https://github.com/melonoidz/system_design_note/assets/27326835/37c4482a-bf2c-4b36-b4a9-333850b9f715)

### Remove a server
- サーバ除去時の挙動は？
- サーバ1が除去されたとき，Key1は時計回りにおける次の位置「サーバ2」に配置される
  - 残りのKeyはそのまま

![image](https://github.com/melonoidz/system_design_note/assets/27326835/e62d4e16-ae2f-4f8a-91e0-9563cf356a2c)


### Two issues in the basic approach
- ここまでのまとめ 
  - MITにより開発された
  - 基本的なステップ
    - 円環上にあるMAPサーバ・Keyは決められたHash関数を使用する
    - 各Keyがどのサーバに配置されているか？は以下の手順で見つけられる
      - Keyを始点に時計回りに探索し，最初に見つかったサーバ
- 2つの問題がある
  - 円環上のサーバが増減することを考えると，すべての区間を等距離にすることは不可能
    - 区間＝Hash space
    - 区間をとても小さいorとても大きい　は可能

![image](https://github.com/melonoidz/system_design_note/assets/27326835/4682b7fb-a99b-47f9-9f33-c7be8f9a5236)

  - 配置したKeyが偏る可能性がある
    - Server2には多くのKeyがあるが，他サーバにはKeyがない

![image](https://github.com/melonoidz/system_design_note/assets/27326835/03a54693-b9b1-4c54-9c00-f366b13e51f3)

Virtual nodes または replica という技術で解決する.

### Virtual nodes
- サーバを分割し，そこに仮想ノードを整備する．
- 3つに分割した例

![image](https://github.com/melonoidz/system_design_note/assets/27326835/a04b42ff-e086-4497-bc61-01a3638c638f)

Key0は時計回りに探索したとき，s1_1に衝突する．これはサーバ1なので，Key0はサーバ1に格納される．

![image](https://github.com/melonoidz/system_design_note/assets/27326835/9c7ed6b5-3498-4622-bb36-39c34f5c76d1)

- Virtual nodesを設定することで，Keyの分散具合が均等になる．
- トレードオフが存在する
  - Virtual nodeを増やすほうが標準偏差が小さくなり，安定
  - Virtual nodeを増やすにつれSpaceが必要になり，コスト増

### Find affected keys
- サーバ増減によって行われるキーの再配置はどこまで影響するか？
- サーバ4を円環上に追加した例
  - Server4から円環を反時計回り方向に見ると，Server3が最初に見つかる．この区間[S3, S4]が影響範囲である．

![image](https://github.com/melonoidz/system_design_note/assets/27326835/2c5115c0-9c9a-4709-970a-93e707caeb02)

- サーバ1を除去した例
  - 同様に考えると，影響区間は[S0, S1]である．

![image](https://github.com/melonoidz/system_design_note/assets/27326835/673c261a-83d7-4a8b-9f94-59f8bf6bb35a)

## Step4:まとめ
### Consistent Hashingの利点
- サーバ増減に伴うKeyの再配置を最小限にする
- データの再配置が効率的なので，水平スケーリングに適している
- Mitigate hotspot key problem．特定のサーバへの過度なアクセスはサーバのオーバーロードを引き起こす．
  - 例：歌手Mapping，同一ノードにKaty PerryやLady Gagaがいる　など
  - Consistent Hashingによりこれらに耐えられる

### 使用されているサービス
- Amazon Dynamo DB
- Apache Cassandra
- Discord chat APP
- Akamai CDN
- Maglev network load balancer 

### Extra
