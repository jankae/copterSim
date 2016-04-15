#include "Prop.h"

using namespace std;

Prop::Prop(char *configFile, double diameter, double inert) {
	dia = diameter;
	inertia = inert;
	ifstream fin(configFile);
	if(!fin.is_open()){
		cout << "ERROR: couldn't open prop file" << endl;
		return;
	}
	string item;
	string line;
	getline(fin, line);
	while (getline(fin, line)) {
		istringstream in(line);
		for (int i = 0; i < 3; i++) {
			getline(in, item, ';');
			values.push_back(atof(item.c_str()));
		}
		coefficients.push_back(values);
		values.clear();
	}
	if(coefficients.empty()){
		cout << "ERROR: no prop coefficients" << endl;
		return;
	}
//	cout << "ConfigFile:" << endl;
//	vector<vector<double>>::iterator it_c;
//	for (it_c = coefficients.begin(); it_c < coefficients.end(); it_c++) {
//		cout << (*it_c)[0] << " " << (*it_c)[1] << " " << (*it_c)[2] << endl;
//	}
}
double Prop::get_thrust(double rpm) {
	// thrust = C_t * p * rpm^2 * diameter^4
	// interpolate coefficient C_t
	double C_t = 0;
	if(coefficients.empty()){
		// no prop coefficients -> no way to calculate thrust
		return 0.0;
	}
	vector<vector<double>>::iterator it = coefficients.begin();
	for (; it < coefficients.end(); it++) {
		if (rpm < (*it)[0])
			break;
	}
	if (it == coefficients.begin()) {
		// rpm lower than lowest rpm coefficient
		C_t = (*it)[1];
	} else if (it == coefficients.end()) {
		// rpm higher than highest rpm coefficient
		it--;
		C_t = (*it)[1];
	} else {
		// rpm between two rpm coefficients -> interpolate
		double lowRPM, highRPM;
		double lowC_t, highC_t;
		highRPM = (*it)[0];
		highC_t = (*it)[1];
		it--;
		lowRPM = (*it)[0];
		lowC_t = (*it)[1];
		double alpha = (rpm - lowRPM) / (highRPM - lowRPM);
		C_t = lowC_t + alpha * (highC_t - lowC_t);
	}

	// air density
	double p = 1.2041;

	// convert rpm from minutes to seconds
	rpm /= 60;

	double thrust = C_t * p * pow(rpm, 2) * pow(dia, 4);
	return thrust;

}
double Prop::get_torque(double rpm) {
	// torque = C_q * p * rpm^2 * diameter^5
	// C_q = C_p/2PI
	// interpolate coefficient C_t
	double C_p = 0;
	if(coefficients.empty()){
		// no prop coefficients -> no way to calculate torque
		return 0.0;
	}
	vector<vector<double>>::iterator it = coefficients.begin();
	for (; it < coefficients.end(); it++) {
		if (rpm < (*it)[0])
			break;
	}
	if (it == coefficients.begin()) {
		// rpm lower than lowest rpm coefficient
		C_p = (*it)[2];
	} else if (it == coefficients.end()) {
		// rpm higher than highest rpm coefficient
		it--;
		C_p = (*it)[2];
	} else {
		// rpm between two rpm coefficients -> interpolate
		double lowRPM, highRPM;
		double lowC_t, highC_t;
		highRPM = (*it)[0];
		highC_t = (*it)[2];
		it--;
		lowRPM = (*it)[0];
		lowC_t = (*it)[2];
		double alpha = (rpm - lowRPM) / (highRPM - lowRPM);
		C_p = lowC_t + alpha * (highC_t - lowC_t);
	}

	double C_q = C_p / (2 * M_PI);
	// air density
	double p = 1.2041;

	// convert rpm from minutes to seconds
	rpm /= 60;

	return C_q * p * pow(rpm, 2) * pow(dia, 5);
}
double Prop::get_inertia(void){
	return inertia;
}

