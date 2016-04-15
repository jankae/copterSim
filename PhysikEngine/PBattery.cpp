#include "PBattery.h"

P_Battery::P_Battery(double vol, double res) {
	voltage = vol;
	resistance = res;
}
void P_Battery::useCurrent(double cur) {
	current = cur;
}
double P_Battery::get_Voltage(void) {
	return voltage - current * resistance;
}

