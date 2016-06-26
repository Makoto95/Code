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

#define PRIME1 69691 //257
//#define PRIME2 67 //263
#define PRIMEHASH 37
#define TRUE 1
#define FALSE 0
#define WSIZE 35

typedef struct element{
  uint8_t word[WSIZE];
  int32_t counter;
  int8_t countFlag;
  struct element *next;
}element_t;

inline void elementInit(element_t* this){
  memset(this->word, '\0', WSIZE);
  this->counter = 0;
  this->countFlag = TRUE;
  this->next = NULL;
}

inline void elementInit(element_t*);
inline uint_fast64_t getHash(char*, int_fast32_t, int_fast32_t);
inline int_fast32_t getWord(char*, int_fast32_t*, int_fast32_t*, int_fast32_t*);
inline int8_t wordcmp(uint8_t* , char*, int_fast32_t);
inline void wordcpy(uint8_t*, char*, int_fast32_t);
inline void q_sort(element_t*[], uint_fast32_t, uint_fast32_t);
inline void storeWord(element_t*, char*, int_fast32_t);
inline int8_t searchAndFlagIndex(element_t*, char*, int_fast32_t);

int main(){
  clock_t passage; //stop watch

  /*----counter value----*/
  uint_fast32_t n;
  uint_fast32_t i=0;//, j=0;
  int cmalloc = 0;
  int store = 0;
  /*----ready index----*/
  element_t fhIndex[PRIME1];
  element_t lhIndex[PRIME1];
  for( i = 0; i < PRIME1; i++){
      elementInit(&fhIndex[i]);
      elementInit(&lhIndex[i]);
  }

  int_fast32_t wordLen;
  uint_fast64_t hashBase;
  uint32_t hash1st;//, hash2nd;

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

  int_fast32_t now = 0;
  int_fast32_t start = 0;
  int_fast32_t end = 0;

  /*====Fist half create Index====*/
  while(1){
    wordLen = getWord(s, &now, &start, &end);
    hashBase = getHash(s, start, end);

    if(s[now] == '@' && (wordLen < 1)) break;

    /*----calculate Array addres----*/
    hash1st = (uint_fast32_t)(hashBase % PRIME1);

    /*----linear search Array addres----*/
    element_t* actElement = &fhIndex[hash1st];
    if(actElement->counter == 0){
      storeWord(actElement, &s[start], wordLen);
      store++;
    }
    else{
      while(actElement->counter != 0){
        if(wordcmp(actElement->word, &s[start], wordLen) == 0){
          (actElement->counter)++;
          break;
        }
        if(actElement->next != NULL){
          actElement = actElement->next;
        }else{
          actElement->next = (element_t*)malloc(sizeof(element_t));
          cmalloc++;
          actElement = actElement->next;
          elementInit(actElement);
          storeWord(actElement, &s[start], wordLen);
          break;
        }
      }
    }
  }
  /*====Fist half create Index end====*/

  now++;

/*====Last half create Index====*/
  while(1){
    wordLen = getWord(s, &now, &start, &end);
    hashBase = getHash(s, start, end);

    if(s[now] == '\0' && (wordLen < 1)) break;

    /*----calculate Array addres----*/
    hash1st = (uint32_t)(hashBase % PRIME1);

    if(searchAndFlagIndex(&fhIndex[hash1st], &s[start], wordLen) == 0){
      /*----linear search Array addres----*/
      element_t* actElement = &lhIndex[hash1st];
      if(actElement->counter == 0){
        storeWord(actElement, &s[start], wordLen);
        store++;
      }
      else{
        while(actElement->counter != 0){
          if(wordcmp(actElement->word, &s[start], wordLen) == 0){
            (actElement->counter)++;
            break;
          }
          if(actElement->next != NULL){
            actElement = actElement->next;
          }else{
            actElement->next = (element_t*)malloc(sizeof(element_t));
            cmalloc++;
            actElement = actElement->next;
            elementInit(actElement);
            storeWord(actElement, &s[start], wordLen);
            break;
          }
        }
      }
    }

  }
/*====Last half create Index end====*/

  //QuickSort first half
  element_t* sortArray[PRIME1];
  n = 0;
  for(i=0; i<PRIME1; i++){
      if(fhIndex[i].counter > 0 && fhIndex[i].countFlag == TRUE){
        sortArray[n] = &fhIndex[i];
        n++;
      }
      if(fhIndex[i].next != NULL){
        element_t* actElement = fhIndex[i].next;
        while(1){
          if(actElement->counter > 0 && actElement->countFlag == TRUE){
            sortArray[n] = actElement;
            n++;
          }
          if(actElement->next == NULL) break;
          actElement = actElement->next;
        }
      }

  }

  q_sort(sortArray, 0, n-1);

  printf("%s\n", "first half");
  for(i=0; i<5; i++){
    printf("%d. %s\t%d\n", i+1, sortArray[n-1-i]->word, sortArray[n-1-i]->counter);
  }

  //QuickSort last half

  n = 0;
  for(i=0; i<PRIME1; i++){

      if(lhIndex[i].counter > 0){
        sortArray[n] = &lhIndex[i];
        n++;
      }
      if(lhIndex[i].next != NULL){
        element_t* actElement = lhIndex[i].next;
        while(1){
          if(actElement->counter > 0){
            sortArray[n] = actElement;
            n++;
          }
          if(actElement->next == NULL) break;
          actElement = actElement->next;
        }
      }

  }

  q_sort(sortArray, 0, n-1);

  printf("%s\n", "last half");
  for(i=0; i<5; i++){
    printf("%d. %s\t%d\n", i+1, sortArray[n-1-i]->word, sortArray[n-1-i]->counter);
  }

  if ( (passage = clock()) == (clock_t)(-1) ) {
        fprintf(stderr,"can not take time\n");
        return 1;
    }
    double millisec = (double)passage / CLOCKS_PER_SEC * 1000;

    /* 経過時間をミリ秒単位で表示 */
    printf("\ntime: %.0f ms %d %d\n", millisec, cmalloc, store);

  return 0;
}

inline uint_fast64_t getHash(char* addr, int_fast32_t start, int_fast32_t end){
  int_fast32_t i = start;
  uint_fast64_t hash = 0;
  while(i <= end){
    hash = hash * PRIMEHASH + tolower(addr[i]);
    i++;
  }
  return hash;
}

inline int_fast32_t getWord(char* addr, int_fast32_t* now, int_fast32_t* start, int_fast32_t* end){
  int_fast32_t i = *now;
  while(!isalnum(addr[i]) && addr[i] != '\0' && addr[i] != '@') i++;
  *start = i;
  while(!isspace(addr[i]) && addr[i] != '\0' && addr[i] != '@') i++;
  *now = i;
  while(!isalnum(addr[i])) i--;
  *end = i;
  return (*end)-(*start)+1;
}

inline void q_sort(element_t* el[], uint_fast32_t left, uint_fast32_t right){
  uint_fast32_t l_hold, r_hold, pivotnum;
  element_t* pivot;
  l_hold = left;
  r_hold = right;
  pivot = el[left];

  while (left < right){

    while ((el[right]->counter >= pivot->counter) && (left < right)) right--;

    if (left != right){
      el[left] = el[right];
      left++;
    }

    while ((el[left]->counter <= pivot->counter) && (left < right)) left++;

    if (left != right){
      el[right] = el[left];
      right--;
    }
  }
  el[left] = pivot;

  pivotnum = left;
  left = l_hold;
  right = r_hold;
  if (left < pivotnum && (right-left) < 300 ) q_sort(el, left, pivotnum-1); //
  if (right > pivotnum ) q_sort(el, pivotnum+1, right);
}

inline void wordcpy(uint8_t* dest, char* src, int_fast32_t n){
  int_fast32_t i;
  for(i = 0; i < n ; i++){
    dest[i] = tolower(src[i]);
  }
}

inline void storeWord(element_t* el, char* s, int_fast32_t L){
  wordcpy(el->word, s, L);
  (el->counter) = 1;
}

inline int8_t wordcmp(uint8_t* s1, char* s2, int_fast32_t n){
  int_fast32_t i;
  for(i = 0; i < n; i++){
    if(s1[i] != (uint8_t)tolower(s2[i])) break;
  }
  if(i == n) return 0;
  else return 1;
}

inline int8_t searchAndFlagIndex(element_t* el, char* s, int_fast32_t L){
  element_t* actEl = el;
  while(1){
    if(wordcmp(actEl->word, s, L) == 0){
      (actEl->countFlag) = FALSE;
      break;
    }
    if(actEl->next == NULL){
      return 0;
    }
    actEl = actEl->next;
  }
  return 1;
}
