#ifndef PROP_H_
#define PROP_H_

#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <iterator>
#include <math.h>

class Prop {
private:
	std::vector<std::vector<double> > coefficients;
	std::vector<double> values;
	double dia;
	double inertia;
public:
	Prop(char *configFile, double diameter, double inert);
	/*
	 * returns the generated thrust in N at a specific rpm rate
	 */
	double get_thrust(double rpm);
	/*
	 * returns the needed torque in Nm to hold a specific rpm rate
	 */
	double get_torque(double rpm);
	double get_inertia(void);
};

#endif /* PROP_H_ */
