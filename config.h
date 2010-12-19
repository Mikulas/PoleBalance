/*
 *  config.h
 *  PoleBalance
 *
 *  Created by Mikulas Dite on 12/15/10.
 *  Copyright 2010 GChD. All rights reserved.
 *
 */

#define GENERATION_SIZE 50
#define GENERATION_COUNT 100


const double fail_position = 2.4; // [meters]

const double g_acceleration = -9.81; // [meters/second/second]
const double force = 10; // [newtons]

const double pole_length = 0.5; // [meters] relative from pivot

const double pole_mass = 0.1; // [kilograms]
const double cart_mass = 1; // [kilograms]

const double time = 20; // [seconds]
const double time_step = 0.025; // [seconds]
