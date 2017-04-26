/*
 * servo.h
 *
 *  Created on: Mar 8, 2017
 *      Author: Brian Bradford and Rajiv Bhoopala
 */

#ifndef SERVO_H_
#define SERVO_H_

#include "timer.h"
#include <inc/tm4c123gh6pm.h>

//period in seconds * clock (16MHz)
//volatile unsigned top = 320000; // pulse period in cycles (20 ms)
volatile signed pulse_width;

void servo_init();
void move_servo(int degree);

#endif /* SERVO_H_ */
