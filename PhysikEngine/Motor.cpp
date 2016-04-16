#include "Motor.h"

using namespace std;

//P_Motor::P_Motor(vect rpy, vect p, bool CW, float thrust) {
//	position = new Transform(rpy, p);
//	this->thrust = thrust;
//	this->CW = CW;
//}

P_Motor::P_Motor(vect rpy, vect p, bool CW) {
	position = new Transform(rpy, p);
//	this->thrust = 0;
	this->CW = CW;
	// TODO dummy values
	voltage = 0;
	KV = 760;
	resistance = 0.290;
	inertia = 0.0000038;
	pwm = 0;
}
void P_Motor::attachProp(Prop *p) {
	prop = p;
}
void P_Motor::attachToBattery(P_Battery *b) {
	bat = b;
}
double P_Motor::get_torque(void) {
// calculate motor current
// I = (voltage-backEMF_voltage)/resistance
	double backEMF = rpm / KV;
	current = (voltage - backEMF) / resistance;
	// no engine braking with current ESCs
	if (current < 0)
		current = 0;
	// torque = backEMF*current/velocity[rad/s]
	double torque;
	// special case: velocity very low:
	if (rpm < 60) {
		torque = 60 * voltage / (KV * resistance * 2 * M_PI);
	} else {
		double velocity = rpm / 60 * 2 * M_PI;
		torque = voltage * current / velocity;
	}
	// account for efficiency
	torque *= 0.76;
	//return current * KT;
	return torque;
}
void P_Motor::update(void) {
	if (bat) {
		voltage = bat->get_Voltage() * pwm / 255;
	} else {
		voltage = 0;
	}
//	cout << "Voltage: " << voltage;
	// calculate current torques
	double m_torque = this->get_torque();
	double p_torque;
	double inert = inertia;
	if (prop) {
		p_torque = prop->get_torque(rpm);
		inert += prop->get_inertia();
	} else {
		p_torque = 0;
	}
//	cout << " m_torque: " << m_torque << " p_torque: " << p_torque
//			<< " inertia: " << inertia;

	double acceleration = (m_torque - p_torque) / inert;

	long long int time = Clock::Instance()->getTime_us();
	double timediff = (double) (time - lastUpdateTime) / 1000000L;
	lastUpdateTime = time;

// update rpm, converting acceleration from rad/s^2 to 1/m^2
	rpm += acceleration * 30 * M_1_PI * timediff;
	if (rpm < 0)
		rpm = 0;
//	cout << " acceleration: " << acceleration << " rpm: " << rpm << endl;
}
vect P_Motor::get_ThrustVector(void) {
	vect thrustVect = { 0, 0, -get_Thrust() };
	thrustVect = position->transform_Vect(thrustVect);
	return thrustVect;
}

double P_Motor::get_Thrust(void) {
	if (prop)
		return prop->get_thrust(rpm);
	else
		return 0;
}

void P_Motor::set_PWM(uint8_t pwm) {
	this->pwm = pwm;
}

double P_Motor::torqueRoll(void) {
// motor thrust is always along motor Z axis
	vect thrustVect = { 0, 0, -1 };
// calculate thrust vector in copter Z/Y plane
	thrustVect = position->transform_Vect(thrustVect);
//	cout << "thrustVect:" << thrustVect.x << " " << thrustVect.y << " "
//			<< thrustVect.z << endl;
// calculate angle between the vector from the CG to the motor
// position and the motor thrust vector in the copter Z/Y plane
	double angle = atan2(thrustVect.y, thrustVect.z)
			- atan2(position->py, position->pz);
//	cout << "angle:" << angle * 180 / M_PI << endl;
// calculate motor distance from CG in copter Z/Y plane
	double dist = sqrt(
			position->pz * position->pz + position->py * position->py);
// calculate magnitude of thrust vector in copter Z/Y plane
	double magn = sqrt(
			thrustVect.y * thrustVect.y + thrustVect.z * thrustVect.z);
// calculate thrust applied to the roll axis by this motor
	return get_Thrust() * (magn * dist * -sin(angle));
}
double P_Motor::torquePitch(void) {
// motor thrust is always along motor Z axis
	vect thrustVect = { 0, 0, -1 };
// calculate thrust vector in copter Z/X plane
	thrustVect = position->transform_Vect(thrustVect);
//	cout << "thrustVect:" << thrustVect.x << " " << thrustVect.y << " "
//			<< thrustVect.z << endl;
// calculate angle between the vector from the CG to the motor
// position and the motor thrust vector in the copter Z/X plane
	double angle = atan2(thrustVect.x, thrustVect.z)
			- atan2(position->px, position->pz);
//	cout << "angle:" << angle * 180 / M_PI << endl;
// calculate motor distance from CG in copter Z/X plane
	double dist = sqrt(
			position->pz * position->pz + position->px * position->px);
// calculate magnitude of thrust vector in copter Z/X plane
	double magn = sqrt(
			thrustVect.x * thrustVect.x + thrustVect.z * thrustVect.z);
// calculate thrust applied to the roll axis by this motor
	return get_Thrust() * (magn * dist * sin(angle));
}
double P_Motor::torqueYaw(void) {
// motor thrust is always along motor Z axis
	vect thrustVect = { 0, 0, -1 };
// calculate thrust vector in copter X/Y plane
	thrustVect = position->transform_Vect(thrustVect);
//	cout << "thrustVect:" << thrustVect.x << " " << thrustVect.y << " "
//			<< thrustVect.z << endl;
// calculate angle between the vector from the CG to the motor
// position and the motor thrust vector in the copter X/Y plane
	double angle = atan2(thrustVect.y, thrustVect.x)
			- atan2(position->py, position->px);
//	cout << "angle:" << angle * 180 / M_PI << endl;
// calculate motor distance from CG in copter X/Y plane
	double dist = sqrt(
			position->px * position->px + position->py * position->py);
// calculate magnitude of thrust vector in copter X/Y plane
	double magn = sqrt(
			thrustVect.y * thrustVect.y + thrustVect.x * thrustVect.x);
// calculate angulartorque
	double angtorque = 0;
	if (prop)
		angtorque = prop->get_torque(rpm);
	if (CW) {
		angtorque = -angtorque;
	}
// calculate thrust applied to the yaw axis by this motor
	return get_Thrust() * (magn * dist * sin(angle)) + angtorque;
}

void P_Motor::printStats(void) {
	cout << "motStats:" << voltage << "V " << current << "A "
			<< current * voltage << "W " << rpm << "rpm" << endl;
	if (prop) {
		double thrust = prop->get_thrust(rpm);
		double torque = prop->get_torque(rpm);
		double power = rpm / 60 * 2 * M_PI * torque;
		cout << "propStats:" << thrust << "N " << torque << "Nm " << power
				<< "W" << endl;
	}
}
