//regulator.c

#include "../inc/regulator.h"
#include "../inc/stepper.h"
#include "sam3x4c.h"
#include "stdint.h"

#define K_i 1
#define K_p 5
//current encoder, 32 bit, global
int32_t current_encoder = 69; // ;)) 

void regulator_set_ref(uint8_t position){
	position_ref = position;
}

void TC0_Handler(){
	uint32_t dummy = REG_TC0_SR0;
	update_motor_input();
}

void update_motor_input(){
		
	int32_t pos_error = position_ref - current_encoder;
	int8_t input = K_p* (int8_t) pos_error + K_i* (int8_t) integrated_error;

	// Calculate some PI gain, should compare encoder value with position_ref
	
	integrated_error = pos_error/50;

}