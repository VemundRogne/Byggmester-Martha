/*
 * uart.h
 *
 * Created: 05.09.2020 13:59:36
 *  Author: marth
 */ 

#ifndef UART_H_
#define UART_H_

// This is the CMD receive buffer, when it is filled we know we have a full CMD
#define CMD_LEN 10
volatile uint8_t cmd_buffer[CMD_LEN];
volatile uint8_t cmd_receive_counter;

void init_UART();

uint8_t UART_rx_polling();

void UART_tx_polling(uint8_t data);

ISR(USART0_RXC_vect);

void UART_execute_cmd();

#endif /* UART_H_ */