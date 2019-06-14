# IM920Receiver

IM920モジュール受信用ライブラリ

## Usage

### Constructor

* `IM920Receiver(RXピン，TXピン，データサイズ)`

### Methods

* `init()`: 初期化関数，setup内で呼び出す
* `update()`: 情報を受け取り，数値に変換
* `getData(uint8_t ary[])`: データを配列に格納 
* `show()`: 受け取ったデータをシリアルモニタに表示