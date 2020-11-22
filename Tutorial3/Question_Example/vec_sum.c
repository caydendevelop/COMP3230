#define _GNU_SOURCE
#include <assert.h>
#include <math.h>
#include <pthread.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/syscall.h>
#include <sys/sysinfo.h>
#include <time.h>
#include <unistd.h>

typedef struct _arg_t {
  float *dst;
  float *src;
  size_t len;
} arg_t;

void init_array(float *p, size_t len) {
  for (int i = 0; i < len; ++i) {
    p[i] = (float)rand() / RAND_MAX;
  }
}

float *gen_array(size_t len) {
  float *p = (float *)malloc(len * sizeof(float));
  if (!p) {
    printf("failed to allocate %ld bytes memory!", len * sizeof(float));
    exit(1);
  }
  return p;
}

void *vec_sum(void *args) {
  arg_t *vec = (arg_t *)args;

  float *dst = vec->dst;
  float *src = vec->src;
  size_t len = vec->len;
  for (int i = 0; i < len; ++i) {
    dst[i] += src[i];
  }

  return NULL;
}

void run(arg_t *args, int k, int flag) {
  pthread_t ph[k];
  int rc;
  cpu_set_t cpu_set[k];
  pthread_attr_t attr[k];
  for (int i = 0; i < k; ++i) {
    rc = pthread_attr_init(&attr[i]);
    assert(rc == 0);
  }

  if (flag) {
    int cpu;
    rc = syscall(SYS_getcpu, &cpu, NULL, NULL);
    assert(rc == 0);
    printf("Main thread runs on CPU %d\n", cpu);
    int start = cpu % 2;

    printf("Set affinity mask to include CPUs (%d, %d, %d, ...  %s)\n", start,
           start + 2, start + 4, start ? "2n+1" : "2n");
    size_t nprocs = get_nprocs();
    int i = 0, j = start;
    while (i < k) {
      if (j == cpu) {
        j += 2;
        continue;
      }
      if (j >= nprocs) {
        j = start;
      }

      CPU_ZERO(&cpu_set[i]);
      CPU_SET(j, &cpu_set[i]);
      pthread_attr_setaffinity_np(&attr[i], sizeof(cpu_set_t), &cpu_set[i]);
      ++i;
      j += 2;
    }
  }

  struct timespec start, end;
  clock_gettime(CLOCK_MONOTONIC_RAW, &start);

  for (int i = 0; i < k; ++i) {
    rc = pthread_create(&ph[i], &attr[i], vec_sum, (void *)&args[i]);
    assert(rc == 0);
  }

  for (int i = 0; i < k; ++i) {
    rc = pthread_join(ph[i], NULL);
    assert(rc == 0);
  }

  clock_gettime(CLOCK_MONOTONIC_RAW, &end);

  uint64_t delta_us = (end.tv_sec - start.tv_sec) * 1.0e6 +
                      (end.tv_nsec - start.tv_nsec) * 1.0e-3;
  printf("The elapsed time is %.2f ms.\n", delta_us / 1000.0);
}

int main(int argc, char *argv[]) {
  if (argc != 3) {
    printf("Error: The number of input integers now is %d. Please input 3 "
           "integers.\n",
           argc - 1);
    exit(1);
  }
  const int num = atoi(argv[1]);
  const int k = atoi(argv[2]);
  printf("vector len=%d. thread num=%d\n", num, k);

  srand(time(NULL));

  float *dst = gen_array(num);
  float *src = gen_array(num);
  init_array(dst, num);
  init_array(src, num);

  int chunk_size = num / k;
  int r = num % k;
  arg_t args[k];
  int lo = 0, hi = 0;
  for (int i = 0; i < k; ++i) {
    lo = hi;
    hi += chunk_size;
    args[i] = (arg_t){dst + lo, src + lo, hi - lo};
  }
  args[k - 1].len += r;

  // without setting affinity
  run(args, k, 0);

  // with setting affinity
  run(args, k, 1);

  free(dst);
  free(src);

  return 0;
}
