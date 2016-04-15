/*
 * led.h
 *
 *  Created on: Oct 17, 2015
 *      Author: jan
 */

#ifndef LED_H_
#define LED_H_

#include <stdint.h>
#include <stdio.h>

typedef enum {
	LED_OFF = 0, LED_ON = 1, LED_TOGGLE = 2
} ledState;

extern struct Led {
	uint8_t state[5];
} led;

void led_Cmd(uint8_t l, ledState s);

#endif /* LED_H_ */
