#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/tcp.h>
#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#define N 140

int main(){
  printf("IPアドレスを入力してください\n");
  char ipAddr[16];
  scanf("%s", ipAddr);

  int s = socket( PF_INET, SOCK_STREAM, 0 );
  if( s == -1 ){
    perror("socket");
    exit(1);
  }

  struct sockaddr_in addr;
  addr.sin_family = AF_INET;
  addr.sin_addr.s_addr = inet_addr(ipAddr);
  addr.sin_port = htons(50001);
  int ret = connect(s, (struct sockaddr *)&addr, sizeof(addr));
  if( ret == -1 ){
    perror("connect");
    exit(1);
  }
  else {
    printf("接続しました\n");
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

  return 0;

}
