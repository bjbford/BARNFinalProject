/**
*	@file uart.h
*	@brief This header contains the methods and definitions for starting and using UART communication
* 	
*	@author Brian Bradford, Rajiv Bhoopala, Andrew Thai, Nick Knuth
*
*	@date 2-15-2017
*/

#ifndef UART_H_
#define UART_H_

#include <stdint.h>
#include <inc/tm4c123gh6pm.h>

/**
*	This method is used to send strings over UART communication from the robot to a PuTTY terminal
*	@author Brian Bradford, Rajiv Bhoopala, Andrew Thai, Nick Knuth
*	@param data	The string data to be sent over UART
* 	@date 2-15-17
*/
void uart_sendStr(char data[]);

/**
*	This method is used to send single characters over UART communication from the robot to a PuTTY terminal
*	@author Brian Bradford, Rajiv Bhoopala, Andrew Thai, Nick Knuth
*	@param data	The character data to be sent over UART
* 	@date 2-15-17
*/
void uart_sendChar(char data);

/**
*	This method is used to send characters over UART communication from a PuTTY terminal to the robot
*	@author Brian Bradford, Rajiv Bhoopala, Andrew Thai, Nick Knuth
* 	@date 2-15-17
*/
char uart_receive(void);

/**
*	This method is used to initiate UART communication
*	@author Brian Bradford, Rajiv Bhoopala, Andrew Thai, Nick Knuth
* 	@date 2-15-17
*/
void uart_init(void);

#endif /* UART_H_ */
