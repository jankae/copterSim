
#ifndef INTERNALI2C_H_
#define INTERNALI2C_H_

#include "hal.h"

extern struct InternalI2C{
	// this flag enables/disables the autoreading feature
	FunctionalState autoReading;
	// current position in the auto-reading cycle
	uint8_t autoPosition;
	uint8_t autoCycleLength;
	/*
	 * Flags incicating available sensors
	 */
	FlagStatus accAvailable, gyroAvailable, magAvailable;
	/*
	 * flag is set when a timeout occurs. in this case the cycle must be started
	 * manual by calling i2c_NextReading
	 */
	FlagStatus error;
	uint32_t errorMessageTimer;
	/*
	 * flag indicates new unread data. It must be reset by the application code once
	 * the data has been read
	 */
	FlagStatus newData;
	/*
	 * flag indicates whether a auto-reading cycle is currently running
	 */
	volatile FlagStatus autoReadingRunning;
} internali2c;


#endif /* INTERNALI2C_H_ */
