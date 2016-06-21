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
  uint8_t data[256];
  int32_t k = 0;
  while(1){
    if((m = read(0, data, 256)) == -1){
      perror("read");
      exit(1);
    }
    else{
      //fprintf(stderr, "read success\n" );
    }
    if(m == 0){
      //fprintf(stderr, "read break!\n" );
      break;
    }
    n = send(fd, data, m, 0);
    //fprintf(stderr, "n = %d\n" ,n);
    if(n != m){
      ////fprintf(stderr, "n = %d\n" ,n);
      perror("send");
      exit(1);
    }
    else{
      //fprintf(stderr, "send success\n" );
    }

    //fprintf(stderr, "%d\n" ,m);
    //fprintf(stderr, "k = %d\n" ,k++);
  }
  shutdown(fd, SHUT_WR);

  while(1){
    if((m = recv(fd, data, 256, 0)) == -1){
      perror("recv");
      exit(1);
    }
    if(m == 0) break;
    ////fprintf(stderr, "%d\n" ,m);
    if((n = write(1, data, m)) != m){
      ////fprintf(stderr, "n = %d\n" ,n);
      perror("write");
      exit(1);
    }
  }
  close(fd);
  return 0;
}
