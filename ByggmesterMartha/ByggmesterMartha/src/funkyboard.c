
#define F_CPU	4915200

#include <util/delay.h>
#include <stdlib.h>

#include "../inc/funkyboard.h"
#include "../inc/adc.h"
#include "../inc/can.h"

void joystick_init(){

	// Wait a second to let the ADC-filter converge on a reasonable offset
	_delay_ms(1000);
	
	// Assuming zero offset
	joystick_offset_x = 0;
	joystick_offset_y = 0;

	// Find actual offset
	struct Joystick_pos joystick = get_joystick_pos();
	joystick_offset_x = joystick.x;
	joystick_offset_y = joystick.y;
}



struct Joystick_pos get_joystick_pos(){
	// Read all adc values
	uint8_t adc_values[4];
	adc_get_values(&adc_values[0]);

	// Store offset adjusted joystick positions
	struct Joystick_pos joystick;
	joystick.x = (int16_t)(adc_values[0]) - joystick_offset_x;
	joystick.y = (int16_t)(adc_values[1]) - joystick_offset_y;
	
	return joystick;
}


enum Joystick_dir get_joystick_dir(){
	struct Joystick_pos joystick = get_joystick_pos();
	
	// Check if neutral
	if ((abs(joystick.x) < 50) && (abs(joystick.y) < 50)){
		return NEUTRAL;
	}
	
	// If not neutral, check if joystick is more tilted in y direction
	if (abs(joystick.y)>abs(joystick.x)){
		// If so, what y-direction?
		if (joystick.y < -35){
			return DOWN;
		}
		return UP;
	}
	
	// If more in x-direction, test what x-direction
	if (joystick.x < -35){
		return LEFT;
	}
	return RIGHT;
	
}

	
void send_button_press(){
	// Check for buttonpress
	if ((PINB &= ( 1 << 3)) == ( 1 << 3)){
		// Create can message and send to node 2
		struct can_msg button_msg;
		button_msg.ID = 52;
		button_msg.len = 2;
		button_msg.data[0] = 1;
		
		can_transmit_message(&button_msg);
	}
}	


// Wraps value into 8 bit signed integer to accomadate a short CAN message
// Filter noise to make servo stop twitching
int8_t wrap_and_filter(int16_t value){
	if (abs(value)<15){
		return 0;
	}
	else if(value < -128){
		return -128;
	}
	else if(value > 127){
		return 127;
	}
	
	return (int8_t) value;
}

void send_joystick_position(struct Joystick_pos js_pos){
	union Data data;
	
	// Create can message
	struct can_msg js_msg;
	js_msg.ID = 69;
	js_msg.len = 2;

	// Load message with joystick data
	data.i = wrap_and_filter(js_pos.x);
	js_msg.data[0] = data.u;
	data.i = wrap_and_filter(js_pos.y);
	js_msg.data[1] = data.u;
	
	// Send joystick data to node 2
	can_transmit_message(&js_msg);
}