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
  for(i = 0; i < n; i++){
    double t = (double)i / FS;
    if(t==0)
      fprintf(stderr, "t= %f\n", t);
    double x = A * sin( 2 * PI * f * t );
    x = round(x);
    data[i] = (short)x;
    write(1, &data[i], 2);
    if(data[i] == 0)
      fprintf(stderr, "%d, %.0f, %d\n", i, x, data[i]);
  }
  fprintf(stderr, "%d\n", i);
/*
  int j = write(1, data, n*2);
  if(j != n*2){
    perror("write");
    _exit(1);
  }
  */
  return 0;
}
