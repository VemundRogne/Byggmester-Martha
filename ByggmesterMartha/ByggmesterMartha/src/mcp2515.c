#define READ 	0b00000011
#define WRITE 	0b00000010
#define RESET 	0b11000000

#define MCP_CS_PORT PORTB
#define MCP_CS_PIN	4
#define MCP_CS_DDR	DDRB

#define F_CPU	4915200
#include <util/delay.h>
#include <avr/io.h>

#include "../inc/spi.h"
#include "../inc/mcp2515.h"

void mcp2515_select(){
	MCP_CS_PORT &= !(1 << MCP_CS_PIN);
}

void mcp2515_deselect(){
	MCP_CS_PORT |= (1 << MCP_CS_PIN);
}

void mcp2515_init(enum mode CANmode){
	//Initialize SPI to be able to communicate with mcp2515
	spi_init();

	// Setting direction of slave_select pin and deselecting mcp
	MCP_CS_DDR |= (1 << MCP_CS_PIN);
	mcp2515_deselect();
	_delay_ms(10);
	//Reset mcp2515 on initialization
	mcp2515_reset();

	//Should we add self-test?

	//Select mode
	uint8_t command[3] = {WRITE, MCP_CANCTRL, CANmode};
	spi_write(&command[0], 3);
}

void mcp2515_RESET(){
	mcp2515_select();

	uint8_t reset_command = RESET;
	spi_write(&reset_command, 1);

	mcp2515_deselect();
}

void mcp2515_READ(uint8_t address, uint8_t *read_buffer, uint8_t n){
	mcp2515_select();

	uint8_t command[2] = {READ, address};
	spi_write(&command[0], 2);
	
	spi_read(read_buffer, n);

	mcp2515_deselect();
}

void mcp2515_WRITE(uint8_t address, uint8_t *write_buffer, uint8_t n){
	mcp2515_select();

	uint8_t write_command[2] = {WRITE, address};
	spi_write(&write_command[0], 2);
	spi_write(write_buffer, n);

	mcp2515_deselect();
}

//First register [0, 0, 0, 0, 1, 0, 0, 0]
//Second register [0, 0, 0,]

//Finn en buffer med TXREQ = 0, altså en som er ledig
//Skriv data til buffer
//Marker at buffer er klar til å sendes med TXREQ = 1

//Lage funksjon som fyller n bytes med


// Initialize transmission buffer
// SJekk om buffer er klar til å sende
// Skriv data til buffer
// Buffer sender interrupt "transmition finished"