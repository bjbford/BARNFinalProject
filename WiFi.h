/*
 * WiFi.h
 *
 *  Created on: 2-15-2017
 *      Author: Brian Bradford and Rajiv Bhoopala
 */

#ifndef WIFI_H_
#define WIFI_H_

#include <stdint.h>
#include <inc/tm4c123gh6pm.h>
#include "uart.h"

char WiFi_start(char psk[]);
char WiFi_stop();


#endif /* WIFI_H_ */
