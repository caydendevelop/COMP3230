#include "definitions.h"
#include "main.h"
#include <omp.h>

sem_t sem_worker;
sem_t sem_space;

sem_t sem_skeleton;
sem_t sem_engine;
sem_t sem_chassis;
sem_t sem_body;

sem_t sem_window;
sem_t sem_tire;
sem_t sem_battery;
sem_t sem_car;

int num_cars;
int num_spaces;
int num_workers;
int work_gp;

void *worker_group(void *args){
	resource_pack *rpack = (resource_pack *)args;
	int i;
	// Create 8 workpacks and 8 corresponding threads
	work_pack wpack[8];
	pthread_t thr[8];
	for(i = 0; i < 8; i++) {
		// prepare work_pack
		wpack[i].tid = i;
		wpack[i].resource = rpack;
		wpack[i].jid = i;
		// printf("-----Main: worker %d doing %d...\n", wpack[i].tid, wpack[i].jid);
		if (i == 4) // Windows
			wpack[i].times = 7;
		else if (i == 5) // Tires
			wpack[i].times = 4;
		else
			wpack[i].times = 1;

		if(pthread_create(&thr[i], NULL, work, &wpack[i]))
			printf("Error creating thread\n");
	}
	for(i = 0; i < 8; i++) {
		pthread_join(thr[i], NULL);
	}
	pthread_exit(NULL);
}

int main(int argc, char** argv)
{
	  if (argc < 4) {
	  printf("Usage: %s <number of cars> <number of spaces> <number of workers>\n",
	  argv[0]);
	  return EXIT_SUCCESS;
	  }
	  num_cars     = atoi(argv[1]);
	  num_spaces   = atoi(argv[2]);
	  num_workers  = atoi(argv[3]);
	printf("Name: Priyank Sharma\tUID: 3035397696\n");
	printf("Job defined, %d workers will build %d cars with %d storage spaces\n",
			num_workers, num_cars, num_spaces);

	resource_pack *rpack = (struct resource_pack*) malloc(sizeof(struct resource_pack));

	// put semaphores into resource_pack
	initResourcePack(rpack, num_spaces, num_workers);

	// Minimum number of spaces required to make a cars
	// is 16 (=7+4+1+1+(1+1+1))
	if (num_spaces < 16) //Handling the deadlock
	{
		// Start working and time the whole process
		int i, j;
		double production_time = omp_get_wtime();
		// printf("num_cars=%d\n", num_cars);

		// Create 8 work packs
		work_pack wpack[8];

		// Atleast 2 threads are needed
		pthread_t thr[2];

		//Define the order of execution of jobs
		int jobs[] = {3,0,1,2,7,4,5,6};
		for (i=0; i<8; i++)
		{
		  // prepare work_pack
		  wpack[i].resource = rpack;
		  wpack[i].jid = jobs[i];

		  if (jobs[i] == 4) // Windows
		    wpack[i].times = 7;
		  else if (jobs[i] == 5) // Tires
		    wpack[i].times = 4;
		  else
		    wpack[i].times = 1;
		}

		for (j = 0; j < num_cars; j++)
		{
		  //First make the body to keep freeing up space
		  wpack[0].tid = 0;
		  if(pthread_create(&thr[0], NULL, work, &wpack[0]))
		    printf("Error creating thread\n");

		  // Now make the body parts
		  for (i = 1; i < 4; i++)
		  {
		    wpack[i].tid = 1;
		    if(pthread_create(&thr[1], NULL, work, &wpack[i]))
		      printf("Error creating thread\n");

		    pthread_join(thr[1], NULL);
		  }

		  // Now the body is made
		  pthread_join(thr[0], NULL);

		  // Make the car to free up space for other parts
		  wpack[4].tid = 0;
		  if(pthread_create(&thr[0], NULL, work, &wpack[4]))
		    printf("Error creating thread\n");

		  // Do the remaining jobs using the remaining threads
		  for (i = 5; i < 8; i++)
		  {
		    wpack[i].tid = 1;
		    if(pthread_create(&thr[1], NULL, work, &wpack[i]))
		      printf("Error creating thread\n");

		    pthread_join(thr[1], NULL);
		  }
		  //Finally make the car
		  pthread_join(thr[0], NULL);
		}

		production_time = omp_get_wtime() - production_time;
		reportResults(production_time);

		destroySem();
		free(rpack);
	}
	else
	{
	//main threads
	pthread_t *threads = (pthread_t*) malloc(sizeof(pthread_t) * num_cars);

	// Start working and time the whole process
	int i, j, id, end;
	double production_time = omp_get_wtime();
	int work_gp = ceil(num_workers/8.0); //MAX NUMBER OF WORKER GROUPS AVAILABLE
	int last_workers = num_workers%8;
	// printf("work_gp=%d\n", work_gp);
	// printf("last_workers=%d\n", last_workers);
	// printf("num_cars=%d\n", num_cars);

	for(i = 0; i < num_cars; i++)
	{
		if(i >= work_gp && i%work_gp == 0)
		{
				if (num_workers%8 == 0)
					pthread_join(threads[i-1], NULL);
				else if (num_workers>8 && num_cars > 2)
					pthread_join(threads[i-2], NULL);
		}
		if((i+1)%work_gp == 0 && num_workers%8 != 0)
		{
			work_pack wpack[8];
			pthread_t thr[8];
			for(id = 0; id < ceil(8.0/last_workers); id++)
			{
				end = ((id+1)*last_workers) > 8? 8 : ((id+1)*last_workers);
				for(j = id*(last_workers); j < end; j++)
				{
					// prepare work_pack
					wpack[j].tid = j;
					wpack[j].resource = rpack;
					wpack[j].jid = j;
					//printf("-----Main: worker %d doing %d...\n", wpack[j].tid, wpack[j].jid);
					if (j == 4) // Windows
						wpack[j].times = 7;
					else if (j == 5) // Tires
						wpack[j].times = 4;
					else
						wpack[j].times = 1;

					if(pthread_create(&thr[j], NULL, work, &wpack[j]))
						printf("Error creating thread\n");
				}
				for(j = id*(last_workers); j < end; j++) {
					pthread_join(thr[j], NULL);
				}
			}
		}
		else{
			if(pthread_create(&threads[i], NULL, worker_group, (void *)rpack))
				printf("Error creating thread\n");
		}
	}
	for(i = 0; i < num_cars; i++) {
		if (!((i+1)%work_gp == 0 && num_workers%8 != 0))
			pthread_join(threads[i], NULL);
	}
	production_time = omp_get_wtime() - production_time;
	reportResults(production_time);

	destroySem();
	free(rpack);
}

	return EXIT_SUCCESS;
}

void reportResults(double production_time) {
	int *sem_value = malloc(sizeof(int));
	printf("=====Final report=====\n");

	sem_getvalue(&sem_skeleton, sem_value);
	printf("Unused Skeleton: %d\n",   *sem_value);
	sem_getvalue(&sem_engine,   sem_value);
	printf("Unused Engine: %d\n",     *sem_value);
	sem_getvalue(&sem_chassis,  sem_value);
	printf("Unused Chassis: %d\n",    *sem_value);
	sem_getvalue(&sem_body,     sem_value);
	printf("Unused Body: %d\n",       *sem_value);
	sem_getvalue(&sem_window,   sem_value);
	printf("Unused Window: %d\n",     *sem_value);
	sem_getvalue(&sem_tire,     sem_value);
	printf("Unused Tire: %d\n",       *sem_value);
	sem_getvalue(&sem_battery,  sem_value);
	printf("Unused Battery: %d\n",    *sem_value);

	sem_getvalue(&sem_space, sem_value);
	if (*sem_value < num_spaces) {
		printf("There are waste car parts!\n");
	}
	sem_getvalue(&sem_car, sem_value);
	printf("Production of %d %s done, production time: %f sec, space usage: %d\n",
			*sem_value,
			*sem_value > 1 ? "cars" : "car",
			production_time, num_spaces);
	printf("==========\n");
	free(sem_value);
}

void initResourcePack(struct resource_pack *pack,
		int space_limit, int num_workers) {
	initSem();
	pack->space_limit  = space_limit;
	pack->num_workers  = num_workers;
	pack->sem_space    = &sem_space   ;
	pack->sem_worker   = &sem_worker  ;

	pack->sem_skeleton = &sem_skeleton;
	pack->sem_engine   = &sem_engine  ;
	pack->sem_chassis  = &sem_chassis ;
	pack->sem_body     = &sem_body    ;

	pack->sem_window   = &sem_window  ;
	pack->sem_tire     = &sem_tire    ;
	pack->sem_battery  = &sem_battery ;
	pack->sem_car      = &sem_car     ;
}

int destroySem(){
#if DEBUG
	printf("Destroying semaphores...\n");
#endif
	sem_destroy(&sem_worker);
	sem_destroy(&sem_space);

	sem_destroy(&sem_skeleton);
	sem_destroy(&sem_engine);
	sem_destroy(&sem_chassis);
	sem_destroy(&sem_body);

	sem_destroy(&sem_window);
	sem_destroy(&sem_tire);
	sem_destroy(&sem_battery);
	sem_destroy(&sem_car);
#if DEBUG
	printf("Semaphores destroyed\n");
#endif
	return 0;
}

int initSem(){
#if DEBUG
	printf("Initiating semaphores...\n");
#endif
	sem_init(&sem_worker,   0, num_workers);
	sem_init(&sem_space,    0, num_spaces);

	sem_init(&sem_skeleton, 0, 0);
	sem_init(&sem_engine,   0, 0);
	sem_init(&sem_chassis,  0, 0);
	sem_init(&sem_body,     0, 0);

	sem_init(&sem_window,   0, 0);
	sem_init(&sem_tire,     0, 0);
	sem_init(&sem_battery,  0, 0);
	sem_init(&sem_car,      0, 0);
#if DEBUG
	printf("Init semaphores done!\n");
#endif
	return 0;
}
