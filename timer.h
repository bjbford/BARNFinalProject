/*
 * Timer.hpp
 *
 *  Created on: Mar 3, 2016
 *      Author: ericm
 */

#ifndef TIMER_H_
#define TIMER_H_

#include <stdint.h>

#define BIT0		0x01
#define BIT1		0x02
#define BIT2		0x04
#define BIT3		0x08
#define BIT4		0x10
#define BIT5		0x20
#define BIT6		0x40
#define BIT7		0x80

typedef uint32_t clock_t;

#include <inc/tm4c123gh6pm.h>
#include <stdint.h>

extern volatile uint32_t _timer_ticks;

void timer_waitMillis(uint32_t millis);

void timer_waitMicros(uint16_t micros);

void timer_startTimer(uint16_t startValue);

void timer_stopTimer(void);

clock_t timer_startClock(void);

clock_t timer_getClock(void);

void timer_stopClock(void);


#endif /* TIMER_H_ */
