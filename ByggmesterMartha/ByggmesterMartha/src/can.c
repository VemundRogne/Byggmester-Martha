#include "../inc/can.h"
#include "../inc/mcp2515.h"
#include <avr/io.h>

#define READ 	0b00000011
#define WRITE 	0b00000010
#define RESET 	0b11000000

#define MCP_CS_PORT PORTB
#define MCP_CS_PIN	4
#define MCP_CS_DDR	DDRB

void spi_write(uint8_t* command, uint8_t n){
	//Do stuff
}

void spi_read(uint8_t* read_buffer, uint8_t n){
	//Do stuff
}

void spi_init(){
	//Do stuff
}


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

	//Reset mcp2515 on initialization
	uint8_t reset_command = RESET;
	spi_write(&reset_command, 1);

	//Should we add self-test?

	//Select mode
	uint8_t command[3] = {WRITE, MCP_CANCTRL, CANmode};
	spi_write(&command[0], 3);
};



void mcp2515_read(uint8_t address, uint8_t *read_buffer, uint8_t n){
	mcp2515_select();

	uint8_t command[2] = {READ, address};
	spi_write(&command[0], 2);
	
	spi_read(read_buffer, n);

	mcp2515_deselect();
}

void mcp2515_write(uint8_t address, uint8_t *write_buffer, uint8_t n){
	mcp2515_select();

	uint8_t write_command[2] = {WRITE, address};
	spi_write(&write_command[0], 2);
	spi_write(write_buffer, n);

	mcp2515_deselect();
};
