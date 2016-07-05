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
#include <pthread.h>
#include <string.h>

void* waitMain(void*);
void* startCallPhone(void);
void* udpClient(void*);
void* udpServe(void*);

pthread_t waitThread;

int main(int argc, char *argv[])
{
  pthread_attr_t waitThread_attr;
  pthread_attr_init(&waitThread_attr);
  pthread_attr_setdetachstate(&waitThread_attr , PTHREAD_CREATE_DETACHED);
  if(pthread_create(&waitThread , &waitThread_attr, waitMain, NULL) !=0)
  perror("pthread_create()");
  pthread_attr_destroy(&waitThread_attr);

  pthread_exit(NULL);  //この行を加える
  return EXIT_SUCCESS;
}

void* waitMain(void* param){
  pthread_t udpServThread;
  pthread_attr_t udpServThread_attr;
  pthread_attr_init(&udpServThread_attr);
  pthread_attr_setdetachstate(&udpServThread_attr , PTHREAD_CREATE_DETACHED);
  if(pthread_create(&udpServThread , &udpServThread_attr, udpServe, NULL) !=0)
  perror("pthread_create()");
  pthread_attr_destroy(&udpServThread_attr);

  while(1){
    printf("1. 電話を掛ける\n");
    printf("2. ファイル送信\n");
    printf("3. チャット\n");
    printf("4. 終了\n");
    char c[1];
    scanf("%[1234]", c);
    if(c[0]=='1'){
      pthread_cancel(udpServThread);
      startCallPhone();
    }
    /*    else if(c=='2')
    startSendFile()
    else if(c=='3')
    startChat();
    */
    else if(c[0]=='4'){
      break;
    }
    else printf("incorrect input\n");
  }
  pthread_cancel(udpServThread);
  pthread_exit(NULL);
}

void* startCallPhone(void){
  pthread_t udpClientThread;
  pthread_attr_t udpClientThread_attr;
  pthread_attr_init(&udpClientThread_attr);
  pthread_attr_setdetachstate(&udpClientThread_attr , PTHREAD_CREATE_DETACHED);
  if(pthread_create(&udpClientThread , &udpClientThread_attr, udpClient, NULL) !=0)
  perror("pthread_create()");
  pthread_attr_destroy(&udpClientThread_attr);
  pthread_exit(NULL);
}

void* udpClient(void* param){
  printf("IPアドレスを入力してください\n");
  char ipAddr[16];
  scanf("%s", ipAddr);
  int32_t sendtoPort = 50000;
  int32_t fd;
  if((fd = socket(AF_INET, SOCK_DGRAM, 0)) == -1){
    perror("socket");
    exit(1);
  }
  struct sockaddr_in addr;
  addr.sin_family = AF_INET;
  addr.sin_port = htons(sendtoPort);
  inet_pton(AF_INET, ipAddr, &addr.sin_addr.s_addr);

  int32_t m,n,i;
  uint8_t data[1000];
  //int32_t k = 0;
  for(i = 0; i<1000; i++){
    data[i] = 0;
  }
  n = sendto(fd, data, 1000, 0, (struct sockaddr *)&addr, sizeof(addr) );
  shutdown(fd, SHUT_WR);

  /*recieve*/
  struct sockaddr_in senderinfo;
  socklen_t addrlen = sizeof(senderinfo);

  if((m = recvfrom(fd, data, sizeof(data), 0, (struct sockaddr *)&senderinfo, &addrlen)) == -1 ){
    perror("recv");
    exit(1);
  }
  else{
    fprintf(stderr, "m = %d, recvfrom success\n", m);
  }
  for(i = 0; i<1000; i++){
    if(data[i] != 0) {
      break;
    }
  }
  if(i == 1000){
    fprintf(stderr, "Connect.\n");
  }
  close(fd);
  return 0;
}

void* udpServe(void* param){
  int port = 50000;
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
  pthread_cancel(waitThread);
  fprintf(stderr,"There is a phone call from %s, do you pick up?[y/n]\n", buf);
  char s[1];
  scanf("%[yn]", s);
  if(s[0] =='y') {
    for(i = 0; i<1000; i++){
      buf[i] = 0;
    }
    addrlen = sizeof(senderinfo);
    if((n = sendto(sock, buf, 1000, 0, (struct sockaddr *)&senderinfo, addrlen)) == -1){
      perror("sendto");
      exit(1);
    }
  }
  pthread_exit(NULL);
}
