/*
 * delay.h 
 * This code was retrieved from a forum discussion on the topic of
 * delays in the ATSAM3X8E micro controller
 * Link: https://community.atmel.com/forum/samd21-delay-functions?fbclid=IwAR0GK1Z7JAE3Zi1POwpBCI7li3vHm5ci7iDNOtG_i-7EFT5g5IYlZX1GVqc
 * Author: 12oclocker
 */ 

#ifndef DELAY_H_
#define DELAY_H_

#include <sam.h>
#define F_CPU 36000000

#define RAMFUNC __attribute__ ((section(".ramfunc")))

#define cpu_ms_2_cy(ms, f_cpu)  (((uint64_t)(ms) * (f_cpu) + (uint64_t)(7e3-1ul)) / (uint64_t)7e3)
#define cpu_us_2_cy(us, f_cpu)  (((uint64_t)(us) * (f_cpu) + (uint64_t)(7e6-1ul)) / (uint64_t)7e6)
#define delay_cycles            portable_delay_cycles
#define cpu_delay_s(delay)      delay_cycles(cpu_ms_2_cy(1000 * delay, F_CPU))
#define cpu_delay_ms(delay)     delay_cycles(cpu_ms_2_cy(delay, F_CPU))
#define cpu_delay_us(delay)     delay_cycles(cpu_us_2_cy(delay, F_CPU))
#define _delay_us	cpu_delay_us
#define _delay_ms	cpu_delay_ms
#define _delay_s	cpu_delay_s

void portable_delay_cycles(uint32_t n);

#endif /* DELAY_H_ */