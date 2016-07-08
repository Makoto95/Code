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

#define SRATE 44100
#define SSIZE 4000
#define BUFSIZE 10

int main(int argc, char *argv[]){
  const int bufsize = BUFSIZE;
//  alutInit(&argc,argv);
  alGetError();
  ALCdevice* device = alcOpenDevice(NULL);
  ALCcontext* context = alcCreateContext(device, NULL);
  alcMakeContextCurrent(context);

  ALshort data[SSIZE];
  memset(data, 0, sizeof(ALshort)*SSIZE);
  ALuint buffer[BUFSIZE];
  ALuint playsource;
  alGenBuffers(BUFSIZE, buffer);
  alGenSources(1, &playsource);
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
  int loop=20;
  while(loop>0){
    int processed;
    alGetSourcei(playsource, AL_BUFFERS_PROCESSED, &processed);
    if(processed > 0){
      ALuint unqueue_buffer[processed];
      alSourceUnqueueBuffers(playsource, processed, unqueue_buffer);
      alSourceQueueBuffers(playsource, processed, unqueue_buffer);
      loop--;
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
