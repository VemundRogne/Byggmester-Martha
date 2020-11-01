/*
 * solenoid.c
 *
 * Created: 01.11.2020 13:31:27
 *  Author: marth
 */ 


#include "../inc/solenoid.h"
 #include <sam.h>
#include <stdint.h>
#include "../inc/delay.h"


void solenoid_init(){
	REG_PIOA_OER |= (1<<15);
	REG_PIOA_SODR |= (1<<15);
}


void solenoid_push_ball(uint8_t pulse_length){
	REG_PIOA_CODR |= (1<<15);
	_delay_ms(100);
	REG_PIOA_SODR |= (1<<15);
}
