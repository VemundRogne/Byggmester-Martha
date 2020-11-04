#ifndef REGULATOR_H_
#define REGULATOR_H_

#include <stdint.h>

int16_t position_ref;
int16_t integrated_error;


void regulator_set_ref(int16_t ref);




#endif /* REGULATOR_H_ */