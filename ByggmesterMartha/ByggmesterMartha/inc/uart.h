/*
 * uart.h
 *
 * Created: 05.09.2020 13:59:36
 *  Author: marth
 */ 

#ifndef UART_H_
#define UART_H_

// This is the CMD receive buffer, when it is filled we know we have a full CMD
#define CMD_LEN 10		// The full buffer: [CMD_NO, CMD_SUBNO, ARG0, ARG1, ... ]
#define ARG_OFFSET 2	// The offset to get to the first ARG
volatile uint8_t cmd_buffer[CMD_LEN];
volatile uint8_t cmd_receive_counter;

void init_UART();

uint8_t UART_rx_polling();

void UART_tx_polling(uint8_t data);

ISR(USART0_RXC_vect);

// Points to the correct CMDs
#define UART_BASIC_CMD	0	// NOTE: This _must_ be at number 0
#define UART_SRAM_CMD 1 
#define UART_MCP2515_CMD 2
#define UART_CAN_CMD 3

void UART_execute_cmd();

// Basic commands
// CMD_SUBNO:
//   0: Synchronize - send the number 1. Used to sync the UART protocol
//   1: ECHO - return the full argument
#define UART_BASIC_CMD_SYNCHRONIZE	0	// Note: THIS _must_ be at number 0
#define UART_BASIC_CMD_ECHO			1
#define UART_BASIC_CMD_JOYSTICK		2

// SRAM commands
// CMD_SUBNO:
//	0: Write - writes cmd_buffer[4] to SRAM 
//	1: Read - reads from SRAM
#define UART_SRAM_WRITE 0
#define UART_SRAM_READ 1

// MCP2515 commands
// CMD_SUBNO:
//   0: Read status - Checks if possible to read status of CAN controller
//   1: Initialize - Initialize can controller with mode cmd_buffer[3]
//   2: Write - Write to MCP2515
//   3: Read - Reads from MCP2515
//   4: RX status - Reads receive buffer status
//   5: Bit modify - Modifies spesific bits in a given address of MCP2515
#define UART_MCP2515_CMD_READSTATUS 0
#define UART_MCP2515_CMD_INIT 1
#define UART_MCP2515_CMD_WRITE 2
#define UART_MCP2515_CMD_READ 3
#define UART_MCP2515_CMD_RX_STATUS 4
#define UART_MCP2515_CMD_BIT_MODIFY 5
#define UART_MCP2515_CMD_RESET 6
#define UART_MCP2515_CMD_RTS 7

// CAN commands
// CMD_SUBNO:
//	0: Transmits data over CAN bus
//	1: Receives data from CAN bus
//  2: Get valid CAN transmit buffer
#define UART_CAN_CMD_TRANSMIT 0
#define UART_CAN_CMD_RECEIVE 1
#define UART_CAN_CMD_TX_BUFFER 2
#define  UART_CAN_CMD_RX_BUFFER 3

void UART_execute_basic_cmd();
void UART_execute_sram_cmd();
void UART_execute_can_cmd();
void UART_execute_mcp2515_cmd();


#endif /* UART_H_ */
