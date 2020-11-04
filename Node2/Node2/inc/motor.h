#ifndef MOTOR_H_
#define MOTOR_H_


#include <stdint.h>

void motor_set_direction(int8_t dir);

void motor_init();

// Stepper speed should be commanded by joystick (-127, 127)
void motor_joystick_command(int8_t stepper_speed);

#endif /* STEPPER_H_ */