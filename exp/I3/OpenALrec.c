#include <AL/al.h>
#include <AL/alc.h>
//#include <AL/alut.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <locale.h>
#include <string.h>

#define SRATE 8000
#define SSIZE 2000

int main(int argc, char *argv[]){
  short dint = 0;
//  alutInit(&argc,argv);
  ALbyte alBuffer[SRATE/2];
  ALint alSample;
  ALCdevice* device = alcOpenDevice(NULL);
  ALCcontext* context = alcCreateContext(device, NULL);
  alcMakeContextCurrent(context);
  ALCdevice *alDevice = alcCaptureOpenDevice(NULL, SRATE, AL_FORMAT_MONO16, SSIZE);
  if (alGetError() != AL_NO_ERROR){
    fprintf(stderr, "%s\n", "po");
    exit(1);
  }
  alcCaptureStart(alDevice);
  alcGetError(alDevice);
  int i = 500;
  fprintf(stderr, "%ld\n", sizeof(ALbyte));
  while (i!=0){
    alcGetIntegerv(alDevice, ALC_CAPTURE_SAMPLES, (ALCsizei)sizeof(alSample), &alSample);
    alcGetError(alDevice);
    if(alSample > 0) {
      //fprintf(stderr, "%d\n", alSample);
      alcCaptureSamples(alDevice, (ALCvoid *)alBuffer, alSample);
      --i;
      write(1, alBuffer, sizeof(ALbyte)*2*alSample);
    }
  }

  alcCaptureStop(alDevice);
  alcCaptureCloseDevice(alDevice);
  alcCloseDevice(device);
//  alutExit();
  return 0;
}
