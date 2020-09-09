/*
 * mmi.c
 *
 * Created: 09/09/2020 16:14:17
 *  Author: vemun
 */ 

#include "../inc/mmi.h"
#include "../inc/adc.h"
#define F_CPU	4915200
#include <util/delay.h>

void init_joystick(){
	_delay_ms(100);
	struct Joystick_pos joystick = get_joystick_pos();
	joystick_offset_x = joystick.x;
	joystick_offset_y = joystick.y;
}

struct Joystick_pos get_joystick_pos(){
	uint8_t adc_values[4];
	rd_adc(&adc_values);
	struct Joystick_pos joystick;
	joystick.x = (int) (((adc_values[0]-128)*100)/128) - joystick_offset_x;
	joystick.y = (int) (((adc_values[1]-128)*100)/128) - joystick_offset_y;
	return joystick;
}

enum Joystick_dir get_joystick_dir(){
	struct Joystick_pos joystick = get_joystick_pos();
	
//	uint8_t adc_values[4];
//	rd_adc(&adc_values);
	
//	int x = adc_values[0];
//	int y = adc_values[1];
	
	if ((abs(joystick.x) < 50) && (abs(joystick.y) < 50)){
		return NEUTRAL;
	}
	
	if (abs(joystick.y)>abs(joystick.x)){
		if (joystick.y < 0){
			return DOWN;
		}
		return UP;
	}
	
	if (joystick.x < 0){
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