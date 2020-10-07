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
	if(cmd_buffer[0] == UART_SRAM){
		UART_execute_sram_cmd();
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