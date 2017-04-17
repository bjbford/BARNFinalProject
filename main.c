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


/*
 * main.c
 */
int main(void) {
	init_All();
	sweep();

	
//	oi_t *sensor_data = oi_alloc() ;
//	oi_init(sensor_data) ;
//
//
//
//	while (1)
//	{
//	    move_roomba() ;
//	}
}

/**
 *	Initializes all functions needed for Rover.
 */
void init_All(){
//WiFi_start("password");
	//wait 10 seconds to get WiFi configured
	timer_waitMillis(10000);
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

