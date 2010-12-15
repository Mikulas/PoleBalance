/*
 *  sgn.h
 *  PoleBalance
 *
 *  Created by Mikulas Dite on 12/15/10.
 *  Copyright 2010 GChD. All rights reserved.
 *
 */


int sgn(double value)
{
	if (value == 0) {
		return 0;
	}
	
	return value > 0 ? 1 : -1;
}


double abs(double value)
{
	return value * sgn(value);
}


double power(double base, int power)
{
	double res = 1;
	for (int i = 0; i < power; i++) {
		res *= base;
	}
	return res;
}



double square(double value)
{
	return power(value, 2);
}



double cube(double value)
{
	return power(value, 3);
}



int getRandomSign()
{
	return (rand() % 100) > 50 ? 1 : -1;
}


double mod(double value, double divider)
{
	double res = value / divider;
	return value - ((int) res * divider);
}
