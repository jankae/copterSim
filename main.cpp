#include "PhysikEngine/Copter.h"
#include "PhysikEngine/Transform.h"
#include "PhysikEngine/Motor.h"
#include "Sensors/Accelerometer.h"
#include "Sensors/Magnetometer.h"
#include "stdlib.h"
#include "stdio.h"
#include <iostream>
#include "copter.h"

using namespace std;

int main(void) {
	Copter *c = new Copter( { 0, 0, 0 }, { 0, 0, 0 });
	SensorAccelerometer *acc = new SensorAccelerometer( { 1000, 1000, 1000 },
			{ 0.01, 0.01, 0.025 });
	acc->attachToCopter(c);
	for (int i = 0; i < 100; i++) {
		vect a = acc->getValue();
		acc->copyRawValue();
		cout << a.x << " " << a.y << " " << a.z << endl;
	}
//	Prop *p = new Prop("propConfig.csv", 0.254, 0.00004);
//	for (int i = 1000; i < 7000; i += 100) {
//		cout << "rpm: " << i << " thrust: " << p->get_thrust(i) << " torque: "
//				<< p->get_torque(i) << endl;
//	}
//	P_Motor *m = new P_Motor( { 0, 0, 0 }, { 0, 0, 0 }, true);
//	P_Battery *b = new P_Battery(14.8, 0.01);
//	m->attachProp(p);
//	m->attachToBattery(b);
//	cout << "time[ms] rpm[1/s] voltage[V] current[A] torque_m[Nm] torque_p[Nm] thrust[N]"
//			<< endl;
//	m->set_PWM(50);
//	for (int i = 0; i < 2000; i++) {
//		m->update();
//		b->useCurrent(m->current);
//		cout << Clock::Instance()->getTime_ms() << " " << m->rpm << " "
//				<< b->get_Voltage() << " " << m->current << " " << m->get_torque() << " "
//				<< p->get_torque(m->rpm) << " " << p->get_thrust(m->rpm)
//				<< "\n";
//		if (i == 500)
//			m->set_PWM(255);
//		if (i == 1000)
//			m->set_PWM(80);
//		Clock::Instance()->waitus(1000);
//	}
//	cout << flush;
//	vect rpy = { 0, 0, 0 };
//	vect p = { 0, 0, 0 };
//	Copter *c = new Copter(rpy, p);
//	p = {0,0.2,0};
//	P_Motor *m = new P_Motor(rpy, p, true, 1);
//	p = {0,-0.2,0};
//	P_Motor *m2 = new P_Motor(rpy, p, true, 1);
//	cout << "torqueRoll:" << m->torqueRoll() << endl;
//	cout << "torquePitch:" << m->torquePitch() << endl;
//	cout << "torqueYaw:" << m->torqueYaw() << endl;
//	p = {-0.2,0,0};
//	P_Motor *m2 = new P_Motor(rpy, p, 0);
//	c->addMotor(m);
//	c->addMotor(m2);
//	cout << "torquePitch:" << m->torquePitch() << endl;
//	cout << "torquePitch2:" << m2->torquePitch() << endl;
//	c->calcAcceleration();
//	c->advanceState();
//	cout << "copterPitchAcceleration:" << c->angularAcceleration.y << endl;
//	cout << "copterZAcceleration:" << c->acceleration.z << endl;
//	SensorAccelerometer *a = new SensorAccelerometer( { 0, 0, 0 }, { 0, 0, 0 });
//	a->attachToCopter(c);
//	cout << "Acc:" << a->getValue().x << " "<< a->getValue().y << " "<< a->getValue().z << endl;
//	//c->advanceState();

//
//	SensorMagnetometer *mag= new SensorMagnetometer( { 0, 0, 0 }, { 0, 0, 0 });
//	mag->attachToCopter(c);
//
//	vect m = mag->getValue();
//
//
//	cout << "AccX:" << a.x << endl;
//	cout << "AccY:" << a.y << endl;
//	cout << "AccZ:" << a.z << endl;
//	cout << "MagX:" << m.x << endl;
//	cout << "MagY:" << m.y << endl;
//	cout << "MagZ:" << m.z << endl;

//	Simulation::Instance();
//
//	log_Init();
//	copter_MainProgram();
}
