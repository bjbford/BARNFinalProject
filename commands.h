/*
 * commands.h
 *
 *  Created on: Apr 12, 2017
 *      Author: bjbford
 */

#ifndef COMMANDS_H_
#define COMMANDS_H_

#include <math.h>
#include <inc/tm4c123gh6pm.h>

#define M_PI 3.14159265358979323846

struct object{
	float width;
	float distance;
	int count;
	int location;
};

void sweep();

#endif /* COMMANDS_H_ */
