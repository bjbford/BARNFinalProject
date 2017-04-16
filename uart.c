/**
*	@file uart.c
*	@brief This source file contains the methods and definitions for starting and using UART communication
*
*	@author Brian Bradford, Rajiv Bhoopala, Andrew Thai, Nick Knuth
*
*	@date 2-15-2017
*/

#include <string.h>
#include "uart.h"
#include "button.h"
#include "timer.h"

/**
*	This method is used to send strings over UART communication from the robot to a PuTTY terminal
*	@author Brian Bradford, Rajiv Bhoopala, Andrew Thai, Nick Knuth
*	@param data	The string data to be sent over UART
* 	@date 2-15-17
*/
void uart_sendStr(char data[]){
	int i=0;
	for(i=0; data[i] != '\0'; i++){
		uart_sendChar(data[i]);
	}
}


/**
*	This method is used to send single characters over UART communication from the robot to a PuTTY terminal
*	@author Brian Bradford, Rajiv Bhoopala, Andrew Thai, Nick Knuth
*	@param data	The character data to be sent over UART
* 	@date 2-15-17
*/
void uart_sendChar(char data){
	//wait until there is room to send data
	while(UART1_FR_R & 0x20)
	{
	}
	//send data
	UART1_DR_R = data;
}

/**
*	This method is used to send characters over UART communication from a PuTTY terminal to the robot
*	@author Brian Bradford, Rajiv Bhoopala, Andrew Thai, Nick Knuth
* 	@date 2-15-17
*/
char uart_receive(void){
	char data = 0;
	//wait to receive
	while(UART1_FR_R & UART_FR_RXFE)
	{
	}
	//mask the 4 error bits and grab only 8 data bits
	data = (char)(UART1_DR_R & 0xFF);
	return data;
}

/**
*	This method is used to initiate UART communication
*	@author Brian Bradford, Rajiv Bhoopala, Andrew Thai, Nick Knuth
* 	@date 2-15-17
*/
void uart_init(void) {
	//enable clock to GPIO, R1 = port B
	SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R1;

	//enable clock to UART1, R1 = UART1. ***Must be done before setting Rx and Tx (See DataSheet)
	SYSCTL_RCGCUART_R |= SYSCTL_RCGCUART_R1;

	//enable alternate functions on port b pins 0 and 1
	GPIO_PORTB_AFSEL_R |= (BIT0 | BIT1);

	//enable Rx and Tx on port B on pins 0 and 1
	GPIO_PORTB_PCTL_R |= 0x00000011;

	//set pin 0 and 1 and 2 to digital
	GPIO_PORTB_DEN_R |= (BIT0 | BIT1 | BIT2);

	//set pin 0 to Rx or input
	GPIO_PORTB_DIR_R &= ~BIT0;

	//set pin 1 and 2 to Tx or output
	GPIO_PORTB_DIR_R |= (BIT1 | BIT2);

	//calculate baudrate
	uint16_t iBRD = 8; //use equations
	uint16_t fBRD = 44;//use equations

	//turn off uart1 while we set it up
	UART1_CTL_R &= ~(UART_CTL_UARTEN);

	//set baud rate
	UART1_IBRD_R = iBRD;
	UART1_FBRD_R = fBRD;

	//set frame, 8 data bits, 1 stop bit, no parity, no FIFO
	UART1_LCRH_R = UART_LCRH_WLEN_8 ;

	//use system clock as source
	UART1_CC_R = UART_CC_CS_SYSCLK;

	//re-enable enable RX, TX, and uart1
	UART1_CTL_R = (UART_CTL_RXE | UART_CTL_TXE | UART_CTL_UARTEN);
}
