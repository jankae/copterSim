/*
 * Motor.h
 *
 *  Created on: Oct 16, 2015
 *      Author: jan
 */

#ifndef PHYSIK_MOTOR_H_
#define PHYSIK_MOTOR_H_

#include "Transform.h"
#include "Prop.h"
#include "Clock.h"
#include "PBattery.h"
#include <iostream>

#define PROPELLER_DRAG_CONSTANT 0.1

class P_Motor {
public:
	// configuration values
	Transform *position;
	bool CW;
	double KV; // 750
	double resistance; // 0.5
	double inertia; // all moving parts (prop+motor)
	Prop *prop;
	P_Battery *bat;
	long long int lastUpdateTime;
	// state values
	//double thrust;
	//double PWM;
	double voltage;
	double current;
	double rpm;
	uint8_t pwm;
public:
	//P_Motor(vect rpy, vect p, bool CW, float thrust);
	P_Motor(vect rpy, vect p, bool CW);
	void attachProp(Prop *p);
	void attachToBattery(P_Battery *b);
	double get_torque(void);
	void update(void);
	vect get_ThrustVector(void);
	double get_Thrust(void);
	void set_PWM(uint8_t pwm);
	double torqueRoll(void);
	double torquePitch(void);
	double torqueYaw(void);

};

#endif /* MOTOR_H_ */
