DEBUG = 1
CC = gcc
CFLAGS = -fopenmp -pthread -Wall -ggdb

SAMPLES = hello semaphore_counting semaphore_binary semaphore_timedwait producer_consumer fix_counter

all: $(SAMPLES)

hello: hello.c
	$(CC) $(CFLAGS) $? -o $@

semaphore_timedwait: semaphore_timedwait.c
	$(CC) $(CFLAGS) $? -o $@ 

semaphore_binary: semaphore_binary.c
	$(CC) $(CFLAGS) $? -o $@

semaphore_counting: semaphore_counting.c
	$(CC) $(CFLAGS) $? -o $@

producer_consumer: producer_consumer.c
	$(CC) $(CFLAGS) $? -o $@

fix_counter.out: fix_counter.c
	$(CC) $(CFLAGS) $? -o $@

clean:
	rm -f $(SAMPLES)
