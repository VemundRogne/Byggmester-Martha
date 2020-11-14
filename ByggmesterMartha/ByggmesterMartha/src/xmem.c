/**
* @file
* @brief Implementation of the external memory library
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
