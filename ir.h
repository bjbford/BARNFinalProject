/*
 * ir.h
 *
 *  Created on: 2-22-2017
 *      Author: Brian Bradford and Rajiv Bhoopala
 */

#ifndef IR_H_
#define IR_H_

#include <stdio.h>
#include <stdint.h>
#include <inc/tm4c123gh6pm.h>

void ir_init();
unsigned ir_read();
float ir_getDistance();

volatile unsigned isr_int_value;

#endif /* IR_H_ */
