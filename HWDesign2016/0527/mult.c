#include <stdio.h>

int main(){
  unsigned char ans;
  unsigned char m1,m2;
  m1 = 2;
  printf("%b\n", m1);
  m2 = 3;
  printf("%b\n", m1);
  
  ans = m1 * m2;
  printf("%b\n", ans);
  return 0;
}
