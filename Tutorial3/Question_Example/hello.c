#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

void *entry_point(void *arg) {
  printf("Hello from thread %d\n", *(int *)arg);
  return NULL;
}

int main(int argc, char *argv[]) {
  const int N = 8;
  pthread_t p[N];
  int args[N];
  int rc;
  printf("main: begin\n");

  // create N threads
  for (int i = 0; i < N; ++i) {
    args[i] = i;
    if ((rc = pthread_create(&p[i], NULL, entry_point, &args[i])) != 0) {
      fprintf(stderr, "error: creation failed. rc=%d\n", rc);
      exit(1);
    }
  }

  // join waits for the threads to finish
  for (int i = 0; i < N; ++i) {
    if ((rc = pthread_join(p[i], NULL)) != 0) {
      fprintf(stderr, "error: join failed. rc=%d\n", rc);
      exit(1);
    }
  }

  printf("main: end\n");
  return 0;
}
