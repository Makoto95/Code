#include <stdio.h>
#include <stdint.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/tcp.h>
#include <netinet/udp.h>
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

#define N 140
#define SRATE 10000
#define SAMPLESIZE 3000
#define ON 1
#define PORT 50021
#define FILEPORT 50040
#define CHATPORT 50030
#define OFF 0
#define MAIN 0
#define OTHER 1

void startwaitMain(void);
void startUdpServThread(void);
void startTcpServThread(void);
void startCallPhone(void);
void startRecAndSend(void);
void startRecvAndPlay(void);
void* waitMain(void*);
void* udpClient(void*);
void* udpServe(void*);
void* recAndSend(void*);
void* recvAndPlay(void*);
void* playFunc(void*);
void* queueFunc(void* param);
void sendFile(void);
void recvFile(void);

typedef struct threadPtr{
  struct sockaddr_in* senderinfo;
  ALuint* playsource;
}threadPtr_t;

pthread_t waitThread;
pthread_t udpServThread;
pthread_t tcpServThread;
int32_t sock;

int callFlag;
int stdinFlag;
pthread_t recThread;
pthread_t recvThread;
ALCdevice* device;
ALCcontext* context;
struct sockaddr_in senderinfo;
int sockFlag;
int chatFlag;
int32_t Port;
FILE* pp;
FILE* chatpp;

int main(int argc, char *argv[]){
  int Port = PORT;
  startwaitMain();
  startUdpServThread();
  pthread_exit(NULL);
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

void startChatServe(void){
  system("gnome-terminal --tab -e './chat s'");
  chatFlag = ON;
}

void startChatClient(void){
  system("gnome-terminal --tab -e './chat c'");
  chatFlag = ON;
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

void startRecAndSend(void){
  pthread_attr_t recThread_attr;
  pthread_attr_init(&recThread_attr);
  if(pthread_create(&recThread , &recThread_attr, recAndSend,NULL) !=0)
  perror("pthread_create()");
  pthread_attr_destroy(&recThread_attr);
}

void startRecvAndPlay(void){
  pthread_attr_t recvThread_attr;
  pthread_attr_init(&recvThread_attr);
  if(pthread_create(&recvThread , &recvThread_attr, recvAndPlay, NULL) !=0)
  perror("pthread_create()");
  pthread_attr_destroy(&recvThread_attr);
}

void sendFile(void){
  char ipAddr[16];
  char filename[30];
  char command[100] = "nc ";
  memset(ipAddr, '\0', 16);
  memset(filename, '\0', 30);
  printf("IPアドレスを入力してください\n");
  scanf("%s", ipAddr);
  printf("送信ファイル名を入力してください\n");
  scanf("%s", filename);
  int i = 0;
  int j = 3;
  while(ipAddr[i]!='\0'){
    command[j++] = ipAddr[i++];
  }
  command[j++] = ' ';
  command[j++] = '5';
  command[j++] = '0';
  command[j++] = '0';
  command[j++] = '4';
  command[j++] = '0';
  command[j++] = ' ';
  command[j++] = '<';
  i=0;
  while(filename[i]!='\0'){
    command[j++] = filename[i++];
  }
  command[++j] = '\0';
  system(command);
}

void recvFile(void){
  char filename[30];
  char command[100] = "nc -l 50040 >";
  memset(filename, '\0', 30);
  printf("受信ファイルの保存名を入力してください\n");
  scanf("%s", filename);
  int i = 0;
  int j = 13;
  while(filename[i]!='\0'){
    command[j++] = filename[i++];
  }
  command[++j] = '\0';
  system(command);
}

void* waitMain(void* param){
  while(1){
    stdinFlag = MAIN;
    if(callFlag == OFF) printf("1. 電話を掛ける\n");
    printf("2. ファイル送信\n");
    printf("3. ファイル受信\n");
    printf("4. チャット待ち受け（未実装）\n");
     printf("5. チャット接続（未実装）\n");
    if(callFlag == OFF) printf("6. 終了\n");
    if(callFlag == ON) printf("9. 電話を切る\n");
    char c[1];
    scanf("%s", c);
    if(c[0]=='1'){
      pthread_cancel(udpServThread);
      stdinFlag = OTHER;
      startCallPhone();
      break;
    }
    else if(c[0]=='2'){
      sendFile();
    }
    else if(c[0]=='3'){
      recvFile();
    }
    else if(c[0]=='4'){
      startChatServe();
    }
    else if(c[0]=='5'){
      startChatClient();
    }
    else if(c[0]=='6'){
      pthread_cancel(udpServThread);
      break;
    }
    else if(c[0]=='9'){
      callFlag = OFF;
      break;
    }
    else printf("incorrect input %s\n", c);
  }
}


void* udpClient(void* param){
  sockFlag = ON;
  printf("IPアドレスを入力してください\n");
  char ipAddr[16];
  scanf("%s", ipAddr);
  int sendtoPort = PORT;
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
  for(i = 0; i<1000; i++){
    data[i] = 0;
  }
  n = sendto(sock, data, sizeof(data), 0, (struct sockaddr *)&addr, sizeof(addr) );

  /*recieve*/
  socklen_t addrlen = sizeof(senderinfo);

  if((m = recvfrom(sock, data, sizeof(data), 0, (struct sockaddr *)&senderinfo, &addrlen)) == -1 ){
    perror("recv");
    exit(1);
  }
  else{
  }
  for(i = 0; i<1000; i++){
    if(data[i] != 0) {
      break;
    }
  }

  if(i == 1000){
    fprintf(stdout, "Connect.\n");
    stdinFlag = MAIN;
    callFlag = ON;
    startwaitMain();
    startRecAndSend();
    startRecvAndPlay();
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
  sockFlag = OFF;
  //  fprintf(stderr, "%s\n", "Client end");
}

void* udpServe(void* param){
  sockFlag = ON;
  int port = PORT;
  struct sockaddr_in addr;
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
    startRecAndSend();
    startRecvAndPlay();
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
    startwaitMain();
  }
  close(sock);
  sockFlag = OFF;
}

void* recAndSend(void * param){
  ALshort alrecBuffer[SAMPLESIZE];
  ALint alSample;
  ALCdevice* device = alcOpenDevice(NULL);
  ALCcontext* context = alcCreateContext(device,  NULL);
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
      socklen_t addrlen = sizeof(senderinfo);
      int i;
      for(i=0; i<alSample; i++){
        if(abs((int)alrecBuffer[i]) > (0.8*INT16_MAX) ) break;
      }
      if(i==alSample) continue;
      if((n = sendto(sock, alrecBuffer, alSample*2, 0, (struct sockaddr *)&senderinfo, addrlen)) != alSample*2){
        perror("sendto");
        exit(1);
      }
    }
    if(callFlag==OFF) break;
  }
  alcCaptureCloseDevice(alDevice);
  alcCaptureStop(alDevice);
  alcCloseDevice(device);
//  fprintf(stderr, "close recAndSend\n");
}

void* recvAndPlay(void* param){
  pthread_t playThread;
  pthread_attr_t playThread_attr;
  ALshort data[SAMPLESIZE];
  memset(data, 0, SAMPLESIZE*sizeof(ALshort));
  pthread_attr_init(&playThread_attr);
  pthread_attr_setdetachstate(&playThread_attr , PTHREAD_CREATE_DETACHED);
  if(pthread_create(&playThread , &playThread_attr , playFunc , NULL) !=0)
  perror("pthread_create()");
  pthread_attr_destroy(&playThread_attr);
  while(1){
    if(callFlag==OFF){
      pthread_cancel(playThread);
      fwrite(data, sizeof(ALshort), SAMPLESIZE, pp);
      pclose(pp);
      break;
    }
  }
//  fprintf(stderr, "close recvAndPlay\n");
}

void* playFunc(void* param){
  socklen_t addrlen;
  if((pp = popen("./playTest","w")) == NULL){
    perror("popen");
    exit(1);
  }
  ALshort data[SAMPLESIZE];
  while(1){
    memset(data, 0, SAMPLESIZE*sizeof(ALshort));
    int m;
    if((m = recvfrom(sock, data, sizeof(ALshort)*SAMPLESIZE, 0, (struct sockaddr *)&senderinfo, &addrlen)) == -1 ){
      perror("recv");
      exit(1);
    }
    fwrite(data, sizeof(ALshort), m/sizeof(ALshort), pp);
  }
}












/*
void* playFunc(void* param){
//  threadPtr_t* ptrBox = (threadPtr_t*)param;
socklen_t addrlen;
ALuint* playsource = (ALuint*)param;
fprintf(stderr, "%s %d\n", "playsource", *playsource);
ALint state, num;
ALboolean Bool;
ALenum error;
alutInit(0, NULL);
alGetError();
//  const ALuint* playsource = ptrBox->playsource;
//  struct sockaddr_in* senderinfo = ptrBox->senderinfo;
if((Bool = alIsSource(*playsource)) != AL_TRUE){
fprintf(stderr, "%s %x\n", "FALSE: alIsSource playFunc", Bool);
}
alSourcei(*playsource, AL_BUFFER, NULL);
while(1){
if((Bool = alIsSource(*playsource)) != AL_TRUE){
fprintf(stderr, "%s %x\n", "FALSE: alIsSource while_in_playFunc", Bool);
}
ALshort data[SAMPLESIZE];
ALuint buffer;
int m;
if((m = recvfrom(sock, data, sizeof(ALshort)*SAMPLESIZE, 0, (struct sockaddr *)&senderinfo, &addrlen)) == -1 ){
perror("recv");
exit(1);
}
fprintf(stderr, "%s %d\n", "recvdata", m);
alGetError();
alGenBuffers(1, &buffer);
if ((error = alGetError()) != AL_NO_ERROR){
fprintf(stderr, "%s %x\n", "Error: alGenBuffers", error);
exit(1);
}

alGetError();
alBufferData(buffer, AL_FORMAT_MONO16, data, m, SRATE);
if ((error = alGetError()) != AL_NO_ERROR){
fprintf(stderr, "%s %x\n", "Error: alBufferData", error);
exit(1);
}

if((Bool = alIsSource(*playsource)) != AL_TRUE){
fprintf(stderr, "%s %x %d\n", "FALSE: alIsSource front alSourceQueueBuffers", Bool, *playsource);
}
alSourceQueueBuffers(*playsource, 1, &buffer);
if ((error = alGetError()) != AL_NO_ERROR){
fprintf(stderr, "%s %x\n", "Error: alSourceQueueBuffers", error);
Bool = alIsSource(*playsource);
fprintf(stderr, "%s %x %d\n", "Error: alIsSource", Bool, *playsource);
exit(1);
}
alGetSourcei(*playsource, AL_SOURCE_STATE, &state);
if (state != AL_PLAYING) {
alGetError();
alSourcePlay(*playsource);
if ((error = alGetError()) != AL_NO_ERROR){
fprintf(stderr, "%s %x\n", "Error: alSourcePlay", error);
}
double sleepTime = (double)m / sizeof(ALshort) / SRATE;
alutSleep(sleepTime - 0.1);
}
if(callFlag==OFF) break;
}
alutExit();
}

void* queueFunc(void* param){
ALuint* playsource = (ALuint*)param;
alutSleep(1.2);
int processed;
int i;
alutInit(0, NULL);
while(1){
ALuint* unqueueBuffer;
alGetSourcei(*playsource, AL_BUFFERS_PROCESSED, &processed);
fprintf(stderr, "%s %d\n","processed", processed);
for(i=0; i < processed; i++){
alSourceUnqueueBuffers(*playsource, 1, unqueueBuffer);
alDeleteBuffers(1, unqueueBuffer);
}
if(callFlag==OFF) break;
sleep(1);
}
alutExit();
}
*/
