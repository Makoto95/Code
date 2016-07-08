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
#define RECSIZE 10000
#define PLAYBUFSIZE 10
#define ON 1
#define OFF 0
#define MAIN 0
#define OTHER 1

int main(){
  ALbyte alrecBuffer[RECSIZE*2];
  ALint alSample;
  ALCdevice* device = alcOpenDevice(NULL);
  ALCcontext* context = alcCreateContext(device, NULL);
  alcMakeContextCurrent(context);
  ALCdevice* alDevice = alcCaptureOpenDevice(NULL, SRATE, AL_FORMAT_MONO16, RECSIZE);
  alcGetError(alDevice);
  alcCaptureStart(alDevice);
  alcGetError(alDevice);
  while (1){
    alcGetIntegerv(alDevice, ALC_CAPTURE_SAMPLES, (ALCsizei)sizeof(ALint), &alSample);
    alcGetError(alDevice);
    if(alSample > 6000) {
      //fprintf(stderr, "%d\n", alSample);
      alcCaptureSamples(alDevice, (ALCvoid *)alrecBuffer, alSample);
      int n;
      if((n = write(fileno(stdout), alrecBuffer, alSample*2) ) != alSample*2){
        perror("write");
        exit(1);
      }
    }
  }
  alcCaptureCloseDevice(alDevice);
  alcCaptureStop(alDevice);
  alcCloseDevice(device);
}