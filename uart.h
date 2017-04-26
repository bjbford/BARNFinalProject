/**
*	@file uart.h
*	@brief This file contains all the methods to communicate between the robot and the computer.
* 	@author	Brian Bradford, Rajiv Bhoopala, Andrew Thai, Nick Knuth
*	@date 2/15/2017
*/

#ifndef UART_H_
#define UART_H_

#include <stdint.h>
#include <inc/tm4c123gh6pm.h>

/**
*	This method is used to send strings from the robot to Putty.
*	@author	Brian Bradford, Rajiv Bhoopala, Andrew Thai, Nick Knuth
*	@param data	String to be sent from the robot to Putty
*	@date 2/15/2017
*/
void uart_sendStr(char data[]);

/**
*	This method is used to send characters from the robot to Putty.
*	@author	Brian Bradford, Rajiv Bhoopala, Andrew Thai, Nick Knuth
*	@param data	Character to be sent from the robot to Putty
*	@date 2/15/2017
*/
void uart_sendChar(char data);

/**
*	This method is used to send characters from Putty to the robot.
*	@author	Brian Bradford, Rajiv Bhoopala, Andrew Thai, Nick Knuth
*	@date 2/15/2017
*/
char uart_receive();

/**
*	This method is used to initiate UART communication.
*	@author	Brian Bradford, Rajiv Bhoopala, Andrew Thai, Nick Knuth
*	@date 2/15/2017
*/
void uart_init();

#endif /* UART_H_ */
