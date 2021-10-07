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
pthread_cond_t mainC, functionC;

void*
threadfn(void *arg) {
    while(1) {
        // lock this critical area
        pthread_mutex_lock(&lock);
        // block mutex to ensure that main called for a signal for functionC
        while(var != WAITING_FOR_SAMPLE) {
            pthread_cond_wait(&functionC, &lock);
        }
        // contintue to here is the signal functionC is sent from main
        if(var == WAITING_FOR_SAMPLE) {
            var = circle();
        }
        // Unlock and signal back to main
        pthread_mutex_unlock(&lock);
        pthread_cond_signal(&mainC);
    }
}


int main(void) {
    uint32_t total_in_arc = 0;
    pthread_t threads[WORKER_COUNT];

    for(int i = 0; i < WORKER_COUNT; i++)  {
        pthread_create(&threads[i], NULL, &threadfn, NULL);
    }

    pthread_mutex_init(&lock, NULL);

	for (uint32_t i = 0; i < NITERS; i++) {
		// Blocking Mutex lock to wait for var to be filled with some sort of data
        // uses signaling and waiting to ask fucntion to send data
		while(var == WAITING_FOR_SAMPLE) {
            pthread_cond_signal(&functionC);
            pthread_cond_wait(&mainC, &lock);
        }
		// check to see if var is in variable
		if (var == INSIDE_CIRCLE) {
			// aggregate the value
			total_in_arc++;
		}
		// Set var back to waiting
		var = WAITING_FOR_SAMPLE;
	}

	/* We multiply by four because circle() uses an 1/4 circular arc */
	printf("The value of pi is: %f\n", (double)total_in_arc / NITERS * 4);
	//pthread_mutex_unlock(&lock);


	return 0;
}
