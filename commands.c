/*
 * Commands for Robot to execute.
 *
 *  Created on: Apr 12, 2017
 *      Author: bjbford
 */

#include "helpers.h"
#include "commands.h"
#include "servo.h"
#include "ping.h"
#include "ir.h"

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
