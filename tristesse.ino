/*tempoを設定して、曲を再生するジュークボックス*/
int pin = 8;
int tempo = 40; // Quarter notes = 40
float tempo_rate = 60.0 / tempo;
bool isPlaying = false; // 曲の再生状態を保持



// sharp = lower  ex ) f4 = F#4;
//本来は B4 E5 D#5 E4 F#4としたい
const char* musicData[] = {
  "B4_08",
  "E5_08 D#5_16 E5_16 F#5_04",
  "F#5_16 G#5_16 G#5_16 F#5_16 G#5_04",
  "G#5_16 A5_16 A5_16 G#5_16 C#6_06 B5_16",
  "A5_16 G#5_16 D#5_16 E5_16 F#5_04",
  "F#5_16 G#5_16 G#5_16 F#5_16 E5_04",
  "G#5_16 A5_16 F#5_16 G#5_16 A5_16 B5_16 G#5_16 A5_16",
  "C#6_08 F#5_04 A#5_99 G#5_16 F#5_16",
  "F#5_08 A#5_99 G#5_16 F#5_16 B5_04",
  "G#5_08 D#5_16 E5_16 F#5_04",
  "F#5_16 G#5_16 G#5_16 F#5_16 G#5_04",
  "G#5_16 A5_16 A5_16 G#5_16 C#6_06 B5_16",
  "A5_16 G#5_16 D#5_16 E5_16 F#5_04",
  "F#5_16 G#5_16 G#5_16 F#5_16 E5_04",
  "B5_16 C#6_16 C#6_16 B5_16 A5_16 B5_16 G#5_16 A5_16",
  "D#6_16 E6_16 E6_16 D#6_16 C#6_16 D#6_16 C6_16 C#6_16",
  "E6_16 F#6_16 D#6_16 E6_16 F#6_16 G#6_16 E6_16 F#6_16",
  "G#6_04 G#6_16 F#6_16 E6_16 C#6_16",
  "D#6_04 E6_16 D#6_16 C#6_16 G#5_16",
  "B5_04 C#6_16 B5_16 A5_16 E5_16",
  "G#3_16 B3_16 G#3_16 B3_16 F#5_04", //変更
  "E5_04"
};

void setup() {
  Serial.begin(9600);
  pinMode(pin, OUTPUT);
}

void loop(){
  // シリアル通信からキーを受信
  while (Serial.available()) {
    char c = Serial.read();
    if (c == 'P') {
      // 'P'キーを受信した場合、曲の再生を開始
      isPlaying = true;
      play_music(); // 曲を再生
    } 
  }
}

void play_music() {
  delay(300); //曲初めの間
  for (int i = 0; i < sizeof(musicData) / sizeof(musicData[0]); i++) {
    //強制終了 & 一時停止
    if(Serial.available()){
      char c = Serial.read();
      if (c == 'S'){
        noTone(pin);
        isPlaying = false;
        return;
      }else if(c == 'W'){
        noTone(pin);
        isPlaying = false;
      }
    }
    //演奏可能か?(Wで演奏再開。Sで強制終了)
    if(!isPlaying){
      while(!isPlaying){
        if(Serial.available()){
          char c = Serial.read();
          if (c == 'W'){
            isPlaying = true;
          }else if(c == 'S'){
            return;
          }
        }
      }
    }
    //演奏
    play_music_data(musicData[i]);
    
    
  }
  delay(300); //曲終わりの休息
}

void play_music_data(const char* music) {
  // 音符をコピーして使用する(元配列を壊したくないので)
  char musicCopy[strlen(music) + 1];
  strcpy(musicCopy, music);

  char* token = strtok(musicCopy, " "); // 空白でトークンを分割
  while (token != NULL) {
    play_note(token);
    token = strtok(NULL, " ");
  }
}

void play_note(const char* note) {

  char noteName = note[0];
  int octave;
  int sharp = 0;
  if(note[1] == '#'){
    noteName = note[0] + 32;
    sharp = 1;
  }
  int duration;
  //休符
  if(noteName == 'R'){
    duration = note[1] - '0';
    int noteDuration = calculate_duration(duration); //音符の種類
    noteDuration *= tempo_rate; // tempoを反映
    noTone(pin);
    delay(noteDuration);
    return;
  }
  //音符
  if(note[3 + sharp] == '0'){
    duration = note[4 + sharp] - '0';
  }else{
    duration = (note[3 + sharp] - '0') * 10 + (note[4+sharp] - '0');
  }
  octave = note[1 + sharp] - '0';

  int frequency = calculate_frequency(noteName, octave); //周波数(音階)
  //Serial.println(frequency);
  int noteDuration = calculate_duration(duration); //音符の長さ
  noteDuration *= tempo_rate; // tempoを反映
  tone(pin, frequency, noteDuration); //再生
  delay(noteDuration); //再生と同じだけ待機
}

int calculate_frequency(char note, int octave) {
  const char* notes = "CcDdEFfGgAaB";
  int noteIndex = strchr(notes, note) - notes; // ノーツのインデックスを計算
  return 261.626 * pow(2.0, (noteIndex + (octave - 4) * 12) / 12.0); //基準C4
}

int calculate_duration(int duration) {
  switch (duration) {
    case 1: return 1000 * 4; //全音符
    case 15: return 1000 * 3; //付点2分音符(15であることに注意)
    case 2: return 1000 * 2; //2分音符
    case 3: return 1000 * 1.5; //付点4分音符
    case 4: return 1000 ; // 4分音符
    case 6: return 1000 / 1.5; //付点8分音符
    case 8: return 1000 / 2; // 8分音符
    case 12: return 1000 / 3; //付点16分音符
    case 16: return 1000 / 4; // 16分音符
    case 24: return 1000 / 6; //付点32分音符
    case 32: return 1000 / 8; //32音符
    case 99: return 1000 / 32; //装飾音符（ひっかける）
    // 他の音符の長さに対する処理を追加
    default: return 0; // エラー値
  }
}