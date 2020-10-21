/*
 * timers.c
 *
 * Created: 21/10/2020 12:15:33
 *  Author: vemun
 */ 

#include <stdint.h>
#include "sam.h"
#include <component/pmc.h>
#include <component/tc.h>
#include <component/pio.h>

#include "../inc/timers.h"

#define MCK	48000000

/*
 * Configures timer0 to provide PWM signal suitable for a servo on PWM2
 * --------------------------------------------------------------------
 * Target frequency: 1/20ms = 50Hz
 * 
 * Target dutycycle:
 *   Neutral position: 1.5ms -> 1.5/20 =  7.5%
 *   Lower position:   0.9ms -> 0.9/20 =  4.5%
 *   Top position:     2.1ms -> 2.1/20 = 10.5%
 *
 * PIN configuration:
 *   Instance:		TC0
 *   Signal:		TIOA0
 *   I/O Line:		PB25
 *   Peripheral:	B
*/
void init_timer(){
	//REG_PIOB_OER |= (1<<25);
	
	PIOB->PIO_PDR |= (1<<25);
	//Give control of PB25 to peripheral B (TC0)
	PIOB->PIO_ABSR |= PIO_ABSR_P25;
	
	PMC->PMC_PCER0 |= PMC_PCDR0_PID27; 

	// Disable write protection
	//TC0->TC_WPMR = "TIM";

	// Clock: Internal MCK/32. Set to Wave mode
	TC0->TC_CHANNEL[0].TC_CMR |= TC_CMR_TCCLKS_TIMER_CLOCK3 | TC_CMR_WAVE | TC_CMR_WAVSEL_UP_RC;
	
	// Setup triggering on TIOA
	TC0->TC_CHANNEL[0].TC_CMR |= TC_CMR_ACPC_SET | TC_CMR_ACPA_CLEAR;
	
	TC0->TC_CHANNEL[0].TC_RA = 53332*0.105;
	TC0->TC_CHANNEL[0].TC_RC = 53332;
	
	TC0->TC_CHANNEL[0].TC_CCR |= TC_CCR_CLKEN | TC_CCR_SWTRG;
}
