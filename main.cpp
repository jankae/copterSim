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
	Simulation::Instance();

	log_Init();
	copter_MainProgram();
}
