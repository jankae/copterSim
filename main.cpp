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
	Prop *p = new Prop("propConfig.csv", 0.254, 0.00005);
//	for (int i = 1000; i < 7000; i += 100) {
//		cout << "rpm: " << i << " thrust: " << p->get_thrust(i) << " torque: "
//				<< p->get_torque(i) << endl;
//	}
	P_Motor *m = new P_Motor( { 0, 0, 0 }, { 0, 0, 0 }, true);
	P_Battery *b = new P_Battery(14.8, 0.01);
	m->attachProp(p);
	m->attachToBattery(b);
	cout
			<< "time[ms] rpm[1/s] voltage[V] current[A] torque_m[Nm] torque_p[Nm] thrust[N]"
			<< endl;
	m->set_PWM(50);
	for (int i = 0; i < 2000; i++) {
		m->update();
		b->useCurrent(m->current);
		cout << Clock::Instance()->getTime_ms() << " " << m->rpm << " "
				<< b->get_Voltage() << " " << m->current << " "
				<< m->get_torque() << " " << p->get_torque(m->rpm) << " "
				<< p->get_thrust(m->rpm) << "\n";
		if (i == 500)
			m->set_PWM(255);
		if (i == 1000)
			m->set_PWM(80);
		Clock::Instance()->waitus(1000);
	}
	cout << flush;
}
