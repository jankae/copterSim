
#ifndef MOTORS_H_
#define MOTORS_H_

#include <stdint.h>
#include "../Simulation/Simulation.h"

void externalI2C_TriggerUpdate(void);

/*
 * updates a servo position
 * @param servo servo number from 0 to 11
 * @param position servo position, -1500=1ms; 0=1.5ms; 1500=2ms
 */
void servo_SetPosition(uint8_t servo, int16_t position);

#endif
