//regulator.c

#include "../inc/regulator.h"
#include "../inc/motor.h"
#include "sam3x4c.h"
#include <stdint.h>
#include <stdlib.h>

#define Kp 3
#define Ki 2

#define INIT_SPEED 1000

void regulator_init(){
	init_flag = 1; // Stop regulator while initializing

	//Initialize sequence
	uint16_t power = INIT_SPEED;
	motor_set_output(1, power); //Drive to the right
	_delay_s(3);
	motorbox_reset_encoder();

	//Not sure if we need all these, but shouldnt hurt
	regulator_mode = 2;
	reverse_dir_action = 1;
	p_gain = Kp;
	i_gain = Ki;
	position_reference = 0; 
	position = 0;
	error = 0;
	integral = 0;
	regulator_output = 0;

	init_flag = 0;
}


void regulator_run(){
	regulator_update_states();

	if (regulator_mode){
		regulator_calc_output();
		regulator_set_output();
	}
}

void regulator_update_states(){
	position = (int32_t)motor_encoder_value;
	error = (int32_t)position - position_reference;
	if (regulator_mode == 2){
		integral += error; //ignore time interval here to make smooth, make sure you include T_i when calculating output
	}
	else{
		integral = 0;
	}
}

void regulator_calc_output(){
	regulator_output = reverse_dir_action*(p_gain*error + (i_gain*integral)>>6);
}

void regulator_set_output(){
	uint8_t dir = 0; //Positive direction
	if (regulator_output < 0){
		dir = 1; //Negative direction;
	}

	uint16_t power = (uint16_t)abs(regulator_output) >> DECIMATION_GAIN; //Compensate for potential decimations in gains
	motor_set_output(dir, power);
};


void TC0_Handler(){
	uint32_t dummy = REG_TC0_SR0; //Clear interrupt flag to avoid continously call to TCO_Handler()
	if (!init_flag){
		regulator_run();
	}
}