#ifndef REGULATOR_H_
#define REGULATOR_H_

#include <stdint.h>

int16_t position_ref;
int16_t integrated_error;

// Flag that can be set to enable/disable the regulator.
// 1 : Enabled
// 0 : Disabled
volatile uint8_t regulator_enabled;

void enable_regulator();
void disable_regulator();

void regulator_set_ref(int16_t ref);


#endif /* REGULATOR_H_ */