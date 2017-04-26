/**
*	@file WiFi.h
*	@brief This file contains all of the methods necessary to operate the WiFi.
*  	@author	Brian Bradford, Rajiv Bhoopala, Andrew Thai, Nick Knuth
*   @date 2/15/2017
*/

#ifndef WIFI_H_
#define WIFI_H_

#include <stdint.h>
#include <inc/tm4c123gh6pm.h>
#include "uart.h"

/**
*	This method is used to start WiFi communication.
*	@author	Brian Bradford, Rajiv Bhoopala, Andrew Thai, Nick Knuth
*	@param psk	The password for the WiFi
*	@date 2/15/2017
*/
char WiFi_start(char psk[]);

/**
*	This method is used to stop WiFi communication.
*	@author	Brian Bradford, Rajiv Bhoopala, Andrew Thai, Nick Knuth
*	@date 2/15/2017
*/
char WiFi_stop();


#endif /* WIFI_H_ */
