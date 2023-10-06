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
    return : int(HIGH or LOW)
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
LEDの色
```c++
void setup()
{
  pinMode(11, OUTPUT);
  pinMode(10, OUTPUT);
  pinMode(9, OUTPUT);
}

void loop()
{
  analogWrite(11, 0);
  analogWrite(10, 0);
  analogWrite(9, 0);
  delay(1000); // Wait for 1000 millisecond(s)
  for (int i=0; i<=255; i+=50){
    for (int j=0; j<=255; j+=50){
        for (int k=0; k<=255; k+=50){
            analogWrite(11, i);
            analogWrite(10, j);
            analogWrite(9, k);
            delay(100);
        } 
    }
  }
  delay(1000); // Wait for 1000 millisecond(s)
}
```

__analogRead__
```text
analogRead(pin)
  pin: アナログ入力ピンの番号 ex) A0
  return: int (Unoの場合、0 ~ 1023)

```

__Serial.begin__
```text
Serial.begin(spped)
  speed: シリアル通信のデータ転送レート(bps)
```

__Serial.println__
```text
Serial.print() //改行無し
Serial.println() //改行あり
  シリアルモニタに出力を送信
```

__map__
```text
map(sensorVal, from_min, from_max, to_min, to_max)
  sensorVal: センサで読み取られる値
  from_min : 予測される最小値
  from_max : 予測される最大値
  to_min : 希望する最小値
  to_max : 希望する最大値
  例) map(20, 0, 1023, 0, 255) 0~1023までの値を、0~255までの値にスケールダウンする
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
- スイッチが入れば、電流はGNDの方に流れる
  - ピンはGNDより電位が高いので 
![](./images/circuit2.svg)

__ポテンショメーター__
```text
・回転角を計測し、電圧に変換する
・アナログ入力を利用
```

__フォトレジスタ__
```text
・光の強さで、抵抗値を変化させる
・光が当たると抵抗値が下がる
・抵抗値を電圧に変換し、analogReadで読み取る
・アナログ入力を利用
```

__PIR(Passive InfraRed)__
```text
・物体から放出される遠赤外線の変化を検出する
・デフォルトはLOWで、光レベルが変化すると一定時間HIGHになる
```

__温度センサ(TMP36)__
```text
出力電圧が温度（摂氏）に比例する
```