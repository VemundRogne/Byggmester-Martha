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
#include "../inc/mmi.h"
#include "../inc/oled.h"
#include "../inc/menu.h"
#include "../inc/timers.h"
#include "../inc/statemachine.h"
#include "../inc/mcp2515.h"
#include "../inc/can.h"

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

	mcp2515_init(LOOPBACK);

	uint8_t data[3] = {0, 1, 2};	
	printf("Starting main:\n");
    while (1) 
    {
		uint8_t MCP_status_register = mcp2515_READ_STATUS();
		printf(" MCP_status_register %u\n", MCP_status_register);
		uint8_t MCP_can_control_register = 0;
		mcp2515_READ(MCP_CANCTRL, &MCP_can_control_register, 1);
		printf(" MCP_can_control_register %u\n", MCP_can_control_register);
		
		struct can_msg msg;
		msg.ID = 200;
		printf("     SETTING ID = %u\n", msg.ID);
		msg.len = 6;
		uint8_t can_data[6] = {1, 2, 3, 4, 5, 6};
		memcpy(&msg.data[0], &can_data[0], 8);

		printf("Writing TX buffer...\n");
		can_transmit_message(msg);
		printf("Reading TX buffer: ");
		for(uint8_t i=0; i<13; i++){
			uint8_t contents = 0;
			mcp2515_READ(MCP_TXB0CTRL + 1 + i, &contents, 1);
			printf("%u, ", contents);
		}
		printf("\n");
		
		mcp2515_RTS(1);
		
		struct can_msg read_msg;
		can_receive_message(&read_msg);
		printf("    READ ID: %u\n", read_msg.ID);
		printf("    READ LEN: %u\n", read_msg.len);
		printf("    READ DATA: ");
		for(uint8_t i=0; i<read_msg.len; i++){
			printf("%u, ", read_msg.data[i]);
		}
		printf("\n");

		_delay_ms(2000);
    }
}