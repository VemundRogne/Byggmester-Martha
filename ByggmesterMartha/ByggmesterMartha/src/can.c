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

// Looks for valid transmit buffer, if non returns 3
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

uint8_t can_pending_receive_buffer(uint8_t *rx_buffer_address){
	uint8_t status_reg = mcp2515_READ_STATUS();
	if  ((status_reg & (1 << 0)) == 0){
		*rx_buffer_address = MCP_RXB0CTRL;
		return 0;
	}
	else if ((status_reg & ( 1 << 1)) == 0){
		*rx_buffer_address = MCP_RXB1CTRL;
		return 1;
	}
	return 3;
}

/*
 * Loads a transmit buffer with ID, length and data
*/
uint8_t can_transmit_message(struct can_msg *msg){
	// Write ID
	// Write length
	// Write data
	// Request to send TX buffer
	
	// The length CAN NOT be greater than four bits.
	if (msg->len > 0x0F){
		return 1;
	}
	
	// This buffer is used to get info from the *msg and store it in a format that the MCP likes
	uint8_t buffer[5+msg->len];
	
	// LOAD ID into buffer:
	// TXBnSIDH - Transmit Buffer n Standard identifier HIGH contains SID<10:3>
	buffer[0] = (uint8_t)(msg->ID >> 3);
	// TXBnSIDL - Transmit Buffer n Standard identifier LOW contains SID<2:0>
	buffer[1] = (uint8_t)(msg->ID << 5);
	// Since we do not support extended ID's we leave the remaining bits as 0

	// NOTE: Here we skip two buffers, as they are the extended IDs
	buffer[2] = 0;
	buffer[3] = 0;

	// LOAD LENGTH into buffer:
	buffer[4] = (uint8_t)(msg->len);

	// LOAD DATA
	for(uint8_t i = 0; i<msg->len; i++){
		buffer[5+i] = msg->data[i];
	}

	uint8_t tx_buffer_address; 

	if (can_valid_transmit_buffer(&tx_buffer_address) != 3){
		mcp2515_WRITE(tx_buffer_address + 1, &buffer[0], 5+msg->len);
		return 0;
	}
	return 1;
}

uint8_t can_receive_message(struct can_msg *msg){
	uint8_t buffer[13];
	
	uint8_t rx_buffer_address;
	if (can_pending_receive_buffer(&rx_buffer_address) != 3){
		mcp2515_READ(rx_buffer_address + 1, &buffer[0], 13);
		mcp2515_BIT_MODIFY(0x2C, 3, 0);
		msg->ID = buffer[0] << 8;
		msg->ID = (msg->ID | buffer[1]);
		msg->len = buffer[4] & 0x0f;
		msg->data[0] = 255;
		msg->data[1] = 254;
		msg->data[2] = 253;
		msg->data[3] = 242;
		//memcpy(&(msg->data[0]), &buffer[5], msg->len);
		return 0;
	}
	return 1;	
}