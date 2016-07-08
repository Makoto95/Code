#include <stdio.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/tcp.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <errno.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char** argv){
  int port = atoi(argv[1]);
  int sock;
  struct sockaddr_in addr;
  struct sockaddr_in senderinfo;
  socklen_t addrlen;
  char buf[1000];
  int m,n;
  int i;
  sock = socket(AF_INET, SOCK_DGRAM, 0);
  addr.sin_family = AF_INET;
  addr.sin_port = htons(port);
  addr.sin_addr.s_addr = INADDR_ANY;
  if(bind(sock, (struct sockaddr *)&addr, sizeof(addr)) == -1){
    perror("bind");
    exit(1);
  }
  else{
    addrlen = sizeof(senderinfo);
    fprintf(stderr,"senderinfo_len = %d, bind success\n", addrlen);
  }

  while(1) {
    fprintf(stderr,"while success\n");
    if(  (m = recvfrom(sock, buf, 1000, 0, (struct sockaddr *)&senderinfo, &addrlen))  == -1){
      perror("recvfrom");
      exit(1);
    }
    else{
      fprintf(stderr,"m =%d, recvfrom success\n", m);
    }
    for(i = 0; i<m; i++){
      if(buf[i] != 0) break;
    }
    fprintf(stderr,"i = %d\n", i);
    if(i == 1000) break;
    n = write(1, buf, strlen(buf));
  }

  inet_ntop(AF_INET, &(senderinfo.sin_addr), buf, 16);
  if(buf == NULL) {
    perror("inet_ntop");
    exit(1);
  }
  fprintf(stderr,"There is a phone call from %s, do you pick up?", buf);
  char s[100], sy[] = "y";
  scanf("%s", s);
  if(strcmp(sy, s) == 0) {
    for(i = 0; i<1000; i++){
      buf[i] = 0;
    }
    if((n = sendto(sock, buf, 1000, 0, (struct sockaddr *)&senderinfo, addrlen)) != m){
      perror("sendto");
      exit(1);
    }
  }
}
