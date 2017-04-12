/*
 * helpers.h
 *
 *  Created on: Apr 12, 2017
 *      Author: bjbford
 */

#ifndef HELPERS_H_
#define HELPERS_H_

#include <inc/tm4c123gh6pm.h>

#define M_PI 3.14159265358979323846

void polar2Cart(int degree, float ir, float sonar, float *x, float *y);
void txtOutput(float x[], float y[]);

#endif /* HELPERS_H_ */
