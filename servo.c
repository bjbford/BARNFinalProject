/*
 * servo.c
 *
 *  Created on: Mar 8, 2017
 *      Author: Brian Bradford and Rajiv Bhoopala
 */

#include "servo.h"

void servo_init(){
	//INIT PORT B: GPIO PB5, turn on clk, alt. function, output, enable
	SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R1;
	GPIO_PORTB_AFSEL_R |= BIT5;	//alt. function
	GPIO_PORTB_PCTL_R |= 0x700000;
	GPIO_PORTB_DIR_R |= BIT5;	//set as outputs
	GPIO_PORTB_DEN_R |= BIT5;	//digital enable

	//CONFIGURE TIMER
	SYSCTL_RCGCTIMER_R |= SYSCTL_RCGCTIMER_R1; //turn on clk for timer1

	TIMER1_CTL_R &= ~(TIMER_CTL_TBEN); //disable timer to config
	TIMER1_CFG_R |= TIMER_CFG_16_BIT; //set size of timer to 16
	TIMER1_TBMR_R |= (TIMER_TBMR_TBMR_PERIOD | TIMER_TBMR_TBAMS); //periodic and PWM enable
	TIMER1_TBMR_R &= ~(TIMER_TBMR_TBCMR);
	TIMER1_CTL_R &= ~(TIMER_CTL_TBPWML);	//set to non-inverted PWM mode
	TIMER1_TBILR_R |= (320000 & 0xFFFF); // set lower 16 bits of interval
	TIMER1_TBPR_R |= (320000 >> 16);	//set upper 8 bits of interval

	pulse_width = 0;
	move_servo(pulse_width);
	TIMER1_TBMATCHR_R |= ((320000 - pulse_width) & 0xFFFF); // set lower 16 bits of pulse width
	TIMER1_TBPMR_R |= ((320000- pulse_width) >> 16);	//set upper 8 bits of pulse width
	TIMER1_CTL_R |= TIMER_CTL_TBEN; //enable timer
}

void move_servo(int degree){
	pulse_width = ((28799.2 * (degree/180.0)) - 1333);
	TIMER1_TBMATCHR_R = ((320000 - pulse_width) & 0xFFFF);	//set lower 16 bits of pulse width
	TIMER1_TBPMR_R |= ((320000 - pulse_width) >> 16);	//set the upper 8 bits of the pulse width
	//Delay for the servo to move to the position (.05 second)
	timer_waitMillis(50);
}
