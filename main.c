
#include <stdio.h>
#include <math.h>
#include <limits.h>
#include <float.h>
#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

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

	srand(time(NULL));
	
	printf("Pole Balance\n");

	Generation g = getRandomGeneration();

	printf("\n");
	
	FILE *stream;
	char output_file[254];
	sprintf(output_file, "average_fitness.%i.txt", GENERATION_SIZE);
	stream = fopen(output_file, "wt");
	
	int i;
	for (i = 0; i < GENERATION_COUNT; i++) {
		printf("\rgenerating %0.2f%%", 100 * ((float) i + 1) / (float) GENERATION_COUNT);
		if (i % 5 == 0 && i == 20) {
			Entity best = getBestEntity(&g);
			writeEntity(&best, i, "-best");
			Entity worst = getWorstEntity(&g);
			writeEntity(&worst, i, "-worst");
		}
		fflush(stdout);
		g = getNextGeneration(&g);
		fprintf(stream, "%i\t%f\n", i + 1, getGenerationAverageFitness(&g));
	}
	fclose(stream);

	printf("\nbest solution:\n");
	Entity solution = getBestEntity(&g);
	printEntity(&solution);
	writeEntity(&solution, i, "-best");
	
	Entity worst = getWorstEntity(&g);
	writeEntity(&worst, i, "-worst");
    return 0;
}
