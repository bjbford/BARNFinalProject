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

void init_All();

/*
 * main.c
 */
int main(void) {
	init_All();
	
	return 0;
}

/**
 *
 */
void init_All(){
	WiFi_start("password");
	uart_init();
	lcd_init();
	button_init();
	ir_init();
	ping_init();
	servo_init();
	oi_t *sensor_data = oi_alloc();
	oi_init(sensor_data);
}
