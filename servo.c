/**
*	@file servo.c
*	@brief This file contains the methods necessary to operate the servo.
*	@author	Brian Bradford, Rajiv Bhoopala, Andrew Thai, Nick Knuth
*	@date 3/8/2017
*/

#include "servo.h"

/**
*	This method initiates servo operation.
*	@author Brian Bradford, Rajiv Bhoopala, Andrew Thai, Nick Knuth
*	@date 3/8/2017
*/
void servo_init(){
	//INIT PORT B: GPIO PB5, turn on clk, alt. function, output, enable
	SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R1;

	//alt. function
	GPIO_PORTB_AFSEL_R |= BIT5;
	GPIO_PORTB_PCTL_R |= 0x700000;

	//set as outputs
	GPIO_PORTB_DIR_R |= BIT5;

	//digital enable
	GPIO_PORTB_DEN_R |= BIT5;

	//CONFIGURE TIMER
	//turn on clk for timer1
	SYSCTL_RCGCTIMER_R |= SYSCTL_RCGCTIMER_R1;

	//disable timer to config
	TIMER1_CTL_R &= ~(TIMER_CTL_TBEN);
	
	//set size of timer to 16
	TIMER1_CFG_R |= TIMER_CFG_16_BIT;
	
	//periodic and PWM enable
	TIMER1_TBMR_R |= (TIMER_TBMR_TBMR_PERIOD | TIMER_TBMR_TBAMS);
	TIMER1_TBMR_R &= ~(TIMER_TBMR_TBCMR);
	
	//set to non-inverted PWM mode
	TIMER1_CTL_R &= ~(TIMER_CTL_TBPWML);
	
	// set lower 16 bits of interval
	TIMER1_TBILR_R |= (320000 & 0xFFFF);
	
	//set upper 8 bits of interval
	TIMER1_TBPR_R |= (320000 >> 16);

	pulse_width = 0;
	move_servo(pulse_width);
	
	// set lower 16 bits of pulse width
	TIMER1_TBMATCHR_R |= ((320000 - pulse_width) & 0xFFFF);
	
	//set upper 8 bits of pulse width
	TIMER1_TBPMR_R |= ((320000- pulse_width) >> 16);
	
	//enable timer
	TIMER1_CTL_R |= TIMER_CTL_TBEN;
}

/**
*	This method moves the servo.
*	@author	Brian Bradford, Rajiv Bhoopala, Andrew Thai, Nick Knuth
*	@param degree	The degree to move the servo to
*	@date 3/8/2017
*/
void move_servo(int degree){
	pulse_width = ((28992 * (degree/180.0)) + 874);
	
	//set lower 16 bits of pulse width
	TIMER1_TBMATCHR_R = ((320000 - pulse_width) & 0xFFFF);
	
	//set the upper 8 bits of the pulse width
	TIMER1_TBPMR_R |= ((320000 - pulse_width) >> 16);
	
	//Delay for the servo to move to the position
	timer_waitMillis(50);
}
