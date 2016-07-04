#include <AL/al.h>
#include <AL/alc.h>
#include <AL/alut.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <locale.h>
#include <string.h>

#define SRATE 44100
#define SSIZE 1024

int main(int argc, char *argv[]){
  alutInit(&argc,argv);
  alGetError();
  ALCdevice* device = alcOpenDevice(NULL);
  ALCcontext* context = alcCreateContext(device, NULL);
  alcMakeContextCurrent(context);
  ALshort data[SRATE*1];
  memset(data, 0, sizeof(ALshort)*SRATE);
  ALuint buffer_id;
  ALuint source_id;
  alGenBuffers(1, &buffer_id);
  alGenSources(1, &source_id);
  int n;
  while(1){
    n = read(0, data, SRATE*sizeof(ALshort)*1);
    if(n==0) break;
    write(1, data, SRATE*sizeof(ALshort));
    alBufferData(buffer_id, AL_FORMAT_MONO16, data, sizeof(ALshort)*SRATE*1, SRATE);
    alSourcei(source_id, AL_BUFFER, buffer_id);
    alSourcei(source_id, AL_LOOPING, AL_FALSE);
    alSourcePlay(source_id);
    alutSleep(1);
    alSourcei(source_id, AL_BUFFER, 0);
  }
  alDeleteSources(1, &source_id);
  alDeleteBuffers(1, &buffer_id);
  alcMakeContextCurrent(NULL);
  alcDestroyContext(context);
  alcCloseDevice(device);
  alutExit();
  return 0;
}
