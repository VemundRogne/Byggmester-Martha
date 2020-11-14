
#ifndef TIMERS_H_
#define TIMERS_H_


// Timer 1 is used for PWM output on PD5. It's the ADC clock
void timer1_init();

// Timer 0 is used to read and filter adc continously
void timer0_init();


#endif /* TIMERS_H_ */