# Keystone-JAM

<!-- [English README is here](https://github.com/pyth0n14n/keystone_JAM/blob/main/README.md ) -->

TEEバイパスフォールト攻撃への対策JAMを実装したRISC-V Keystone。

このリポジトリは、RISC-V Keystoneにフォールトインジェクション攻撃対策を適用したものです。
特に、TCHES'22で提案したフォールトによるTEE (Trusted Execution Environment) バイパス攻撃への対策を提供します。
対策のメインは、TCHES'22で提案のジャンプアドレスマスキング (Jump Address Masking: JAM)です。
その他、フォールトによるTEEバイパス攻撃対策として、メモリ暗号化、ランダム遅延、命令冗長化も実装しています。

攻撃と対策の詳細は、[出版と引用](#出版と引用) に記載の論文を参照下さい。

## 提供物
本リポジトリでは、以下を提供します。
ライセンスを分けるため、既存モジュールと新規開発物を分けています。
既存モジュールをscriptが切り貼りしたりパッチを適用したりすることでKeystoneに対策が適用されます。

|フォルダ名|バージョン|説明|
|:--|:--|:--|
|tiny-AES-c| f06ac37f (commit) | メモリ暗号化で使用|
|ascon-c| v1.2.7| メモリ暗号化で使用|
|keystone| v1.0.0| RISC-VベースのTEE実装|
|build| - | メモリダンプを行うサンプルプログラム|
|linux-keystone-driver| - |JAMのための機能拡張|
|sm| - | JAM及びその他の対策の実装 |
|script| - | 既存モジュールとパッチをもとに、keystoneに対策を適用|

![Components](include/components.jpg)

## システム要件

### ハードウェア

KeystoneはQEMUでも動作するため、ハードウェアは必須ではありません。
SiFive Unleashedボードがあれば、実機で動作させることが可能です。

### ソフトウェア

ツールチェイン等はkeystoneにより提供されます。また、scriptはシェルスクリプトとpatchコマンドしか使用しないため、特別なソフトウェアは不要です。
ただし、keystoneを動作させる環境は必要です。

## 利用方法

### 導入

1. リポジトリをクローンする: `$ git clone github.com/pyth0n14n/keystone-JAM`
2. サブモジュールを取得する: `$ git submodule update --init`
3. Keystoneをビルドする (QEMUで動作確認するところまで)
   ![公式ドキュメント](https://docs.keystone-enclave.org/en/latest/Getting-Started/index.html)参照。
<!-- 4. scriptより、必要なファイル -->
TBD

### 動作確認

検証環境
|項目 | 内容 |バージョン| 備考|
|:--|:--|:--|:--|
|OS  | Pop!_OS| 22.04 LTS| Ubuntuベース|


<!-- ![Expected response](image4readme/correct_response.jpg) -->

### 対策の切り替え

## 出版と引用

コードの一部でもお役に立ちましたら、引用いただけると幸甚です。
JAMの説明については、下記の論文もご参照下さい。

```bibtex
@article{nashimoto2022bypassing,
  title={Bypassing Isolated Execution on RISC-V using Side-Channel-Assisted Fault-Injection and Its Countermeasure},
  author={Nashimoto, Shoei and Suzuki, Daisuke and Ueno, Rei and Homma, Naofumi},
  journal={IACR Transactions on Cryptographic Hardware and Embedded Systems},
  pages={28--68},
  year={2022}
}

@inproceedings{nashimoto2024,
  author={Nashimoto, Shoei and Ueno, Rei and Homma, Naofumi},
  title = {Comparative Analysis and Implementation of Jump Address Masking for Preventing TEE Bypassing Fault Attacks},
  year = {2024},
  publisher = {Association for Computing Machinery},
  address = {New York, NY, USA},
  url = {https://doi.org/TBD},
  doi = {TBD},
  booktitle = {Proceedings of the 19th International Conference on Availability, Reliability and Security},
  series = {ARES '24}
}

@online{nashimoto2024keystonejam,
  title={Keystone-JAM: RISC-V Keystone with the countermeasure JAM against TEE bypassing fault attacks},
  author={Nashimoto, Shoei},
  year={2024},
  publisher={GitHub},
  url={https://github.com/pyth0n14n/keystone-JAM},
}
```