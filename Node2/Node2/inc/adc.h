/*
 * adc.h
 *
 * Created: 21.10.2020 14:52:11
 *  Author: marth
 */ 


#ifndef ADC_H_
#define ADC_H_

#include <stdint.h>

//ADC init
void adc_init();

//ADC read
uint16_t adc_read();

#endif /* ADC_H_ */