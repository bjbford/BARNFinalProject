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
#include "open_interface.h"

#define M_PI 3.14159265358979323846

struct object{
	float width;
	float distance;
	int count;
	int location;
};

void sweep();
void init_All();
void move_roomba();
void move_forward(oi_t *sensor, int centimeters);
void move_backwards(oi_t *sensor, int centimeters);
void turn_counterClockwise(oi_t *sensor, int degrees);
void turn_clockwise(oi_t *sensor, int degrees);
int checkBumper(oi_t *sensor);
int checkCliff(oi_t *sensor);
int cliffWarning(oi_t *sensor);

#endif /* COMMANDS_H_ */
