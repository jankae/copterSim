#include "led.h"

struct Led led;

void led_Cmd(uint8_t l, ledState s) {
	if (l <= 4) {
		if (led.state[l] != s) {
			led.state[l] = s;
			if (s == 0)
				printf("LED %i off\n", (int) l);
			else
				printf("LED %i on\n", (int) l);
		}
	}
}
