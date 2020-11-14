
#define MCP_CS_PORT PORTB
#define MCP_CS_PIN	4
#define MCP_CS_DDR	DDRB

#define F_CPU	4915200
#include <util/delay.h>
#include <avr/io.h>
#include <avr/interrupt.h>

#include "../inc/spi.h"
#include "../inc/mcp2515.h"
#include "../inc/can.h"
#include "../inc/game.h"
#include "../inc/uart.h"

union signed_64_unsigned_8{
	int64_t signed_64;
	uint8_t unsigned_8[8];
} signed_64_unsigned_8;

void mcp2515_select(){
	MCP_CS_PORT &= !(1 << MCP_CS_PIN);
}

void mcp2515_deselect(){
	MCP_CS_PORT |= (1 << MCP_CS_PIN);
}

void enable_RX_interrupts(){
	// Enable interrupts in MCP2515
	// CANINTE bits 1 and 0 enable RX1IE and RX0IE
	mcp2515_BIT_MODIFY(MCP_CANINTE, 3, 3);
	
	// The interrupt pin is connected to INT0 on the atmega
	// Ensure the pin is in input mode (high impedance)
	PORTD &= ~(1<<PD2);
	MCUCR |= (1<<ISC01);	// INT0 interrupt triggers on falling edge
	GICR |= (1<<INT0);		// Enable INT0 interrupts
}


// Interrupt service routine for INT0
ISR(INT0_vect){
	// Check what kind of interrupt it was
	uint8_t CANINTF_register = 0;
	

	// This flag is used for automated loopback testing. In normal operation this will always run
	if(receive_can_on_interrupt == 1){
		mcp2515_READ(MCP_CANINTF, &CANINTF_register, 1);
		// This passes for both receive buffers. We don't really care which buffer got data...
		struct can_msg msg_r;
		uint8_t can_status = can_receive_message(&msg_r);
		
		if (can_status == 0){
			if(msg_r.ID == 15){
				game_over_check(msg_r.data[0]);
			}

			// This function simply echoes whatever data straight to UART
			// Used by transfer_signed_32_to_python in node 2
			if(msg_r.ID == 1){
				for(uint8_t i = 0; i<msg_r.len; i++){
					UART_tx_polling(msg_r.data[i]);
				}
			}

			// Transfer encoder position to UART
			if(msg_r.ID == 1010){
				UART_tx_polling(msg_r.data[1]);
				UART_tx_polling(msg_r.data[0]);
			}
			
			if (msg_r.ID == 139){
				enter_play_game(score_count);
			}
		}
		// Clear the interrupt
		mcp2515_BIT_MODIFY(MCP_CANINTF, 3, 0);
	}
}


void mcp2515_configure_bit_timing(){
	// Setting TQ duration: 0x3F in both mask and value gives the maximum prescaling
	// and the lowest baudrate.
	// TQ = 2*(BRP/F_OSC) = 63uS
	mcp2515_BIT_MODIFY(MCP_CNF1, 0x3F, 0x3F);
	
	// Setting Synchronization Jump Width: 0x00 results in SJW = 1
	mcp2515_BIT_MODIFY(MCP_CNF1, 0xC0, 0x00);

	// Setting PropSeg = 2 TQ
	// CNF2 bits 2-0 (PRSEG2, 1, 0) configures the PropSeg length: 
	// (PRSEG + 1) x TQ. Thus PRSEG = 1 gives PropSeg = 2
	mcp2515_BIT_MODIFY(MCP_CNF2, 0x07, 0x01);
	
	// Setting PS1 length:
	// We target a length of 7 TQ.
	// (PHSEG1 + 1) = 7 -> PHSEG1 = 6
	// This is done with bit 5-3.
	mcp2515_BIT_MODIFY(MCP_CNF2, 0x07 << 3, 0x06 << 3);
	
	// Setting PS2 length:
	// We target a lenght of 6 TQ
	// (PHSEG2 + 1) = 6 -> PHSEG2 = 5
	mcp2515_BIT_MODIFY(MCP_CNF3, 0x07, 0x05);
}

void mcp2515_RESET(){
	// Select the MCP2515 (pull CS low)
	mcp2515_select();

	// Creates a reset instruction and writes it to the MCP2515
	uint8_t reset_instruction = MCP_RESET;
	spi_write(&reset_instruction, 1);

	// Deselect MCP2515 (release CS)
	mcp2515_deselect();
}


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


void mcp2515_BIT_MODIFY(uint8_t register_addr, uint8_t mask_byte, uint8_t value_byte){
	// Select the MCP2515 (pull CS low)
	mcp2515_select();
	
	uint8_t modify_command[4] = {MCP_BITMOD, register_addr, mask_byte, value_byte};
	spi_write(&modify_command[0], 4);

	// Deselect MCP2515 (release CS)
	mcp2515_deselect();
};


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


void mcp2515_init(enum mcp2515_mode CANmode){
	//Initialize SPI to be able to communicate with mcp2515
	spi_init();

	// Setting direction of slave_select pin and deselecting mcp
	MCP_CS_DDR |= (1 << MCP_CS_PIN);
	mcp2515_deselect();
	_delay_ms(10);
	//Reset mcp2515 on initialization
	mcp2515_RESET();
	
	mcp2515_BIT_MODIFY(MCP_CNF1, 0xff, 0x43);
	mcp2515_BIT_MODIFY(MCP_CNF2, 0xff, 0xb5);
	mcp2515_BIT_MODIFY(MCP_CNF3, 0xff, 0x01);

	//Set up timing for CAN bus
	//mcp2515_configure_bit_timing();
	
	//Select mode
	mcp2515_BIT_MODIFY(MCP_CANCTRL, 0xE0, CANmode << 5);
	
	// Enable buffer 0 to receive all messages
	// This turns off the mask and fileter -> receive any message
	mcp2515_BIT_MODIFY(MCP_RXB0CTRL, 0b01100000, 0b01100000);
	
	// Enable reception of messages in interrupt mode
	receive_can_on_interrupt = 1;
	enable_RX_interrupts();
}