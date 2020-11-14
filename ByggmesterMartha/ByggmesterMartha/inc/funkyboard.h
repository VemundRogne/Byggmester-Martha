
#ifndef FUNKYBOARD_H_
#define FUNKYBOARD_H_

#include <avr/io.h>

// Compensation for biased joystick
int16_t joystick_offset_x;
int16_t joystick_offset_y;

// Union to send int8_t as uint8_t values over CAN
union Data {
	uint8_t u;
	int8_t i;
};

struct Joystick_pos{
	int16_t x; 
	int16_t y;
};
enum Joystick_dir {LEFT = 0, RIGHT = 1, UP = 2, DOWN = 3, NEUTRAL = 4};



void joystick_init();

// Get ADC values 
struct Joystick_pos get_joystick_pos();
enum Joystick_dir get_joystick_dir();

// Send values to node 2
void send_button_press();
uint8_t send_joystick_position(struct Joystick_pos js_pos);


#endif /* FUNKYBOARD_H_ */