/*
 * can_handler.c
 *
 * Created: 21/10/2020 10:49:37
 *  Author: vemun
 */ 

#include <stdint.h>
#include "sam.h"

#include "../inc/can_controller.h"
#include "../inc/can_handler.h"
#include "../inc/servo.h"
#include <component/tc.h>

union Data {
	uint8_t u;
	int8_t i;
};

void handle_can_message(struct can_message_t *message){
	// The message contains info about the joystick!
	if (message->id == 69){
		union Data data;
		
		data.u = message->data[0];
		volatile int8_t joystick_x = data.i;
		
		data.u = message->data[1];
		volatile int8_t joystick_y = data.i;
		
		REG_PIOA_SODR |= (1<<19);
	}
	
	// Move servo motor <3 data should contain some duty cycle?
	if(message->id == 50){
		uint8_t position = (uint8_t)(message->data[0]);
		servo_set_position(position);
	}
};