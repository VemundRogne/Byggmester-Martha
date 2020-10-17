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

uint8_t Joystick_can();

//joystick_press(); return true if pressed or something 

#endif /* MMI_H_ */