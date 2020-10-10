/*
 * can.h
 *
 * Created: 04/10/2020 13:06:56
 *  Author: vemun
 */ 


#ifndef CAN_H_
#define CAN_H_

#include <avr/io.h>
#include "../inc/mcp2515.h"

struct can_msg{
	uint16_t ID;
	uint8_t len;
	uint8_t data[8];
};

void can_init();

void can_transmit_message(struct can_msg msg);
void can_receive_message(struct can_msg *msg);


#endif /* CAN_H_ */