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
  short data[SRATE];
  ALuint buffer_id;
  ALuint source_id;
  alGenBuffers(1, &buffer_id);
  alGenSources(1, &source_id);
  int n;
  while(1){
    n = read(0, data, SRATE*2);
    if(n==0) break;
    alBufferData (buffer_id, AL_FORMAT_MONO16, &data[0], sizeof(data), SRATE);
    alSourcei(source_id, AL_BUFFER, buffer_id);
    alSourcePlay(source_id);
    sleep(1);
    alSourcei(source_id, AL_BUFFER, 0);
  }
  alDeleteSources(1, &source_id);
  alDeleteBuffers(1, &buffer_id);
  alcMakeContextCurrent(NULL);
  alcDestroyContext(context);
  alcCloseDevice(device);
  alutExit();
}
