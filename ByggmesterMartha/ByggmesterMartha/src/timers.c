/**
* @file
* @brief Implementation of timers
*/

#include "../inc/timers.h"
#include <avr/io.h>
#include "../inc/adc.h"
#include <avr/interrupt.h>

/**
* @brief Interrupt service triggered by timer 0. Read and filter ADC values by adc_rd()
*
* @return void
*/

ISR(TIMER0_OVF_vect){
	adc_rd();
}

// Configures timer0 to be a system-wide interrupt generator at ~70 Hz
void timer0_init(){
	// Waveform generation defaults to "Normal"
	// We configure no direct outputs
	
	// Clock select: prescale F_CPU by 256.
	// This results in a frequency of (F_CPU / prescaler) / 2**8 = 70 Hz
	TCCR0 |= (1<<CS02);
	
	// Enable Timer0 Overflow interrupt
	TIMSK |= (1<<TOIE0);
}

// Configures timer1 and OC1A as an ADC clock
void timer1_init(){
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