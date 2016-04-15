
#ifndef DEFINES_H_
#define DEFINES_H_

#define SIMULATION

#define ROLL_AXIS 0
#define PITCH_AXIS 1
#define YAW_AXIS 2
#define X_AXIS 0
#define Y_AXIS 1
#define Z_AXIS 2

#include <math.h>

// magnetic field defines
#define MAGNETIC_FIELD_DECLINATION 70
#define MAGNETIC_FIELD_STRENGTH		0.5

#define DEG_TO_RAD M_PI/180.0
#define RAD_TO_DEG 180.0/M_PI

typedef enum {DISABLE = 0, ENABLE = !DISABLE} FunctionalState;
typedef enum {RESET = 0, SET = !RESET} FlagStatus, ITStatus;
typedef enum {ERROR = 0, SUCCESS = !ERROR} ErrorStatus;

#define UINT32_MAX (4294967295U)
#define INT16_MIN (-32767-1)
#define INT16_MAX (32767)


#endif
