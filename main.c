/**
*  	@file main.c
*  	@brief this file will contains the main for the Roomba to navigate the obstacle course.
*
*	@author	Brian Bradford, Nick Knuth, Andrew Thai, and Rajiv Bhoopala
*
*  	@date	4/12/2017
*/

#include <stdio.h>
#include "WiFi.h"
#include "uart.h"
#include "timer.h"
#include "servo.h"
#include "ping.h"
#include "open_interface.h"
#include "lcd.h"
#include "ir.h"
#include "button.h"
#include "commands.h"


/**
*	Infinite loop for Roomba to navigate with GUI instructions for Putty.
*	@author	Brian Bradford, Nick Knuth, Andrew Thai, and Rajiv Bhoopala
*	@date	4/12/2017
*/
int main(void) {
	init_All();

	char instructions[] = "Welcome to roomba obstacle course! \r\nMove forward = up on dpad\r\nMove backwards = down on dpad\r\nRotate clockwise = right on dpad\r\n Rotate counterclock = left on dpad\r\nScan = X\r\n Song = Triangle\r\n";
	uart_sendStr(instructions);
	while (1)
	{
	    move_roomba();
	}
}

/**
*	This method initializes all functions needed for Rover.
*	@author	Brian Bradford, Nick Knuth, Andrew Thai, and Rajiv Bhoopala
*	@date	4/12/2017
*/
void init_All(){
	//WiFi_start("password");
	//wait 10 seconds to get WiFi configured
	//timer_waitMillis(10000);
	uart_init();
	//wait 5 seconds for putty to connect
	//timer_waitMillis(5000);
	lcd_init();
	button_init();
	ir_init();
	ping_init();
	servo_init();
	//wait one second for servo to reset to 0 degrees.
	timer_waitMillis(1000);
	//oi_t *sensor_data = oi_alloc();
	//oi_init(sensor_data);
	//oi_t *sensor_data = oi_alloc();
	//oi_init(sensor_data);
}

