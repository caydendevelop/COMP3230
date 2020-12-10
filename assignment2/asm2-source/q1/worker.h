#include "definitions.h"
#include "job.h"
typedef struct resource_pack {
	int space_limit;
	int num_workers;
	sem_t *sem_space;
	sem_t *sem_worker;

	sem_t *sem_skeleton;
	sem_t *sem_engine;
	sem_t *sem_chassis;
	sem_t *sem_body;

	sem_t *sem_window;
	sem_t *sem_tire;
	sem_t *sem_battery;
	sem_t *sem_car;
} resource_pack;

typedef struct work_pack {
	int tid;   // worker ID
	int jid;   // job ID
	int times; // how many times the job be run
	resource_pack *resource;
} work_pack;



void work(work_pack *arg);
