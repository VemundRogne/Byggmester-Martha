/*
 * ByggmesterMartha.c
 *
 * Created: 26/08/2020 09:09:34
 * Author : Vemund
 */ 

#include <avr/io.h>

#define F_CPU	4915200
#define BAUD	9600
#define MYUBRR	F_CPU/16/BAUD-1

#include <util/delay.h>

void init_UART(){
	// Set Baudrate
	UBRR0L = MYUBRR;

	// Enable RX and TX	
	UCSR0B = (1<<RXEN0) | (1<<TXEN0);

	// Set frame format: 8data, 2stop bit	
	UCSR0C = (1<<URSEL0) | (1<<USBS0) | (3<<UCSZ00);
}

// receives one byte in polling mode
uint8_t UART_rx_polling(){
	// Wait for data to be received
	while ( !(UCSR0A & (1<<RXC0) ));
	
	// Get data from UART and return
	return UDR0;
}

// transmits one byte in polling mode
void UART_tx_polling(uint8_t data){
	// Wait for empty transmit buffer
	while ( !(UCSR0A & (1<<UDRE0) ));
	
	// Put data into buffer, sends the data
	UDR0 = data;
}

int main(void)
{
	init_UART();
	
	DDRA |= (1<<PA0);
	
    while (1) 
    {
		uint8_t n = UART_rx_polling();
		UART_tx_polling(n);
		
		for (uint8_t i=0; i<n; i++){
				PORTA |= (1<<PA0);
				_delay_ms(200);
				PORTA &= ~(1<<PA0);
				_delay_ms(500);
		}
    }
}

