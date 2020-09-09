/*
 * adc.h
 *
 * Created: 09/09/2020 09:57:51
 *  Author: vemun
 */ 


#ifndef ADC_H_
#define ADC_H_

// NOTE: The adc is dependent on TIMER1.

// Sets up Timer1 to be a PWM output on PD5 (OC1A)
void init_adc();


#endif /* ADC_H_ */