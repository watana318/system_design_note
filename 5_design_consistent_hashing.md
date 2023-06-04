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
- 4 cache keysを円環上にマッピングすると下図：

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
- 

### Two issues in the basic approach

### Virtual nodes

### Find affected keys

## Step4:まとめ
### Extra
