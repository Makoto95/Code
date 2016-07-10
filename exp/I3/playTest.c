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
#define RECSIZE 10000
#define SSIZE 10000
#define PLAYBUFSIZE 10
#define ON 1
#define OFF 0
#define MAIN 0
#define OTHER 1
#define NUM_BUFFERS 32


void* playFunc(void* param);
void* queueFunc(void* param);

ALuint playsource;

ALenum error;
int callFlag;
int stdinFlag;
pthread_t recThread;
pthread_t recvThread;

int main(){
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
  if(pthread_create(&playThread , &playThread_attr , playFunc , NULL) !=0)
  perror("pthread_create()");
  pthread_attr_destroy(&playThread_attr);


  pthread_attr_init(&queueingThread_attr);
  if(pthread_create(&queueingThread , &queueingThread_attr , queueFunc, NULL) !=0)
  perror("pthread_create()");
  pthread_attr_destroy(&queueingThread_attr);

  pthread_join(playThread, NULL);
  pthread_join(queueingThread, NULL);

  printf("end\n" );
  alDeleteSources(1, &playsource);
  alcMakeContextCurrent(NULL);
  alcDestroyContext(context);
  alcCloseDevice(device);
}

void* playFunc(void* param){
  while(1){
    ALshort data[SSIZE];
    ALint state, num;
    ALenum error;
    ALuint buffer;
    int m;
    if((m = read(fileno(stdin), data, sizeof(ALshort)*SSIZE)) == -1 ){
      perror("read");
      exit(1);
    }
    else{
      fprintf(stderr, "data %d\n", m / sizeof(ALshort));
    }

    alGenBuffers(1, &buffer);
    alGetError();
    alBufferData(buffer, AL_FORMAT_MONO16, data, m, SRATE);
    if ((error = alGetError()) != AL_NO_ERROR){
      // エラー処理
      fprintf(stderr, "%s %x\n", "Error: alBufferData", error);
      exit(1);
    }
    alGetError();
    alSourceQueueBuffers(playsource, 1, &buffer);
    if ((error = alGetError()) != AL_NO_ERROR){
      // エラー処理
      fprintf(stderr, "%s %x\n", "Error: alSourceQueueBuffers", error);
      exit(1);
    }
    alGetSourcei(playsource, AL_SOURCE_STATE, &state);
    if (state != AL_PLAYING) {
      alGetError();
      alSourcePlay(playsource);
      if ((error = alGetError()) != AL_NO_ERROR){
        // エラー処理
        fprintf(stderr, "%s %x\n", "Error: Play", error);
        exit(1);
      }
      double sleepTime = (double)m / sizeof(ALshort) / SRATE;
      fprintf(stderr, "%f\n", sleepTime);
      alutSleep(sleepTime);
    }
  }
}


void* queueFunc(void* param){
  int processed;
  int i;
  while(1){
    ALuint buffer;
    alGetSourcei(playsource, AL_BUFFERS_PROCESSED, &processed);
    fprintf(stderr, "%s %d\n","processed", processed);
    for(i=0; i < processed; i++){
      alSourceUnqueueBuffers(playsource, 1, &buffer);
      alDeleteBuffers(1, &buffer);
    }
    alutSleep(0.3);
  }
}
