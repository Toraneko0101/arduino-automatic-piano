## 個人的なメモ

__pinMode__
```text

pinMode(pin, mode)
    pin : ピン番号
    mode: Input or Output
        Input   : センサの状態を検知
        Output  : LEDのような駆動装置を駆動可能

```
__digitalWrite__
```text
digitalWrite(pin, val)
    pin : ピン番号
    val : HIGH or LOW
        HIGH    : ピンの電圧を5V(3.3V)に
        LOW     : ピンの電圧を0Vに
```
__digitalRead__
```text
digitalRead(pin)
    pin : ピンがHIGHかLOWか読み取る
```
__analogWrite__
```text
analogWrite(pin, brightness)
    pin : ピン番号
    brightness : 明るさ(0~255)
    メモ : 
           ・[~]がついていないピンには設定できない
           ・パルス幅変調を利用している
           ・繰り返しパターンでHIGHとLOWを交互に駆動している。
           ・LEDが点灯する時間と消灯する時間の比率で人間に明るさを認識させている
           ・点灯する時間が長ければ明るく見える⇒平均電圧が高くなるので
           ・デューティー比 = パルス幅(HIGHの時間)/周期
           ・パルス幅はオシロスコープなどで確認可能
           ・ループ文などで徐々に変化させられるはず
    使用シーン :
            ・LEDの色
            ・モータの速度
            ・音符の作成
```

## その他
__GND__
```text
プルダウン抵抗やプルアップ抵抗が必要な理由:
    ・スイッチが押されていないときの状態を制御するため　-> 閉回路にしたいってこと？
    ・電圧が定まっていないと（浮いていると）、外部の静電気などでボタンが押されたと判断される可能性がある
    ・pinModeの第二引数にINPUT_PULLUPと設定すれば、内部プルアップが有効化される
        >ただし、スイッチの状態が逆になるので注意（スイッチを押していない状態がOFFで入力端子がHIGH）
        >ジャンパーなどを省ける
    ・各ピンとGNDは内部で100MΩでつながっている
```
__ダメな回路図__
- スイッチから抵抗までの間が、完全に浮いているので
![](./images/circuit1.svg)

__内部プルアップ抵抗__
- 電源の陽極とピンをつなぎ、デフォルトを5Vにしている
![](./images/circuit2.svg)