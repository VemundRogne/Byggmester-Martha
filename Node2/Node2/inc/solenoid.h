
#ifndef SOLENOID_H_
#define SOLENOID_H_
 #include "adc.h"
 #include "sam.h"
 
 
volatile uint8_t solenoid_counter;
 // 0 when free, 1 when busy
volatile uint8_t solenoid_free_flag;
 
 void solenoid_init();

void solenoid_push();

void solenoid_contract();


#endif /* SOLENOID_H_ */