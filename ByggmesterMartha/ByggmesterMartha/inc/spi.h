
#include <avr/io.h>
#include <stdio.h>

#ifndef SPI_H_
#define SPI_H_

/*
 * Function: Initialize SPI low-level driver
 * -----------------------------------------
 * Initializes the SPI peripheral to support MCP2515:
 *  - Master mode
 *  - Data order MSB
 *  - CPHA = CPOL = 0 or 1
 *  - MCP2515 supports any frequency up to 10MHz
*/	
void spi_init();

/*
 * Function: Read data from the SPI peripheral in polling mode
 * -----------------------------------------------------------
 * Reads n bytes of data from SPI peripheral into *read_buffer
 * in polling mode. Transmits 0xFF while reading.
*/
void spi_read(uint8_t *read_buffer, uint8_t n);

/*
 * Function: Write data to the SPI peripheral in polling mode
 * ----------------------------------------------------------
 * Writes n bytes of the data in write_buffer to the SPI
 * peripheral. Any received bytes during this is lost.
*/
void spi_write(uint8_t *write_buffer, uint8_t n);

/*
 * Function: Read and write from the SPI peripheral in polling mode
 * ----------------------------------------------------------------
 * Writes n bytes of data from write_buffer to the SPI peripheral,
 * After each write the received data is read into read_buffer.
*/
void spi_readwrite(uint8_t *write_buffer,
                   uint8_t *read_buffer,
				   uint8_t n);

#endif /* SPI_H_ */