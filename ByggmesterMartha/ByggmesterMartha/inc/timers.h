/**
* @file
* @brief Library containing timers necessary for the ADC
*/

#ifndef TIMERS_H_
#define TIMERS_H_

/**
* @brief Timer 1 is used for PWM output on PD5. Works as a clock for the ADC
*
* @return void
*/
void timer1_init();

/**
* @brief Timer 0 is used to read and filter adc continously
*
* @return void
*/
void timer0_init();


#endif /* TIMERS_H_ */