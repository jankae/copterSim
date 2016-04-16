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
	SensorAccelerometer *acc = new SensorAccelerometer( { 33, 33, 33 }, { 0.1,
			0.1, 0.25 });
	acc->attachToCopter(c);
	for (int i = 0; i < 100; i++) {
		acc->copyRawValue();
		vect a;
		a.x = (int16_t)(accelerometer.rawValues[1]
				+ (accelerometer.rawValues[0] << 8));
		a.y = (int16_t)(accelerometer.rawValues[3]
				+ (accelerometer.rawValues[2] << 8));
		a.z = (int16_t)(accelerometer.rawValues[5]
				+ (accelerometer.rawValues[4] << 8));
		cout << a.x << " " << a.y << " " << a.z << endl;
	}
}
