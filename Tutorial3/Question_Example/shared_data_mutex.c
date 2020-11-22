#include <stdio.h>
#include <pthread.h>
#include <assert.h>

static volatile int counter = 0;
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

void *entry_point(void *arg) {
    printf("Thread %s: begin\n", (char *)arg);
    pthread_mutex_lock(&lock);
    for (int i = 0; i < 1e7; ++i) {
        counter += 1;
    }
    pthread_mutex_unlock(&lock);
    printf("Thread %s: done\n", (char *)arg);
    return NULL;
}

int main(int argc, char *argv[]) {
    pthread_t p1, p2;
    printf("main: begin with counter = %d\n", counter);
    int rc;
    rc = pthread_create(&p1, NULL, entry_point, (void *)"A");
    assert(rc == 0);
    rc = pthread_create(&p2, NULL, entry_point, (void *)"B");
    assert(rc == 0);

    // join waits for the threads to finish
    rc = pthread_join(p1, NULL);
    assert(rc == 0);
    rc = pthread_join(p2, NULL);
    assert(rc == 0);
    printf("main: done with counter = %d\n", counter);
    return 0;
}
