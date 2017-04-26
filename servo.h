/**
*	@file servo.h
*	@brief This file contains the methods necessary to operate the servo.
*	@author	Brian Bradford, Rajiv Bhoopala, Andrew Thai, Nick Knuth
*	@date 3/8/2017
*/

#ifndef SERVO_H_
#define SERVO_H_

#include "timer.h"
#include <inc/tm4c123gh6pm.h>

//period in seconds * clock (16MHz)
//volatile unsigned top = 320000; // pulse period in cycles (20 ms)
volatile signed pulse_width;

/**
*	This method initiates servo operation.
*	@author Brian Bradford, Rajiv Bhoopala, Andrew Thai, Nick Knuth
*	@date 3/8/2017
*/
void servo_init();

/**
*	This method moves the servo.
*	@author	Brian Bradford, Rajiv Bhoopala, Andrew Thai, Nick Knuth
*	@param degree	The degree to move the servo to
*	@date 3/8/2017
*/
void move_servo(int degree);

#endif /* SERVO_H_ */
