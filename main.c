
#include <stdio.h>
#include <math.h>
#include <float.h>

#include "config.h"
#include "math.h"
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
	
	printf("Hello, World!\n");
	
	Generation g = getRandomGeneration();
	for (int i = 0; i < GENERATION_COUNT; i++) {
		g = getNextGeneration(g);
	}
	
	Entity solution = getBestEntity(g);

	printf("best solution:\n");
	printf("\tfailed: "); printf(solution.failed == -1 ? "TRUE\n" : "FALSE\n");
	printf("\tfitness: %f\n", solution.fitness);
	printf("\tc_cart_position: %f\n", solution.c_cart_position);
	printf("\tc_cart_velocity: %f\n", solution.c_cart_velocity);
	printf("\tc_pole_angle: %f\n", solution.c_pole_angle);
	printf("\tc_pole_velocity: %f\n", solution.c_pole_velocity);
	
	getchar();
	getchar();
	
    return 0;
}
