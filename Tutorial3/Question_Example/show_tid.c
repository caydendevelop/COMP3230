#include <assert.h>
#include <pthread.h>
#include <stdio.h>
#include <sys/syscall.h>
#include <unistd.h>

#define gettidv1() syscall(__NR_gettid) // new form
#define gettidv2() syscall(SYS_gettid)  // traditional form

void *ThreadFunc1(void *arg) {
  printf("the pthread_1 id is %ld\n", pthread_self());
  printf("the thread_1's PID is %d\n", getpid());
  printf("The LWP (TID) of thread_1 is: %ld\n", (long int)gettidv1());

  return 0;
}

void *ThreadFunc2(void *arg) {
  printf("the pthread_2 id is %ld\n", pthread_self());
  printf("the thread_2's PID is %d\n", getpid());
  printf("The LWP (TID) of thread_2 is: %ld\n", (long int)gettidv1());

  return 0;
}

int main(int argc, char *argv[]) {
  pthread_t p1, p2;
  int rc;

  printf("the main thread's pthread id is %ld\n", pthread_self());
  printf("the main thread's PID is %d\n", getpid());
  printf("The LWP of main thread is: %ld\n", (long int)gettidv1());

  // create two threads
  rc = pthread_create(&p1, NULL, ThreadFunc2, NULL);
  assert(rc == 0);
  rc = pthread_create(&p2, NULL, ThreadFunc1, NULL);
  assert(rc == 0);

  rc = pthread_join(p1, NULL);
  assert(rc == 0);
  rc = pthread_join(p2, NULL);
  assert(rc == 0);

  return 0;
}
