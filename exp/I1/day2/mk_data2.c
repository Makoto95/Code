#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>

int main(int argc, char** argv){
  int fd = open(argv[1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
  if(fd == -1){
    perror("open");
    _exit(1);
  }
  unsigned char data[256];
  int i;
  for(i = 0; i<256; i++){
    data[i] = (unsigned char)i;
  }
  int n = write(fd, data, 256);
  if(n != 256){
    perror("write");
    _exit(1);
  }
  close(fd);
  return 0;
}
