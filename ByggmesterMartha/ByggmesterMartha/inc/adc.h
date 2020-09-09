/*
 * adc.h
 *
 * Created: 09/09/2020 09:57:51
 *  Author: vemun
 */ 


#ifndef ADC_H_
#define ADC_H_

#define ADC_A0		0x00
#define ADC_A1		0x01
#define ADC_A2		0x02
#define ADC_PD		0x03
#define ADC_INH		0X04
#define ADC_BIP		0x05
#define ADC_DIFF	0x06
#define ADC_ALL		0x07

// NOTE: The adc is dependent on TIMER1.

// Sets up Timer1 to be a PWM output on PD5 (OC1A)
void init_adc();

void load_mux_config();

void rd_adc();


#endif /* ADC_H_ */