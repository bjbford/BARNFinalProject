/**
*	@file button.h
*	@brief this header file contains all required definitions
*	and basic button functions.
*
*	@author Eric Middleton
*	@date 7/18/2016
*/

#ifndef BUTTON_H_
#define BUTTON_H_

#include <stdint.h>

#include <inc/tm4c123gh6pm.h>

/**
*	Initialize PORTE and configure bits 5-0 to be used as inputs.
*	@author Eric Middleton
*	@date 7/18/2016
*/
void button_init();

/**
* 	This method returns a 6-bit bit field, representing the push buttons,
* 	a 1 means the corresponding button is pressed.
*	@author Eric Middleton
*	@date 7/18/2016
*/
uint8_t button_checkButtons();

/**
*	This method returns the position of the leftmost button being pushed,
*	a 6 is the leftmost button, 1 is the rightmost button.
*	@author Eric Middleton
*	@date 7/18/2016
*/
uint8_t button_getButton();

/**
*	This method returns highest button pressed, waits until at least one is pressed.
*	@author Eric Middleton
*	@date 7/18/2016
*/
uint8_t button_getButtonBlocking();

/**
*	This method debounces the button and returns button press.
*	@author Eric Middleton
*	@date 7/18/2016
*/
int8_t button_getButtonChange();

/**
*	This method blocks the button and returns debounces button press.
*	@author Eric Middleton
*	@date 7/18/2016
*/
int8_t button_getButtonChangeBlocking();


#endif /* BUTTON_H_ */
