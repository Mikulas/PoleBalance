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
	double fitness_sum;
};



double getGenerationFitnessSum(Generation *gen)
{
	double sum = 0;
	if (gen->fitness_sum == 0) {
		for (int i = 0; i < GENERATION_SIZE; i++) {
			sum += getEntityFitness(&gen->population[i]);
		}
		gen->fitness_sum = sum;
	}
	
	return gen->fitness_sum;
}



Entity getBestEntity(Generation *gen)
{
	double best_fitness = -DBL_MAX;
	int index = -1;
	
	for (int i = 0; i < GENERATION_SIZE; i++) {
		if (getEntityFitness(&gen->population[i]) > best_fitness) {
			index = i;
			best_fitness = getEntityFitness(&gen->population[i]);
		}
	}
	
	return gen->population[index];
}
