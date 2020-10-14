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

int can_valid_transmit_buffer();
int can_pending_receive_buffer();
uint8_t can_transmit_message(struct can_msg msg);
uint8_t can_receive_message(struct can_msg *msg);


#endif /* CAN_H_ */