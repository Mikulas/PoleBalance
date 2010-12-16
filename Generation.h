/*
 *  Generation.h
 *  PoleBalance
 *
 *  Created by Mikulas Dite on 12/15/10.
 *  Copyright 2010 GChD. All rights reserved.
 *
 */

typedef struct Generation Generation;

struct Generation
{	
	Entity population[GENERATION_SIZE];
};



Entity getBestEntity(Generation gen)
{
	double best_fitness = -DBL_MAX;
	int index = -1;
	
	for (int i = 0; i < GENERATION_SIZE; i++) {
		// gen.population[i].fitness = getEntityFitness(gen.population[i]);
		if (gen.population[i].fitness > best_fitness) {
			index = i;
			best_fitness = gen.population[i].fitness;
		}
	}
	
	return gen.population[index];
}
