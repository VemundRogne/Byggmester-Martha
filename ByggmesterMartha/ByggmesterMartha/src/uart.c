/*
 * uart.c
 *
 * Created: 05.09.2020 13:59:53
 *  Author: marth
 */ 

#define F_CPU	4915200
#define BAUD	9600
#define MYUBRR	F_CPU/16/BAUD-1

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <string.h>
#include <util/delay.h>
#include <stdlib.h>

#include "../inc/uart.h"
#include "../inc/mcp2515.h"
#include "../inc/can.h"
#include "../inc/funkyboard.h"

// NOTE: This has not been verified to work after refactoring, but I think
// it should work just fine.
ISR(USART0_RXC_vect){
	cmd_buffer[cmd_receive_counter] = UDR0;
	cmd_receive_counter += 1;
	
	if(cmd_receive_counter == CMD_LEN){
		UART_execute_cmd();
		cmd_receive_counter = 0;
	}
}

void UART_execute_cmd(){
	if(cmd_buffer[0] == UART_BASIC_CMD){
		UART_execute_basic_cmd();
	}
	if(cmd_buffer[0] == UART_MCP2515_CMD){
		UART_execute_mcp2515_cmd();
	}
	if(cmd_buffer[0] == UART_SRAM_CMD){
		UART_execute_sram_cmd();
	}
	if(cmd_buffer[0]== UART_CAN_CMD){
		UART_execute_can_cmd();
	}
	if(cmd_buffer[0] == UART_MOTOR_CMD){
		UART_execute_motor_cmd();
	}
}


void UART_execute_basic_cmd(){
	// Synchronize CMD
	if(cmd_buffer[1] == UART_BASIC_CMD_SYNCHRONIZE){
		UART_tx_polling(1);
	}
	
	// ECHO CMD
	if(cmd_buffer[1] == UART_BASIC_CMD_ECHO){
		for(uint8_t i=ARG_OFFSET; i < (CMD_LEN); i++){
			UART_tx_polling(cmd_buffer[i]);
		}
	}

	if(cmd_buffer[1] == UART_BASIC_CMD_JOYSTICK){
		struct Joystick_pos js = get_joystick_pos();
		int8_t x = js.x;
		uint8_t x_size = abs(x);
		int8_t y = js.y;
		uint8_t y_size = abs(y);

		UART_tx_polling(x_size);
		UART_tx_polling(y_size);

	}
}


void UART_execute_mcp2515_cmd(){
	switch(cmd_buffer[1]){

		// Reads status of MCP2515
		// Writes status back to pytest
		case UART_MCP2515_CMD_READSTATUS:{
			uint8_t status = mcp2515_READ_STATUS();
			UART_tx_polling(status);
			break;
		}

		// Initialize MCP2515 to mode given in cmd_buffer
		// Writes mode back to pytest for confirmation
		case UART_MCP2515_CMD_INIT:{
			uint8_t mcp2515_mode = cmd_buffer[ARG_OFFSET];
			mcp2515_init(mcp2515_mode);
			UART_tx_polling(mcp2515_mode);			//Tell pytest that it received this command?

			break;
		}


		// Writes data from cmd_buffer to MCP2515. (Max 6 bytes)
		// Writes 0x01 to pytest for confirmation
		case UART_MCP2515_CMD_WRITE:{
			uint8_t address = cmd_buffer[ARG_OFFSET];
			uint8_t data_length = cmd_buffer[ARG_OFFSET+1];
			
			mcp2515_WRITE(address, (uint8_t*)&cmd_buffer[ARG_OFFSET+2], data_length);
			UART_tx_polling(0x01);

			break;
		}

		// Reads data from MCP2515
		// Sends data back to pytest
		case UART_MCP2515_CMD_READ:{
			uint8_t address = cmd_buffer[ARG_OFFSET];
			uint8_t data_length = cmd_buffer[ARG_OFFSET+1];

			uint8_t read_buffer[data_length];
			mcp2515_READ(address, &read_buffer[0], data_length);

			for(uint8_t i=0; i < data_length; i++){
				UART_tx_polling(read_buffer[i]);
			}

			break;
		}

		// Reads status of receive buffers of MCP2515
		// Writes status back to pytest
		case UART_MCP2515_CMD_RX_STATUS:{
			uint8_t status = mcp2515_RX_STATUS();
			UART_tx_polling(status);
			break;
		}

		// Modifies a given register, cmd_buffer should include address, data_byte and mask_byte
		// Writes 0x01 to pytest for confirmation
		case UART_MCP2515_CMD_BIT_MODIFY:{
			uint8_t address = cmd_buffer[ARG_OFFSET];
			uint8_t data_byte = cmd_buffer[ARG_OFFSET+1];
			uint8_t mask_byte = cmd_buffer[ARG_OFFSET+2];
			
			mcp2515_BIT_MODIFY(address, data_byte, mask_byte);
			UART_tx_polling(0x01);

			break;
		}
		
		// Reset MCP2515
		// Writes 0x01 to pytest for confirmation 
		case UART_MCP2515_CMD_RESET:{
			mcp2515_RESET();
			UART_tx_polling(0x01);
			break;
		}
		
		// Request to send transmissino buffer of MCP2515
		// Writes back the buffer-selection to pytest for confirmation 
		case UART_MCP2515_CMD_RTS:{
			uint8_t buffer_selection = cmd_buffer[ARG_OFFSET];  //Get the buffers that should request to send
			mcp2515_RTS(buffer_selection);
			UART_tx_polling(buffer_selection);
			break;
		}
		
		/*
		 * Set the "receive_can_on_interrupt" flag to the first arugment
		*/
		case UART_MCP2515_CMD_WRITE_RX_CAN_FLAG:{
			receive_can_on_interrupt = cmd_buffer[ARG_OFFSET];
			UART_tx_polling(0);	// Send an ACK
			break;
		}
    }
}

void UART_execute_can_cmd(){
 
	if (cmd_buffer[1] == UART_CAN_CMD_TRANSMIT){
		struct can_msg tx_msg;
		tx_msg.ID = (cmd_buffer[2] << 8) | cmd_buffer[3];
		tx_msg.len = cmd_buffer[4];
		for(uint8_t i = 0; i<tx_msg.len; i++){
			tx_msg.data[i] = cmd_buffer[5+i];
		}

		uint8_t can_status = can_transmit_message(&tx_msg);
		UART_tx_polling(can_status);
	}
	if (cmd_buffer[1] == UART_CAN_CMD_RECEIVE){
		struct can_msg msg_r;
		
		// Try to receive a message
		uint8_t can_status = can_receive_message(&msg_r);
		
		UART_tx_polling(can_status);	// Send out the status so the master knows if there
		// will be a message soon.
		
		// We actually got a message
		if (can_status == 0){
			// Transfer the message
			uint8_t polling_array[11];
			polling_array[0] = (uint8_t) (msg_r.ID << 8);
			polling_array[1] = (uint8_t) (msg_r.ID);
			polling_array[2] = msg_r.len;
			
			// Copy out the data into a nice buffer
			for (uint8_t i=0; i<msg_r.len; i++){
				polling_array[3+i] = msg_r.data[i];
			}
			
			for ( uint8_t i = 0; i < 11; i++){
				UART_tx_polling(polling_array[i]);
			}
		}
	}
	
	if (cmd_buffer[1] == UART_CAN_CMD_TX_BUFFER){
		uint8_t tx_buffer_address = 3;
		uint8_t valdi_tx_buffer = can_valid_transmit_buffer(&tx_buffer_address);
		uint8_t polling_array[2];
		polling_array[0] = tx_buffer_address;
		polling_array[1] = valdi_tx_buffer;
		for ( uint8_t i = 0; i < 2; i++){
			UART_tx_polling(polling_array[i]);
		}
	}
	
	if (cmd_buffer[1] == UART_CAN_CMD_RX_BUFFER){
		uint8_t rx_buffer_address = 3;
		uint8_t pending_rx_buffer = can_pending_receive_buffer(&rx_buffer_address);
		uint8_t polling_array[2] = {rx_buffer_address, pending_rx_buffer};
		for ( uint8_t i = 0; i < 2; i++){
			UART_tx_polling(polling_array[i]);
		}
	}
	
}


void UART_execute_sram_cmd(){
	
	volatile char *ext_ram = (char *) 0x1800; // Start address for the SRAM

	// SRAM write
	// sram_address_H = cmd_buffer[2]
	// sram_address_L = cmd_buffer[3]
	// value to write at address = cmd_buffer[4]
	// Returns 0 to UART
	if (cmd_buffer[1] == UART_SRAM_WRITE){
		uint16_t sram_address = (cmd_buffer[2] << 8) | cmd_buffer[3];
		uint8_t sram_value = cmd_buffer[4];
		ext_ram[sram_address] = sram_value;
		UART_tx_polling(0);
	}
	
	// SRAM read
	// sram_address_H = cmd_buffer[2]
	// sram_address_L = cmd_buffer[3]
	// value to retrieve at address = ext_ram[sram_address]
	// returns 0 to UART
	if (cmd_buffer[1] == UART_SRAM_READ){
		uint16_t sram_address = (cmd_buffer[2] << 8) | cmd_buffer[3];
		UART_tx_polling(ext_ram[sram_address]);
	}
}

void UART_execute_motor_cmd(){
	/* Triggers a read of the encoder. Note the return is in the CAN interrupt */
	if(cmd_buffer[1] == UART_MOTOR_READ_ENCODER){
		struct can_msg tx_msg;
		tx_msg.ID = 1010;
		tx_msg.len = 0;

		uint8_t can_status = can_transmit_message(&tx_msg);
	}
}

void uart_init(){
	// Set Baudrate
	UBRR0L = MYUBRR;

	// Enable RX and TX
	UCSR0B = (1<<RXEN0) | (1<<TXEN0);

	// Set frame format: 8data, 1stop bit
	UCSR0C = (1<<URSEL0) | (3<<UCSZ00);
	
	// Enable RX interrupt
	UCSR0B |= (1<<RXCIE0);
	
	cmd_receive_counter = 0;
}

// receives one byte in polling mode
uint8_t UART_rx_polling(){
	// Wait for data to be received
	while ( !(UCSR0A & (1<<RXC0) ));
	
	// Get data from UART and return
	return UDR0;
}

// transmits one byte in polling mode
void UART_tx_polling(uint8_t data){
	// Wait for empty transmit buffer
	while ( !(UCSR0A & (1<<UDRE0) ));
	
	// Put data into buffer, sends the data
	UDR0 = data;
}
