/*
 * Copter.cpp
 *
 *  Created on: Oct 16, 2015
 *      Author: jan
 */

#include "Copter.h"
#include "../flightstate.h"

using namespace std;

Copter::Copter(vect rpy, vect p) {
	timestamp_us = Clock::Instance()->getTime_us();
	position = new Transform(rpy, p);
	acceleration = {0,0,0};
	angularAcceleration = {0,0,0};
	velocity = {0,0,0};
	angularVelocity = {0,0,0};
	Inertia = {0.02,0.02,0.04};
	dragCoefficient = 0.1;
	weight = 0.8;
	unlink("/tmp/fifoCopter");
	mkfifo("/tmp/fifoCopter", 0666);
	if ((fd_fifo = open("/tmp/fifoCopter", O_RDWR)) == -1) {
		perror("couldn't open fifo");
	}
}

bool Copter::addMotor(P_Motor *mot) {
	if (mot == NULL) {
		return false;
	} else {
		this->motorList.push_back(mot);
		return true;
	}
}
void Copter::setMotors(int16_t *velList) {
	std::vector<P_Motor*>::iterator it;
	int i = 0;
//	cout << "motors: ";
	double currentSum = 0;
	for (it = motorList.begin(); it < motorList.end(); it++) {
		(*it)->set_PWM(constrain(velList[i], 0, 255));
//		cout << (int) velList[i] << " ";
		motor.error[i] = 0;
		currentSum += (*it)->current;
		double uint8Current = (*it)->current * 10;
		// current scaling due to pulse-width modulation
		uint8Current *= (*it)->pwm;
		uint8Current /= 255;
		// arbitrary efficiency of switching mosfets
		uint8Current /= 0.90;
		if (uint8Current > 255)
			uint8Current = 255;
		motor.rawCurrent[i] = (uint8_t) uint8Current;
		i++;
	}

	// TODO this only works if the same battery is used for all motors
	it = motorList.begin();
	(*it)->bat->useCurrent(currentSum);

//	cout << endl;
}

void Copter::calcAcceleration(void) {
	vect torque = { 0, 0, 0 };
	vect force = { 0, 0, 0 };
	std::vector<P_Motor*>::iterator it;
	for (it = motorList.begin(); it < motorList.end(); it++) {
		torque.x += (*it)->torqueRoll();
		torque.y += (*it)->torquePitch();
		torque.z += (*it)->torqueYaw();
		vect thrustVect = (*it)->get_ThrustVector();
		force += thrustVect;
	}
	// transform force vector from copter frame to world frame
	force = position->transform_Vect(force);
	// subtract drag TODO implement this in the copter frame (3 different drag coefficients would be possible)
	force -= velocity * dragCoefficient;
	acceleration = force / weight;
	// add gravity
	acceleration.z += 9.81;

	angularAcceleration.x = torque.x / Inertia.x;
	angularAcceleration.y = torque.y / Inertia.y;
	angularAcceleration.z = torque.z / Inertia.z;
	// add effects from perpendicular axis (see Eulers equations of rigid body dynamics)
	angularAcceleration.x += (Inertia.y - Inertia.z) / Inertia.x
			* angularVelocity.y * angularVelocity.z;
	angularAcceleration.y += (Inertia.z - Inertia.x) / Inertia.y
			* angularVelocity.x * angularVelocity.z;
	angularAcceleration.z += (Inertia.x - Inertia.y) / Inertia.z
			* angularVelocity.x * angularVelocity.y;
}
void Copter::advanceState(void) {
	std::vector<P_Motor*>::iterator it;
	for (it = motorList.begin(); it < motorList.end(); it++) {
		(*it)->update();
	}
	long long time = Clock::Instance()->getTime_us();
	double time_diff = (double) (time - timestamp_us) / 1000000L;
	timestamp_us = time;
	velocity += acceleration * time_diff;
	if (position->pz >= 0 && velocity.z > 0) {
		// copter on ground and still moving downwards
		// -> stop velocity
		velocity = {0,0,0};
		acceleration = {0,0,0};
//		angularAcceleration = {0,0,0};
//		angularVelocity = {0,0,0};
	}

	angularVelocity += angularAcceleration * time_diff;

	position->rotateRPY(angularVelocity * time_diff);
	position->move(velocity * time_diff);

}
vect Copter::getCopterFrameAngularVelocity(void) {
	return angularVelocity;
}
vect Copter::getCopterFrameAcceleration(void) {
	return position->transformInv_Vect(acceleration);
}
vect Copter::getCopterFrameGravitation(void) {
	return position->transformInv_Vect( { 0, 0, 9.81 });
}
vect Copter::getCopterFrameMagneticField(void) {
	vect magneticField = { cos(MAGNETIC_FIELD_DECLINATION * DEG_TO_RAD), 0, sin(
	MAGNETIC_FIELD_DECLINATION * DEG_TO_RAD) };
	magneticField *= MAGNETIC_FIELD_STRENGTH;
	return position->transformInv_Vect(magneticField);
}
double Copter::getCopterMeanPWM(void) {
	std::vector<P_Motor*>::iterator it;
	int i = 0;
	double mean = 0;
	for (it = motorList.begin(); it < motorList.end(); it++) {
		mean += (*it)->pwm;
		i++;
	}
	if (i != 0)
		mean /= i;
	else
		// no motors attached to copter
		mean = 0;
	return mean;
}
void Copter::printState(void) {
	char buffer[200];
	vect rpy = position->toRPY();
	sprintf(buffer, "CState %f %f %f %f %f %f %d\n", position->px, position->py,
			position->pz, rpy.x, rpy.y, rpy.z, !flightState.motorOff);

//	std::vector<P_Motor*>::iterator it;
//	cout << "Motorvalues:" << endl;
//	for (it = motorList.begin(); it < motorList.end(); it++) {
//		(*it)->printStats();
//	}

//	printf(buffer);
//	cout << "CState " << position->px << endl;
	write(fd_fifo, buffer, strlen(buffer));
}

