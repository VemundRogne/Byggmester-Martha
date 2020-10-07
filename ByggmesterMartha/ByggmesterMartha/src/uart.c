/*
 * uart.c
 *
 * Created: 05.09.2020 13:59:53
 *  Author: marth
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>

#define F_CPU	4915200
#define BAUD	9600
#define MYUBRR	F_CPU/16/BAUD-1

#include <util/delay.h>

#include "../inc/uart.h"
#include "../inc/mcp2515.h"

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
}

void UART_execute_mcp2515_cmd(){
	switch(cmd_buffer[1]){
		case UART_MCP2515_CMD_READSTATUS:{
			uint8_t status = mcp2515_READ_STATUS();
			UART_tx_polling(status);
			break;
		}

		default:{
			uint8_t error_val = 0x00;
			UART_tx_polling(error_val);
		}

	}
}

void init_UART(){
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