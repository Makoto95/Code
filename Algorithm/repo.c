#include <unistd.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <ctype.h>



#define WNUM 20000
#define TRUE 1
#define FALSE 0

int getWord(char*, int*, int*, int*);
int getHash(char*, int, int);
int wordcmp(char* , char*, int);
void wordcpy(char*, char*, int);

void wordcpy(char* dest, char* src, int n){
  int i;
  for(i=0; i<n; i++){
    dest[i] = tolower(src[i]);
  }
}

int wordcmp(char* s1, char* s2, int n){
  int i;
  for(i = 0; i<n; i++){
    if(s1[i] != tolower(s2[i])) break;
  }
  if(i==n) return 0;
  else return 1;
}





int main(){
  char fhword[WNUM][25];
  memset(fhword, '\0', WNUM * 25);
  char lhword[WNUM][20];
  memset(lhword, '\0', WNUM * 25);
  //unsigned char hashFlag[WNUM];
  int fhWordCounter[WNUM] = {};
  char countFlag[WNUM] = {};
  memset(countFlag, TRUE, WNUM);
  int lhWordCounter[WNUM] = {};
  //memset(hashFlag, NONE, WNUM);
  //char buf[25];
  int wordLen;
  int hash;
  int m, n;
  int i, j;
  //int l;

  char* s = "kane! bouryoku, sex! oppai ippai akne oppai zzzzzzzzzzz\n \t . .. piyo hoge. @ wrgareg frbaerb e! oppai ippai pi";
/*
  int filesize;
  int pagesize, mmapsize;
  char* addr;
  filesize = lseek(0, 0, SEEK_END);
  pagesize = getpagesize();
  mmapsize = (filesize + (pagesize-1)) / pagesize * pagesize;
  addr = mmap(0, mmapsize, PROT_READ, MAP_PRIVATE, 0, 0);
*/
  int now = 0;
  int start = 0;
  int end = 0;
  while(1){
    wordLen = getWord(s, &now, &start, &end);
    if(s[now] == '@' && (wordLen < 1)) break;
    hash = getHash(s, start, end);

    i = hash;
    n = hash+10;
    while(fhWordCounter[i] != 0 && i < n){
      if(wordcmp(fhword[i], &s[start], wordLen) == 0){
        fhWordCounter[i]++;
        break;
      }
      i++;
    }
    if(fhWordCounter[i] == 0){
      wordcpy(fhword[i], &s[start], wordLen);
      fhWordCounter[i] = 1;
    }

    fprintf(stderr, "%s\t%d\n", fhword[i], fhWordCounter[i]);
  }

  now++;

  while(1){
    wordLen = getWord(s, &now, &start, &end);
    if(s[now] == '\0' && (wordLen < 1)) break;
    hash = getHash(s, start, end);

    i = hash;
    n = hash+10;
    while(fhWordCounter[i] != 0 && i < n){
      if(wordcmp(fhword[i], &s[start], wordLen) == 0){
        countFlag[i] = FALSE;
        break;
      }
      i++;
    }
    if(fhWordCounter[i] == 0){
      j = hash;
      m = hash+10;
      while(lhWordCounter[j] != 0 && j < m){
        if(wordcmp(lhword[j], &s[start], wordLen) == 0){
          lhWordCounter[j]++;
          break;
        }
        j++;
      }
      if(lhWordCounter[j] == 0){
        wordcpy(lhword[i], &s[start], wordLen);
        lhWordCounter[i] = 1;
      }
    }

    fprintf(stderr, "%s\t%d\n", lhword[i], lhWordCounter[i]);
  }

  fprintf(stderr, "\nfast half\n");
  for(i=0; i<WNUM; i++){
    if(fhWordCounter[i] != 0 && countFlag[i] == TRUE){
      fprintf(stderr, "%s\t%d\n", fhword[i], fhWordCounter[i]);
    }
  }
  fprintf(stderr, "\nlast half\n");
  for(i=0; i<WNUM; i++){
    if(lhWordCounter[i] != 0){
      fprintf(stderr, "%s\t%d\n", lhword[i], lhWordCounter[i]);
    }
  }


  return 0;
}

int getHash(char* a, int s, int e){
  int hash = 0;
  int i;
  for(i=s; i<=e; i++){
    hash += tolower(a[i]);
  }
  return hash * 10;
}

int getWord(char* addr, int* now, int* start, int* end){
  int i = *now;
  while(!isalnum(addr[i]) && addr[i] != '\0' && addr[i] != '@') i++;
  *start = i;
  while(!ispunct(addr[i]) && !isspace(addr[i]) && addr[i] != '\0' && addr[i] != '@'){
    i++;
  }
  *now = i;
  while(!isalnum(addr[i])){
    i--;
  }
  *end = i;
  return (*end)-(*start)+1;
}