#include "queue.h"
#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>

sem_t semEnQ;
sem_t semDeQFront;
sem_t semDeQRear;


Queue queueCreate(unsigned capacity){
  sem_init(&semEnQ, 0, 1);
  sem_init(&semDeQFront, 0, 1);
  sem_init(&semDeQRear, 0, 1);
  Queue q = malloc(sizeof(queue_t)); 
  q->capacity = capacity;
  q->front = q->size = 0;
  q->rear = capacity - 1;
  q->array = malloc(q->capacity * sizeof(*q->array));
  return q;
}

void queueDestroy(Queue q){
  free(q->array);
  free(q);
  sem_destroy(&semEnQ);
  sem_destroy(&semDeQFront);
  sem_destroy(&semDeQRear);
}

int queueIsFull(Queue q){
  return (q->size == q->capacity); 
}

int queueIsEmpty(Queue q){
  return (q->size == 0);
}

int queueEnqueue(Queue q, int* value){ 
  sem_wait(&semEnQ);
  if (queueIsFull(q)) return -1;
  q->rear = (q->rear + 1) % q->capacity;
  q->array[q->rear] = *value;
  q->size = q->size + 1;
  sem_post(&semEnQ);
  return 0;
}

int queueDequeueFront(Queue queue, int* container){ 
  sem_wait(&semDeQFront);
  if (queueIsEmpty(queue)) 
  { 
    sem_post(&semDeQFront);
    return -1; 
  }
  *container = queue->array[queue->front];
  queue->front = (queue->front + 1) % queue->capacity;
  queue->size = queue->size - 1;
  sem_post(&semDeQFront);
  return 0;
}

int queueDequeueRear(Queue queue, int* container){
  sem_wait(&semDeQRear);
  if (queueIsEmpty(queue)) {
    sem_post(&semDeQRear);
    return -1; 
  }
  *container = queue->array[queue->rear];
  queue->rear = (queue->capacity + queue->rear - 1) % queue->capacity;
  queue->size = queue->size - 1; 
  sem_post(&semDeQRear);
  return 0;
}

int* queueFront(Queue queue){
  if (queueIsEmpty(queue)) return NULL;
  return &queue->array[queue->front]; 
}

int* queueRear(Queue queue){ 
  if (queueIsEmpty(queue)) return NULL;
  return &queue->array[queue->rear];
}

void queuePrint(Queue q){
  printf("Queue(^: front, *: rear): [");
  int i;
  for(i = 0; i < q->capacity; i++){
    printf("%d", q->array[i]);
    if(i == q->front) printf("^");
    if(i == q->rear) printf("*");
    i == q->capacity - 1 ? printf("] ") : printf(", ");
  }
  printf("size=%d, capacity=%d\n", q->size, q->capacity);
  fflush(stdout);
} 
