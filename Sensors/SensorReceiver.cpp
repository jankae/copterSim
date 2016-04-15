
#include "SensorReceiver.h"
#include "../receiver.h"

#include <iostream>

using namespace std;

SensorReceiver::	SensorReceiver(int port) {
	connectedToPort = false;
	if(!RS232_OpenComport(port, 115200)){
		connectedToPort = true;
		this->port = port;
	}
}

void SensorReceiver::Handler(void){
	if(connectedToPort){
		uint8_t data;
		while(RS232_PollComport(port, &data, 1)){
			receiver_IncomingData(data);
		}
	}
}

