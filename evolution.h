/*
 *  Evolution.h
 *  PoleBalance
 *
 *  Created by Mikulas Dite on 12/15/10.
 *  Copyright 2010 GChD. All rights reserved.
 *
 */

Generation getRandomGeneration()
{
	Generation gen;
	for (int i = 0; i < GENERATION_SIZE; i++) {
		gen.population[i].c_cart_position = getRandomSign() * rand() % 100;
		gen.population[i].c_cart_velocity = getRandomSign() * rand() % 100;
		gen.population[i].c_pole_angle = getRandomSign() * rand() % 100;
		gen.population[i].c_pole_velocity = getRandomSign() * rand() % 100;

		gen.population[i].cart_position = 0; // (getRandomSign() * (rand() % fail_position));
		gen.population[i].cart_velocity = 0;
		gen.population[i].cart_acceleration = 0;

		gen.population[i].pole_angle = mod(getRandomSign() * (double) rand() / 10e7, 2 * M_PI);
		gen.population[i].pole_velocity = 0;
		gen.population[i].pole_acceleration = 0;
		
		gen.population[i].failed = 0;
		gen.population[i].force = 0;
		gen.population[i].fitness = 0;
	}
	
	gen.fitness_sum = 0;
	
	return gen;
}


double getEntityOccurenceChance(Generation *gen, Entity *e)
{
	return getEntityFitness(e) / getGenerationFitnessSum(gen);
}



Generation getNextGeneration(Generation *gen)
{
	for (int i = 0; i < GENERATION_SIZE; i++) {
		getEntityOccurenceChance(gen, &gen->population[i]);
		//y(&gen.population[i]);
	}
	Generation nextGen;
	
	
	for (int i = 0; i < GENERATION_SIZE; i++) {
		gen->population[i].fitness = getEntityFitness(&gen->population[i]);
		nextGen.population[i] = gen->population[i];
		nextGen.population[i].fitness = 0;
	}
	/**
	 * get rulette wheel
	 * look to kindle book how should I pair them
	 * add mutation ?
	 * add permutation ?
	 * replace few in the middle to perserve best in the gen
	 */
	
	return nextGen;
}
