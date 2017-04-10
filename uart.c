/*
 * uart.c
 *
 *  Created on: 2-15-2017
 *      Author: Brian Bradford and Rajiv Bhoopala
 */

#include <string.h>
#include "uart.h"
#include "button.h"
#include "timer.h"

//Send string over uart
void uart_sendStr(char data[]){
	int i=0;
	for(i=0; data[i] != '\0'; i++){
		uart_sendChar(data[i]);
	}
}


//Blocking call that sends 1 char over UART 1
void uart_sendChar(char data){
	//wait until there is room to send data
	while(UART1_FR_R & 0x20)
	{
	}
	//send data
	UART1_DR_R = data;
}

//Blocking call to receive over uart1
//returns char with data
char uart_receive(void){
	char data = 0;
	//wait to receive
	while(UART1_FR_R & UART_FR_RXFE)
	{
//		//PART 3
//		uint8_t button = button_getButton();
//		static uint8_t tempButton = 0;			//to be used in order to avoid improper hold down of button
//
//		if(button == tempButton){
//			//do nothing
//		}
//		else if(button == 6){					//Button 6 pressed
//			char string[] = "Yes\r\n";
//			int j = 0;
//			for(j=0;j<strlen(string);j++){
//				uart_sendChar(string[j]);
//			}
//		}
//		else if(button == 5){					//Button 5 pressed
//			char string[] = "No\r\n";
//			int j = 0;
//			for(j=0;j<strlen(string);j++){
//				uart_sendChar(string[j]);
//			}
//		}
//		else if(button == 4){					//Button 4 pressed
//			char string[] = "Blue, no green, Ahhhhh!!!\r\n";
//			int j = 0;
//			for(j=0;j<strlen(string);j++){
//				uart_sendChar(string[j]);
//			}
//		}
//		else if(button == 3){					//Button 3 pressed
//			char string[] = "Hello?\r\n";
//			int j = 0;
//			for(j=0;j<strlen(string);j++){
//				uart_sendChar(string[j]);
//			}
//		}
//		else if(button == 2){					//Button 2 pressed
//			char string[] = "Are you receiving me?\r\n";
//			int j = 0;
//			for(j=0;j<strlen(string);j++){
//				uart_sendChar(string[j]);
//			}
//		}
//		else if(button == 1){					//Button 1 pressed
//			char string[] = "Talk to me!\r\n";
//			int j = 0;
//			for(j=0;j<strlen(string);j++){
//				uart_sendChar(string[j]);
//			}
//		}
//		tempButton = button;					//update with new button press
	}
	//mask the 4 error bits and grab only 8 data bits
	data = (char)(UART1_DR_R & 0xFF);
	return data;
}

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
