#include "functions.h"

int constrain(int v, int lower, int upper) {
	if (v < lower)
		v = lower;
	else if (v > upper)
		v = upper;
	return v;
}
