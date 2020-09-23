/*
 * timers.c
 *
 * Created: 23/09/2020 13:06:45
 *  Author: vemun
 */ 

#include "../inc/timers.h"
#include <avr/io.h>

void init_timer1(){
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