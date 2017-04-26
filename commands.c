/**
*  @file commands.c
*  @brief this file will contain all required commands for
*  the Roomba to execute.
*
*  @author	Brian Bradford, Nick Knuth, Andrew Thai, and Rajiv Bhoopala
*
*  @date	4/12/2017
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
* 	Sweep with both the Ping sensor and IR distance sensor over the 180 degree range of the servo,
* 	taking distance measurements from both sensors every 2 degrees and transmitting the data over WiFi to Putty.
* 	@author	Brian Bradford, Nick Knuth, Andrew Thai, and Rajiv Bhoopala
* 	@date 4/12/2017
*/
void sweep(){
	int degree=0, beginDegree = 0, angSize = 0;
	int objectLocation = 0;		//location of object in degrees
	int objectCount = 0;		//total count of object
	float rawDist = 0.0;		//used to average out distance of object
	float actualDist = 0.0;		//actual distance in centimeters from object
	float angWidth = 0.0;		//actual angular width/linear size of object
	int objectIter = 0;			//used to divide raw distance for average distance of an object

	/**
	 *  OBJECT STATES (7):
	 *
	 *  BEGIN = 0: no objects have been detected. (iterate servo)
	 *  FIRSTDETECT = 1: first data point that could be an object. *start values* (Prev. state == BEGIN || NOTOBJECT || NONE)
	 *  STILLDETECT = 2: second data point that could be an object. *iterate values* (Prev. state == FIRSTDETECT)
	 *  OBJECT = 3: third data point, so definitely an object since angular size is >= 4 degrees. *iterate values and count object* (Prev. state == STILLDETECT)
	 *  NOTOBJECT = 4: Cant be an object because we didn't point three data points. *reset values* (Prev. state == FIRSTDETECT || STILLDETECT)
	 *  STILLOBJECT = 5: fourth or more data point. Already know and counted it as an object *iterate values* (Prev. state == OBJECT || STILLOBJECT)
	 *  NONE = 6: Object was detected, but not seen anymore. *do math and save values in object_data* (Prev. state == OBJECT || STILL OBJECT)
	 */
	int objectState = 0;

	//Arrays to store Cartesian distance to be outputed to Putty.
	float xCartesian[92] = {};
	float yCartesian[92] = {};

	//Data for each object detected to send over Putty (assumption that we max detect 6 objects)
	struct object object_data[6];

	for(degree=0; degree<=180; degree+=2){
		move_servo(degree);	//move 2 degrees
		timer_waitMillis(10);
		float irDist = ir_getDistance();
		float sonarDist = ping_getDistance();

		//lcd_printf("Object State: %d\nIR (cm): %.1f\nSonar (cm): %.1f\nObject Count: %d",objectState, irDist, sonarDist, objectCount);

		//FIRST DETECT STATE
		if((sonarDist < 100.0) && (irDist < 100.0) && ((objectState == 0) || (objectState == 4) || (objectState == 6))){
			//start values
			objectState = 1;
			beginDegree = degree;
			rawDist += sonarDist;
			objectIter++;
		}
		//STILL DETECT STATE
		else if((irDist < 100.0) && (sonarDist < 100.0) && (objectState == 1)){
			//object still detected (not first time seeing it)
			objectState = 2;
			rawDist += sonarDist;
			objectIter++;
		}
		//OBJECT STATE
		else if((sonarDist < 100.0) && (irDist < 100.0) && (objectState == 2)){
			objectState = 3;
			rawDist += sonarDist;
			objectIter++;
			objectCount++;
		}
		//NOT OBJECT STATE
		else if(((sonarDist > 100.0) || (irDist > 100.0)) && ((objectState == 1) || (objectState == 2))){
		    //Reset values
		    if(objectCount < 1){
		        objectState = 0;
		    }
		    else{
		        objectState = 4;
		    }
		    rawDist = 0.0;
		    objectIter = 0;
		}
		//STILL OBJECT STATE
		else if((sonarDist < 100.0) && (irDist < 100.0) && (objectState == 3 || (objectState == 5))){
		    //iterate values
			objectState = 5;
			rawDist += sonarDist;
			objectIter++;
		}
		//NONE STATE
		else if(((sonarDist > 100.0) || (irDist > 100.0)) && (objectState == 3 || (objectState == 5))){
			objectState = 6;
			objectLocation = ((degree-2) + beginDegree) / 2; 	//location of object in degrees (endingDegree - beginningDegree)/2
			angSize = (degree-2) - beginDegree;					//angular size of object
			actualDist = (rawDist/((float) objectIter));		//actual or averaged sensor distance away from object
			rawDist = 0.0;										//reset raw distance to be used for new object
			objectIter = 0;										//reset iterator to be used for new object

			//calculate width of object, convert degrees to radians (degree*pi/180) for tanf() function
			angWidth = 2 * actualDist * tanf((angSize*M_PI)/(2.0*180));

			//Set detected object data
			setObjectData(object_data, objectCount, angWidth, actualDist, objectLocation);
		}

		//Polar to Cartesian calculations for RadialPlot in excel
		polar2Cart(degree, irDist, sonarDist, xCartesian, yCartesian);
	}
	//send Arrays of x and y cartesian coordinates to Putty
	//arrayOutput(xCartesian, yCartesian);
	//timer_waitMillis(2000);
	arrayGridPutty(xCartesian, yCartesian);
	timer_waitMillis(3000);
	//send struct of object_data to Putty
	objectDataOutput(object_data, objectCount);
	move_servo(0);
	timer_waitMillis(1000);
}

/**
*	This method will be used to receive commands from PS4 controller on the Roomba to execute,
*	and output feedback back to Putty.
*	@author	Brian Bradford, Nick Knuth, Andrew Thai, and Rajiv Bhoopala
*	@date 4/12/2017
*/
void move_roomba ()
{
       oi_t *sensor_data = oi_alloc();
       oi_init(sensor_data);
       char input;

        input = uart_receive();
        //forwards
        if (input == 0x31)
        {
            char moveForward[] = "Moving Forward\r\n";
            uart_sendStr(moveForward);
            move_forward(sensor_data, 25); //move forward

        }
        //backwards
        else if (input == 0x32)
        {
            char moveBackwards[] = "Moving Backwards\r\n";
            uart_sendStr(moveBackwards) ;
            move_backwards(sensor_data, -100); //backwards
        }

        //rotates clockwise
        else if (input == 0x33)
        {
            char turnRight[] = "Rotating Clockwise 10 degrees\r\n";
            uart_sendStr(turnRight);
            turn_clockwise(sensor_data, 5) ;//rotate 90 degrees

        }

        //rotates counterclockwise
        else if (input == 0x34)
        {
            char turnLeft[] = "Rotating CountClockwise 10 degrees\r\n";
            uart_sendStr(turnLeft) ;
            turn_counterClockwise(sensor_data, 5) ;
        }

        //scan surroundings
        else if (input == 0x35)
        {
            char scanning[] = "Scanning..... please wait......\r\n";
            uart_sendStr(scanning);
            sweep() ;
        }

        //play song
        else if (input == 0x36)
        {
            char songStatement[] = "Playing Song\r\n";
            uart_sendStr(songStatement) ;

            unsigned char notes[36] = {67,66,67,64,65,66,67,72,76,76,74,76,77,71,74,72};
            unsigned char lengths[36] = {17,17,17,102,51,51,102,51,38,13,51,51,51,51,102,51};
            oi_loadSong(0, 16, notes, lengths);

            oi_play_song(0);

        }

        oi_free(sensor_data);
}

/**
*	This method will be used to move the Roomba forward a given distance
*	while checking for a white line, cliff, or object bump.
*	@author	Brian Bradford, Nick Knuth, Andrew Thai, and Rajiv Bhoopala
*	@param	*sensor			Pointer to struct containing Roomba sensor data
*	@param 	centimeters		Distance for Roomba to move
*	@date 4/12/2017
*/
void move_forward(oi_t *sensor, int centimeters){
    int sum = 0 ;
    while (sum < centimeters)
    {
        int leftSignal = sensor -> cliffLeftSignal;
        int rightSignal = sensor -> cliffRightSignal;
        int frontLeftSignal = sensor -> cliffFrontLeftSignal;
        int frontRightSignal = sensor -> cliffFrontRightSignal;
        oi_setWheels(75,75) ; //move forward
        oi_update(sensor);
        sum += sensor->distance;

        //Finish line detection
        if(leftSignal < 1200)
		{
        	char* warning = "Finish is on Left Side! \n\r";
        	uart_sendStr(warning);
		}
	    if(rightSignal < 1200)
	    {
	    	char* warning = "Finish is on RIGHT side! \n\r";
	    	uart_sendStr(warning);
	    }
	    if(frontLeftSignal < 1200)
	    {
	    	char* warning = "Finish is on Front Left side! \n\r";
	    	uart_sendStr(warning);
	    }
	    if(frontRightSignal < 1200)
	    {
	    	char* warning = "Finish is on Front Right side! \n\r";
	    	uart_sendStr(warning);
	    }

	    //White line (boundary) detection
        if(leftSignal > 2500)
        {
            char* warning = "White Line on Left Side! \n\r";
            uart_sendStr(warning);
            oi_setWheels(0,0);
            break;
        }
        if(rightSignal > 2600)
        {
            char* warning = "White Line on RIGHT side! \n\r";
            uart_sendStr(warning);
            oi_setWheels(0,0);
            break ;
        }
        if(frontLeftSignal > 2600)
        {
            char* warning = "White Line on Front Left side! \n\r";
            uart_sendStr(warning);
            oi_setWheels(0,0);
            break ;
        }
        if(frontRightSignal > 2600)
        {
            char* warning = "White Line on Front Right side! \n\r";
            uart_sendStr(warning);
            oi_setWheels(0,0);
            break ;
        }

        //Bumper hit object detection
        if (checkBumper(sensor) == 1)
        {
            char* warning = "Hit right bumper! \n\r";
            uart_sendStr(warning);
            oi_setWheels(0,0);
            break ;
        }
        if (checkBumper(sensor) == 2)
        {
            char* warning = "Hit front right bumper! \n\r";
            uart_sendStr(warning);
            oi_setWheels(0,0);
            break ;
        }
        if (checkBumper(sensor) == 3)
        {
			char* warning = "Hit center right bumper! \n\r";
			uart_sendStr(warning);
			oi_setWheels(0,0);
			break ;
		}
        if (checkBumper(sensor) == 4)
		{
			char* warning = "Hit center left bumper! \n\r";
			uart_sendStr(warning);
			oi_setWheels(0,0);
			break ;
		}
        if (checkBumper(sensor) == 5)
		{
			char* warning = "Hit front left bumper! \n\r";
			uart_sendStr(warning);
			oi_setWheels(0,0);
			break;
		}
        if (checkBumper(sensor) == 6)
		{
			char* warning = "Hit left bumper! \n\r";
			uart_sendStr(warning);
			oi_setWheels(0,0);
			break;
		}

        //Cliff detection check
        if (checkCliff(sensor) == 1)
        {
            char* warning = "WARNING CLIFF ON LEFT SIDE! \n\r";
            uart_sendStr(warning);
            oi_setWheels(0,0);
            break;
        }
        if (checkCliff(sensor) ==2)
        {
            char* warning = "WARNING CLIFF ON RIGHT SIDE! \n\r";
            uart_sendStr(warning);
            oi_setWheels(0,0);
            break;
        }
        if (checkCliff(sensor) ==3)
        {
            char* warning = "WARNING CLIFF IN FRONT ON LEFT SIDE! \n\r";
            uart_sendStr(warning);
            oi_setWheels(0,0);
            break;
        }
        if (checkCliff(sensor) == 4)
        {
            char* warning = "WARNING CLIFF IN FRONT ON RIGHT SIDE \n\r";
            uart_sendStr(warning);
            oi_setWheels(0,0);
            break;
        }
        timer_waitMillis(100);
    }

    oi_setWheels(0,0); //stop wheels
    oi_free(sensor);
}

/**
*	This method will be used to move the Roomba backwards a given distance.
*	@author	Brian Bradford, Nick Knuth, Andrew Thai, and Rajiv Bhoopala
*	@param	*sensor			Pointer to struct containing Roomba sensor data
*	@param 	centimeters		Distance for Roomba to move
*	@date 4/12/2017
*/
void move_backwards(oi_t *sensor, int centimeters)
{
    oi_setWheels(-100, -100); //move backward; half speed

    int sum = 0 ;

    while (sum > centimeters)
    {
        oi_update(sensor);

        sum += sensor->distance;
        timer_waitMillis(1);

    }

    oi_setWheels(0,0); //stop wheels
    oi_free(sensor);
}

/**
*	This method will be used to rotate the Roomba counterClockwise a given amount of degrees.
*	@author	Brian Bradford, Nick Knuth, Andrew Thai, and Rajiv Bhoopala
*	@param	*sensor		Pointer to struct containing Roomba sensor data
*	@param 	degrees		Degrees for Roomba to rotate
*	@date 4/12/2017
*/
void turn_counterClockwise(oi_t *sensor, int degrees){
    oi_setWheels(100, -100);
    timer_waitMillis(degrees*22);
    oi_setWheels(0, 0); //stop wheels
    oi_free(sensor) ;

}

/**
*	This method will be used to rotate the Roomba clockwise a given amount of degrees.
*	@author	Brian Bradford, Nick Knuth, Andrew Thai, and Rajiv Bhoopala
*	@param	*sensor		Pointer to struct containing Roomba sensor data
*	@param 	degrees		Degrees for Roomba to rotate
*	@date 4/12/2017
*/
void turn_clockwise(oi_t *sensor, int degrees){
    oi_setWheels(-100, 100);
    timer_waitMillis(degrees*22);
    oi_setWheels(0, 0); //stop wheels
    oi_free(sensor) ;
}

/**
*	This method will be used to check the state of the bumper sensors.
*	@author	Brian Bradford, Nick Knuth, Andrew Thai, and Rajiv Bhoopala
*	@param	*sensor		Pointer to struct containing Roomba sensor data
*	@date 4/12/2017
*/
int checkBumper(oi_t *sensor)
{
	/**
	 * Bumper States:
	 * bumper = 0, no bumps
	 * bumper = 1, right bumper
	 * bumper = 2, front right bumper
	 * bumper = 3, center right bumper
	 * bumper = 4, center left bumper
	 * bumper = 5, front left bumper
	 * bumper = 6, left bumper
	 */
    int bumper = 0;

    oi_update(sensor);
    if (sensor-> lightBumperLeft)
    {
        bumper = 6;
    }
    else if (sensor-> lightBumperFrontLeft)
    {
        bumper = 5;
    }
    else if (sensor-> lightBumperCenterLeft)
        {
            bumper = 4;
        }
    else if (sensor-> lightBumperCenterRight)
    {
        bumper = 3;
    }
    else if (sensor-> lightBumperFrontRight)
    {
        bumper = 2;
    }
    else if (sensor-> lightBumperRight)
    {
        bumper = 1;
    }
    else
    {
        bumper = 0;
    }

    return bumper;
}

/**
*	This method will be used to check the state of the cliff sensors.
*	@author	Brian Bradford, Nick Knuth, Andrew Thai, and Rajiv Bhoopala
*	@param	*sensor		Pointer to struct containing Roomba sensor data
*	@date 4/12/2017
*/
int checkCliff(oi_t *sensor)
{
    oi_update(sensor);

    /**
     *  0 = no cliff
     *  1 = cliffleft
     *  2 = cliffright
     *  3 = cliffront left
     *  4 = clifffront right
     */
    int cliff = 0;

    if(sensor-> cliffLeft)
    {
        cliff = 1;
    }
    else if(sensor -> cliffRight)
    {
        cliff = 2;
    }
    else if(sensor -> cliffFrontLeft)
    {
        cliff = 3;
    }
    else if (sensor -> cliffFrontRight)
    {
        cliff = 4;
    }

    else
    {
    	cliff = 0;
    }

    return cliff;
}
