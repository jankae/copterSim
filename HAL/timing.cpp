#include <timing.h>

int32_t time_GetMillis(void) {
	return Clock::Instance()->getTime_ms();
}

int32_t time_Get100us(void) {
	return Clock::Instance()->getTime_us() / 100;
}

void time_Waitms(uint16_t ms) {
	int32_t tstart = Clock::Instance()->getTime_ms();
	while (Clock::Instance()->getTime_ms() - tstart < ms)
		;
}

/*
 * initializes a timer
 * @param timer pointer to the timer
 * @param time time in ms until the timer should elapse
 */
void time_SetTimer(uint32_t *timer, uint32_t time) {
	*timer = Clock::Instance()->getTime_us() / 100 + time * 10;
}
/*
 * checks whether a timer has elapsed
 * @param timer pointer to the timer
 * @return 0 timer still running / 1 timer elapsed
 */
uint8_t time_TimerElapsed(uint32_t *timer) {
	if (*timer >= Clock::Instance()->getTime_us() / 100)
		return 0;
	else
		return 1;
}
