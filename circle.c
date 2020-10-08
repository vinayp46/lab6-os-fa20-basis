#include "circle.h"
#include <stdint.h>

/**
 * Generates a random coordinate on a 1x1 plane and returns true if it lies within the circular arc intersecting [0,1] and [1,0]
 * @return 1 if in circle (technically an arc), 0 if not
 * 
 * High School Math Recap:
 * 
 * Imagine a quadrant of size [0,1] on the X and Y axis, 
 * A circular arc of radius 1 intersects the axes at [0,1] and [1,0].
 * 
 * We generate a random coordinate between [0,0] and [1,1], and can caculate the distance from the origin
 * via the Pythogorean Theorem.
 *  
 * If this distance is less than or equal to the radius of the circle (1), then it landed in the circle
 * 
 * This arc is 1/4 of a circle
 * 
 * 1 |..
 *   |  ``..
 *   |  /|  `.
 *   | / |    .
 *   |/  |     :
 * 0 ------------
 *              1
 */
int circle(void)
{
	/* Software is generally more robust when defined using constants */
	const double x = (double)rand() / RAND_MAX;
	const double y = (double)rand() / RAND_MAX;

	/* Technically, x^2 + y^2 <= 1^2, but 1^2 simplifies to 1 */
	return x * x + y * y <= 1;
}
