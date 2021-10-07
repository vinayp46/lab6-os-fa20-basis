#include <stdint.h>
#include <pthread.h>

#include "circle.h"

#define WAITING_FOR_SAMPLE -1
#define OUTSIDE_CIRCLE 0
#define INSIDE_CIRCLE 1

#define WORKER_COUNT 5
#define NITERS 10000

/* shared variable for the threads to update */
volatile int var = WAITING_FOR_SAMPLE;
pthread_mutex_t lock;


void*
threadfn(void *arg) {
	while(1) {
		while(var != WAITING_FOR_SAMPLE);
		pthread_mutex_lock(&lock);
		if(var == WAITING_FOR_SAMPLE) {
			var = circle();
		}
		pthread_mutex_unlock(&lock);
	}
}


int main(void)
{
	uint32_t total_in_arc = 0;
	pthread_t threads[WORKER_COUNT];

	for(int i = 0; i < WORKER_COUNT; i++)  {
		pthread_create(&threads[i], NULL, &threadfn, NULL);
	}

  pthread_mutex_init(&lock, NULL);

	for (uint32_t i = 0; i < NITERS; i++) {
		// Spin lock to wait for var to be filled with some sort of data
		while(var == WAITING_FOR_SAMPLE);
		// Now that we have data, take the lock
		pthread_mutex_lock(&lock);
		// check to see if var is in variable
		if (var == INSIDE_CIRCLE) {
			// aggregate the value
			total_in_arc++;
		}
		// Set var back to waiting
		var = WAITING_FOR_SAMPLE;
		pthread_mutex_unlock(&lock);
	}

	/* We multiply by four because circle() uses an 1/4 circular arc */
	printf("The value of pi is: %f\n", (double)total_in_arc / NITERS * 4);


	return 0;
}
