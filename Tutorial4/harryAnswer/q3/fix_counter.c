#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>

#define N 1000000
long long counter = 0;
static sem_t cou_thred;

void* counting_thread(void* arg) {
	int offset = *(int*) arg;
	int i;
	for(i = 0; i < N; i++){
		counter += offset;
	}
	sem_post(&cou_thred);
	pthread_exit(NULL);
}

int main() {
	sem_init(&cou_thred, 0, 0);
	
	pthread_t th1;
	int offset = 1;
	pthread_create(&th1, NULL, counting_thread, &offset);

	sem_wait(&cou_thred);

	pthread_t th2;
  	offset = -1;
	pthread_create(&th2, NULL, counting_thread, &offset);

	pthread_join(th1, NULL);
	pthread_join(th2, NULL);

	printf("Final counter value: %lld\n", counter);
	sem_destroy(&cou_thred);
	return 0;
} 

