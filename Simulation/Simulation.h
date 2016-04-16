/*
 * Simulation.h
 *
 *  Created on: Oct 17, 2015
 *      Author: jan
 */

#ifndef SIMULATION_H_
#define SIMULATION_H_

#include <stddef.h>
#include <iostream>
#include "../PhysikEngine/Copter.h"
#include "../PhysikEngine/Motor.h"
#include "../PhysikEngine/Prop.h"
#include "../PhysikEngine/PBattery.h"
#include "../Sensors/Accelerometer.h"
#include "../Sensors/Gyro.h"
#include "../Sensors/Magnetometer.h"
#include "../Sensors/SensorReceiver.h"
#include "Barometer.h"

#define SIMULATION_SENSOR_FREQUENCY 	500
#define SIMULATION_SENSOR_PERIOD		(1000000/SIMULATION_SENSOR_FREQUENCY)

#define SIMULATION_BARO_FREQUENCY		60
#define SIMULATION_BARO_PERIOD			(1000000/SIMULATION_BARO_FREQUENCY)

#define SIMULATION_TASKS_FREQUENCY 		10000
#define SIMULATION_TASKS_PERIOD			(1000000/SIMULATION_TASKS_FREQUENCY)

class Simulation {
private:
	Copter *copter;
	SensorGyro *gyr;
	SensorAccelerometer *acc;
	SensorMagnetometer *mag;
	SensorReceiver *rec;
	SensorBarometer *baro;
	P_Motor *mot1, *mot2, *mot3, *mot4;
	Prop *prop1, *prop2, *prop3, *prop4;
	P_Battery *bat;
	long long int starttime, lastSensorUpdate, lastBaroUpdate, lastTask;
	long long int printTime;
	long long int handlerTime;
	Simulation();
	static Simulation* m_pInstance;
public:
	static Simulation* Instance();
	void Handler(void);
	void setMotors(void);
};

#endif /* SIMULATION_H_ */
