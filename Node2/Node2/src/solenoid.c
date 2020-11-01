/*
 * solenoid.c
 *
 * Created: 01.11.2020 13:31:27
 *  Author: marth
 */ 


#include "../inc/solenoid.h"
 #include "sam.h"
#include <stdint.h>


void solenoid_init(){
	PIOA->PIO_OER |= PIO_OER_P15;
	PIOA->PIO_SODR |= PIO_ODR_P15;
}


void solenoid_push_ball(uint8_t pulse_length){
	PIOA->PIO_CODR |= PIO_CODR_P15;
	
}
