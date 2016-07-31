#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/tcp.h>
#include <netinet/udp.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <string.h>
#define N 140


int main() {

  int ss = socket(PF_INET, SOCK_STREAM, 0);
  if (ss == -1) {
    perror("socket");
    exit(1);
  }

  struct sockaddr_in addr;
  addr.sin_family = AF_INET;
  addr.sin_port = htons(50001);
  addr.sin_addr.s_addr = INADDR_ANY;
  int bi = bind(ss, (struct sockaddr *)&addr, sizeof(addr));
  if (bi == -1)  {
    perror("bind");
    exit(1);
  }

  listen(ss, 10);

  struct sockaddr_in client_addr;
  socklen_t len = sizeof(struct sockaddr_in);
  int s = accept(ss, (struct sockaddr *)&client_addr, &len);
  if (s == -1)  {
    perror("accept");
    exit(1);
  }
  if (s >= 0 ) {
    char ipAddr[16];
    inet_ntop(AF_INET, &(client_addr.sin_addr), ipAddr, 16);
    if(ipAddr == NULL) {
      perror("inet_ntop");
      exit(1);
    }
    printf("%s からチャット要求が来ています\n", ipAddr);
  }

  char data[N];
  char data1[N];
  while(1) {
    scanf("%s", data1);
    if (data1[0] == 'q') break;

    int m = send(s, data1, N, 0);
    if (m == -1)  {
      perror("send");
      exit(1);
    }

    int n = recv(s, data, N, 0);
    if( n == -1 ){
      perror("recv");
      exit(1);
    }
    if( n == 0 ) break;

    printf("%s\n", data);

  }

  close(s);
  close(ss);
}
