# Consistent hashing
水平スケーリングの際に検討が必要な、分散リクエストに対応するためのシステム
## Rehasing problem
N個のキャッシュサーバを持っていた時にロードバランスするための方法
```
serverIndex=hash(key)%N
```
### 具体例
- 4サーバ+8個のstring keyを持っていた場合
- 

<img width="248" alt="image" src="https://github.com/melonoidz/system_design_note/assets/27326835/6d8b5a18-b30e-4b1a-9e14-66a85a0d1654">
<br>
<img width="341" alt="image" src="https://github.com/melonoidz/system_design_note/assets/27326835/db2b3dab-82e4-4bde-b186-260031622c3b">
<br>
<img width="341" alt="image" src="https://github.com/melonoidz/system_design_note/assets/27326835/f4669781-e685-43a0-aa18-b9c30809409c">
<br>
<img width="305" alt="image" src="https://github.com/melonoidz/system_design_note/assets/27326835/4bbd3f77-d272-4eca-aaeb-11b1dd500317">
<br>
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
