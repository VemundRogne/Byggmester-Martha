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

// Looks for valid transmit buffer, if non returns -1
// If valid transmit buffer found tx_buffer_address points to
// the valid register. 
uint8_t can_valid_transmit_buffer(uint8_t *tx_buffer_address){
	uint8_t status_reg = mcp2515_READ_STATUS();
	if  ((status_reg &= (1 << 2)) == 0){
		*tx_buffer_address = MCP_TXB0CTRL;
		return 0;
	}
	if ((status_reg &= ( 1 << 4)) == 0){
		*tx_buffer_address = MCP_TXB1CTRL;
		return 1;
	}
	if ((status_reg &= ( 1 << 6)) == 0){
		*tx_buffer_address = MCP_TXB2CTRL;	
		return 2;
	}
	return 3;
	
}

int8_t can_pending_receive_buffer(uint8_t *rx_buffer_address){
	uint8_t status_reg = mcp2515_READ_STATUS();
	if  ((status_reg & (1 << 0)) == 0){
		*rx_buffer_address = MCP_RXB0CTRL;
		return 0;
	}
	if ((status_reg & ( 1 << 1)) == 0){
		*rx_buffer_address = MCP_RXB1CTRL;
		return 1;
	}
	return -1;
}

uint8_t can_transmit_message(struct can_msg msg){
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
	
	uint8_t tx_buffer_address; 
	
	if (can_valid_transmit_buffer(&tx_buffer_address) != 3){
		mcp2515_WRITE(tx_buffer_address + 1, &buffer[0], 5+msg.len);
		return 0;
	}
	return 1;		
}

uint8_t can_receive_message(struct can_msg *msg){
	uint8_t buffer[13];
	
	uint8_t rx_buffer_address;
	if (can_pending_receive_buffer(&rx_buffer_address) != -1){
		mcp2515_READ(rx_buffer_address + 1, &buffer[0], 13);
		msg->ID = buffer[0] << 8;
		msg->ID = (msg->ID | buffer[1]);
		msg->len = buffer[4];
		memcpy(&(msg->data[0]), &buffer[5], msg->len);
		return 0;
	}
	return 1;	
}