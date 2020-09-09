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
	
	MCUCR |= (1<<SRW11);
	
	//Enable XMEM
	MCUCR |= (1 << SRE);
}
