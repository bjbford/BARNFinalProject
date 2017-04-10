/*
 * ping.c
 *
 *  Created on: Mar 1, 2017
 *      Author: Brian Bradford and Rajiv Bhoopala
 */
#include "ping.h"

void TIMER3B_Handler(void);

void ping_init(void){
	SYSCTL_RCGCTIMER_R |= SYSCTL_RCGCTIMER_R3;	//Turn on clock to TIMER3
	SYSCTL_RCGCGPIO_R |= 0x02;	//Port B (bit1) system clock
	GPIO_PORTB_DEN_R |= 0x08;	//Pin 3 init
	GPIO_PORTB_PCTL_R |= GPIO_PCTL_PB3_T3CCP1;

	TIMER3_CTL_R &= ~TIMER_CTL_TBEN;	//Configure the timer for input capture mode
	TIMER3_CTL_R |= TIMER_CTL_TBEVENT_BOTH;	//Edge select
	TIMER3_CFG_R |= TIMER_CFG_16_BIT;	//Set to a 16 bit counter
	TIMER3_TBMR_R |= TIMER_TBMR_TBMR_CAP;	//Capture mode
	TIMER3_TBMR_R |= TIMER_TBMR_TBCMR;	//Edge-time mode
	TIMER3_TBMR_R |= TIMER_TBMR_TBCDIR;	//count up.
	TIMER3_TBILR_R |= 0xFFFF;	//Set upper bound
	TIMER3_IMR_R |= TIMER_IMR_CBEIM;	//enable capture input
	TIMER3_ICR_R |= TIMER_ICR_CBECINT;	//clears interrupt flags

	//105 and 142 interrupt #
	NVIC_EN1_R |= 0x10; //Timer 3B: 36 Register 5, set Enable (EN1) BIT4 or 0x10

	IntRegister(INT_TIMER3B, TIMER3B_Handler);	//register TIMER3B interrupt handler
	IntMasterEnable();	//initialize global interrupts

	//TIMER3_CTL_R |= TIMER_CTL_TBEN;	//Enables TIMER3B
}

// send out a pulse on PB3
void send_pulse(){
	//alternative and port control (AFSEL and CTL)
	//alt function off
	GPIO_PORTB_AFSEL_R &= ~(0x08);
	GPIO_PORTB_PCTL_R &= ~(0x0000F000);//off
	GPIO_PORTB_DIR_R |= 0x08; // set PB3 as output
	GPIO_PORTB_DATA_R |= 0x08; // set PB3 to high
	timer_waitMicros(5); // wait at least 5 microseconds based on data sheet
	GPIO_PORTB_DATA_R &= 0xF7; // set PB3 to low
	state = RISE;
	GPIO_PORTB_DIR_R &= 0xF7; // set PB3 as input
	//alt function on
	GPIO_PORTB_AFSEL_R |= 0x08;
	GPIO_PORTB_PCTL_R |= 0x00007000; //enable pin3 alternate function 7 T3CCP1

	//enable TIMER3B and start counting
	TIMER3_CTL_R |= TIMER_CTL_TBEN;// 0x100
}



void TIMER3B_Handler(void)
{
	if(TIMER3_MIS_R & TIMER_MIS_CBEMIS) {// fixed condition
		if(state == RISE)
		{
			TIMER3_ICR_R = TIMER_ICR_CBECINT; //clear capture interrupt flag
			risingEdge = ((int)TIMER3_TBPS_R << 16) | TIMER3_TBR_R; //captures time of rising edge event
			state = FALL; //now capturing falling edge
		}
		else if(state == FALL){
			TIMER3_ICR_R = TIMER_ICR_CBECINT; //clear capture interrupt flag
			fallingEdge = ((int)TIMER3_TBPS_R << 16) | TIMER3_TBR_R; //capture time of falling edge
			state = DONE;
			TIMER3_CTL_R &= ~TIMER_CTL_TBEN; //disable timerB
		}
	}
}
float ping_getDistance()
{
	send_pulse();
	//wait until end of pulse
	while(state != DONE){
	}
	//Correction for overflow
	int overflow = (fallingEdge < risingEdge);

	pulseLength = fallingEdge + (overflow<<24) - risingEdge;

	//calculate distance in mm
	return pulseLength / PING_FACTOR;
}
