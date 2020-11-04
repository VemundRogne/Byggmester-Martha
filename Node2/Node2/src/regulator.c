//regulator.c

#include "../inc/regulator.h"
#include "../inc/motor.h"
#include "sam3x4c.h"
#include <stdint.h>
#include <stdlib.h>

#define K_i 1
#define K_p 12
//current encoder, 32 bit, global

void regulator_set_ref(int16_t ref){
	position_ref = ref;
}

void update_motor_input(int16_t current_encoder){
		
	int16_t pos_error = position_ref - current_encoder;
	int16_t input = K_p*pos_error + K_i*integrated_error;

	uint8_t dir = 0;
	if (input > 0){
		dir = 1;
	}
	
	uint16_t power = abs(input);

	//Check for saturation to avoid integrator wind-up
	if (!(power >> 12)){
		integrated_error += pos_error/50; //Dette tull?
	}

	//power = power >> 4; //Ikke endre denne, da kan du heller tøyse med Kp og Ki
	
	motor_set_output(dir, power);

}

void TC0_Handler(){
	uint32_t dummy = REG_TC0_SR0;
	int16_t current_encoder_position = 0;
	encoder_read(&current_encoder_position);

	update_motor_input(current_encoder_position);
}