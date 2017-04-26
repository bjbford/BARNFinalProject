/**
*  @file helpers.c
*  @brief this file will contain all required helper functions
*  used for formating object data and output to Putty.
*
*  @author	Brian Bradford, Nick Knuth, Andrew Thai, and Rajiv Bhoopala
*
*  @date	4/12/2017
*/

#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include "helpers.h"
#include "uart.h"
#include "commands.h"

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
void polar2Cart(int degree, float ir, float sonar, float *x, float *y){
    int i = degree/2;
    //Cap at 100 cm if distance is > 100.0 cm
    if((ir > 100.0) || (sonar > 100.0)){
        x[i] = 100.0 * cosf((degree*M_PI)/(180.0));
        y[i] = 100.0 * sinf((degree*M_PI)/(180.0));
    }
    //Good distance data
    else{
        x[i] = sonar * cosf((degree*M_PI)/(180.0));
        y[i] = sonar * sinf((degree*M_PI)/(180.0));
    }
}

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
void setObjectData(struct object *object_data, int objectCount, float angWidth, float actualDist, int objectLocation){
    int i=0;
    //Detected object data
    switch(objectCount){
    //Default case, no objected detected. Array initialized to 0.
    case 0:
        for(i=0;i<6;i++){
            object_data[i].width = 0.0;
            object_data[i].distance = 0.0;
            object_data[i].count = 0;
            object_data[i].location = 0;
        }
    //first object detected
    case 1:
        object_data[(objectCount-1)].width = angWidth;
        object_data[(objectCount-1)].distance = actualDist;
        object_data[(objectCount-1)].count = objectCount;
        object_data[(objectCount-1)].location = objectLocation;
    break;
    //second object detected
    case 2:
        object_data[(objectCount-1)].width = angWidth;
        object_data[(objectCount-1)].distance = actualDist;
        object_data[(objectCount-1)].count = objectCount;
        object_data[(objectCount-1)].location = objectLocation;
    break;
    //third object detected
    case 3:
        object_data[(objectCount-1)].width = angWidth;
        object_data[(objectCount-1)].distance = actualDist;
        object_data[(objectCount-1)].count = objectCount;
        object_data[(objectCount-1)].location = objectLocation;
    break;
    //fourth object detected
    case 4:
        object_data[(objectCount-1)].width = angWidth;
        object_data[(objectCount-1)].distance = actualDist;
        object_data[(objectCount-1)].count = objectCount;
        object_data[(objectCount-1)].location = objectLocation;
    break;
    //fifth object detected
    case 5:
        object_data[(objectCount-1)].width = angWidth;
        object_data[(objectCount-1)].distance = actualDist;
        object_data[(objectCount-1)].count = objectCount;
        object_data[(objectCount-1)].location = objectLocation;
    break;
    //sixth object detected
    case 6:
        object_data[(objectCount-1)].width = angWidth;
        object_data[(objectCount-1)].distance = actualDist;
        object_data[(objectCount-1)].count = objectCount;
        object_data[(objectCount-1)].location = objectLocation;
    break;
    }
}

/**
* 	Write cartesian values from arrays capped at a range of 100 cm to Putty for use of Radial plot in Putty.
* 	@author Brian Bradford, Nick Knuth, Andrew Thai, and Rajiv Bhoopala
* 	@param *x		Pointer to array to storing xCartesian distance
* 	@param *y		Pointer to array to storing yCartesian distance
* 	@date 4/12/2017
*/
void arrayOutput(float *x, float *y){
    //Send xCartesian coordinate to Putty in form of string
    uart_sendStr("xCartesian (capped at 100 cm)\r\n");
    int i=0;
    for(i = 0;i<91;i++){
        char xBuff[50] = "\0";
        snprintf(xBuff,50,"%f",x[i]);
        uart_sendStr(xBuff);
        uart_sendStr("\r\n");
    }

    //Space between x and y data
    uart_sendStr("\r\n");

    uart_sendStr("yCartesian (capped at 100 cm)\r\n");
    //Send xCartesian coordinate to Putty in form of string
    int j=0;
    for(j = 0;j<91;j++){
        char yBuff[50] = "\0";
        snprintf(yBuff,50,"%f",y[j]);
        uart_sendStr(yBuff);
        uart_sendStr("\r\n");
    }
}

/**
* 	Print out a 2 dimensional array in Putty, based off of a radial plot for object detection.
* 	@author Brian Bradford, Nick Knuth, Andrew Thai, and Rajiv Bhoopala
*	@param *x		Pointer to array to storing xCartesian distance
* 	@param *y		Pointer to array to storing yCartesian distance
* 	@date 4/12/2017
*/
void arrayGridPutty(float *x, float *y){
    int i = 0, j = 0;
    char map[1][200];
    //Final Y value to search for in cartesian array
    for(i=100; i >= 0; i--)
    {
        //Flushing row string after each row
        for(j = 0; j < 200; j++)
        {
            map[0][j] = ' ';
        }
        int abs_y = i;
        //Iterating through the y cartesian value to find match
        for(j = 0; j < 91; j++)
        {
            int tmp_y =  (int) roundf(y[j]);
            if(tmp_y < 0)
            {
                tmp_y = 0;
            }
            //If match found set character at x value found at that y value
            if(tmp_y == abs_y)
            {
                int abs_x = 100 + (int) roundf(x[j]);

                map[0][abs_x] = 'X';
            }
        }
        //print row at end of search for matching y value
        int k=0;
        for(k=0; k < 200; k++)
        {
            uart_sendChar(map[0][k]);
        }
        uart_sendChar('\n');
        uart_sendChar('\r');
    }
}

/**
* 	Output info. from struct with object data to Putty.
* 	@author Brian Bradford, Nick Knuth, Andrew Thai, and Rajiv Bhoopala
* 	@param *object_data		Pointer to struct containing detected objects data
* 	@param objectCount		Total count of objects seen in sweep
* 	@date 4/12/2017
*/
void objectDataOutput(struct object *object_data, int objectCount){
    char string[20];
    snprintf(string, 20, "\r\n%d: objects!\r\n", objectCount);
    uart_sendStr(string);
    int i=0;
    for(i=0;i<objectCount;i++){
        //Output location to Putty
        uart_sendStr("Location: ");
        char locationBuff[4] = "\0";
        snprintf(locationBuff,4,"%d",object_data[i].location);
        uart_sendStr(locationBuff);
        uart_sendStr("\r\n");

        //Output distance to Putty
        uart_sendStr("Distance: ");
        char distanceBuff[50] = "\0";
        snprintf(distanceBuff,50,"%.2f",object_data[i].distance);
        uart_sendStr(distanceBuff);
        uart_sendStr("\r\n");

        //Output width to Putty
        uart_sendStr("Width: ");
        char widthBuff[50] = "\0";
        snprintf(widthBuff,50,"%.2f",object_data[i].width);
        uart_sendStr(widthBuff);
        uart_sendStr("\r\n\r\n");
    }
}
