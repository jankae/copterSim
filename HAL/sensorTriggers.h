#ifndef SENSORTRIGGERS_H_
#define SENSORTRIGGERS_H_

#include <stdint.h>

extern struct SensorTrigger{
	uint8_t newDataI2C;
	uint8_t newDataExternalADC;
} sensorTrigger;

/*
 * return value:
 * 1 new data is available
 * 0 no new data
 *
 * The function clears the new-data-flag
 */
uint8_t i2c_NewSensorData(void);

/*
 * checks whether a new conversion is available
 */
uint8_t externalADC_Ready(void);

/*
 * retrieves the converted data from the ADC
 */
void externalADC_ReadData(void);

#endif
