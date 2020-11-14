/**
* @file
* @brief Library for spi communication
*/

#include <avr/io.h>
#include <stdio.h>

#ifndef SPI_H_
#define SPI_H_


/**
* @brief Initializes the SPI peripheral to support MCP2515:
 *  - Master mode
 *  - Data order MSB
 *  - CPHA = CPOL = 0 or 1
 *  - MCP2515 supports any frequency up to 10MHz
*
* @return void
*/
void spi_init();


/**
* @brief  Read data from the SPI peripheral in polling mode
*
* @param[out] read_buffer Pointer to buffer where read bytes are stored
* @param[in] n Number of bytes to read

* @return void
*/
void spi_read(uint8_t *read_buffer, uint8_t n);

/*
 * Function: Write data to the SPI peripheral in polling mode
 * ----------------------------------------------------------
 * Writes n bytes of the data in write_buffer to the SPI
 * peripheral. Any received bytes during this is lost.
*/

/**
* @brief  Write data to the SPI peripheral in polling mode
*
* @param[in] write_buffer Pointer to buffer where bytes to be written are stored
* @param[in] n Number of bytes to write

* @return void
*/
void spi_write(uint8_t *write_buffer, uint8_t n);


/**
* @brief  Read and write from the SPI peripheral in polling mode
*
* @param[in] write_buffer Pointer to buffer where bytes to be written are stored
* @param[out] read_buffer Pointer to buffer where read bytes are stored
* @param[in] n Number of bytes to write

* @return void
*/
void spi_readwrite(uint8_t *write_buffer,
                   uint8_t *read_buffer,
				   uint8_t n);

#endif /* SPI_H_ */