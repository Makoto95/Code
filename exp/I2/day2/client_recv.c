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

int main(int32_t argc, uint8_t** argv){
  int32_t port = atoi(argv[2]);
  int32_t fd;
  if((fd = socket(PF_INET, SOCK_STREAM, 0)) == -1){
    perror("socket");
    exit(1);
  }
  struct sockaddr_in addr;
  addr.sin_family = AF_INET;
  addr.sin_addr.s_addr = inet_addr(argv[1]);
  addr.sin_port = htons(port);
  int32_t ret;
  if( (ret=connect(fd, (struct sockaddr*)&addr, sizeof(addr))) == -1){
    perror("connect");
    exit(1);
  }

  int32_t m,n;
  uint8_t data[1024];
  while(1){
    m = recv(fd, data, 1024, 0);
    //fprintf(stderr, "%d\n" ,m);
    if((n = write(1, data, m)) != m){
      //fprintf(stderr, "n = %d\n" ,n);
      perror("write");
      exit(1);
    }
    if(m == 0){break;}
  }
  close(fd);
  return 0;
}
