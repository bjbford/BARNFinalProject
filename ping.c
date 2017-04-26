/**
*	@file ping.c
*	@brief This file contains the methods necessary to operate and collect data from the ping sensor.
*	@author Brian Bradford, Rajiv Bhoopala, Andrew Thai, Nick Knuth
*	@date 3/1/2017
*/
#include "ping.h"

/**
*	This method initiates the ping sensor.
*	@author Brian Bradford, Rajiv Bhoopala, Andrew Thai, Nick Knuth
*	@date 3/1/2017
*/
void ping_init(void){
	//Turn on clock to TIMER3
	SYSCTL_RCGCTIMER_R |= SYSCTL_RCGCTIMER_R3;

	//Port B (bit1) system clock
	SYSCTL_RCGCGPIO_R |= 0x02;
	
	//Pin 3 init
	GPIO_PORTB_DEN_R |= 0x08;
	GPIO_PORTB_PCTL_R |= GPIO_PCTL_PB3_T3CCP1;

	//Configure the timer for input capture mode
	TIMER3_CTL_R &= ~TIMER_CTL_TBEN;
	
	//Edge select
	TIMER3_CTL_R |= TIMER_CTL_TBEVENT_BOTH;
	
	//Set to a 16 bit counter
	TIMER3_CFG_R |= TIMER_CFG_16_BIT;
	
	//Capture mode
	TIMER3_TBMR_R |= TIMER_TBMR_TBMR_CAP;
	
	//Edge-time mode
	TIMER3_TBMR_R |= TIMER_TBMR_TBCMR;
	
	//count up.
	TIMER3_TBMR_R |= TIMER_TBMR_TBCDIR;
	
	//Set upper bound
	TIMER3_TBILR_R |= 0xFFFF;
	
	//enable capture input
	TIMER3_IMR_R |= TIMER_IMR_CBEIM;
	
	//clears interrupt flags
	TIMER3_ICR_R |= TIMER_ICR_CBECINT;

	//105 and 142 interrupt #
	//Timer 3B: 36 Register 5, set Enable (EN1) BIT4 or 0x10
	NVIC_EN1_R |= 0x10;

	//register TIMER3B interrupt handler
	IntRegister(INT_TIMER3B, TIMER3B_Handler);
	
	//initialize global interrupts
	IntMasterEnable();

	//TIMER3_CTL_R |= TIMER_CTL_TBEN;	//Enables TIMER3B
}

/**
*	This method operates the ping sensor.
*	@author Brian Bradford, Rajiv Bhoopala, Andrew Thai, Nick Knuth
*	@date 3/1/2017
*/
void send_pulse(){
	//alternative and port control (AFSEL and CTL)
	//alt function off
	GPIO_PORTB_AFSEL_R &= ~(0x08);
	
	//off
	GPIO_PORTB_PCTL_R &= ~(0x0000F000);
	
	// set PB3 as output
	GPIO_PORTB_DIR_R |= 0x08; 
	
	// set PB3 to high
	GPIO_PORTB_DATA_R |= 0x08;
	
	// wait at least 5 microseconds based on data sheet
	timer_waitMicros(5); 
	
	// set PB3 to low
	GPIO_PORTB_DATA_R &= 0xF7;
	state = RISE;
	
	// set PB3 as input
	GPIO_PORTB_DIR_R &= 0xF7;
	
	//alt function on
	GPIO_PORTB_AFSEL_R |= 0x08;
	
	//enable pin3 alternate function 7 T3CCP1
	GPIO_PORTB_PCTL_R |= 0x00007000;

	//enable TIMER3B and start counting
	TIMER3_CTL_R |= TIMER_CTL_TBEN;// 0x100
}

/**
*	This method captures the time from when the sensor sends a pulse and it returns.
*	@author	Brian Bradford, Rajiv Bhoopala, Andrew Thai, Nick Knuth
*	@date 3/1/2017
*/
void TIMER3B_Handler(void)
{
	// fixed condition
	if(TIMER3_MIS_R & TIMER_MIS_CBEMIS) {
		if(state == RISE)
		{
			//clear capture interrupt flag
			TIMER3_ICR_R = TIMER_ICR_CBECINT;
			
			//captures time of rising edge event
			risingEdge = ((int)TIMER3_TBPS_R << 16) | TIMER3_TBR_R;

			//now capturing falling edge
			state = FALL;
		}
		else if(state == FALL)
		{
			//clear capture interrupt flag
			TIMER3_ICR_R = TIMER_ICR_CBECINT;
			
			//capture time of falling edge
			fallingEdge = ((int)TIMER3_TBPS_R << 16) | TIMER3_TBR_R; 
			state = DONE;
			
			//disable timerB
			TIMER3_CTL_R &= ~TIMER_CTL_TBEN;
		}
	}
}

/**
*	This method collects the ping sensor data.
*	@author Brian Bradford, Rajiv Bhoopala, Andrew Thai, Nick Knuth
*	@date 3/1/2017
*/
float ping_getDistance()
{
	send_pulse();
	
	//wait until end of pulse
	while(state != DONE)
	{
		
	}
	
	//Correction for overflow
	int overflow = (fallingEdge < risingEdge);

	pulseLength = fallingEdge + (overflow<<24) - risingEdge;

	//calculate distance in mm
	return pulseLength / PING_FACTOR;
}
