/**
*	@file open_interface.h
*
*	@brief Header file containing all functionality to interface with the
* 	IRobot Create V2 Communication over UART4 at 115200.
*
* 	@author Noah Bergman
* 	@date 03/11/2016
*/

#ifndef OPEN_INTERFACE_H_
#define OPEN_INTERFACE_H_

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include "Timer.h"
#include "lcd.h"
#include <inc/tm4c123gh6pm.h>

#define M_PI 3.14159265358979323846

/// iRobot Create Sensor Data
typedef struct {
	//Boolean sensor values
	uint32_t wheelDropLeft : 1;
	uint32_t wheelDropRight : 1;
	uint32_t bumpLeft : 1;
	uint32_t bumpRight : 1;
	uint32_t cliffLeft : 1;
	uint32_t cliffFrontLeft : 1;
	uint32_t cliffFrontRight : 1;
	uint32_t cliffRight : 1;

	uint32_t lightBumperRight : 1;
	uint32_t lightBumperFrontRight : 1;
	uint32_t lightBumperCenterRight : 1;
	uint32_t lightBumperCenterLeft : 1;
	uint32_t lightBumperFrontLeft : 1;
	uint32_t lightBumperLeft : 1;

	uint32_t wallSensor : 1;
	uint32_t virtualWall : 1;

	uint32_t overcurrentLeftWheel : 1;
	uint32_t overcurrentRightWheel : 1;
	uint32_t overcurrentMainBrush : 1;
	uint32_t overcurrentSideBrush : 1;

	uint32_t buttonClock : 1;
	uint32_t buttonSchedule : 1;
	uint32_t buttonDay : 1;
	uint32_t buttonHour : 1;
	uint32_t buttonMinute : 1;
	uint32_t buttonDock : 1;
	uint32_t buttonSpot : 1;
	uint32_t buttonClean : 1;

	//Cliff sensors
	uint16_t cliffLeftSignal;
	uint16_t cliffFrontLeftSignal;
	uint16_t cliffFrontRightSignal;
	uint16_t cliffRightSignal;

	//Light bump sensors
	uint16_t lightBumpLeftSignal;
	uint16_t lightBumpFrontLeftSignal;
	uint16_t lightBumpCenterLeftSignal;
	uint16_t lightBumpCenterRightSignal;
	uint16_t lightBumpFrontRightSignal;
	uint16_t lightBumpRightSignal;

	//Misc sensors
	uint16_t wallSignal;
	uint8_t dirtDetect;

	//Power
	int16_t leftMotorCurrent;
	int16_t rightMotorCurrent;
	int16_t mainBrushMotorCurrent;
	int16_t sideBrushMotorCurrent;

	//Motion sensors
	int16_t distance;
	int16_t angle;
	int8_t requestedVelocity;
	int8_t requestedRadius;
	int16_t requestedRightVelocity;
	int16_t requestedLeftVelocity;
	uint16_t leftEncoderCount;        //here the encoder counts were made unsigned
	uint16_t rightEncoderCount;		  //made unsigned

	//Information from the infrared beacon sensors
	char infraredCharOmni;
	char infraredCharLeft;
	char infraredCharRight;

	//Battery information
	uint8_t chargingState;
	uint8_t chargingSourcesAvailable;
	uint16_t batteryVoltage;
	int16_t batteryCurrent;
	uint8_t batteryTemperature;
	uint16_t batteryCharge;
	uint16_t batteryCapacity;

	//Music
	uint8_t songNumber;
	uint8_t songPlaying;

	//Misc
	uint8_t oiMode;
	uint8_t numberOfStreamPackets;
	uint8_t stasis;

} oi_t;


///Allocate and clear all memory for OI Struct
oi_t * oi_alloc();

///Free memory from pointer to Open Interface Struct
void oi_free(oi_t *self);


///Initialize open interface
void oi_init(oi_t *self);

void oi_close();

///Update sensor data
void oi_update(oi_t *self);

/// \brief Set the LEDS on the Create
/// \param play_led 0=off, 1=on
/// \param advance_led 0=off, 1=on
/// \param power_color (0-255), 0=green, 255=red
/// \param power_intensity (0-255) 0=off, 255=full intensity
void oi_setLeds(uint8_t play_led, uint8_t advance_led, uint8_t power_color, uint8_t power_intensity);

/// \brief Set direction and speed of the robot's wheels
/// \param linear velocity in mm/s values range from -500 -> 500 of right wheel
/// \param linear velocity in mm/s values range from -500 -> 500 of left wheel
void oi_setWheels(int16_t right_wheel, int16_t left_wheel);


/// \brief Load song sequence
/// \param An integer value from 0 - 15 that acts as a label for note sequence
/// \param An integer value from 1 - 16 indicating the number of notes in the sequence
/// \param A pointer to a sequence of notes stored as integer values
/// \param A pointer to a sequence of durations that correspond to the notes
void oi_loadSong(int song_index, int num_notes, unsigned char  *notes, unsigned char  *duration);

/// \brief Play song
/// \param An integer value from 0 - 15 that is a previously establish song index
void oi_play_song(int index);

/// Calls in built in demo to send the iRobot to an open home base
/// This will cause the iRobot to enter the Passive state
void go_charge(void);

char* oi_checkFirmware();

//initializes interrupt and gpio to handle button press to end OI
void oi_shutoff_init(void);

//used to handle interrupt to shut off OI
void GPIOF_Handler(void);

//move forward
void move_forward(oi_t *sensor, int centimeters);

//move backwards
void move_backwards(oi_t *sensor, int centimeters);

//turn clockwise
void turn_clockwise(oi_t *sensor, int degrees);

//turn counter clockwise
void turn_counterClockwise(oi_t *sensor, int degrees);

//used to get the current moved degrees from encoder count
int getDegrees(oi_t *self);

#endif /* OPEN_INTERFACE_H_ */
