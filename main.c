
#include <stdio.h>
#include <math.h>

#include "config.h"
#include "sgn.h"
#include "Generation.h"


/**
 * @author Mikuláš Dítě
 * @see http://www.scribd.com/doc/22946879/The-Pole-Balancing-Problem-A-Benchmark-Control-Theory-Problem
 */


double getPoleMotion(Entity e)
{
	return (g_acceleration * sin(e.pole_angle) + cos(e.pole_angle) * ((-force - pole_mass * pole_length * e.pole_velocity * e.pole_velocity * sin(e.pole_angle) / (cart_mass + pole_mass)))) / (pole_length * (4/3 - (pole_mass * cos(e.pole_angle) * cos(e.pole_angle)) / (cart_mass + pole_mass)));
}

double getCartMotion(Entity e)
{
	return (force + pole_mass * pole_length * (e.pole_velocity * e.pole_velocity * sin(e.pole_angle) - e.pole_acceleration * cos(e.pole_angle))) / (cart_mass + pole_mass);
}

double getNewCartPosition(Entity e)
{
	return e.cart_position + time_step * e.cart_velocity;
}

double getNewCartVelocity(Entity e)
{
	return e.cart_velocity + time_step * e.cart_acceleration;
}

double getForceToApply(Entity e)
{
	return force * sgn(e.c_cart_position * e.cart_position + e.c_cart_velocity * e.cart_velocity + e.c_pole_angle * e.pole_angle + e.c_pole_velocity * e.pole_velocity);
}



int main (int argc, const char * argv[])
{
	
	/**
	 * The pole must remain upright within ±r the pole failure angle
	 * The cart must remain within ±h of origin
	 * The controller must always exert a non-zero force F
	 */
	
	/**
	 * The movement of cart and pivot is completely frictionless
	 * The electrical system for response is instantaneous
	 * The wheels of cart do not slip
	 * The motor torque limit is not encountered
	 */
	
	// insert code here...
    printf("Hello, World!\n");
    return 0;
}
