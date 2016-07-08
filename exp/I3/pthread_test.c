#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/types.h>

void *thread_func(void *param)
{
  int pid;
  int i;

  pthread_t thread_id;
  thread_id = pthread_self();
  fprintf(stderr , "thread_func called\n");
  fprintf(stderr , "  thread ID = %ld\n" , thread_id);
  pid = getpid();
  fprintf(stderr , "  2:pid=%d\n" , pid);
  for(i = 0 ; i < 6 ; i++)
  {
        sleep(1);
        fprintf(stderr , "thread_func() : count = %d\n" , i);
  }
  return NULL;
}

int main(int argc, char *argv[])
{
  pthread_t thread;
  pthread_attr_t thread_attr;
  int pid;
  int i;

  fprintf(stderr , "---Program start---\n");
  pid = getpid();
  fprintf(stderr , "1:pid = %d\n" , pid);

  pthread_attr_init(&thread_attr);
  pthread_attr_setdetachstate(&thread_attr , PTHREAD_CREATE_DETACHED);
  if(pthread_create(&thread , &thread_attr , thread_func , NULL) !=0)
    perror("pthread_create()");
  pthread_attr_destroy(&thread_attr);
  fprintf(stderr , "Next line of pthread_create() called. thread ID=%ld\n" , thread);
  for(i = 0 ; i < 3 ; i++)
  {
        sleep(1);
        fprintf(stderr , "main() : count=%d\n" , i);
  }
  fprintf(stderr , "---Program end---\n");
  pthread_exit(NULL);  //この行を加える
  return EXIT_SUCCESS;
}
