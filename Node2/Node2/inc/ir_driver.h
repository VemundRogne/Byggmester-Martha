
#ifndef IR_DRIVER_H_
#define IR_DRIVER_H_
#include "../inc/adc.h"
#include "../inc/can_controller.h"
#include <stdint.h>


//uint8_t prev_status_ball;
uint8_t status_ball;
volatile uint8_t transmit_ball_status_flag;

// IR init
void ir_init();

// Detects if ball is in IR beam
void ir_ball_in_beam();

// Transmits IR message over can bus to Node1
uint8_t ir_transmit();


#endif /* IR_DRIVER_H_ */