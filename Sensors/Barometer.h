#ifndef SENSORS_BAROMETER_H_
#define SENSORS_BAROMETER_H_

#include "../PhysikEngine/Copter.h"
#include "../common/vect.h"
#include "../common/defines.h"
#include "../common/functions.h"
#include <random>

class SensorBarometer {
private:
	// noise in LSB with stopped motors
	double zeroNoise;
	// factor between noise and motorpwm^2
	double NoiseFactor;
	Copter *copter;

public:
	SensorBarometer(double zNoise, double Noisefact);
	double getValue(void);
	void copyRawValue(void);
	void attachToCopter(Copter *c);
};

#endif /* SENSORS_BAROMETER_H_ */
