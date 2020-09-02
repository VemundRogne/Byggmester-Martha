/*
 * xmem.c
 *
 * Created: 02.09.2020 09:59:38
 *  Author: marth
 */ 

#include <avr/io.h>

#include "../inc/xmem.h"

void init_XMEM(){
	//Masking out JTAG
	SFIOR |= (1 << XMM2);
	//Enable XMEM
	MCUCR |= (1 << SRE);
}

void test_XMEM(){
	volatile char *ext_ram = (char*) 0x0500;
	
	for (uint16_t i = 0; i < 255; i ++){
		ext_ram[i] = 0;
	}
}

