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
};

struct Generation
{	
	Entity population[GENERATION_SIZE];
};
