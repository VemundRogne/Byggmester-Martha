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
	
	// Move servo motor <3
	if(message->id == 50){
		TC0->TC_CHANNEL[0].TC_RA = 53332*((float)(message->data[0]) / 100.0);
	}
};