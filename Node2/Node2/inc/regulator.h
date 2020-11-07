#ifndef REGULATOR_H_
#define REGULATOR_H_

#include <stdint.h>

/*
 * Specifies how hard each variable should be shifted to allow decimals
*/
#define DECIMATION_GAIN	2

// Regulator parameters

/* 
 * 0: Regulator is off
 * 1: Regulator is in P-mode
 * 2: Regulator is in PI-mode
*/
volatile uint8_t regulator_mode;

/* 
 * -1 or 1 sets if regulator is direct-action or reverse action
 * Should a positive error yield a positive output?
*/
volatile int8_t reverse_direct_action;

// Regulator variables
volatile uint16_t p_gain;
volatile uint16_t i_gain;

volatile int64_t position_reference;
int64_t position;

int64_t error;
int64_t integral;

int64_t regulator_output;

// Regulator functions:

// Helper function that calls the internal regulator functions
void regulator_run();

// Updates encoder position and error
void regulator_update_states();

// Steps the regulator
void regulator_calc_output();

// Set the output from the regulator to the motor
void regulator_set_output();



// Flag that can be set to enable/disable the regulator.
// 1 : Enabled
// 0 : Disabled
volatile uint8_t regulator_enabled;

void enable_regulator();
void disable_regulator();

void regulator_set_ref(int16_t ref);


#endif /* REGULATOR_H_ */