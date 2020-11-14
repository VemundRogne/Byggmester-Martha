/*
 * delay.c 
 *
 * This source file was taken from forum discussion on the topic of 
 * delays in the atsam3x8e
 * Link: https://community.atmel.com/forum/samd21-delay-functions?fbclid=IwAR0GK1Z7JAE3Zi1POwpBCI7li3vHm5ci7iDNOtG_i-7EFT5g5IYlZX1GVqc
 * Author: 12oclocker
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