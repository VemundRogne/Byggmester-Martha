/*
 * xmem.c
 *
 * Created: 02.09.2020 09:59:38
 *  Author: marth
 */ 

#include <avr/io.h>

#include "../inc/xmem.h"

void xmem_init(){
	//Masking out JTAG
	SFIOR |= (1 << XMM2);
	
	MCUCR |= (1<<SRW11);
	
	//Enable XMEM
	MCUCR |= (1 << SRE);
}
