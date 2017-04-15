/*
 * Commands for Robot to execute.
 *
 *  Created on: Apr 12, 2017
 *      Author: bjbford
 */


#include "helpers.h"
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
 * Sweep with both the Ping sensor and IR distance sensor over the 180 degree range of the servo,
 * taking distance measurements from both sensors every 2 degrees and transmitting the data over WiFi to Putty.
 */
void sweep(){
	int degree=0, beginDegree = 0, angSize = 0;
	int objectLocation = 0;		//location of object in degrees
	int objectCount = 0;		//total count of object
	float rawDist = 0.0;		//used to average out distance of object
	float actualDist = 0.0;		//actual distance in centimeters from object
	float angWidth = 0.0;		//actual angular width/linear size of object
	int objectIter = 0;			//used to divide raw distance for average distance of an object

//	//variables used for smallest object
//	float smallestDist = 0.0, smallestWidth = 0.0;
//	int smallestSize = 0, smallestIndex = 0.0, smallestLocation = 0;

	//BEGINNING STATE - no object detected
	int objectState = 0;		//BEGIN = 0, FIRSTDETECT = 1, STILLDETECTED = 2, NONE = 3

	//Arrays to store Cartesian distance for Excel to be outputed to a .txt file.
	float xCartesian[100] = {};
	float yCartesian[100] = {};

	for(degree=0; degree<=180; degree+=2){
		move_servo(degree);	//move 2 degrees
		float irDist = ir_getDistance();
		float sonarDist = ping_getDistance();

		//IR object detection (< 100 cm) and first glimpse of an object
		if((irDist < 100.0) && (sonarDist < 100.0) && (objectState == 0 || objectState == 3)){
			//object has been detected for first time
			objectState = 1;				//FIRSTDETECT
			beginDegree = degree;
			rawDist += sonarDist;
			objectIter++;
			objectCount++;
		}
		else if((irDist < 100.0) && (sonarDist < 100.0) && (objectState == 1)){
			//object still detected (not first time seeing it)
			objectState = 2;				//STILLDETECTED
			rawDist += sonarDist;
			objectIter++;
		}
		//object was detected at some point, but not visable anymore
		else if((objectState == 2) && ((irDist > 100.0) || (sonarDist > 100.0))){
			objectState = 3;									//NONE - object not visable anymore
			objectLocation = ((degree-2) + beginDegree) / 2; 	//location of object in degrees (endingDegree - beginningDegree)/2
			angSize = (degree-2) - beginDegree;					//angular size of object
			actualDist = (rawDist/((float) objectIter));		//actual or averaged sensor distance away from object
			rawDist = 0.0;										//reset raw distance to be used for new object
			objectIter = 0;

			//calc. width of object, convert degrees to radians (degree*pi/180) for tanf() function
			angWidth = 2 * actualDist * tanf((angSize*M_PI)/(2.0*180));

			//Check for smallest object
			/*
			//first object seen, so set all variables for smallest object
			if(objectCount == 1){
				smallestWidth = angWidth;
				smallestDist = actualDist;
				smallestLocation = objectLocation;
				smallestIndex = objectCount;
				smallestSize = angSize;
			}
			//check for smallest object, and not first object seen
			else if ((objectCount > 1) && (angWidth < smallestWidth)){
				smallestWidth = angWidth;
				smallestDist = actualDist;
				smallestLocation = objectLocation;
				smallestIndex = objectCount;
				smallestSize = angSize;
			}
			*/
		}
		//Polar to Cartesian calculations for RadialPlot in excel
		polar2Cart(degree, irDist, sonarDist, xCartesian, yCartesian);
	}
	//send Arrays to .txt file
	txtOutput(xCartesian, yCartesian);
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
            sweep() ;
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



