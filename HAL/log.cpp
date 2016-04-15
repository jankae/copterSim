#include "log.h"

struct Logfile logfile;

/*
 * creates the necessary directories
 */
void log_Init(void) {
	//set pointers to logValues
	logfile.flightLogEntry[0] = &attitude.pitch;
	logfile.flightLogEntry[1] = &attitude.roll;
	logfile.flightLogEntry[2] = &attitude.yaw;
	logfile.flightLogEntry[3] = &accelerometer.X;
	logfile.flightLogEntry[4] = &accelerometer.Y;
	logfile.flightLogEntry[5] = &accelerometer.Z;
	logfile.flightLogEntry[6] = &accelerometer.magnitude;
	logfile.flightLogEntry[7] = &magnetometer.X;
	logfile.flightLogEntry[8] = &magnetometer.Y;
	logfile.flightLogEntry[9] = &magnetometer.Z;
	logfile.flightLogEntry[10] = &magnetometer.magnitude;
	logfile.flightLogEntry[11] = &gyro.X;
	logfile.flightLogEntry[12] = &gyro.Y;
	logfile.flightLogEntry[13] = &gyro.Z;
	logfile.flightLogEntry[14] = &gps.position.X;
	logfile.flightLogEntry[15] = &gps.position.Y;
	logfile.flightLogEntry[16] = &gps.position.Z;
	logfile.flightLogEntry[17] = &gps.velocity.X;
	logfile.flightLogEntry[18] = &gps.velocity.Y;
	logfile.flightLogEntry[19] = &pressure.height;
	logfile.flightLogEntry[20] = &externalSensor.angle;
	logfile.flightLogEntry[21] = &position.X;
	logfile.flightLogEntry[22] = &position.velocity.X;
	logfile.flightLogEntry[23] = &position.acceleration.X;
	logfile.flightLogEntry[24] = &position.Y;
	logfile.flightLogEntry[25] = &position.velocity.Y;
	logfile.flightLogEntry[26] = &position.acceleration.Y;
	logfile.flightLogEntry[27] = &position.Z;
	logfile.flightLogEntry[28] = &position.velocity.Z;
	logfile.flightLogEntry[29] = &position.acceleration.Z;
	logfile.flightLogEntry[30] = &distance.bottom;
	logfile.flightLogEntry[31] = &flightState.howerPower;
	// pointer to integer log values
	logfile.flightLogEntry2[0] = (int32_t*) &battery.voltage;
	logfile.flightLogEntry2IntType[0] = LOG_INT_UINT16;
	logfile.flightLogEntry2[1] = (int32_t*) &battery.usedCapacity;
	logfile.flightLogEntry2IntType[1] = LOG_INT_UINT16;
	logfile.flightLogEntry2[2] = (int32_t*) &battery.current;
	logfile.flightLogEntry2IntType[2] = LOG_INT_UINT16;
	logfile.flightLogEntry2[3] = (int32_t*) &gps.latitude;
	logfile.flightLogEntry2IntType[3] = LOG_INT_INT32;
	logfile.flightLogEntry2[4] = (int32_t*) &gps.longitude;
	logfile.flightLogEntry2IntType[4] = LOG_INT_INT32;
	logfile.flightLogEntry2[5] = (int32_t*) &gps.altitude;
	logfile.flightLogEntry2IntType[5] = LOG_INT_INT32;
	logfile.flightLogEntry2[6] = (int32_t*) &gps.heading;
	logfile.flightLogEntry2IntType[6] = LOG_INT_UINT16;
	logfile.flightLogEntry2[7] = (int32_t*) &gps.speed;
	logfile.flightLogEntry2IntType[7] = LOG_INT_UINT16;
	logfile.flightLogEntry2[8] = (int32_t*) &pressure.ADCValue;
	logfile.flightLogEntry2IntType[8] = LOG_INT_INT32;
	logfile.logfile = fopen("Systemlog.txt", "w");
	if (logfile.logfile) {
		logfile.logFileRunning = 1;
	} else
		logfile.logFileRunning = 0;
}

uint8_t log_CheckIfCardAvailable(void) {
	return 1;
}

/*
 * prepares a line to be entered into the logfile.
 */
void log_LogFileEntry(char* entry) {
	if (logfile.logFileRunning) {
		uint8_t minutes, seconds;
		uint32_t millis;
		millis = time_GetMillis();
		minutes = millis / 60000;
		seconds = (millis / 1000) % 60;
		millis %= 1000;
		char timeString[11];
		/*
		 * timeString format:
		 * 'mm:ss.mmm '
		 */
		timeString[0] = minutes / 10 + '0';
		timeString[1] = minutes % 10 + '0';
		timeString[2] = ':';
		timeString[3] = seconds / 10 + '0';
		timeString[4] = seconds % 10 + '0';
		timeString[5] = '.';
		timeString[6] = millis / 100 + '0';
		timeString[7] = (millis / 10) % 10 + '0';
		timeString[8] = millis % 10 + '0';
		timeString[9] = ' ';
		timeString[10] = 0;
		fprintf(logfile.logfile, timeString);
		fprintf(logfile.logfile, entry);
		fprintf(logfile.logfile, "\r\n");
	}
}

/*
 * writes the buffered entries into the logfile
 */
void log_FlushBufferedEntries(void) {
}

/*
 * flushes all data into logfiles
 */
void log_SyncFileSystem(void) {
	if (logfile.logFileRunning)
		fflush(logfile.logfile);
}

/*********************************************
 * flightlog functions
 ********************************************/
/*
 * requests the start of a new flightLog
 */
void log_FlightLogStartRequest(void) {
	logfile.flightLogStartRequest = 1;
}

/*
 * requests the end of the current flightLog
 */
void log_FlightLogStopRequest(void) {
	if (logfile.flightLogRunning)
		logfile.flightLogStopRequest = 1;
}

/*
 * starts a new flightLog.
 * !! DO NOT USE THIS FUNCTION IN AN INTERRUPT !!
 * use log_FlightLogStartRequest instead
 */
void log_StartFlightLog(void) {
	logfile.flightLogStartRequest = 0;
	// close the potentially running flightLog
	if (logfile.flightLogRunning)
		log_StopFlightLog();
	/*
	 * Name format for flightLogs is FLLOGXXX.CSV, where XXX is a 3-digit number.
	 * Already existent files are skipped.
	 */
	char logfilename[] = "FLLOG000.CSV";
	uint8_t fileAlreadyExists = 1;
	logfilename[5] = '0' - 1;
	do {
		logfilename[5]++;
		logfilename[6] = '0' - 1;
		do {
			logfilename[6]++;
			logfilename[7] = '0' - 1;
			do {
				logfilename[7]++;
				if ((logfile.flightLog = fopen(logfilename, "r"))) {
					fclose(logfile.flightLog);
				} else {
					logfile.flightLog = fopen(logfilename, "w");
					fileAlreadyExists = 0;
				}
			} while (logfilename[7] < '9' && fileAlreadyExists);
		} while (logfilename[6] < '9' && fileAlreadyExists);
	} while (logfilename[5] < '9' && fileAlreadyExists);
	if (fileAlreadyExists) {
		// wasn't able to create file
		log_LogFileEntry("ERROR: failed to create flightLog");
		return;
	}
	logfile.flightLogRunning = 1;
	char logentry[] = "created flightLog:        ";
	uint8_t i;
	for (i = 0; i < 8; i++) {
		logentry[i + 18] = logfilename[i];
	}
	log_LogFileEntry(logentry);
	/*
	 * write title line of flightLog
	 */
	// float values
	char logValueName[LOG_MAXIMUM_LOGVALUES_FLOAT][15] = { "pitch", "roll",
			"yaw", "accX", "accY", "accZ", "accMagnitude", "magX", "magY",
			"magZ", "magMagnitude", "gyroX", "gyroY", "gyroZ", "GPS X", "GPS Y",
			"GPS Z", "GPSvelX", "GPSvelY", "baroHeight", "extTilt", "PositionX",
			"VelocityX", "accelerationX", "PositionY", "VelocityY",
			"accelerationY", "PositionZ", "VelocityZ", "accelerationZ",
			"distanceBottom", "howerpower" };
	char logline[2000];
	char *p = logline;
	uint32_t mask = config.flightLogMask;
	for (i = 0; i < LOG_MAXIMUM_LOGVALUES_FLOAT; i++) {
		if (mask & 0x01) {
			p = StringCopy(p, logValueName[i]);
			*p++ = ';';
		}
		mask >>= 1;
	}
	// integer values
	char logValueName2[LOG_MAXIMUM_LOGVALUES_INT][15] = { "batVoltage",
			"usedCapacity", "current", "GPSLatitude", "GPSLongitude",
			"GPSAltitude", "GPSHeading", "GPSSpeed", "rawPressureADC" };
	mask = config.flightLogMask2;
	for (i = 0; i < LOG_MAXIMUM_LOGVALUES_INT; i++) {
		if (mask & 0x01) {
			p = StringCopy(p, logValueName2[i]);
			*p++ = ';';
		}
		mask >>= 1;
	}
	p--;
	*p++ = '\r';
	*p++ = '\n';
	*p++ = '\0';
	fprintf(logfile.flightLog, logline);
}

/*
 * stops the current flightLog
 * !! DO NOT USE THIS FUNCTION IN AN INTERRUPT !!
 * use log_FlightLogStopRequest instead
 */
void log_StopFlightLog(void) {
	logfile.flightLogStopRequest = 0;
	if (logfile.flightLogRunning) {
		// close file
		fclose(logfile.flightLog);
		logfile.flightLogRunning = 0;
		log_LogFileEntry("stopped flightLog");
	}
}

/*
 * writes a new data set into the flightLog
 */
void log_FlightLogAddEntry(void) {
	if (logfile.flightLogRunning) {
		char logline[2000];
		char *p = logline;
		uint32_t mask = config.flightLogMask;
		uint8_t i;
		for (i = 0; i < LOG_MAXIMUM_LOGVALUES_FLOAT; i++) {
			if (mask & 0x01) {
				p = ftoa(*logfile.flightLogEntry[i], p);
				*p++ = ';';
			}
			mask >>= 1;
		}
		mask = config.flightLogMask2;
		for (i = 0; i < LOG_MAXIMUM_LOGVALUES_INT; i++) {
			if (mask & 0x01) {
				int32_t value = 0;
				switch (logfile.flightLogEntry2IntType[i]) {
				case LOG_INT_UINT8:
					value = *(uint8_t*) logfile.flightLogEntry2[i];
					break;
				case LOG_INT_INT8:
					value = *(int8_t*) logfile.flightLogEntry2[i];
					break;
				case LOG_INT_UINT16:
					value = *(uint16_t*) logfile.flightLogEntry2[i];
					break;
				case LOG_INT_INT16:
					value = *(int16_t*) logfile.flightLogEntry2[i];
					break;
				case LOG_INT_INT32:
					value = *logfile.flightLogEntry2[i];
					break;
				}
				p = itoASCII(value, p);
				*p++ = ';';
			}
			mask >>= 1;
		}
		p--;
		*p++ = '\r';
		*p++ = '\n';
		fwrite(logline, sizeof(char), p - logline, logfile.flightLog);
	}
}

/*********************************************
 * kalmanlog functions
 ********************************************/
/*
 * requests the start of a new kalmanLog
 */
void log_KalmanLogStartRequest(void) {

}

/*
 * requests the end of the current kalmanLog
 */
void log_KalmanLogStopRequest(void) {

}

/*
 * starts a new kalmanLog.
 * !! DO NOT USE THIS FUNCTION IN AN INTERRUPT !!
 * use log_KalmanLogStartRequest instead
 */
void log_StartKalmanLog(void) {

}

/*
 * stops the current kalmanLog
 * !! DO NOT USE THIS FUNCTION IN AN INTERRUPT !!
 * use log_kalmanLogStopRequest instead
 */
void log_StopKalmanLog(void) {

}

/*
 * adds a new set of data to the kalman log buffer
 */
void log_KalmanLogAddEntry(void) {

}

/*
 * writes buffered entries into kalmanlog
 */
void log_KalmanLogFlushEntries(void) {

}
