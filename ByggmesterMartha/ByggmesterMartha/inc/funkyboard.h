/*
 * mmi.h
 *
 * Created: 09/09/2020 16:13:57
 *  Author: vemun
 */ 


#ifndef MMI_H_
#define MMI_H_

#include <avr/io.h>

int joystick_offset_x;
int joystick_offset_y;

union Data {
	uint8_t u;
	int8_t i;
};

struct Joystick_pos{
	int x; 
	int y;
};

struct Joystick_pos get_joystick_pos();


struct Slider_pos {
	int right;
	int left;
};

void init_joystick();

enum Joystick_dir {LEFT = 0, RIGHT = 1, UP = 2, DOWN = 3, NEUTRAL = 4};

enum Joystick_dir get_joystick_dir();

struct Slider_pos get_slider_pos();

struct Joystick_pos get_joystick_pos();

struct Joystick_pos set_joystick_pos(uint8_t x_pos, uint8_t y_pos);

struct Slider_pos set_slider_pos(uint8_t right_pos, uint8_t left_pos);

void send_button_press();
void get_button_press();

uint8_t Joystick_can(struct Joystick_pos js_pos);

uint8_t slider_can(struct Slider_pos s_pos);

//joystick_press(); return true if pressed or something 

#endif /* MMI_H_ */