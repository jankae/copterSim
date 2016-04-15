/*
 * Clock.cpp
 *
 *  Created on: Oct 17, 2015
 *      Author: jan
 */

#include "Clock.h"

Clock* Clock::m_pInstance = NULL;

Clock* Clock::Instance() {
	if (!m_pInstance)
		m_pInstance = new Clock();
	return m_pInstance;
}

Clock::Clock() {
	struct timeval t;
	gettimeofday(&t, NULL);
	starttime = t.tv_sec * 1000000L + t.tv_usec;
}

uint64_t Clock::getTime_us(void) {
	struct timeval t;
	gettimeofday(&t, NULL);
	long long int currenttime = t.tv_sec * 1000000L + t.tv_usec;
	return currenttime - starttime;
}

uint32_t Clock::getTime_ms(void) {
	return this->getTime_us() / 1000;
}
void Clock::waitus(long long int us) {
	long long time = getTime_us();
	while (getTime_us() - time < us)
		;
}
