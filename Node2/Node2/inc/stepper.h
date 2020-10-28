#ifndef STEPPER_H_
#define STEPPER_H_

void stepper_init();

// Stepper speed should be commanded by joystick (-127, 127)
void stepper_joystick_command(int8_t stepper_speed);





#endif /* STEPPER_H_ */