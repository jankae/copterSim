#include "motors.h"

void externalI2C_TriggerUpdate(void){
	// TODO transfer motor values from motor.velocity into motor classes
	Simulation::Instance()->setMotors();
}

/*
 * updates a servo position
 * @param servo servo number from 0 to 11
 * @param position servo position, -1500=1ms; 0=1.5ms; 1500=2ms
 */
void servo_SetPosition(uint8_t servo, int16_t position){

}
