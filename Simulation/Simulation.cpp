#include "Simulation.h"
#include "../copter.h"
#include "../motor.h"

using namespace std;

Simulation* Simulation::m_pInstance = NULL;

Simulation* Simulation::Instance() {
	if (!m_pInstance)
		m_pInstance = new Simulation();
	return m_pInstance;
}

Simulation::Simulation() {
	vect rpy = { 0, 0, 0 };
	vect p = { 0, 0, -10 };
	copter = new Copter(rpy, p);
	// create sensors
	acc = new SensorAccelerometer( { 33, 33, 33 }, { 0.1, 0.1, 0.05 });
	gyr = new SensorGyro( { 5, 7, 10 }, { 0.002, 0.0045, 0.0042 });
	mag = new SensorMagnetometer( { 3, 5, 3 }, { 0, 0, 0 });
	acc->attachToCopter(copter);
	gyr->attachToCopter(copter);
	mag->attachToCopter(copter);
	// create motors
	p = {0.2,0,0};
	mot1 = new P_Motor(rpy, p, true);
	p = {0,-0.2,0};
	mot2 = new P_Motor(rpy, p, false);
	p = {-0.2,0,0};
	mot3 = new P_Motor(rpy, p, true);
	p = {0,0.2,0};
	mot4 = new P_Motor(rpy, p, false);
	prop1 = new Prop((char*)"propConfig.csv", 0.254, 0.00004);
	prop2 = new Prop((char*)"propConfig.csv", 0.254, 0.00004);
	prop3 = new Prop((char*)"propConfig.csv", 0.254, 0.00004);
	prop4 = new Prop((char*)"propConfig.csv", 0.254, 0.00004);
	bat = new P_Battery(14.8, 0.01);
	mot1->attachProp(prop1);
	mot2->attachProp(prop2);
	mot3->attachProp(prop3);
	mot4->attachProp(prop4);
	mot1->attachToBattery(bat);
	mot2->attachToBattery(bat);
	mot3->attachToBattery(bat);
	mot4->attachToBattery(bat);
	copter->addMotor(mot1);
	copter->addMotor(mot2);
	copter->addMotor(mot3);
	copter->addMotor(mot4);
	rec = new SensorReceiver(16);
	starttime = Clock::Instance()->getTime_us();
	lastSensorUpdate = starttime;
	lastTask = starttime;
	printTime = starttime;
	handlerTime = starttime;
	bat->copyRawValue();
}

void Simulation::Handler(void) {
	long long time = Clock::Instance()->getTime_us();
//	long long timediff = time - handlerTime;
	handlerTime = time;
//	long freq = 1000000 / timediff;
//	cout << "SimHandlerFreq:" << freq << endl;
	if (time - lastSensorUpdate >= SIMULATION_SENSOR_PERIOD) {
		lastSensorUpdate += SIMULATION_SENSOR_PERIOD;
		acc->copyRawValue();
		gyr->copyRawValue();
		mag->copyRawValue();
		internali2c.accAvailable = SET;
		internali2c.gyroAvailable = SET;
		internali2c.magAvailable = SET;
		sensorTrigger.newDataI2C = 1;
	}
	if (time - lastTask >= SIMULATION_TASKS_PERIOD) {
		lastTask += SIMULATION_TASKS_PERIOD;
		copter_ImportantTasks();
	}
	rec->Handler();

	if (time - printTime >= 50000) {
		printTime += 50000;
		copter->printState();
//		vect a = copter->position->toRPY();
//		cout << "SRPY: " << a.x << " " << a.y << " " << a.z << endl;
//		cout << "MRPY: " << attitude.roll << " " << attitude.pitch << " " << attitude.yaw << endl;
//		cout << "Kalman states:";
//		for (int i = 0; i < 12; i++) {
//			cout << " " << kalman.x[i];
//		}
//		cout << endl;
//		cout << "Motors: " << mot1->get_Thrust() << " " << mot2->get_Thrust()
//				<< " " << mot3->get_Thrust() << " " << mot4->get_Thrust()
//				<< endl;
//		cout << copter->angularAcceleration.z << " "
//				<< copter->angularVelocity.z << endl;

	}
	copter->calcAcceleration();
	copter->advanceState();
	bat->copyRawValue();
}

void Simulation::setMotors(void) {
	copter->setMotors(motor.velocity);
}

