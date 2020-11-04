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

	return min_val + (uint16_t)(val*scale);
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
		
		// Get motor command and shift it from (-127, 127) to (0, 255)
		data.u = message->data[0];
		float motor_ref = (float)(data.i + (1<<7)); //Make positive :) 

		//Motor ref should be in interval (0, 8192)
		uint16_t min_val = 0;
		uint16_t max_val = 1<<13;
		int16_t motor_ref = fit_to_interval(motor_ref, min_val, max_val);
 		regulator_set_ref(motor_ref);


		// Get servo command and shift it from (-127, 127) to (0, 255)
		data.u = message->data[1];
		float servo_ref = (float)(data.i + (1<<7));

		// Servo ref should be in interval (0, 255)
		uint16_t min_val = 0;
		uint16_t max_val = 255;
		uint8_t servo_ref = (uint8_t) fit_to_interval(servo_ref, min_val, max_val);
		servo_set_position(motor_ref);
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