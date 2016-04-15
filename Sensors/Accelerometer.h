#ifndef SENSOR_ACCELEROMETER_H_
#define SENSOR_ACCELEROMETER_H_

#include "../PhysikEngine/Copter.h"
#include "../common/vect.h"
#include "../common/defines.h"
#include "../common/functions.h"
#include <random>

//#define DEBUG_NOISE

class SensorAccelerometer {
private:
	// configuration values
	vect offset;
	vect stdDeviation;
	// noise in LSB with stopped motors
	vect zeroNoise;
	// factor between noise and motorpwm^4
	vect NoiseFactor;
	Copter *copter;

public:
	SensorAccelerometer(/*vect off, vect stdDev, */vect zNoise, vect Noisefact);
	vect getValue(void);
	void copyRawValue(void);
	void attachToCopter(Copter *c);
};

#endif
