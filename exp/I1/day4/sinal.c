/*
 * sine.c
 * シンプルなサイン波を鳴らす
 * February 14th, 2013
 * Namagi Products
 * gcc sine.c -lOpenAL32 -lalut -s  -o sine
 */
#include <stdio.h>
#include <math.h>
#include <AL/alut.h>

int main(int argc, char *argv[]) {
  alutInit(&argc,argv);
  ALuint buffer, source;
  alGenBuffers(1, &buffer);
  alGenSources(1, &source);
  int i, hz;
  /* サンプリング周波数 */
  int samplingfreq = 44100;
  /* 点間のステップ */
  float step = 2 * M_PI / samplingfreq;
  /* 角速度 */
  float w = 0;
  /* 出力最大 1 */
  float amp = 0.5;
  char str[8];

  for (i = 0; i < 3; i++) {
    /* 周波数の入力 */
    printf("sine wave frequency? 20-10000Hz\n");
    /* 整数入力 */
    fgets(str, 8, stdin);
    /* 数値判定 数字であれば抜ける
       数字でなければ再入力 3回まで行う
       1文字目しか見ていないけど */
    if((isdigit(str[0]) != 0)) {
       hz = atoi(str);
       break;
    }
  }
  /* 周波数の範囲調整 */
  if (hz < 20 ) {
        hz = 20;
  } else if (hz > 10000) {
        hz = 10000;
  }

  ALshort data[samplingfreq / hz];
  /* サイン波1周期作成  16bitなので32767をmaxとする */
  for (i = 0; i < samplingfreq / hz ; ++i) {
     data[i] = amp * 32767 * sin(w * hz);
     w += step;
  }

  /* データ用バッファに入れる */
  alBufferData(buffer, AL_FORMAT_MONO16, data,
         samplingfreq / hz * sizeof(ALshort), samplingfreq);
  /* バッファを再生用バッファに入れる */
  alSourcei(source, AL_BUFFER, buffer);
  /* ループさせる */
  alSourcei(source, AL_LOOPING, AL_TRUE);
  /* 再生 */
  alSourcePlay(source);
  /* 再生中はコマンドプロンプトに表示 */
  printf("Play Sine Wave %dHz\n"
         "Press Enter key to exit.",hz);
  /* enterで終了 */
  getchar();
  alSourceStop(source);
  alDeleteSources(1, &source);
  alDeleteBuffers(1, &buffer);
  return 0;
}
