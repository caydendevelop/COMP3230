#include <assert.h>
#include <limits.h>
#include <pthread.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>

typedef struct _data {
  int *arr;
  int *tmp;
  int low;
  int high;
  int k;
  int threshold;
} data_t;

void init_array(int *p, size_t len) {
  for (int i = 0; i < len; ++i) {
    p[i] = rand() % 100000;
  }
}

int *gen_array(size_t len) {
  int *p = (int *)malloc(len * sizeof(int));
  if (!p) {
    printf("failed to allocate %ld bytes memory!", len * sizeof(int));
    exit(1);
  }
  return p;
}

void swap(int *a, int *b) {
  int tmp = *a;
  *a = *b;
  *b = tmp;
}

void kway_merge(int *arr, int *tmp, int *sep, int k) {
  int low = sep[0];
  int high = sep[k];
  memcpy(tmp + low, arr + low, (high - low) * sizeof(int));

  int *sub_arr[k];
  for (int i = 0; i < k; ++i) {
    sub_arr[i] = tmp + sep[i];
  }

  int sub_idx[k];
  memset(sub_idx, 0, k * sizeof(int));
  int sub_len[k];
  for (int i = 0; i < k; ++i) {
    sub_len[i] = sep[i + 1] - sep[i];
  }
  int idx = low;

  while (idx < high) {
    int min_value = INT_MAX;
    int min_index = -1;

    // choose the smallest
    for (int i = 0; i < k; ++i) {
      if (sub_idx[i] < sub_len[i] && min_value > sub_arr[i][sub_idx[i]]) {
        min_value = sub_arr[i][sub_idx[i]];
        min_index = i;
      }
    }

    arr[idx++] = min_value;
    sub_idx[min_index]++;
  }
}

void sort_k(int *arr, int low, int high) {
  for (int i = low; i < high; ++i) {
    for (int j = i + 1; j < high; ++j) {
      if (arr[i] > arr[j]) {
        swap(&arr[i], &arr[j]);
      }
    }
  }
}

void *kway_mergesort(void *arg) {
  data_t *data = (data_t *)(arg);
  int *arr = data->arr;
  int *tmp = data->tmp;
  int low = data->low;
  int high = data->high;
  int k = data->k;
  int threshold = data->threshold;

  if (1 >= high - low) {
    return NULL;

  } else if (k > high - low) {
    sort_k(arr, low, high);
    return NULL;
  }

  int len = high - low;
  int chunk_size = len / k;
  int r = len % k;

  data_t args[k];
  int sep[k + 1];
  int lo = low, hi = low;
  for (int i = 0; i < k; ++i) {
    lo = hi;
    hi += chunk_size;
    sep[i] = lo;
    args[i] = (data_t){arr, tmp, lo, hi, k, threshold};
  }
  sep[k] = high;
  if (r) {
    args[k - 1].high += r;
  }
  assert(args[k - 1].high == high);
  assert(args[0].arr);

  // if size is not too small
  if (high - low > threshold) {
    int rc;
    pthread_t ph[k];

    // create k threads
    for (int i = 0; i < k; ++i) {
      rc = pthread_create(&ph[i], NULL, kway_mergesort, (void *)&args[i]);
      assert(rc == 0);
    }

    for (int i = 0; i < k; ++i) {
      rc = pthread_join(ph[i], NULL);
      assert(rc == 0);
    }
  } else {
    for (int i = 0; i < k; ++i) {
      kway_mergesort((void *)&args[i]);
    }
  }

  // merging the sorted arrays
  kway_merge(arr, tmp, sep, k);
  return NULL;
}

int verify_sort_results(int *arr, int len) {
  for (int i = 1; i < len; ++i) {
    if (arr[i - 1] > arr[i]) {
      return 1;
    }
  }
  return 0;
}

int main(int argc, char *argv[]) {
  printf("This is the BEGINNING of the program.\n");
  if (argc - 1 != 3) {
    printf("Error: The number of input integers now is %d. Please input 3 "
           "integers.\n",
           argc - 1);
    exit(1);
  }
  const int num = atoi(argv[1]);
  const int k = atoi(argv[2]);
  const int level = atoi(argv[3]);
  if (k < 2) {
    printf("k should not be less than 2!\n");
    exit(1);
  }

  printf("num: %d; ", num);
  printf("k: %d.", k);
  printf("level: %d.", level);
  int spawn_num = pow(k, level);
  int threshold = num / spawn_num;
  printf("threshold: %d.\n", threshold);

  printf("Sort max_num = %d integers.\n", num);

  srand(time(NULL));

  int *arr = gen_array(num);
  init_array(arr, num);
  int *tmp = gen_array(num);

  struct timespec start, end;
  printf("Start timing...\n");
  clock_gettime(CLOCK_MONOTONIC_RAW, &start);

  data_t arg = {arr, tmp, 0, num, k, threshold};
  kway_mergesort((void *)&arg);

  clock_gettime(CLOCK_MONOTONIC_RAW, &end);
  printf("End timing.\n");

  uint64_t delta_us = (end.tv_sec - start.tv_sec) * 1.0e6 +
                      (end.tv_nsec - start.tv_nsec) * 1.0e-3;
  printf("[Merge sort]The elapsed time is %.2f ms.\n", delta_us / 1000.0);

  if (verify_sort_results(arr, num)) {
    printf("Result is wrong!\n");
  } else {
    printf("Result is correct!\n");
  }
  free(arr);
  free(tmp);
  printf("This is the END of the program.\n");
  return 0;
}
