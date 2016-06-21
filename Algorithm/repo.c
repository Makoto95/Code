#include <unistd.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

#define WNUM 10000

char* mmapfile(int*);

int main(){
  int filesize;
  //char fhword[WNUM][20];
  //char lhword[WNUM][20];
  char buf[20];
  char c;
  /*int n;
  int k;
  int l;*/
  int i = 0;
  int pagesize, mmapsize;
  char* addr;
  //char s[] = "qwert e6thywrtw ryjtyje rt6hwryh wet4hw";
  char token[] = " .\t\n";
  filesize = lseek(0, 0, SEEK_END);
  pagesize = getpagesize();
  mmapsize = (filesize + (pagesize-1)) / pagesize * pagesize;
  addr = mmap(0, mmapsize, PROT_READ, MAP_PRIVATE, 0, 0);
  //puts(addr);
  puts(token);
  printf("%c\n", addr[0]);
  while(addr[i] != ' '){
    buf[i] = addr[i++];
  }
  puts(buf);
  /*while(buf != NULL){
    buf = strtok(NULL, token);
    if(buf != NULL) puts(buf);
  }*/
  return 0;
}
