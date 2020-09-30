#include "../inc/can.h"
#include "../inc/mcp2515.h"

#define READ 	0b00000011
#define WRITE 	0b00000010
#define RESET 	0b11000000

void spi_write(uint8_t* set_mode_command, uint8_t n){
	//Do stuff
}

void spi_init(){
	//Do stuff
}

void can_init(enum mode CANmode){
	//Initialize SPI to be able to communicate with mcp2515
	spi_init();

	//Reset mcp2515 on initialization
	uint8_t reset_command = RESET;
	spi_write(&reset_command, 1);

	//Should we add self-test?

	//Select mode
	uint8_t set_mode_command[3] = {WRITE, MCP_CANCTRL, CANmode};
	spi_write(&set_mode_command[0], 3);
};



uint8_t can_read(uint8_t address){
	uint8_t data;

	//write(READ, address, 2)
	//read()
}