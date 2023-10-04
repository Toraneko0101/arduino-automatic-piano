int pin = 8;
int tempo = 40; // Quarter notes = 40
float tempo_rate = 60.0 / tempo;

//tempoと登録しているキーを押すと、音を再生するジュークボックス

// sharp = lower  ex ) f4 = F#4;
//本来は B4 E5 D#5 E4 F#4としたい
//できれば標準音A4を基準にしたい -> 今の基準はC4
//休符も導入したい
//圧電を二つ以上置けば2つ音を出せる？
const char* musicData[] = {
  "B4_08",
  "E5_08 d5_16 E5_16 f5_04",
  "f5_16 g5_16 g5_16 f5_16 g5_04",
  "g5_16 A5_16 A5_16 g5_16 c6_06 B5_16",
  "A5_16 g5_16 d5_16 E5_16 f5_04",
  "f5_16 g5_16 g5_16 f5_16 E5_04",
  "g5_16 A5_16 f5_16 g5_16 A5_16 B5_16 g5_16 A5_16",
  "c6_08 f5_04 a5_99 g5_16 f5_16",
  "f5_08 a5_99 g5_16 f5_16 B5_04",
  "g5_08 d5_16 E5_16 f5_04",
  "f5_16 g5_16 g5_16 f5_16 g5_04",
  "g5_16 A5_16 A5_16 g5_16 c6_06 B5_16",
  "A5_16 g5_16 d5_16 E5_16 f5_04",
  "f5_16 g5_16 g5_16 f5_16 E5_04",
  "B5_16 c6_16 c6_16 B5_16 A5_16 B5_16 g5_16 A5_16",
  "d6_16 E6_16 E6_16 d6_16 c6_16 d6_16 C6_16 c6_16",
  "E6_16 f6_16 d6_16 E6_16 f6_16 g6_16 E6_16 f6_16",
  "g6_04 g6_16 f6_16 E6_16 c6_16",
  "d6_04 E6_16 d6_16 c6_16 g5_16",
  "B5_04 c6_16 B5_16 A5_16 E5_16",
  "g3_16 B3_16 g3_16 B3_16 f5_16", //変更
  "E5_04"
};

void setup() {
  Serial.begin(9600);
  pinMode(pin, OUTPUT);
}

void loop() {
  delay(300);
  for (int i = 0; i < sizeof(musicData) / sizeof(musicData[0]); i++) {
    play_music(musicData[i]);
  }
  delay(5000); //曲終わりの休息
}

void play_music(const char* music) {
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
  int octave = note[1] - '0';
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
  if (note[3] == '0') {
    duration = note[4] - '0';
  } else {
    duration = (note[3] - '0') * 10 + (note[4] - '0');
  }

  int frequency = calculate_frequency(noteName, octave); //周波数(音階)
  Serial.println(frequency);
  int noteDuration = calculate_duration(duration); //音符の種類
  noteDuration *= tempo_rate; // tempoを反映
  tone(pin, frequency, noteDuration); //再生
  delay(noteDuration); //再生と同じだけ待機
}

int calculate_frequency(char note, int octave) {
  const char* notes = "CcDdEFfGgAaB";
  int noteIndex = strchr(notes, note) - notes; // ノーツのインデックスを計算
  return 261.626 * pow(2.0, (noteIndex + (octave - 4) * 12) / 12.0);
}

int calculate_duration(int duration) {
  switch (duration) {
    case 4: return 1000 ; // 4分音符
    case 6: return 1000 / 1.5; //付点8分音符
    case 8: return 1000 / 2; // 8分音符
    case 12: return 1000 / 3; //付点16分音符
    case 16: return 1000 / 4; // 16分音符
    case 32: return 1000 / 8; //32音符
    case 99: return 1000 / 32; //装飾音符（ひっかける）
    case 1: return 1000 * 4;    // 全音符
    // 他の音符の長さに対する処理を追加
    default: return 0; // エラー値
  }
}