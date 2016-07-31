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
#include <AL/alut.h>
#include <locale.h>
#include <string.h>

#define SRATE 10000
#define SAMPLESIZE 3000
#define ON 1
#define OFF 0
#define MAIN 0
#define OTHER 1


void* playFunc(void*);
void* queueFunc(void*);

ALenum error;
int callFlag;
int stdinFlag;
pthread_t recThread;
pthread_t recvThread;

int main(){
  callFlag = ON;
  ALuint playsource;
  ALCdevice* device = alcOpenDevice(NULL);
  alGetError();
  ALCcontext* context = alcCreateContext(device, NULL);
  alcMakeContextCurrent(context);
  ALuint buffer;
  pthread_t playThread, queueingThread;
  pthread_attr_t playThread_attr, queueingThread_attr;

  alGenSources(1, &playsource);
  if ((error = alGetError()) != AL_NO_ERROR){
    // エラー処理
    fprintf(stderr, "%s %x\n", "Error: alGenSources", error);
    exit(1);
  }

  pthread_attr_init(&playThread_attr);
  if(pthread_create(&playThread , &playThread_attr , playFunc , (void*)&playsource) !=0)
  perror("pthread_create()");
  pthread_attr_destroy(&playThread_attr);


  pthread_attr_init(&queueingThread_attr);
  if(pthread_create(&queueingThread , &queueingThread_attr , queueFunc, (void*)&playsource) !=0)
  perror("pthread_create()");
  pthread_attr_destroy(&queueingThread_attr);

  pthread_join(playThread, NULL);
  pthread_join(queueingThread, NULL);

  alDeleteSources(1, &playsource);
  alcMakeContextCurrent(NULL);
  alcDestroyContext(context);
  alcCloseDevice(device);
//  printf("playTest end\n" );
  return 0;
}

void* playFunc(void* param){
  ALuint* playsource = (ALuint*)param;
  while(1){
    ALshort data[SAMPLESIZE];
    ALint state, num;
    ALenum error;
    ALuint buffer;
    int m;
    if((m = read(fileno(stdin), data, sizeof(ALshort)*SAMPLESIZE)) == -1 ){
      perror("read");
      exit(1);
    }
    int i;
    for(i = 0; i<m; i++){
      if(data[i] != 0) break;
    }
    if(i == m){
//      fprintf(stderr, "%s\n", "data = 0 break");
      break;
    }
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
    alGetError();
    alSourceQueueBuffers(*playsource, 1, &buffer);
    if ((error = alGetError()) != AL_NO_ERROR){
      fprintf(stderr, "%s %x\n", "Error: alSourceQueueBuffers", error);
      exit(1);
    }
    alGetSourcei(*playsource, AL_SOURCE_STATE, &state);
    if (state != AL_PLAYING) {
      alGetError();
      alSourcePlay(*playsource);
      if ((error = alGetError()) != AL_NO_ERROR){
        fprintf(stderr, "%s %x\n", "Error: Play", error);
        exit(1);
      }
    }
    double sleepTime = (double)m / sizeof(ALshort) / SRATE;
    alutSleep(sleepTime-0.1);
  }
  callFlag = OFF;
//  fprintf(stderr, "%s\n", "play close");
}

void* queueFunc(void* param){
  ALuint* playsource = (ALuint*)param;
  int processed;
  int i;
  while(1){
    ALuint buffer;
    alGetSourcei(*playsource, AL_BUFFERS_PROCESSED, &processed);
//    fprintf(stderr, "%s %d\n","processed", processed);
    for(i=0; i < processed; i++){
      alSourceUnqueueBuffers(*playsource, 1, &buffer);
      alDeleteBuffers(1, &buffer);
    }
    if(callFlag==OFF) break;
  }
//  fprintf(stderr, "%s\n", "queue end");
}
