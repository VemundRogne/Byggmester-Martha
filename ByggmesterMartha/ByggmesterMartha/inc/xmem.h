/**
* @file
* @brief Library to keep control of external memory addressing
*/

#ifndef XMEM_H_
#define XMEM_H_

#define ADC_START_ADDRESS	0x1500
#define SRAM_START_ADDRESS 0x1800

/**
* @brief Initialize external memory interface for atmega162
*
* @return void
*/
void xmem_init();

#endif /* XMEM_H_ */