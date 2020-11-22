#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>

#define N 1000000
long long counter = 0;
static sem_t sem_1;

void* counting_thread(void* arg) {
	
	int offset = *(int*) arg;
	int i;
	for(i = 0; i < N; i++){
		counter += offset;
	}
	sem_post(&sem_1);
	pthread_exit(NULL);
}

int main() {
	sem_init(&sem_1, 0, 0);
	
	pthread_t thread_1;
	int offset = 1;
	pthread_create(&thread_1, NULL, counting_thread, &offset);

	sem_wait(&sem_1); // wait here until &thread_1 transmit post signal semaphore.

	pthread_t thread_2;
  	offset = -1;
	pthread_create(&thread_2, NULL, counting_thread, &offset);

	pthread_join(thread_1, NULL);
	pthread_join(thread_2, NULL);

	printf("Final counter value: %lld\n", counter);
	sem_destroy(&sem_1);
	return 0;
} 

