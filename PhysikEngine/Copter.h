#ifndef COPTER2_H_
#define COPTER2_H_

#include <stdlib.h>
#include <vector>
#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>


#include "Transform.h"
#include "Motor.h"
#include "Clock.h"

class Copter {
public:
	// state values
	Transform *position;
	vect velocity;
	vect acceleration;

	vect angularVelocity;
	vect angularAcceleration;

	// configuration values
	std::vector<P_Motor*> motorList;
	double weight;
	vect Inertia;
	double dragCoefficient;

	long long int timestamp_us;

	// fifo values
	int fd_fifo;

public:
	Copter(vect rpy, vect p);
	bool addMotor(P_Motor *mot);
	void setMotors(int16_t *velList);
	void calcAcceleration(void);
	void advanceState(void);
	vect getCopterFrameAngularVelocity(void);
	vect getCopterFrameAcceleration(void);
	vect getCopterFrameGravitation(void);
	vect getCopterFrameMagneticField(void);
	double getCopterMeanPWM(void);
	void printState(void);
};

#endif
