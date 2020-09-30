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
	mcp2515_RESET();

	//Should we add self-test?

	//Select mode
	uint8_t command[3] = {MCP_WRITE, MCP_CANCTRL, CANmode};
	spi_write(&command[0], 3);
}

/*
 * Function: Send reset command to mcp2515
 * ---------------------------------------
 * Single-byte instruction to re-initialize the internal registers
 * of the MCP2515 and set Configuration mode.
*/
void mcp2515_RESET(){
	// Select the MCP2515 (pull CS low)
	mcp2515_select();

	// Creates a reset instruction and writes it to the MCP2515
	uint8_t reset_instruction = MCP_RESET;
	spi_write(&reset_instruction, 1);

	// Deselect MCP2515 (release CS)
	mcp2515_deselect();
}


/*
 * Function: Read n bytes from MCP2515
 * -----------------------------------
 * Reads n bytes from MCP2515 starting from address
*/
void mcp2515_READ(uint8_t address, uint8_t *read_buffer, uint8_t n){
	// Select the MCP2515 (pull CS low)
	mcp2515_select();

	// Create a read instruction and writes it to MCP2515
	uint8_t read_instruction[2] = {MCP_READ, address};
	spi_write(&read_instruction[0], 2);

	// Reads the data from MCP2515
	spi_read(read_buffer, n);

	mcp2515_deselect();
}


/*
 * Function: Write n bytes to the MCP2515
 * --------------------------------------
 * Write n bytes to the MCP2515 starting at address
*/
void mcp2515_WRITE(uint8_t address, uint8_t *write_buffer, uint8_t n){
	// Select the MCP2515 (pull CS low)
	mcp2515_select();
	
	// Create a write instruction and send it to MCP2515
	uint8_t write_instruction[2] = {MCP_WRITE, address};
	spi_write(&write_instruction[0], 2);

	// Write the data to MCP2515
	spi_write(write_buffer, n);

	// Deselect MCP2515 (release CS)
	mcp2515_deselect();
}


/*
 * Function: Get status of receive buffers
 * --------------------------------------
 * See FIGURE 12-9 in MCP2515 datasheet for description of the byte returned
*/
uint8_t mcp2515_RX_STATUS(){
	uint8_t rx_status;

	// Select the MCP2515 (pull CS low)
	mcp2515_select();

	// Create a write instruction and send it to MCP2515
	uint8_t status_command = MCP_RX_STATUS;
	spi_write(&status_command, 1);

	// Loads status to rx_status pointer
	spi_read(&rx_status, 1);

	// Deselect MCP2515 (release CS)
	mcp2515_deselect();
	return rx_status;
}



/*
 * Function: Modify a bit in a chosen register in MCP2515
 * --------------------------------------
 * Sets bit in register_addr to value (0/1)
*/
void mcp2515_BIT_MODIFY(uint8_t register_addr, uint8_t bit, uint8_t value){
	// Select the MCP2515 (pull CS low)
	mcp2515_select();
	
	uint8_t modify_command[4] = {MCP_BITMOD, register_addr, 1 << bit, 1 << value};
	spi_write(&modify_command[0], 4);

	// Deselect MCP2515 (release CS)
	mcp2515_deselect();
};