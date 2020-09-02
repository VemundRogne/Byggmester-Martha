/*
 * xmem.c
 *
 * Created: 02.09.2020 09:59:38
 *  Author: marth
 */ 

#include <avr/io.h>
#define F_CPU	4915200
#include <util/delay.h>

#include "../inc/xmem.h"

void init_XMEM(){
	//Masking out JTAG
	SFIOR |= (1 << XMM2);
	//Enable XMEM
	MCUCR |= (1 << SRE);
}


// Verifies that we have correct addressing through inputs
void test_XMEM(){
	// ADC-Chip select: PD3
	// SRAM-Chip select: PD4
	// OLED-Chip: PD5
	
	
	
	volatile char *ext_ram = (char*) 0x0500;
	
	for (uint16_t i = 0; i < 255; i ++){
		ext_ram[i] = 0xff;
		_delay_ms(1000);
	}
}

