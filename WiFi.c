/**
*	@file WiFi.c
*	@brief This file contains all of the methods necessary to operate the WiFi.
*  	@author	Brian Bradford, Rajiv Bhoopala, Andrew Thai, Nick Knuth
*   @date 2/15/2017
*/

#include <string.h>
#include "uart.h"
#include "WiFi.h"
#include "timer.h"

/**
*	This method is used to start WiFi communication.
*	@author	Brian Bradford, Rajiv Bhoopala, Andrew Thai, Nick Knuth
*	@param psk	The password for the WiFi
*	@date 2/15/2017
*/
char WiFi_start(char psk[]){
	uart_init();
	
	//Set command pin (PB2) to high
	GPIO_PORTB_DATA_R |= BIT2;
	
	//Send command
	uart_sendChar(0x00);
	
	//Send WiFi PSK
	uart_sendStr(psk);
	
	//NULL terminator
	uart_sendChar('\0');
	
	//Wait for response
	char result = uart_receive();

	//Set command pin to low
	GPIO_PORTB_DATA_R &= ~BIT2;

	if(result != 0)
	{
		//An error occurred...
	}
	return result;
}

/**
*	This method is used to stop WiFi communication.
*	@author	Brian Bradford, Rajiv Bhoopala, Andrew Thai, Nick Knuth
*	@date 2/15/2017
*/
char WiFi_stop(void){
	//Set command pin (PB2) to high
	GPIO_PORTB_DATA_R |= BIT2;

	//Send command
	uart_sendChar(0x01);

	//Wait for response
	char result = uart_receive();

	//Set command pin to low
	GPIO_PORTB_DATA_R &= ~BIT2;

	if(result != 0)
	{
		//An error occurred...
	}
	return result;
}
