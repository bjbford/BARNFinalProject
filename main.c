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
void move_roomba() ;
void move_forward(oi_t *sensor, int centimeters);
void move_backwards(oi_t *sensor, int centimeters) ;
void turn_counterClockwise(oi_t *sensor, int degrees);
void turn_clockwise(oi_t *sensor, int degrees) ;
int checkBumper() ;



/*
 * main.c
 */
int main(void) {
	init_All();
	

	oi_t *sensor_data = oi_alloc() ;
	oi_init(sensor_data) ;



	while (1)
	{
	    move_roomba() ;
	}



}

/**
 *	Initializes all functions needed for Rover.
 */
void init_All(){
//	WiFi_start("password");
	uart_init();
	lcd_init();
	button_init();
	ir_init();
	ping_init();
	servo_init();
	oi_t *sensor_data = oi_alloc();
	oi_init(sensor_data);

}

/**
 *
 */
void move_roomba ()
{
       oi_t *sensor_data = oi_alloc();
       oi_init(sensor_data);
       char input ;

//        scanf("%c", &input) ;
        input = uart_receive();
        //forwards
        if (input == 'u')
        {
            move_forward(sensor_data, 100); //move forward

        }
        //backwards
        else if (input == 'b')
        {
            move_backwards(sensor_data, -100); //backwards
        }

        //rotates clockwise
        else if (input == 'd')
        {
            turn_clockwise(sensor_data, 5) ;//rotate 90 degrees

        }

        //rotates counterclockwise
        else if (input == 'a')
        {
            turn_counterClockwise(sensor_data, 5) ;
        }

        //scan surroundings
        else if (input == 'p')
        {

        }
        oi_free(sensor_data) ;
}

void move_forward(oi_t *sensor, int centimeters){


    oi_setWheels(100,100) ; //move forward

    int sum = 0 ;

    while (sum < centimeters)
    {

        oi_update(sensor);
        sum += sensor->distance;
        timer_waitMillis(100);
    }

    oi_setWheels(0,0); //stop wheels
    oi_free(sensor) ;

}

void move_backwards(oi_t *sensor, int centimeters)
{
//    oi_t *sensor_data = oi_alloc() ;
//    oi_init(sensor_data) ;

    oi_setWheels(-100, -100) ; //move backward; half speed

    int sum = 0 ;

    while (sum > centimeters)
    {
        oi_update(sensor) ;

        sum += sensor->distance;
        timer_waitMillis(1);

    }

    oi_setWheels(0,0) ; //stop wheels
    oi_free(sensor) ;
}

void turn_counterClockwise(oi_t *sensor, int degrees){
//    oi_t *sensor_data = oi_alloc();
//    oi_init(sensor_data);

    oi_setWheels(100, -100);
//    int initial_angle = sensor->angle;
//    int angle = 0;
//    while(abs(initial_angle-angle) < degrees){
//        oi_update(sensor);
//        angle += sensor->angle;
//    }
    timer_waitMillis(degrees*22);
    oi_setWheels(0, 0); //stop wheels
    oi_free(sensor) ;

}

void turn_clockwise(oi_t *sensor, int degrees){
//    oi_t *sensor_data = oi_alloc();
//    oi_init(sensor_data);

    oi_setWheels(-100, 100);
//    int initial_angle = sensor->angle;
//    int angle = 0;
//    while(abs(initial_angle-angle) < degrees){
//        oi_update(sensor);
//        angle += sensor->angle;
//    }
    timer_waitMillis(degrees*22);
    oi_setWheels(0, 0); //stop wheels
    oi_free(sensor) ;
}


void scan()
{

}





