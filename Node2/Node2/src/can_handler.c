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
#include "../inc/regulator.h"
#include <component/tc.h>
#include "../inc/joystick.h"
#include "../inc/ir_driver.h"
#include "../inc/solenoid.h"

union Data {
	uint8_t u;
	int8_t i;
};

uint16_t fit_to_interval(float val, uint16_t min_val, uint16_t max_val){
	float scale = (max_val-min_val)/(0xFF);

	return = min_val + (uint16_t)(val*scale)
}


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
		uint8_t value = (uint8_t)(data.i + (1<<7)); //Make positive :) 

		//Ref should be in interval 8192 er 1<<13
		uint16_t max_val = 1<<13;
		uint16_t min_val = 0;

		int16_t ref = fit_to_interval((float) value, min_val, max_val);
 		regulator_set_ref(ref);
		
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


	/* Set output on motor */
	if(message->id == 1000){
		// Data format: [direction, power_MSB, power_LSB]
		uint8_t direction = message->data[0];		
		uint16_t power = (message->data[1] << 8) | (message->data[1]);
		motor_set_output(direction, power);
	}
	
	ir_transmit();
};