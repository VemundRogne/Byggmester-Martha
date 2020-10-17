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
	// Enable buffer 0 to receive all messages
	// This turns off the mask and fileter -> receive any message
	mcp2515_BIT_MODIFY(MCP_RXB0CTRL, 0b01100000, 0b01100000);
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
	if ((status_reg &= (1 << 0)) == (1<<0)){
		*rx_buffer_address = MCP_RXB0CTRL;
		return 0;
	}
	else if ((status_reg &= ( 1 << 1)) == (1<<1)){
		*rx_buffer_address = MCP_RXB1CTRL;
		return 1;
	}
	return 3;
}

/*
 * Loads a transmit buffer with ID, length and data
 * Returns 0 on success, 1 on failiure
*/
uint8_t can_transmit_message(struct can_msg *msg){
	// Write ID
	// Write length
	// Write data
	// Request to send TX buffer
	
	// The length CAN NOT be greater than four bits.
	if (msg->len > 8){
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
	uint8_t tx_buffer_id = can_valid_transmit_buffer(&tx_buffer_address);

	if (tx_buffer_id != 3){
		mcp2515_WRITE(tx_buffer_address + 1, &buffer[0], 5+msg->len);
		mcp2515_RTS(1<<tx_buffer_id);
		return 0;
	}
	return 1;
}

/*
 * This function is used to clear the CANINTF.RXnIF flag
 * 
 * When the MCP receives some data into a buffer the flag is set, the MCU must clear
 * the bit to allow new data to get into the buffer.
*/
void clear_receive_flag(uint8_t receive_buffer_id){
	// CANINTF register:
	// Address: 0x2C
	
	// |bit7 |bit6 |bit5 |bit4 |bit3 |bit2 |bit1 |bit0 |
	// |MERRF|WAKIF|ERRIF|TX2IF|TX1IF|TX0IF|RX1IF|RX0IF|
	
	// This register can be modified using the BIT_MODIFY function
	// The simplest way to write one of these bits to 0 is to keep the value_byte = 0, and move the mask
	
	// We want to clear bit0 -> mask = 0b0000 0001, value = 0
	// We want to clear bit1 -> mask = 0b0000 0010, value = 0
	// This can be accomplished with a bitshift on the mask register.
	
	mcp2515_BIT_MODIFY(MCP_CANINTF, (1<<receive_buffer_id), 0);
}

uint8_t can_receive_message(struct can_msg *msg){
	uint8_t buffer[13];

	// Look for a buffer that has some data
	uint8_t receive_buffer_id;	// This is the ID of the buffer (0, 1), a 3 indicates no buffer has data
	uint8_t rx_buffer_address;	// This is the MPC_ADDRESS of the buffer
	receive_buffer_id = can_pending_receive_buffer(&rx_buffer_address);

	// If we actually have some data:
	if (receive_buffer_id != 3){
		// Read out the data
		mcp2515_READ(rx_buffer_address + 1, &buffer[0], 13);

		// Clear the receive flag, allowing new data to be received.		
		clear_receive_flag(receive_buffer_id);
		
		// ID reception:
		// RXBnSIDH contains SID<10:3>		HHHH HHHH
		// RXBnSIDL contains SID<2:0>		LLL0 0000
		// We must shift into:				0000 0HHH HHHH HLLL
		msg->ID = buffer[0] << 3;		// Shifting the H bits
		msg->ID |= buffer[1] >> 5;		// Shifting the L bits

		// The length is in the format      XXXX nnnn	(Where X are other bits, n is the length code)
		msg->len = (buffer[4] & 0x0F);	// Here we and with 0000 1111 to only get info in the last four bits

		// Go through and copy out the data
		for(uint8_t i=0; i<msg->len; i++){
			msg->data[i] = buffer[5 + i];
		}

		return 0;
	}
	return 1;	
}