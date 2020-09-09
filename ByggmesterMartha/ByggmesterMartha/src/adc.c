/*
 * adc.c
 *
 * Created: 09/09/2020 09:58:11
 *  Author: vemun
 */ 

#include "../inc/adc.h"
#include <avr/io.h>

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