/**
*	@file WiFi.h
*	@brief This header contains the methods and definitions for starting and stopping WiFi.
*	
*	@author Brian Bradford, Rajiv Bhoopala, Andrew Thai, Nick Knuth
*	
*	@date 2-15-2017
*/

#ifndef WIFI_H_
#define WIFI_H_

#include <stdint.h>
#include <inc/tm4c123gh6pm.h>
#include "uart.h"

/**
*	This method is used to initiate WiFi communication.
*	@author Brian Bradford, Rajiv Bhoopala, Andrew Thai, Nick Knuth
*	@param psk	The password to be used for WiFi
*	@date 2-15-2017
*/
char WiFi_start(char psk[]);

/**
*	This method is used to stop WiFi communication.
*	@author Brian Bradford, Rajiv Bhoopala, Andrew Thai, Nick Knuth
*	@date 2-15-2017
*/
char WiFi_stop();


#endif /* WIFI_H_ */
