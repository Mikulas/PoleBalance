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
		gen.population[i] = getNewEntity();
		gen.population[i].c_cart_position = getRandomSign() * rand() % 100;
		gen.population[i].c_cart_velocity = getRandomSign() * rand() % 100;
		gen.population[i].c_pole_angle = getRandomSign() * rand() % 100;
		gen.population[i].c_pole_velocity = getRandomSign() * rand() % 100;		
	}
	
	gen.fitness_sum = 0;
	
	return gen;
}



double getEntityOccurenceChance(Generation *gen, Entity *e)
{
	return getEntityFitness(e) / getGenerationFitnessSum(gen);
}



Entity getWeightedEntity(Generation *gen)
{
	/** roulette wheel method */
	double limit_min = 0;
	for (int i = 0; i < GENERATION_SIZE; i++) {
		gen->population[i].limit_min = limit_min;
		gen->population[i].limit_max = limit_min + getEntityOccurenceChance(gen, &gen->population[i]);
		limit_min = gen->population[i].limit_max;
	}
	double roll = mod((double) rand() / 10e8, 1);
	for (int i = 0; i < GENERATION_SIZE; i++) {
		if (roll >= gen->population[i].limit_min && roll < gen->population[i].limit_max) {
			return gen->population[i]; 
		}
	}
}



Entity getMerge(Entity *e, Entity *f)
{
	Entity merge = getNewEntity();
	
	merge.c_cart_position = (e->c_cart_position + f->c_cart_position) / 2;
	merge.c_cart_velocity = (e->c_cart_velocity + f->c_cart_velocity) / 2;
	merge.c_pole_angle = (e->c_pole_angle + f->c_pole_angle) / 2;
	merge.c_pole_velocity = (e->c_pole_velocity + f->c_pole_velocity) / 2;
	
	return merge;
}



Generation getNextGeneration(Generation *gen)
{
	Generation nextGen;
	
	for (int i = 0; i < GENERATION_SIZE; i += 2) {
		Entity e = getWeightedEntity(gen);
		Entity f = getWeightedEntity(gen);
		
		nextGen.population[i] = getMerge(&e, &f);
		nextGen.population[i + 1] = getMerge(&f, &e);
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
