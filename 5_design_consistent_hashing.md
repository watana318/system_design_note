# Consistent hashing
水平スケーリングの際に検討が必要な、分散リクエストに対応するためのシステム
## Rehashing problem
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

Keyの配置が変わるので，fetch時に異常になる．

## Consistent hashing
- Wikipedia：

### Hash space and Hash ring
- Consistent hashingの動作を観察する
f:=SHA-1

<img width="357" alt="image" src="https://github.com/melonoidz/system_design_note/assets/27326835/f886107e-7c90-4197-af8e-7f14565e0c74">
<br>
<img width="163" alt="image" src="https://github.com/melonoidz/system_design_note/assets/27326835/3fb53ac7-a00f-45d4-900f-f20615afb376">

#### Hash servers
- 先ほどと同様のHash関数fを考える。Hash ring上に4台のサーバを配置すると以下になる。

<img width="366" alt="image" src="https://github.com/melonoidz/system_design_note/assets/27326835/3f933553-87d3-4c17-bd4c-65b9eb5043b1">

####  Hash keys
- wip

<img width="309" alt="image" src="https://github.com/melonoidz/system_design_note/assets/27326835/b17dc867-a03c-426a-b986-1ae541473d4f">


## Design Process Framework
### Step1: 課題理解，スコープ定義
#### フェルミ推定
### Step2:基本設計
### Step3:詳細設計
### Step4:まとめ
#### Extra
