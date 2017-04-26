/**
*  @file helpers.h
*  @brief this header file will contain all required helper functions
*  used for formating object data and output to Putty.
*
*  @author	Brian Bradford, Nick Knuth, Andrew Thai, and Rajiv Bhoopala
*
*  @date	4/12/2017
*/

#ifndef HELPERS_H_
#define HELPERS_H_

#include <inc/tm4c123gh6pm.h>
#include "commands.h"

#define M_PI 3.14159265358979323846

/**
*	Convert polar coordinates to Cartesian, and cap distances to 100 cm.
*	@author Brian Bradford, Nick Knuth, Andrew Thai, and Rajiv Bhoopala
* 	@param degree	current degree of servo (0-180)
* 	@param ir		ir sensor distance
*	@param sonar		sonar sensor distance
*	@param *x		Pointer to array to storing xCartesian distance
*	@param *y		Pointer to array to storing yCartesian distance
*	@date 4/12/2017
*/
void polar2Cart(int degree, float ir, float sonar, float *x, float *y);

/**
* 	Sets the proper data for each object detected to be outputted to Putty later.
* 	@author Brian Bradford, Nick Knuth, Andrew Thai, and Rajiv Bhoopala
* 	@param *object_data		Pointer to struct containing detected objects data
* 	@param objectCount		Total count of objects seen in sweep
* 	@param angWidth			Actual angular width/linear size of object
*	@param actualDist		Actual distance in centimeters from object
* 	@param objectLocation	Location of object in degrees
*	@date 4/12/2017
*/
void setObjectData(struct object *object_data, int objectCount, float angWidth, float actualDist, int objectLocation);

/**
* 	Write cartesian values from arrays capped at a range of 100 cm to Putty for use of Radial plot in Putty.
* 	@author Brian Bradford, Nick Knuth, Andrew Thai, and Rajiv Bhoopala
* 	@param *x		Pointer to array to storing xCartesian distance
* 	@param *y		Pointer to array to storing yCartesian distance
* 	@date 4/12/2017
*/
void arrayOutput(float *x, float *y);

/**
* 	Print out a 2 dimensional array in Putty, based off of a radial plot for object detection.
* 	@author Brian Bradford, Nick Knuth, Andrew Thai, and Rajiv Bhoopala
*	@param *x		Pointer to array to storing xCartesian distance
* 	@param *y		Pointer to array to storing yCartesian distance
* 	@date 4/12/2017
*/
void arrayGridPutty(float *x, float *y);

/**
* 	Output info. from struct with object data to Putty.
* 	@author Brian Bradford, Nick Knuth, Andrew Thai, and Rajiv Bhoopala
* 	@param *object_data		Pointer to struct containing detected objects data
* 	@param objectCount		Total count of objects seen in sweep
* 	@date 4/12/2017
*/
void objectDataOutput(struct object *object_data, int objectCount);

#endif /* HELPERS_H_ */
