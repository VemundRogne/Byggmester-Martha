/*
 * adc.c
 *
 * Created: 21.10.2020 14:52:23
 *  Author: marth
 */ 

#include "sam.h"

void adc_init(){
	PMC->PMC_PCER1 |= PMC_PCER1_PID37;
	//PMC->PMC_MCKR = PMC_MCKR_CSS_MAIN_CLK;
	
	//Channel enable
	// val: ADC_CHER_CH0
	// register: 
	// Pin AD0
	ADC->ADC_CHER |= ADC_CHER_CH0; //Pop
	
	ADC->ADC_CR |= ADC_CR_START; //NTNU
	ADC->ADC_MR |= ADC_MR_FREERUN; //Nike
	ADC->ADC_MR |= ADC_MR_PRESCAL(100);
	
}

uint16_t adc_read(){
	return (uint16_t)(ADC->ADC_CDR[0]);	
}