/**
*  @file ir.h
*  @brief this file will contain all basic ir functions.
*
*  @author	Brian Bradford, Nick Knuth, Andrew Thai, and Rajiv Bhoopala
*
*  @date	2/22/2017
*/

#ifndef IR_H_
#define IR_H_

#include <stdio.h>
#include <stdint.h>
#include <inc/tm4c123gh6pm.h>

/**
* 	This method initializes and sets up ADC and raw interrupt polling for ir.
* 	@author	Brian Bradford, Nick Knuth, Andrew Thai, and Rajiv Bhoopala
* 	@date	2/22/2017
*/
void ir_init();

/**
* 	This method using polling to grab ir data from FIFO register.
* 	@author	Brian Bradford, Nick Knuth, Andrew Thai, and Rajiv Bhoopala
* 	@date	2/22/2017
*/
unsigned ir_read();


/**
* 	This method returns an average ir distance from 100 samples.
* 	@author	Brian Bradford, Nick Knuth, Andrew Thai, and Rajiv Bhoopala
* 	@date	2/22/2017
*/
float ir_getDistance();

volatile unsigned isr_int_value;

#endif /* IR_H_ */
