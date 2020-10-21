#ifndef JOYSTICK_H_
#define JOYSTICK_H_

// Buffers to save joystick values and direction when can message is received with new joystick values
volatile uint8_t joystick_values[2]; //[x_val, y_val];
volatile uint8_t joystick_dir;	//From enum in node 1 (LEFT = 0, RIGHT = 1, UP = 2, DOWN = 3, NEUTRAL = 4)


#endif /* JOYSTICK_H_ */