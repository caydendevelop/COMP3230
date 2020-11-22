DEBUG=0
CC=gcc
CFLAGS= -pthread -Wall -lm

ifeq ($(DEBUG), 1)
	CFLAGS += -g
else
	CFLAGS += -O3
endif 

TARGET=hw1 hw2 hw3
ALL: $(TARGET)

$(TARGET): %: %.c
	$(CC) -o $@ $< $(CFLAGS)

clean:
	rm -rf *.o $(TARGET) *.s
