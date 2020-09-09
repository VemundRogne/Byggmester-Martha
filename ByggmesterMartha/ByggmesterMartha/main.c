/*
 * ByggmesterMartha.c
 *
 * Created: 26/08/2020 09:09:34
 * Author : Vemund
 */ 


#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>

#define F_CPU	4915200
#define BAUD	9600
#define MYUBRR	F_CPU/16/BAUD-1

#include <util/delay.h>
#include "inc/xmem.h"
#include "inc/uart.h"

#include <stdlib.h>
void SRAM_test(void)
{
	volatile char *ext_ram = (char *) 0x1800; // Start address for the SRAM
	uint16_t ext_ram_size = 0x800;
	uint16_t write_errors = 0;
	uint16_t retrieval_errors = 0;
	printf("Starting SRAM test...\n");
	// rand() stores some internal state, so calling this function in a loop will
	// yield different seeds each time (unless srand() is called before this function)
	uint16_t seed = rand();
	// Write phase: Immediately check that the correct value was stored
	srand(seed);
	for (uint16_t i = 0; i < ext_ram_size; i++) {
		uint8_t some_value = rand();
		ext_ram[i] = some_value;
		uint8_t retreived_value = ext_ram[i];
		if (retreived_value != some_value) {
			printf("Write phase error: ext_ram[%4d] = %02X (should be %02X)\n", i, retreived_value, some_value);
			write_errors++;
		}
	}
	// Retrieval phase: Check that no values were changed during or after the write phase
	srand(seed);
	// reset the PRNG to the state it had before the write phase
	for (uint16_t i = 0; i < ext_ram_size; i++) {
		uint8_t some_value = rand();
		uint8_t retreived_value = ext_ram[i];
		if (retreived_value != some_value) {
			printf("Retrieval phase error: ext_ram[%4d] = %02X (should be %02X)\n", i, retreived_value, some_value);
			retrieval_errors++;
		}
	}
	printf("SRAM test completed with \n%4d errors in write phase and \n%4d errors in retrieval phase\n\n", write_errors, retrieval_errors);
}

void test_XMEM_addressing(){
	// ADC-Chip select: PD3
	// SRAM-Chip select: PD4
	// OLED-Chip: PD5
	
	// OLED addresses: 0x1000 -> 0x13FF
	// ADC addresses:  0x1400 -> 0x17FF
	// SRAM addresses: 0x1800 -> 0x1FFF
	
	volatile char *OLED_add = (char *) 0x1000; // OLED start_address
	uint16_t OLED_len = 0x13FF - 0x1000; // OLED address length
	
	volatile char *ADC_add = (char *) 0x1400; // ADC start-address
	uint16_t ADC_len = 0x17FF - 0x1400; // ADC address length
	
	volatile char *SRAM_add = (char *) 0x1800; // ADC start-address
	uint16_t SRAM_len = 0x1FFF - 0x1800; // ADC address length
	
	printf("Running through all OLED addresses...\n");
	// Run through all OLED addresses
	for(uint16_t index = 0; index < OLED_len; index ++){
		OLED_add[index] = 0xff;
		_delay_ms(1);
		
		// verify only OLED chip-select is low
		uint8_t OLED_CS = PIND & (1<<PD5);
		uint8_t ADC_CS = PIND & (1<<PD3);
		uint8_t SRAM_CS = PIND & (1<<PD4);
		
		if (OLED_CS != 0) {
			printf("OLED_CS read as high, when it should have been low. [OLED-ADDRESS-PHASE]\n");
		}
		
		if (ADC_CS == 0) {
			printf("ADC_CS read as low, when it should have been high. [OLED-ADDRESS-PHASE]\n");
		}
		
		if (SRAM_CS == 0) {
			printf("SRAM_CS read as low, when it should have been high. [OLED-ADDRESS-PHASE]\n");
		}
		
		_delay_ms(1);
	}
	printf("Done running through OLED addresses!\n");
	
//	printf("Running through all ADC addresses...\n");
//	// Run through all OLED addresses
//	for(uint16_t index = 0; index < ADC_len; index ++){
//		ADC_add[index] = 0xff;
//		_delay_ms(1);
		
//		// verify only OLED chip-select is low
//		uint8_t OLED_CS = PIND & (1<<PD5);
//		uint8_t ADC_CS = PIND & (1<<PD3);
//		uint8_t SRAM_CS = PIND & (1<<PD4);
		
//		if (ADC_CS != 0) {
//			printf("ADC_CS read as high, when it should have been low. [ADC-ADDRESS-PHASE]\n");
//		}
		
//		if (OLED_CS == 0) {
//			printf("OLED_CS read as low, when it should have been high. [ADC-ADDRESS-PHASE]\n");
//		}
		
//		if (SRAM_CS == 0) {
//			printf("SRAM_CS read as low, when it should have been high. [ADC-ADDRESS-PHASE]\n");
//		}
		
//		_delay_ms(1);
//	}
//	printf("Done running through ADC addresses!\n");
	
	printf("Running through all SRAM addresses...\n");
	// Run through all SRAM addresses
	for(uint16_t index = 0; index < SRAM_len; index ++){
		SRAM_add[index] = 0xff;
		_delay_ms(1);
		
		// verify only OLED chip-select is low
		uint8_t OLED_CS = PIND & (1<<PD5);
		uint8_t ADC_CS = PIND & (1<<PD3);
		uint8_t SRAM_CS = PIND & (1<<PD4);
		
		if (SRAM_CS != 0) {
			printf("SRAM_CS read as high, when it should have been low. [SRAM-ADDRESS-PHASE]\n");
		}
		
		if (OLED_CS == 0) {
			printf("OLED_CS read as low, when it should have been high. [SRAM-ADDRESS-PHASE]\n");
		}
		
		if (ADC_CS == 0) {
			printf("ADC_CS read as low, when it should have been high. [SRAM-ADDRESS-PHASE]\n");
		}
		
		_delay_ms(1);
	}
	printf("Done running through SRAM addresses!\n");
}

int main(void)
{
	init_UART();
	init_XMEM();
	
	DDRA |= (1<<PA0);

	fdevopen(&UART_tx_polling, &UART_rx_polling);	
//	fdevopen(&UART_tx_polling(char, stdout *), &UART_rx_polling(stdin *));
	
	sei();
	
	
	uint8_t number = 0;
	
	test_XMEM_addressing();
	
    while (1) 
    {	
    }
}

