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
#include "../inc/motor.h"
#include <component/tc.h>
#include "../inc/joystick.h"
#include "../inc/ir_driver.h"
#include "../inc/solenoid.h"

union Data {
	uint8_t u;
	int8_t i;
};



void handle_can_message(struct can_message_t *message){
	// The message contains info about the joystick!
	if (message->id == 68){
		union Data data;
		
		data.u = message->data[0];
		joystick_values[0] = data.i;
		volatile int8_t joystick_x = data.i;

		data.u = message->data[1];
		joystick_values[1] = data.i;
		volatile int8_t joystick_y = data.i;
		
		REG_PIOA_SODR |= (1<<19);
	}

	// Move servo and stepper from joystick! 
	if (message->id == 69){
		union Data data;
		
		data.u = message->data[0];
		stepper_joystick_command(data.i);

		data.u = message->data[1];
		servo_joystick_command(data.i);
	}


	// Move servo motor <3 data should contain some duty cycle?
	if(message->id == 50){
		uint8_t position = (uint8_t)(message->data[0]);
		servo_set_position(position);
	}

	if (message->id == 5){
		transmit_ball_status_flag = message->data[0];
	}
	
	if ((message->id == 52) && (message->data[0] == 1)){
		uint8_t pulse_length = message->data[1];
		solenoid_push_ball(pulse_length);
	}
	
	ir_transmit();
};