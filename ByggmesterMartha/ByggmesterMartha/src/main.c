/*
 * ByggmesterMartha.c
 *
 * Created: 26/08/2020 09:09:34
 * Author : Vemund
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

int main(void)
{
	init_UART();
	//fdevopen(&UART_tx_polling, &UART_rx_polling);
	//printf("Start!\n");
	init_XMEM();
	init_adc();
	oled_init();
	menu_init();
	init_timer0();
	init_statemachine();	
	game_init();
	
	sei();

	load_mux_config();
	init_joystick();
	
	menu_home();

	mcp2515_init(NORMAL);

	uint8_t rd_adc_values[4];
	uint8_t ir_status = 0;
	
	struct can_msg flag_msg;
	flag_msg.ID = 5;
	flag_msg.data[0] = 1;
	flag_msg.len = 1;
	
//	can_transmit_message(&flag_msg);
	
	uint8_t test = 0;

	//printf("Starting main:\n");
    while (1){
    	Joystick_can();
		send_button_press();
		//statemachine_execute_current_state();
		_delay_ms(50);
	}
	return (0);
}