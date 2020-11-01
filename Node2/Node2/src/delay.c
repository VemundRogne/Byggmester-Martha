/*
 * delay.c
 *
 * Created: 01/11/2020 15:09:57
 *  Author: vemun
 */ 

#include "../inc/delay.h"

RAMFUNC
void portable_delay_cycles(uint32_t n)
{
	__asm (
	"loop: DMB	\n"
	#ifdef __ICCARM__
	"SUBS r0, r0, #1 \n"
	#else
	"SUB r0, r0, #1 \n"
	#endif
	"CMP r0, #0  \n"
	"BNE loop         "
	);
}