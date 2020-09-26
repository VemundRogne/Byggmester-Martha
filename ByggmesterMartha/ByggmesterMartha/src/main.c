/*
 * ByggmesterMartha.c
 *
 * Created: 26/08/2020 09:09:34
 * Author : Vemund
 */ 


#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>

#define F_CPU	4915200
#define BAUD	9600
#define MYUBRR	F_CPU/16/BAUD-1

#include <util/delay.h>
#include "../inc/xmem.h"
#include "../inc/uart.h"
#include "../inc/adc.h"
#include "../inc/mmi.h"
#include "../inc/oled.h"
#include "../inc/menu.h"
#include "../inc/timers.h"
#include "../inc/statemachine.h"

#include <stdlib.h>

int main(void)
{
	init_UART();
	init_XMEM();
	init_adc();
	oled_init();
	menu_init();
	init_timer0();
	init_statemachine();
	
	fdevopen(&UART_tx_polling, &UART_rx_polling);	
	
	sei();

	load_mux_config();
	init_joystick();
	
	menu_home();
	
    while (1) 
    {
		statemachine_execute_current_state();
    }
}