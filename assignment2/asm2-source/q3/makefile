OBJS = main.o job.o worker.o
CC = gcc
DEBUG = -g
CFLAGS = -Wall -fopenmp -pthread

all: tesla_factory.out

tesla_factory.out: $(OBJS)  
	$(CC) $(CFLAGS) $(OBJS) -o tesla_factory.out -lm

main.o: definitions.h main.h main.c
	$(CC) $(CFLAGS) -c main.c

worker.o: worker.h job.h definitions.h worker.c
	$(CC) $(CFLAGS) -c worker.c

job.o: definitions.h job.h job.c
	$(CC) $(CFLAGS) -c job.c

clean:
	rm *.o *.out
