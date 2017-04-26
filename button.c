/*
 * button.c
 *
 *  Created on: Jul 18, 2016
 *      Author: Eric Middleton, Zhao Zhang, Chad Nelson, & Zachary Glanz.
 */

//The buttons are on PORTE 0:5
#include "button.h"

#define BIT0		0x01
#define BIT1		0x02
#define BIT2		0x04
#define BIT3		0x08
#define BIT4		0x10
#define BIT5		0x20
#define BIT6		0x40
#define BIT7		0x80

#define BUTTON_PORT		GPIO_PORTE_DATA_R

static uint8_t _prevButton = 0; //must be set yourself in button_getButton()

/**
 * Initialize PORTE and configure bits 5-0 to be used as inputs.
 */
void button_init() {
	static uint8_t initialized = 0;

	//Check if already initialized
	if(initialized)
		return;

	//Turn on PORTE system clock
	SYSCTL_RCGCGPIO_R |= BIT4;

	//Set the buttons to inputs and enable
	GPIO_PORTE_DIR_R &= ~(BIT6 - 1); //Clear bits 5:0
	GPIO_PORTE_DEN_R |= (BIT6 - 1);

	initialized = 1;
}

/**
 * returns a 6-bit bit field, representing the push buttons. A 1 means the corresponding button is pressed.
 */
uint8_t button_checkButtons() {
	return (~BUTTON_PORT) & (BIT6 - 1); //Return the button status
}

/**
 * Returns the position of the leftmost button being pushed.
 * @return the position of the leftmost button being pushed. A 6 is the leftmost button, 1 is the rightmost button.  0 indicates no button being pressed
 */
uint8_t button_getButton() {

//	#warning "Unimplemented function: uint8_t button_getButton(void)"	// delete warning after implementing

	//
	// DELETE ME - How bitmasking works
	// ----------------------------------------
	// In embedded programming, often we only care about one or a few bits in a piece of 
	// data.  There are several bitwise operators that we can apply to data in order
	// to "mask" the bits that we don't care about.
	//
	//	| = bitwise OR		& = bitwise AND		^ = bitwise XOR		~ = bitwise NOT
	//		  << x = shift left by x bits		 >> x = shift right by x bits 
	//
	// Let's say we want to know if push button 3 (SW3) of GPIO_PORTE_DATA_R is
	// pushed.  Since push buttons are high (1) initially, and low (0) if pushed, PORTE should
	// look like:
	// GPIO_PORTE_DATA_R => 0b???? ?0?? if SW3 is pushed
	// GPIO_PORTE_DATA_R => 0b???? ?1?? if SW3 is not pushed
	//
	// This is not useful: There are 128 different 8 bit numbers that have the 3rd bit high or low.
	// We can make it more clear if we mask the other 7 bits:
	//	
	// Bitwise AND:
	// (GPIO_PORTE_DATA_R & 0b0000 0100) => 0b0000 0000 if SW3 is pushed
	// (GPIO_PORTE_DATA_R & 0b0000 0100) => 0b0000 0100 if SW3 is not pushed
	//
	// Bitwise OR:
	// (GPIO_PORTE_DATA_R | 0b1111 1011) => 0b1111 1011 if SW3 is pushed
	// (GPIO_PORTE_DATA_R | 0b1111 1011) => 0b1111 1111 if SW3 is not pushed
	//
	// Other techniques (Shifting and bitwise AND)
	// ((GPIO_PORTE_DATA_R >> 2) & 1) => 0 if SW3 is pushed
	// ((GPIO_PORTE_DATA_R >> 2) & 1) => 1 if SW3 is not pushed

	// INSERT CODE HERE!
	
	if(!((GPIO_PORTE_DATA_R >> 5) & 1)){ //if SW6 is pushed
		_prevButton = 6;
		return 6;
	}
	else if(!((GPIO_PORTE_DATA_R >> 4) & 1)){ //if SW5 is pushed
		_prevButton = 5;
		return 5;
	}
	else if(!((GPIO_PORTE_DATA_R >> 3) & 1)){ //if SW4 is pushed
		_prevButton = 4;
		return 4;
	}
	else if(!((GPIO_PORTE_DATA_R >> 2) & 1)){ //if SW3 is pushed
		_prevButton = 3;
		return 3;
	}
	else if(!((GPIO_PORTE_DATA_R >> 1) & 1)){ //if SW2 is pushed
		_prevButton = 2;
		return 2;
	}
	else if(!((GPIO_PORTE_DATA_R >> 0) & 1)){ //if SW1 is pushed
		_prevButton = 1;
		return 1;
	}
	else{
		_prevButton = 0;
		return 0;
	}
}

uint8_t button_getButtonBlocking() {
	uint8_t button;

	//Wait for button to be pressed
	while( (button = button_getButton()) == 0 );

	return button;
}


int8_t button_getButtonChange() {
	uint8_t lastButton = _prevButton; //_prevButton can be set in button_getButton if you want to use this function.
	int8_t curButton = button_getButton();

	if(curButton != lastButton) {
		return curButton;
	}
	else {
		return -1;
	}
}

int8_t button_getButtonChangeBlocking() {
	int8_t button;

	while( (button = button_getButtonChange()) == -1);

	return button;
}
