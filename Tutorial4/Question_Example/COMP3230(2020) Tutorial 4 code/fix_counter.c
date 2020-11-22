#include <stdio.h>
#include <pthread.h>

#define N 1000000
long long counter = 0;

void* counting_thread(void* arg) {
	int offset = *(int*) arg;
	int i;
	for(i = 0; i < N; i++){
		counter += offset;
	}
	pthread_exit(NULL);
}

int main() {
	pthread_t th1;
	int offset = 1;
	pthread_create(&th1, NULL, counting_thread, offset);

	pthread_t th2;
  offset = -1;
	pthread_create(&th2, NULL, counting_thread, offset);

	pthread_join(th1, NULL);
	pthread_join(th2, NULL);

	printf("Final counter value: %lld\n", counter);
	return 0;
} 

