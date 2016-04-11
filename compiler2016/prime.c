#include <stdio.h>

int main(){
  int x = 2;
  int i;
  int flag;
  while(x<=5){
    i = 2;
    flag = 0;
    while(i<x){
      if(x % i == 0){
        flag = 1;
      }
      i++;
    }
    if(flag == 0){
      printf("%d\n",x);
    }
    x++;
  }
  return 0;
}