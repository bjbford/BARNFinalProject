/*
 * ping.h
 *
 *  Created on: 3-1-2017
 *      Author: Brian Bradford and Rajiv Bhoopala
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

void ping_init(void);

void send_pulse(void);

float ping_getDistance(void);

#endif /* PING_H_ */
