/*
 * can.c
 *
 * Created: 04/10/2020 13:07:08
 *  Author: vemun
 */ 

#define F_CPU	4915200
#include <avr/io.h>
#include <util/delay.h>
#include <string.h>

#include "../inc/mcp2515.h"
#include "../inc/can.h"

void can_init(){
	mcp2515_init(LOOPBACK);
}

void can_transmit_message(struct can_msg msg){
	// Write ID
	// Write length
	// Write data
	// Request to send TX buffer
	uint8_t buffer[5+msg.len];
	
	buffer[0] = (uint8_t)(msg.ID >> 8);
	buffer[1] = (uint8_t)(msg.ID);
	buffer[2] = 0;	// extended ID
	buffer[3] = 0;	// extended ID
	buffer[4] = (uint8_t)(msg.len);
	memcpy(&buffer[5], &msg.data[0], msg.len);
	
	mcp2515_WRITE(MCP_TXB0CTRL + 1, &buffer[0], 5+msg.len);
}

void can_receive_message(struct can_msg *msg){
	uint8_t buffer[13];
	
	mcp2515_READ(MCP_RXB0CTRL + 1, &buffer[0], 13);
	
	msg->ID = buffer[0] << 8;
	msg->ID = (msg->ID | buffer[1]);
	msg->len = buffer[4];
	memcpy(&(msg->data[0]), &buffer[5], msg->len);
}