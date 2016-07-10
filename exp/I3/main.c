#include <stdio.h>
#include <stdint.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/tcp.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <errno.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <AL/al.h>
#include <AL/alc.h>
#include <locale.h>
#include <string.h>

#define SRATE 10000
#define SAMPLESIZE 5000
#define ON 1
#define OFF 0
#define MAIN 0
#define OTHER 1

void startwaitMain(void);
void startUdpServThread(void);
void startCallPhone(void);
void startRecAndSend(struct sockaddr_in * senderinfo);
void startRecvAndPlay(struct sockaddr_in * senderinfo);
void* waitMain(void*);
void* udpClient(void*);
void* udpServe(void*);
void* recAndSend(struct sockaddr_in * senderinfo);
void* recvAndPlay(struct sockaddr_in * senderinfo);
void* playFunc(struct sockaddr_in * senderinfo);
void* queueFunc(void* param);


pthread_t waitThread;
pthread_t udpServThread;
int32_t sock;
ALuint playsource;
int callFlag;
int stdinFlag;
pthread_t recThread;
pthread_t recvThread;

int main(int argc, char *argv[]){
  startUdpServThread();
  startwaitMain();
  pthread_exit(NULL);  //この行を加える
  return EXIT_SUCCESS;
}

void startwaitMain(void){
  pthread_attr_t waitThread_attr;
  pthread_attr_init(&waitThread_attr);
  pthread_attr_setdetachstate(&waitThread_attr, PTHREAD_CREATE_DETACHED);
  if(pthread_create(&waitThread , &waitThread_attr, waitMain, NULL) !=0){
    perror("pthread_create()");
    exit(1);
  }
  pthread_attr_destroy(&waitThread_attr);
}

void startUdpServThread(void){
  pthread_attr_t udpServThread_attr;
  pthread_attr_init(&udpServThread_attr);
  pthread_attr_setdetachstate(&udpServThread_attr , PTHREAD_CREATE_DETACHED);
  if(pthread_create(&udpServThread , &udpServThread_attr, udpServe, NULL) !=0)
  perror("pthread_create()");
  pthread_attr_destroy(&udpServThread_attr);
}

void startCallPhone(void){
  stdinFlag = OTHER;
  pthread_t udpClientThread;
  pthread_attr_t udpClientThread_attr;
  pthread_attr_init(&udpClientThread_attr);
  pthread_attr_setdetachstate(&udpClientThread_attr , PTHREAD_CREATE_DETACHED);
  if(pthread_create(&udpClientThread , &udpClientThread_attr, udpClient, NULL) !=0)
  perror("pthread_create()");
  pthread_attr_destroy(&udpClientThread_attr);
  callFlag = ON;
}

void startRecAndSend(struct sockaddr_in * senderinfo){
  pthread_attr_t recThread_attr;
  pthread_attr_init(&recThread_attr);
  if(pthread_create(&recThread , &recThread_attr, recAndSend, senderinfo) !=0)
  perror("pthread_create()");
  pthread_attr_destroy(&recThread_attr);
}

void startRecvAndPlay(struct sockaddr_in * senderinfo){
  pthread_attr_t recvThread_attr;
  pthread_attr_init(&recvThread_attr);
  if(pthread_create(&recvThread , &recvThread_attr, recvAndPlay, senderinfo) !=0)
  perror("pthread_create()");
  pthread_attr_destroy(&recvThread_attr);
}

void* waitMain(void* param){
  while(1){
    stdinFlag = MAIN;
    printf("1. 電話を掛ける\n");
    printf("2. ファイル送信\n");
    printf("3. チャット\n");
    printf("4. 終了\n");
    if(callFlag == ON) printf("9.電話を切る\n");
    char c[1];
    scanf("%s", c);
    if(c[0]=='1'){
      pthread_cancel(udpServThread);
      stdinFlag = OTHER;
      startCallPhone();
      while(1){
        if(stdinFlag==MAIN){
          break;
        }
        sleep(1);
      }
    }
    /*    else if(c=='2')
    startSendFile()
    else if(c=='3')
    startChat();
    */
    else if(c[0]=='4'){
      break;
    }
    if(c[0]=='9'){
      callFlag = OFF;
    }
    else printf("incorrect input\n");
  }
  pthread_cancel(udpServThread);
  return 0;
}

void* udpClient(void* param){
  printf("IPアドレスを入力してください\n");
  char ipAddr[16];
  scanf("%s", ipAddr);
  int32_t sendtoPort = 50000;
  //  int32_t sock;
  if((sock = socket(AF_INET, SOCK_DGRAM, 0)) == -1){
    perror("socket");
    exit(1);
  }
  else{
    printf("Calling!\n");
  }
  struct sockaddr_in addr;
  addr.sin_family = AF_INET;
  addr.sin_port = htons(sendtoPort);
  inet_pton(AF_INET, ipAddr, &addr.sin_addr.s_addr);

  int32_t m,n,i;
  uint8_t data[1000];
  //int32_t k = 0;
  for(i = 0; i<1000; i++){
    data[i] = 0;
  }
  n = sendto(sock, data, sizeof(data), 0, (struct sockaddr *)&addr, sizeof(addr) );

  /*recieve*/
  struct sockaddr_in senderinfo;
  socklen_t addrlen = sizeof(senderinfo);

  if((m = recvfrom(sock, data, sizeof(data), 0, (struct sockaddr *)&senderinfo, &addrlen)) == -1 ){
    perror("recv");
    exit(1);
  }
  else{
//    fprintf(stderr, "m = %d, recvfrom success\n", m);
  }
  for(i = 0; i<1000; i++){
    if(data[i] != 0) {
      break;
    }
  }

  if(i == 1000){
    fprintf(stdout, "Connect.\n");
    startRecAndSend(&senderinfo);
    startRecvAndPlay(&senderinfo);
    stdinFlag = MAIN;
    callFlag = ON;
    startwaitMain();
    pthread_join(recThread, NULL);
    pthread_join(recvThread, NULL);
  }
  else{
    fprintf(stdout, "Not Connect.\n");
    stdinFlag = MAIN;
    callFlag = ON;
    startwaitMain();
  }
  close(sock);
}

void* udpServe(void* param){
  fprintf(stderr, "%s\n", "Serve po");
  int port = 50000;
  struct sockaddr_in addr;
  struct sockaddr_in senderinfo;
  socklen_t addrlen;
  char buf[1000];
  int m,n;
  int i;
  sock = socket(AF_INET, SOCK_DGRAM, 0);
  addr.sin_family = AF_INET;
  addr.sin_port = htons(port);
  addr.sin_addr.s_addr = INADDR_ANY;
  if(bind(sock, (struct sockaddr *)&addr, sizeof(addr)) == -1){
    perror("bind");
    exit(1);
  }
  addrlen = sizeof(senderinfo);

  while(1) {
    if(  (m = recvfrom(sock, buf, 1000, 0, (struct sockaddr *)&senderinfo, &addrlen))  == -1){
      perror("recvfrom");
      exit(1);
    }
    else{
    }
    for(i = 0; i<m; i++){
      if(buf[i] != 0) break;
    }
    if(i == 1000) break;
  }

  inet_ntop(AF_INET, &(senderinfo.sin_addr), buf, 16);
  if(buf == NULL) {
    perror("inet_ntop");
    exit(1);
  }

  pthread_cancel(waitThread);
  stdinFlag = OTHER;
  fprintf(stdout,"There is a phone call from %s, do you pick up?[y/n]\n", buf);
  char s[1];
  scanf("%[yn]", s);
  if(s[0] =='y') {
    for(i = 0; i<1000; i++){
      buf[i] = 0;
    }
    addrlen = sizeof(senderinfo);
    if((n = sendto(sock, buf, 1000, 0, (struct sockaddr *)&senderinfo, addrlen)) == -1){
      perror("sendto");
      exit(1);
    }
    startRecAndSend(&senderinfo);
    startRecvAndPlay(&senderinfo);
    callFlag = ON;
    stdinFlag = MAIN;
    startwaitMain();
    pthread_join(recThread, NULL);
    pthread_join(recvThread, NULL);
  }
  else{
    for(i = 0; i<1000; i++){
      buf[i] = 1;
    }
    addrlen = sizeof(senderinfo);
    if((n = sendto(sock, buf, 1000, 0, (struct sockaddr *)&senderinfo, addrlen)) == -1){
      perror("sendto");
      exit(1);
    }
  }
  close(sock);
}

void* recAndSend(struct sockaddr_in * senderinfo){
  ALbyte alrecBuffer[SAMPLESIZE*sizeof(ALshort)];
  ALint alSample;
  ALCdevice* device = alcOpenDevice(NULL);
  ALCcontext* context = alcCreateContext(device, NULL);
  alcMakeContextCurrent(context);
  ALCdevice* alDevice = alcCaptureOpenDevice(NULL, SRATE, AL_FORMAT_MONO16, SAMPLESIZE);
  alcGetError(alDevice);
  alcCaptureStart(alDevice);
  alcGetError(alDevice);
  while (1){
    alcGetIntegerv(alDevice, ALC_CAPTURE_SAMPLES, (ALCsizei)sizeof(ALint), &alSample);
    alcGetError(alDevice);
    if(alSample > SAMPLESIZE/2) {
      alcCaptureSamples(alDevice, (ALCvoid *)alrecBuffer, alSample);
      int n;
      socklen_t addrlen = sizeof(*senderinfo);
      if((n = sendto(sock, alrecBuffer, alSample*2, 0, (struct sockaddr *)senderinfo, addrlen)) != alSample*2){
        perror("sendto");
        exit(1);
      }
      fprintf(stderr, "%s %d\n", "sendSample", n/2);
    }
    if(callFlag==OFF) break;
  }
  alcCaptureCloseDevice(alDevice);
  alcCaptureStop(alDevice);
  alcCloseDevice(device);
}

void* recvAndPlay(struct sockaddr_in * senderinfo){
  ALCdevice* device = alcOpenDevice(NULL);
  alGetError();
  ALCcontext* context = alcCreateContext(device, NULL);
  alcMakeContextCurrent(context);
  ALenum error;

  pthread_t playThread, queueingThread;
  pthread_attr_t playThread_attr, queueingThread_attr;
  alGetError();
  alGenSources(1, &playsource);
  if ((error = alGetError()) != AL_NO_ERROR){
    fprintf(stderr, "%s %x\n", "Error: alGenSources", error);
    exit(1);
  }

  pthread_attr_init(&playThread_attr);
  if(pthread_create(&playThread , &playThread_attr , playFunc , senderinfo) !=0)
  perror("pthread_create()");
  pthread_attr_destroy(&playThread_attr);

  pthread_attr_init(&queueingThread_attr);
  if(pthread_create(&queueingThread , &queueingThread_attr , queueFunc, NULL) !=0)
  perror("pthread_create()");
  pthread_attr_destroy(&queueingThread_attr);

  pthread_join(playThread, NULL);
  pthread_join(queueingThread, NULL);

  alDeleteSources(1, &playsource);
  alcMakeContextCurrent(NULL);
  alcDestroyContext(context);
  alcCloseDevice(device);
}

void* playFunc(struct sockaddr_in * senderinfo){
  ALint state, num;
  ALboolean Bool;
  ALenum error;
  Bool = alIsSource(playsource);
  fprintf(stderr, "%s %x\n", "FALSE: alIsSource playFunc", Bool);
  while(1){
    ALshort data[SAMPLESIZE];
    ALuint buffer;
    socklen_t addrlen;
    alutInit(0, NULL);
    int m;
    if((m = recvfrom(sock, data, sizeof(ALshort)*SAMPLESIZE, 0, (struct sockaddr *)senderinfo, &addrlen)) == -1 ){
      perror("recv");
      exit(1);
    }
    fprintf(stderr, "%s %d\n", "recvdata", m);
    alGetError();
    alGenBuffers(1, &buffer);
    if ((error = alGetError()) != AL_NO_ERROR){
      fprintf(stderr, "%s %x\n", "Error: alGenBuffers", error);
//      exit(1)
    }
    Bool = alIsBuffer(buffer);
    fprintf(stderr, "%s %x\n", "FALSE: alIsBuffers", Bool);

    alGetError();
    alBufferData(buffer, AL_FORMAT_MONO16, data, m, SRATE);
    if ((error = alGetError()) != AL_NO_ERROR){
      fprintf(stderr, "%s %x\n", "Error: alBufferData", error);
      exit(1);
    }
    if ((Bool = alIsBuffer(buffer)) != AL_TRUE){
      fprintf(stderr, "%s %x\n", "FALSE: alIsBuffers2", Bool);
      exit(1);
    }

    alGetError();
    alSourceQueueBuffers(playsource, 1, &buffer);
    if ((error = alGetError()) != AL_NO_ERROR){
      fprintf(stderr, "%s %x\n", "Error: alSourceQueueBuffers", error);
      Bool = alIsSource(playsource);
      fprintf(stderr, "%s %x\n", "Error: alIsSource", Bool);
      Bool = alIsBuffer(buffer);
      fprintf(stderr, "%s %x\n", "Error: alIsBuffer3", Bool);
      exit(1);
    }
    alGetSourcei(playsource, AL_SOURCE_STATE, &state);
    exit(1);
    if (state != AL_PLAYING) {
      alGetError();
      alSourcePlay(playsource);
      if ((error = alGetError()) != AL_NO_ERROR){
        fprintf(stderr, "%s %x\n", "Error: alSourcePlay", error);
      }
      double sleepTime = (double)m / sizeof(ALshort) / SRATE;
      alutSleep(sleepTime-0.1);
    }
    if(callFlag==OFF) break;
  }
  alutExit();
}

void* queueFunc(void* param){
  alutSleep(0.2);
  int processed;
  int i;
  alutInit(0, NULL);
  while(1){
    ALuint* unqueueBuffer;
    alGetSourcei(playsource, AL_BUFFERS_PROCESSED, &processed);
//    fprintf(stderr, "%s %d\n","processed", processed);
    for(i=0; i < processed; i++){
      alSourceUnqueueBuffers(playsource, 1, unqueueBuffer);
      alDeleteBuffers(1, unqueueBuffer);
    }
    if(callFlag==OFF) break;
    alutSleep(0.1);
  }
  alutExit();
}
