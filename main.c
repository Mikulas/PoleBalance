
#include <stdio.h>
#include <math.h>
#include <limits.h>
#include <float.h>

#include "config.h"
#include "math.h"
#include "Entity.h"
#include "Generation.h"
#include "evolution.h"


/**
 * @author Mikuláš Dítě
 * @see http://www.scribd.com/doc/22946879/The-Pole-Balancing-Problem-A-Benchmark-Control-Theory-Problem
 */

int main (int argc, const char * argv[])
{
	/**
	 * The pole must remain upright within ±r the pole failure angle
	 * The cart must remain within ±h of origin
	 * The controller must always exert a non-zero force F
	 */
	
	/**
	 * The movement of cart and pivot is completely frictionless
	 * The electrical system for response is instantaneous
	 * The wheels of cart do not slip
	 * The motor torque limit is not encountered
	 */
	
	printf("Pole Balance\n");

	Generation g = getRandomGeneration();

	printf("\n");
	
	int i;
	for (i = 0; i < GENERATION_COUNT; i++) {
		printf("generating %d out of %d\n", i + 1, GENERATION_COUNT);
		g = getNextGeneration(&g);
	}
	
	Entity solution = getBestEntity(&g);

	printf("best solution:\n");
	printEntity(&solution);
	writeEntity(&solution, i);
	
    return 0;
}
