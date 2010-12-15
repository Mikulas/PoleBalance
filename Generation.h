/*
 *  Generation.h
 *  PoleBalance
 *
 *  Created by Mikulas Dite on 12/15/10.
 *  Copyright 2010 GChD. All rights reserved.
 *
 */



typedef struct Generation Generation;
typedef struct Entity Entity;

struct Entity
{
	double cart_position; // [meters] relative
	double cart_velocity; // [meters/second]
	double cart_acceleration; // [meters/second/second]
	
	double pole_angle; // [radians] relative from top
	double pole_velocity; // [radians/second]
	double pole_acceleration; // [radians/second/second]
	
	/** variables to find */
	double c_cart_position;
	double c_cart_velocity;
	double c_pole_angle;
	double c_pole_velocity;
	
	double force;
	
	int failed;
	
	double fitness;
};

struct Generation
{	
	Entity population[GENERATION_SIZE];
};


double getForceToApply(Entity e)
{
	return force * ((e.c_cart_position * e.cart_position + e.c_cart_velocity * e.cart_velocity + e.c_pole_angle * e.pole_angle + e.c_pole_velocity * e.pole_velocity) > 0 ? 1 : -1);
}

double getPoleAcceleration(Entity e)
{
	return (g_acceleration * sin(e.pole_angle) + cos(e.pole_angle) * ((-e.force - pole_mass * pole_length * e.pole_velocity * e.pole_velocity * sin(e.pole_angle) / (cart_mass + pole_mass)))) / (pole_length * (4/3 - (pole_mass * cos(e.pole_angle) * cos(e.pole_angle)) / (cart_mass + pole_mass)));
}

double getCartAcceleration(Entity e)
{
	return (e.force + pole_mass * pole_length * (e.pole_velocity * e.pole_velocity * sin(e.pole_angle) - e.pole_acceleration * cos(e.pole_angle))) / (cart_mass + pole_mass);
}

double getCartPosition(Entity e)
{
	return e.cart_position + time_step * e.cart_velocity;
}

double getCartVelocity(Entity e)
{
	return e.cart_velocity + time_step * e.cart_acceleration;
}

double getPoleAngle(Entity e)
{
	return mod(e.pole_angle + time_step * e.pole_velocity, 2 * M_PI);
}

double getPoleVelocity(Entity e)
{
	return e.pole_velocity + time_step * e.pole_acceleration;
}



double getEntityFitness(Entity e)
{
	//printf("fitness was %f\n", e.fitness);
	if (e.fitness == 0) {
		for (double t = 0; t < time; t += time_step) {
			e.force = getForceToApply(e);
			e.cart_acceleration = getCartAcceleration(e);
			e.pole_acceleration = getPoleAcceleration(e);
			e.pole_velocity = getPoleVelocity(e);
			e.cart_velocity = getCartVelocity(e);
			e.pole_angle = getPoleAngle(e);
			e.cart_position = getCartPosition(e);
		
			if (e.cart_position * sgn(e.cart_position) >= fail_position) {
				e.failed = 1;
				break;
			}
		}
		/** @todo fixme */
		return -e.failed * (cube(e.pole_angle) * square(e.pole_velocity) * e.pole_acceleration + cube(e.cart_position) * square(e.cart_velocity) * e.cart_acceleration) + power(e.cart_position, 4);
	}
	
	return e.fitness;
}


Entity getBestEntity(Generation gen)
{
	double best_fitness = -DBL_MAX;
	int index = -1;
	
	for (int i = 0; i < GENERATION_SIZE; i++) {
		// gen.population[i].fitness = getEntityFitness(gen.population[i]);
		printf("fitness = %f\n", gen.population[i].fitness);
		if (gen.population[i].fitness > best_fitness) {
			index = i;
			best_fitness = gen.population[i].fitness;
		}
	}
	
	return gen.population[index];
}

