/*
 * helpers.c
 *
 *  Created on: Apr 12, 2017
 *      Author: bjbford
 */

#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include "helpers.h"
#include "uart.h"
#include "commands.h"

/**
 * Convert polar coordinates to Cartesian, and cap distances to 100 cm.
 */
void polar2Cart(int degree, float ir, float sonar, float *x, float *y){
	int i = degree/2;
	if((ir > 100.0) || (sonar > 100.0)){
		x[i] = 100.0 * cosf((degree*M_PI)/(180.0));
		y[i] = 100.0 * sinf((degree*M_PI)/(180.0));
	}
	else{
		x[i] = sonar * cosf((degree*M_PI)/(180.0));
		y[i] = sonar * sinf((degree*M_PI)/(180.0));
	}
}


/**
 * Sets the proper data for each object detected to be outputted to Putty later.
 */
void setObjectData(struct object *object_data, int objectCount, float angWidth, float actualDist, int objectLocation){
	int i=0;
	//Detected object data
	switch(objectCount){
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
	//fourth object detected
	case 5:
		object_data[(objectCount-1)].width = angWidth;
		object_data[(objectCount-1)].distance = actualDist;
		object_data[(objectCount-1)].count = objectCount;
		object_data[(objectCount-1)].location = objectLocation;
	break;
	//fourth object detected
	case 6:
		object_data[(objectCount-1)].width = angWidth;
		object_data[(objectCount-1)].distance = actualDist;
		object_data[(objectCount-1)].count = objectCount;
		object_data[(objectCount-1)].location = objectLocation;
	break;
	//Default case, no objected detected. Array initialized to 0.
	default:
		for(i=0;i<6;i++){
			object_data[i].width = 0.0;
			object_data[i].distance = 0.0;
			object_data[i].count = 0;
			object_data[i].location = 0;
		}
	}
}

/**
 * Write cartesian values from arrays capped at a range of 100 cm to Putty for use of Radial plot in Excel.
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
 * Output info. from struct with object data to Putty.
 */
void objectDataOutput(struct object *object_data, int objectCount){
	char string[] = {};
	sprintf(string, "\r\n%d: Objects detected!\r\n", objectCount);
	uart_sendStr(string);
	int i=0;
	for(i=0;i<objectCount;i++){
		//Output location
		char locString[] = {};
		sprintf(locString, "Object %d location: ", object_data[i].count);
		uart_sendStr(locString);
		char locationBuff[4] = "\0";
		snprintf(locationBuff,4,"%d",object_data[i].location);
		uart_sendStr(locationBuff);
		uart_sendStr("\r\n");

		//Output distance
		char distString[] = {};
		sprintf(distString, "Object %d distance: ", object_data[i].count);
		uart_sendStr(distString);
		char distanceBuff[50] = "\0";
		snprintf(distanceBuff,50,"%.2f",object_data[i].distance);
		uart_sendStr(distanceBuff);
		uart_sendStr("\r\n");

		//Output width
		char widthString[] = {};
		sprintf(widthString, "Object %d width: ", object_data[i].count);
		uart_sendStr(widthString);
		char widthBuff[50] = "\0";
		snprintf(widthBuff,50,"%.2f",object_data[i].width);
		uart_sendStr(widthBuff);
		uart_sendStr("\r\n\r\n");
	}
}
