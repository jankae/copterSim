
#ifndef SENSORRECEIVER_H_
#define SENSORRECEIVER_H_

#include "../common/rs232.hpp"

class SensorReceiver {
private:
	bool connectedToPort;
	int port;
public:
	SensorReceiver(int port);
	void Handler(void);
};

#endif /* SENSORRECEIVER_H_ */
