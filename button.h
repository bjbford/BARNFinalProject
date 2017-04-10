/*
 * button.h
 *
 *  Created on: Jul 18, 2016
 *      Author: Eric Middleton
 */

#ifndef BUTTON_H_
#define BUTTON_H_

#include <stdint.h>

#include <inc/tm4c123gh6pm.h>

//initialize the push buttons
void button_init();

//Non-blocking call
//Returns 8-bit integer with bits 0:5 indicating if button is pressed (set)
//	 or not (cleared)
uint8_t button_checkButtons();

///Non-blocking call
///Returns highest value button being pressed, 0 if no button pressed
uint8_t button_getButton();

//Blocking call
//Returns highest value button being pressed, waits until at least one button is pressed
uint8_t button_getButtonBlocking();


int8_t button_getButtonChange();

int8_t button_getButtonChangeBlocking();


#endif /* BUTTON_H_ */
