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
#include <pthread.h>
#include <wchar.h>
#include <sys/select.h>

#define N 140
#define PORT 50066

void startTcpServThread(void);
void startTcpClientThread(void);
void startScanAndSend(int* s);
void startRecvAndPrint(int* s);
void* tcpServ(void* param);
void* tcpClient(void* param);
void* scanAndSend(void* param);
void* recvAndPrint(void* param);

pthread_t tcpServThread;
pthread_t tcpClientThread;
pthread_t scanAndSendThread;
pthread_t recvAndPrintThread;

int main(int argc, char** argv){
  if(argv[1][0] == 's'){
    startTcpServThread();
    pthread_join(tcpServThread, NULL);
  }
  else if(argv[1][0] == 'c'){
    startTcpClientThread();
    pthread_join(tcpClientThread, NULL);
  }
  return 0;
}

void startTcpServThread(void){
  pthread_attr_t tcpServThread_attr;
  pthread_attr_init(&tcpServThread_attr);
  if(pthread_create(&tcpServThread , &tcpServThread_attr, tcpServ, NULL) !=0)
  perror("pthread_create()");
  pthread_attr_destroy(&tcpServThread_attr);
}

void startTcpClientThread(void){
  pthread_attr_t tcpClientThread_attr;
  pthread_attr_init(&tcpClientThread_attr);
  if(pthread_create(&tcpClientThread , &tcpClientThread_attr, tcpClient, NULL) !=0)
  perror("pthread_create()");
  pthread_attr_destroy(&tcpClientThread_attr);
}

/*void startScanAndSend(int* s){
  pthread_attr_t scanAndSendThread_attr;
  pthread_attr_init(&scanAndSendThread_attr);
  if(pthread_create(&scanAndSendThread , &scanAndSendThread_attr, scanAndSend, (void*)s) !=0)
  perror("pthread_create()");
  pthread_attr_destroy(&scanAndSendThread_attr);
}*/

void startRecvAndPrint(int* s){
  pthread_attr_t recvAndPrintThread_attr;
  pthread_attr_init(&recvAndPrintThread_attr);
  if(pthread_create(&recvAndPrintThread , &recvAndPrintThread_attr, recvAndPrint, (void*)s) !=0)
  perror("pthread_create()");
  pthread_attr_destroy(&recvAndPrintThread_attr);
}

void* tcpServ(void* param){
  int ss = socket(PF_INET, SOCK_STREAM, 0);
  printf("待ち受け中...\n");
  if (ss == -1) {
    perror("socket");
    exit(1);
  }
  struct sockaddr_in client_addr;
  socklen_t len = sizeof(struct sockaddr_in);

  struct sockaddr_in addr;
  addr.sin_family = AF_INET;
  addr.sin_port = htons(PORT);
  addr.sin_addr.s_addr = INADDR_ANY;
  int bi = bind(ss, (struct sockaddr *)&addr, sizeof(addr));
  if (bi == -1)  {
    perror("bind");
    exit(1);
  }
  while(1){
    listen(ss, 10);
    int s = accept(ss, (struct sockaddr *)&client_addr, &len);
    if (s == -1)  {
      perror("accept");
      exit(1);
    }
    if (s >= 0 ) {
      char ipAddr[16];
      inet_ntop(AF_INET, &(client_addr.sin_addr), ipAddr, 16);
      perror("inet_ntop");
      if(ipAddr == NULL) {
        exit(1);
      }
      char c[0];
      printf("%s とチャット要求が来ています. チャットに応答しますか？[y/n]\n", ipAddr);
      scanf("%s", c);
      if(c[0] == 'y'){
//        startScanAndSend(&s);
        startRecvAndPrint(&s);
        pthread_join(scanAndSendThread, NULL);
        break;
      }
    }
    close(s);
  }
  close(ss);
}

void* tcpClient(void* param){
  printf("IPアドレスを入力してください\n");
  char ipAddr[16];
  scanf("%s", ipAddr);

  int s = socket( PF_INET, SOCK_STREAM, 0 );
  if( s == -1 ){
    perror("socket");
    exit(1);
  }
//  fprintf(stderr, "%d\n", s);
  struct sockaddr_in addr;
  addr.sin_family = AF_INET;
  addr.sin_addr.s_addr = inet_addr(ipAddr);
  addr.sin_port = htons(PORT);
  int ret = connect(s, (struct sockaddr *)&addr, sizeof(addr));
  if( ret == -1 ){
    perror("connect");
    exit(1);
  }
  else {
    printf("接続しました\n");
  }
//  startScanAndSend(&s);
  startRecvAndPrint(&s);
  pthread_join(scanAndSendThread, NULL);
  close(s);
}
/*
void* scanAndSend(void* param){
  int* s = (int*)param;
  //  wchar_t* afk = L"/afk";
  char data[N];
  fflush(stdin);
  while(1){
    fprintf(stderr, "scanf wait\n");
    scanf("%s\n", data);
    if (strcmp(data, end)==0) break;
    if (m == -1) {
    int m = send(*s, data, N*sizeof(char), 0);
      perror("send");
      exit(1);
    }
  }
  pthread_cancel(recvAndPrintThread);
}
*/
void* recvAndPrint(void* param){
  printf("/end でチャット終了\n");
  char* end = "/end";
  int* s = (int*)param;
  char data[N];
  fd_set rfds;
  struct timeval tv;
  int retval;
  fflush(stdout);
  FD_ZERO(&rfds);
  FD_SET(*s, &rfds);
  while(1){
    tv.tv_sec = 1;
    tv.tv_usec = 0;
    retval = select(*s, &rfds, NULL, NULL, &tv);
    if (retval == -1)
      perror("select()");
    else if (retval){
      int n = recv(*s, data, N*sizeof(char), 0);
      if( n == -1 ){
        perror("recv");
        exit(1);
      }
      if( n == 0 ) continue;
//      fprintf(stderr, "%s %d\n", "recieve", n);
      printf(">> %s\n", data);
      /* FD_ISSET(0, &rfds) が true になる。*/
    }
    else{
      printf("<< ");
      scanf("%s\n", data);
      if (strcmp(data, end)==0) break;
      int m = send(*s, data, N*sizeof(char), 0);
      if (m == -1){
        perror("send");
        exit(1);
      }
    }
  }
}
