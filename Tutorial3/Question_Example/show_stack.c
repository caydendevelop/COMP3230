#define _GNU_SOURCE /* To get pthread_getattr_np() declaration */
#include <assert.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

void show_stack(pthread_attr_t *attr, pthread_t thread, char *prefix) {
  size_t stack_size, guard_size;
  void *stack_addr;
  int rc;

  rc = pthread_attr_getguardsize(attr, &guard_size);
  assert(rc == 0);

  rc = pthread_attr_getstack(attr, &stack_addr, &stack_size);
  assert(rc == 0);

  printf("Thread %s (id=%lu) stack:\n", prefix, thread);
  printf("\tstart address\t= %p\n", stack_addr);
  printf("\tend address\t= %p\n", stack_addr + stack_size);
  printf("\tstack size\t= %.2f MB\n", stack_size / 1024.0 / 1024.0);
  printf("\tguard size\t= %lu B\n", guard_size);
}

void *entry_point(void *arg) {
  pthread_t thread = pthread_self();

  int rc;
  pthread_attr_t attr;
  rc = pthread_getattr_np(thread, &attr);
  assert(rc == 0);

  pthread_mutex_lock(&lock);
  show_stack(&attr, thread, (char *)arg);
  pthread_mutex_unlock(&lock);

  return NULL;
}

int main(int argc, char *argv[]) {
  pthread_t p1, p2;
  int rc;

  rc = pthread_create(&p1, NULL, entry_point, "1");
  assert(rc == 0);
  rc = pthread_create(&p2, NULL, entry_point, "2");
  assert(rc == 0);

  entry_point("main");

  rc = pthread_join(p1, NULL);
  assert(rc == 0);
  rc = pthread_join(p2, NULL);
  assert(rc == 0);

  return 0;
}
