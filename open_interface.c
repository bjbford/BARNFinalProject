/*
 * open_interface.c
 *
 *  Created on: Mar 11, 2016
 *      Author: nbergman
 */


/*
 * Open Interface
 *
 *  Created on: Mar 3, 2016
 *      Author: Noah Bergman, Eric Middleton, dmlarson
 */

#include "open_interface.h"

#define OI_OPCODE_START            128
#define OI_OPCODE_BAUD             129
#define OI_OPCODE_CONTROL          130
#define OI_OPCODE_SAFE             131
#define OI_OPCODE_FULL             132
#define OI_OPCODE_POWER            133		//POWER DOWN ROOMBA
#define OI_OPCODE_SPOT             134
#define OI_OPCODE_CLEAN            135
#define OI_OPCODE_MAX              136
#define OI_OPCODE_DRIVE            137
#define OI_OPCODE_MOTORS           138
#define OI_OPCODE_LEDS             139
#define OI_OPCODE_SONG             140
#define OI_OPCODE_PLAY             141
#define OI_OPCODE_SENSORS          142
#define OI_OPCODE_FORCEDOCK        143

#define OI_OPCODE_PWM_MOTORS       144
#define OI_OPCODE_DRIVE_WHEELS     145
#define OI_OPCODE_DRIVE_PWM        146
#define OI_OPCODE_OUTPUTS          147
#define OI_OPCODE_STREAM           148
#define OI_OPCODE_QUERY_LIST       149
#define OI_OPCODE_DO_STREAM        150
#define OI_OPCODE_SEND_IR_CHAR     151
#define OI_OPCODE_SCRIPT           152
#define OI_OPCODE_PLAY_SCRIPT      153
#define OI_OPCODE_SHOW_SCRIPT      154
#define OI_OPCODE_WAIT_TIME        155
#define OI_OPCODE_WAIT_DISTANCE    156
#define OI_OPCODE_WAIT_ANGLE       157
#define OI_OPCODE_WAIT_EVENT       158

#define OI_OPCODE_RESET				7	//RESET ROOMBA - GO TO
#define OI_OPCODE_STOP				173
#define OI_OPCODE_SCHEDULE			167
#define OI_OPCODE_SCHED_LED			162	//MONDAY THROUGH SUNDAY LEDS
#define OI_OPCODE_7SEG				163


// Contains Packets 7-26
#define OI_SENSOR_PACKET_GROUP0 0
// Contains Packets 7-16
#define OI_SENSOR_PACKET_GROUP1 1
// Contains Packets 17-20
#define OI_SENSOR_PACKET_GROUP2 2
// Contains Packets 21-26
#define OI_SENSOR_PACKET_GROUP3 3
// Contains Packets 27-34
#define OI_SENSOR_PACKET_GROUP4 4
// Contains Packets 35-42
#define OI_SENSOR_PACKET_GROUP5 5
// Contains Packets 7-42
#define OI_SENSOR_PACKET_GROUP6 6
// Contains Packets 7-58 For Use With Create 2 Only
#define OI_SENSOR_PACKET_GROUP100 100
// Contains Packets 43-58 For Use With Create 2 Only
#define OI_SENSOR_PACKET_GROUP101 101
// Contains Packets 46-51 For Use With Create 2 Only
#define OI_SENSOR_PACKET_GROUP106 106
// Contains Packets 54-58 For Use With Create 2 Only
#define OI_SENSOR_PACKET_GROUP107 107

#define SENSOR_PACKET_SIZE	80


/// Initialize the iRobot open interface without updating a struct
/// internal function
void oi_init_noupdate(void);

///	\brief Initialize UART3 for OI Communication and Debugging
///	internal function
void oi_uartInit(void);

///Set baud to 115200
void oi_uartFastMode(void);

///transmit character
///	internal function
void oi_uartSendChar(char data);

///transmit character array
///	internal function
void uart_sendStr(const char *theData);

///Receive from UART
///	internal function
char oi_uartReceive(void);

///Parse data from iRobot into oi_t struct
void oi_parsePacket(oi_t* self, uint8_t packet[]);

///Send large data set from array
///	internal function
void oi_uartSendBuff(const uint8_t theData[], uint8_t theSize);

///Helper function to convert big-endian integer from pointer into little endian integer
/// internal function
int16_t oi_parseInt(uint8_t* theInt);

///Allocate and clear all memory for OI Struct
oi_t* oi_alloc()
{
	return calloc(1, sizeof(oi_t));
}


///Free memory from pointer to Open Interface Struct
void oi_free(oi_t *self)
{
	free(self);

	//Send the stop command to the iRobot
	oi_close();
}

void oi_init_noupdate()
{
	oi_uartInit();
	oi_uartSendChar(OI_OPCODE_START);

	oi_uartSendChar(OI_OPCODE_FULL);		//Use full mode, unrestricted control
	oi_setLeds(1,1,7,255);

	oi_shutoff_init(); //allows for pushbutton SW2 on PF0 to kill oi

}


/**
 * Initialize open interface communication with IRobot.
 *
 * This function needs to be called to setup UART and other
 * OI peripherals
 *
 *
 */
void oi_init(oi_t *self)
{
	oi_init_noupdate();

	oi_update(self);
	oi_update(self); //Call twice to clear distance/angle

}

void oi_close() {
	oi_setWheels(0, 0);
	oi_uartSendChar(OI_OPCODE_STOP);
}

///Update all sensor and store in oi_t struct
void oi_update(oi_t *self)
{
	uint8_t sensorBuffer[SENSOR_PACKET_SIZE];

	//Query list of sensors
	oi_uartSendChar(OI_OPCODE_SENSORS);
	oi_uartSendChar(OI_SENSOR_PACKET_GROUP100);

	// Read all the sensor data
	uint8_t i;
	for (i = 0; i < SENSOR_PACKET_SIZE; i++) {
		// read each sensor byte
		sensorBuffer[i] = oi_uartReceive();
	}

	//Parse the sensor data into the struct
	oi_parsePacket(self, sensorBuffer);

	timer_waitMillis(25); // reduces USART errors that occur when continuously transmitting/receiving min wait time=15ms
}

void oi_parsePacket(oi_t* self, uint8_t packet[]) {
	self->wheelDropLeft = !!(packet[0] & 0x08);
	self->wheelDropRight = !!(packet[0] & 0x04);
	self->bumpLeft = !!(packet[0] & 0x02);
	self->bumpRight = packet[0] & 0x01;

	self->wallSensor = packet[1];

	self->cliffLeft = packet[2];
	self->cliffFrontLeft = packet[3];
	self->cliffFrontRight = packet[4];
	self->cliffRight = packet[5];

	self->virtualWall = packet[6];

	self->overcurrentLeftWheel = !!(packet[7] & 0x10);
	self->overcurrentRightWheel = !!(packet[7] & 0x08);
	self->overcurrentMainBrush = !!(packet[7] & 0x04);
	self->overcurrentSideBrush = packet[7] & 0x01;

	self->dirtDetect = packet[8];

	//Byte 9 unused

	self->infraredCharOmni = packet[10];

	self->buttonClock = !!(packet[11] & 0x80);
	self->buttonSchedule = !!(packet[11] & 0x40);
	self->buttonDay = !!(packet[11] & 0x20);
	self->buttonHour = !!(packet[11] & 0x10);
	self->buttonMinute = !!(packet[11] & 0x08);
	self->buttonDock = !!(packet[11] & 0x04);
	self->buttonSpot = !!(packet[11] & 0x02);
	self->buttonClean = packet[11] & 0x01;

	self->distance = oi_parseInt(packet + 12);


	self->chargingState = packet[16];
	self->batteryVoltage = oi_parseInt(packet + 17);
	self->batteryCurrent = oi_parseInt(packet + 19);
	self->batteryTemperature = packet[21];
	self->batteryCharge = oi_parseInt(packet + 22);
	self->batteryCapacity = oi_parseInt(packet + 24);

	self->wallSignal = oi_parseInt(packet + 26);

	self->cliffLeftSignal = oi_parseInt(packet + 28);
	self->cliffFrontLeftSignal = oi_parseInt(packet + 30);
	self->cliffFrontRightSignal = oi_parseInt(packet + 32);
	self->cliffRightSignal = oi_parseInt(packet + 34);

	//Bytes 36-38 unused

	self->chargingSourcesAvailable = packet[39];
	self->oiMode = packet[40];

	self->songNumber = packet[41];
	self->songPlaying = packet[42];

	self->numberOfStreamPackets = packet[43];

	self->requestedVelocity = oi_parseInt(packet + 44);
	self->requestedRadius = oi_parseInt(packet + 46);
	self->requestedRightVelocity = oi_parseInt(packet + 48);
	self->requestedLeftVelocity = oi_parseInt(packet + 50);
	self->leftEncoderCount = oi_parseInt(packet + 52);
	self->rightEncoderCount = oi_parseInt(packet + 54);

	self->lightBumperRight = !!(packet[56] & 0x20);
	self->lightBumperFrontRight = !!(packet[56] & 0x10);
	self->lightBumperCenterRight = !!(packet[56] & 0x08);
	self->lightBumperCenterLeft = !!(packet[56] & 0x04);
	self->lightBumperFrontLeft = !!(packet[56] & 0x02);
	self->lightBumperLeft = packet[56] & 0x01;

	self->lightBumpLeftSignal = oi_parseInt(packet + 57);
	self->lightBumpFrontLeftSignal = oi_parseInt(packet + 59);
	self->lightBumpCenterLeftSignal = oi_parseInt(packet + 61);
	self->lightBumpCenterRightSignal = oi_parseInt(packet + 63);
	self->lightBumpFrontRightSignal = oi_parseInt(packet + 65);
	self->lightBumpRightSignal = oi_parseInt(packet + 67);

	self->infraredCharLeft = packet[69];
	self->infraredCharRight = packet[70];

	self->leftMotorCurrent = oi_parseInt(packet + 71);
	self->rightMotorCurrent = oi_parseInt(packet + 73);
	self->mainBrushMotorCurrent = oi_parseInt(packet + 75);
	self->sideBrushMotorCurrent = oi_parseInt(packet + 77);

	self->stasis = packet[79];

	self->angle = getDegrees(self);
}

inline int16_t oi_parseInt(uint8_t* theInt) {
	return (theInt[0] << 8) | theInt[1];
}

/// \brief Set the LEDS on the Create
/// \param play_led 0=off, 1=on
/// \param advance_led 0=off, 1=on
/// \param power_color (0-255), 0=green, 255=red
/// \param power_intensity (0-255) 0=off, 255=full intensity
void oi_setLeds(uint8_t play_led, uint8_t advance_led, uint8_t power_color, uint8_t power_intensity)
{
	// LED Opcode
	oi_uartSendChar(OI_OPCODE_LEDS);

	// Set the Play and Advance LEDs
	oi_uartSendChar(advance_led << 3 && play_led << 2);

	// Set the power led color
	oi_uartSendChar(power_color);

	// Set the power led intensity
	oi_uartSendChar(power_intensity);
}

/// \brief Set direction and speed of the robot's wheels
/// \param linear velocity in mm/s values range from -500 -> 500 of right wheel
/// \param linear velocity in mm/s values range from -500 -> 500 of left wheel
void oi_setWheels(int16_t right_wheel, int16_t left_wheel)
{
	oi_uartSendChar(OI_OPCODE_DRIVE_WHEELS);
	oi_uartSendChar(right_wheel>>8);
	oi_uartSendChar(right_wheel & 0xff);
	oi_uartSendChar(left_wheel>>8);
	oi_uartSendChar(left_wheel& 0xff);
}


/// \brief Load song sequence
/// \param An integer value from 0 - 15 that acts as a label for note sequence
/// \param An integer value from 1 - 16 indicating the number of notes in the sequence
/// \param A pointer to a sequence of notes stored as integer values
/// \param A pointer to a sequence of durations that correspond to the notes
void oi_loadSong(int song_index, int num_notes, unsigned char  *notes, unsigned char  *duration)
{
	int i;
	oi_uartSendChar(OI_OPCODE_SONG);
	oi_uartSendChar(song_index);
	oi_uartSendChar(num_notes);
	for (i=0;i<num_notes;i++) {
		oi_uartSendChar(notes[i]);
		oi_uartSendChar(duration[i]);
	}
}

/// Plays a given song; use oi_load_song(...) first
void oi_play_song(int index){
	oi_uartSendChar(OI_OPCODE_PLAY);
	oi_uartSendChar(index);
}


/// Runs default go charge program; robot will search for dock
void go_charge(void) {
	char charging_state=0;

/*	//Calling demo that will cause Create to seek out home base
	oi_uartSendChar(OI_OPCODE_MAX);
	oi_uartSendChar(0x01);

	//Control is returned immediately, so need to check for docking status
	DDRB &= ~0x80; //Setting pin7 to input
	PORTB |= 0x80; //Setting pullup on pin7

	do {
		charging_state = PINB >> 7;
	} while (charging_state == 0);
	*/
}




///	\brief Initialize UART3 for OI Communication and Debugging
///	internal function
void oi_uartInit(void)
{
	//Calculated Baudrate for 115200;
	uint16_t iBRD = 0x08;//BRD=SYSCLK/((ClkDiv)(BaudRate)), HSE=0 ClkDiv=16, BaudRate=115,200
	uint16_t fBRD = 44;//Fractional remainder is 0.6805, DIVFRAC = (.6805)(64)+0.5 = 44

	SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R2; //enable GPIO Port C

	SYSCTL_RCGCUART_R |= SYSCTL_RCGCUART_R4; //enable UART4

	GPIO_PORTC_AFSEL_R |= (BIT4 | BIT5); //Enable alternate function on PC6,PC7
	GPIO_PORTC_PCTL_R |= 0x00110000; //Enable function 1 (UART Rx/Tx) on PC4,PC5
	GPIO_PORTC_DEN_R |= (BIT4 | BIT5); //Enable PC4,5 data
	GPIO_PORTC_DIR_R |= BIT5; //Set pin direction to output on PC5
	GPIO_PORTC_DIR_R &= ~BIT4; //Set pin direction to input on PC4

	UART4_CTL_R &= ~(UART_CTL_UARTEN); //Disable UART4 while we mess with it

	UART4_IBRD_R = iBRD;
	UART4_FBRD_R = fBRD;

	UART4_LCRH_R = UART_LCRH_WLEN_8; //8 bit, 1 stop, no parity, no FIFO
	UART4_CC_R = UART_CC_CS_SYSCLK; //Use System Clock
	UART4_CTL_R = UART_CTL_RXE | UART_CTL_TXE | UART_CTL_UARTEN; //Enable Rx, Tx and UART module
}

///transmit character
///	internal function
void oi_uartSendChar(char data)
{
	while((UART4_FR_R & UART_FR_TXFF) != 0); //holds until no data in transmit buffer

	UART4_DR_R = data; //puts data in transmission buffer

	//For debugging
	//timer_waitMicros(1000);
}

char oi_uartReceive(void)
{
	static int count = 0;
	//uint32_t tempData; //used for error checking
	char data;

	while((UART4_FR_R & UART_FR_RXFE)); //wait here until data is recieved

	data = (char)(UART4_DR_R & 0xFF);

	count++;

	//ToDo: Implement error checking
	return data;
}


///transmit character array
void oi_uartSendStr(const char *theData)
{
	while(*theData != '\0')
	{
		oi_uartSendChar(*theData);
		theData++;
	}

}

void oi_uartSendBuff(const uint8_t theData[], uint8_t theSize)
{
	int i;

	for(i = 0; i < theSize; i++)
	{
		oi_uartSendChar(theData[i]);
	}
}

char* oi_checkFirmware() {
	const char FIRM_STR[] = "r3_robot/tags/";
	const char FIRM_END = ':';
	const uint8_t FIRM_STRLEN = 14;

	static char firmware[21];

	char buffer[512];
	uint16_t ptr;

	//Reset the iRobot
	oi_uartSendChar(OI_OPCODE_RESET);

	char c;
	while( (c = oi_uartReceive()) || 1) {
		buffer[ptr++] = c;

		//Do a string compare
		buffer[ptr] = '\0';
		if(ptr >= FIRM_STRLEN && !strcmp(buffer + ptr - FIRM_STRLEN, FIRM_STR)) {
			ptr = 0;
			//Firmware version incoming
			while( (c = oi_uartReceive()) != FIRM_END ) {
				firmware[ptr++] = c;
			}

			break;
		}
	}

	firmware[ptr] = '\0';

	return firmware;
}

///initializes the user button to shut off OI
void oi_shutoff_init(void){
	//PF0 user switch - OI shut down
	SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R5; //enable clock to GPIO F
	GPIO_PORTF_LOCK_R = 0x4C4F434B;//unlock PF0
	GPIO_PORTF_CR_R |= 0x01;//allow writes PF0 DEN to be commited
	GPIO_PORTF_DEN_R |= BIT0; //enable digital on pin 0
	GPIO_PORTF_DIR_R |= ~BIT0; //set pin 0 to input
	GPIO_PORTF_IBE_R |= BIT0; //both edges can trigger
	GPIO_PORTF_IEV_R |= BIT0; //enable rising edge trigger
	GPIO_PORTF_ICR_R |= BIT0; //clear interrupt if there is already one
	GPIO_PORTF_IM_R |= BIT0; //unmask interrupt

	NVIC_EN0_R |= 0x40000000; //enable IRQ by setting bit 30
	IntRegister(INT_GPIOF, GPIOF_Handler);//tell cpu to use ISR handler for GPIOF
	IntMasterEnable();//enable global interrupts
}

void GPIOF_Handler(void) {
	if(GPIO_PORTF_RIS_R & BIT0) {
		//shutoff button was pressed, turn off OI
		oi_close();

		GPIO_PORTF_ICR_R |= BIT0;//clear interrupt
	}
}


/**
 * Get the moved degrees from the previous call of oi_update
 * @param self : the sensor data
 */
int getDegrees(oi_t *self){
	static int iterations = 0;
	static int prevLeft = 0;
	static int prevRight = 0;


	//if(self->leftEncoderCount == 0 || self->rightEncoderCount == 0){  // update was called with no movement of the bot
	//	return 0;
	//}

	if((self->leftEncoderCount == prevLeft) && (self->rightEncoderCount == prevRight)){ // if the bot has not moved since previous update
		prevLeft = self->leftEncoderCount;
		prevRight = self->rightEncoderCount;
		return 0;
	}
	//ignore the first run, such that we do not have prevLeft or right=0, this would give a very large degree moved.
	else if(iterations == 0){
		prevLeft = self->leftEncoderCount;
		prevRight = self->rightEncoderCount;
		iterations++;
		return 0;
	}

	//get distance moved in mm for the left and right wheel
	// equation: ticks * (1/508)*72pi
	//update the previous values to be correct
	int distLeft = (self->leftEncoderCount - prevLeft)*(0.445265);
	int distRight = (self->rightEncoderCount - prevRight)*(0.445265);
	prevLeft = self->leftEncoderCount;
	prevRight = self->rightEncoderCount;


	//calculate the degree travelled by (right-left)/wheel base in mm
	double deg = (distRight - distLeft)/178.5;//change 178.5 to improve accuracy of turning if needed
	deg = (deg*180)/M_PI;

	return (deg);

}


