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
  int32_t sendtoPort = atoi(argv[2]);
  int32_t fd;
  if((fd = socket(AF_INET, SOCK_DGRAM, 0)) == -1){
    perror("socket");
    exit(1);
  }
  struct sockaddr_in addr;
  addr.sin_family = AF_INET;
  addr.sin_port = htons(sendtoPort);
  inet_pton(AF_INET, argv[1], &addr.sin_addr.s_addr);

  int32_t m,n,i;
  uint8_t data[1000];
  //int32_t k = 0;
  while(1){
    if((m = read(0, data, 1000)) == -1){
      perror("read");
      exit(1);
    }
    if(m == 0){
      //fprintf(stderr, "read break!\n" );
      break;
    }
    else{
      fprintf(stderr, "m = %d, read success\n", m );
    }
    n = sendto(fd, data, m, 0, (struct sockaddr *)&addr, sizeof(addr));
    //write(1, data, m);
    //fprintf(stderr, "n = %d\n" ,n);
    if(n == -1){
      //fprintf(stderr, "n = %d\n" ,n);
      perror("sendto");
      exit(1);
    }
  }
  for(i = 0; i<1000; i++){
    data[i] = 1;
  }
  n = sendto(fd, data, 1000, 0, (struct sockaddr *)&addr, sizeof(addr) );
  shutdown(fd, SHUT_WR);

  struct sockaddr_in senderinfo;
  socklen_t addrlen = sizeof(senderinfo);

  while(1){
    if((m = recvfrom(fd, data, sizeof(data), 0, (struct sockaddr *)&senderinfo, &addrlen)) == -1 ){
      perror("recv");
      exit(1);
    }
    else{
      fprintf(stderr, "m = %d, recvfrom success\n", m);
    }
    for(i = 0; i<1000; i++){
      if(data[i] != 1) {
        break;
      }
    }
    if(i == 1000) break;
    fprintf(stderr, "not EoD\n");
    //fprintf(stderr, "%d\n" ,m);
    if((n = write(1, data, m)) != m){
      //fprintf(stderr, "n = %d\n" ,n);
      perror("write");
      exit(1);
    }
    else{
      fprintf(stderr, "n = %d, write success\n", m);
    }
  }
  close(fd);
  return 0;
}
