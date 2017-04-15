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
int checkBumper(oi_t *sensor) ;
int checkCliff(oi_t *sensor) ;
int cliffWarning(oi_t *sensor) ;

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
            move_forward(sensor_data, 25); //move forward

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



    int sum = 0 ;
    char str[15];
    while (sum < centimeters)
    {
//        int flag = cliffWarning(sensor);
//        if (flag == 1 )
//        {
//            break ;
//        }
        int leftSignal = sensor -> cliffLeftSignal ;
        int rightSignal = sensor -> cliffRightSignal ;
        int frontLeftSignal = sensor -> cliffFrontLeftSignal ;
        int frontRightSignal = sensor -> cliffFrontRightSignal ;
        oi_setWheels(75,75) ; //move forward
        oi_update(sensor);
        sum += sensor->distance;
        if(leftSignal > 2500)
        {
            char* warning = "White Line on Left Side! \n\r" ;
            uart_sendStr(warning);
            oi_setWheels(0,0);
            break ;
        }
//        sprintf(str, "%d\n\r", rightSignal);
//        uart_sendStr(str);
        if(rightSignal > 2600)
        {
            char* warning = "White Line on RIGHT side! \n\r" ;
            uart_sendStr(warning);
            oi_setWheels(0,0);
            break ;
        }
        if(frontLeftSignal > 2600)
        {
            char* warning = "White Line on Front Left side! \n\r" ;
            uart_sendStr(warning);
            oi_setWheels(0,0);
            break ;
        }
        if(frontRightSignal > 2600)
        {
            char* warning = "White Line on Front Right side! \n\r" ;
            uart_sendStr(warning);
            oi_setWheels(0,0);
            break ;
        }
        if (checkBumper(sensor) == 1)
        {
            char* warning = "Hit right bumper! \n\r" ;
            uart_sendStr(warning);
            oi_setWheels(0,0);
            break ;
        }
        if (checkBumper(sensor) == 2)
        {
            char* warning = "Hit left bumper! \n\r" ;
            uart_sendStr(warning) ;
            oi_setWheels(0,0);
            break ;
        }
        if (checkCliff(sensor) == 1)
        {
            char* warning = "WARNING CLIFF ON LEFT SIDE! \n\r" ;
            uart_sendStr(warning) ;
            oi_setWheels(0,0) ;
            break ;
        }
        if (checkCliff(sensor) ==2)
        {
            char* warning = "WARNING CLIFF ON RIGHT SIDE! \n\r" ;
            uart_sendStr(warning) ;
            oi_setWheels(0,0) ;
            break ;
        }
        if (checkCliff(sensor) ==3)
        {
            char* warning = "WARNING CLIFF IN FRONT ON LEFT SIDE! \n\r" ;
            uart_sendStr(warning) ;
            oi_setWheels(0,0) ;
            break;
        }
        if (checkCliff(sensor) == 4)
        {
            char* warning = "WARNING CLIFF IN FRONT ON RIGHT SIDE \n\r" ;
            uart_sendStr(warning) ;
            oi_setWheels(0,0) ;
            break ;
        }
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

int checkBumper(oi_t *sensor)
{
    //bumper = 0 no bumps
    //bumper = 1 right bumper
    //bumper = 2 left bumper
    int bumper = 0 ;

    oi_update(sensor) ;
    if (sensor-> bumpLeft)
        {
        bumper = 2 ;
        }
    else if (sensor-> bumpRight)
    {
        bumper = 1 ;
    }
    else
    {
        bumper = 0 ;
    }

    return bumper ;
}

int checkCliff(oi_t *sensor)
{
    oi_update(sensor) ;

    /*
     *  0 = no cliff
     *  1 = cliffleft
     *  2 = cliffright
     *  3 = cliffront left
     *  4 = clifffront right
     */


    int cliff = 0 ;

    if(sensor-> cliffLeft)
    {
        cliff = 1 ;
    }
    else if(sensor -> cliffRight)
    {
        cliff = 2 ;
    }
    else if(sensor -> cliffFrontLeft)
    {
        cliff = 3 ;
    }
    else if (sensor -> cliffFrontRight)
    {
        cliff = 4 ;
    }

    else
    {
        cliff = 0 ;
    }




    return cliff ;

}

int cliffWarning(oi_t *sensor)
{
    int flag = 0 ;
    oi_update(sensor) ;

    if (checkCliff(sensor) == 1)
    {
        char* warning = "WARNING CLIFF ON LEFT SIDE! \n\r" ;
        uart_sendStr(warning) ;
        oi_setWheels(0,0) ;
        flag = 1 ;
    }
    else if (checkCliff(sensor) ==2)
    {
        char* warning = "WARNING CLIFF ON RIGHT SIDE! \n\r" ;
        uart_sendStr(warning) ;
        oi_setWheels(0,0) ;
        flag = 1 ;
    }
    else if (checkCliff(sensor) ==3)
    {
        char* warning = "WARNING CLIFF IN FRONT ON LEFT SIDE! \n\r" ;
        uart_sendStr(warning) ;
        oi_setWheels(0,0) ;
        flag = 1 ;
    }
    else if (checkCliff(sensor) == 4)
    {
        char* warning = "WARNING CLIFF IN FRONT ON RIGHT SIDE \n\r" ;
        uart_sendStr(warning) ;
        oi_setWheels(0,0) ;
        flag = 1 ;
    }




}


