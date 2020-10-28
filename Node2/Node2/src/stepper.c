#include "../inc/stepper.h"



// MJ1 output from shield
// DIR 		-> PIN32 	-> PD10
// EN 		-> PIN30 	-> PD9
// SEL 		-> PIN27 	-> PD2
// NOT_RST	-> PIN26 	-> PD1
// NOT_OE	-> PIN25	-> PD0
#define DIR 10
#define EN 9
#define SEL 2
#define RST 1
#define OE 0

//Set direction based on negative or positiv argument (int8_t dir)
void stepper_set_direction(int8_t dir){
	if (dir > 0){
		REG_PIOD_SODR |= (1<<DIR);
	}
	else{
		REG_PIOD_CODR |= (1<<DIR);
	}
}

//Enable and disable motor
void stepper_enable_motor(){
	REG_PIOD_SODR |= (1<<EN);
}
void stepper_disable_motor(){
	REG_PIOD_CODR |= (1<<EN);
}

// Select low or high byte for 8-bit encoder output on MJ2. (The encoder is 16bit) 
void stepper_select_low_encoder_byte(){
	REG_PIOD_CODR |= (1<<SEL);
}
void stepper_select_high_encoder_byte(){
	REG_PIOD_SODR |= (1<<SEL);
}

// Set/Reset encoder (counter) by writing to MJ1
void stepper_set_encoder(){
	REG_PIOD_SODR |= (1<<RST);
}
void stepper_reset_encoder(){
	REG_PIOD_CODR |= (1<<RST);
}

//Enable output of encoder to be read in MJ2
void stepper_enable_output(){
	REG_PIOD_CODR |= (1<<OE);
}


void stepper_init(){
	REG_PIOD_OER |= (1 << DIR) | (1 << EN) | (1 << SEL) | (1 << RST) | (1 << OE); //Enable output for our desired control pins for MJ1
}


void stepper_joystick_command(int8_t stepper_speed){
	stepper_enable_motor();
	stepper_set_direction(stepper_speed);

	uint16_t scaled_speed

	/////////////Send analog val (0-5V)//////////////// 
	// We will use DAC1 output pin on the shield (found on P3)
	// This can be read from B16?
	// DAC is 12bit

	// DACC_MR (dac mode register)
	// DACC_CHER (channel enable register)
	// DACC_CDR (conversion data register) <- I assume we write our data here, but our int8_t must probably be converted to 12bit 

};

/*
void servo_joystick_command(int8_t position){
	servo_set_position((uint8_t)(position + 128));
};*/