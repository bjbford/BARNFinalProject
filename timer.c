/*
 * Timer.cpp
 *
 *  @author Eric Middleton
 */

#include "Timer.h"

volatile uint32_t _timer_ticks;

void timer_waitMillis(uint32_t millis) {
	///Start timer with period of 1ms
	timer_startTimer(999);

	///loop until enough milliseconds have passed
	while(millis > 0) {
		///wait until the millisecond timer has timed out
		while(!(TIMER5_RIS_R & TIMER_RIS_TATORIS));

		///Reset the timeout flag
		TIMER5_ICR_R |= TIMER_RIS_TATORIS;

		millis--;
	}

	///Stop the timer
	timer_stopTimer();
}

void timer_waitMicros(uint16_t micros) {
	//Start timer with period of 1us
	timer_startTimer(micros - 1);

	while(!(TIMER5_RIS_R & TIMER_RIS_TATORIS));

	TIMER5_ICR_R |= TIMER_RIS_TATORIS;

	timer_stopTimer();
}

void timer_startTimer(uint16_t startValue) {
	//Enable Timer5
	SYSCTL_RCGCTIMER_R |= SYSCTL_RCGCTIMER_R5;

	//Disable timer (clear bit TnEN in GPTMCTL)
	TIMER5_CTL_R &= ~TIMER_CTL_TAEN;

	//Set as 16-bit timer
	TIMER5_CFG_R = TIMER_CFG_16_BIT;

	//Configure the timer for periodic mode
	//and countdown
	TIMER5_TAMR_R = TIMER_TAMR_TAMR_PERIOD;

	//Load initial timer value
	TIMER5_TAILR_R = startValue;

	//Clear the timeout flag
	TIMER5_ICR_R = TIMER_ICR_TATOCINT;

	//Set the prescaler to 15 (period = 1us)
	TIMER5_TAPR_R = 15;

	//Clear the interrupt mask register
	TIMER5_IMR_R = 0;

	//Enable Timer5 A
	TIMER5_CTL_R |= TIMER_CTL_TAEN;
}

void timer_stopTimer(void) {
	//Disable timer (clear bit TnEN in GPTMCTL)
	TIMER5_CTL_R &= ~TIMER_CTL_TAEN;

	//Disable system control for Timer1
	SYSCTL_RCGCTIMER_R &= ~SYSCTL_RCGCTIMER_R5;
}

clock_t timer_startClock(void) {
	//Enable Timer5
	SYSCTL_RCGCTIMER_R |= SYSCTL_RCGCTIMER_R5;

	//Disable timer (clear bit TnEN in GPTMCTL)
	TIMER5_CTL_R &= ~TIMER_CTL_TBEN;

	//Set as 16-bit timer
	TIMER5_CFG_R = TIMER_CFG_16_BIT;

	//Configure the timer for periodic mode
	//and countdown
	TIMER5_TBMR_R = TIMER_TBMR_TBMR_PERIOD;

	//Load initial timer value
	TIMER5_TBILR_R = 0;

	//Clear the timeout flag
	TIMER5_ICR_R = TIMER_ICR_TBTOCINT;

	//Set the prescaler to 16000 (period = 1us)
	TIMER5_TBPR_R = 16000 - 1;

	//Clear the interrupt mask register
	TIMER5_IMR_R = 0;

	//Enable Timer5 B
	TIMER5_CTL_R |= TIMER_CTL_TBEN;

	return TIMER5_TBR_R;
}

clock_t timer_getClock(void) {
	return TIMER5_TBR_R;
}

void timer_stopClock(void) {
	TIMER5_CTL_R &= ~TIMER_CTL_TBEN;
}
