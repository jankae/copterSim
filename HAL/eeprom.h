/*
 * eeprom.h
 *
 *  Created on: Oct 17, 2015
 *      Author: jan
 */

#ifndef EEPROM_H_
#define EEPROM_H_

#include <stdint.h>
#include <stdio.h>
#include "../common/defines.h"
#include <fstream>
#include <iostream>

#define VERSION			1
#define SUBVERSION		2

/*
 * EEPROM usage map:
 * 0x0000-0x0292	config struct
 * 0x0293-0x3FFF	reserved for config struct expansion
 * 0x4000-0x4FFF	reserved for barometer temperatur compensation data
 */

#define CONTROLLER_ANGULARVELOCITY_X	0
#define CONTROLLER_ANGULARVELOCITY_Y	1
#define CONTROLLER_ANGULARVELOCITY_Z	2
#define CONTROLLER_ANGLE_X				3
#define CONTROLLER_ANGLE_Y				4
#define CONTROLLER_ANGLE_Z				5
#define CONTROLLER_ASCEND				6
#define CONTROLLER_HEIGHT				7
#define NUMBER_OF_CONTROLLERS			10

struct controllerConfig {
	float P, I, D, lowerLimit, upperLimit;
	uint16_t interval;
};

extern struct Config{
	int16_t gyroXOffset, gyroYOffset, gyroZOffset;
	int16_t accXOffset, accYOffset, accZOffset;
	int16_t magXOffset, magYOffset, magZOffset;
	float magXScale, magYScale, magZScale;
	struct {
		float mixer[12][4];
		// quantity of available motors
		uint8_t num;
		// defines used motor outputs (e.g. CAN, I2C, PPM)
		uint8_t output;
	} motor;
	struct controllerConfig controller[NUMBER_OF_CONTROLLERS];
	// maximal RC limits (in rad and rad/s and m/s)
	float maxAngle, maxRotation;
	// time difference between two flightLog lines in ms
	uint16_t flightLogInterval;
	// set bits to activate the specific values in the flightLog (see log.c for order of logfile values)
	// flightLogMask: float values
	uint32_t flightLogMask;
	// flightLogMask2: int32_t values
	uint32_t flightLogMask2;
	struct {
		float CovMeasAcc, CovAccError;
		float CovMeasMag, CovMagError;
		float CovMeasGyro;
		float CovStateAcc;
		float CovStateMag;
		float CovStateGyro;
		float CovStateGyroBias;
	} attitudeKalman;
	// lowpass-filter for gyroscope, accelerometer and magnetometer
	float alphaAcc, alphaGyro, alphaMag;
	/******************************
	 * bit | meaning
	 *	0  | enable kalmanLog
	 *****************************/
#define CONFIG_MASK_KALMAN_LOG			0x01
	uint32_t configMask1;
	float maxAscend;
	struct {
		float CovMeasAcc;
		float CovMeasGPSVertical;
		float CovMeasGPSHorizontal;
		float CovMeasGPSVelocity;
		float CovMeasBarometer;

		float CovStateAcc;
		float CovStateGPSVertical;
		float CovStateGPSHorizontal;
		float CovStateGPSVelocity;
		float CovStateVerticalVelocity;
		float CovStateBaroOffset;
	} positionKalman;
	float howerPower;
	uint8_t servoSource[12];
	// rotation matrices for sensors to allow different sensor orientations.
	// Usage: axis X influences axis Y with the weight [Y][X]
	// Example: normal sensor orientation: identity matrix
	// Example2: 180° rotation on Z-Axis:
	// [-1  0  0]
	// [ 0 -1  0]
	// [ 0  0  1]
	float MagMatrix[3][3];
} config;

extern struct Eeprom {
	FlagStatus configLoaded;
} eeprom;

//extern Eeprom eeprom;

/*
 * writes a single byte to the eeprom
 */
void eeprom_WriteByte(uint16_t adr, uint8_t data);
/*
 * reads a single byte from the eeprom
 */
void eeprom_ReadByte(uint16_t adr, uint8_t *data);
/*
 * writes a page (see EEPROM_PAGE_SIZE) to the eeprom
 * @param page 	number of the eeprom page to be writte
 * @param data 	pointer to the data to be written
 * @param size 	number of bytes to be written, might be
 * 				any value between 1 and EEPROM_PAGE_SIZE
 */
void eeprom_WritePage(uint16_t page, uint8_t *data, uint8_t size);
/*
 * reads a block of data from the eeprom
 */
void eeprom_ReadBlock(uint16_t adr, uint8_t *data, uint16_t size);
/*
 * writes a block of data to the eeprom
 */
void eeprom_WriteBlock(uint16_t adr, uint8_t *data, uint16_t size);
/*
 * loads config values from the external i2c eeprom
 */
void eeprom_LoadConfig(void);
/*
 * saves the config values to the eeprom
 */
void eeprom_SaveConfig(void);

#endif /* EEPROM_H_ */
