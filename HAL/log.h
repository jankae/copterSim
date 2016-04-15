/*
 * handles all data written to the SD card. There are three types of logfiles:
 * 1. Systemlog
 * 		Named 'LOGxxx.TXT', where xxx is a decimal number. This file
 * 		is started right after booting and ends when the copter is powered down
 * 		(i.e. only one file per system-startup). It contains important system
 * 		information such as battery failure, sensor failure and the beginning
 * 		of a flight. Each entry has a timestamp indicating the time since boot
 * 2. FlightLog
 * 		Named 'FLLOGxxx.CSV', these files contain sensor data gathered during
 * 		a flight. A new flightlog is started at every start of the motors. The
 * 		content of the flightlogs can be configured via the CopterConfig tool.
 * 3. KalmanLog
 * 		Named 'KLLOGxxx.BIN', these files contain raw sensor data sampled at
 * 		the I2C_CYCLE_RATE (i.e. ALL sensordata). Additionally the true angle
 * 		(measured by an external potentiometer) is included. To reduce the
 * 		write time, this data is not encoded in ASCII code. The raw float values
 * 		are directly written into the logfile, following this format:
 * 		- each data packet consists of 44 bytes
 * 		- the first 12 bytes are the acceleration data (X-axis, Y-axis and Z-axis
 * 			in float representation)
 * 		- the next 12 bytes are the magnetometer data (same format as acceleration
 * 			data)
 * 		- the next 12 bytes are the gyro data (same format as acceleration data)
 * 		- the next 4 bytes are the 'true' angle
 * 		- the last 4 bytes contain a consecutive number (uint32_t) in order to
 * 			identify missing data packets
 */

#ifndef LOG_H_
#define LOG_H_

#include "hal.h"
#include "stringconversion.h"
#include "distance.h"
#include "imu.h"
#include "accelerometer.h"
#include "gyro.h"
#include "externalSensors.h"
#include <stdio.h>

/********************************************
 * systemlog configuration
 *******************************************/
// number of entries in buffer
#define LOG_ENTRY_BUFFER_SIZE		32
// maximum length of one entry
#define LOG_ENTRY_BUFFER_LENGTH		84
/********************************************
 * flightlog configuration
 *******************************************/
// maximum number of values per flightLog line
#define LOG_MAXIMUM_LOGVALUES_FLOAT		32
#define LOG_MAXIMUM_LOGVALUES_INT		9
/********************************************
 * kalmanlog configuration
 *******************************************/
#define LOG_KALMAN_BUFFER_LENGTH	32

struct KalmanLogEntry {
	float accX, accY, accZ;
	float magX, magY, magZ;
	float gyroX, gyroY, gyroZ;
	float trueAngle;
	// continuous numeration for detection dropped data
	uint32_t number;
};

extern struct Logfile {
	// card (physically) available
	uint8_t cardAvailable;
	// logFile was successful created
	uint8_t logFileRunning;
	// flightLog is currently active
	uint8_t flightLogRunning;
//	FATFS fatfs;
	// file handler
	FILE *logfile, *flightLog, *kalmanLog;
//	FIL logFile, flightLog, kalmanLog;
	// flightLog requests
	uint8_t flightLogStartRequest;
	uint8_t flightLogStopRequest;
	uint32_t flightLogStartTimestamp;

	uint8_t cardMissingCounter;

	/*
	 * pointer for flightLog entries
	 */
	float *flightLogEntry[LOG_MAXIMUM_LOGVALUES_FLOAT];
	int32_t *flightLogEntry2[LOG_MAXIMUM_LOGVALUES_INT];
#define LOG_INT_UINT8		0
#define LOG_INT_INT8		1
#define LOG_INT_UINT16		2
#define LOG_INT_INT16		3
#define LOG_INT_INT32		4
	uint8_t flightLogEntry2IntType[LOG_MAXIMUM_LOGVALUES_INT];

	/*
	 * ring-buffer for logfile entries
	 */
	// actual text entry
	char entryBuffer[LOG_ENTRY_BUFFER_SIZE][LOG_ENTRY_BUFFER_LENGTH];
	// timestamp of the entry
	uint32_t entryTimestamp[LOG_ENTRY_BUFFER_SIZE];
	// length of the entry in chars
	uint8_t entryLength[LOG_ENTRY_BUFFER_SIZE];
	uint8_t entryBufferRead;
	uint8_t entryBufferWrite;

	/*
	 * KalmanLog data
	 */
	struct KalmanLogEntry kalmanEntries[LOG_KALMAN_BUFFER_LENGTH];
	uint8_t kalmanBufferRead;
	uint8_t kalmanBufferWrite;
	uint8_t kalmanLogStartRequest;
	uint8_t kalmanLogStopRequest;
	uint32_t kalmanLogEntryNumber;
	uint8_t kalmanLogRunning;

	/*
	 * various flags (extracted from flag file if available)
	 */
	struct {
		uint8_t baroTempProfileActive;
	} flags;
} logfile;

/*
 * creates the necessary directories
 */
void log_Init(void);

uint8_t log_CheckIfCardAvailable(void);

/*
 * prepares a line to be entered into the logfile.
 */
void log_LogFileEntry(char* entry);

/*
 * writes the buffered entries into the logfile
 */
void log_FlushBufferedEntries(void);

/*
 * flushes all data into logfiles
 */
void log_SyncFileSystem(void);

/*********************************************
 * flightlog functions
 ********************************************/
/*
 * requests the start of a new flightLog
 */
void log_FlightLogStartRequest(void);

/*
 * requests the end of the current flightLog
 */
void log_FlightLogStopRequest(void);

/*
 * starts a new flightLog.
 * !! DO NOT USE THIS FUNCTION IN AN INTERRUPT !!
 * use log_FlightLogStartRequest instead
 */
void log_StartFlightLog(void);

/*
 * stops the current flightLog
 * !! DO NOT USE THIS FUNCTION IN AN INTERRUPT !!
 * use log_FlightLogStopRequest instead
 */
void log_StopFlightLog(void);

/*
 * writes a new data set into the flightLog
 */
void log_FlightLogAddEntry(void);

/*********************************************
 * kalmanlog functions
 ********************************************/
/*
 * requests the start of a new kalmanLog
 */
void log_KalmanLogStartRequest(void);

/*
 * requests the end of the current kalmanLog
 */
void log_KalmanLogStopRequest(void);

/*
 * starts a new kalmanLog.
 * !! DO NOT USE THIS FUNCTION IN AN INTERRUPT !!
 * use log_KalmanLogStartRequest instead
 */
void log_StartKalmanLog(void);

/*
 * stops the current kalmanLog
 * !! DO NOT USE THIS FUNCTION IN AN INTERRUPT !!
 * use log_kalmanLogStopRequest instead
 */
void log_StopKalmanLog(void);

/*
 * adds a new set of data to the kalman log buffer
 */
void log_KalmanLogAddEntry(void);

/*
 * writes buffered entries into kalmanlog
 */
void log_KalmanLogFlushEntries(void);

#endif /* LOG_H_ */
