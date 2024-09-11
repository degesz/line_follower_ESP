#ifndef MOTOR_CTRL
#define MOTOR_CTRL

#include "driver/mcpwm.h"
#include "parameters.h"


void motor_setup();
void motor_write(int left, int right);
void motors_set_pwm_frequency(uint32_t frequency);



#endif //MOTOR_CTRL