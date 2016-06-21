#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define FS 44100
#define CHNL 1
#define PI M_PI
int main(int argc, char** argv){
  double A = atof(argv[1]);
  double f = atof(argv[2]);
  int n = atoi(argv[3]);
  short* data = (short*)malloc(sizeof(short)*n);
  int i;
  for(i = 0; i<n; i++){
    double t = (double)i / FS;
    double x = A * sin( 2 * PI * f * t );
    data[i] = (short)x;
  }
  int j = write(1, data, n);
  if(j != n){
    perror("write");
    _exit(1);
  }
  return 0;
}
