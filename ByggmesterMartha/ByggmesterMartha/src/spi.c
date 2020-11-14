
#include <avr/io.h>
#include <stdio.h>
#include "../inc/spi.h"

/*
 * Function: Initialize SPI low-level driver
 * -----------------------------------------
 * Initializes the SPI peripheral to support MCP2515:
 *  - Master mode
 *  - Data order MSB
 *  - CPHA = CPOL = 0 or 1
 *  - MCP2515 supports any frequency up to 10MHz
*/
void spi_init(){
	// Set MOSI, SCK and SS as output pins
	DDRB |= (1<<DDB5) | (1<<DDB7) |(1<<DDB4);
	
	// Enables SPI in master mode and frequency to F_CPU/16
	SPCR |= (1<<SPE) | (1<<MSTR) | (1<<SPR0);
};

/*
 * Function: write and read a single byte with the SPI peripheral
 * --------------------------------------------------------------
 * Writes the input byte to the SPI peripheral, and return the
 * read value.
*/
uint8_t spi_readwrite_byte(uint8_t byte){
	SPDR = byte;
	// Wait for transmission to complete
	while(!(SPSR & (1<<SPIF))){
	}
	return SPDR;
}

/*
 * Function: Read data from the SPI peripheral in polling mode
 * -----------------------------------------------------------
 * Reads n bytes of data from SPI peripheral into *read_buffer
 * in polling mode. Transmits 0xFF while reading.
*/
void spi_read(uint8_t *read_buffer, uint8_t n){
	for(uint8_t i=0; i<n; i++){
		read_buffer[i] = spi_readwrite_byte(0xFF);
	}
}

/*
 * Function: Write data to the SPI peripheral in polling mode
 * ----------------------------------------------------------
 * Writes n bytes of the data in write_buffer to the SPI
 * peripheral. Any received bytes during this is ignored.
*/
void spi_write(uint8_t *write_buffer, uint8_t n){
	for(uint8_t i=0; i<n; i++){
		spi_readwrite_byte(write_buffer[i]);
	}
}

/*
 * Function: Read and write from the SPI peripheral in polling mode
 * ----------------------------------------------------------------
 * Writes n bytes of data from write_buffer to the SPI peripheral,
 * After each write the received data is read into read_buffer.
*/
void spi_readwrite(uint8_t *write_buffer,
                   uint8_t *read_buffer,
				   uint8_t n){
	
	for(uint8_t i=0; i<n; i++){
		read_buffer[i] = spi_readwrite_byte(write_buffer[i]);
	}
}