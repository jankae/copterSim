#include "sensorTriggers.h"

struct SensorTrigger sensorTrigger;

/*
 * return value:
 * 1 new data is available
 * 0 no new data
 *
 * The function clears the new-data-flag
 */
uint8_t i2c_NewSensorData(void){
	if (sensorTrigger.newDataI2C) {
		sensorTrigger.newDataI2C = 0;
			return 1;
		} else
			return 0;
	}
/*
 * checks whether a new conversion is available
 */
uint8_t externalADC_Ready(void){
	return sensorTrigger.newDataExternalADC;
}

/*
 * retrieves the converted data from the ADC
 */
void externalADC_ReadData(void){
	// TODO move raw pressure data into pressure.rawADC
}
