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

union signed_16_unsigned_8{
	int16_t signed_16;
	uint8_t unsigned_8[2];
} signed_16_unsigned_8;

union signed_32_unsigned_8{
	int32_t signed_32;
	uint8_t unsigned_8[4];
} signed_32_unsigned_8;




/* This is a helper function to transfer a signed 32-bit number to python

 * This function generates a can message with ID 1000
*/
void transfer_signed_32_to_python(int32_t value){
	signed_32_unsigned_8.signed_32 = value;

	struct can_message_t signed_32_unsigned_8_can_message;
	signed_32_unsigned_8_can_message.id = 1;
	signed_32_unsigned_8_can_message.data_length = 4;
	
	for(uint8_t i=0; i<4; i++){
		signed_32_unsigned_8_can_message.data[i] = signed_32_unsigned_8.unsigned_8[3-i];
	}

	can_send(&signed_32_unsigned_8_can_message, 0);
}

int16_t fit_to_interval(float val, int16_t min_val, int16_t max_val){
	float scale = (max_val-min_val)/(0xFF);

	return min_val + (int16_t)(val*scale);
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
		float _motor_ref = (float)(-data.i + (1<<7)); //Make positive :) 

		//Motor ref should be in interval (0, 8192)
		int16_t min_val = -2500;
		int16_t max_val = 9000;
		int16_t motor_ref = fit_to_interval(_motor_ref, min_val, max_val);
 		position_setpoint = motor_ref;


		// Get servo command and shift it from (-127, 127) to (0, 255)
		data.u = message->data[1];
		float _servo_ref = (float)(data.i + (1<<7));

		// Servo ref should be in interval (0, 255)
		min_val = 0;
		max_val = 255;
		uint8_t servo_ref = (uint8_t) fit_to_interval(_servo_ref, min_val, max_val);
		servo_set_position(servo_ref);
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
		//uint8_t pulse_length = message->data[1];
		if(solenoid_free_flag == 0){
			solenoid_push();
			solenoid_free_flag = 1;
		}
		
		//solenoid_push_ball(pulse_length);
	}

	/* ----------------- REGULATOR INPUTS ------------------------ */
	
	if (message->id == 138){
		regulator_init();
	}
	
	// SET REGULATOR MODE
	if(message->id == 900){
		regulator_mode = message->data[0];
	}

	// SET REGULATOR P-GAIN
	if(message->id == 901){
		signed_16_unsigned_8.unsigned_8[0] = message->data[1];
		signed_16_unsigned_8.unsigned_8[1] = message->data[0];
		p_gain = signed_16_unsigned_8.signed_16;
	}

	// SET REGULATOR I-GAIN
	if(message->id == 902){
		signed_16_unsigned_8.unsigned_8[0] = message->data[1];
		signed_16_unsigned_8.unsigned_8[1] = message->data[0];
		i_gain = signed_16_unsigned_8.signed_16;
	}

	// SET POSITION REFERENCE
	if(message->id == 903){
		signed_32_unsigned_8.unsigned_8[0] = message->data[3];
		signed_32_unsigned_8.unsigned_8[1] = message->data[2];
		signed_32_unsigned_8.unsigned_8[2] = message->data[1];
		signed_32_unsigned_8.unsigned_8[3] = message->data[0];
		position_setpoint = signed_32_unsigned_8.signed_32;

	}

	// Set regulator reverse_direct_action
	if(message->id == 904){
		reverse_dir_action = message->data[0];

	}


	/* ---------------- REGULATOR OUTPUTS --------------------------*/
	// Read position
	if(message->id == 952){
		transfer_signed_32_to_python(position);
	}

	// Read error
	if(message->id == 953){
		transfer_signed_32_to_python(error);
	}

	// Read integral
	if(message->id == 954){
		transfer_signed_32_to_python(integral);
	}

	// Read regulator_output
	if(message->id == 955){
		transfer_signed_32_to_python(regulator_output);
	}


	/* ----------------- DIRECT MOTOR CONTROL ---------------------- */
	/* Set output on motor */
	if(message->id == 1000){
		// Data format: [direction, power_MSB, power_LSB]
		uint8_t direction = message->data[0];		
		uint16_t power = (message->data[1] << 8) | (message->data[1]);
		motor_set_output(direction, power);
	}

	/* Request from Node 1 to Node 2 to transfer current encoder position */
	if(message->id == 1010){
		int16_t motorbox_encoder_value = 0;
		encoder_read(&motorbox_encoder_value);

		union signed_16_unsigned_8{
			int16_t signed_16;
			uint8_t unsigned_8[2];
		} data;

		data.signed_16 = motorbox_encoder_value;

		struct can_message_t encoder_msg;
		encoder_msg.id = 1010;
		encoder_msg.data[0] = data.unsigned_8[0];
		encoder_msg.data[1] = data.unsigned_8[1];
		encoder_msg.data_length = 2;

		can_send(&encoder_msg, 0);
	}

	/* Request from Node 1 to Node 2 to transfer current regulator output */
	/* Request from Node 1 to Node 2 to read current regulator integral */
	/* Request from Node 1 to Node 2 to Read current error */
	
	ir_transmit();
};

