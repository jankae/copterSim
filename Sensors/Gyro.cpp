/*
 * Gyro.cpp
 *
 *  Created on: Oct 17, 2015
 *      Author: jan
 */

#include "Gyro.h"
#include "../gyro.h"

SensorGyro::SensorGyro(/*vect off, vect stdDev,*/vect zNoise, vect Noisefact) {
//	offset = off;
//	stdDeviation = stdDev;
	copter = NULL;
	zeroNoise = zNoise;
	NoiseFactor = Noisefact;
}

vect SensorGyro::getValue(void) {
	if (copter == NULL)
		return {0,0,0};
	vect angularVel = copter->getCopterFrameAngularVelocity();

//	std::normal_distribution<double> distributionX(offset.x, stdDeviation.x);
//	angularVel.x += distributionX(generator);
//	std::normal_distribution<double> distributionY(offset.y, stdDeviation.y);
//	angularVel.y += distributionY(generator);
//	std::normal_distribution<double> distributionZ(offset.z, stdDeviation.z);
//	angularVel.z += distributionZ(generator);

	return angularVel;
}
void SensorGyro::copyRawValue(void) {
    std::random_device rd;
    std::mt19937 gen(rd());
	// calculate integer values
	vect gyr = getValue();
	int32_t gyroX = -gyr.x * RAD_TO_DEG * 16.4;
	int32_t gyroY = gyr.y * RAD_TO_DEG * 16.4;
	int32_t gyroZ = -gyr.z * RAD_TO_DEG * 16.4;
	// add noise
	double meanPWM = this->copter->getCopterMeanPWM();
	meanPWM = pow(meanPWM, 2);
	std::normal_distribution<double> distributionX(0,
			zeroNoise.x + meanPWM * NoiseFactor.x);
	gyroX += distributionX(gen);
	std::normal_distribution<double> distributionY(0,
			zeroNoise.y + meanPWM * NoiseFactor.y);
	gyroY += distributionY(gen);
	std::normal_distribution<double> distributionZ(0,
			zeroNoise.z + meanPWM * NoiseFactor.z);
	gyroZ += distributionZ(gen);
	int16_t ngyroX = constrain(gyroX, -32768, 32767);
	int16_t ngyroY = constrain(gyroY, -32768, 32767);
	int16_t ngyroZ = constrain(gyroZ, -32768, 32767);
	gyro.rawValues[0] = (uint8_t) (ngyroX >> 8);
	gyro.rawValues[1] = (uint8_t) (ngyroX & 0xff);
	gyro.rawValues[2] = (uint8_t) (ngyroY >> 8);
	gyro.rawValues[3] = (uint8_t) (ngyroY & 0xff);
	gyro.rawValues[4] = (uint8_t) (ngyroZ >> 8);
	gyro.rawValues[5] = (uint8_t) (ngyroZ & 0xff);
}

void SensorGyro::attachToCopter(Copter *c) {
	copter = c;
}
