/**
*	@file ping.h
*	@brief This file contains the methods necessary to operate and collect data from the ping sensor.
*	@author Brian Bradford, Rajiv Bhoopala, Andrew Thai, Nick Knuth
*	@date 3/1/2017
*/

#ifndef PING_H_
#define PING_H_

#include <inc/tm4c123gh6pm.h>
#include <stdint.h>
#include <stdbool.h>
#include "Timer.h"
#include "driverlib/interrupt.h"


#define BIT0		0x01
#define BIT1		0x02
#define BIT2		0x04
#define BIT3		0x08
#define BIT4		0x10
#define BIT5		0x20
#define BIT6		0x40
#define BIT7		0x80

#define PING_FACTOR	930.24f //Factor to get distance in cm

volatile enum {RISE, FALL, DONE} state;
volatile uint32_t risingEdge;
volatile uint32_t fallingEdge;
volatile uint32_t pulseLength;

/**
*	This method captures the time from when the sensor sends a pulse and it returns.
*	@author	Brian Bradford, Rajiv Bhoopala, Andrew Thai, Nick Knuth
*	@date 3/1/2017
*/
void TIMER3B_Handler(void);

/**
*	This method initiates the ping sensor.
*	@author Brian Bradford, Rajiv Bhoopala, Andrew Thai, Nick Knuth
*	@date 3/1/2017
*/
void ping_init();

/**
*	This method operates the ping sensor.
*	@author Brian Bradford, Rajiv Bhoopala, Andrew Thai, Nick Knuth
*	@date 3/1/2017
*/
void send_pulse();

/**
*	This method collects the ping sensor data.
*	@author Brian Bradford, Rajiv Bhoopala, Andrew Thai, Nick Knuth
*	@date 3/1/2017
*/
float ping_getDistance();

#endif /* PING_H_ */
