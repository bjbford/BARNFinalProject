/*
 * WiFi.c
 *
 *  Created on: 2-15-2017
 *      Author: Brian Bradford and Rajiv Bhoopala
 */

#include <string.h>
#include "uart.h"
#include "WiFi.h"
#include "timer.h"

//PART 4
char WiFi_start(char psk[]){
	uart_init();

	GPIO_PORTB_DATA_R |= BIT2;			//Set command pin (PB2) to high

	uart_sendChar(0x00); 				//Send command
	uart_sendStr(psk);		 			//Send WiFi PSK
	uart_sendChar('\0'); 				//NULL terminator
	char result = uart_receive();		//Wait for response

	GPIO_PORTB_DATA_R &= ~BIT2;			//Set command pin to low

	if(result != 0){
		//An error occurred...
	}
	return result;
}

char WiFi_stop(void){
	GPIO_PORTB_DATA_R |= BIT2;			//Set command pin (PB2) to high

	uart_sendChar(0x01); 				//Send command

	char result = uart_receive();		//Wait for response

	GPIO_PORTB_DATA_R &= ~BIT2;			//Set command pin to low

	if(result != 0){
		//An error occurred...
	}
	return result;
}
