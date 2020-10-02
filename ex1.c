#include "circle.h"

/* shared variable for the threads to update */
volatile int var = -1;

int
main(void)
{
	/* number of samples */
	int niters = 10000000;
	int count = 0;
	int i = 0;

	for (; i < niters; i++) {
		if (circle() == 1)
			count++;
	}

	printf("The value of pi is: %f\n", (double)count / niters * 4);

	return 0;
}
