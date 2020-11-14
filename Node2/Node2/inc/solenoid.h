
#ifndef SOLENOID_H_
#define SOLENOID_H_
 #include "adc.h"
 #include "sam.h"

volatile uint8_t solenoid_counter; //Counter to regulate the pulse length

volatile uint8_t solenoid_free_flag; //0 when free, 1 when busy
 
// Initialize solenoid
void solenoid_init();

// Start the solenoid pulse
void solenoid_push();

// End the solenoid pulse
void solenoid_contract();


#endif /* SOLENOID_H_ */