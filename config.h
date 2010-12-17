/*
 *  config.h
 *  PoleBalance
 *
 *  Created by Mikulas Dite on 12/15/10.
 *  Copyright 2010 GChD. All rights reserved.
 *
 */

#define GENERATION_SIZE 60
#define GENERATION_COUNT 200


const double fail_position = 2.4; // [meters]
//const double fail_angle = 12 * M_PI / 180; // [radians] 12 degrees

const double g_acceleration = 9.81; // [meters/second/second]
const double force = 10; // [newtons]

const double pole_length = 0.5; // [meters] relative from pivot

const double pole_mass = 0.1; // [kilograms]
const double cart_mass = 1; // [kilograms]

const double time = 20; // [seconds]
const double time_step = 0.02; // [seconds]
