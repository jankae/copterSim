#include <Barometer.h>
#include "pressure.h"

SensorBarometer::SensorBarometer(double zNoise, double Noisefact) {
	zeroNoise = zNoise;
	NoiseFactor = Noisefact;
	copter = NULL;
}

double SensorBarometer::getValue(void) {
	if (copter == NULL) {
		return 0;
	} else {
		double height = -copter->position->pz;
		double hPa = 1013.25 - height * 0.12;
		return hPa;
	}
}

void SensorBarometer::copyRawValue(void) {
    std::random_device rd;
    std::mt19937 gen(rd());
	// no temperature sensor data available
	adc.raw[5] = 0;
	// sensor has a sensitivity of 4.6mV/hPa
	double sensorOutput = this->getValue() * 0.0046;
	double ADC_p = sensorOutput;
	const double ADC_n = 2.5;
	const double ADC_ref = 2.5;
	// ADC value at Vref is 2^21 (=0x200000)
	uint32_t adcValue = ((ADC_p - ADC_n) / 2.5) * 0x200000;
	// add noise
	double meanPWM = this->copter->getCopterMeanPWM();
	meanPWM = pow(meanPWM, 2);
	std::normal_distribution<double> distributionX(0,
			zeroNoise + meanPWM * NoiseFactor);
	adcValue += distributionX(gen);
	pressure.rawADC = adcValue;
}

void SensorBarometer::attachToCopter(Copter* c) {
	copter = c;
}
