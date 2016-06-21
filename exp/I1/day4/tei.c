#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <time.h>
#include <stdio.h>

int main(){
  clock_t passage;
  double sec;
  char* filename = "tei.raw";
  int fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
  int i, j;
  char data[1000][1000];
  for ( i = 0; i < 1000; i++ ) {
    for ( j = 0; j < 1000; j++ ){
      data[i][j] = 0;
      write(fd, &data[i][j], 1);
    }
  }
  if ( (passage = clock()) == (clock_t)(-1) ) {
    fprintf(stderr,"経過時間の取得に失敗しました.\n");
    return 1;
  }
  sec = (double)passage / CLOCKS_PER_SEC * 1000;

  /* 経過時間を秒単位で表示 */
  printf("\nプログラム開始から%.2fm秒経過しました．\n", sec);

  return 0;
}
