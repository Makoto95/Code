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
  int n = atoi(argv[2]);
  short* data = (short*)malloc(sizeof(short)*n*FS*0.3);
  int i=0,j=0,m;
  double f[n];
  m = (n<3) ? n : 3;
  for(i = 0; i< m; i++){
    f[i] = 440 * pow(2, (2*(double)i-9)/12);
  }
  if(n>3){
    m = (n<7) ? n : 7;
    for(i = 3; i < m; i++){
      f[i] = 440 * pow(2,((double)i-5)/6);
    }
  }
  if(n>7){
    for(i = 7; i<n; i++){
      f[i] = f[i-7] * 2;
    }
  }
  for(i = 0; i<n; i++){
    for(j = 0; j<FS*0.3; j++){
      double s = (double)i*FS*0.3 + (double)j;
      double t = s / FS;
      double x = A * sin( 2 * PI * f[i] * t );
      data[(int)s] = (short)x;
    }
  }
  int k = write(1, data, n*FS*0.3*2);
  if(k != n*FS*0.3*2){
    perror("write");
    _exit(1);
  }
  return 0;
}
