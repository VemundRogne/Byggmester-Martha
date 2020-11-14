#include "../inc/motor.h"
#include "../inc/delay.h"

#include <sam.h>
#include <stdint.h>
#include <component/dacc.h>

// ------------------ MOTOR -----------------------

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

void motor_init(){
	REG_PIOD_OER |= (1 << DIR) | (1 << EN) | (1 << SEL) | (1 << RST) | (1 << OE); //Enable output for our desired control pins for MJ1
	motor_enable_output();
	
	PMC->PMC_PCER1 |= PMC_PCDR1_PID38; 

	DACC->DACC_MR |= DACC_MR_TAG; //Tag selection cus studass said so
	DACC->DACC_CHER |= (1<<1); //Enable channel 1
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

	DACC->DACC_CHER |= (1<<1); //Enable channel 1
	DACC->DACC_CDR |= (val | (1<<12)); //Channel data register gets tag-bit + digital value to convert
};


// ------------------ ENCODER -----------------------

/* Resets all out internal counters and stuff */
void encoder_init(){
	motor_encoder_value = 0;
	motorbox_reset_encoder();

	PMC->PMC_PCER0 |= (1<<13);	// Enables clock on PIOC
	REG_PIOD_SODR |= (1<<RST);
}


/* Reset the encoder counter in the motorbox 
 * Pull the active low RST pin down, wait for a short time, then set it high again
*/
void motorbox_reset_encoder(){
	REG_PIOD_CODR |= (1<<RST);
	_delay_ms(1);
	REG_PIOD_SODR |= (1<<RST);
}


/* Reads the encoder value from the motorbox 
 *
 * Normal procedure of reading the encoder (according to motorbox datasheet):
 *  1. Set !OE low to enable output of encoder
 *  2. Set SEL low to get high byte
 *  3. Wait about 20 microseconds
 *  4. Read MSB
 *  5. Set SEL high to get low byte
 *  6. wait about 20 microseconds
 *  7. Read LSB
 *  8. Toggle !RST to reset encoder
 *  9. Set !OE high to disable output of encoder
*/
void encoder_read(int16_t *motorbox_encoder_value){
	// 1. Set !OE low to enable output of encoder
	REG_PIOD_CODR |= (1<<OE);

	// 2. Set SEL low to get high byte
	REG_PIOD_CODR |= (1<<SEL);

	// 3. Wait about 20 microseconds
	_delay_ms(5);

	// 4. Read MSB
	uint8_t MSB = 0;
	_encoder_byte_read(&MSB);

	// 5. Set SEL high to get low byte
	REG_PIOD_SODR |= (1<<SEL);

	// 6. Wait about 20 microseconds
	_delay_ms(5);

	// 7. Read LSB
	uint8_t LSB = 0;
	_encoder_byte_read(&LSB);


	// 9. SET !OE high to disable output of encoder
	REG_PIOD_SODR |= (1<<OE);

	union signed_16_unsigned_8{
		int16_t signed_16;
		uint8_t unsigned_8[2];
	} data;

	data.unsigned_8[1] = MSB;
	data.unsigned_8[0] = LSB;

	*motorbox_encoder_value = data.signed_16;
}

/*
 * From the ATSAM3X8E datasheet: 
 *   The level on each I/O line can be read through PIO_PDSR (Pin Data Status Register). This register indicates the
 *   level of the I/O lines regardless of their configuration, whether uniquely as an input or driven by the PIO controller
 *   or driven by a peripheral.
 *   Reading the I/O line levels requires the clock of the PIO controller to be enabled, otherwise PIO_PDSR reads the
 *   levels present on the I/O line at the time the clock was disabled.

 *  This means we can get the value from reading PIO_PDSR and masking and shifting correctly
*/
void _encoder_byte_read(uint8_t *_data){
	uint32_t pin_data_status_register = REG_PIOC_PDSR;
	*_data = (uint8_t)(pin_data_status_register >> 1);
}