/*
 * delay.h
 *
 * Created: 01/11/2020 15:00:19
 *  Author: vemun
 */ 

#ifndef DELAY_H_
#define DELAY_H_

#include <sam.h>
#define F_CPU 36000000

#define RAMFUNC __attribute__ ((section(".ramfunc")))
//#define OPTIMIZE_HIGH __attribute__((optimize(s)))

#define cpu_ms_2_cy(ms, f_cpu)  (((uint64_t)(ms) * (f_cpu) + (uint64_t)(7e3-1ul)) / (uint64_t)7e3)
#define cpu_us_2_cy(us, f_cpu)  (((uint64_t)(us) * (f_cpu) + (uint64_t)(7e6-1ul)) / (uint64_t)7e6)
#define delay_cycles            portable_delay_cycles
#define cpu_delay_s(delay)      delay_cycles(cpu_ms_2_cy(1000 * delay, F_CPU))
#define cpu_delay_ms(delay)     delay_cycles(cpu_ms_2_cy(delay, F_CPU))
#define cpu_delay_us(delay)     delay_cycles(cpu_us_2_cy(delay, F_CPU))
#define _delay_us	cpu_delay_us
#define _delay_ms	cpu_delay_ms
#define _delay_s	cpu_delay_s

// Delay loop is put to SRAM so that FWS will not affect delay time
//OPTIMIZE_HIGH

void portable_delay_cycles(uint32_t n);

#endif /* DELAY_H_ */