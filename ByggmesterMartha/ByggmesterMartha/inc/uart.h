/*
 * uart.h
 *
 * Created: 05.09.2020 13:59:36
 *  Author: marth
 */ 


#ifndef UART_H_
#define UART_H_

void init_UART();

uint8_t UART_rx_polling();

void UART_tx_polling(uint8_t data);

ISR(USART0_RXC_vect);

#endif /* UART_H_ */