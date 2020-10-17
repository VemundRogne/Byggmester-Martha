/*
 * mmi.c
 *
 * Created: 09/09/2020 16:14:17
 *  Author: vemun
 */ 

#include "../inc/funkyboard.h"
#include "../inc/adc.h"
#define F_CPU	4915200
#include <util/delay.h>
#include <stdlib.h>

void init_joystick(){
	_delay_ms(1000);
	struct Joystick_pos joystick = get_joystick_pos();
	joystick_offset_x = joystick.x;
	joystick_offset_y = joystick.y;
}

struct Joystick_pos get_joystick_pos(){
	uint8_t adc_values[4];
	adc_get_values(&adc_values[0]);
	struct Joystick_pos joystick;

	joystick.x = (int)(adc_values[0]) - joystick_offset_x;
	joystick.y = (int)(adc_values[1]) - joystick_offset_y;
	return joystick;
}

enum Joystick_dir get_joystick_dir(){
	struct Joystick_pos joystick = get_joystick_pos();
	
	if ((abs(joystick.x) < 50) && (abs(joystick.y) < 50)){
		return NEUTRAL;
	}
	
	if (abs(joystick.y)>abs(joystick.x)){
		if (joystick.y < -35){
			return DOWN;
		}
		return UP;
	}
	
	if (joystick.x < -35){
		return LEFT;
	}
	return RIGHT;
	
	};

struct Slider_pos get_slider_pos(){
	uint8_t adc_values[4];
	rd_adc(&adc_values);
	struct Slider_pos slider;
	slider.right = (int) (((adc_values[2]-128)*100)/128);
	slider.left = (int) (((adc_values[3]-128)*100)/128);
	return slider;
	};