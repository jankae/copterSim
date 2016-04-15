
#ifndef TIME_H_
#define TIME_H_

#include "../PhysikEngine/Clock.h"
#include <stdint.h>

int32_t time_GetMillis(void);

int32_t time_Get100us(void);

void time_Waitms(uint16_t ms);

/*
 * initializes a timer
 * @param timer pointer to the timer
 * @param time time in ms until the timer should elapse
 */
void time_SetTimer(uint32_t *timer, uint32_t time);
/*
 * checks whether a timer has elapsed
 * @param timer pointer to the timer
 * @return 0 timer still running / 1 timer elapsed
 */
uint8_t time_TimerElapsed(uint32_t *timer);

#endif
