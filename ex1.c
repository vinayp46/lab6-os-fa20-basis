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

int main(void)
{
	uint32_t total_in_arc = 0;

	for (uint32_t i = 0; i < NITERS; i++)
	{
		if (circle() == INSIDE_CIRCLE)
		{
			total_in_arc++;
		}
	}

	/* We multiply by four because circle() uses an 1/4 circular arc */
	printf("The value of pi is: %f\n", (double)total_in_arc / NITERS * 4);

	return 0;
}
