#ifndef REGULATOR_H_
#define REGULATOR_H_

uint8_t position_ref;

void regulator_timer();

void set_position_ref(uint8_t position);



#endif /* REGULATOR_H_ */