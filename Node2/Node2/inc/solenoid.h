/*
 * solenoid.h
 *
 * Created: 01.11.2020 13:31:42
 *  Author: marth
 */ 


#ifndef SOLENOID_H_
#define SOLENOID_H_
 #include "adc.h"
 #include "sam.h"
 
 
 void solenoid_init();

void solenoid_push_ball(uint8_t pulse_length);

#endif /* SOLENOID_H_ */