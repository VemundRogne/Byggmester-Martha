/*
 * timers.c
 *
 * Created: 23/09/2020 13:06:45
 *  Author: Vemund
 */ 

#include "../inc/timers.h"
#include <avr/io.h>

// This ISR is called approximately 18.75 times per second, as per init_timer0();
ISR(TIMER0_OVF_vect){
}

// Configures timer0 to be a system-wide interrupt generator at ~18.75 Hz
void init_timer0(){
	// Waveform generation defaults to "Normal"
	// We configure no direct outputs
	
	// Clock select: prescale F_CPU by 1024.
	// This results in a frequency of (F_CPU / prescaler) / 2**8 = 18.75 Hz
	TCCR0 |= (1<<CS02) | (1<<CS00);
	
	// Enable Timer0 Overflow interrupt
	TIMSK |= (1<<TOIE0);
}

// Configures timer1 and OC1A as an ADC clock
void init_timer1(){
	// Sets Waveform Generation Mode to mode 5: Fast PWM, 8-bit
	TCCR1A |= (1<<WGM10);
	TCCR1B |= (1<<WGM12);

	// Set the OC1A pin as output. This is the physical pin that is used as a clock for the ADC
	DDRD |= (1<<PD5);

	// Sets output compare to 128
	OCR1AL = 128;

	// Sets up OC1A to "Clear OC1A on Compare Match, set OC1A at TOP"
	TCCR1A |= (1<< COM1A1);

	// Clock select: No prescaling
	TCCR1B |= (1<<CS10);
}