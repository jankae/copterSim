/*
 * Magnetometer.cpp
 *
 *  Created on: Oct 17, 2015
 *      Author: jan
 */

#include "Magnetometer.h"
#include "../magnetometer.h"

SensorMagnetometer::SensorMagnetometer(/*vect off, vect stdDev,*/vect zNoise, vect Noisefact) {
//	offset = off;
//	stdDeviation = stdDev;
	copter = NULL;
	zeroNoise = zNoise;
	NoiseFactor = Noisefact;
}

vect SensorMagnetometer::getValue(void) {
	if (copter == NULL)
		return {0,0,0};
	vect magField = copter->getCopterFrameMagneticField();

//	std::normal_distribution<double> distributionX(offset.x, stdDeviation.x);
//	magField.x += distributionX(generator);
//	std::normal_distribution<double> distributionY(offset.y, stdDeviation.y);
//	magField.y += distributionY(generator);
//	std::normal_distribution<double> distributionZ(offset.z, stdDeviation.z);
//	magField.z += distributionZ(generator);

	return magField;
}
void SensorMagnetometer::copyRawValue(void) {
    std::random_device rd;
    std::mt19937 gen(rd());
	// calculate integer values
	vect mag = getValue();
	int32_t magX = mag.x * 1370;
	int32_t magY = mag.y * 1370;
	int32_t magZ = -mag.z * 1370;
	// add noise
	double meanPWM = this->copter->getCopterMeanPWM();
	meanPWM = pow(meanPWM, 2);
	std::normal_distribution<double> distributionX(0,
			zeroNoise.x + meanPWM * NoiseFactor.x);
	magX += distributionX(gen);
	std::normal_distribution<double> distributionY(0,
			zeroNoise.y + meanPWM * NoiseFactor.y);
	magY += distributionY(gen);
	std::normal_distribution<double> distributionZ(0,
			zeroNoise.z + meanPWM * NoiseFactor.z);
	magZ += distributionZ(gen);
	int16_t nmagX = constrain(magX, -32768, 32767);
	int16_t nmagY = constrain(magY, -32768, 32767);
	int16_t nmagZ = constrain(magZ, -32768, 32767);
	magnetometer.rawValues[4] = (uint8_t) (nmagX >> 8);
	magnetometer.rawValues[5] = (uint8_t) (nmagX & 0xff);
	magnetometer.rawValues[0] = (uint8_t) (nmagY >> 8);
	magnetometer.rawValues[1] = (uint8_t) (nmagY & 0xff);
	magnetometer.rawValues[2] = (uint8_t) (nmagZ >> 8);
	magnetometer.rawValues[3] = (uint8_t) (nmagZ & 0xff);
}
void SensorMagnetometer::attachToCopter(Copter *c) {
	copter = c;
}
