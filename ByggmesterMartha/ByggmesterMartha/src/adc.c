/*
 * adc.c
 *
 * Created: 09/09/2020 09:58:11
 *  Author: vemun
 */ 

#include "../inc/adc.h"
#include <avr/io.h>
#include "../inc/xmem.h"

#define F_CPU	4915200
#include <util/delay.h>

// Sets up Timer1 to be a PWM output on PD5 (OC1A)
void init_adc(){
	// Sets Waveform Generation Mode to mode 5: Fast PWM, 8-bit
	TCCR1A |= (1<<WGM10);
	TCCR1B |= (1<<WGM12);
	
	// Set the OC1A pin as output
	DDRD |= (1<<PD5);
	
	// Sets output compare
	OCR1AL = 128;
	
	// Sets up OC1A to "Clear OC1A on Compare Match, set OC1A at TOP"
	TCCR1A |= (1<< COM1A1);
	
	// Clock select: No prescaling
	TCCR1B |= (1<<CS10);
}

// Loads configuration of each channel according to figura 4a (page 11) in datasheet
void load_mux_config(){
	volatile char *ext_adc = (char *) ADC_START_ADDRESS; // Start address for the ADC
	
	printf("Loading mux config \n");
	
	uint8_t config_data;
	
	for(uint8_t i=0; i<4; i++){
		// This runs through to configurate each channel
		config_data = i;
		
		// Inhibits conversion while loading config.
		config_data |= (1<<ADC_INH);
		
		// For debugging:
		printf("  channel %u: %u\n", i, config_data);
		ext_adc[i] = config_data;
	}
}

void rd_adc(){
	volatile char *ext_adc = (char *) ADC_START_ADDRESS; // Start address for the ADC
	
	uint8_t adc_val[8];
	
	printf("Reading values from ADC\n");
	
	//Write start to start conversion
	ext_adc[0] = 0x00;
	_delay_ms(10);
	
	for(uint8_t i=0; i<4; i++){
		adc_val[i] = ext_adc[i];
		printf("  channel %u: %u\n", i, adc_val[i]);
	}
}