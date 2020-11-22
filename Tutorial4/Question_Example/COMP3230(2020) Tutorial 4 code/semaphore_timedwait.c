#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <errno.h>
#include <stdarg.h>


void time_printf(const char *funcName, const char *format, ...);

sem_t relationship;
sem_t respond;
int delay = 3;

struct timespec ts;
int setTimespec(int sec, long nanoSec){
  int ret;
  if ((ret = clock_gettime(CLOCK_REALTIME, &ts)) != 0) {
    /* handle error */
    printf("Init clock failed\n");
  } else {
    ts.tv_sec += sec;
    ts.tv_nsec += nanoSec;
  } 
  return ret;
}


void* theBoy(void* arg){
  sleep(1); // he is thinking...
  time_printf(__func__, "This is not the right time. How about we disscuss it %d sec later?\n", delay);
  sem_post(&respond);
  setTimespec(delay, 0);
  sleep(delay + 1);  // the boy go to do something else

  time_printf(__func__, "Still not the right time. Maybe another %d sec?\n", delay);
  sem_post(&respond);
  setTimespec(delay, 0);
  sleep(delay + 1);

  time_printf(__func__, "Busy now. I'll confirm in %d sec, OK?\n", delay);
  sem_post(&respond);
  setTimespec(delay, 0);
  sleep(delay + 1);

  /* time_printf(__func__, "Confirm relationship\n"); */
  /* sem_post(&respond); */
  /* sem_post(&relationship); */

  pthread_exit(NULL);
}

void* theGirl(void* arg){
  time_printf(__func__, "Hey, how come we haven't confirmed our relationship yet?\n"); 
  sem_wait(&respond);
  int s = sem_timedwait(&relationship, &ts);
  /* Check what happened */
  if (s == -1) {
    if (errno == ETIMEDOUT) time_printf(__func__, "(Timeout) Now what?\n");
    else perror("sem_timedwait");
  } else {
    time_printf(__func__, "Relationship confirmed.\n");
    pthread_exit(NULL);
  }

  sem_wait(&respond);
  time_printf(__func__, "I'm wating...\n"); 
  s = sem_timedwait(&relationship, &ts);
  if (s == -1) {
    if (errno == ETIMEDOUT) time_printf(__func__, "(Timeout) Last chance!\n");
    else perror("sem_timedwait");
  } else {
    time_printf(__func__, "Relationship confirmed.\n");
  }

  sem_wait(&respond);
  time_printf(__func__, "I'm still wating...\n"); 
  s = sem_timedwait(&relationship, &ts);
  if (s == -1) {
    if (errno == ETIMEDOUT) time_printf(__func__, "(Timeout) You liar! Bye!\n");
    else perror("sem_timedwait");
  } else {
    time_printf(__func__, "Relationship confirmed.\n");
  }

  pthread_exit(NULL);
}

int main(int argc, char *argv[]) {
  sem_init(&relationship, 0, 0);
  sem_init(&respond, 0, 0);
  pthread_t girl;
  pthread_t boy;

  int ret;
  if ((ret = pthread_create(&girl, NULL, theGirl, NULL)) != 0){
    printf("line: %d, pthread_create failed!\n", __LINE__);
    exit(-1);
  }

  if ((ret = pthread_create(&boy, NULL, theBoy, NULL)) != 0){
    printf("line: %d, pthread_create failed!\n", __LINE__);
    exit(-1);
  }

  pthread_join(girl, NULL);
  pthread_join(boy, NULL);
  
  return 0;
}


#define PRINT_BLUE      fprintf(stderr,"\033[0;34m");printf("\033[0;34m");
#define PRINT_YELLOW_B  fprintf(stderr,"\033[1;33m");printf("\033[1;33m");
#define PRINT_DEFAULT   fprintf(stderr,"\033[0m");   printf("\033[0m");
#define PRINT_CYAN      fprintf(stderr,"\033[0;36m");printf("\033[0;36m"); 
// How to use mutex?
static pthread_mutex_t print_mx;
static int print_mx_inited = 0;
static void initMX() {
  pthread_mutex_init(&print_mx, NULL);
  print_mx_inited = 1;
} 
// thread safe customized printf function
void time_printf(const char *funcName, const char *format, ...) {
  if (!print_mx_inited) initMX();
  pthread_mutex_lock(&print_mx);
  PRINT_BLUE;
  time_t t = time(NULL);
  struct tm tm = *localtime(&t);
  printf("[%02d:%02d:%02d]", tm.tm_hour, tm.tm_min, tm.tm_sec);
  PRINT_YELLOW_B;
  printf("%s ", funcName);
  PRINT_CYAN;
  va_list args;
  va_start(args, format);
  vprintf(format, args);
  va_end(args);
  fflush(stdout);
  PRINT_DEFAULT;
  pthread_mutex_unlock(&print_mx);
}

