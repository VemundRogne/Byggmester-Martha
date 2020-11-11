/*
 * solenoid.c
 *
 * Created: 01.11.2020 13:31:27
 *  Author: marth
 */ 


#include "../inc/solenoid.h"
 #include <sam.h>
#include <stdint.h>


void solenoid_init(){
	REG_PIOA_OER |= (1<<15);
	REG_PIOA_SODR |= (1<<15);
	solenoid_counter = 0;
	solenoid_free_flag = 0; 
}


void solenoid_push(){
	REG_PIOA_CODR |= (1<<15);
}

void solenoid_contract(){
	REG_PIOA_SODR |= (1<<15);
}