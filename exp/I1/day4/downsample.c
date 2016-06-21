#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <stdlib.h>
#include <fcntl.h>

int main(int argc, char** argv){
  int m = atoi(argv[1]);
  fprintf(stderr, "%d\n",m);
  int n=0,i=0;
  int j=0,k=0;
  short* data = (short*)malloc(256);
  while(1){
    n = read(0, data, 1000);
    k += n;
    if(n == -1){perror("read"), _exit(1);}
    if(n == 0) break;
    for(i = 0;i < n / 2;i++){
      if(i % m == 0){
        write(1, &data[i], 2);
//        fprintf(stderr, "%d\n",i);
        j++;
      }
    }
  }
  fprintf(stderr, "k=%d,j=%d\n",k,j);
    return 0;
}
