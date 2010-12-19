/*
 *  Entity.h
 *  PoleBalance
 *
 *  Created by Mikulas Dite on 12/16/10.
 *  Copyright 2010 GChD. All rights reserved.
 *
 */

typedef struct Entity Entity;

struct Entity
{
	double cart_position; // [meters] relative
	double cart_velocity; // [meters/second]
	double cart_acceleration; // [meters/second/second]
	
	double pole_angle; // [radians] relative from top
	double pole_angle_origin; // [radians]
	double pole_velocity; // [radians/second]
	double pole_acceleration; // [radians/second/second]
	
	/** variables to find */
	long int c_cart_position;
	long int c_cart_velocity;
	long int c_pole_angle;
	long int c_pole_velocity;
	
	double force;
	
	int failed;
	
	double fitness;
	double limit_min;
	double limit_max;
};



Entity getNewEntity()
{
	Entity e;
	e.c_cart_position = e.c_cart_velocity = e.c_pole_angle = e.c_pole_velocity = 0;
	
	e.cart_position = 0; // (getRandomSign() * (rand() % fail_position));
	e.cart_velocity = 0;
	e.cart_acceleration = 0;
	
	e.pole_angle = e.pole_angle_origin = M_PI;//mod(getRandomSign() * (double) rand() / 10e7, 2 * M_PI);
	e.pole_velocity = 0;
	e.pole_acceleration = 0;
	
	e.failed = 0;
	e.force = 0;
	
	e.fitness = 0;
	e.limit_min = -1;
	e.limit_max = -1;
	return e;
}



void printEntity(Entity *e)
{
	printf("---\n");
	printf("\tfailed: "); printf(e->failed == 1 ? "TRUE\n" : "FALSE\n");
	printf("\tfitness: %f\n", e->fitness);
	printf("\tc_cart_position: %ld\n", e->c_cart_position);
	printf("\tc_cart_velocity: %ld\n", e->c_cart_velocity);
	printf("\tc_pole_angle: %ld\n", e->c_pole_angle);
	printf("\tc_pole_velocity: %ld\n", e->c_pole_velocity);
	printf("\tpole_angle: %f\n", e->pole_angle);
	printf("\tpole_angle_origin: %f\n", e->pole_angle_origin);
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
			/**
			 * @see http://www.profjrwhite.com/system_dynamics/sdyn/s7/s7invp2/s7invp2.html
			 */
			e->pole_angle = (e->pole_angle + time_step * e->pole_velocity);
			e->cart_position = e->cart_position + time_step * e->cart_velocity;
			
			e->force = force * sgn(e->c_cart_position * e->cart_position + e->c_cart_velocity * e->cart_velocity + e->c_pole_angle * e->pole_angle + e->c_pole_velocity * e->pole_velocity);
			e->cart_acceleration = (e->force + pole_mass * pole_length * square(sin(e->pole_angle)) - pole_mass * g_acceleration * cos(e->pole_angle) * sin(e->pole_angle)) / (cart_mass + pole_mass - pole_mass * square(cos(e->pole_angle)));
			e->pole_acceleration = (e->force * cos(e->pole_angle) - g_acceleration * (pole_mass + cart_mass) * sin(e->pole_angle) + pole_mass * pole_length * cos(e->pole_angle) * sin(e->pole_angle) * e->pole_velocity) / (pole_mass * pole_length * square(cos(e->pole_angle)) - (pole_mass + cart_mass) * pole_length);
			
			e->pole_velocity = e->pole_velocity - time_step * e->pole_acceleration;
			e->cart_velocity = e->cart_velocity + time_step * e->cart_acceleration;
			
			//printEntity(e);
			if (e->cart_position * sgn(e->cart_position) >= fail_position) {
				e->failed = 1;
				break;
			}
		}
		
		/**
		 * abs(): direction does not matter
		 * mod(): base value of angle
		 */
		e->pole_angle = mod(abs(e->pole_angle), 2 * M_PI);
		if (e->pole_angle > M_PI) {
			e->pole_angle = 2 * M_PI - e->pole_angle;
		}
		
		e->fitness = - 15 * abs(e->pole_angle) - 2 * abs(e->pole_velocity) - 10 * abs(e->cart_position) - 0.1 * abs(e->cart_velocity);
	}
	
	return e->fitness;
}



void writeEntity(Entity *f, int generation)
{
	Entity* e = f;
	e->pole_velocity = 0;
	e->pole_acceleration = 0;
	e->cart_position = 0;
	e->cart_velocity = 0;
	e->cart_acceleration = 0;
	e->pole_angle = f->pole_angle_origin;

	FILE *stream;
	
	stream = fopen("header.dat", "wt");
	fprintf(stream, "generation fitness k l m n time_step fail_position\n");
	fprintf(stream, "%d %f %ld %ld %ld %ld %f %f\n", generation, getEntityFitness(e), e->c_cart_position, e->c_cart_velocity, e->c_pole_angle, e->c_pole_velocity, time_step, fail_position);
	fclose(stream);
	
	stream = fopen("movement.dat", "wt");
	fprintf(stream, "cart_position pole_angle\n");
	for (double t = 0; t < time; t += time_step) {
		
		/** @todo fixme this is just copy and paste of getFitness */
		e->pole_angle = (e->pole_angle + time_step * e->pole_velocity);
		e->cart_position = e->cart_position + time_step * e->cart_velocity;
		
		e->force = force * sgn(e->c_cart_position * e->cart_position + e->c_cart_velocity * e->cart_velocity + e->c_pole_angle * e->pole_angle + e->c_pole_velocity * e->pole_velocity);
		e->cart_acceleration = (e->force + pole_mass * pole_length * square(sin(e->pole_angle)) - pole_mass * g_acceleration * cos(e->pole_angle) * sin(e->pole_angle)) / (cart_mass + pole_mass - pole_mass * square(cos(e->pole_angle)));
		e->pole_acceleration = (e->force * cos(e->pole_angle) - g_acceleration * (pole_mass + cart_mass) * sin(e->pole_angle) + pole_mass * pole_length * cos(e->pole_angle) * sin(e->pole_angle) * e->pole_velocity) / (pole_mass * pole_length * square(cos(e->pole_angle)) - (pole_mass + cart_mass) * pole_length);
		
		e->pole_velocity = e->pole_velocity - time_step * e->pole_acceleration;
		e->cart_velocity = e->cart_velocity + time_step * e->cart_acceleration;
		
		if (e->pole_angle < M_PI / 2 || e->pole_angle > 3 * M_PI / 2) {
			//printf("\nDOES NOT WORK\n");
			//exit(5);
		}
		
		if (abs(e->cart_position) > fail_position) {
			e->cart_position = sgn(e->cart_position) * fail_position;
		}
		fprintf(stream, "%f %f %s\n", e->cart_position, e->pole_angle, sgn(e->force) == -1 ? "left" : "right");
		//printEntity(e);
		if (e->cart_position * sgn(e->cart_position) >= fail_position) {
			e->failed = 1;
			break;
		}
	}
	fclose(stream);
}
