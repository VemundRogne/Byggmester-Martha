#include "../inc/motor.h"

#include <sam.h>
#include <stdint.h>
#include <component/dacc.h>

//Set direction based on negative or positiv argument (int8_t dir)
void motor_set_direction(int8_t dir){
	if (dir > 0){
		REG_PIOD_SODR |= (1<<DIR);
	}
	else{
		REG_PIOD_CODR |= (1<<DIR);
	}
}

//Enable and disable motor
void motor_enable_output(){
	REG_PIOD_SODR |= (1<<EN);
}
void motor_disable_output(){
	REG_PIOD_CODR |= (1<<EN);
}

// Select low or high byte for 8-bit encoder output on MJ2. (The encoder is 16bit) 
void motor_select_low_encoder_byte(){
	REG_PIOD_CODR |= (1<<SEL);
}
void motor_select_high_encoder_byte(){
	REG_PIOD_SODR |= (1<<SEL);
}

// Set/Reset encoder (counter) by writing to MJ1
void motor_set_encoder(){
	REG_PIOD_SODR |= (1<<RST);
}
void motor_reset_encoder(){
	REG_PIOD_CODR |= (1<<RST);
}

//Enable output of encoder to be read in MJ2
//void motor_enable_output(){
//	REG_PIOD_CODR |= (1<<OE);
//}


void motor_init(){
	REG_PIOD_OER |= (1 << DIR) | (1 << EN) | (1 << SEL) | (1 << RST) | (1 << OE); //Enable output for our desired control pins for MJ1
	motor_enable_output();

	//DACC stuff
	//REG_PIOB_OER |= (1<<DACC_PIO);

	//PIOB->PIO_ABSR |= PIO_ABSR_P25; Dont think we need this cus dac no peripheral?
	
	PMC->PMC_PCER1 |= PMC_PCDR1_PID38; 

	DACC->DACC_MR |= DACC_MR_TAG; //Tag selection cus studass said so
	DACC->DACC_CHER |= (1<<1); //Enable channel 1
	
	//DACC->DACC_CDR = 1000 | (1<<12);
}

/*
 * Sets output on the motor

 * Currently does no sanity-checks on the power
*/
void motor_set_output(uint8_t direction, uint16_t power){
	motor_enable_output();

	// This is just a hack to make it faster to test this command over CAN
	if(direction == 0){
		motor_set_direction(1);
	}
	else{
		motor_set_direction(-1);
	}

	DACC->DACC_CHER |= (1<<1);
	DACC->DACC_CDR |= (power | (1<<12));	// TODO: Check if this method of setting power is dangerous
}


// Vi vil gi DACCen en verdi mellom 0 og 2048 (alts� en 12-bit verdi)
void motor_joystick_command(int8_t motor_speed){
	motor_enable_motor();
	motor_set_direction(motor_speed);

	uint16_t val = abs(motor_speed);
	val = val << 4;

	//DACC->DACC_MR |= DACC_MR_TAG; //Tag selection cus studass said so
	//uint16_t tag_channel_enabler = (1<<13); //Tag channel 1

	DACC->DACC_CHER |= (1<<1); //Enable channel 1
	DACC->DACC_CDR |= (val | (1<<12)); //Channel data register gets tag-bit + digital value to convert
};

/*
void servo_joystick_command(int8_t position){
	servo_set_position((uint8_t)(position + 128));
};*/