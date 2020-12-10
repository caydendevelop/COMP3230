#include "job.h"

void reportJobDone(sem_t *sem_worker, int num_worker) {
	int *num_free_worker = malloc(sizeof(num_free_worker));
	sem_getvalue(sem_worker, num_free_worker);
	if(*num_free_worker < num_worker) {
		free(num_free_worker);
		sem_post(sem_worker);
	}else {
		printf("Error, number of free workers exceeds num_worker\n");
		free(num_free_worker);
		exit(1);
	}
}

int requestSpace(sem_t *space) {
#if DEBUG
	int *num_free_space = malloc(sizeof(num_free_space));
	sem_getvalue(space, num_free_space);
	printf("Requesting free space, current space=%d...\n", *num_free_space);
#endif
	sem_wait(space);
#if DEBUG
	sem_getvalue(space, num_free_space);
	printf("Space requested, current space=%d...\n", *num_free_space);
#endif

	return 0;
}

void releaseSpace(sem_t *space, int space_limit) {
	int *num_free_space = malloc(sizeof(num_free_space));
	sem_getvalue(space, num_free_space);
	if(*num_free_space < space_limit) {
#if DEBUG
		printf("releasing free space, current space=%d...\n", *num_free_space);
#endif
		sem_post(space);
#if DEBUG
		sem_getvalue(space, num_free_space);
		printf("Space released, current space=%d...\n", *num_free_space);
#endif
		free(num_free_space);
	} else {
		printf("Error, releasing space that doesn't exist\n");
		free(num_free_space);
		exit(1);
	}
}

void makeItem(sem_t *space, int makeTime, sem_t* item) {
	requestSpace(space);
	sleep(makeTime);
	sem_post(item);
}

void getItem(sem_t *space, int space_limit, sem_t *item) {
	sem_wait(item);
	releaseSpace(space, space_limit);
}

void makeSkeleton(sem_t *sem_space, sem_t *sem_skeleton) {
	// printf("Making skeleton\n");
	makeItem(sem_space, TIME_SKELETON, sem_skeleton);
}

void makeEngine(sem_t *sem_space, sem_t *sem_engine) {
	// printf("Making engine\n");
	makeItem(sem_space, TIME_ENGINE, sem_engine);
}

void makeChassis(sem_t *sem_space, sem_t *sem_chassis) {
	// printf("Making chassis\n");
	makeItem(sem_space, TIME_CHASSIS, sem_chassis);
}

void makeWindow(sem_t *sem_space, sem_t *sem_window) {
	// printf("Making Window\n");
	makeItem(sem_space, TIME_WINDOW, sem_window);
}

void makeTire(sem_t *sem_space, sem_t *sem_tire) {
	// printf("Making Tire\n");
	makeItem(sem_space, TIME_TIRE, sem_tire);
}

void makeBattery(sem_t *sem_space, sem_t *sem_battery ) {
	// call makeItem and pass in the time for makeing battery
	// printf("Making Battery\n");
	makeItem(sem_space, TIME_BATTERY, sem_battery);
}

void makeBody(sem_t *sem_space, int space_limit, sem_t *sem_body,
		sem_t *sem_skeleton, sem_t *sem_engine, sem_t *sem_chassis) {
	getItem(sem_space, space_limit, sem_skeleton);
	getItem(sem_space, space_limit, sem_engine);
	getItem(sem_space, space_limit, sem_chassis);
	// printf("Making Body after getting skeleton, engine, chassis\n");
	makeItem(sem_space, TIME_BODY, sem_body);
}

void makeCar(sem_t *sem_space, int space_limit, sem_t *sem_car,
		sem_t *sem_window, sem_t *sem_tire, sem_t *sem_battery, sem_t *sem_body) {
	int i;
	// call getItem to get a car body
	getItem(sem_space, space_limit, sem_body);

	// call getItem 7 times to get 7 windows
	for (i=0; i<7; i++)
	{
		getItem(sem_space, space_limit, sem_window);
	}
	// call getItem 4 times to get 4 tires
	for (i=0; i<4; i++)
	{
		getItem(sem_space, space_limit, sem_tire);
	}
	// call getItem to get a battery pack
	getItem(sem_space, space_limit, sem_battery);

	// printf("Making CAR after getting 7 windows, 4 tires, battery and body\n");
	// make the car
	makeItem(sem_space, TIME_CAR, sem_car);

	// notify the completion of making a car by post to sem_car
	sem_post(sem_space);

}
