/* 
 * p06.c
 */

/* 
 * usage:
 * 
 *   ./a.out
 *
 * Intented behavior:
 * It should print the content of this file.
 *
 */
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/io.h>
#include <unistd.h>

int main()
{
  int fd = open("p06.c", O_RDONLY);
  char buf[1000];
  while (1) {
    int n = read(fd, buf, 1000);
    if (n == 0) break;
    write(1, buf, n);
  }
  return 0;
}
