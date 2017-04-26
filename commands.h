/**
*  @file commands.h
*  @brief this header file will contain all required commands
*  for the Roomba to execute.
*
*  @author	Brian Bradford, Nick Knuth, Andrew Thai, and Rajiv Bhoopala
*
*  @date	4/12/2017
*/

#ifndef COMMANDS_H_
#define COMMANDS_H_

#include <math.h>
#include <inc/tm4c123gh6pm.h>
#include "open_interface.h"

#define M_PI 3.14159265358979323846

//Struct for detected object data.
struct object{
	float width;
	float distance;
	int count;
	int location;
};

/**
* 	Sweep with both the Ping sensor and IR distance sensor over the 180 degree range of the servo,
* 	taking distance measurements from both sensors every 2 degrees and transmitting the data over WiFi to Putty.
* 	@author	Brian Bradford, Nick Knuth, Andrew Thai, and Rajiv Bhoopala
* 	@date 4/12/2017
*/
void sweep();

/**
*	This method will be used to receive commands from PS4 controller on the Roomba to execute,
*	and output feedback back to Putty.
*	@author	Brian Bradford, Nick Knuth, Andrew Thai, and Rajiv Bhoopala
*	@date 4/12/2017
*/
void move_roomba();

/**
*	This method will be used to move the Roomba forward a given distance
*	while checking for a white line, cliff, or object bump.
*	@author	Brian Bradford, Nick Knuth, Andrew Thai, and Rajiv Bhoopala
*	@param	*sensor			Pointer to struct containing Roomba sensor data
*	@param 	centimeters		Distance for Roomba to move
*	@date 4/12/2017
*/
void move_forward(oi_t *sensor, int centimeters);

/**
*	This method will be used to move the Roomba backwards a given distance.
*	@author	Brian Bradford, Nick Knuth, Andrew Thai, and Rajiv Bhoopala
*	@param	*sensor			Pointer to struct containing Roomba sensor data
*	@param 	centimeters		Distance for Roomba to move
*	@date 4/12/2017
*/
void move_backwards(oi_t *sensor, int centimeters);

/**
*	This method will be used to rotate the Roomba counterClockwise a given amount of degrees.
*	@author	Brian Bradford, Nick Knuth, Andrew Thai, and Rajiv Bhoopala
*	@param	*sensor		Pointer to struct containing Roomba sensor data
*	@param 	degrees		Degrees for Roomba to rotate
*	@date 4/12/2017
*/
void turn_counterClockwise(oi_t *sensor, int degrees);

/**
*	This method will be used to rotate the Roomba clockwise a given amount of degrees.
*	@author	Brian Bradford, Nick Knuth, Andrew Thai, and Rajiv Bhoopala
*	@param	*sensor		Pointer to struct containing Roomba sensor data
*	@param 	degrees		Degrees for Roomba to rotate
*	@date 4/12/2017
*/
void turn_clockwise(oi_t *sensor, int degrees);

/**
*	This method will be used to check the state of the bumper sensors.
*	@author	Brian Bradford, Nick Knuth, Andrew Thai, and Rajiv Bhoopala
*	@param	*sensor		Pointer to struct containing Roomba sensor data
*	@date 4/12/2017
*/
int checkBumper(oi_t *sensor);

/**
*	This method will be used to check the state of the cliff sensors.
*	@author	Brian Bradford, Nick Knuth, Andrew Thai, and Rajiv Bhoopala
*	@param	*sensor		Pointer to struct containing Roomba sensor data
*	@date 4/12/2017
*/
int checkCliff(oi_t *sensor);

#endif /* COMMANDS_H_ */
