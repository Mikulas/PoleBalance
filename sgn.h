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
