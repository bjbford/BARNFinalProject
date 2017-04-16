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

/**
 * Convert polar coordinates to Cartesian.
 */
void polar2Cart(int degree, float ir, float sonar, float *x, float *y){
	int i = degree/2;
	if(ir > 100.0){
		x[i] = 100.0 * cosf((degree*M_PI)/(2.0*180));
		y[i] = 100.0 * sinf((degree*M_PI)/(2.0*180));
	}
	else{
		x[i] = sonar * cosf((degree*M_PI)/(2.0*180));
		y[i] = sonar * sinf((degree*M_PI)/(2.0*180));
	}
}


/**
 * Write cartesian values capped at a range of 100 cm to a .txt file for use of Radial plot in Excel.
 */
void txtOutput(float *x, float *y){
	//Create .txt file called "dist.txt" to write into
	FILE *file = fopen("dist.txt","w");

	//write xCartesian Array values to "dist.txt" file
	int i = 0;
	for(i=0;i<sizeof(x);i++){
	    fprintf(file, "%f",x[i]);
	}

	//print a new line to space out x and y
	char newLine[] = "\r\n";
	fprintf(file, "%s",newLine);

	//write yCartesian Array values to "dist.txt" file
	int j = 0;
	for(j=0;i<sizeof(y);j++){
	    fprintf(file, "%f",y[j]);
	}

	fclose(file);
}
