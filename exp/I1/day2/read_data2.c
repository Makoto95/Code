#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <stdlib.h>
#include <fcntl.h>

int main(int argc, char** argv){
  char* filename = argv[1];
  int fd = open(filename, O_RDONLY);
  int n,i=0,j=0;
  if(fd == -1){perror("open"); _exit(1);}
  short* data = (short*)malloc(256);
  while(1){
    n = read(fd, data, 256);
    if(n == -1){perror("read"), _exit(1);}
    if(n == 0) break;
    for(i=0;i<n;i++){
      printf("%d\t%d\n", j++, data[i]);
    }
  }
  close(fd);
  return 0;
}
