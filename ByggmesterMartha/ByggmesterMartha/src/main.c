/**
* @file
* @brief Main file for node 1
*/


#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <string.h>

#define F_CPU	4915200
#define BAUD	9600
#define MYUBRR	F_CPU/16/BAUD-1

#include <util/delay.h>
#include "../inc/xmem.h"
#include "../inc/uart.h"
#include "../inc/adc.h"
#include "../inc/funkyboard.h"
#include "../inc/oled.h"
#include "../inc/menu.h"
#include "../inc/timers.h"
#include "../inc/statemachine.h"
#include "../inc/mcp2515.h"
#include "../inc/can.h"
#include "../inc/game.h"
#include <stdlib.h>


/**
* @brief Initializes everything on node 1, then oops game_sequence
*
* @return int 0
*/

int main(void)
{
	sei();

	uart_init();
	xmem_init();
	adc_init();
	oled_init();
	menu_init();
	timer0_init();
	statemachine_init();	
	game_init();
	joystick_init();
	mcp2515_init(NORMAL);

    while (1){
		game_sequence();
		_delay_ms(50);
	}
	return (0);
}