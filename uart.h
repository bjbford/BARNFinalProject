/*
 * uart.h
 *
 *  Created on: 2-15-2017
 *      Author: Brian Bradford and Rajiv Bhoopala
 */

#ifndef UART_H_
#define UART_H_

#include <stdint.h>
#include <inc/tm4c123gh6pm.h>

void uart_sendStr(char data[]);
void uart_sendChar(char data);
char uart_receive(void);
void uart_init(void);

#endif /* UART_H_ */
