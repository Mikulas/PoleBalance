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


double getForceToApply(Entity *e)
{
	return force * ((e->c_cart_position * e->cart_position + e->c_cart_velocity * e->cart_velocity + e->c_pole_angle * e->pole_angle + e->c_pole_velocity * e->pole_velocity) > 0 ? 1 : -1);
}

double getPoleAcceleration(Entity *e)
{
	return (g_acceleration * sin(e->pole_angle) + cos(e->pole_angle) * ((-e->force - pole_mass * pole_length * e->pole_velocity * e->pole_velocity * sin(e->pole_angle) / (cart_mass + pole_mass)))) / (pole_length * (4/3 - (pole_mass * cos(e->pole_angle) * cos(e->pole_angle)) / (cart_mass + pole_mass)));
}

double getCartAcceleration(Entity *e)
{
	return (e->force + pole_mass * pole_length * (e->pole_velocity * e->pole_velocity * sin(e->pole_angle) - e->pole_acceleration * cos(e->pole_angle))) / (cart_mass + pole_mass);
}

double getCartPosition(Entity *e)
{
	return e->cart_position + time_step * e->cart_velocity;
}

double getCartVelocity(Entity *e)
{
	return e->cart_velocity + time_step * e->cart_acceleration;
}

double getPoleAngle(Entity *e)
{
	return mod(e->pole_angle + time_step * e->pole_velocity, 2 * M_PI);
}

double getPoleVelocity(Entity *e)
{
	return e->pole_velocity + time_step * e->pole_acceleration;
}


void printEntity(Entity *e)
{
	printf("---\n");
	printf("\tfailed: "); printf(e->failed == 1 ? "TRUE\n" : "FALSE\n");
	printf("\tfitness: %f\n", e->fitness);
	printf("\tc_cart_position: %f\n", e->c_cart_position);
	printf("\tc_cart_velocity: %f\n", e->c_cart_velocity);
	printf("\tc_pole_angle: %f\n", e->c_pole_angle);
	printf("\tc_pole_velocity: %f\n", e->c_pole_velocity);
	printf("\tpole_angle: %f\n", e->pole_angle);
	printf("\tpole_velocity: %f\n", e->pole_velocity);
	printf("\tpole_acceleration: %f\n", e->pole_acceleration);
	printf("\tcart_position: %f\n", e->cart_position);
	printf("\tcart_velocity: %f\n", e->cart_velocity);
	printf("\tcart_acceleration: %f\n", e->cart_acceleration);
}



double getEntityFitness(Entity *e)
{
	if (e->fitness == 0) {
		for (double t = 0; t < time; t += time_step) {
			e->force = getForceToApply(e);
			e->cart_acceleration = getCartAcceleration(e);
			e->pole_acceleration = getPoleAcceleration(e);
			e->pole_velocity = getPoleVelocity(e);
			e->cart_velocity = getCartVelocity(e);
			e->pole_angle = getPoleAngle(e);
			e->cart_position = getCartPosition(e);
		
			//printEntity(e);
			if (e->cart_position * sgn(e->cart_position) >= fail_position) {
				e->failed = 1;
				break;
			}
		}
		/** @todo fixme */
		e->fitness = -e->failed * abs(cube(e->pole_angle) * square(e->pole_velocity) * e->pole_acceleration + 0.2 * cube(e->cart_position) * square(e->cart_velocity) * e->cart_acceleration);
	}
	
	return e->fitness;
}


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

