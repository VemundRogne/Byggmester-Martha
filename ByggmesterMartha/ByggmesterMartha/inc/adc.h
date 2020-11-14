/**
* @file
* @brief Library for reading values through ADC
*/

#ifndef ADC_H_
#define ADC_H_

#include <stdio.h>

// See: https://www.mikrocontroller.net/attachment/139580/schift-filter-tp.pdf
// for info about the filter
volatile uint16_t _adc_values[4];
#define FILTER_CONSTANT 4

// Configuration registers
#define ADC_A0		0x00
#define ADC_A1		0x01
#define ADC_A2		0x02
#define ADC_PD		0x03
#define ADC_INH		0X04
#define ADC_BIP		0x05
#define ADC_DIFF	0x06
#define ADC_ALL		0x07

// PIN configuration
#define ADC_BUSY_BIT	PE0	
#define ADC_BUSY_PIN	PINE
#define ADC_BUSY_PORT	PORTE
#define ADC_BUSY_DDR	DDRE




/**
* @brief Sets up Timer1 to be a PWM output on PD5 (OC1A)
*
* @return void
*/

void adc_init();

/**
* @brief Read and store adc values internally
*
* @return void
*/
void adc_rd();

/**
* @brief Read filtered adc-values from memory
*
* @param[out] dest Pointer to list of ADC values
*
* @return void
*/
void adc_get_values(uint8_t *dest);


#endif /* ADC_H_ */