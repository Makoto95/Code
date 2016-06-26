#include <unistd.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <ctype.h>
#include <time.h>

#define WNUM 83000
#define TRUE 1
#define FALSE 0
#define LISTLEN 1000
#define WSIZE 35
#define ALSIZE 100
#define ALISTLEN 200

int getWord(char*, int*, int*, int*);
int getHash(char*, int, int);
int wordcmp(char* , char*, int);
void wordcpy(char*, char*, int);
void q_sort(int[], int[], int, int);

int main(){
  clock_t passage;
  char fhword[WNUM][WSIZE];
  memset(fhword, '\0', WNUM * WSIZE);
  char lhword[WNUM][WSIZE];
  memset(lhword, '\0', WNUM * WSIZE);
  int fhWordCounter[WNUM] = {};
  char countFlag[WNUM] = {};
  memset(countFlag, TRUE, WNUM);
  int lhWordCounter[WNUM] = {};
  int wordLen;
  int hash;
  int m, n;
  int i, j;

/*----file mapping----*/
  int filesize;
  int pagesize, mmapsize;
  char* addr;
  filesize = lseek(0, 0, SEEK_END);
  pagesize = getpagesize();
  mmapsize = (filesize + (pagesize-1)) / pagesize * pagesize;
  addr = mmap(0, mmapsize, PROT_READ, MAP_PRIVATE, 0, 0);
  /*----file mapping end----*/

  char* s = addr;
  //char* s = "thine kane! bouryoku, sex! oppai thee thou Oppai ippai akne oppai zzzzzzzzzzz kurose kerfaer fwe dwe\n \t . ..ergqr gfr3 - w34ef'nrr don't ipyo piyo hoge. @ wrgareg frbaerb e! Ippai pi TANGO Holmes Watson wewre e3rwef wdfwr oppai";

  int now = 0;
  int start = 0;
  int end = 0;

  int addListHashFh[ALSIZE] = {};
  char addListFh[ALSIZE][ALISTLEN][WSIZE];
  memset(addListFh, '\0', ALSIZE * ALISTLEN * WSIZE);
  int addListWordCounterFh[ALSIZE][ALISTLEN] = {};
//  int addListHashLh[ALSIZE] = {};

  int k = 0;
  int segfauCount = 0;

  while(1){
    wordLen = getWord(s, &now, &start, &end);
    if(s[now] == '@' && (wordLen < 1)) break;
    hash = getHash(s, start, end);
    //fprintf(stdout, "hash = %d\n", hash);
    i = hash;
    n = hash+LISTLEN;
    while(fhWordCounter[i] != 0 && i < n){
      if(wordcmp(fhword[i], &s[start], wordLen) == 0){
        fhWordCounter[i] += 1;
        //fprintf(stdout, "add [%d]\n", i);
        break;
      }
      i++;
    }

    if(i == n){
      int l;
      int p;
      for(l=0; l < ALSIZE; l++){
        if(addListHashFh[l] == hash){
          p = 0;
          while(p < ALISTLEN && addListWordCounterFh[l][p] != 0){
            if(wordcmp(addListFh[l][p], &s[start], wordLen) == 0){
              addListWordCounterFh[l][p]++;
              break;
            }
            p++;
          }
          if(p == ALISTLEN){
            //fprintf(stdout, "segfau p\n");
            segfauCount++;
          }
          if(addListWordCounterFh[l][p] == 0){
            wordcpy(addListFh[l][p], &s[start], wordLen);
            addListWordCounterFh[l][p]++;
          }
          break;
        }
      }
      if(l==ALSIZE){
        addListHashFh[k] = hash;
        wordcpy(addListFh[l][p], &s[start], wordLen);
        k++;
      }
    }



    if(fhWordCounter[i] == 0){
      wordcpy(fhword[i], &s[start], wordLen);
      fhWordCounter[i] = 1;
      //fprintf(stdout, "store [%d]\n", i);
    }
    //fprintf(stderr, "%s\t%d\n", fhword[i], fhWordCounter[i]);
  }

  fprintf(stderr, "@\n");

  now++;

  fprintf(stdout, "kFh = %d\n", k);
  fprintf(stdout, "segfauP = %d\n", segfauCount);
/*
  int l;
  for(l=0; l < k; l++){
    fprintf(stdout, "%d\n", addListHashFh[l]);
  }
*/
  k = 0;

  while(1){
    wordLen = getWord(s, &now, &start, &end);
    if(s[now] == '\0' && (wordLen < 1)) break;
    hash = getHash(s, start, end);

    //fprintf(stderr, "hash = %d\n", hash);


    i = hash;
    n = hash+LISTLEN;

    while(fhWordCounter[i] != 0 && i < n){
      if(wordcmp(fhword[i], &s[start], wordLen) == 0){
        countFlag[i] = FALSE;
        fprintf(stderr, "%s, countFlag[%d] = FALSE\n",fhword[i], i);
        break;
      }
      i++;
    }
    if(i == hash){
      //fprintf(stdout, "i==n \n");
      j = hash;
      m = hash+LISTLEN;
      while(lhWordCounter[j] != 0 && j < m){
        if(wordcmp(lhword[j], &s[start], wordLen) == 0){
          lhWordCounter[j] += 1;
          //fprintf(stderr, "add [%d], %s\t%d\n", j, lhword[j], lhWordCounter[j]);
          break;
        }
        j++;
      }

      if(j == m){
        int l;
        for(l=0; l < 100; l++){
          if(addListHashFh[l] == hash) break;
        }
        if(l==100){
          addListHashFh[k++] = hash;
        }
      }
//      fprintf(stderr, "j = %d, m = %d\n", j, m);
      if(lhWordCounter[j] == 0){
        wordcpy(lhword[j], &s[start], wordLen);
        lhWordCounter[j] = 1;
        //fprintf(stderr, "store [%d], %s\t%d\n", j, lhword[j], lhWordCounter[j]);
      }
    }
    //fprintf(stderr, "%s\t%d\n", lhword[i], lhWordCounter[i]);
  }
  fprintf(stdout, "kLh = %d\n", k);

  //QuickSort fast half
  int sortArray[WNUM/2];
  j = 0;
  for(i=0; i<WNUM; i++){
    if(fhWordCounter[i] > 0 && countFlag[i] == TRUE){
      sortArray[j] = i;
      j++;
    }
  }
  q_sort(sortArray, fhWordCounter, 0, j-1);

  printf("%s\n", "fast half");
  for(i=0; i<5; i++){
    printf("%d. %s\t%d\n", i+1,fhword[sortArray[j-1-i]], fhWordCounter[sortArray[j-1-i]]);
  }

  //QuickSort last half
  j = 0;
  for(i=0; i<WNUM; i++){
    if(lhWordCounter[i] > 0 ){
      sortArray[j] = i;
      j++;
    }
  }
  q_sort(sortArray, lhWordCounter, 0, j-1);

  printf("\n%s\n", "last half");
  for(i=0; i<5; i++){
    printf("%d. %s\t%d\n", i+1, lhword[sortArray[j-1-i]], lhWordCounter[sortArray[j-1-i]]);
  }
  passage = clock();
  fprintf(stdout, "time: %.2fms\n", (double)passage / CLOCKS_PER_SEC * 1000);

  return 0;
}

int getHash(char* a, int s, int e){
  unsigned long hash = 0;
  int i;
  for(i=s; i<=e; i++){
    hash = hash * 29 + tolower(a[i]);
  }
  return (int)( (hash * LISTLEN) % (WNUM / LISTLEN) );
}

int getWord(char* addr, int* now, int* start, int* end){
  int i = *now;
  while(!isalnum(addr[i]) && addr[i] != '\0' && addr[i] != '@') i++;
  *start = i;
  while(!isspace(addr[i]) && addr[i] != '\0' && addr[i] != '@'){
    i++;
  }
  *now = i;
  while(!isalnum(addr[i])){
    i--;
  }
  *end = i;
  return (*end)-(*start)+1;
}

void q_sort(int s[], int w[], int left, int right){
  int pivot, l_hold, r_hold;

  l_hold = left;
  r_hold = right;
  pivot = s[left];

  while (left < right){

    while ((w[ s[right] ] >= w[pivot]) && (left < right)) right--;

    if (left != right){
      s[left] = s[right];
      left++;
    }

    while ((w[s[left]] <= w[pivot]) && (left < right)) left++;

    if (left != right){
      s[right] = s[left];
      right--;
    }
  }
  s[left] = pivot;
  pivot = left;
  left = l_hold;
  right = r_hold;
  if (left < pivot && (right-left) < 20) q_sort(s, w, left, pivot-1);
  if (right > pivot ) q_sort(s, w, pivot+1, right);
}

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
  if(i == n) return 0;
  else return 1;
}

/*
fprintf(stdout, "\nfast half\n");
for(i=0; i<WNUM; i++){
if(fhWordCounter[i] > 0 && countFlag[i] == TRUE){
fprintf(stdout, "%s\t%d\n", fhword[i], fhWordCounter[i]);
}
}
fprintf(stdout, "\nlast half\n");
for(i=0; i<WNUM; i++){
if(lhWordCounter[i] > 0){
fprintf(stdout, "%s\t%d\n", lhword[i], lhWordCounter[i]);
}
}
*/
