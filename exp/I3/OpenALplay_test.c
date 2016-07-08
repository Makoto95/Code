#include <AL/al.h>
#include <AL/alc.h>
#include <AL/alut.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <locale.h>
#include <string.h>
#include <math.h>
#include <limits.h>
#include <pthread.h>
#include <sys/types.h>

#define SRATE 44100
#define SSIZE 4000
#define BUFSIZE 10

int main(int argc, char *argv[]){
  const int bufsize = BUFSIZE;
//  alutInit(&argc,argv);
  ALCdevice* device = alcOpenDevice(NULL);
  alGetError();
  ALCcontext* context = alcCreateContext(device, NULL);
  alcMakeContextCurrent(context);

  ALshort data[SSIZE];
//  memset(data, 0, sizeof(ALshort)*SSIZE);
  ALuint buffer[BUFSIZE];
  ALuint playsource;
  alGenBuffers(BUFSIZE, buffer);
  pthread_t playThread, queueingThread;
  pthread_attr_t playThread_attr, queueingThread_attr;

  alGenSources(1, &playsource);

  pthread_attr_init(&playThread_attr);
  pthread_attr_setdetachstate(&playThread_attr , PTHREAD_CREATE_DETACHED);
  if(pthread_create(&playThread , &playThread_attr , playFunc , NULL) !=0)
    perror("pthread_create()");
  pthread_attr_destroy(&playThread_attr);

  pthread_attr_init(&queueingThread_attr);
  pthread_attr_setdetachstate(&queueingThread_attr , PTHREAD_CREATE_DETACHED);
  if(pthread_create(&queueingThread , &queueingThread_attr , queueFunc , NULL) !=0)
    perror("pthread_create()");
  pthread_attr_destroy(&queueingThread_attr);

}

void* playFunc(){

}
  int i,j;
  for(i=0; i<BUFSIZE; i++){
    const double key_freq = 220*(i);
    ALshort pcm_data[SRATE];
    for(j=0; j<SRATE; j++){
      pcm_data[j] = sin(key_freq * M_PI * 2.0 * j / SRATE) * SHRT_MAX;
    }
    alBufferData(buffer[i], AL_FORMAT_MONO16, pcm_data, SRATE*sizeof(ALshort), SRATE);
  }


  alSourceQueueBuffers(playsource, (ALsizei)bufsize, buffer);

  alSourcePlay(playsource);
  int loop = 20;
  while(1){
    int processed;
    alGetSourcei(playsource, AL_BUFFERS_PROCESSED, &processed);
    if(processed > 0){
//      ALuint unqueue_buffer[processed];
      alSourceUnqueueBuffers(playsource, processed, buffer);
      alSourceQueueBuffers(playsource, processed, buffer);
      loop--;
      if(loop < 0) break;
    }
  }
/*    n = read(0, data, SRATE*sizeof(ALshort)*1);
    if(n==0) break;
    write(1, data, SRATE*sizeof(ALshort));
  }
  }
  */
  alDeleteSources(1, &playsource);
  alDeleteBuffers(1, buffer);
  alcMakeContextCurrent(NULL);
  alcDestroyContext(context);
  alcCloseDevice(device);
  return 0;
}
