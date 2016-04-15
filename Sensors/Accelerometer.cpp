/*
 * Accelerometer.cpp
 *
 *  Created on: Oct 17, 2015
 *      Author: jan
 */

#include "Accelerometer.h"
#include "../accelerometer.h"

SensorAccelerometer::SensorAccelerometer(/*vect off, vect stdDev, */vect zNoise,
		vect Noisefact) {
//	offset = off;
//	stdDeviation = stdDev;
	copter = NULL;
	zeroNoise = zNoise;
	NoiseFactor = Noisefact;
}

vect SensorAccelerometer::getValue(void) {
	if (copter == NULL)
		return {0,0,0};
	vect acceleration = copter->getCopterFrameAcceleration();
	vect gravitation = copter->getCopterFrameGravitation();
	vect trueValue = (acceleration - gravitation) / 9.81;

//	std::normal_distribution<double> distributionX(offset.x, stdDeviation.x);
//	trueValue.x += distributionX(generator);
//	std::normal_distribution<double> distributionY(offset.y, stdDeviation.y);
//	trueValue.y += distributionY(generator);
//	std::normal_distribution<double> distributionZ(offset.z, stdDeviation.z);
//	trueValue.z += distributionZ(generator);

	return trueValue;
}

void SensorAccelerometer::copyRawValue(void) {

    std::random_device rd;
    std::mt19937 gen(rd());
	// calculate integer values
	vect acc = getValue();
	int32_t accX = -acc.x * 4096;
	int32_t accY = acc.y * 4096;
	int32_t accZ = -acc.z * 4096;
	// add noise
	double meanPWM = this->copter->getCopterMeanPWM();
	meanPWM = pow(meanPWM, 4);
	std::normal_distribution<double> distributionX(0,
			zeroNoise.x + meanPWM * NoiseFactor.x);
	accX += distributionX(gen);
	std::normal_distribution<double> distributionY(0,
			zeroNoise.y + meanPWM * NoiseFactor.y);
	accY += distributionY(gen);
	std::normal_distribution<double> distributionZ(0,
			zeroNoise.z + meanPWM * NoiseFactor.z);
	accZ += distributionZ(gen);
	int16_t naccX = constrain(accX, -32768, 32767);
	int16_t naccY = constrain(accY, -32768, 32767);
	int16_t naccZ = constrain(accZ, -32768, 32767);
#ifdef DEBUG_NOISE
	std::cout << "raw int accelerometer values: " << naccX << " " << naccY << " "
			<< naccZ << std::endl;
#endif
	// calculate sensor noise
	accelerometer.rawValues[0] = (uint8_t) (naccX >> 8);
	accelerometer.rawValues[1] = (uint8_t) (naccX & 0xff);
	accelerometer.rawValues[2] = (uint8_t) (naccY >> 8);
	accelerometer.rawValues[3] = (uint8_t) (naccY & 0xff);
	accelerometer.rawValues[4] = (uint8_t) (naccZ >> 8);
	accelerometer.rawValues[5] = (uint8_t) (naccZ & 0xff);
}

void SensorAccelerometer::attachToCopter(Copter *c) {
	copter = c;
}
