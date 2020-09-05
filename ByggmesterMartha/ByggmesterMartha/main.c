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
#include "inc/xmem.h"
#include "inc/uart.h"

ISR(USART0_RXC_vect){
	cli();
	//uint8_t n = UART_rx_polling();
	
	for (uint8_t i=0; i<5; i++){
		PORTA |= (1<<PA0);
		_delay_ms(200);
		PORTA &= ~(1<<PA0);
		_delay_ms(500);
	}
	sei();
}

int main(void)
{
	init_UART();
	init_XMEM();
	
	DDRA |= (1<<PA0);

	fdevopen(&UART_tx_polling, &UART_rx_polling);	
//	fdevopen(&UART_tx_polling(char, stdout *), &UART_rx_polling(stdin *));
	
	sei();
	
	uint8_t number = 0;
	
	test_XMEM();
	
    while (1) 
    {
		
	
		
    }
}

