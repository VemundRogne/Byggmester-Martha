/*
 * adc.c
 *
 * Created: 09/09/2020 09:58:11
 *  Author: vemun
 */ 

#define F_CPU	4915200

#include <avr/io.h>
#include <util/delay.h>

#include "../inc/adc.h"
#include "../inc/timers.h"
#include "../inc/xmem.h"


// Loads configuration of each channel according to figura 4a (page 11) in datasheet
void load_mux_config(){
	volatile char *ext_adc = (char *) ADC_START_ADDRESS; // Start address for the ADC
	
	uint8_t config_data;
	
	for(uint8_t i=0; i<4; i++){
		// This runs through to configurate each channel
		config_data = i;
		
		// Inhibits conversion while loading config.
		config_data |= (1<<ADC_INH);

		ext_adc[i] = config_data;
	}
}


// Sets up Timer1 to be a PWM output on PD5 (OC1A)
// This is the required clock that the ADC needs to operate
void adc_init(){
	timer1_init();
	load_mux_config();
}

// Updates the internally buffered adc_val register with new values from the ADC
void adc_rd(){
	volatile char *ext_adc = (char *) ADC_START_ADDRESS; // Start address for the ADC
	
	//Write start to start conversion
	ext_adc[0] = 0x00;
	
	// Wait for conversion to complete
	while( (ADC_BUSY_PIN & (1<<ADC_BUSY_BIT)) == 0){
		// WAIT
	}
	
	for(uint8_t i=0; i<4; i++){
		_adc_values[i] = _adc_values[i] - (_adc_values[i] >> FILTER_CONSTANT) + ext_adc[i];
	}
}

void adc_get_values(uint8_t *dest){
	for(uint8_t i=0; i<4; i++){
		dest[i] = _adc_values[i] >> FILTER_CONSTANT;
	}
}