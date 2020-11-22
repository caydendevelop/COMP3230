#define _GNU_SOURCE
#include <assert.h>
#include <pthread.h>
#include <stdio.h>
#include <sys/sysinfo.h>

int main(int argc, char *argv[]) {
  int rc;
  cpu_set_t cpuset;
  pthread_t thread;

  thread = pthread_self();

  rc = pthread_getaffinity_np(thread, sizeof(cpu_set_t), &cpuset);
  assert(rc == 0);
  printf("Before setting: pthread_setaffinity_np() contained:\n");
  printf("CPU ");
  for (int j = 0; j < CPU_SETSIZE; j++)
    if (CPU_ISSET(j, &cpuset))
      printf("%d ", j);
  printf("\n");

  printf("Set affinity mask to include CPUs (1, 3, 5, ...  2n+1)\n");
  CPU_ZERO(&cpuset);
  for (int j = 0; j < get_nprocs(); j++)
    if (j % 2 == 1)
      CPU_SET(j, &cpuset);
  rc = pthread_setaffinity_np(thread, sizeof(cpu_set_t), &cpuset);
  assert(rc == 0);

  rc = pthread_getaffinity_np(thread, sizeof(cpu_set_t), &cpuset);
  assert(rc == 0);
  printf("After setting: pthread_setaffinity_np() contained:\n");
  printf("CPU ");
  for (int j = 0; j < CPU_SETSIZE; j++)
    if (CPU_ISSET(j, &cpuset))
      printf("%d ", j);
  printf("\n");

  return 0;
}
