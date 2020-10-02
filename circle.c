#include "circle.h"

int
circle(void)
{
	double x, y, z;

	x = (double)rand() / RAND_MAX;
	y = (double)rand() / RAND_MAX;

	z = x * x + y * y;

	if (z > 1)
		return 0;
	else
		return 1;
}
