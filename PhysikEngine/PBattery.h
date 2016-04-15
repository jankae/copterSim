/*
 * PBattery.h
 *
 *  Created on: Oct 20, 2015
 *      Author: jan
 */

#ifndef PBATTERY_H_
#define PBATTERY_H_

class P_Battery {
private:
	double voltage;
	double current;
	double resistance;
public:
	P_Battery(double vol, double res);
	void useCurrent(double cur);
	double get_Voltage(void);
};

#endif /* PBATTERY_H_ */
