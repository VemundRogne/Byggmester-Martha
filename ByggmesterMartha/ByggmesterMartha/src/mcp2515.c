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

void mcp2515_init(enum mcp2515_mode CANmode){
	//Initialize SPI to be able to communicate with mcp2515
	spi_init();

	// Setting direction of slave_select pin and deselecting mcp
	MCP_CS_DDR |= (1 << MCP_CS_PIN);
	mcp2515_deselect();
	_delay_ms(10);
	//Reset mcp2515 on initialization
	mcp2515_RESET();

	//Should we add self-test?

	mcp2515_select();
	//Select mode
	mcp2515_BIT_MODIFY(MCP_CANCTRL, 0xE0, CANmode << 5);
	mcp2515_deselect();
	
	
	//T_osc = 62.5 ns
	mcp2515_BIT_MODIFY(MCP_CNF1, 0x3F, 0x04); //BRP<5:0>, T_q = 625 ns
	mcp2515_BIT_MODIFY(MCP_CNF1, 0xC0, 0); //SJW<1:0>

	mcp2515_BIT_MODIFY(MCP_CNF2, 0x3F, 0); //PHSEG<12:0>
	mcp2515_BIT_MODIFY(MCP_CNF2, 0x40, 0); //SAM<bit 6>
	mcp2515_BIT_MODIFY(MCP_CNF2, 0x80, 0); //BTLMODE<bit 7>
	
	mcp2515_BIT_MODIFY(MCP_CNF3, 0x07, 0); //PHSEG<22:20>
	mcp2515_BIT_MODIFY(MCP_CNF3, 0x40, 0); //WAKFIL<bit 6>
	mcp2515_BIT_MODIFY(MCP_CNF3, 0x80, 0); //SOF<bit 7
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
 * Function: Read most used status bits for message transmission and reception
 * ---------------------------------------------------------------------------
 * This is a single-instruction way to find many status bits:
 *
 * Returns status byte:
 *  Bit 7: CANINTF.TX2IF	- Flag is set when transmission completes
 *  Bit 6: TXB2CNTRL.TXREQ	- Indicates status of pending transmission (0: register clear, 1: register is pending transmission)
 *  Bit 5: CANINTF.TX1IF	- Flag is set when transmission completes
 *  Bit 4: TXB1CNTRL.TXREQ	- Indicates status of pending transmission (0: register clear, 1: register is pending transmission)
 *  Bit 3: CANINTF.TX0IF	- Flag is set when transmission completes
 *  Bit 2: TXB0CNTRL.TXREQ	- Indicates status of pending transmission (0: register clear, 1: register is pending transmission)
 *  Bit 1: CANINTFL.RX1IF	- Indicates that a message has been received into Receive buffer 1
 *  Bit 0: CANINTF.RX0IF	- Indicates that a message has been received into Receive buffer 0
*/
uint8_t mcp2515_READ_STATUS(){
	// Select the MCP2515 (pull CS low)
	mcp2515_select();
	
	// Create and transmit READ STATUS instruction
	uint8_t read_status_instruction = MCP_READ_STATUS;
	spi_write(&read_status_instruction, 1);

	// Read the returned status register	
	uint8_t status_register = 0;
	spi_read(&status_register, 1);
	
	// Deselect MCP2515 (release CS)
	mcp2515_deselect();
	
	return status_register;
}


/*
 * Function: Modify a bit in a chosen register in MCP2515
 * --------------------------------------
 * Sets bit in register_addr to value (0/1)
*/
void mcp2515_BIT_MODIFY(uint8_t register_addr, uint8_t mask_byte, uint8_t value_byte){
	// Select the MCP2515 (pull CS low)
	mcp2515_select();
	
	uint8_t modify_command[4] = {MCP_BITMOD, register_addr, mask_byte, value_byte};
	spi_write(&modify_command[0], 4);

	// Deselect MCP2515 (release CS)
	mcp2515_deselect();
};

/*
 * Function: Request to send for MCP2515
 * --------------------------------------
	Gets selected transmit buffer(s) as input
	and initiates message transmission for the selected transmit buffer(s). 
 * 
*/
void mcp2515_RTS(uint8_t RTS_selection){
	// Select the MCP2515 (pull CS low)
	mcp2515_select();
	
	// Create a RTS instruction and send it to MCP2515
	// logic for rts_instruction variable permits using 
	// both the defined MCP_RTS_TX as input, as well as 1, 2, 4 and 7
	
	uint8_t rts_instruction = (0x80 | RTS_selection);
	spi_write(&rts_instruction, 1);
	
	// Deselect MCP2515 (release CS)
	mcp2515_deselect();	
}
