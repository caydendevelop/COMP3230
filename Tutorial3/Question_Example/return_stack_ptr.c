#include <assert.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct _arg_t {
  int a;
  int b;
} arg_t;

typedef struct _ret_t {
  int x;
  int y;
} ret_t;

void *entry_point(void *arg) {
  arg_t *p_arg = (arg_t *)arg;
  printf("%d %d\n", p_arg->a, p_arg->b);
  ret_t *r = (ret_t *)malloc(sizeof(ret_t));
  r->x = 1;
  r->y = 2;
  return (void *)r;
}

int main(int argc, char *argv[]) {
  pthread_t p;
  int rc;
  ret_t *r;

  arg_t args;
  args.a = 10;
  args.b = 20;
  rc = pthread_create(&p, NULL, entry_point, &args);
  assert(rc == 0);

  rc = pthread_join(p, (void **)&r);
  assert(rc == 0);
  printf("returned %d %d\n", r->x, r->y);
  free(r);
  return 0;
}
